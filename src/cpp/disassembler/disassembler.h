// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_PREPROCESSOR_AIE2PS_DISASSEMBLER_H_
#define AIEBU_PREPROCESSOR_AIE2PS_DISASSEMBLER_H_

#include <memory>
#include <vector>
#include <map>

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>


#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

#include <string>
#include "elfio/elfio.hpp"
#include "specification/aie2ps/isa.h"
#include "common/disassembler_state.h"

namespace aiebu {

class asm_disassembler {
public:
    explicit asm_disassembler(const std::string& filename, const std::string& lfile);

    void run();

    bool is_text_section(const std::string& name) { return name.substr(0, 9) == ".ctrltext"; }
    bool is_data_section(const std::string& name) { return name.substr(0, 9) == ".ctrldata"; }
private:
    void dump();
    void dump_section_info(const ELFIO::section* sec);
    void dump_text_section(const ELFIO::section* sec, std::shared_ptr<disassembler_state> state);
    void dump_data_section(const ELFIO::section* sec, std::shared_ptr<disassembler_state> state);
    void dump_pad_section(const ELFIO::section* /*sec*/, std::shared_ptr<disassembler_state> /*state*/)
    {
      std::cout << "Dumping .pad not supported\n";
    }

    ELFIO::elfio reader;
    ctrl_writer writer;
    std::shared_ptr<std::map<uint8_t, std::shared_ptr<isa_op>>> isa_ops;
};

}
#endif //AIEBU_PREPROCESSOR_AIE2PS_DISASSEMBLER_H_
