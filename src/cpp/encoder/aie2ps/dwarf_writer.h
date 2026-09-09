// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_ENCODER_AIE2PS_DWARF_WRITER_H_
#define AIEBU_ENCODER_AIE2PS_DWARF_WRITER_H_

#include "report.h"

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace aiebu {

/**
 * @struct dwarf_sections
 * @brief Raw byte buffers for each DWARF v5 section produced by dwarf_writer.
 */
struct dwarf_sections {
  std::vector<uint8_t> debug_abbrev;
  std::vector<uint8_t> debug_str;
  std::vector<uint8_t> debug_line;   // all per-column stmt_lists concatenated
  std::vector<uint8_t> debug_info;
};

/**
 * @class dwarf_writer
 * @brief Emits DWARF v5 .debug_* sections from the aie2ps assembler debug data.
 *
 * Produces four raw byte buffers (.debug_abbrev, .debug_str, .debug_line,
 * .debug_info) from the Debug object populated during encoding.  The emitter
 * uses DWARF32 format with address_size=4 (ELFCLASS32), matching the
 * .ctrltext address encoding: upper 19 bits = page index, lower 13 bits =
 * byte offset within the page.
 *
 * One CU is emitted per call.  Each column in the Debug data becomes a
 * DW_TAG_module child of the CU.  Annotated instructions become
 * DW_TAG_label children of their column's module.
 */
class dwarf_writer {
public:
  /**
   * Build all four DWARF sections from the given debug data.
   *
   * @param cu_name     "kernel:instance" string (e.g. "DPU:dpu"), written as
   *                    DW_AT_name on the compile unit.
   * @param debug       Debug object populated by aie2ps_encoder::process().
   */
  dwarf_sections build(const std::string& cu_name, const Debug& debug);

private:
  // ── .debug_str helpers ──────────────────────────────────────────────────
  // Intern a string into the string table; returns its byte offset.
  uint32_t intern_str(const std::string& s);

  std::vector<uint8_t> m_str_buf;                        // .debug_str accumulator
  std::unordered_map<std::string, uint32_t> m_str_index; // string → offset cache

  // ── Little-endian write helpers ─────────────────────────────────────────
  static void append_u8 (std::vector<uint8_t>& buf, uint8_t  v);
  static void append_u16(std::vector<uint8_t>& buf, uint16_t v);
  static void append_u32(std::vector<uint8_t>& buf, uint32_t v);
  // Overwrite a previously reserved uint32_t slot (for back-patching lengths).
  static void patch_u32 (std::vector<uint8_t>& buf, size_t offset, uint32_t v);
  // Append a ULEB128-encoded value.
  static void append_uleb128(std::vector<uint8_t>& buf, uint64_t v);
  // Append a SLEB128-encoded value.
  static void append_sleb128(std::vector<uint8_t>& buf, int64_t v);
  // Append a null-terminated UTF-8 string (writes the bytes + NUL).
  static void append_str(std::vector<uint8_t>& buf, const std::string& s);

  // ── .debug_abbrev ───────────────────────────────────────────────────────
  static std::vector<uint8_t> build_abbrev();

  // ── .debug_line ─────────────────────────────────────────────────────────
  // Build one statement-list (prologue + line-number program) for a single column.
  // Returns the byte buffer; the caller appends it to the global .debug_line buffer.
  std::vector<uint8_t> build_line_table(
      const std::vector<std::shared_ptr<Function>>& col_funcs);

  // ── .debug_info ─────────────────────────────────────────────────────────
  // Build the full .debug_info CU with one DW_TAG_module per column.
  // col_stmt_offsets[i] is the byte offset of column i's stmt_list in .debug_line.
  // col_byte_sizes[i]   is DW_AT_byte_size for that module.
  std::vector<uint8_t> build_info(
      const std::string& cu_name,
      const std::vector<uint32_t>& col_nums,
      const std::vector<uint32_t>& col_stmt_offsets,
      const std::vector<uint32_t>& col_byte_sizes,
      const std::vector<std::vector<std::shared_ptr<Function>>>& col_funcs,
      const std::vector<annotation_type>& annotations);
};

} // namespace aiebu

#endif // AIEBU_ENCODER_AIE2PS_DWARF_WRITER_H_
