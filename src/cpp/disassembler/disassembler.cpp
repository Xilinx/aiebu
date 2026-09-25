// SPDX-License-Identifier: MIT
// Copyright (C) 2025-2026, Advanced Micro Devices, Inc. All rights reserved.
#include "disassembler/disassembler.h"
#include "disassembler/disassembler_merged.h"
#include "elf/aie_elf_constants.h"
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <optional>
#include <set>

namespace aiebu {

namespace {

bool
elf_uses_target_directive(unsigned char abi_version)
{
  return abi_version >= elf_version_config_v1 ||
         abi_version == elf_version_aie2p_config;
}

std::optional<std::string>
os_abi_to_target_name(unsigned char os_abi)
{
  switch (os_abi) {
  case osabi_aie2ps:
    return "aie2ps";
  case osabi_aie4:
    return "aie4";
  case osabi_aie4a:
    return "aie4a";
  case osabi_aie4z:
    return "aie4z";
  case osabi_aie2p:
    return "aie2p";
  default:
    return std::nullopt;
  }
}

std::optional<std::string>
target_from_buffer_type(aiebu_assembler::buffer_type buffer_type)
{
  using bt = aiebu_assembler::buffer_type;
  switch (buffer_type) {
  case bt::elf_aie4:
  case bt::elf_aie4_config:
  case bt::blob_aie4:
    return "aie4";
  case bt::elf_aie4a:
  case bt::elf_aie4a_config:
  case bt::blob_aie4a:
    return "aie4a";
  case bt::elf_aie4z:
  case bt::elf_aie4z_config:
  case bt::blob_aie4z:
    return "aie4z";
  case bt::elf_aie2ps:
  case bt::elf_aie2ps_config:
  case bt::blob_aie2ps:
    return "aie2ps";
  case bt::elf_aie2:
    return "aie2p";
  default:
    return std::nullopt;
  }
}

std::optional<std::string>
resolve_disasm_target(unsigned char os_abi, unsigned char abi_version,
                      aiebu_assembler::buffer_type buffer_type)
{
  if (!elf_uses_target_directive(abi_version))
    return std::nullopt;

  if (auto name = os_abi_to_target_name(os_abi))
    return name;

  // Config ELFs that still use legacy group OSABI — infer from buffer type / -m.
  if (os_abi == osabi_aie2ps_group)
    return target_from_buffer_type(buffer_type);

  return std::nullopt;
}

} // namespace

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
static constexpr size_t page_header_page_idx_low = 2;         // Page index low byte offset
static constexpr size_t page_header_page_idx_high = 3;      // Page index high byte offset
static constexpr size_t page_header_in_order_len_low = 10;  // In-order page length low byte offset
static constexpr size_t page_header_in_order_len_high = 11; // In-order page length high byte offset

// Magic Values
static constexpr uint8_t page_header_magic = 0xFF;          // Page header magic byte value
static constexpr uint8_t align_opcode = 0xA5;               // .align pseudo-instruction opcode
static constexpr uint8_t eof_opcode = 0xFF;                 // End-of-file opcode
static constexpr uint8_t zero_padding = 0x00;               // Zero padding byte

// Opcode Sizes
static constexpr size_t eof_size = 4;                       // EOF instruction size in bytes
static constexpr size_t align_4 = 4;                        // 4-byte alignment
static constexpr size_t align_16 = 16;                      // 16-byte alignment
static constexpr size_t preempt_insn_size = 8;              // PREEMPT instruction size in bytes
static constexpr size_t min_insn_size = 2;                  // Minimum instruction size in bytes
static constexpr size_t bits_per_byte = 8;                  // Bits per byte for width conversion
static constexpr size_t hex_word_width = 8;                 // Hex digit width for .long emission

// Instruction Opcodes
static constexpr uint8_t preempt_opcode = 0x19;             // PREEMPT opcode

// Section Name Lengths
static constexpr size_t ctrltext_string_length = 9;        // Length of ".ctrltext"
static constexpr size_t ctrldata_string_length = 9;        // Length of ".ctrldata"

// Bit Shift Constants
static constexpr size_t byte_shift = 8;                    // Bit shift for byte-to-word conversion

// Base class constructor
asm_disassembler::asm_disassembler(std::ostream& output_stream)
    : m_asm_writer(output_stream), m_buffer_type(aiebu_assembler::buffer_type::unspecified) {
    isa_op_map = isa_disasm.get_isa_map();
}

// Create architecture-specific disassembler state based on buffer_type
std::shared_ptr<disassembler_state> asm_disassembler::create_disassembler_state() const {
    switch (m_buffer_type) {
        case aiebu_assembler::buffer_type::elf_aie4:
        case aiebu_assembler::buffer_type::blob_aie4:
            return std::make_shared<disassembler_state_aie4>();
        default:
            // Default to aie2ps for aie2ps, aie2, aie2p, or unknown architectures
            return std::make_shared<disassembler_state_aie2ps>();
    }
}

// Common helper to write text section header
// Use .section directive so assembler switches to text mode after EOF
void asm_disassembler::add_text_sec_comment() {
    m_asm_writer.write_directive("");
    m_asm_writer.write_directive(".section .ctrltext");
    m_asm_writer.write_directive("");
}

// Common helper to write data section header
// Use .section directive for consistency (assembler switches to data mode after EOF anyway)
void asm_disassembler::add_data_sec_comment() {
    m_asm_writer.write_directive("");
    m_asm_writer.write_directive(".section .ctrldata");
    m_asm_writer.write_directive("");
}

// Common text block processing (used by both ELF and binary disassemblers)
void asm_disassembler::process_text_block(const char* data, size_t start_offset, size_t end_offset,
                                         std::shared_ptr<disassembler_state> state) {
    for (size_t offset = start_offset; offset < end_offset;) {
        state->set_current_text_offset(static_cast<uint32_t>(offset - start_offset));
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
                                         std::shared_ptr<disassembler_state> state,
                                         bool section_align_emitted) {
    isa_op_disasm dummy_isa_op("dummy", 0, std::vector<opArg>{});
    bool align_4_written = false;
    bool skip_bd_align = section_align_emitted;

    for (size_t offset = 0; offset < size;) {
        uint8_t opcode = *reinterpret_cast<const uint8_t*>(data + offset);
        auto label_map = state->get_labels();
        auto local_ptr_map = state->get_local_ptrs();

        // Check for UC_DMA_BD at label positions
        if (label_map.find(state->get_address()) != label_map.end()) {
            if (!skip_bd_align) {
                m_asm_writer.write_directive("");
                m_asm_writer.write_directive("  .align             " + std::to_string(align_16));
            }
            skip_bd_align = false;
            align_4_written = false;
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
        else if (opcode == align_opcode || opcode == zero_padding) {
            state->increment_address(1);
            ++offset;
        }
        else {
            // Unknown byte(s) in data region.  Emit .long only for a full 4-byte
            // run or the final partial word at section end; always advance the
            // tracked address by 4 when a .long is emitted so it matches the
            // reassembled output size.
            auto is_special_at = [&](uint32_t addr) {
                return label_map.find(addr) != label_map.end() ||
                       local_ptr_map.find(addr) != local_ptr_map.end();
            };

            size_t run = 0;
            while (run < align_4 && offset + run < size) {
                const uint32_t addr = state->get_address() + static_cast<uint32_t>(run);
                if (is_special_at(addr))
                    break;
                const auto byte = static_cast<uint8_t>(data[offset + run]);
                if (byte == align_opcode || byte == zero_padding)
                    break;
                ++run;
            }

            if (run == 0) {
                state->increment_address(1);
                ++offset;
                continue;
            }

            const size_t avail = size - offset;
            const bool section_tail = (offset + align_4 > size);
            const bool emit_long = (run == align_4) || (section_tail && run == avail);

            if (emit_long) {
                if (!align_4_written) {
                    m_asm_writer.write_directive("");
                    m_asm_writer.write_directive("  .align             " + std::to_string(align_4));
                    align_4_written = true;
                }
                uint32_t word = 0;
                std::memcpy(&word, data + offset, run);
                std::ostringstream hex;
                hex << "  .long              0x" << std::hex << std::setw(hex_word_width)
                    << std::setfill('0') << word;
                m_asm_writer.write_directive(hex.str());
                state->increment_address(align_4);
                offset += run;
            } else {
                // Short run before the next label/structure — skip without .long
                // so subsequent label addresses stay aligned.
                state->increment_address(static_cast<uint32_t>(run));
                offset += run;
            }
        }
    }
}

// ELF disassembler constructor
elf_asm_disassembler::elf_asm_disassembler(const std::string& input_elf_path, std::ostream& output_stream,
                                          aiebu_assembler::buffer_type buffer_type)
    : asm_disassembler(output_stream) {
    if (!m_elf_reader.load(input_elf_path)) {
        throw error(error::error_code::invalid_elf, "Failed to load ELF:" + input_elf_path + "\n");
    }
    m_buffer_type = buffer_type;
}

// ELF disassembler constructor from input stream
elf_asm_disassembler::elf_asm_disassembler(std::istream& input_stream, std::ostream& output_stream,
                                          aiebu_assembler::buffer_type buffer_type)
    : asm_disassembler(output_stream) {
    if (!m_elf_reader.load(input_stream)) {
        throw error(error::error_code::invalid_elf, "Failed to load ELF from input stream\n");
    }
    m_buffer_type = buffer_type;
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
    m_buffer_type = buffer_type;

    // Output target architecture information for binary files
    std::string arch_name = (buffer_type == aiebu_assembler::buffer_type::blob_aie4) ? "aie4" : "aie2ps";
    m_asm_writer.write_directive("; Target Architecture: " + arch_name);
}

// Binary disassembler run method
void bin_asm_disassembler::run() {
    process_binary();
}

uint16_t elf_asm_disassembler::read_page_in_order_len(const char* page_data) {
    if (static_cast<uint8_t>(page_data[page_header_magic_byte_0]) != page_header_magic ||
        static_cast<uint8_t>(page_data[page_header_magic_byte_1]) != page_header_magic) {
        return 0;
    }
    return static_cast<uint8_t>(page_data[page_header_in_order_len_low]) |
           (static_cast<uint8_t>(page_data[page_header_in_order_len_high]) << byte_shift);
}

uint16_t elf_asm_disassembler::read_page_index_from_header(const char* page_data) {
    return static_cast<uint8_t>(page_data[page_header_page_idx_low]) |
           (static_cast<uint8_t>(page_data[page_header_page_idx_high]) << byte_shift);
}

static uint16_t get_in_order_page_len(const ELFIO::section* section) {
    if (section->get_size() < elf_section_header_padding)
        return 0;
    const char* data = section->get_data();
    if (static_cast<uint8_t>(data[page_header_magic_byte_0]) != page_header_magic ||
        static_cast<uint8_t>(data[page_header_magic_byte_1]) != page_header_magic)
        return 0;
    return static_cast<uint8_t>(data[page_header_in_order_len_low]) |
           (static_cast<uint8_t>(data[page_header_in_order_len_high]) << byte_shift);
}

// Parse column number from section name and update current_column if changed
// Parse column number from section name (e.g., ".ctrltext.0.1" -> 0)
// Section names have column number between first and second dots
// Returns the parsed column number, or -1 if parsing fails
int elf_asm_disassembler::parse_section_column(const std::string& section_name) {
    size_t first_dot = section_name.find('.', 1);
    if (first_dot != std::string::npos) {
        size_t second_dot = section_name.find('.', first_dot + 1);
        if (second_dot != std::string::npos) {
            std::string col_str = section_name.substr(first_dot + 1, second_dot - first_dot - 1);
            try {
                return std::stoi(col_str);
            } catch (...) { /* ignore parse errors */ }
        }
    }
    return -1;  // Parsing failed
}

void elf_asm_disassembler::process_sections() {
    auto state = create_disassembler_state();

    m_merged_ctx = merged_disasm_context::build(m_elf_reader, isa_op_map,
                                                m_elf_reader.get_abi_version());
    if (m_merged_ctx.is_active())
        state->set_merged_context(std::make_shared<merged_disasm_context>(m_merged_ctx));

    // Count unique columns to determine partition size
    std::set<int> columns;
    int first_column = 0;
    for (const auto& section_ptr : m_elf_reader.sections) {
        const ELFIO::section* section = section_ptr.get();
        const std::string section_name = section->get_name();
        if (section->get_type() != ELFIO::SHT_PROGBITS)
            continue;
        if (is_text_section(section_name)) {
            // Parse column from section name like ".ctrltext.0.1" -> column 0
            int col = parse_section_column(section_name);
            if (col != -1) {
                if (columns.empty()) first_column = col;
                columns.insert(col);
            }
        }
    }

    const unsigned char os_abi = m_elf_reader.get_os_abi();
    const unsigned char abi_version = m_elf_reader.get_abi_version();
    if (const auto target = resolve_disasm_target(os_abi, abi_version, m_buffer_type))
        m_asm_writer.write_target(*target);

    // Emit partition directive (number of columns)
    if (!columns.empty()) {
        m_asm_writer.write_partition(std::to_string(columns.size()) + "column");
    }

    // Emit initial attach_to_group
    m_asm_writer.write_attach_to_group(first_column);

    int current_column = first_column;
    int page_counter = 0;  // Track page number for unique labels
    std::string current_page_label;  // Track current page label for .endl
    uint16_t prev_in_order_page_len = 0;  // Track previous page's in_order_page_len

    for (const auto& section_ptr : m_elf_reader.sections) {
        const ELFIO::section* section = section_ptr.get();
        const std::string section_name = section->get_name();
        if (section->get_type() != ELFIO::SHT_PROGBITS)
            continue;

        // Check for column change in text sections
        if (is_text_section(section_name)) {
            int section_col = parse_section_column(section_name);
            if (section_col != -1 && section_col != current_column) {
                m_asm_writer.write_attach_to_group(section_col);
                current_column = section_col;
                state->set_current_col(section_col);
                state->reset_column_state();
            }
        }

        print_section_info(section);
        if (is_text_section(section_name)) {
            // Note: add_text_sec_comment() is called inside process_text_section()
            // for merged-page sections (once per page).  For single-page sections
            // we emit it here so label/OOO logic below can still interleave correctly.
            const bool sec_is_merged =
                (m_elf_reader.get_abi_version() >= elf_version_config);  // 0x21+ = merged-page

            if (sec_is_merged) {
                state->set_current_col(current_column);
                process_merged_text_section(section, state, current_column, page_counter,
                                            current_page_label, prev_in_order_page_len);
            } else {
                if (!sec_is_merged)
                    add_text_sec_comment();

                uint16_t current_in_order_len = get_in_order_page_len(section);

                if (page_counter > 0 && prev_in_order_page_len == 0) {
                    if (state->has_pending_ooo_labels()) {
                        std::string ooo_label = state->get_next_ooo_label();
                        if (!ooo_label.empty() && ooo_label.front() == '@')
                            current_page_label = ooo_label.substr(1);
                        else
                            current_page_label = ooo_label;
                        m_asm_writer.write_label(ooo_label);
                    } else {
                        std::ostringstream oss;
                        oss << "zpage_" << std::setw(4) << std::setfill('0') << page_counter;
                        current_page_label = oss.str();
                        m_asm_writer.write_label(current_page_label);
                    }
                }

                process_text_section(section, state);
                prev_in_order_page_len = current_in_order_len;
                page_counter++;
            }
        }
        if (is_data_section(section_name)) {
            add_data_sec_comment();
            process_data_section(section, state);
            state->reset();
            // Emit .endl to close the current page label scope when this scope ends
            // The scope ends when in_order_page_len was 0 (no continuation to next page)
            if (!current_page_label.empty() && prev_in_order_page_len == 0) {
                m_asm_writer.write_directive(".endl " + current_page_label);
                current_page_label.clear();
            }
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

void elf_asm_disassembler::emit_hintmap_data(const merged_disasm_context::preempt_point& pt) {
    if (!pt.has_hintmap || pt.hintmap_words.empty())
        return;

    m_asm_writer.write_directive("  .align             " + std::to_string(align_4));
    m_asm_writer.write_label(pt.hintmap_label);
    for (const uint32_t word : pt.hintmap_words) {
        std::ostringstream hex;
        hex << "  .long              0x" << std::hex << std::setw(hex_word_width)
            << std::setfill('0') << word;
        m_asm_writer.write_directive(hex.str());
    }
}

void elf_asm_disassembler::process_merged_text_section(const ELFIO::section* section,
                                                       std::shared_ptr<disassembler_state> state,
                                                       int col, int& page_counter,
                                                       std::string& current_page_label,
                                                       uint16_t& prev_in_order_page_len) {
    const char* section_data = section->get_data();
    const size_t section_size = section->get_size();

    for (size_t page_start = 0; page_start < section_size; page_start += page_size) {
        const size_t avail = section_size - page_start;
        if (avail < page_header_size)
            break;

        const char* page_data = section_data + page_start;
        if (static_cast<uint8_t>(page_data[page_header_magic_byte_0]) != page_header_magic ||
            static_cast<uint8_t>(page_data[page_header_magic_byte_1]) != page_header_magic)
            break;

        const uint16_t page_idx = read_page_index_from_header(page_data);
        const uint16_t current_in_order_len = read_page_in_order_len(page_data);

        if (m_merged_ctx.should_skip_page(col, page_idx)) {
            prev_in_order_page_len = current_in_order_len;
            ++page_counter;
            continue;
        }

        add_text_sec_comment();

        // New label scope when previous page ended its in-order span.
        if (page_counter > 0 && prev_in_order_page_len == 0) {
            std::string ooo_label = state->take_external_page_label(page_idx);
            if (ooo_label.empty() && state->has_pending_ooo_labels())
                ooo_label = state->get_next_ooo_label();
            if (ooo_label.empty()) {
                const std::string mapped = m_merged_ctx.pending_page_label(col, page_idx);
                if (!mapped.empty())
                    ooo_label = "@" + mapped;
            }

            if (!ooo_label.empty()) {
                if (ooo_label.front() == '@')
                    current_page_label = ooo_label.substr(1);
                else
                    current_page_label = ooo_label;
                m_asm_writer.write_label(ooo_label);
            } else {
                std::ostringstream oss;
                oss << "zpage_" << std::setw(4) << std::setfill('0') << page_counter;
                current_page_label = oss.str();
                m_asm_writer.write_label(current_page_label);
            }
        }

        const size_t code_start = page_start + elf_section_header_padding;
        const size_t page_end = page_start + page_size;

        size_t code_end = page_end;
        for (size_t i = code_start; i < page_end;) {
            const auto op = static_cast<uint8_t>(section_data[i]);
            if (op == eof_opcode) {
                code_end = i + eof_size;
                break;
            }
            if (op == align_opcode) { ++i; continue; }
            auto it = isa_op_map->find(op);
            if (it == isa_op_map->end()) break;
            size_t insn_size = min_insn_size;
            for (const auto& arg : it->second.get_args())
                insn_size += static_cast<size_t>(arg.get_width()) / bits_per_byte;
            if (insn_size < min_insn_size) break;
            i += insn_size;
        }

        state->set_current_page_idx(page_idx);
        process_text_block(section_data, code_start, code_end, state);

        const size_t data_start = code_end;
        const size_t data_size = page_end - data_start;
        add_data_sec_comment();
        if (data_size > 0)
            process_data_block(section_data + data_start, data_size, state, false);

        // Emit hintmaps for PREEMPT opcodes on this page into the data section.
        for (size_t pos = code_start; pos < code_end;) {
            if (static_cast<uint8_t>(section_data[pos]) != preempt_opcode) {
                auto it = isa_op_map->find(static_cast<uint8_t>(section_data[pos]));
                if (it == isa_op_map->end()) break;
                size_t insn_size = min_insn_size;
                for (const auto& arg : it->second.get_args())
                    insn_size += static_cast<size_t>(arg.get_width()) / bits_per_byte;
                pos += insn_size;
                continue;
            }
            const auto text_off = static_cast<uint32_t>(pos - code_start);
            if (const auto* pt = m_merged_ctx.preempt_at(col, page_idx, text_off))
                emit_hintmap_data(*pt);
            pos += preempt_insn_size;
        }

        state->reset_page_state();

        if (current_in_order_len == 0 && !current_page_label.empty()) {
            m_asm_writer.write_directive(".endl " + current_page_label);
            current_page_label.clear();
        }

        prev_in_order_page_len = current_in_order_len;
        ++page_counter;
    }
}

void elf_asm_disassembler::process_text_section(const ELFIO::section* section, std::shared_ptr<disassembler_state> state) {
    const char* section_data = section->get_data();
    size_t section_size = section->get_size();

    // Merged-page sections are handled by process_merged_text_section().
    process_text_block(section_data, elf_section_header_padding, section_size, state);
}

void elf_asm_disassembler::process_data_section(const ELFIO::section* section, std::shared_ptr<disassembler_state> state) {
    const char* section_data = section->get_data();
    size_t section_size = section->get_size();
    // Use common base class method for data processing
    process_data_block(section_data, section_size, state, true);
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

        // cur_page_len includes the header itself, so content is (cur_page_len
        // - page_header_size)

        const size_t avail = remaining - page_header_size;
        const size_t content_size = (cur_page_len > page_header_size) ?
            (cur_page_len - page_header_size) : 0;
        if (content_size > avail)
            throw error(error::error_code::invalid_asm,
                        "page_len exceeds remaining bytes");

        // Process this page
        auto state = create_disassembler_state();

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
    process_data_block(data, size, state, true);
}
} // namespace aiebu
