// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
#include "disassembler/disassembler.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace aiebu {

disassembler::disassembler(const std::string& elf_path, const std::string& label_path)
    : ctrl_writer_(label_path) {
    if (!elf_reader_.load(elf_path)) {
        throw std::runtime_error("Failed to load ELF: " + elf_path);
    }
    aiebu::isa_disassembler isa_disasm;
    isa_op_map_ = isa_disasm.get_isamap();
}

void disassembler::run() {
    process_sections();
}

void disassembler::process_sections() {
    auto state = std::make_shared<disassembler_state>();
    for (const auto& section_ptr : elf_reader_.sections) {
        const ELFIO::section* section = section_ptr.get();
        std::string section_name = section->get_name();
        std::cout << "SECTION: " << section_name << "\n";
        if (section->get_type() != ELFIO::SHT_PROGBITS)
            continue;
        print_section_info(section);
        if (is_text_section(section_name)) {
            process_text_section(section, state);
        } else if (is_data_section(section_name)) {
            process_data_section(section, state);
            state->reset();
        }
    }
}

void disassembler::print_section_info(const ELFIO::section* section) {
    std::string flags;
    if (section->get_flags() & ELFIO::SHF_ALLOC) flags += "a";
    if (section->get_flags() & ELFIO::SHF_WRITE) flags += "w";
    if (section->get_flags() & ELFIO::SHF_EXECINSTR) flags += "x";
    ctrl_writer_.write_directive("");
    ctrl_writer_.write_directive(".section " + section->get_name() + ",\"" + flags + "\"");
    ctrl_writer_.write_directive(".align " + std::to_string(section->get_addr_align()));
}

void disassembler::process_text_section(const ELFIO::section* section, std::shared_ptr<disassembler_state> state) {
    const char* section_data = section->get_data();
    size_t section_size = section->get_size();
    for (size_t offset = 16; offset < section_size;) {
        uint8_t opcode = *reinterpret_cast<const uint8_t*>(section_data + offset);
        auto op_it = isa_op_map_->find(opcode);
        if (op_it != isa_op_map_->end()) {
            offset += op_it->second->deserializer()->deserialize(ctrl_writer_, state, section_data + offset);
        } else {
            std::cerr << "Unknown opcode " << static_cast<int>(opcode) << " at position " << offset << "\n";
            ++offset;
        }
    }
}

void disassembler::process_data_section(const ELFIO::section* section, std::shared_ptr<disassembler_state> state) {
    const char* section_data = section->get_data();
    size_t section_size = section->get_size();
    auto dummy_isa_op = std::make_shared<isa_op>("dummy", 0, std::vector<opArg>{});
    for (size_t offset = 0; offset < section_size;) {
        uint8_t opcode = *reinterpret_cast<const uint8_t*>(section_data + offset);
        auto label_map = state->get_labels();
        auto local_ptr_map = state->get_local_ptrs();
        if (label_map.find(state->get_address()) != label_map.end()) {
            ucDmaBd_op_deserializer deserializer(dummy_isa_op);
            offset += deserializer.deserialize(ctrl_writer_, state, section_data + offset);
        } else if (local_ptr_map.find(state->get_address()) != local_ptr_map.end()) {
            ctrl_writer_.write_directive(".align 4");
            long_op_deserializer deserializer(dummy_isa_op);
            offset += deserializer.deserialize(ctrl_writer_, state, section_data + offset);
        } else if (opcode == align) {
            state->increment_address(1);
            ++offset;
        } else if (opcode == 0) {
            state->increment_address(1);
            ++offset;
        } else {
            throw std::runtime_error("Illegal state at position " + std::to_string(opcode));
        }
    }
}

void disassembler::process_pad_section(const ELFIO::section* /*section*/, std::shared_ptr<disassembler_state> /*state*/) {
    std::cout << "Dumping .pad not supported\n";
}

bool disassembler::is_text_section(const std::string& section_name) const {
    return section_name.substr(0, 9) == ".ctrltext";
}

bool disassembler::is_data_section(const std::string& section_name) const {
    return section_name.substr(0, 9) == ".ctrldata";
}

} // namespace aiebu