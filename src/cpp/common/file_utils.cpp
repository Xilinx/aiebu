// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include "file_utils.h"
#include "aiebu/aiebu_assembler.h"

namespace aiebu {

aiebu_assembler::buffer_type
identify_buffer_type(const std::vector<unsigned char> &buffer)
{
  if (buffer.size() < 16)
    return aiebu_assembler::buffer_type::unspecified;

  // ELF magic number
  // TODO: add additional check to distinguish between aie2 and aie2ps
  if ((buffer[0] == 0x7f) && (buffer[1] == 0x45) && (buffer[2] == 0x4c) &&
      (buffer[3] == 0x46))
    return aiebu_assembler::buffer_type::elf_aie2;

  // Transaction ctrlcode header
  if ((buffer[0] == 0x00) && (buffer[1] == 0x01) && (buffer[2] == 0x04) &&
      (buffer[3] == 0x06))
    return aiebu_assembler::buffer_type::blob_instr_transaction;

  // TODO: Put the reference to PDI format from bootgen
  // TODO: Add code to distinguish between aie2 and aie2ps
  if ((buffer[0] == 0xdd) && (buffer[1] == 0x00) && (buffer[2] == 0x00) &&
      (buffer[3] == 0x00) && (buffer[4] == 0x44) && (buffer[5] == 0x33) &&
      (buffer[6] == 0x22) && (buffer[7] == 0x11) && (buffer[8] == 0x88) &&
      (buffer[9] == 0x77) && (buffer[10] == 0x66) && (buffer[11] == 0x55))
    return aiebu_assembler::buffer_type::pdi_aie2;

  // TODO: Put the reference to Packet Header and Control Packet here
  // ctrlpkt identification is WIP
  if (((buffer[1] & 0x88) == 0x0) && ((buffer[2] & 0x80) == 0x0) &&
      ((buffer[3] & 0x70) == 0x0) && ((buffer[6] & 0xc0) == 0x0) &&
      ((buffer[7] & 0x60) == 0x0))
    return aiebu_assembler::buffer_type::blob_control_packet;

  return aiebu_assembler::buffer_type::unspecified;
}

}
