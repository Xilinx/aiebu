// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
#include "disassembler/disassembler.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace aiebu {

// Part of this code are generated using Cursor.
// ELF and Binary Format Constants
static constexpr size_t elf_section_header_padding = 16;  // ELF-specific header padding
static constexpr size_t page_size = 8192;                   // Binary page size (8KB)
static constexpr size_t page_header_size = 16;              // Page header size in bytes
static constexpr size_t min_header_size = 16;               // Minimum header size for detection

// Page Header Field Offsets
static constexpr size_t page_header_magic_byte_0 = 0;       // First magic byte offset
static constexpr size_t page_header_magic_byte_1 = 1;       // Second magic byte offset
static constexpr size_t page_header_cur_len_low = 8;        // Current page length low byte offset
static constexpr size_t page_header_cur_len_high = 9;       // Current page length high byte offset

// Magic Values
static constexpr uint8_t page_header_magic = 0xFF;          // Page header magic byte value
static constexpr uint8_t align_opcode = 0xA5;               // .align pseudo-instruction opcode
static constexpr uint8_t eof_opcode = 0xFF;                 // End-of-file opcode
static constexpr uint8_t zero_padding = 0x00;               // Zero padding byte

// Opcode Sizes
static constexpr size_t eof_size = 4;                       // EOF instruction size in bytes
static constexpr size_t align_4 = 4;                        // 4-byte alignment
static constexpr size_t align_16 = 16;                      // 16-byte alignment

// Section Name Lengths
static constexpr size_t ctrltext_string_length = 9;        // Length of ".ctrltext"
static constexpr size_t ctrldata_string_length = 9;        // Length of ".ctrldata"

// Bit Shift Constants
static constexpr size_t byte_shift = 8;                    // Bit shift for byte-to-word conversion

// Base class constructor
asm_disassembler::asm_disassembler(std::ostream& output_stream)
    : m_asm_writer(output_stream), m_target_arch("") {
    isa_op_map = isa_disasm.get_isa_map();
}

// Common helper to write text section header
void asm_disassembler::add_text_sec_comment() {
    m_asm_writer.write_directive("");
    m_asm_writer.write_directive(";");
    m_asm_writer.write_directive(";text");
    m_asm_writer.write_directive(";");
    m_asm_writer.write_directive("");
}

// Common helper to write data section header
void asm_disassembler::add_data_sec_comment() {
    m_asm_writer.write_directive("");
    m_asm_writer.write_directive(";");
    m_asm_writer.write_directive(";data");
    m_asm_writer.write_directive(";");
    m_asm_writer.write_directive("");
}

// Common text block processing (used by both ELF and binary disassemblers)
void asm_disassembler::process_text_block(const char* data, size_t start_offset, size_t end_offset, 
                                         std::shared_ptr<disassembler_state> state) {
    for (size_t offset = start_offset; offset < end_offset;) {
        uint8_t opcode = *reinterpret_cast<const uint8_t*>(data + offset);
        
        // Handle alignment padding
        if (opcode == align_opcode) {
            state->increment_address(1);
            ++offset;
            continue;
        }
        
        // Look up opcode in ISA map
        auto op_it = isa_op_map->find(opcode);
        if (op_it == isa_op_map->end()) {
            std::ostringstream err;
            err << "Unknown Opcode: 0x" << std::hex << static_cast<int>(opcode) 
                << " at offset " << std::dec << offset << "\n";
            throw error(error::error_code::invalid_asm, err.str());
        }
        
        // Deserialize and consume bytes
        auto deserializer = op_it->second.create_deserializer();
        size_t consumed = deserializer->deserialize(m_asm_writer, state, data + offset);
        offset += consumed;
    }
}

// Common data block processing (used by both ELF and binary disassemblers)
void asm_disassembler::process_data_block(const char* data, size_t size, 
                                         std::shared_ptr<disassembler_state> state) {
    isa_op_disasm dummy_isa_op("dummy", 0, std::vector<opArg>{});
    bool align_4_written = false;
    
    for (size_t offset = 0; offset < size;) {
        uint8_t opcode = *reinterpret_cast<const uint8_t*>(data + offset);
        auto label_map = state->get_labels();
        auto local_ptr_map = state->get_local_ptrs();
        
        // Check for UC_DMA_BD at label positions
        if (label_map.find(state->get_address()) != label_map.end()) {
            ucDmaBd_op_deserializer deserializer(&dummy_isa_op);
            size_t consumed = deserializer.deserialize(m_asm_writer, state, data + offset);
            offset += consumed;
        } 
        // Check for .long at local pointer positions
        else if (local_ptr_map.find(state->get_address()) != local_ptr_map.end()) {
            if (!align_4_written) {
                m_asm_writer.write_directive("");
                m_asm_writer.write_directive("  .align             " + std::to_string(align_4));
                align_4_written = true;
            }
            long_op_deserializer deserializer(&dummy_isa_op);
            size_t consumed = deserializer.deserialize(m_asm_writer, state, data + offset);
            offset += consumed;
        } 
        // Handle padding bytes
        else if (opcode == align || opcode == zero_padding) {
            state->increment_address(1);
            ++offset;
        } 
        else {
            std::ostringstream err;
            err << "Illegal state at offset " << offset 
                << " (opcode: 0x" << std::hex << static_cast<int>(opcode) << ")\n";
            throw error(error::error_code::invalid_asm, err.str());
        }
    }
}

