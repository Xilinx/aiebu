// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
#include "disassembler/disassembler.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace aiebu {

constexpr size_t ELF_SECTION_HEADER_PADDING = 16;  // ELF-specific header padding
constexpr uint8_t ALIGN_OPCODE = 0xA5;  // .align pseudo-instruction opcode

asm_disassembler::asm_disassembler(const std::string& input_elf_path, std::ostream& output_stream)
    : asm_write(output_stream) {
    if (!elf_reader.load(input_elf_path)) {
        throw std::runtime_error("Failed to load ELF: " + input_elf_path);
    }
    isa_disasm = new isa_disassembler();
    isa_op_map = isa_disasm->get_isa_map();
}

void asm_disassembler::run() {
    process_sections();
}

void asm_disassembler::process_sections() {
    auto state = std::make_shared<disassembler_state>();
    for (const auto& section_ptr : elf_reader.sections) {
        const ELFIO::section* section = section_ptr.get();
        std::string section_name = section->get_name();
        if (section->get_type() != ELFIO::SHT_PROGBITS) {
            continue;
        }
        print_section_info(section);
        if (is_text_section(section_name)) {
            process_text_section(section, state);
        } else if (is_data_section(section_name)) {
            process_data_section(section, state);
            state->reset();
        } else {
            std::cout << "Section not recognized as text or data: " << section_name << std::endl;
        }
    }
}

void asm_disassembler::print_section_info(const ELFIO::section* section) {
    std::string flags;
    if (section->get_flags() & ELFIO::SHF_ALLOC) flags += "a";
    if (section->get_flags() & ELFIO::SHF_WRITE) flags += "w";
    if (section->get_flags() & ELFIO::SHF_EXECINSTR) flags += "x";
    asm_write.write_directive("");
    if (is_data_section(section->get_name())) {
        asm_write.write_directive(".align " + std::to_string(section->get_addr_align()));
    }
}

void asm_disassembler::process_text_section(const ELFIO::section* section, std::shared_ptr<disassembler_state> state) {
    const char* section_data = section->get_data();
    size_t section_size = section->get_size();
    for (size_t offset = 16; offset < section_size;) {
        uint8_t opcode = *reinterpret_cast<const uint8_t*>(section_data + offset);
        auto op_it = isa_op_map->find(opcode);
        if (op_it != isa_op_map->end()) {
            if (opcode == ALIGN_OPCODE) {
                state->increment_address(1);
                ++offset;
                continue;
            }
            auto deserializer = op_it->second.create_deserializer();
            size_t consumed = deserializer->deserialize(asm_write, state, section_data + offset);
            offset += consumed;
        } else {
            std::cerr << "[DEBUG] Unknown opcode " << static_cast<int>(opcode) << " at position " << offset << "\n";
            ++offset;
        }
    }
}

void asm_disassembler::process_data_section(const ELFIO::section* section, std::shared_ptr<disassembler_state> state) {
    const char* section_data = section->get_data();
    size_t section_size = section->get_size();
    isa_op_disasm dummy_isa_op("dummy", 0, std::vector<opArg>{});
    for (size_t offset = 0; offset < section_size;) {
        uint8_t opcode = *reinterpret_cast<const uint8_t*>(section_data + offset);
        auto label_map = state->get_labels();
        auto local_ptr_map = state->get_local_ptrs();
        if (label_map.find(state->get_address()) != label_map.end()) {
            ucDmaBd_op_deserializer deserializer(&dummy_isa_op);
            size_t consumed = deserializer.deserialize(asm_write, state, section_data + offset);
            offset += consumed;
        } else if (local_ptr_map.find(state->get_address()) != local_ptr_map.end()) {
            asm_write.write_directive(".align 4");
            long_op_deserializer deserializer(&dummy_isa_op);
            size_t consumed = deserializer.deserialize(asm_write, state, section_data + offset);
            offset += consumed;
        } else if (opcode == align) {
            state->increment_address(1);
            ++offset;
        } else if (opcode == 0) {
            state->increment_address(1);
            ++offset;
        } else {
            std::cerr << "[DEBUG] Illegal state at position " << opcode << std::endl;
            throw std::runtime_error("Illegal state at position " + std::to_string(opcode));
        }
    }
}

void asm_disassembler::process_pad_section(const ELFIO::section* /*section*/, std::shared_ptr<disassembler_state> /*state*/) {
    std::cout << "[DEBUG] Dumping .pad not supported\n";
}

bool asm_disassembler::is_text_section(const std::string& section_name) const {
    bool result = section_name.substr(0, 9) == ".ctrltext";
    return result;
}

bool asm_disassembler::is_data_section(const std::string& section_name) const {
    bool result = section_name.substr(0, 9) == ".ctrldata";
    return result;
}

asm_disassembler::~asm_disassembler() {
    delete isa_disasm;
}

} // namespace aiebu
