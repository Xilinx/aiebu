// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_ELF_COREDUMP_ELFWRITER_H_
#define AIEBU_ELF_COREDUMP_ELFWRITER_H_

#include "aiebu/aiebu_assembler.h"

#include <cstdint>
#include <optional>
#include <vector>

namespace aiebu {

/*!
 * @class coredump_elf_writer
 *
 * @brief
 * Standalone ELF writer that produces an ET_CORE ELF from a raw AIE dump
 * blob.  Does NOT inherit from elf_writer so that e_shnum remains 0.
 *
 * Layout:
 *   seg[0] – PT_NOTE, PF_R  : note blob (prpsinfo + AIE dump header)
 *   seg[1] – PT_LOAD, PF_R  : raw dump payload
 */
class coredump_elf_writer
{
public:
  /*!
   * @param abi   OS/ABI byte for the target AIE architecture.
   * @param blob  Raw AIE memory dump bytes.
   * @param meta  Optional coredump metadata.
   */
  coredump_elf_writer(unsigned char abi,
                      const std::vector<char>& blob,
                      std::optional<aie_coredump_meta> meta);

  /*!
   * Assemble and return the ELF binary.
   */
  [[nodiscard]]
  std::vector<char> finalize() const;

private:
  unsigned char                    m_abi;
  std::vector<char>                m_blob;
  std::optional<aie_coredump_meta> m_meta;

  /// Build the NT_PRPSINFO note descriptor bytes.
  [[nodiscard]]
  std::vector<char> build_prpsinfo_desc() const;

  /// Build the NT_AIE_DUMP_HDR note descriptor bytes from metadata.
  [[nodiscard]]
  std::vector<char> build_aie_dump_hdr_desc(const aie_coredump_meta& meta) const;
};

/*!
 * @brief
 * Parse the NT_AIE_DUMP_HDR note from a coredump ELF and return the
 * metadata it contains.  Returns nullopt if the ELF is not ET_CORE,
 * cannot be loaded, or contains no AMDAIE_CORE note.
 *
 * @param elf_data  Raw bytes of the coredump ELF.
 */
std::optional<aie_coredump_meta>
parse_coredump_meta(const std::vector<char>& elf_data);

} // namespace aiebu

#endif // AIEBU_ELF_COREDUMP_ELFWRITER_H_