// ELF disassembler constructor
elf_asm_disassembler::elf_asm_disassembler(const std::string& input_elf_path, std::ostream& output_stream)
    : asm_disassembler(output_stream) {
    if (!m_elf_reader.load(input_elf_path)) {
        throw error(error::error_code::invalid_elf, "Failed to load ELF:" + input_elf_path + "\n");
    }
    // Set target architecture from ELF header (EI_OSABI)
    // For now, default to aie2ps; future: extract from ELF header byte 7
    m_target_arch = "aie2ps";
}

// ELF disassembler run method
void elf_asm_disassembler::run() {
    process_sections();
}

// Binary disassembler constructor
bin_asm_disassembler::bin_asm_disassembler(const std::vector<char>& binary_data, 
                                          std::ostream& output_stream, 
                                          aiebu_assembler::buffer_type buffer_type)
    : asm_disassembler(output_stream), m_binary_data(binary_data) {
    // Set target architecture from buffer type
    switch (buffer_type) {
        case aiebu_assembler::buffer_type::blob_aie2ps:
            m_target_arch = "aie2ps";
            break;
        case aiebu_assembler::buffer_type::blob_aie4:
            m_target_arch = "aie4";
            break;
        default:
            m_target_arch = "unknown";
            break;
    }
    
    // TODO: Load architecture-specific ISA when differences emerge between aie2ps and aie4
    // Currently both use the same ISA specification from specification/aie2ps/isa.h
    // Future implementation:
    //   if (m_target_arch == "aie4") {
    //     // Load aie4-specific ISA from specification/aie4/isa.h
    //     isa_disasm_aie4 aie4_disasm;
    //     isa_op_map = aie4_disasm.get_isa_map();
    //   } else {
    //     // Load aie2ps ISA (default)
    //     isa_op_map = isa_disasm.get_isa_map();
    //   }
    
    // Output target architecture information for binary files
    m_asm_writer.write_directive("; Target Architecture: " + m_target_arch);
}

// Binary disassembler run method
void bin_asm_disassembler::run() {
    process_binary();
}

void elf_asm_disassembler::process_sections() {
    auto state = std::make_shared<disassembler_state>();
    for (const auto& section_ptr : m_elf_reader.sections) {
        const ELFIO::section* section = section_ptr.get();
        const std::string section_name = section->get_name();
        if (section->get_type() != ELFIO::SHT_PROGBITS)
            continue;
        print_section_info(section);
        if (is_text_section(section_name)) {
            add_text_sec_comment();
            process_text_section(section, state);
        }
        if (is_data_section(section_name)) {
            add_data_sec_comment();
            process_data_section(section, state);
            state->reset();
        }
    }
}

void elf_asm_disassembler::print_section_info(const ELFIO::section* section) {
    std::string flags;
    if (section->get_flags() & ELFIO::SHF_ALLOC)
        flags += "a";
    if (section->get_flags() & ELFIO::SHF_WRITE)
        flags += "w";
    if (section->get_flags() & ELFIO::SHF_EXECINSTR)
        flags += "x";
    m_asm_writer.write_directive("");
    if (is_data_section(section->get_name()))
        m_asm_writer.write_directive("  .align             " + std::to_string(section->get_addr_align()));
}

void elf_asm_disassembler::process_text_section(const ELFIO::section* section, std::shared_ptr<disassembler_state> state) {
    const char* section_data = section->get_data();
    size_t section_size = section->get_size();
    // Use common base class method for text processing
    process_text_block(section_data, elf_section_header_padding, section_size, state);
}

void elf_asm_disassembler::process_data_section(const ELFIO::section* section, std::shared_ptr<disassembler_state> state) {
    const char* section_data = section->get_data();
    size_t section_size = section->get_size();
    // Use common base class method for data processing
    process_data_block(section_data, section_size, state);
}

void elf_asm_disassembler::process_pad_section(const ELFIO::section* /*section*/, std::shared_ptr<disassembler_state> /*state*/) {
    std::cout << "Dumping .pad not supported\n";
}

bool elf_asm_disassembler::is_text_section(const std::string& section_name) const {
    bool result = section_name.substr(0, ctrltext_string_length) == ".ctrltext";
    return result;
}

