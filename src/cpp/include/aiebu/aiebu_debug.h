// SPDX-License-Identifier: MIT
// Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_DEBUG_H_
#define AIEBU_DEBUG_H_

#include <elfio/elfio.hpp>
#include <cstdint>
#include <string>

namespace aiebu {

/**
 * @struct opcode_information
 * @brief Decoded opcode result returned by get_opcode_information().
 *
 * Populated from the .dump section when present (provides source file and line),
 * or from an ISA binary walk as a fallback (provides argument values).
 * Check the `found` field before using other members.
 */
struct opcode_information {
  bool        found       = false;  // true if the opcode was successfully decoded
  std::string opcode_name;          // e.g. "MASK_POLL_32"
  std::string args_str;             // hex argument values (ISA path); empty if decoded from dump
  uint64_t    opcode_size = 0;      // instruction size in bytes; 0 if unknown
  uint64_t    page_offset = 0;      // absolute byte offset within the section (dump path)
  uint32_t    line        = 0;      // source line number; 0 if unknown
  std::string source_file;          // source file path; empty if unknown
  std::string diag_info;            // diagnostic detail on decode failure
};

/**
 * get_opcode_information() - Decode the opcode at a firmware-reported location.
 *
 * No debug_tools instance is required. Suitable for callers (e.g. XRT) that
 * already hold a parsed ELFIO object. The caller receives a structured result
 * and decides how to format and present it.
 *
 * Prefers the .dump section (richer output: source file, line number) and falls
 * back to an ISA binary walk when no dump section is present.
 *
 * @param elf          Pre-parsed ELFIO object.
 * @param group_id     Control code group identifier from the ELF loader
 *                     (e.g. returned by elf_impl::get_ctrlcode_id()).
 *                     Pass UINT32_MAX for single-instance ELFs with no group.
 * @param uc_idx       Microcontroller index reported by firmware at timeout.
 * @param page_idx     Page index reported by firmware at timeout.
 * @param offset       Byte offset within the page reported by firmware at timeout.
 * @return             Populated opcode_information; check `found` before use.
 */
opcode_information get_opcode_information(const ELFIO::elfio& elf,
                                          uint32_t group_id,
                                          uint32_t uc_idx,
                                          uint32_t page_idx,
                                          uint32_t offset);

} // namespace aiebu

#endif // AIEBU_DEBUG_H_
