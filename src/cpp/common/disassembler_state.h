// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_SRC_CPP_COMMON_DISASSEMBLER_STATE_H
#define AIEBU_SRC_CPP_COMMON_DISASSEMBLER_STATE_H

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <sstream>

#include "aiebu/aiebu_error.h"

namespace aiebu {

class disassembler_state {
private:
    uint32_t position = 0;
    std::map<uint32_t, std::string> labels;
    std::map<uint32_t, std::string> external_labels;
    std::map<uint32_t, std::pair<std::string, uint32_t>> local_ptr;

public:
    uint32_t get_address() const { return position; }

    void increment_address(uint32_t offset) {
        position += offset;
    }

    const std::map<uint32_t, std::string>& get_labels() const {
        return labels;
    }

    void add_label(uint32_t addr, const std::string& label) {
        labels[addr] = label;
    }

    const std::map<uint32_t, std::pair<std::string, uint32_t>>& get_local_ptrs() const {
        return local_ptr;
    }

    const std::map<uint32_t, std::string>& get_external_labels() const {
        return external_labels;
    }

    void add_external_label(uint32_t address, std::string label) {
        external_labels[address] = std::move(label);
    }

    void add_local_ptr(uint32_t address, const std::string& label, uint32_t offset) {
        local_ptr[address] = std::make_pair(label, offset);
    }

    void reset() {
        position = 0;
        labels.clear();
        external_labels.clear();
        local_ptr.clear();
    }

    std::string to_string() const {
        std::ostringstream oss;
        oss << "[POS:" << position
            << "\tLABELS:" << labels.size()
            << "\tLOCAL_PTRS:" << local_ptr.size() << "]";
        return oss.str();
    }

    std::string to_tile(uint32_t arg) {
        uint32_t row = arg & 0x1F;   //NOLINT
        uint32_t col = (arg >> 5) & 0x7F;  //NOLINT
        return "TILE_" + std::to_string(col) + "_" + std::to_string(row);
    }

    // FOR AIE2PS
    std::string to_actor(uint32_t val, uint32_t tile) {
        uint32_t row = tile & 0x1F;  //NOLINT

        if (row == 0) {  //NOLINT
          // One SHIM TILE
          switch(val)
          {
            case 0: return "SHIM_S2MM_0"; //NOLINT
            case 1: return "SHIM_S2MM_1"; //NOLINT
            case 6: return "SHIM_MM2S_0"; //NOLINT
            case 7: return "SHIM_MM2S_1"; //NOLINT
            default: throw error(error::error_code::invalid_asm, "Invalid Shim tile actor:" + std::to_string(val) + "\n");          }
        }
        else if (row == 1 || row == 2) { //NOLINT
          // Two MEM TILE
          switch(val)
          {
            case 0: return "MEM_S2MM_0"; //NOLINT
            case 1: return "MEM_S2MM_1"; //NOLINT
            case 2: return "MEM_S2MM_2"; //NOLINT
            case 3: return "MEM_S2MM_3"; //NOLINT
            case 4: return "MEM_S2MM_4"; //NOLINT
            case 5: return "MEM_S2MM_5"; //NOLINT
            case 6: return "MEM_MM2S_0"; //NOLINT
            case 7: return "MEM_MM2S_1"; //NOLINT
            case 8: return "MEM_MM2S_2"; //NOLINT
            case 9: return "MEM_MM2S_3"; //NOLINT
            case 10: return "MEM_MM2S_4"; //NOLINT
            case 11: return "MEM_MM2S_5"; //NOLINT
            default: throw error(error::error_code::invalid_asm, "Invalid Mem tile actor:" + std::to_string(val) + "\n");
          }
        }
        else { // CORE TILE
          switch(val)
          {
            case 0: return "TILE_S2MM_0"; //NOLINT
            case 1: return "TILE_S2MM_1"; //NOLINT
            case 6: return "TILE_MM2S_0"; //NOLINT
            case 7: return "TILE_MM2S_1"; //NOLINT
            case 15: return "TILE_CORE"; //NOLINT
            default: throw error(error::error_code::invalid_asm, "Invalid Core tile actor:" + std::to_string(val) + "\n");
          }
        }
    }
};

}// namespace aiebu

#endif //AIEBU_COMMON_DISASSEMBLER_STATE_H_
