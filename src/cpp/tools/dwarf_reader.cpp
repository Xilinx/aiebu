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
#include <string_view>
#include <boost/endian/conversion.hpp>
#include <elfio/elfio.hpp>

namespace aiebu {

using namespace dwarf5;

// ─── LEB128 encoding constants ───────────────────────────────────────────────

static constexpr uint8_t  LEB128_VALUE_MASK  = 0x7F;
static constexpr uint8_t  LEB128_MORE_BIT    = 0x80;
static constexpr uint8_t  LEB128_SIGN_BIT    = 0x40;
static constexpr unsigned LEB128_SHIFT       = 7;
static constexpr unsigned LEB128_MAX_SHIFT   = 64U; // bits in int64_t

// Minimum byte counts for DWARF v5 headers (used as guard checks before parsing).
// CU header:   unit_length(4) + version(2) + unit_type(1) + addr_size(1) + abbrev_off(4) = 12,
//              but we check 11 to detect at least the length + version fields.
static constexpr size_t CU_HEADER_MIN_SIZE   = 11;
// Line table:  total_length(4) + version(2) + addr_size(1) + seg_size(1) + hdr_length(4) = 12,
//              but we check 10 to detect the length + version fields.
static constexpr size_t LINE_HEADER_MIN_SIZE = 10;

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
    const uint8_t byte = *p++;
    v |= (static_cast<uint64_t>(byte & LEB128_VALUE_MASK) << shift);
    shift += LEB128_SHIFT;
    if ((byte & LEB128_MORE_BIT) == 0)
      break;
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
    v |= (static_cast<int64_t>(byte & LEB128_VALUE_MASK) << shift);
    shift += LEB128_SHIFT;
    if ((byte & LEB128_MORE_BIT) == 0)
      break;
  }
  // Sign extend
  if (shift < LEB128_MAX_SHIFT && (byte & LEB128_SIGN_BIT) != 0)
    v |= -(static_cast<int64_t>(1) << shift);
  return p;
}

const uint8_t* dwarf_reader::read_cstr(const uint8_t* p, const uint8_t* end, std::string& s)
{
  const auto* nul = static_cast<const uint8_t*>(
      std::memchr(p, 0, static_cast<size_t>(end - p)));
  if (!nul)
    nul = end;
  s.assign(reinterpret_cast<const char*>(p), nul - p);
  return (nul < end) ? nul + 1 : end;
}

std::string dwarf_reader::read_strp(const uint8_t* p, const uint8_t* str_data, size_t str_size)
{
  uint32_t offset = 0;
  read_u32(p, offset);
  if (!str_data || offset >= str_size)
    return {};
  const char* cstr = reinterpret_cast<const char*>(str_data + offset);
  // Find NUL within bounds
  size_t max_len = str_size - offset;
  const auto* nul = static_cast<const char*>(std::memchr(cstr, 0, max_len));
  size_t len = nul ? static_cast<size_t>(nul - cstr) : max_len;
  return {cstr, len};
}

// ─── Section lookup ──────────────────────────────────────────────────────────

std::pair<const uint8_t*, size_t>
dwarf_reader::get_section_data_by_index(const ELFIO::elfio& elf, size_t index)
{
  if (index >= elf.sections.size())
    return {nullptr, 0};
  const ELFIO::section* sec = elf.sections[static_cast<ELFIO::Elf_Half>(index)];
  if (sec && sec->get_data() && sec->get_size() > 0)
    return {reinterpret_cast<const uint8_t*>(sec->get_data()), sec->get_size()};
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
    case DW_FORM_addr:
    case DW_FORM_strp:
    case DW_FORM_data4:
    case DW_FORM_sec_offset: return 4;
    case DW_FORM_data2:      return 2;
    default:                 return 0; // unknown / variable
  }
}

