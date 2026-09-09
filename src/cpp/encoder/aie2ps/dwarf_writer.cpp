// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

// Emits DWARF v5 .debug_* sections for the merged-format AIE control-code ELF.
//
// Section layout produced:
//
//   .debug_abbrev  — fixed abbreviation table (one copy per ELF, offset always 0)
//   .debug_str     — string pool referenced by DW_FORM_strp offsets
//   .debug_line    — per-column statement lists, concatenated
//   .debug_info    — one CU with one DW_TAG_module per column
//
// DWARF v5 CU header (32-bit DWARF):
//   uint32_t  unit_length        (total bytes after this field)
//   uint16_t  version = 5
//   uint8_t   unit_type = 0x01   DW_UT_compile
//   uint8_t   address_size = 4
//   uint32_t  debug_abbrev_offset = 0
//
// Abbreviation codes used:
//   1  DW_TAG_compile_unit, has_children
//   2  DW_TAG_module,       has_children
//   3  DW_TAG_label,        no_children
//   0  terminator

#include "dwarf_writer.h"
#include "common/dwarf_constants.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <map>
#include <memory>
#include <unordered_map>

namespace aiebu {

using namespace dwarf5;

// Line-number program parameters (match the spec)
static constexpr int8_t  LINE_BASE      = -5;
static constexpr uint8_t LINE_RANGE     = 14;
static constexpr uint8_t OPCODE_BASE    = 13;  // first special opcode
static constexpr uint8_t MIN_INST_LEN  = 1;
static constexpr uint8_t MAX_OPS_INST  = 1;
static constexpr uint8_t DEFAULT_STMT  = 1;
// Standard opcode argument counts [1..12] as per spec
static constexpr std::array<uint8_t, 12> STD_OPCODE_LENGTHS = {0,1,1,1,1,0,0,0,1,0,0,1};

// Address encoding for the merged format:
//   address = page_index * DWARF_DWARF_PAGE_SIZE + byte_offset_in_page
// DWARF_DWARF_PAGE_SIZE = 0x2000 (8 KiB, 13 bits) — defined in dwarf_constants.h.

// Producer string embedded in every CU
static constexpr const char* PRODUCER_STR = "aiebu 1.0; ctrltext debug";

// ─── Abbreviation codes ─────────────────────────────────────────────────────

static constexpr uint8_t ABBREV_COMPILE_UNIT = 1;
static constexpr uint8_t ABBREV_MODULE       = 2;
static constexpr uint8_t ABBREV_LABEL        = 3;

// ─── LEB128 encoding constants ──────────────────────────────────────────────

static constexpr uint8_t  LEB128_VALUE_MASK = 0x7F; // low 7 bits of each byte
static constexpr uint8_t  LEB128_MORE_BIT   = 0x80; // continuation bit
static constexpr uint8_t  LEB128_SIGN_BIT   = 0x40; // sign bit of last group
static constexpr unsigned LEB128_SHIFT      = 7;    // bits per LEB128 group

// ─── Byte-manipulation constants ────────────────────────────────────────────

static constexpr unsigned SHIFT_BYTE1 = 8;
static constexpr unsigned SHIFT_BYTE2 = 16;
static constexpr unsigned SHIFT_BYTE3 = 24;
static constexpr uint32_t LOW_BYTE    = 0xFF;

// Maximum value of a special opcode byte
static constexpr uint8_t MAX_SPECIAL_OPCODE = 255;

// ─── Little-endian write helpers ────────────────────────────────────────────

void dwarf_writer::append_u8(std::vector<uint8_t>& buf, uint8_t v)
{
  buf.push_back(v);
}

void dwarf_writer::append_u16(std::vector<uint8_t>& buf, uint16_t v)
{
  buf.push_back(static_cast<uint8_t>(v & LOW_BYTE));
  buf.push_back(static_cast<uint8_t>((v >> SHIFT_BYTE1) & LOW_BYTE));
}

void dwarf_writer::append_u32(std::vector<uint8_t>& buf, uint32_t v)
{
  buf.push_back(static_cast<uint8_t>(v & LOW_BYTE));
  buf.push_back(static_cast<uint8_t>((v >> SHIFT_BYTE1) & LOW_BYTE));
  buf.push_back(static_cast<uint8_t>((v >> SHIFT_BYTE2) & LOW_BYTE));
  buf.push_back(static_cast<uint8_t>((v >> SHIFT_BYTE3) & LOW_BYTE));
}

void dwarf_writer::patch_u32(std::vector<uint8_t>& buf, size_t offset, uint32_t v)
{
  buf[offset + 0] = static_cast<uint8_t>(v & LOW_BYTE);
  buf[offset + 1] = static_cast<uint8_t>((v >> SHIFT_BYTE1) & LOW_BYTE);
  buf[offset + 2] = static_cast<uint8_t>((v >> SHIFT_BYTE2) & LOW_BYTE);
  buf[offset + 3] = static_cast<uint8_t>((v >> SHIFT_BYTE3) & LOW_BYTE);
}

void dwarf_writer::append_uleb128(std::vector<uint8_t>& buf, uint64_t v)
{
  while (true) {
    auto byte = static_cast<uint8_t>(v & LEB128_VALUE_MASK);
    v >>= LEB128_SHIFT;
    if (v != 0) byte |= LEB128_MORE_BIT;
    buf.push_back(byte);
    if (v == 0) break;
  }
}

void dwarf_writer::append_sleb128(std::vector<uint8_t>& buf, int64_t v)
{
  while (true) {
    auto byte = static_cast<uint8_t>(v & LEB128_VALUE_MASK);
    v >>= LEB128_SHIFT;
    // For signed LEB128 we need arithmetic shift; check if sign extension is correct.
    const bool done = (v == 0 && (byte & LEB128_SIGN_BIT) == 0) ||
                      (v == -1 && (byte & LEB128_SIGN_BIT) != 0);
    if (!done) byte |= LEB128_MORE_BIT;
    buf.push_back(byte);
    if (done) break;
  }
}

void dwarf_writer::append_str(std::vector<uint8_t>& buf, const std::string& s)
{
  buf.insert(buf.end(), s.begin(), s.end());
  buf.push_back(0); // NUL terminator
}

// ─── String pool ────────────────────────────────────────────────────────────

uint32_t dwarf_writer::intern_str(const std::string& s)
{
  auto it = m_str_index.find(s);
  if (it != m_str_index.end())
    return it->second;
  const uint32_t offset = static_cast<uint32_t>(m_str_buf.size());
  append_str(m_str_buf, s);
  m_str_index.emplace(s, offset);
  return offset;
}

// ─── .debug_abbrev ──────────────────────────────────────────────────────────

// Fixed abbreviation table — identical for every AIEBU-produced ELF.
// Layout:
//   Abbrev 1: DW_TAG_compile_unit, has_children
//     DW_AT_producer  DW_FORM_strp
//     DW_AT_language  DW_FORM_data2
//     DW_AT_name      DW_FORM_strp
//   Abbrev 2: DW_TAG_module, has_children
//     DW_AT_name      DW_FORM_strp
//     DW_AT_stmt_list DW_FORM_sec_offset
//     DW_AT_byte_size DW_FORM_data4
//   Abbrev 3: DW_TAG_label, no_children
//     DW_AT_low_pc      DW_FORM_addr
//     DW_AT_const_value DW_FORM_strp
//     DW_AT_name        DW_FORM_strp
//     DW_AT_description DW_FORM_strp
//   0 — terminator
std::vector<uint8_t> dwarf_writer::build_abbrev()
{
  std::vector<uint8_t> buf;

  // Abbrev 1: compile_unit
  append_uleb128(buf, ABBREV_COMPILE_UNIT);
  append_uleb128(buf, DW_TAG_compile_unit);
  append_u8(buf, DW_CHILDREN_yes);
  append_uleb128(buf, DW_AT_producer);   append_uleb128(buf, DW_FORM_strp);
  append_uleb128(buf, DW_AT_language);   append_uleb128(buf, DW_FORM_data2);
  append_uleb128(buf, DW_AT_name);       append_uleb128(buf, DW_FORM_strp);
  append_uleb128(buf, 0); append_uleb128(buf, 0); // end of attribute list

  // Abbrev 2: module
  append_uleb128(buf, ABBREV_MODULE);
  append_uleb128(buf, DW_TAG_module);
  append_u8(buf, DW_CHILDREN_yes);
  append_uleb128(buf, DW_AT_name);       append_uleb128(buf, DW_FORM_strp);
  append_uleb128(buf, DW_AT_stmt_list);  append_uleb128(buf, DW_FORM_sec_offset);
  append_uleb128(buf, DW_AT_byte_size);  append_uleb128(buf, DW_FORM_data4);
  append_uleb128(buf, 0); append_uleb128(buf, 0);

  // Abbrev 3: label
  append_uleb128(buf, ABBREV_LABEL);
  append_uleb128(buf, DW_TAG_label);
  append_u8(buf, DW_CHILDREN_no);
  append_uleb128(buf, DW_AT_low_pc);      append_uleb128(buf, DW_FORM_addr);
  append_uleb128(buf, DW_AT_const_value); append_uleb128(buf, DW_FORM_strp);
  append_uleb128(buf, DW_AT_name);        append_uleb128(buf, DW_FORM_strp);
  append_uleb128(buf, DW_AT_description); append_uleb128(buf, DW_FORM_strp);
  append_uleb128(buf, 0); append_uleb128(buf, 0);

  // Table terminator
  append_uleb128(buf, 0);

  return buf;
}

// ─── .debug_line statement list ─────────────────────────────────────────────

// Build one DWARF v5 statement list (prologue + line-number program) for
// a single column.  All Function objects in col_funcs belong to the same column.
//
// DWARF v5 line-number program prologue (32-bit format):
//   uint32_t total_length
//   uint16_t version = 5
//   uint8_t  address_size = 4
//   uint8_t  segment_selector_size = 0
//   uint32_t header_length
//   uint8_t  minimum_instruction_length = 1
//   uint8_t  maximum_operations_per_instruction = 1
//   uint8_t  default_is_stmt = 1
//   int8_t   line_base = -5
//   uint8_t  line_range = 14
//   uint8_t  opcode_base = 13
//   uint8_t  standard_opcode_lengths[12]
//   --- DWARF v5 directory / file table ---
//   uint8_t  directory_entry_format_count = 0  (no directories)
//   ULEB128  directories_count = 0
//   uint8_t  file_name_entry_format_count = 1
//   ULEB128  DW_LNCT_path = 1
//   ULEB128  DW_FORM_string = 0x08
//   ULEB128  file_names_count
//   for each file: null-terminated name string
std::vector<uint8_t>
dwarf_writer::build_line_table(
    const std::vector<std::shared_ptr<Function>>& col_funcs)
{
  // ── Collect unique source filenames for this column ──────────────────────
  // Maintain insertion order so file indices remain stable.
  std::vector<std::string> filenames;
  std::unordered_map<std::string, uint32_t> filename_to_idx;

  auto intern_filename = [&](const std::string& fn) -> uint32_t {
    auto it = filename_to_idx.find(fn);
    if (it != filename_to_idx.end()) return it->second;
    const uint32_t idx = static_cast<uint32_t>(filenames.size());
    filenames.push_back(fn);
    filename_to_idx.emplace(fn, idx);
    return idx;
  };

  // ── Build the line-number program ────────────────────────────────────────
  // We emit the program into a separate buffer and then prepend the prologue.
  std::vector<uint8_t> program;

  // Registers (line-number machine state)
  uint32_t reg_address  = 0;
  uint32_t reg_file     = 0;
  int32_t  reg_line     = 1;

  // Helper: advance PC and line using special opcodes or standard ones.
  // Emits the minimum sequence of opcodes to transition from current state
  // to (target_addr, target_line, target_file).
  auto emit_row = [&](uint32_t addr, int32_t line_num, uint32_t file_idx) {
    // Set file if changed
    if (file_idx != reg_file) {
      append_u8(program, DW_LNS_set_file);
      append_uleb128(program, file_idx);
      reg_file = file_idx;
    }

    const auto line_delta = static_cast<int32_t>(line_num - reg_line);
    const auto addr_delta = static_cast<int32_t>(addr) - static_cast<int32_t>(reg_address);

    // Try special opcode (encodes both address and line advance in one byte).
    // Special opcode = (line_delta - LINE_BASE) + (addr_delta * LINE_RANGE) + OPCODE_BASE
    // Valid when result fits in [OPCODE_BASE, 255].
    const auto line_enc = static_cast<int32_t>(line_delta - LINE_BASE);
    if (line_enc >= 0 && line_enc < static_cast<int32_t>(LINE_RANGE) && addr_delta >= 0) {
      const auto special = static_cast<int32_t>(line_enc + (addr_delta * static_cast<int32_t>(LINE_RANGE))
                        + static_cast<int32_t>(OPCODE_BASE));
      if (special >= static_cast<int32_t>(OPCODE_BASE) && special <= static_cast<int32_t>(MAX_SPECIAL_OPCODE)) {
        append_u8(program, static_cast<uint8_t>(special));
        reg_address = addr;
        reg_line    = line_num;
        return;
      }
    }

    // Fall back to standard opcodes
    if (addr_delta != 0) {
      append_u8(program, DW_LNS_advance_pc);
      append_uleb128(program, static_cast<uint64_t>(addr_delta));
      reg_address = addr;
    }
    if (line_delta != 0) {
      append_u8(program, DW_LNS_advance_line);
      append_sleb128(program, static_cast<int64_t>(line_delta));
      reg_line = line_num;
    }
    append_u8(program, DW_LNS_copy);
  };

  // Emit one row per text-section Line in each function
  for (const auto& func : col_funcs) {
    const uint32_t file_idx = intern_filename(func->get_filename());

    for (const auto& line : func->get_textlines()) {
      // DWARF address = page_index * DWARF_PAGE_SIZE + byte_offset_within_page.
      // Line::get_lowpc() already stores this encoding: it is set in
      // aie2ps_encoder.cpp as pc_low = pagenum * DWARF_PAGE_SIZE + textwriter->tell().
      const auto addr = static_cast<uint32_t>(line->get_lowpc());

      const auto lnum = static_cast<int32_t>(line->get_linenumber());
      if (lnum <= 0) continue;

      emit_row(addr, lnum, file_idx);
    }
  }

  // Emit DW_LNE_end_sequence
  append_u8(program, 0x00);          // extended opcode marker
  append_uleb128(program, 1);        // length of extended opcode body
  append_u8(program, DW_LNE_end_sequence);

  // ── Build the prologue ───────────────────────────────────────────────────
  // Prologue content (everything after header_length field):
  std::vector<uint8_t> prologue_body;
  append_u8(prologue_body, MIN_INST_LEN);
  append_u8(prologue_body, MAX_OPS_INST);
  append_u8(prologue_body, DEFAULT_STMT);
  append_u8(prologue_body, static_cast<uint8_t>(static_cast<int8_t>(LINE_BASE)));
  append_u8(prologue_body, LINE_RANGE);
  append_u8(prologue_body, OPCODE_BASE);
  for (uint8_t v : STD_OPCODE_LENGTHS)
    append_u8(prologue_body, v);

  // DWARF v5 directory table: no directories (all file entries use dir_index=0)
  append_u8(prologue_body, 0);   // directory_entry_format_count = 0
  append_uleb128(prologue_body, 0); // directories_count = 0

  // DWARF v5 file name table:
  //   format: 1 entry: DW_LNCT_path with DW_FORM_string
  append_u8(prologue_body, 1);                          // file_name_entry_format_count = 1
  append_uleb128(prologue_body, DW_LNCT_path);          // DW_LNCT_path
  append_uleb128(prologue_body, DW_FORM_string);        // DW_FORM_string
  append_uleb128(prologue_body, static_cast<uint64_t>(filenames.size())); // file_names_count
  for (const auto& fn : filenames)
    append_str(prologue_body, fn);

  // Full line table buffer: header_length field covers prologue_body
  std::vector<uint8_t> result;

  // Reserve space for total_length (uint32) — filled in at the end
  const size_t total_length_offset = result.size();
  append_u32(result, 0);              // placeholder

  append_u16(result, DWARF_VERSION);   // version = 5
  append_u8(result, DWARF_ADDR_SIZE); // address_size = 4
  append_u8(result, 0);               // segment_selector_size = 0

  // header_length = size of prologue_body
  const size_t header_length_offset = result.size();
  append_u32(result, 0);             // placeholder

  // Write prologue body
  result.insert(result.end(), prologue_body.begin(), prologue_body.end());

  // Patch header_length
  const uint32_t header_length = static_cast<uint32_t>(prologue_body.size());
  patch_u32(result, header_length_offset, header_length);

  // Write line number program
  result.insert(result.end(), program.begin(), program.end());

  // Patch total_length = everything after the total_length field itself
  const uint32_t total_length = static_cast<uint32_t>(result.size()) - 4;
  patch_u32(result, total_length_offset, total_length);

  return result;
}

// ─── .debug_info ────────────────────────────────────────────────────────────

std::vector<uint8_t>
dwarf_writer::build_info(
    const std::string& cu_name,
    const std::vector<uint32_t>& col_nums,
    const std::vector<uint32_t>& col_stmt_offsets,
    const std::vector<uint32_t>& col_byte_sizes,
    const std::vector<std::vector<std::shared_ptr<Function>>>& col_funcs,
    const std::vector<annotation_type>& annotations)
{
  // Pre-intern all strings so m_str_buf is stable before we write offsets.
  const uint32_t producer_off = intern_str(PRODUCER_STR);
  const uint32_t cu_name_off  = intern_str(cu_name);

  // Column module names
  std::vector<uint32_t> col_name_offs(col_nums.size());
  for (size_t i = 0; i < col_nums.size(); ++i)
    col_name_offs[i] = intern_str("column " + std::to_string(col_nums[i]));

  // Annotation strings: pre-intern once per annotation index to avoid repeated
  // hash-map lookups when the same annotation appears on multiple lines.
  struct ann_string_offsets { uint32_t id = 0, name = 0, desc = 0; };
  std::vector<ann_string_offsets> ann_offs(annotations.size());
  for (size_t i = 0; i < annotations.size(); ++i) {
    ann_offs[i].id   = intern_str(annotations[i].get_id());
    ann_offs[i].name = intern_str(annotations[i].get_name());
    ann_offs[i].desc = intern_str(annotations[i].get_description());
  }

  // Build CU body (DIEs after the CU header)
  std::vector<uint8_t> dies;

  // DW_TAG_compile_unit
  append_uleb128(dies, ABBREV_COMPILE_UNIT);
  append_u32(dies, producer_off);   // DW_AT_producer DW_FORM_strp
  append_u16(dies, DW_LANG_Mips_Assembler);  // DW_AT_language DW_FORM_data2
  append_u32(dies, cu_name_off);    // DW_AT_name     DW_FORM_strp

  // One DW_TAG_module per column
  for (size_t ci = 0; ci < col_nums.size(); ++ci) {
    append_uleb128(dies, ABBREV_MODULE);
    append_u32(dies, col_name_offs[ci]);         // DW_AT_name       DW_FORM_strp
    append_u32(dies, col_stmt_offsets[ci]);      // DW_AT_stmt_list  DW_FORM_sec_offset
    append_u32(dies, col_byte_sizes[ci]);        // DW_AT_byte_size  DW_FORM_data4

    // DW_TAG_label children for annotated instructions
    for (const auto& func : col_funcs[ci]) {
      for (const auto& line : func->get_textlines()) {
        const int ann_idx = line->get_annotation_index();
        if (ann_idx < 0 || ann_idx >= static_cast<int>(ann_offs.size()))
          continue;
        const auto& offs = ann_offs[static_cast<size_t>(ann_idx)];

        append_uleb128(dies, ABBREV_LABEL);
        append_u32(dies, static_cast<uint32_t>(line->get_lowpc())); // DW_AT_low_pc  DW_FORM_addr
        append_u32(dies, offs.id);                                   // DW_AT_const_value
        append_u32(dies, offs.name);                                 // DW_AT_name
        append_u32(dies, offs.desc);                                 // DW_AT_description
      }
    }

    // Module end marker
    append_u8(dies, 0);
  }

  // CU end marker
  append_u8(dies, 0);

  // Build the DWARF v5 CU header
  // unit_length covers everything after the unit_length field itself.
  // v5 header: version(2) + unit_type(1) + addr_size(1) + abbrev_offset(4) = 8 bytes
  const uint32_t unit_length = 2 + 1 + 1 + 4 + static_cast<uint32_t>(dies.size());

  std::vector<uint8_t> result;
  append_u32(result, unit_length);    // unit_length
  append_u16(result, DWARF_VERSION);  // version = 5
  append_u8(result,  DW_UT_compile);  // unit_type
  append_u8(result,  DWARF_ADDR_SIZE);// address_size
  append_u32(result, 0);              // debug_abbrev_offset = 0

  result.insert(result.end(), dies.begin(), dies.end());
  return result;
}

// ─── Top-level build ────────────────────────────────────────────────────────

dwarf_sections
dwarf_writer::build(const std::string& cu_name, const Debug& debug)
{
  m_str_buf.clear();
  m_str_index.clear();

  const auto& annotations = debug.get_annotations();
  const auto all_funcs     = debug.get_functions_in_order();

  // Partition functions by column number, preserving insertion order per column.
  std::map<uint32_t, std::vector<std::shared_ptr<Function>>> by_col;
  for (const auto& func : all_funcs)
    by_col[func->get_column()].push_back(func);

  // Ordered column list
  std::vector<uint32_t> col_nums;
  col_nums.reserve(by_col.size());
  for (const auto& kv : by_col)
    col_nums.push_back(kv.first);

  // Build .debug_line — one statement list per column, concatenated.
  std::vector<uint8_t> debug_line;
  std::vector<uint32_t> col_stmt_offsets(col_nums.size());
  std::vector<uint32_t> col_byte_sizes(col_nums.size());
  std::vector<std::vector<std::shared_ptr<Function>>> col_funcs(col_nums.size());

  for (size_t i = 0; i < col_nums.size(); ++i) {
    const uint32_t col = col_nums[i];
    col_funcs[i] = by_col.at(col);

    col_stmt_offsets[i] = static_cast<uint32_t>(debug_line.size());

    auto stmt_list = build_line_table(col_funcs[i]);
    debug_line.insert(debug_line.end(), stmt_list.begin(), stmt_list.end());

    // DW_AT_byte_size = highest address seen across all functions in this column.
    // Stored lowpc already encodes page_index * DWARF_PAGE_SIZE + in_page_offset,
    // so the maximum lowpc + 1 gives a reasonable byte_size.
    uint32_t max_end = 0;
    for (const auto& func : col_funcs[i]) {
      for (const auto& line : func->get_textlines()) {
        const auto end = static_cast<uint32_t>(line->get_lowpc()) +
            static_cast<uint32_t>(line->get_highpc() - line->get_lowpc() + 1);
        if (end > max_end) max_end = end;
      }
    }
    // Round up to the next full page (DWARF_PAGE_SIZE multiple) to cover all pages.
    if (max_end > 0)
      max_end = ((max_end + DWARF_PAGE_SIZE - 1) / DWARF_PAGE_SIZE) * DWARF_PAGE_SIZE;
    col_byte_sizes[i] = max_end;
  }

  // Build .debug_info
  auto debug_info = build_info(cu_name, col_nums, col_stmt_offsets,
                                col_byte_sizes, col_funcs, annotations);

  // Build .debug_abbrev (fixed, same for every ELF)
  auto debug_abbrev = build_abbrev();

  // .debug_str is m_str_buf after build_info() has interned all strings
  auto debug_str = std::move(m_str_buf);

  dwarf_sections out;
  out.debug_abbrev = std::move(debug_abbrev);
  out.debug_str    = std::move(debug_str);
  out.debug_line   = std::move(debug_line);
  out.debug_info   = std::move(debug_info);
  return out;
}

} // namespace aiebu
