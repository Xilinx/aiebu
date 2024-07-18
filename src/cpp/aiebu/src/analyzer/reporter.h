// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_REPORTER_H_
#define _AIEBU_REPORTER_H_

#include "aiebu_assembler.h"

#include <elfio/elfio_dump.hpp>

namespace aiebu {

    class reporter {
    private:
        ELFIO::elfio my_elf_reader;
    public:
        reporter(aiebu::aiebu_assembler::buffer_type type, const std::vector<char>& elf_data);
        void elf_summary(std::ostream &stream) const;
        void ctrlcode_summary(std::ostream &stream) const;
        void ctrlcode_detail_summary(std::ostream &stream) const;
    };
}

#endif
