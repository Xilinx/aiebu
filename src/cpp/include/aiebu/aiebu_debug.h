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
 * @brief Decoded opcode result returned by AIEDebug::get_opcode_information().
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
  std::string diag_info;            // diagnostic detail on decode failure. This field contains a human-readable
                                    // explanation: which section was searched, offset within page header and why
                                    // the decode failed (section not found, offset out of range, unrecognised
                                    // opcode byte, or a malformed .dump JSON)
};

/**
 * @class AIEDebug
 * @brief Decodes opcode information from AIE2PS / AIE4 (AIE4A, AIE4Z) ELF binaries.
 *
 * Binds to a pre-parsed ELFIO object and exposes opcode decode operations.
 *
 * Construct once per ELF, then call get_opcode_information() with the
 * firmware-reported (uc_idx, page_idx, offset) triple.
 */
class AIEDebug {
public:
  /**
   * Construct an AIEDebug bound to the given ELF.
   * The ELFIO object must outlive this AIEDebug instance.
   */
  explicit AIEDebug(const ELFIO::elfio& elf);

  /**
   * get_opcode_information() - Decode the opcode at a firmware-reported location.
   *
   * Prefers the .dump section (richer output: source file, line number) and falls
   * back to an ISA binary walk when no dump section is present.
   *
   * For multi-instance (group) ELFs the kernel_name selects the correct instance
   * by locating its .group.N section via the ELF symbol table; the N suffix is
   * then used to find the matching .dump.N and .ctrltext sections.
   *
   * @param kernel_name  Kernel/instance identifier in "kernel:instance" format
   *                     (e.g. "DPU:dpu0"). Pass an empty string for
   *                     single-instance ELFs that have no .group sections.
   * @param uc_idx       Microcontroller index reported by firmware at timeout.
   * @param page_idx     Page index reported by firmware at timeout.
   * @param offset       Byte offset within the page reported by firmware at timeout.
   * @return             Populated opcode_information; check `found` before use.
   */
  opcode_information get_opcode_information(const std::string& kernel_name,
                                             uint32_t uc_idx,
                                             uint32_t page_idx,
                                             uint32_t offset) const;

private:
  const ELFIO::elfio* m_elf;

  uint32_t    resolve_group_name_id(const std::string& kernel_name) const;
  std::string get_dump_json_from_elf(uint32_t name_id) const;
  opcode_information decode_opcode(uint32_t uc_idx, uint32_t page_idx,
                                    uint32_t offset, uint32_t name_id) const;
};

} // namespace aiebu

#endif // AIEBU_DEBUG_H_
