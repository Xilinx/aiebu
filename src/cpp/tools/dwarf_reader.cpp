// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

// DWARF v5 .debug_* section parser for AIEBU merged-format ELFs.
//
// Only the subset of DWARF v5 produced by dwarf_writer.cpp is handled:
//   - 32-bit DWARF format, address_size=4
//   - Fixed abbreviation codes 1 (compile_unit), 2 (module), 3 (label)
//   - DW_FORM_strp (4-byte offset into .debug_str)
//   - DW_FORM_sec_offset (4-byte)
//   - DW_FORM_data2, DW_FORM_data4, DW_FORM_addr (4-byte)
//   - No DW_LNE_AIE_set_operation extension (mnemonic path not read here)

#include "tools/dwarf_reader.h"
#include "common/dwarf_constants.h"

#include <cstring>
#include <boost/endian/conversion.hpp>
#include <elfio/elfio.hpp>

namespace aiebu {

using namespace dwarf5;

// ─── Raw-read helpers ────────────────────────────────────────────────────────

const uint8_t* dwarf_reader::read_u8(const uint8_t* p, uint8_t& v)
{
  v = *p;
  return p + 1;
}

const uint8_t* dwarf_reader::read_u16(const uint8_t* p, uint16_t& v)
{
  v = boost::endian::load_little_u16(p);
  return p + 2;
}

const uint8_t* dwarf_reader::read_u32(const uint8_t* p, uint32_t& v)
{
  v = boost::endian::load_little_u32(p);
  return p + 4;
}

const uint8_t* dwarf_reader::read_uleb128(const uint8_t* p, const uint8_t* end, uint64_t& v)
{
  v = 0;
  unsigned shift = 0;
  while (p < end) {
    uint8_t byte = *p++;
    v |= (static_cast<uint64_t>(byte & 0x7F) << shift);
    shift += 7;
    if ((byte & 0x80) == 0) break;
  }
  return p;
}

const uint8_t* dwarf_reader::read_sleb128(const uint8_t* p, const uint8_t* end, int64_t& v)
{
  v = 0;
  unsigned shift = 0;
  uint8_t byte = 0;
  while (p < end) {
    byte = *p++;
    v |= (static_cast<int64_t>(byte & 0x7F) << shift);
    shift += 7;
    if ((byte & 0x80) == 0) break;
  }
  // Sign extend
  if (shift < 64 && (byte & 0x40))
    v |= -(static_cast<int64_t>(1) << shift);
  return p;
}

const uint8_t* dwarf_reader::read_cstr(const uint8_t* p, const uint8_t* end, std::string& s)
{
  const uint8_t* nul = static_cast<const uint8_t*>(
      std::memchr(p, 0, static_cast<size_t>(end - p)));
  if (!nul) nul = end;
  s.assign(reinterpret_cast<const char*>(p), nul - p);
  return (nul < end) ? nul + 1 : end;
}

std::string dwarf_reader::read_strp(const uint8_t* p, const uint8_t* str_data, size_t str_size)
{
  uint32_t offset = 0;
  read_u32(p, offset);
  if (!str_data || offset >= str_size) return {};
  const char* cstr = reinterpret_cast<const char*>(str_data + offset);
  // Find NUL within bounds
  size_t max_len = str_size - offset;
  size_t len = strnlen(cstr, max_len);
  return {cstr, len};
}

// ─── Section lookup ──────────────────────────────────────────────────────────

std::pair<const uint8_t*, size_t>
dwarf_reader::get_section_data(const ELFIO::elfio& elf,
                                const std::string& name,
                                const std::string& suffix)
{
  // Try exact name first, then name+suffix
  for (const auto& s : {name + suffix, name}) {
    const ELFIO::section* sec = elf.sections[s];
    if (sec && sec->get_data() && sec->get_size() > 0)
      return {reinterpret_cast<const uint8_t*>(sec->get_data()), sec->get_size()};
  }
  return {nullptr, 0};
}

// ─── .debug_info parser ──────────────────────────────────────────────────────

// We parse the fixed abbreviation table inline rather than reading the actual
// .debug_abbrev bytes, since we know exactly what dwarf_writer.cpp emits.
// The attribute-value skip logic handles the general case using form sizes.

// Returns the byte size to skip for a given form in our fixed schema.
// Returns 0 for variable-length forms that must be handled by the caller.
static size_t form_fixed_size(uint64_t form)
{
  switch (form) {
    case DW_FORM_addr:       return 4;
    case DW_FORM_strp:       return 4;
    case DW_FORM_data2:      return 2;
    case DW_FORM_data4:      return 4;
    case DW_FORM_sec_offset: return 4;
    default:                 return 0; // unknown / variable
  }
}

void
dwarf_reader::parse_debug_info(
    const uint8_t* info_data, size_t info_size,
    const uint8_t* str_data,  size_t str_size,
    std::vector<col_info>& cols_out)
{
  const uint8_t* p   = info_data;
  const uint8_t* end = info_data + info_size;

  if (p + 11 > end) return; // Too small for a v5 CU header

  // Parse DWARF v5 CU header
  uint32_t unit_length = 0;
  p = read_u32(p, unit_length);
  const uint8_t* cu_end = p + unit_length;
  if (cu_end > end) return;

  uint16_t version = 0;
  p = read_u16(p, version);
  if (version != DWARF_VERSION) return;

  uint8_t unit_type = 0;
  p = read_u8(p, unit_type);
  uint8_t addr_size = 0;
  p = read_u8(p, addr_size);
  uint32_t abbrev_offset = 0;
  p = read_u32(p, abbrev_offset);

  if (addr_size != DWARF_ADDR_SIZE) return; // Only support 32-bit addresses

  // We now parse DIEs using our fixed abbreviation knowledge:
  //   abbrev 1 = compile_unit (has_children): producer(strp), language(data2), name(strp)
  //   abbrev 2 = module       (has_children): name(strp), stmt_list(sec_offset), byte_size(data4)
  //   abbrev 3 = label        (no_children):  low_pc(addr), const_value(strp), name(strp), description(strp)

  // Current DW_TAG_module state
  uint32_t cur_col  = UINT32_MAX;
  uint32_t cur_stmt = 0;

  // Annotation lookup: col → list of (low_pc, ann_id, ann_name, ann_desc)
  // We build these as rows into m_rows with line=0.
  // After line table parsing, the main rows get their line/file.
  struct pending_label {
    uint32_t col    = UINT32_MAX;
    uint32_t low_pc = 0;
    std::string ann_id;
    std::string ann_name;
    std::string ann_desc;
  };
  std::vector<pending_label> pending_labels;

  while (p < cu_end) {
    uint64_t abbrev_code = 0;
    p = read_uleb128(p, cu_end, abbrev_code);
    if (abbrev_code == 0) {
      // null DIE — ends children list
      // If we were in a module, record it
      if (cur_col != UINT32_MAX) {
        col_info ci;
        ci.col_num     = cur_col;
        ci.stmt_offset = cur_stmt;
        cols_out.push_back(ci);
        cur_col = UINT32_MAX;
      }
      continue;
    }

    switch (abbrev_code) {
      case 1: { // compile_unit
        // producer(strp) language(data2) name(strp)
        p += 4; // producer — skip
        p += 2; // language — skip
        p += 4; // name — skip
        break;
      }
      case 2: { // module
        if (p + 4 + 4 + 4 > cu_end) return;
        // name(strp)
        std::string mod_name = read_strp(p, str_data, str_size);
        p += 4;
        // stmt_list(sec_offset)
        uint32_t stmt_list = 0;
        p = read_u32(p, stmt_list);
        // byte_size(data4) — skip
        p += 4;

        // Parse column number from "column N"
        uint32_t col_num = UINT32_MAX;
        if (mod_name.size() > COLUMN_PREFIX_LEN &&
            mod_name.compare(0, COLUMN_PREFIX_LEN, COLUMN_PREFIX) == 0) {
          try {
            col_num = static_cast<uint32_t>(std::stoul(mod_name.substr(COLUMN_PREFIX_LEN)));
          } catch (...) {}
        }
        cur_col  = col_num;
        cur_stmt = stmt_list;
        break;
      }
      case 3: { // label
        if (p + 4 + 4 + 4 + 4 > cu_end) return;
        uint32_t low_pc = 0;
        p = read_u32(p, low_pc);
        std::string ann_id   = read_strp(p, str_data, str_size); p += 4;
        std::string ann_name = read_strp(p, str_data, str_size); p += 4;
        std::string ann_desc = read_strp(p, str_data, str_size); p += 4;
        if (cur_col != UINT32_MAX)
          pending_labels.push_back({cur_col, low_pc, ann_id, ann_name, ann_desc});
        break;
      }
      default:
        // Unknown DIE — we cannot safely skip; stop parsing
        return;
    }
  }

  // Convert pending_labels into dwarf_debug_row annotations (line=0, file empty).
  for (const auto& lbl : pending_labels) {
    dwarf_debug_row row{};
    row.column          = lbl.col;
    row.address         = lbl.low_pc;
    row.page_index      = lbl.low_pc / DWARF_PAGE_SIZE;
    row.page_offset     = lbl.low_pc % DWARF_PAGE_SIZE;
    row.annotation_id   = lbl.ann_id;
    row.annotation_name = lbl.ann_name;
    row.annotation_desc = lbl.ann_desc;
    m_rows.push_back(row);
  }
}

// ─── .debug_line interpreter ─────────────────────────────────────────────────

void
dwarf_reader::parse_line_table(
    const uint8_t* line_data, size_t line_size,
    size_t stmt_offset,
    uint32_t col_num)
{
  if (stmt_offset >= line_size) return;

  const uint8_t* p   = line_data + stmt_offset;
  const uint8_t* end = line_data + line_size;

  if (p + 10 > end) return;

  // Parse prologue
  uint32_t total_length = 0;
  p = read_u32(p, total_length);
  const uint8_t* stmt_end = p + total_length;
  if (stmt_end > end) return;

  uint16_t version = 0;
  p = read_u16(p, version);
  if (version != DWARF_VERSION) return;

  uint8_t addr_size = 0; p = read_u8(p, addr_size);
  uint8_t seg_size  = 0; p = read_u8(p, seg_size);

  uint32_t header_length = 0;
  p = read_u32(p, header_length);
  const uint8_t* program_start = p + header_length;
  if (program_start > stmt_end) return;

  uint8_t min_inst_len  = 0; p = read_u8(p, min_inst_len);
  uint8_t max_ops_inst  = 0; p = read_u8(p, max_ops_inst);
  { uint8_t tmp = 0; p = read_u8(p, tmp); (void)tmp; } // default_is_stmt — not used by this consumer
  uint8_t line_base_u   = 0; p = read_u8(p, line_base_u);
  const int8_t line_base = static_cast<int8_t>(line_base_u);
  uint8_t line_range    = 0; p = read_u8(p, line_range);
  uint8_t opcode_base   = 0; p = read_u8(p, opcode_base);

  // Standard opcode lengths [1..opcode_base-1]
  if (opcode_base == 0) return; // guard against degenerate prologue
  std::vector<uint8_t> std_opcode_lens(opcode_base - 1, 0);
  for (uint8_t i = 0; i < opcode_base - 1 && p < program_start; ++i)
    p = read_u8(p, std_opcode_lens[i]);

  // Parse DWARF v5 prologue tables and collect file names.
  std::vector<std::string> filenames;
  if (p < program_start) {
    // DWARF v5 directory table
    uint8_t dir_fmt_count = 0;
    p = read_u8(p, dir_fmt_count);
    if (dir_fmt_count > 0) {
      for (uint8_t i = 0; i < dir_fmt_count; ++i) {
        uint64_t ct = 0, form = 0;
        p = read_uleb128(p, program_start, ct);
        p = read_uleb128(p, program_start, form);
      }
    }
    uint64_t dir_count = 0;
    p = read_uleb128(p, program_start, dir_count);
    // We don't use directory entries; dir_count should be 0 in our output.

    // DWARF v5 file name table
    if (p < program_start) {
      uint8_t fn_fmt_count = 0;
      p = read_u8(p, fn_fmt_count);
      struct fmt_pair { uint64_t ct = 0; uint64_t form = 0; };
      std::vector<fmt_pair> fn_fmts(fn_fmt_count);
      for (uint8_t i = 0; i < fn_fmt_count && p < program_start; ++i) {
        p = read_uleb128(p, program_start, fn_fmts[i].ct);
        p = read_uleb128(p, program_start, fn_fmts[i].form);
      }
      uint64_t fn_count = 0;
      p = read_uleb128(p, program_start, fn_count);
      for (uint64_t fi = 0; fi < fn_count && p < program_start; ++fi) {
        std::string fname;
        for (const auto& fmt : fn_fmts) {
          if (fmt.form == DW_FORM_string) {
            p = read_cstr(p, program_start, fname);
          } else {
            size_t fsz = form_fixed_size(static_cast<uint64_t>(fmt.form));
            if (fsz) p += fsz;
            else break;
          }
        }
        filenames.push_back(fname);
      }
    }
  }

  // Run the line-number program starting at program_start
  p = program_start;
  {
    // Line-number machine registers
    uint32_t reg_addr = 0;
    uint32_t reg_file = 0;
    int32_t  reg_line = 1;
    bool     end_seq  = false;

    auto emit_row = [&]() {
      if (end_seq) return;
      dwarf_debug_row row{};
      row.column      = col_num;
      row.address     = reg_addr;
      row.page_index  = reg_addr / DWARF_PAGE_SIZE;
      row.page_offset = reg_addr % DWARF_PAGE_SIZE;
      row.line        = static_cast<uint32_t>(reg_line);
      if (reg_file < filenames.size())
        row.file = filenames[reg_file];
      m_rows.push_back(row);
    };

    while (p < stmt_end) {
      uint8_t opcode = 0;
      p = read_u8(p, opcode);

      if (opcode == 0) {
        // Extended opcode
        uint64_t ext_len = 0;
        p = read_uleb128(p, stmt_end, ext_len);
        const uint8_t* ext_end = p + ext_len;
        if (ext_len == 0 || ext_end > stmt_end) break;
        uint8_t ext_op = 0;
        p = read_u8(p, ext_op);
        if (ext_op == DW_LNE_end_sequence) {
          emit_row();
          end_seq = true;
          reg_addr = 0; reg_file = 0; reg_line = 1; end_seq = false;
        }
        p = ext_end; // skip remainder of extended opcode
      } else if (opcode < opcode_base) {
        // Standard opcode
        switch (opcode) {
          case DW_LNS_copy:
            emit_row();
            break;
          case DW_LNS_advance_pc: {
            uint64_t delta = 0;
            p = read_uleb128(p, stmt_end, delta);
            reg_addr += static_cast<uint32_t>(delta * min_inst_len);
            break;
          }
          case DW_LNS_advance_line: {
            int64_t delta = 0;
            p = read_sleb128(p, stmt_end, delta);
            reg_line += static_cast<int32_t>(delta);
            break;
          }
          case DW_LNS_set_file: {
            uint64_t fi = 0;
            p = read_uleb128(p, stmt_end, fi);
            reg_file = static_cast<uint32_t>(fi);
            break;
          }
          case DW_LNS_set_column: {
            uint64_t col = 0;
            p = read_uleb128(p, stmt_end, col);
            (void)col;
            break;
          }
          case DW_LNS_negate_stmt:
            break; // is_stmt not used by this consumer
          case DW_LNS_set_basic_block:
            break;
          case DW_LNS_const_add_pc: {
            // advance PC by: ((255 - opcode_base) / line_range) * min_inst_len
            if (line_range > 0) {
              uint32_t advance = static_cast<uint32_t>(
                  ((255 - opcode_base) / line_range) * min_inst_len);
              reg_addr += advance;
            }
            break;
          }
          case DW_LNS_fixed_advance_pc: {
            uint16_t advance = 0;
            p = read_u16(p, advance);
            reg_addr += advance;
            break;
          }
          case DW_LNS_set_prologue_end:
          case DW_LNS_set_epilogue_begin:
            break;
          case DW_LNS_set_isa: {
            uint64_t isa = 0;
            p = read_uleb128(p, stmt_end, isa);
            break;
          }
          default: {
            // Skip unknown standard opcode using opcode length table
            const size_t opcode_idx = static_cast<size_t>(opcode - 1U);
            const uint8_t nargs = (opcode_idx < std_opcode_lens.size())
                                  ? std_opcode_lens[opcode_idx] : 0;
            for (uint8_t a = 0; a < nargs; ++a) {
              uint64_t skip = 0;
              p = read_uleb128(p, stmt_end, skip);
            }
            break;
          }
        }
      } else {
        // Special opcode
        const uint8_t adjusted = opcode - opcode_base;
        const int32_t line_inc = line_base + static_cast<int32_t>(adjusted % line_range);
        const uint32_t addr_inc = static_cast<uint32_t>((adjusted / line_range) * min_inst_len);
        reg_addr += addr_inc;
        reg_line += line_inc;
        emit_row();
      }
    }
  }
}

// ─── Constructor ─────────────────────────────────────────────────────────────

dwarf_reader::dwarf_reader(const ELFIO::elfio& elf, const std::string& suffix)
{
  const auto [str_data, str_size] =
      get_section_data(elf, ".debug_str", suffix);
  const auto [line_data, line_size] =
      get_section_data(elf, ".debug_line", suffix);
  const auto [info_data, info_size] =
      get_section_data(elf, ".debug_info", suffix);

  if (!info_data || !line_data) return; // No DWARF present

  // Parse .debug_info to get column→stmt_offset mapping + annotation DIEs
  std::vector<col_info> cols;
  parse_debug_info(info_data, info_size,
                   str_data, str_size,
                   cols);

  // Parse each column's line-number program
  for (const auto& ci : cols)
    parse_line_table(line_data, line_size, ci.stmt_offset, ci.col_num);

  m_has_dwarf = !m_rows.empty() || !cols.empty();
}

// ─── Lookup ──────────────────────────────────────────────────────────────────

dwarf_debug_row
dwarf_reader::find_row(uint32_t col, uint32_t page_idx, uint32_t offset) const
{
  for (const auto& row : m_rows) {
    if (row.column == col && row.page_index == page_idx && row.page_offset == offset)
      return row;
  }
  return {};
}

} // namespace aiebu
