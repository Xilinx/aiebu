// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_DISASSEMBLER_H_
#define AIEBU_DISASSEMBLER_H_

#include <memory>
#include <string>
#include <cstdint>
#include "elfio/elfio.hpp"
#include "common/disassembler_state.h"
#include "specification/aie2ps/isa.h"
#include "ops/ops.h"
#include "writer.h"
#include "aiebu/aiebu_assembler.h"

namespace aiebu {

// Abstract base class for disassemblers
class asm_disassembler {
public:
    virtual ~asm_disassembler() = default;

    // Delete copy constructor and copy assignment operator
    asm_disassembler(const asm_disassembler&) = delete;
    asm_disassembler& operator=(const asm_disassembler&) = delete;

    // Delete move constructor and move assignment operator
    asm_disassembler(asm_disassembler&&) = delete;
    asm_disassembler& operator=(asm_disassembler&&) = delete;

    // Main entry point - to be implemented by derived classes
    virtual void run() = 0;

protected:
    // Protected constructor for derived classes
    explicit asm_disassembler(std::ostream& output_stream);

    // Common members accessible to derived classes
    asm_writer m_asm_writer;
    const std::map<uint8_t, isa_op_disasm>* isa_op_map;
    isa_disassembler isa_disasm;
    aiebu_assembler::buffer_type m_buffer_type;  // Set by derived classes based on their input format

    // Comments marking start of sections
    void add_text_sec_comment();
    void add_data_sec_comment();

    // Common section processing methods
    void process_text_block(const char* data, size_t start_offset, size_t end_offset, 
                           std::shared_ptr<disassembler_state> state);
    void process_data_block(const char* data, size_t size, 
                           std::shared_ptr<disassembler_state> state);
    
    // Create architecture-specific disassembler state
    [[nodiscard]] std::shared_ptr<disassembler_state> create_disassembler_state() const;
};

// ELF disassembler - handles ELF file format
class elf_asm_disassembler : public asm_disassembler {
public:
    // Constructor for ELF input files
    elf_asm_disassembler(const std::string& input_elf_path, std::ostream& output_stream,
                        aiebu_assembler::buffer_type buffer_type = aiebu_assembler::buffer_type::elf_aie2ps);
    
    void run() override;

private:
    ELFIO::elfio m_elf_reader;

    // ELF-specific processing methods
    void process_sections();
    void print_section_info(const ELFIO::section* section);
    void process_text_section(const ELFIO::section* section, std::shared_ptr<disassembler_state> state);
    void process_data_section(const ELFIO::section* section, std::shared_ptr<disassembler_state> state);
    void process_pad_section(const ELFIO::section* /*section*/, std::shared_ptr<disassembler_state> /*state*/);
    bool is_text_section(const std::string& section_name) const;
    bool is_data_section(const std::string& section_name) const;
};

// Binary disassembler - handles raw binary files with architecture specification
class bin_asm_disassembler : public asm_disassembler {
public:
    // Constructor for binary input files
    bin_asm_disassembler(const std::vector<char>& binary_data, 
                        std::ostream& output_stream, 
                        aiebu_assembler::buffer_type buffer_type = aiebu_assembler::buffer_type::blob_aie2ps);
    
    void run() override;

private:
    std::vector<char> m_binary_data;

    // Binary-specific processing methods
    void process_binary();
    void process_binary_data(const char* data, size_t size, std::shared_ptr<disassembler_state> state);
    void process_data_section_binary(const char* data, size_t size, std::shared_ptr<disassembler_state> state);
    size_t detect_binary_header_offset() const;
};

} // namespace aiebu

#endif // AIEBU_DISASSEMBLER_H_
