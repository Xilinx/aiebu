// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include "reporter.h"
#include "aiebu_error.h"

#include "transaction.hpp"

#include <boost/interprocess/streams/bufferstream.hpp>
#include <elfio/elfio_dump.hpp>

namespace aiebu {

    reporter::reporter(aiebu::aiebu_assembler::buffer_type type, const std::vector<char>& elf_data)
    {
        boost::interprocess::ibufferstream istr(elf_data.data(), elf_data.size());
        bool result = my_elf_reader.load(istr);
        if (!result)
            throw error(error::error_code::invalid_buffer_type, "Invalid ELF buffer");
    }

    void reporter::elfreport(std::ostream &stream) const
    {
        ELFIO::dump::header(stream, my_elf_reader );
        ELFIO::dump::section_headers( stream, my_elf_reader);
        ELFIO::dump::segment_headers( stream, my_elf_reader);
    }

    void reporter::txnreport(std::ostream &stream) const
    {
        ELFIO::Elf_Half sec_num = my_elf_reader.sections.size();
        for ( int i = 0; i < sec_num; ++i ) {
            const ELFIO::section* psec = my_elf_reader.sections[i];
            if (psec->get_type() != ELFIO::SHT_PROGBITS)
                continue;

            stream << "  [" << i << "] " << psec->get_name() << "\t"
                   << psec->get_size() << std::endl;

            transaction tprint(psec->get_data(), psec->get_size());
            stream << tprint.get_txn_summary() << std::endl;
        }
    }
}
