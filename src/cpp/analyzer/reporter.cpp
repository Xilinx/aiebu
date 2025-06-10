// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.
#include "reporter.h"
#include "packets.h"
#include "transaction.hpp"
#include "transaction.hpp"
#include "common/file_utils.h"


#include "aiebu/aiebu_error.h"

#include <boost/interprocess/streams/bufferstream.hpp>
#include <elfio/elfio_dump.hpp>
namespace aiebu {

    reporter::reporter(aiebu::aiebu_assembler::buffer_type /*type*/, const std::vector<char>& elf_data)
    {
        boost::interprocess::ibufferstream istr(elf_data.data(), elf_data.size());
        bool result = my_elf_reader.load(istr);
        if (!result)
            throw error(error::error_code::invalid_buffer_type, "Invalid ELF buffer");
    }

    void reporter::elf_summary(std::ostream &stream) const
    {
        ELFIO::dump::header(stream, my_elf_reader );
        ELFIO::dump::section_headers( stream, my_elf_reader);
        ELFIO::dump::segment_headers( stream, my_elf_reader);
    }

    void reporter::ctrlcode_summary(std::ostream &stream) const
    {
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

    void reporter::disassemble(const std::filesystem::path &root, bool all) const
    {
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
            stream << ";  [" << i << "] " << psec->get_name() << "\t"
                   << psec->get_size() << 'B' << std::endl;

            transaction tprint(psec->get_data(), psec->get_size());
            stream << tprint.get_all_ops() << std::endl;
        }
    }

    void reporter::disassemble(std::ostream &stream, bool all) const
    {
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
}
