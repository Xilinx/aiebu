// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
#include "disassembler/disassembler.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace aiebu {

static constexpr size_t ELF_SECTION_HEADER_PADDING = 16;  // ELF-specific header padding
static constexpr uint8_t ALIGN_OPCODE = 0xA5;  // .align pseudo-instruction opcode
static constexpr size_t CTRLTEXT_STRING_LENGTH = 9;  // Length of ".ctrltext"
static constexpr size_t CTRLDATA_STRING_LENGTH = 9;  // Length of ".ctrldata"

asm_disassembler::asm_disassembler(const std::string& input_elf_path, std::ostream& output_stream)
    : m_asm_writer(output_stream), m_is_binary_mode(false) {
    if (!m_elf_reader.load(input_elf_path)) {
        throw error(error::error_code::invalid_elf, "Failed to load ELF:" + input_elf_path + "\n");
    }
    isa_op_map = isa_disasm.get_isa_map();
}

asm_disassembler::asm_disassembler(const std::vector<char>& binary_data, std::ostream& output_stream, bool is_binary)
    : m_asm_writer(output_stream), m_binary_data(binary_data), m_is_binary_mode(is_binary) {
    if (!is_binary) {
        throw error(error::error_code::invalid_input, "Binary mode constructor requires is_binary to be true\n");
    }
    isa_op_map = isa_disasm.get_isa_map();
}

void asm_disassembler::run() {
    if (m_is_binary_mode) {
        process_binary();
    } else {
        process_sections();
    }
}

void asm_disassembler::process_sections() {
    auto state = std::make_shared<disassembler_state>();
    for (const auto& section_ptr : m_elf_reader.sections) {
        const ELFIO::section* section = section_ptr.get();
        const std::string section_name = section->get_name();
        if (section->get_type() != ELFIO::SHT_PROGBITS)
            continue;
        print_section_info(section);
        if (is_text_section(section_name))
            process_text_section(section, state);
        if (is_data_section(section_name)) {
            process_data_section(section, state);
            state->reset();
        }
    }
}

void asm_disassembler::print_section_info(const ELFIO::section* section) {
    std::string flags;
    if (section->get_flags() & ELFIO::SHF_ALLOC)
        flags += "a";
    if (section->get_flags() & ELFIO::SHF_WRITE)
        flags += "w";
    if (section->get_flags() & ELFIO::SHF_EXECINSTR)
        flags += "x";
    m_asm_writer.write_directive("");
    if (is_data_section(section->get_name()))
        m_asm_writer.write_directive("  .ALIGN             " + std::to_string(section->get_addr_align()));
}

void asm_disassembler::process_text_section(const ELFIO::section* section, std::shared_ptr<disassembler_state> state) {
    const char* section_data = section->get_data();
    size_t section_size = section->get_size();
    for (size_t offset = ELF_SECTION_HEADER_PADDING; offset < section_size;) {
        uint8_t opcode = *reinterpret_cast<const uint8_t*>(section_data + offset);
        auto op_it = isa_op_map->find(opcode);
        if (op_it == isa_op_map->end())
            throw error(error::error_code::invalid_asm, "Unknown Opcode:" + std::to_string(opcode) + " at position " + std::to_string(offset) + "\n");
        if (opcode == ALIGN_OPCODE) {
            state->increment_address(1);
            ++offset;
            continue;
        }
        auto deserializer = op_it->second.create_deserializer();
        size_t consumed = deserializer->deserialize(m_asm_writer, state, section_data + offset);
        offset += consumed;
    }
}

