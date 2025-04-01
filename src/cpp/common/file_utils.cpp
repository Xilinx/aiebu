// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include "file_utils.h"
#include "aiebu/aiebu_assembler.h"
#include "utils.h"

namespace aiebu {

struct cp_pktheader
{
  uint32_t stream_packet_ID : 5;
  uint32_t out_of_order_bd_idx : 6;
  uint32_t one_0 : 1;
  uint32_t stream_id_rtn : 3;
  uint32_t one_1 : 1;
  uint32_t source_row : 5;
  uint32_t source_col : 5;
  uint32_t three_0 : 3;
  uint32_t parity : 1;
};

struct cp_ctrlinfo
{
  uint32_t local_byte_addr : 20;
  uint32_t data_size : 2;
  uint32_t two_0 : 2;
  uint32_t stream_id_rtn : 5;
  uint32_t two_1 : 2;
  uint32_t parity : 1;
};


constexpr unsigned int magic_length = 16;
constexpr unsigned int elf_magic = 0x464c457f;

// TODO: Add magic numbers for other AIE flavors
constexpr unsigned int ctrlcode_magic_aie2 = 0x06040100;

// https://github.com/Xilinx/bootgen/blob/master/bootheader-versal.cpp
constexpr unsigned int pdi_magic0 = 0x000000dd;
constexpr unsigned int pdi_magic1 = 0x11223344;

aiebu_assembler::buffer_type
identify_buffer_type(const std::vector<unsigned char>& buffer)
{
  if (buffer.size() < magic_length)
    return aiebu_assembler::buffer_type::unspecified;

  const auto data = reinterpret_cast<const unsigned int *>(buffer.data());
  // ELF magic number
  // TODO: add additional check to distinguish between aie2 and aie2ps
  if (data[0] == elf_magic)
    return aiebu_assembler::buffer_type::elf_aie2;

  // Transaction ctrlcode header
  if (data[0] == ctrlcode_magic_aie2)
    return aiebu_assembler::buffer_type::blob_instr_transaction;

  // TODO: Put the reference to PDI format from bootgen
  // TODO: Add code to distinguish between aie2 and aie2ps
  if ((data[0] == pdi_magic0) && (data[1] == pdi_magic1))
    return aiebu_assembler::buffer_type::pdi_aie2;

  // TODO: Put the reference to Packet Header and Control Packet here
  // ctrlpkt identification is WIP
  const auto cphdr = reinterpret_cast<const cp_pktheader *>(data);
  const auto cp = reinterpret_cast<const cp_ctrlinfo *>(data + 1);

  if ((cphdr->one_0 == 0x0) && (cphdr->one_1 == 0x0) &&
      (cphdr->three_0 == 0x0) && (cp->two_0 == 0x0) && (cp->two_1 == 0x0)) {
    if (odd_parity_check(data[0]) && odd_parity_check(data[1]))
      return aiebu_assembler::buffer_type::blob_control_packet;
    else
      return aiebu_assembler::buffer_type::unspecified;
  }

  return aiebu_assembler::buffer_type::unspecified;
}

}
