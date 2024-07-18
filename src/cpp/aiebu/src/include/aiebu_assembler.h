// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ASSEMBLER_H_
#define _AIEBU_ASSEMBLER_H_

#include <string>
#include <cstdint>
#include <vector>
#include <iostream>

#if defined(_WIN32)
#define DRIVER_DLLESPEC __declspec(dllexport)
#else
#define DRIVER_DLLESPEC __attribute__((visibility("default")))
#endif

namespace aiebu {

enum class patch_buffer_type {
  instruct,
  control_packet,
  max
};
 /*
 * This enum defines the patching schema that will be applied by the
 * elf loader. The schema will be encoded into the elf's addend field
 * in RELA section. All the patching starts at places indicated by buffer
 * type and offset.
 *
 * scaler_32:   symbol value (32 bits) will replace data in
 *              place
 * shim_dma_48: offset points to the start of shim DMA BD(8DW).
 *              symbol address (lower 48 bits) will be added
 *              to the value in offset[1] (full 32 bits) and
 *              offset[2] (lower 16 bits)
 * shim_dma_57: offset points to the start of shim DMA BD(9DW).
 *              symbol address (lower 57 bits) will be added
 *              to the value in offset[1] (full 32 bits),
 *              offset[2] (lower 16 bits) and offset[8]
 *              (lower 9 bits)
 * control_packet_48: offset points to the start of control-packet.
 *                    symbol address (lower 48 bits) will be added
 *                    to the value in offset[2] (full 32 bits) and
 *                    offset[3] (lower 16 bits)
 */
enum class patch_schema {
  uc_dma_remote_ptr_symbol = 1,
  shim_dma_57 = 2,
  scaler_32 = 3,
  control_packet_48 = 4,
  shim_dma_48 = 5,
  unknown
};

/*
 * Define the patch information for a given symbol.
 * @symbol     the symobl name
 * @buf_type   buf type, instruction buffer or control buffer
 * @schema     patching schema, see patch_schema
 * @offsets    the places the symbol should be patched.
 */
struct patch_info {
  std::string  symbol;
  patch_buffer_type  buf_type;
  patch_schema schema;
  uint32_t offset;
  uint32_t addend;
};

// Assembler Class

class DRIVER_DLLESPEC aiebu_assembler {
  std::vector<char> elf_data;

  public:

    enum class buffer_type {
      blob_instr_dpu,
      blob_instr_prepost,
      blob_instr_transaction,
      blob_control_packet
    };

  private:
    const buffer_type _type;

  public:
    /*
     * Constructor takes buffer type , 2 buffer and a vector of symbols with
     * their patching information as argument.
     * its throws aiebu::error object.
     * User may pass any combination like
     * 1. type as blob_instr_dpu, buffer1 as instruction buffer
     *    and buffer2 as control_packet: in this case it will package buffers in text and data
     *    section of elf respectively.
     * 2. type as blob_instr_dpu, buffer1 as instruction buffer
     *    and buffer2 as empty: in this case it will package buffer in text section.
     *
     * @type           buffer type
     * @instr_buf      first buffer
     * @constrol_buf   second buffer
     * @patch_data     relocatable information
     * @libs           libs to include in elf
     * @libpaths       paths to search for libs
     */
     aiebu_assembler(buffer_type type,
               const std::vector<char>& buffer1,
               const std::vector<char>& buffer2,
               const std::vector<patch_info>& patch_data,
               const std::vector<std::string>& libs = {},
               const std::vector<std::string>& libpaths = {});

    /*
     * Constructor takes buffer type, buffer,
     * and a vector of symbols with their patching information as argument.
     * its throws aiebu::error object.
     *
     * @type           buffer type
     * @instr_buf      first buffer
     * @libs           libs to include in elf
     * @libpaths       paths to search for libs
     * @patch_data     relocatable information
     */
    aiebu_assembler(buffer_type type,
              const std::vector<char>& buffer,
              const std::vector<std::string>& libs = {},
              const std::vector<std::string>& libpaths = {},
              const std::vector<patch_info>& patch_data = {});

    /*
     * This function return vector with elf content.
     *
     * Inside elf for IPU, instr_buf will be placed in .text section and control_buf will
     * be placed in .data section. There are other dynamic sections in the elf
     * containing the relocatable information. With this elf, at runtime, XRT
     * will patch the symbols (value or address based on the schema) into their
     * instruction buffer and control buffer before sending the buffer to device.
     *
     * return: vector of char with elf content
     */
    [[nodiscard]]
    std::vector<char>
    get_elf() const;

    void
    get_report(std::ostream &stream) const;
};

} //namespace aiebu

#endif // _AIEBU_ASSEMBLER_H_
