// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_ENCODER_AIE2PS_DWARF_WRITER_H_
#define AIEBU_ENCODER_AIE2PS_DWARF_WRITER_H_

#include "report.h"

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
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
 * .debug_info) from a collection of Debug objects populated during encoding.
 * The emitter uses DWARF32 format with address_size=4 (ELFCLASS32), matching
 * the .ctrltext address encoding: upper 19 bits = page index, lower 13 bits =
 * byte offset within the page.
 *
 * All CUs are concatenated into a single .debug_info following the standard
 * DWARF layout: one section of each kind per ELF.  The shared .debug_str
 * pool naturally deduplicates strings across CUs (e.g. the producer string
 * is only stored once).  Each column in a CU's Debug data becomes a
 * DW_TAG_module child of the CU.  Annotated instructions become either
 * DW_TAG_variable (data-referencing: APPLY_OFFSET_57, UC_DMA_WRITE_DES*) or
 * DW_TAG_label (code-position probes: NOP, START_JOB, WAIT_TCTS, ...) children
 * of their column's module.  Classification is done by aie2ps_encoder; new
 * opcodes default to DW_TAG_label.
 */
class dwarf_writer {
public:
  /**
   * Build all four DWARF sections from a collection of compile units.
   *
   * All CUs are concatenated into a single set of sections following the
   * standard DWARF layout (one .debug_info, one .debug_str, one .debug_line,
   * one .debug_abbrev per ELF).  Each CU has its own unit_length header so
   * standard tools can skip to the next CU.
   *
   * @param cus  Ordered list of (cu_name, Debug) pairs.  cu_name is the
   *             "kernel:instance" string (e.g. "DPU:dpu0") written as
   *             DW_AT_name on the compile unit.  Debug is the object
   *             populated by aie2ps_encoder::process() for that instance.
   */
  dwarf_sections build(const std::vector<std::pair<std::string, Debug>>& cus);

private:
  // ── .debug_str helpers ──────────────────────────────────────────────────
  // Intern a string into the string table; returns its byte offset.
  // str_buf and str_index are owned by build() and passed through.
  static uint32_t intern_str(std::vector<uint8_t>& str_buf,
                             std::unordered_map<std::string, uint32_t>& str_index,
                             const std::string& s);

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
  // Per-column data bundle passed to build_info() to avoid adjacent same-type params.
  struct col_data {
    std::vector<uint32_t>                             nums;          ///< column numbers
    std::vector<uint32_t>                             stmt_offsets;  ///< byte offset into .debug_line
    std::vector<uint32_t>                             byte_sizes;    ///< DW_AT_byte_size per column
    std::vector<std::vector<std::shared_ptr<Function>>> funcs;       ///< functions per column
  };

  // Build the full .debug_info CU with one DW_TAG_module per column.
  static std::vector<uint8_t> build_info(
      std::vector<uint8_t>& str_buf,
      std::unordered_map<std::string, uint32_t>& str_index,
      const std::string& cu_name,
      const col_data& cols,
      const std::vector<annotation_type>& annotations);
};

} // namespace aiebu

#endif // AIEBU_ENCODER_AIE2PS_DWARF_WRITER_H_
