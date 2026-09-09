// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_TOOLS_DWARF_READER_H_
#define AIEBU_TOOLS_DWARF_READER_H_

#include <cstdint>
#include <string>
#include <vector>

// Forward declaration — consumers that include this header do not need elfio.hpp
namespace ELFIO { class elfio; }

namespace aiebu {

/**
 * @struct dwarf_debug_row
 * @brief One decoded row from the DWARF v5 .debug_line / .debug_info sections.
 *
 * Address encoding used in AIEBU merged ELFs:
 *   page_index  = address >> 13  (upper 19 bits)
 *   page_offset = address & 0x1FFF (lower 13 bits)
 */
struct dwarf_debug_row {
  uint32_t    column;           ///< Column number from "column N" DW_TAG_module
  uint32_t    page_index;       ///< address >> 13
  uint32_t    page_offset;      ///< address & 0x1FFF (byte offset within page)
  uint32_t    address;          ///< Raw DWARF address (page_index*PAGE_SIZE + page_offset)
  uint32_t    line;             ///< Source line number
  std::string file;             ///< Source file name (basename from prologue table)
  std::string annotation_name; ///< From DW_TAG_label DW_AT_name (empty if none)
  std::string annotation_id;   ///< From DW_TAG_label DW_AT_const_value (empty if none)
  std::string annotation_desc; ///< From DW_TAG_label DW_AT_description (empty if none)
};

/**
 * @class dwarf_reader
 * @brief Parses DWARF v5 .debug_* sections from an ELFIO object.
 *
 * Reads .debug_abbrev, .debug_str, .debug_line, and .debug_info and
 * exposes a per-row lookup suitable for the same use cases as the legacy
 * .dump JSON section.
 *
 * Usage:
 *   dwarf_reader reader(elf);
 *   if (reader.has_dwarf()) {
 *     auto row = reader.find_row(col, page_idx, offset);
 *   }
 */
class dwarf_reader {
public:
  /**
   * Construct and parse DWARF sections from the given ELF.
   * @param elf     ELFIO object with the ELF already loaded.
   * @param suffix  Optional group-ELF section suffix (e.g. ".0", ".1").
   *                Pass "" for single-instance ELFs.
   */
  explicit dwarf_reader(const ELFIO::elfio& elf, const std::string& suffix = "");

  /// True if DWARF .debug_info was found and parsed successfully.
  bool has_dwarf() const { return m_has_dwarf; }

  /**
   * Find the row matching (column, page_index, page_offset).
   * Returns a row with line==0 and empty file if not found.
   */
  dwarf_debug_row find_row(uint32_t col, uint32_t page_idx, uint32_t offset) const;

  /// All decoded rows, in statement-list order.
  const std::vector<dwarf_debug_row>& get_all_rows() const { return m_rows; }

private:
  static constexpr uint32_t PAGE_SIZE = 0x2000; ///< 8 KiB per page

  bool m_has_dwarf = false;
  std::vector<dwarf_debug_row> m_rows;

  // ── Parse helpers ────────────────────────────────────────────────────────
  static const uint8_t* read_u8 (const uint8_t* p, uint8_t&  v);
  static const uint8_t* read_u16(const uint8_t* p, uint16_t& v);
  static const uint8_t* read_u32(const uint8_t* p, uint32_t& v);
  static const uint8_t* read_uleb128(const uint8_t* p, const uint8_t* end, uint64_t& v);
  static const uint8_t* read_sleb128(const uint8_t* p, const uint8_t* end, int64_t& v);
  // Read a NUL-terminated string; advances p past the NUL.
  static const uint8_t* read_cstr(const uint8_t* p, const uint8_t* end, std::string& s);

  // ── Section data helpers ─────────────────────────────────────────────────
  /// Return (data, size) for a section by name, checking suffix fallback.
  static std::pair<const uint8_t*, size_t>
  get_section_data(const ELFIO::elfio& elf, const std::string& name, const std::string& suffix);

  // ── DWARF v5 line-number program interpreter ─────────────────────────────
  struct col_info {
    uint32_t col_num     = 0;
    uint32_t stmt_offset = 0;  // byte offset into .debug_line
  };

  // Parse .debug_info to get per-column stmt_list offsets and annotation DIEs.
  // Fills m_rows with annotation entries; fills cols_out with (col_num, stmt_offset) pairs.
  // The .debug_abbrev section is not parsed: the fixed schema emitted by dwarf_writer is assumed.
  void parse_debug_info(
      const uint8_t* info_data, size_t info_size,
      const uint8_t* str_data,  size_t str_size,
      std::vector<col_info>& cols_out);

  // Interpret one DWARF v5 line-number program for the given column and append rows.
  void parse_line_table(
      const uint8_t* line_data, size_t line_size,
      size_t stmt_offset,
      uint32_t col_num);

  // Read a strp (4-byte offset into .debug_str)
  static std::string read_strp(const uint8_t* p, const uint8_t* str_data, size_t str_size);
};

} // namespace aiebu

#endif // AIEBU_TOOLS_DWARF_READER_H_
