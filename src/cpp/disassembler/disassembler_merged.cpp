// SPDX-License-Identifier: MIT
// Copyright (C) 2025-2026, Advanced Micro Devices, Inc. All rights reserved.

#include "disassembler/disassembler_merged.h"
#include "elf/aie_elf_constants.h"
#include "preprocessor/asm/hintmap_bitset.h"

#include <algorithm>
#include <cstring>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

namespace aiebu {
namespace {

constexpr size_t page_size = PAGE_SIZE;
constexpr size_t page_header_size = 16;
constexpr size_t page_header_magic_byte_0 = 0;
constexpr size_t page_header_magic_byte_1 = 1;
constexpr size_t page_header_page_idx_low = 2;
constexpr size_t page_header_page_idx_high = 3;
constexpr size_t page_header_in_order_len_low = 10;
constexpr uint8_t page_header_magic = 0xFF;
constexpr uint8_t eof_opcode = 0xFF;
constexpr uint8_t align_opcode = 0xA5;
constexpr uint8_t preempt_opcode = 0x19;
constexpr uint8_t load_pdi_opcode = 0x1a;
constexpr uint8_t load_cores_opcode = 0x04;
constexpr size_t eof_size = 4;
constexpr size_t byte_shift = 8;
constexpr size_t word_shift_16 = 16;
constexpr size_t word_shift_24 = 24;
constexpr size_t bits_per_byte = 8;
constexpr size_t min_insn_size = 2;
constexpr size_t bd_words_size = 12;
constexpr size_t bd_word1_offset = 4;
constexpr size_t bd_word2_offset = 8;
constexpr size_t preempt_insn_size = 8;
constexpr size_t preempt_id_offset = 2;
constexpr size_t preempt_save_page_offset = 4;
constexpr size_t preempt_restore_page_offset = 6;
constexpr size_t ooo_insn_size = 12;
constexpr size_t ooo_page_id_offset = 8;
constexpr size_t section_name_page_idx = 2;
constexpr size_t ctrltext_prefix_len = 9;

uint16_t
read_u16_le(const char* data)
{
  return static_cast<uint16_t>(static_cast<uint8_t>(data[0])) |
         (static_cast<uint16_t>(static_cast<uint8_t>(data[1])) << byte_shift);
}

uint32_t
read_u32_le(const char* data)
{
  return static_cast<uint32_t>(static_cast<uint8_t>(data[0])) |
         (static_cast<uint32_t>(static_cast<uint8_t>(data[1])) << byte_shift) |
         (static_cast<uint32_t>(static_cast<uint8_t>(data[2])) << word_shift_16) |
         (static_cast<uint32_t>(static_cast<uint8_t>(data[3])) << word_shift_24);
}

size_t
instruction_size(uint8_t opcode, const std::map<uint8_t, isa_op_disasm>* isa_map)
{
  auto it = isa_map->find(opcode);
  if (it == isa_map->end())
    return 0;

  size_t size = min_insn_size;
  for (const auto& arg : it->second.get_args())
    size += static_cast<size_t>(arg.get_width()) / bits_per_byte;
  return size;
}

struct scratch_bd {
  int col = -1;
  uint16_t page_idx = 0;
  uint64_t addr = 0;
  uint64_t len_bytes = 0;
};

std::vector<scratch_bd>
collect_scratchpad_bds(ELFIO::elfio& elf, bool merged_format)
{
  std::vector<scratch_bd> out;

  auto* dynsym_sec = elf.sections[".dynsym"];
  auto* dynstr_sec = elf.sections[".dynstr"];
  auto* rela_sec = elf.sections[".rela.dyn"];
  if (!dynsym_sec || !dynstr_sec || !rela_sec)
    return out;

  std::set<uint32_t> scratch_sym_indices;
  const auto* dynsym_data = dynsym_sec->get_data();
  const auto* dynstr_data = dynstr_sec->get_data();
  const size_t sym_entsz = sizeof(ELFIO::Elf32_Sym);
  for (size_t off = 0; off + sym_entsz <= dynsym_sec->get_size(); off += sym_entsz) {
    const auto* sym = reinterpret_cast<const ELFIO::Elf32_Sym*>(dynsym_data + off);
    if (sym->st_name >= dynstr_sec->get_size())
      continue;
    const char* name = dynstr_data + sym->st_name;
    if (std::string(name) == "scratch-pad-mem")
      scratch_sym_indices.insert(static_cast<uint32_t>(off / sym_entsz));
  }

  std::map<ELFIO::Elf_Half, const ELFIO::section*> sec_by_idx;
  for (ELFIO::Elf_Half i = 0; i < elf.sections.size(); ++i)
    sec_by_idx[i] = elf.sections[i];

  const auto* rela_begin = reinterpret_cast<const ELFIO::Elf32_Rela*>(rela_sec->get_data());
  const auto* rela_end = rela_begin + rela_sec->get_size() / sizeof(ELFIO::Elf32_Rela);

  for (const auto* rela = rela_begin; rela != rela_end; ++rela) {
    const uint32_t symidx = ELFIO::get_sym_and_type<ELFIO::Elf32_Rela>::get_r_sym(rela->r_info);
    if (!scratch_sym_indices.count(symidx))
      continue;

    const auto dsym_off = symidx * sizeof(ELFIO::Elf32_Sym);
    if (dsym_off + sizeof(ELFIO::Elf32_Sym) > dynsym_sec->get_size())
      continue;
    const auto* sym = reinterpret_cast<const ELFIO::Elf32_Sym*>(dynsym_data + dsym_off);
    const auto* ct_sec = sec_by_idx[sym->st_shndx];
    if (!ct_sec)
      continue;

    const std::string& sec_name = ct_sec->get_name();
    if (sec_name.size() < ctrltext_prefix_len ||
        sec_name.substr(0, ctrltext_prefix_len) != ".ctrltext")
      continue;

    const int col = merged_disasm_context::parse_column(sec_name);
    if (col < 0)
      continue;

    const ELFIO::section* data_sec = ct_sec;
    uint64_t bd_offset = 0;

    const uint64_t offset = rela->r_offset;
    if (merged_format) {
      bd_offset = offset + page_header_size;
    } else {
      const auto* paired = sec_by_idx[sym->st_shndx + 1];
      if (!paired)
        continue;
      data_sec = paired;
      bd_offset = offset - ct_sec->get_size() + page_header_size;
    }

    if (bd_offset + bd_words_size > data_sec->get_size())
      continue;

    const char* sec_data = data_sec->get_data();
    const uint32_t w0 = read_u32_le(sec_data + bd_offset);
    const uint32_t w1 = read_u32_le(sec_data + bd_offset + bd_word1_offset);
    const uint32_t w2 = read_u32_le(sec_data + bd_offset + bd_word2_offset);
    const uint64_t addr = ((static_cast<uint64_t>(w0) & 0x1FFFFFFULL) << 32) | w1;
    const uint64_t len_bytes = static_cast<uint64_t>(w2) * 4;

    uint16_t page_idx = 0;
    if (merged_format) {
      const size_t page_base = (offset / page_size) * page_size;
      if (page_base + page_header_page_idx_high < ct_sec->get_size())
        page_idx = read_u16_le(ct_sec->get_data() + page_base + page_header_page_idx_low);
      else
        page_idx = static_cast<uint16_t>(page_base / page_size);
    } else {
      std::vector<int> nums;
      size_t start = 0;
      while (start < sec_name.size()) {
        const size_t dot = sec_name.find('.', start);
        const std::string part = (dot == std::string::npos)
            ? sec_name.substr(start)
            : sec_name.substr(start, dot - start);
        try {
          nums.push_back(std::stoi(part));
        } catch (...) {
        }
        if (dot == std::string::npos)
          break;
        start = dot + 1;
      }
      page_idx = (nums.size() >= section_name_page_idx)
          ? static_cast<uint16_t>(nums[section_name_page_idx - 1])
          : 0;
    }

    out.push_back({col, page_idx, addr, len_bytes});
  }

  return out;
}

hintmap_chunk_bits
bd_list_to_hintmap_bits(const std::vector<scratch_bd>& bds)
{
  hintmap_chunk_bits bm;
  for (const auto& bd : bds) {
    if (bd.len_bytes == 0)
      continue;
    const uint64_t first_chunk = bd.addr / CHUNK_SIZE;
    const uint64_t last_addr = bd.addr + bd.len_bytes - 1;
    const uint64_t last_chunk = last_addr / CHUNK_SIZE;
    for (uint64_t chunk = first_chunk; chunk <= last_chunk && chunk < HINTMAP_CHUNK_BITS; ++chunk)
      bm.set(static_cast<std::size_t>(chunk));
  }
  return bm;
}

std::vector<uint32_t>
bitset_to_hintmap_words(const hintmap_chunk_bits& bm)
{
  std::vector<uint32_t> words(HINTMAP_WORD_COUNT, 0);
  for (std::size_t w = 0; w < HINTMAP_WORD_COUNT; ++w) {
    uint32_t val = 0;
    for (std::size_t bit = 0; bit < HINTMAP_WORD_BITS; ++bit) {
      if (bm.test(w * HINTMAP_WORD_BITS + bit))
        val |= (1U << bit);
    }
    if (val != 0)
      words[w] = val;
  }

  while (!words.empty() && words.back() == 0)
    words.pop_back();
  return words;
}

struct page_scan_record {
  int col = -1;
  uint16_t page_idx = 0;
  uint32_t text_offset = 0;
  std::string op_name;
  uint16_t page_ref = 0;
  uint16_t preempt_id = 0;
  uint16_t save_page = 0;
  uint16_t restore_page = 0;
};

using page_span_key = std::pair<int, uint16_t>;
using page_span_map = std::map<page_span_key, std::vector<uint16_t>>;

uint16_t
read_page_index_local(const char* page_data)
{
  return read_u16_le(page_data + page_header_page_idx_low);
}

uint16_t
read_in_order_page_len_local(const char* page_data)
{
  if (static_cast<uint8_t>(page_data[page_header_magic_byte_0]) != page_header_magic ||
      static_cast<uint8_t>(page_data[page_header_magic_byte_1]) != page_header_magic)
    return 0;
  return read_u16_le(page_data + page_header_in_order_len_low);
}

bool
page_header_valid(const char* page_data)
{
  return static_cast<uint8_t>(page_data[page_header_magic_byte_0]) == page_header_magic &&
         static_cast<uint8_t>(page_data[page_header_magic_byte_1]) == page_header_magic;
}

// Walk merged ctrltext and record every in-order page chain keyed by its first page_idx.
void
build_in_order_page_spans(const ELFIO::section* section, int col, page_span_map& spans)
{
  const char* section_data = section->get_data();
  const size_t section_size = section->get_size();

  for (size_t page_start = 0; page_start < section_size; page_start += page_size) {
    if (section_size - page_start < page_header_size)
      break;

    const char* page_data = section_data + page_start;
    if (!page_header_valid(page_data))
      break;

    bool is_continuation = false;
    if (page_start >= page_size) {
      const char* prev_page = section_data + page_start - page_size;
      if (page_header_valid(prev_page) && read_in_order_page_len_local(prev_page) != 0)
        is_continuation = true;
    }
    if (is_continuation)
      continue;

    std::vector<uint16_t> chain;
    for (size_t slot = page_start; slot < section_size; slot += page_size) {
      if (section_size - slot < page_header_size)
        break;

      const char* slot_data = section_data + slot;
      if (!page_header_valid(slot_data))
        break;

      chain.push_back(read_page_index_local(slot_data));
      if (read_in_order_page_len_local(slot_data) == 0)
        break;
    }

    if (!chain.empty())
      spans[{col, chain.front()}] = std::move(chain);
  }
}

std::vector<uint16_t>
page_span_for(const page_span_map& spans, int col, uint16_t page_idx)
{
  const auto it = spans.find({col, page_idx});
  if (it != spans.end())
    return it->second;
  return {page_idx};
}

std::vector<scratch_bd>
collect_bds_for_page_span(
    const std::map<std::pair<int, uint16_t>, std::vector<scratch_bd>>& bds_by_page,
    int col, const std::vector<uint16_t>& span)
{
  std::vector<scratch_bd> out;
  for (const uint16_t page_idx : span) {
    const auto it = bds_by_page.find({col, page_idx});
    if (it == bds_by_page.end())
      continue;
    out.insert(out.end(), it->second.begin(), it->second.end());
  }
  return out;
}

void
add_page_span_to_skip_set(const page_span_map& spans, int col, uint16_t page_idx,
                          std::set<merged_disasm_context::page_loc>& skip_pages)
{
  for (const uint16_t idx : page_span_for(spans, col, page_idx))
    skip_pages.insert({col, idx});
}


} // namespace

int
merged_disasm_context::parse_column(const std::string& section_name)
{
  size_t first_dot = section_name.find('.', 1);
  if (first_dot == std::string::npos)
    return -1;
  size_t second_dot = section_name.find('.', first_dot + 1);
  if (second_dot == std::string::npos)
    return -1;
  try {
    return std::stoi(section_name.substr(first_dot + 1, second_dot - first_dot - 1));
  } catch (...) {
    return -1;
  }
}

uint16_t
merged_disasm_context::read_page_index(const char* page_data)
{
  return read_u16_le(page_data + page_header_page_idx_low);
}

uint16_t
merged_disasm_context::read_in_order_page_len(const char* page_data)
{
  if (static_cast<uint8_t>(page_data[page_header_magic_byte_0]) != page_header_magic ||
      static_cast<uint8_t>(page_data[page_header_magic_byte_1]) != page_header_magic)
    return 0;
  return read_u16_le(page_data + page_header_in_order_len_low);
}

size_t
merged_disasm_context::find_code_end(const char* page_data, size_t page_data_size,
                                     const std::map<uint8_t, isa_op_disasm>* isa_map)
{
  const size_t code_start = page_header_size;
  const size_t limit = std::min(page_data_size, page_size);

  for (size_t i = code_start; i < limit;) {
    const auto op = static_cast<uint8_t>(page_data[i]);
    if (op == eof_opcode)
      return i + eof_size;
    if (op == align_opcode) {
      ++i;
      continue;
    }
    const size_t insn_size = instruction_size(op, isa_map);
    if (insn_size < min_insn_size)
      break;
    i += insn_size;
  }
  return limit;
}

merged_disasm_context
merged_disasm_context::build(ELFIO::elfio& elf,
                             const std::map<uint8_t, isa_op_disasm>* isa_map,
                             unsigned char abi_version)
{
  merged_disasm_context ctx;
  if (abi_version < elf_version_config)
    return ctx;

  ctx.m_active = true;

  std::set<int> columns;
  page_span_map page_spans;
  std::vector<page_scan_record> ooo_refs;
  std::vector<std::tuple<int, uint16_t, uint32_t, preempt_point>> preempts;

  for (const auto& section_ptr : elf.sections) {
    const ELFIO::section* section = section_ptr.get();
    if (section->get_type() != ELFIO::SHT_PROGBITS)
      continue;

    const std::string& name = section->get_name();
    if (name.size() < ctrltext_prefix_len || name.substr(0, ctrltext_prefix_len) != ".ctrltext")
      continue;

    const int col = parse_column(name);
    if (col < 0)
      continue;
    columns.insert(col);
    build_in_order_page_spans(section, col, page_spans);

    const char* section_data = section->get_data();
    const size_t section_size = section->get_size();

    for (size_t page_start = 0; page_start < section_size; page_start += page_size) {
      const size_t avail = section_size - page_start;
      if (avail < page_header_size)
        break;

      const char* page_data = section_data + page_start;
      if (static_cast<uint8_t>(page_data[page_header_magic_byte_0]) != page_header_magic ||
          static_cast<uint8_t>(page_data[page_header_magic_byte_1]) != page_header_magic)
        break;

      const uint16_t page_idx = read_page_index(page_data);
      const size_t code_end = find_code_end(page_data, avail, isa_map);

      for (size_t pos = page_header_size; pos < code_end;) {
        const auto op = static_cast<uint8_t>(page_data[pos]);
        if (op == align_opcode) {
          ++pos;
          continue;
        }

        const size_t insn_size = instruction_size(op, isa_map);
        if (insn_size < min_insn_size || pos + insn_size > code_end)
          break;

        if (op == preempt_opcode && pos + preempt_insn_size <= code_end) {
          preempt_point pt;
          pt.id = read_u16_le(page_data + pos + preempt_id_offset);
          pt.save_page = read_u16_le(page_data + pos + preempt_save_page_offset);
          pt.restore_page = read_u16_le(page_data + pos + preempt_restore_page_offset);
          preempts.emplace_back(col, page_idx, static_cast<uint32_t>(pos - page_header_size), pt);
        } else if (op == load_pdi_opcode && pos + ooo_insn_size <= code_end) {
          page_scan_record rec;
          rec.col = col;
          rec.page_idx = page_idx;
          rec.text_offset = static_cast<uint32_t>(pos - page_header_size);
          rec.op_name = "load_pdi";
          // Layout: opcode(1)+pad(1)+pad(2)+pdi_id(4)+page_id(2)+pad(2)
          rec.page_ref = read_u16_le(page_data + pos + ooo_page_id_offset);
          ooo_refs.push_back(rec);
        } else if (op == load_cores_opcode && pos + ooo_insn_size <= code_end) {
          page_scan_record rec;
          rec.col = col;
          rec.page_idx = page_idx;
          rec.text_offset = static_cast<uint32_t>(pos - page_header_size);
          rec.op_name = "load_cores";
          rec.page_ref = read_u16_le(page_data + pos + ooo_page_id_offset);
          ooo_refs.push_back(rec);
        }

        pos += insn_size;
      }
    }
  }

  ctx.m_num_columns = static_cast<int>(columns.size());

  // Mark save/restore pages (including in-order continuations) to skip during disassembly.
  for (const auto& [col, page_idx, offset, pt] : preempts) {
    (void)page_idx;
    (void)offset;
    add_page_span_to_skip_set(page_spans, col, pt.save_page, ctx.m_skip_pages);
    add_page_span_to_skip_set(page_spans, col, pt.restore_page, ctx.m_skip_pages);
  }

  // Assign OOO page labels from load_pdi / load_cores references.
  std::map<int, unsigned int> load_pdi_count;
  std::map<int, unsigned int> load_cores_count;
  for (const auto& rec : ooo_refs) {
    page_loc target{rec.col, rec.page_ref};
    if (ctx.m_page_labels.count(target))
      continue;

    if (rec.op_name == "load_pdi") {
      const unsigned int idx = load_pdi_count[rec.col]++;
      ctx.m_page_labels[target] = "pdi" + std::to_string(idx);
    } else if (rec.op_name == "load_cores") {
      const unsigned int idx = load_cores_count[rec.col]++;
      ctx.m_page_labels[target] = "cores" + std::to_string(idx);
    }
  }

  // Assign save/restore labels; group by (col, save_page, restore_page).
  std::map<std::tuple<int, uint16_t, uint16_t>, unsigned int> sr_group_idx;
  std::map<std::tuple<int, uint16_t, uint16_t>, std::pair<std::string, std::string>> sr_labels;

  for (auto& [col, page_idx, offset, pt] : preempts) {
    (void)page_idx;
    const auto key = std::make_tuple(col, pt.save_page, pt.restore_page);
    if (!sr_labels.count(key)) {
      unsigned int idx = sr_group_idx[key]++;
      std::string save_name;
      std::string restore_name;
      if (idx == 0) {
        save_name = "save";
        restore_name = "restore";
      } else {
        save_name = "save_" + std::to_string(col) + "_" + std::to_string(idx);
        restore_name = "restore_" + std::to_string(col) + "_" + std::to_string(idx);
      }
      sr_labels[key] = {save_name, restore_name};
      ctx.m_page_labels[{col, pt.save_page}] = save_name;
      ctx.m_page_labels[{col, pt.restore_page}] = restore_name;
    }
    pt.save_label = sr_labels[key].first;
    pt.restore_label = sr_labels[key].second;
  }

  // Compute hintmaps from scratch-pad-mem BDs across the save page's in-order span.
  const auto all_bds = collect_scratchpad_bds(elf, true);
  std::map<std::pair<int, uint16_t>, std::vector<scratch_bd>> bds_by_page;
  for (const auto& bd : all_bds)
    bds_by_page[{bd.col, bd.page_idx}].push_back(bd);

  std::map<int, unsigned int> hintmap_counter;
  for (auto& [col, page_idx, offset, pt] : preempts) {
    (void)page_idx;
    const auto& save_span = page_span_for(page_spans, col, pt.save_page);
    const auto save_bds = collect_bds_for_page_span(bds_by_page, col, save_span);
    if (!save_bds.empty()) {
      const auto bm = bd_list_to_hintmap_bits(save_bds);
      if (!bm.none())
        pt.hintmap_words = bitset_to_hintmap_words(bm);
    }

    // NOP save/restore (no scratch-pad BDs on the save page) uses an all-zero hintmap.
    if (pt.hintmap_words.empty())
      pt.hintmap_words.push_back(0);

    pt.has_hintmap = true;
    pt.hintmap_label = "hintmap_" + std::to_string(hintmap_counter[col]++);

    const preempt_key key{col, page_idx, offset};
    ctx.m_preempt_points[key] = pt;
  }

  return ctx;
}

bool
merged_disasm_context::should_skip_page(int col, uint16_t page_idx) const
{
  return m_skip_pages.count({col, page_idx}) > 0;
}

std::string
merged_disasm_context::page_label(int col, uint16_t page_idx) const
{
  const auto it = m_page_labels.find({col, page_idx});
  return (it != m_page_labels.end()) ? it->second : std::string();
}

std::string
merged_disasm_context::pending_page_label(int col, uint16_t page_idx) const
{
  return page_label(col, page_idx);
}

const merged_disasm_context::preempt_point*
merged_disasm_context::preempt_at(int col, uint16_t page_idx, uint32_t text_offset) const
{
  const preempt_key key{col, page_idx, text_offset};
  const auto it = m_preempt_points.find(key);
  return (it != m_preempt_points.end()) ? &it->second : nullptr;
}

} // namespace aiebu
