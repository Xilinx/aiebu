// SPDX-License-Identifier: MIT
// Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_OPCODE_DECODER_H_
#define AIEBU_OPCODE_DECODER_H_

#include <elfio/elfio.hpp>
#include <cstdint>
#include <ostream>
#include <string>

namespace aiebu {

/**
 * write_opcode_information() - Decode opcode at (uc_idx, page_idx, offset) from
 * a pre-parsed ELF and write human-readable output to stream.
 *
 * No debug_tools instance is required. Suitable for callers (e.g. XRT) that
 * already hold a parsed ELFIO object and want to avoid ELF re-parsing overhead.
 *
 * @param stream       Output stream for the result.
 * @param elf          Pre-parsed ELFIO object.
 * @param filename     ELF file name (informational; omitted from output if empty).
 * @param kernel_name  Kernel instance name (reserved, currently unused).
 * @param uc_idx       Microcontroller index reported by firmware at timeout.
 * @param page_idx     Page index reported by firmware at timeout.
 * @param offset       Byte offset within the page reported by firmware at timeout.
 */
void write_opcode_information(std::ostream& stream,
                              const ELFIO::elfio& elf,
                              const std::string& filename,
                              const std::string& kernel_name,
                              uint32_t uc_idx,
                              uint32_t page_idx,
                              uint32_t offset);

} // namespace aiebu

#endif // AIEBU_OPCODE_DECODER_H_
