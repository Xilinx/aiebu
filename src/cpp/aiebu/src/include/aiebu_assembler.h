// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ASSEMBLER_H_
#define _AIEBU_ASSEMBLER_H_

#include <string>
#include <cstdint>
#include <vector>
#include <iostream>
#include <filesystem>
#include <map>

#if defined(_WIN32)
#define DRIVER_DLLESPEC __declspec(dllexport)
#else
#define DRIVER_DLLESPEC __attribute__((visibility("default")))
#endif

namespace aiebu {

// Assembler Class

class aiebu_assembler {
  std::vector<char> elf_data;

  public:

    enum class DRIVER_DLLESPEC buffer_type {
      blob_instr_dpu,
      blob_instr_prepost,
      blob_instr_transaction,
      blob_control_packet,
#ifdef AIEBU_FULL
      asm_aie2ps,
#endif
      asm_aie2
    };

  private:
    const buffer_type _type;

  public:
    /*
     * Constructor takes buffer type , 2 buffer and a vector of symbols with
     * external_buffer_id json as argument.
     * its throws aiebu::error object.
     * User may pass any combination like
     * 1. type as blob_instr_transaction, buffer1 as instruction buffer
     *    and buffer2 as control_packet and pm_ctrlpkt as map of <pm_ctrlpkt_ID, pm_ctrlpkt_buf>
     *    : in this case it will package buffers in text section, data section and
     *    ctrlpkt_pm_N section of elf respectively.
     * 2. type as blob_instr_transaction, buffer1 as instruction buffer
     *    and buffer2 as empty and and pm_ctrlpkt as map of <pm_ctrlpkt_ID, pm_ctrlpkt_buf>
     *    : in this case it will package buffer in text section and ctrlpkt_pm_N section of elf respectively.
     * 3. type as asm_aie2ps, buffer1 as asm buffer and buffer2
     *    as empty: in this case it will assemble the asm code and package in elf.
     *
     * @type           buffer type
     * @instr_buf      first buffer
     * @constrol_buf   second buffer
     * @patch_json     external_buffer_id json
     * @libs           libs to include in elf
     * @libpaths       paths to search for libs
     * @ctrlpkt        map of pm id and pm control packet buffer
     */
     DRIVER_DLLESPEC
     aiebu_assembler(buffer_type type,
               const std::vector<char>& buffer1,
               const std::vector<char>& buffer2,
               const std::vector<char>& patch_json,
               const std::vector<std::string>& libs = {},
               const std::vector<std::string>& libpaths = {},
               const std::map<uint8_t, std::vector<char> >& pm_ctrlpkt = {});

    /*
     * Constructor takes buffer type, buffer,
     * and a vector of symbols with their patching information as argument.
     * its throws aiebu::error object.
     *
     * @type           buffer type
     * @instr_buf      first buffer
     * @libs           libs to include in elf
     * @libpaths       paths to search for libs
     * @patch_json     external_buffer_id json
     */
    DRIVER_DLLESPEC
    aiebu_assembler(buffer_type type,
              const std::vector<char>& buffer,
              const std::vector<std::string>& libs = {},
              const std::vector<std::string>& libpaths = {},
              const std::vector<char>& patch_json = {});

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
    DRIVER_DLLESPEC
    std::vector<char>
    get_elf() const;

    void
    DRIVER_DLLESPEC
    get_report(std::ostream &stream) const;

    void
    DRIVER_DLLESPEC
    disassemble(const std::filesystem::path &root) const;
};

} //namespace aiebu

#endif // _AIEBU_ASSEMBLER_H_
