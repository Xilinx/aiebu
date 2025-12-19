// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.
#include "reporter.h"
#include "packets.h"
#include "transaction.hpp"
#include "transaction.hpp"
#include "common/file_utils.h"
#include "disassembler/disassembler.h"


#include "aiebu/aiebu_error.h"

#include <boost/interprocess/streams/bufferstream.hpp>
#include <elfio/elfio_dump.hpp>
namespace aiebu {

    reporter::reporter(aiebu::aiebu_assembler::buffer_type type, const std::vector<char>& buffer) :
        m_buffer_type(type), m_buffer(buffer)
    {
        if (buffer.empty()) {
            throw error(error::error_code::invalid_input, "Input buffer is empty");
        }
        if (type == aiebu::aiebu_assembler::buffer_type::elf_aie2) {
            boost::interprocess::ibufferstream istr(buffer.data(), buffer.size());
            bool result = my_elf_reader.load(istr);
            if (!result)
                throw error(error::error_code::invalid_elf, "Invalid ELF buffer");
        }
        else if (m_buffer_type == aiebu::aiebu_assembler::buffer_type::elf_aie2_config ||
            m_buffer_type == aiebu::aiebu_assembler::buffer_type::elf_aie2ps_config ||
            m_buffer_type == aiebu::aiebu_assembler::buffer_type::elf_aie4 ||
            m_buffer_type == aiebu::aiebu_assembler::buffer_type::elf_aie4_config) {
               throw error(error::error_code::internal_error, "Not supported");
        }
    }

    void reporter::elf_summary(std::ostream &stream) const
    {
        if (!stream) {
            throw error(error::error_code::invalid_input, "The given stream is not writable or has failed");
        }
        if (m_buffer_type == aiebu::aiebu_assembler::buffer_type::elf_aie2) {
            ELFIO::dump::header(stream, my_elf_reader );
            ELFIO::dump::section_headers( stream, my_elf_reader);
            ELFIO::dump::segment_headers( stream, my_elf_reader);
        }
        else {
            throw error(error::error_code::invalid_buffer_type,
                  "Invalid buffer type for ELF summary");
        }
    }

    void reporter::ctrlcode_summary(std::ostream &stream) const
    {
        if (!stream) {
            throw error(error::error_code::invalid_input, "The given stream is not writable or has failed");
        }
        if (m_buffer_type == aiebu::aiebu_assembler::buffer_type::elf_aie2) {
            ELFIO::Elf_Half sec_num = my_elf_reader.sections.size();
            for ( int i = 0; i < sec_num; ++i ) {
                const ELFIO::section* psec = my_elf_reader.sections[i];

                // Decoding not supported for ".ctrldata" section
                // for aie2 ".ctrldata" contain control packet and ".ctrlpkt-pm-N" contain
                // pm control packet which cannot be decoded
                if (psec->get_type() != ELFIO::SHT_PROGBITS || is_ctrldata(psec->get_name())
                || is_pm_ctrlpkt(psec->get_name()))
                    continue;

                stream << "  [" << i << "] " << psec->get_name() << "\t"
                    << psec->get_size() << std::endl;

                transaction tprint(psec->get_data(), psec->get_size());
                stream << tprint.get_txn_summary() << std::endl;
            }
        }
        else if (m_buffer_type == aiebu::aiebu_assembler::buffer_type::blob_instr_transaction) {
            ctrlcode_blob_summary(stream);
        }
        else {
            throw error(error::error_code::invalid_buffer_type,
                  "Invalid buffer type for summary");
        }
    }

    void reporter::disassemble(const std::filesystem::path &root, bool all) const
    {
        if (m_buffer_type == aiebu::aiebu_assembler::buffer_type::elf_aie2) {
            ELFIO::Elf_Half sec_num = my_elf_reader.sections.size();
            for ( int i = 0; i < sec_num; ++i ) {
                const ELFIO::section* psec = my_elf_reader.sections[i];

                if (psec->get_type() != ELFIO::SHT_PROGBITS)
                    continue;
                if (is_pm_ctrlpkt(psec->get_name()) || is_ctrldata(psec->get_name())) {
                    if (all) {
                        std::filesystem::path file(root);
                        file += psec->get_name();
                        file += ".ctrl";
                        std::ofstream stream(file);
                        if (!stream) {
                            throw error(error::error_code::internal_error, "Failed to open file for writing");
                        }
                        stream << ";  [" << i << "] " << psec->get_name() << "\t"
                            << psec->get_size() << 'B' << std::endl;
                        // Check type of control packet
                        aiebu::aiebu_assembler::buffer_type packet_type =
                        identify_control_packet(psec->get_data(), psec->get_size());
                        packets pprint(psec->get_data(), psec->get_size(), packet_type);
                        stream << pprint.get_dump();
                    }
                    continue;
                }

                // Write out the ctrlcode in rudimentary ASM format
                std::filesystem::path file(root);
                file += psec->get_name();
                file += ".asm";
                std::ofstream stream(file);
                if (!stream) {
                    throw error(error::error_code::internal_error, "Failed to open file for writing");
                }
                stream << ";  [" << i << "] " << psec->get_name() << "\t"
                    << psec->get_size() << 'B' << std::endl;

                transaction tprint(psec->get_data(), psec->get_size());
                stream << tprint.get_all_ops() << std::endl;
            }
        }
        else if (m_buffer_type == aiebu::aiebu_assembler::buffer_type::blob_instr_transaction) {
            disassemble_blob(root);
        }
        else if (m_buffer_type == aiebu::aiebu_assembler::buffer_type::elf_aie2ps ) {
            try {
                aiebu::elf_asm_disassembler disasm(root.string(), std::cout);
                disasm.run();
            } catch (const std::exception& ex) {
                throw error(error::error_code::internal_error,
                    "Disassembler error: " + std::string(ex.what()));
            }
        }
        else {
            throw error(error::error_code::invalid_buffer_type,
                  "Invalid buffer type for disassembly");
        }
    }

