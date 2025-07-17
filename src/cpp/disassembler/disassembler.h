// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
#ifndef AIEBU_DISASSEMBLER_H_
#define AIEBU_DISASSEMBLER_H_

#include <memory>
#include <string>
#include "elfio/elfio.hpp"
#include "common/disassembler_state.h"
#include "specification/aie2ps/isa.h"
#include "writer.h"

namespace aiebu {

class disassembler {
public:
    disassembler(const std::string& elf_path, const std::string& label_path);

    void run();

private:
    void process_sections();
    void print_section_info(const ELFIO::section* section) ;
    void process_text_section(const ELFIO::section* section, std::shared_ptr<disassembler_state> state);
    void process_data_section(const ELFIO::section* section, std::shared_ptr<disassembler_state> state);
    void process_pad_section(const ELFIO::section* /*section*/, std::shared_ptr<disassembler_state> /*state*/);
    bool is_text_section(const std::string& section_name) const;
    bool is_data_section(const std::string& section_name) const;

    ELFIO::elfio elf_reader_;
    ctrl_writer ctrl_writer_;
    std::shared_ptr<std::map<uint8_t, std::shared_ptr<isa_op>>> isa_op_map_;
};

} // namespace aiebu

#endif // AIEBU_DISASSEMBLER_H_