void asm_disassembler::process_data_section(const ELFIO::section* section, std::shared_ptr<disassembler_state> state) {
    const char* section_data = section->get_data();
    size_t section_size = section->get_size();
    isa_op_disasm dummy_isa_op("dummy", 0, std::vector<opArg>{});
    bool align_4_written = false;
    for (size_t offset = 0; offset < section_size;) {
        uint8_t opcode = *reinterpret_cast<const uint8_t*>(section_data + offset);
        auto label_map = state->get_labels();
        auto local_ptr_map = state->get_local_ptrs();
        if (label_map.find(state->get_address()) != label_map.end()) {
            ucDmaBd_op_deserializer deserializer(&dummy_isa_op);
            size_t consumed = deserializer.deserialize(m_asm_writer, state, section_data + offset);
            offset += consumed;
        } else if (local_ptr_map.find(state->get_address()) != local_ptr_map.end()) {
            if (!align_4_written) {
                m_asm_writer.write_directive("");
                m_asm_writer.write_directive("  .ALIGN             4");
                align_4_written = true;
            }
            long_op_deserializer deserializer(&dummy_isa_op);
            size_t consumed = deserializer.deserialize(m_asm_writer, state, section_data + offset);
            offset += consumed;
        } else if (opcode == align || opcode == 0) {
            state->increment_address(1);
            ++offset;
        } else {
            throw error(error::error_code::invalid_asm, "Illegal state at position " + std::to_string(opcode) + "\n");
        }
    }
}

void asm_disassembler::process_pad_section(const ELFIO::section* /*section*/, std::shared_ptr<disassembler_state> /*state*/) {
    std::cout << "Dumping .pad not supported\n";
}

bool asm_disassembler::is_text_section(const std::string& section_name) const {
    bool result = section_name.substr(0, CTRLTEXT_STRING_LENGTH) == ".ctrltext";
    return result;
}

bool asm_disassembler::is_data_section(const std::string& section_name) const {
    bool result = section_name.substr(0, CTRLDATA_STRING_LENGTH) == ".ctrldata";
    return result;
}

void asm_disassembler::process_binary() {
    if (m_binary_data.empty()) {
        throw error(error::error_code::invalid_input, "Binary data is empty\n");
    }
    
    // Binary files contain multiple pages, each exactly PAGE_SIZE (8192) bytes
    // Each page has: 16-byte header + content (up to cur_page_len) + padding to 8192
    static constexpr size_t PAGE_SIZE = 8192;
    static constexpr size_t PAGE_HEADER_SIZE = 16;
    
    size_t offset = 0;
    int page_num = 0;
    
    while (offset < m_binary_data.size()) {
        // Check if there's enough data for a page
        size_t remaining = m_binary_data.size() - offset;
        if (remaining < PAGE_HEADER_SIZE) {
            break; // Not enough data for another page
        }
        
        // Check for page header (0xFF 0xFF magic bytes)
        if (static_cast<uint8_t>(m_binary_data[offset]) != 0xFF ||
            static_cast<uint8_t>(m_binary_data[offset + 1]) != 0xFF) {
            // No more pages
            break;
        }
        
        // Read cur_page_len from header (bytes 8-9)
        uint16_t cur_page_len = static_cast<uint8_t>(m_binary_data[offset + 8]) | 
                               (static_cast<uint8_t>(m_binary_data[offset + 9]) << 8);
        
        // cur_page_len includes the header itself, so content is (cur_page_len - PAGE_HEADER_SIZE)
        size_t content_size = (cur_page_len > PAGE_HEADER_SIZE) ? 
                             (cur_page_len - PAGE_HEADER_SIZE) : 0;
        
        // Process this page
        auto state = std::make_shared<disassembler_state>();
        
        if (content_size > 0) {
            process_binary_data(m_binary_data.data() + offset + PAGE_HEADER_SIZE, 
                               content_size, state);
        }
        
        // Move to next page (always at PAGE_SIZE boundary)
        offset += PAGE_SIZE;
        page_num++;
    }
    
    if (page_num == 0) {
        throw error(error::error_code::invalid_input, 
                   "No valid pages found in binary file\n");
    }
}