    void reporter::disassemble(std::ostream &stream, bool all) const
    {
        if (!stream) {
            throw error(error::error_code::invalid_input, "The given stream is not writable or has failed");
        }
        if (m_buffer_type == aiebu::aiebu_assembler::buffer_type::elf_aie2) {
            ELFIO::Elf_Half sec_num = my_elf_reader.sections.size();
            for ( int i = 0; i < sec_num; ++i ) {
                const ELFIO::section* psec = my_elf_reader.sections[i];

                if (psec->get_type() != ELFIO::SHT_PROGBITS)
                    continue;
                if (is_pm_ctrlpkt(psec->get_name()) || is_ctrldata(psec->get_name())) {
                    if (all) {
                        stream << "\n";
                        stream << "Section[" << i << "]: " << psec->get_name()
                            << "\tSize: " << psec->get_size() << 'B' << std::endl;

                        // Check type of control packet
                        aiebu::aiebu_assembler::buffer_type packet_type =
                        identify_control_packet(psec->get_data(), psec->get_size());
                        packets pprint(psec->get_data(), psec->get_size(), packet_type);
                        stream << "\n" << pprint.get_dump();
                    }
                    continue;
                }

                stream << "\n";
                stream << "Section[" << i << "]: " << psec->get_name() << "\tSize: "
                    << psec->get_size() << 'B' << std::endl;
                transaction tprint(psec->get_data(), psec->get_size());
                stream << tprint.get_all_ops() << std::endl;
            }
        }
        else if (m_buffer_type == aiebu::aiebu_assembler::buffer_type::blob_instr_transaction ||
                 m_buffer_type == aiebu::aiebu_assembler::buffer_type::blob_aie2ps ||
                 m_buffer_type == aiebu::aiebu_assembler::buffer_type::blob_aie4) {
            // Disassemble binary files - delegate to disassemble_blob for evaluation
            disassemble_blob(stream);
        }
        else {
            throw error(error::error_code::invalid_buffer_type,
                  "Invalid buffer type for disassembly");
        }
    }

    void reporter::ctrlcode_blob_summary(std::ostream &stream) const
    {
        transaction tprint(m_buffer.data(), m_buffer.size());
        stream << tprint.get_txn_summary() << std::endl;
    }

    void reporter::disassemble_blob(std::ostream &stream) const
    {
        try {
            // Evaluate buffer type and disassemble accordingly
            if (m_buffer_type == aiebu::aiebu_assembler::buffer_type::blob_aie2ps ||
                m_buffer_type == aiebu::aiebu_assembler::buffer_type::blob_aie4) {
                aiebu::bin_asm_disassembler disasm(m_buffer, stream, m_buffer_type);
                disasm.run();
            }
            else if (m_buffer_type == aiebu::aiebu_assembler::buffer_type::blob_instr_transaction) {
                // Legacy transaction format disassembly
                transaction tprint(m_buffer.data(), m_buffer.size());
                stream << tprint.get_all_ops() << std::endl;
            }
            else {
                throw error(error::error_code::invalid_buffer_type,
                    "disassemble_blob called with unsupported buffer type");
            }
        } catch (const std::exception& ex) {
            throw error(error::error_code::internal_error,
                "Binary disassembler error: " + std::string(ex.what()));
        }
    }

    void reporter::disassemble_blob(const std::filesystem::path &root) const
    {
        std::filesystem::path file(root);
        file += "_disassemble.asm";
        std::ofstream stream(file);
        if (!stream) {
            throw error(error::error_code::internal_error, "Failed to open file for writing");
        }
        disassemble_blob(stream);
    }
}