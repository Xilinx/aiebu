// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
#ifndef AIEBU_DISASSEMBLER_H_
#define AIEBU_DISASSEMBLER_H_

#include <memory>
#include <string>
#include "elfio/elfio.hpp"
#include "common/disassembler_state.h"
#include "specification/aie2ps/isa.h"
#include "ops/ops.h"
#include "writer.h"

namespace aiebu {

class asm_disassembler {
public:
    asm_disassembler(const std::string& input_elf_path, std::ostream& output_stream);
    ~asm_disassembler() = default;

    // Delete copy constructor and copy assignment operator
    asm_disassembler(const asm_disassembler&) = delete;
    asm_disassembler& operator=(const asm_disassembler&) = delete;

    // Delete move constructor and move assignment operator
    asm_disassembler(asm_disassembler&&) = delete;
    asm_disassembler& operator=(asm_disassembler&&) = delete;

    void run();

private:
    ELFIO::elfio m_elf_reader;
    asm_writer m_asm_writer;
    const std::map<uint8_t, isa_op_disasm>* isa_op_map;
    isa_disassembler isa_disasm;
    
    void process_sections();
    void print_section_info(const ELFIO::section* section) ;
    void process_text_section(const ELFIO::section* section, std::shared_ptr<disassembler_state> state);
    void process_data_section(const ELFIO::section* section, std::shared_ptr<disassembler_state> state);
    void process_pad_section(const ELFIO::section* /*section*/, std::shared_ptr<disassembler_state> /*state*/);
    bool is_text_section(const std::string& section_name) const;
    bool is_data_section(const std::string& section_name) const;
};

} // namespace aiebu

#endif // AIEBU_DISASSEMBLER_H_