std::string
dwarf_reader::parse_debug_info(
    const uint8_t*& p_info,   size_t& info_remaining,
    const uint8_t*  str_data, size_t  str_size,
    std::vector<col_info>& cols_out)
{
  std::string cu_name_result;
  const uint8_t* p = p_info;

  if (info_remaining < CU_HEADER_MIN_SIZE) {
    info_remaining = 0;
    return cu_name_result;
  }

  // Parse DWARF v5 CU header — read unit_length first so we can advance
  // p_info past this entire CU before any early return.
  uint32_t unit_length = 0;
  p = read_u32(p, unit_length);

  // Advance the caller's pointer and remaining count past this whole CU now,
  // so every return path (including error returns below) leaves the caller
  // positioned at the next CU.
  const size_t cu_total = 4 + static_cast<size_t>(unit_length); // length field + body
  if (cu_total > info_remaining) {
    info_remaining = 0;
    return cu_name_result;
  }
  p_info         += cu_total;
  info_remaining -= cu_total;

  const uint8_t* cu_end = p_info; // one-past-end of this CU's body

  uint16_t version = 0;
  p = read_u16(p, version);
  if (version != DWARF_VERSION)
    return cu_name_result;

  uint8_t unit_type = 0;
  p = read_u8(p, unit_type);
  if (unit_type != DW_UT_compile)
    return cu_name_result; // Only compile units supported
  uint8_t addr_size = 0;
  p = read_u8(p, addr_size);
  p += 4; // abbrev_offset — skip (.debug_abbrev not parsed; fixed schema assumed)

  if (addr_size != DWARF_ADDR_SIZE)
    return cu_name_result; // Only support 32-bit addresses

  // We now parse DIEs using our fixed abbreviation knowledge:
  //   abbrev 1 = compile_unit (has_children): producer(strp), language(data2), name(strp)
  //   abbrev 2 = module       (has_children): name(strp), stmt_list(sec_offset), byte_size(data4)
  //   abbrev 3 = variable     (no_children):  name(strp), location(exprloc: DW_OP_addr+addr4),
  //                                           const_value(strp), description(strp)
  //   abbrev 4 = label        (no_children):  name(strp), low_pc(addr),
  //                                           const_value(strp), description(strp)

  // Current DW_TAG_module state
  uint32_t cur_col  = UINT32_MAX;
  uint32_t cur_stmt = 0;

  // Helper: push an annotation row to m_rows (used by cases 3 and 4).
  // compile_unit DIE (abbrev 1) always precedes module/annotation DIEs, so
  // cu_name_result is set before any annotation DIE is encountered.
  auto push_annotation_row = [&](uint32_t addr,
                                 std::string ann_name,
                                 std::string ann_id,
                                 std::string ann_desc) {
    if (cur_col == UINT32_MAX) return;
    dwarf_debug_row row{};
    row.column          = cur_col;
    row.address         = addr;
    row.page_index      = addr / DWARF_PAGE_SIZE;
    row.page_offset     = addr % DWARF_PAGE_SIZE;
    row.annotation_name = std::move(ann_name);
    row.annotation_id   = std::move(ann_id);
    row.annotation_desc = std::move(ann_desc);
    row.cu_name         = cu_name_result;
    m_rows.emplace_back(std::move(row));
  };

  while (p < cu_end) {
    uint64_t abbrev_code = 0;
    p = read_uleb128(p, cu_end, abbrev_code);
    if (abbrev_code == 0) {
      // null DIE — ends children list
      // If we were in a module, record it
      if (cur_col != UINT32_MAX) {
        cols_out.emplace_back(col_info{cur_col, cur_stmt});
        cur_col = UINT32_MAX;
      }
      continue;
    }

    switch (abbrev_code) {
      case 1: { // compile_unit: producer(strp) language(data2) name(strp)
        if (p + 4 + 2 + 4 > cu_end)
          return cu_name_result;
        p += 4; // producer — skip
        p += 2; // language — skip
        cu_name_result = read_strp(p, str_data, str_size);
        p += 4; // name
        break;
      }
      case 2: { // module
        if (p + 4 + 4 + 4 > cu_end)
          return cu_name_result; // name(strp) + stmt_list(sec_offset) + byte_size(data4)
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
      case 3: { // variable — annotated instruction
        // name(strp) + location(exprloc) + const_value(strp) + description(strp)
        // Minimum: 4(name) + 1(exprloc_len) + 1(DW_OP_addr) + 4(addr4) + 4(id) + 4(desc) = 18
        if (p + 4 + 1 + 1 + 4 + 4 + 4 > cu_end)
          return cu_name_result;
        std::string ann_name = read_strp(p, str_data, str_size); p += 4;
        // DW_AT_location as exprloc: ULEB128 length, then DW_OP_addr + 4-byte address
        uint64_t loc_len = 0;
        p = read_uleb128(p, cu_end, loc_len);
        if (p + static_cast<size_t>(loc_len) > cu_end)
          return cu_name_result;
        // DW_OP_addr expression: 1 opcode byte + 4-byte address
        uint32_t addr = 0;
        if (loc_len >= dwarf5::DW_OP_ADDR_EXPR_SIZE) {
          p += 1;                                                                            // skip DW_OP_addr byte
          p = read_u32(p, addr);
          p += static_cast<size_t>(loc_len - dwarf5::DW_OP_ADDR_EXPR_SIZE);  // skip any trailing bytes (none expected)
        } else {
          p += static_cast<size_t>(loc_len);                               // skip malformed expression
        }
        std::string ann_id   = read_strp(p, str_data, str_size); p += 4;
        std::string ann_desc = read_strp(p, str_data, str_size); p += 4;
        push_annotation_row(addr, std::move(ann_name), std::move(ann_id), std::move(ann_desc));
        break;
      }
      case 4: { // label — code-position probe point
        // name(strp) + low_pc(addr) + const_value(strp) + description(strp)
        if (p + 4 + 4 + 4 + 4 > cu_end)
          return cu_name_result;
        std::string ann_name = read_strp(p, str_data, str_size); p += 4;
        uint32_t addr = 0;
        p = read_u32(p, addr);           // DW_AT_low_pc DW_FORM_addr
        std::string ann_id   = read_strp(p, str_data, str_size); p += 4;
        std::string ann_desc = read_strp(p, str_data, str_size); p += 4;
        push_annotation_row(addr, std::move(ann_name), std::move(ann_id), std::move(ann_desc));
        break;
      }
      default:
        // Unknown DIE — we cannot safely skip; stop parsing
        return cu_name_result;
    }
  }

  return cu_name_result;
}

// ─── .debug_line interpreter ─────────────────────────────────────────────────

void
dwarf_reader::parse_line_table(
    const uint8_t* line_data, size_t line_size,
    size_t stmt_offset,
    uint32_t col_num,
    const uint8_t* str_data, size_t str_size,
    const std::string& cu_name)
{
  if (stmt_offset >= line_size)
    return;

  const uint8_t* p   = line_data + stmt_offset;
  const uint8_t* end = line_data + line_size;

  if (p + LINE_HEADER_MIN_SIZE > end)
    return;

  // Parse prologue
  uint32_t total_length = 0;
  p = read_u32(p, total_length);
  const uint8_t* stmt_end = p + total_length;
  if (stmt_end > end)
    return;

  uint16_t version = 0;
  p = read_u16(p, version);
  if (version != DWARF_VERSION)
    return;

  uint8_t addr_size = 0; p = read_u8(p, addr_size);
  p += 1; // seg_selector_size — skip (always 0 for AIEBU; not used by this consumer)

  uint32_t header_length = 0;
  p = read_u32(p, header_length);
  const uint8_t* program_start = p + header_length;
  if (program_start > stmt_end)
    return;

  uint8_t min_inst_len  = 0; p = read_u8(p, min_inst_len);
  uint8_t max_ops_inst  = 0; p = read_u8(p, max_ops_inst);
  { uint8_t tmp = 0; p = read_u8(p, tmp); (void)tmp; } // default_is_stmt — not used by this consumer
  uint8_t line_base_u   = 0; p = read_u8(p, line_base_u);
  const auto line_base = static_cast<int8_t>(line_base_u);
  uint8_t line_range    = 0; p = read_u8(p, line_range);
  uint8_t opcode_base   = 0; p = read_u8(p, opcode_base);

  // Standard opcode lengths [1..opcode_base-1]
  if (opcode_base == 0)
    return; // guard against degenerate prologue
  std::vector<uint8_t> std_opcode_lens(opcode_base - 1, 0);
  for (uint8_t i = 0; i < opcode_base - 1 && p < program_start; ++i)
    p = read_u8(p, std_opcode_lens[i]);

  // Parse DWARF v5 prologue tables and collect file names.
  std::vector<std::string> filenames;
  if (p < program_start) {
    // DWARF v5 directory table — read format descriptors then skip all entries.
    uint8_t dir_fmt_count = 0;
    p = read_u8(p, dir_fmt_count);
    struct fmt_pair { uint64_t ct = 0; uint64_t form = 0; };
    std::vector<fmt_pair> dir_fmts(dir_fmt_count);
    for (uint8_t i = 0; i < dir_fmt_count && p < program_start; ++i) {
      p = read_uleb128(p, program_start, dir_fmts[i].ct);
      p = read_uleb128(p, program_start, dir_fmts[i].form);
    }
    uint64_t dir_count = 0;
    p = read_uleb128(p, program_start, dir_count);
    // Skip directory entries — we use full paths in the file name table.
    for (uint64_t di = 0; di < dir_count && p < program_start; ++di) {
      for (const auto& dfmt : dir_fmts) {
        if (dfmt.form == DW_FORM_string) {
          std::string dummy;
          p = read_cstr(p, program_start, dummy);
        } else {
          const size_t fsz = form_fixed_size(dfmt.form);
          if (fsz) p += fsz;
          else break;
        }
      }
    }

    // DWARF v5 file name table
    if (p < program_start) {
      uint8_t fn_fmt_count = 0;
      p = read_u8(p, fn_fmt_count);
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
          if (fmt.ct == DW_LNCT_path) {
            if (fmt.form == DW_FORM_string) {
              p = read_cstr(p, program_start, fname);       // inline NUL-terminated
            } else if (fmt.form == DW_FORM_strp) {
              fname = read_strp(p, str_data, str_size);     // 4-byte .debug_str offset
              p += 4;
            } else {
              const size_t fsz = form_fixed_size(fmt.form);
              if (fsz) p += fsz;
              else break;
            }
          } else {
            const size_t fsz = form_fixed_size(fmt.form);
            if (fsz) p += fsz;
            else break;
          }
        }
        filenames.emplace_back(std::move(fname));
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

    // Template row: column and cu_name are constant for this column/CU — set once.
    dwarf_debug_row row_tmpl{};
    row_tmpl.column  = col_num;
    row_tmpl.cu_name = cu_name;

    auto emit_row = [&]() {
      dwarf_debug_row row = row_tmpl;
      row.address     = reg_addr;
      row.page_index  = reg_addr / DWARF_PAGE_SIZE;
      row.page_offset = reg_addr % DWARF_PAGE_SIZE;
      row.line        = static_cast<uint32_t>(reg_line);
      if (reg_file < filenames.size())
        row.file = filenames[reg_file];
      m_rows.emplace_back(std::move(row));
    };

    while (p < stmt_end) {
      uint8_t opcode = 0;
      p = read_u8(p, opcode);

      if (opcode == 0) {
        // Extended opcode
        uint64_t ext_len = 0;
        p = read_uleb128(p, stmt_end, ext_len);
        const uint8_t* ext_end = p + ext_len;
        if (ext_len == 0 || ext_end > stmt_end)
          break;
        const uint8_t ext_op = *p; // read opcode byte; p advanced to ext_end below
        if (ext_op == DW_LNE_end_sequence) {
          // DW_LNE_end_sequence marks the end of a sequence; it does NOT
          // represent a real instruction address and must not be emitted as
          // a source-location row (DWARF v5 spec §6.2.5.3).
          reg_addr = 0; reg_file = 0; reg_line = 1;
        }
        p = ext_end; // skip entire extended opcode (including ext_op byte)
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
            reg_file = static_cast<uint32_t>(fi); // NOLINT(bugprone-narrowing-conversions)
            break;
          }
          case DW_LNS_set_column: {
            uint64_t col = 0;
            p = read_uleb128(p, stmt_end, col);
            (void)col;
            break;
          }
          case DW_LNS_negate_stmt:   // is_stmt not used by this consumer
          case DW_LNS_set_basic_block:
            break;
          case DW_LNS_const_add_pc: {
            // advance PC by: ((DWARF_MAX_SPECIAL_OPCODE - opcode_base) / line_range) * min_inst_len
            if (line_range > 0) {
              auto advance = static_cast<uint32_t>(
                  ((DWARF_MAX_SPECIAL_OPCODE - opcode_base) / line_range) * min_inst_len);
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
            const auto opcode_idx = static_cast<size_t>(opcode - 1U);
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
        if (line_range == 0)
          break; // guard against degenerate prologue
        const auto adjusted = static_cast<uint8_t>(opcode - opcode_base);
        const auto line_inc = static_cast<int32_t>(line_base) + static_cast<int32_t>(adjusted % line_range);
        const auto addr_inc = static_cast<uint32_t>((adjusted / line_range) * min_inst_len);
        reg_addr += addr_inc;
        reg_line += line_inc;
        emit_row();
      }
    }
  }
}

// ─── Constructor ─────────────────────────────────────────────────────────────

dwarf_reader::dwarf_reader(const ELFIO::elfio& elf)
{
  // Standard DWARF layout (produced by dwarf_writer after the switch to
  // concatenated single-section format): one .debug_info, one .debug_line,
  // and one .debug_str per ELF, each containing data for all CUs.
  //
  // .debug_info may contain multiple CUs concatenated back-to-back.  We loop
  // through them using the unit_length field in each CU header to advance.
  const ELFIO::section* info_sec = elf.sections[".debug_info"];
  const ELFIO::section* line_sec = elf.sections[".debug_line"];
  const ELFIO::section* str_sec  = elf.sections[".debug_str"];

  if (!info_sec || !line_sec)
    return; // No DWARF present

  const auto* info_data = reinterpret_cast<const uint8_t*>(info_sec->get_data());
  const size_t info_size = info_sec->get_size();
  const auto* line_data = reinterpret_cast<const uint8_t*>(line_sec->get_data());
  const size_t line_size = line_sec->get_size();
  const uint8_t* str_data  = str_sec  ? reinterpret_cast<const uint8_t*>(str_sec->get_data())  : nullptr;
  const size_t   str_size  = str_sec  ? str_sec->get_size()  : 0;

  if (!info_data || info_size == 0 || !line_data || line_size == 0)
    return;

  // Walk all CUs in .debug_info.  parse_debug_info() advances p_info and
  // info_remaining past each CU it processes.
  const uint8_t* p_info       = info_data;
  size_t         info_remaining = info_size;

  while (info_remaining > 0) {
    std::vector<col_info> cols;
    const std::string cu_name = parse_debug_info(p_info, info_remaining,
                                                  str_data, str_size, cols);
    // cu_name is empty on error; the pointer has already been advanced past
    // the malformed CU so we can safely try the next one.
    if (cu_name.empty())
      continue;

    for (const auto& ci : cols)
      parse_line_table(line_data, line_size, ci.stmt_offset, ci.col_num,
                       str_data, str_size, cu_name);
  }

  m_has_dwarf = !m_rows.empty();
}

// ─── Lookup ──────────────────────────────────────────────────────────────────

dwarf_debug_row
dwarf_reader::find_row(uint32_t col, uint32_t page_idx, uint32_t offset,
                       const std::string& kernel_name) const
{
  // If kernel_name contains an instance (e.g. "DPU:subgraph_0"), filter by the
  // instance portion so that multi-instance ELFs return the correct CU's row.
  std::string filter_instance;
  if (!kernel_name.empty()) {
    const auto colon = kernel_name.find(':');
    if (colon != std::string::npos)
      filter_instance = kernel_name.substr(colon + 1);
  }

  for (const auto& row : m_rows) {
    if (row.column != col || row.page_index != page_idx || row.page_offset != offset)
      continue;
    if (!filter_instance.empty()) {
      // cu_name format: "kernel:instance" — match instance suffix
      const auto colon = row.cu_name.find(':');
      if (colon == std::string::npos ||
          row.cu_name.compare(colon + 1, std::string::npos, filter_instance) != 0)
        continue;
    }
    return row;
  }
  return {};
}

} // namespace aiebu