bool elf_asm_disassembler::is_data_section(const std::string& section_name) const {
    bool result = section_name.substr(0, ctrldata_string_length) == ".ctrldata";
    return result;
}

void bin_asm_disassembler::process_binary() {
    if (m_binary_data.empty()) {
        throw error(error::error_code::invalid_input, "Binary data is empty\n");
    }
    
    size_t offset = 0;
    int page_num = 0;
    
    while (offset < m_binary_data.size()) {
        // Check if there's enough data for a page
        size_t remaining = m_binary_data.size() - offset;
        if (remaining < page_header_size) {
            break; // Not enough data for another page
        }
        
        // Check for page header magic bytes
        if (static_cast<uint8_t>(m_binary_data[offset + page_header_magic_byte_0]) != page_header_magic ||
            static_cast<uint8_t>(m_binary_data[offset + page_header_magic_byte_1]) != page_header_magic) {
            // No more pages
            break;
        }
        
        // Read cur_page_len from header
        uint16_t cur_page_len = static_cast<uint8_t>(m_binary_data[offset + page_header_cur_len_low]) | 
                               (static_cast<uint8_t>(m_binary_data[offset + page_header_cur_len_high]) << byte_shift);
        
        // cur_page_len includes the header itself, so content is (cur_page_len - page_header_size)
        size_t content_size = (cur_page_len > page_header_size) ? 
                             (cur_page_len - page_header_size) : 0;
        
        // Process this page
        auto state = std::make_shared<disassembler_state>();
        
        if (content_size > 0) {
            process_binary_data(m_binary_data.data() + offset + page_header_size, 
                               content_size, state);
        }
        
        // Move to next page (always at page_size boundary)
        offset += page_size;
        page_num++;
    }
    
    if (page_num == 0) {
        throw error(error::error_code::invalid_input, 
                   "No valid pages found in binary file\n");
    }
}

size_t bin_asm_disassembler::detect_binary_header_offset() const {
    // Check for various binary formats and determine header size
    if (m_binary_data.size() < min_header_size) {
        return 0; // Too small for any header
    }
    
    // Check for AIE2PS/AIE4 page header
    // Page header format: { 0xFF, 0xFF, page_index[2], ooo_len1[2], ooo_len2[2], 
    //                       cur_len[2], in_order_len[2], reserved[4] }
    if (static_cast<uint8_t>(m_binary_data[page_header_magic_byte_0]) == page_header_magic && 
        static_cast<uint8_t>(m_binary_data[page_header_magic_byte_1]) == page_header_magic) {
        return page_header_size;
    }
    
    // Check if this looks like ELF section padding (all zeros or alignment padding)
    if (m_binary_data.size() >= min_header_size) {
        bool looks_like_padding = true;
        for (size_t i = 0; i < min_header_size; i++) {
            if (m_binary_data[i] != zero_padding && m_binary_data[i] != static_cast<char>(align_opcode)) {
                looks_like_padding = false;
                break;
            }
        }
        if (looks_like_padding) {
            return page_header_size;
        }
    }
    
    // No header detected, start from beginning
    return 0;
}

void bin_asm_disassembler::process_binary_data(const char* data, size_t size, std::shared_ptr<disassembler_state> state) {
    // Process PAGE structure: TEXT section → EOF → DATA section → padding
    // This mirrors how ELF sections are processed
    
    size_t text_end_offset = 0;
    bool found_eof = false;
    
    // Find EOF to determine TEXT section boundary
    for (size_t i = 0; i < size; i++) {
        if (static_cast<uint8_t>(data[i]) == eof_opcode) {
            // Check if this is actually an EOF opcode (not just 0xFF in data)
            auto op_it = isa_op_map->find(eof_opcode);
            if (op_it != isa_op_map->end()) {
                text_end_offset = i + eof_size;
                found_eof = true;
                break;
            }
        }
    }
    
    // If no EOF found, entire file is TEXT section
    if (!found_eof) {
        text_end_offset = size;
    }
    
    // Process TEXT section (mirroring process_text_section for ELF)
    add_text_sec_comment();
    
    // Use common base class method for text processing
    process_text_block(data, 0, text_end_offset, state);
    
    // Process DATA section if present (mirroring process_data_section for ELF)
    if (found_eof && text_end_offset < size) {
        add_data_sec_comment();
        m_asm_writer.write_directive("  .align             " + std::to_string(align_16));
        
        process_data_section_binary(data + text_end_offset, size - text_end_offset, state);
        
        // Reset state after DATA section (like ELF disassembler does)
        state->reset();
    }
}

void bin_asm_disassembler::process_data_section_binary(const char* data, size_t size, std::shared_ptr<disassembler_state> state) {
    // Use common base class method for data processing
    process_data_block(data, size, state);
}
} // namespace aiebu
