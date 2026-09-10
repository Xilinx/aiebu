// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_DECOMPRESS_H_
#define AIEBU_DECOMPRESS_H_

#include <cstddef>
#include <vector>

namespace ELFIO {
class elfio;
class section;
}

namespace aiebu {

// ---------------------------------------------------------------------------
// Full-ELF decompression APIs (raw bytes, no ELFIO dependency)
// ---------------------------------------------------------------------------

/*
 * Return true if the ELF contains any SHF_COMPRESSED sections.
 *
 * Implemented as a direct raw scan of ELF section headers — no ELFIO, no
 * heap allocation.  Callers should use this to avoid the full ELFIO
 * parse cost of decompress_elf() for ELFs that are not compressed:
 *
 *   if (aiebu::is_elf_compressed(elf))
 *       elf = aiebu::decompress_elf(elf);
 *
 * @elf_bytes: ELF bytes to inspect.
 * @return:    true if at least one section has SHF_COMPRESSED set.
 */
bool
is_elf_compressed(const std::vector<char>& elf_bytes);

/*
 * Decompress a compressed AIE ELF and return the decompressed bytes.
 *
 * The algorithm is auto-detected per section from ch_type in the standard
 * ELF Elf_Chdr header — callers do not need to know which algorithm was
 * used during assembly.
 *
 * Takes const& because the decompressor always builds a new output buffer —
 * it never reuses or moves from the input.  The input only needs to stay
 * alive during the call.  If the ELF contains no SHF_COMPRESSED sections,
 * a copy of the input is returned.
 *
 * @elf_bytes: ELF bytes, possibly containing SHF_COMPRESSED sections.
 * @return:    Decompressed ELF bytes.  Throws std::runtime_error on corrupt
 *             data or unsupported compression type.
 */
std::vector<char>
decompress_elf(const std::vector<char>& elf_bytes);

// ---------------------------------------------------------------------------
// High-level per-section APIs (ELFIO-aware)
//
// These APIs abstract away compression details.  Callers pass ELFIO objects
// and aiebu determines internally whether decompression is needed.
// The "uncompressed" in the names communicates that the returned size /
// copied data may differ from the raw ELF section size.
// ---------------------------------------------------------------------------

/*
 * Returns the uncompressed data size of a section.
 *
 * If the section is SHF_COMPRESSED, returns the uncompressed size from the
 * Elf_Chdr header.  If not compressed, returns the raw section size.
 *
 * @sec:  Pointer to the ELFIO section (nullptr returns 0).
 * @elf:  ELFIO object that owns the section (used for elf_class).
 * @return: Uncompressed data size in bytes.
 * @throws: std::runtime_error if SHF_COMPRESSED section has invalid Chdr.
 */
std::size_t
get_section_uncompressed_size(const ELFIO::section* sec, const ELFIO::elfio& elf);

/*
 * Copies uncompressed section data into a caller-provided buffer.
 *
 * If the section is SHF_COMPRESSED, decompresses directly into dest.
 * If not compressed, memcpy's the raw section data.
 *
 * @sec:       Pointer to the ELFIO section (nullptr returns 0).
 * @elf:       ELFIO object that owns the section (used for elf_class).
 * @dest:      Output buffer to receive section data.
 * @dest_size: Size of the output buffer (must be >= get_section_uncompressed_size()).
 * @return:    Number of bytes written to dest.
 * @throws:    std::runtime_error on corrupt data, unsupported ch_type,
 *             or dest_size too small.
 */
std::size_t
copy_section_uncompressed_data(const ELFIO::section* sec, const ELFIO::elfio& elf,
                               void* dest, std::size_t dest_size);

} // namespace aiebu

#endif // AIEBU_DECOMPRESS_H_
