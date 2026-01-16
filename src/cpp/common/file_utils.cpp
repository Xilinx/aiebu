// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include "file_utils.h"
#include "aiebu/aiebu_assembler.h"
#include "utils.h"
#include "elfio/elfio.hpp"
#include <iostream>

namespace aiebu {

constexpr unsigned int magic_length = 16;
constexpr unsigned int elf_magic = 0x464c457f;

// optimized header major:1 minor:0 device:4(XAIE_DEV_GEN_AIE2P) row:6
constexpr unsigned int ctrlcode_header_aie2_opt = 0x06040001;
// non-optimized header major:0 minor:1 device:4(XAIE_DEV_GEN_AIE2P) row:6
constexpr unsigned int ctrlcode_header_aie2 = 0x06040100;

// https://github.com/Xilinx/bootgen/blob/master/bootheader-versal.cpp
constexpr unsigned int pdi_magic0 = 0x000000dd;
constexpr unsigned int pdi_magic1 = 0x11223344;

// Size of word in bytes
constexpr unsigned int word_size = 4;
// For AIE2 control packets are 8 words (8words * 4bytes/word = 32bytes) aligned
constexpr unsigned int ctrlpkt_offset_aie2 = 8 * word_size;

// ELF OS ABI values for AIE2 and AIE2PS
constexpr unsigned int os_abi_aie2p = 69;
constexpr unsigned int os_abi_aie2ps = 64;
constexpr unsigned int os_abi_aie2ps_group = 70;
// ELF header is at least 52 bytes for 32-bit, 64 bytes for 64-bit
constexpr unsigned int min_elf_header_size = 52;
// EI_OSABI is at offset 7 in the ELF header
constexpr unsigned int elf_os_abi_offset = 7;

aiebu_assembler::buffer_type
identify_buffer_type(const std::vector<char>& buffer)
{
  if (buffer.size() < magic_length)
    return aiebu_assembler::buffer_type::unspecified;
  const auto data = reinterpret_cast<const unsigned int *>(buffer.data());
  if (data[0] == elf_magic) {
    return identify_elf_type(buffer);
  }

  // Transaction ctrlcode header
  if (data[0] == ctrlcode_header_aie2 || data[0] == ctrlcode_header_aie2_opt)
    return aiebu_assembler::buffer_type::blob_instr_transaction;

  // TODO: Put the reference to PDI format from bootgen
  if ((data[0] == pdi_magic0) && (data[1] == pdi_magic1))
    return aiebu_assembler::buffer_type::pdi_aie2;

  return identify_control_packet(buffer.data(), buffer.size());
}


aiebu_assembler::buffer_type
identify_elf_type(const std::vector<char>& buffer)
{
  // ELF header is at least 52 bytes for 32-bit, 64 bytes for 64-bit
  if (buffer.size() >= min_elf_header_size) {
    // EI_OSABI is at offset 7 in the ELF header
    const auto os_abi = static_cast<unsigned char>(buffer.data()[elf_os_abi_offset]);
    if (os_abi == os_abi_aie2p)
      return aiebu_assembler::buffer_type::elf_aie2;
    else if (os_abi == os_abi_aie2ps || os_abi == os_abi_aie2ps_group)
      return aiebu_assembler::buffer_type::elf_aie2ps;
    else
      return aiebu_assembler::buffer_type::unspecified;
  }
  else
    throw error(error::error_code::invalid_elf, "ELF header size is less than expected");
}


aiebu_assembler::buffer_type
identify_control_packet(const char* buffer, uint64_t size)
{
  if (size < magic_length)
    return aiebu_assembler::buffer_type::unspecified;

  auto control_packet = reinterpret_cast<const unsigned int *>(buffer);
  uint64_t count = 0;
  // Check if the input buffer is control packet for aie2p
  while (count < size) {
    const auto cphdr = reinterpret_cast<const cp_pktheader_aie2p *>(control_packet);
    const auto cp = reinterpret_cast<const cp_ctrlinfo_aie2p *>(control_packet + 1);

    if ((cphdr->reserved_a_0 == 0x0) && (cphdr->reserved_b_0 == 0x0) &&
        (cphdr->reserved_c_000 == 0x0) && (cp->reserved_00 == 0x0) &&
        odd_parity_check(*control_packet) && odd_parity_check(*(control_packet + 1))) {
      uint64_t offset = ((sizeof(*cphdr) + sizeof(*cp)) / word_size) + (cp->num_data_beat + 1);
      control_packet += offset;
      count += offset * word_size;
    } else {
      break;
    }
  }

  if (count == size)
    return aiebu_assembler::buffer_type::blob_control_packet;

  // Check if the input buffer is control packet for aie2
  control_packet = reinterpret_cast<const unsigned int *>(buffer);
  count = 0;

  while (count < size) {
    const auto cphdr_aie2 = reinterpret_cast<const cp_pktheader_aie2p *>(control_packet);
    const auto cp_aie2 = reinterpret_cast<const cp_ctrlinfo_aie2p *>(control_packet + 1);
    if ((cphdr_aie2->reserved_a_0 == 0x0) && (cphdr_aie2->reserved_b_0 == 0x0) &&
        (cphdr_aie2->reserved_c_000 == 0x0) && (cp_aie2->reserved_00 == 0x0) &&
        odd_parity_check(*control_packet) && odd_parity_check(*(control_packet + 1))) {
      control_packet += ctrlpkt_offset_aie2 / word_size;
      count += ctrlpkt_offset_aie2;
    } else {
      return aiebu_assembler::buffer_type::unspecified;
    }
  }

  return aiebu_assembler::buffer_type::blob_control_packet_aie2;
}

}