size_t asm_disassembler::detect_binary_header_offset() const {
    // Check for various binary formats and determine header size
    if (m_binary_data.size() < 16) {
        return 0; // Too small for any header
    }
    
    // Check for AIE2PS/AIE4 page header (16 bytes)
    // Page header format: { 0xFF, 0xFF, page_index[2], ooo_len1[2], ooo_len2[2], 
    //                       cur_len[2], in_order_len[2], reserved[4] }
    if (static_cast<uint8_t>(m_binary_data[0]) == 0xFF && 
        static_cast<uint8_t>(m_binary_data[1]) == 0xFF) {
        return 16;  // Skip 16-byte page header
    }
    
    // Check if this looks like ELF section padding (all zeros or alignment padding)
    if (m_binary_data.size() >= 16) {
        bool looks_like_padding = true;
        for (size_t i = 0; i < 16; i++) {
            if (m_binary_data[i] != 0 && m_binary_data[i] != static_cast<char>(0xA5)) {
                looks_like_padding = false;
                break;
            }
        }
        if (looks_like_padding) {
            return 16;  // Skip padding
        }
    }
    
    // No header detected, start from beginning
    return 0;
}

void asm_disassembler::process_binary_data(const char* data, size_t size, std::shared_ptr<disassembler_state> state) {
    // Process PAGE structure: TEXT section → EOF → DATA section → padding
    // This mirrors how ELF sections are processed
    
    size_t text_end_offset = 0;
    bool found_eof = false;
    
    // Find EOF to determine TEXT section boundary
    for (size_t i = 0; i < size; i++) {
        if (static_cast<uint8_t>(data[i]) == 0xFF) {
            // Check if this is actually an EOF opcode (not just 0xFF in data)
            auto op_it = isa_op_map->find(0xFF);
            if (op_it != isa_op_map->end()) {
                text_end_offset = i + 4; // EOF is 4 bytes (opcode + pad + pad)
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
    m_asm_writer.write_directive("");
    m_asm_writer.write_directive(";");
    m_asm_writer.write_directive("; Code");
    m_asm_writer.write_directive(";");
    m_asm_writer.write_directive("");
    
    size_t offset = 0;
    while (offset < text_end_offset) {
        uint8_t opcode = *reinterpret_cast<const uint8_t*>(data + offset);
        
        // Handle alignment padding
        if (opcode == ALIGN_OPCODE) {
            state->increment_address(1);
            ++offset;
            continue;
        }
        
        auto op_it = isa_op_map->find(opcode);
        if (op_it == isa_op_map->end()) {
            std::ostringstream err;
            err << "Unknown Opcode: 0x" << std::hex << static_cast<int>(opcode) 
                << " at offset " << std::dec << offset << "\n";
            throw error(error::error_code::invalid_asm, err.str());
        }
        
        auto deserializer = op_it->second.create_deserializer();
        size_t consumed = deserializer->deserialize(m_asm_writer, state, data + offset);
        offset += consumed;
    }
    
    // Process DATA section if present (mirroring process_data_section for ELF)
    if (found_eof && text_end_offset < size) {
        m_asm_writer.write_directive("");
        m_asm_writer.write_directive(";");
        m_asm_writer.write_directive("; Data");
        m_asm_writer.write_directive(";");
        m_asm_writer.write_directive("");
        m_asm_writer.write_directive("  .ALIGN             16");
        
        process_data_section_binary(data + text_end_offset, size - text_end_offset, state);
        
        // Reset state after DATA section (like ELF disassembler does)
        state->reset();
    }
}

void asm_disassembler::process_data_section_binary(const char* data, size_t size, std::shared_ptr<disassembler_state> state) {
    // Process DATA section - mirrors ELF process_data_section logic
    isa_op_disasm dummy_isa_op("dummy", 0, std::vector<opArg>{});
    size_t offset = 0;
    bool align_4_written = false;
    
    while (offset < size) {
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
                m_asm_writer.write_directive("  .ALIGN             4");
                align_4_written = true;
            }
            long_op_deserializer deserializer(&dummy_isa_op);
            size_t consumed = deserializer.deserialize(m_asm_writer, state, data + offset);
            offset += consumed;
        } 
        // Handle alignment and padding bytes (most common case in DATA section)
        else if (opcode == ALIGN_OPCODE || opcode == 0) {
            state->increment_address(1);
            ++offset;
        } 
        // Unknown byte - this shouldn't happen in a valid binary
        // Just skip it silently (it's likely padding we don't recognize)
        else {
            state->increment_address(1);
            ++offset;
        }
    }
}
} // namespace aiebu
