// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_GEN_COMMON_H_
#define _AIEBU_GEN_COMMON_H_


// AIE Driver headers
#include "xaiengine.h"
#include <system_error>
#include <map>

class
test_XAie_error : public std::system_error
{
private:
    static const std::map<AieRC, std::string> AieRCNames;

private:
  static std::string
  message(AieRC code, const std::string& what) {
    std::string str = what;
    str += ": ";
    str += AieRCNames.at(code);
    return str;
  }

public:
  explicit
  test_XAie_error(AieRC code, const std::string& what = "")
    : system_error(code, std::system_category(), message(code, what))
  {}
};

inline void
gen_XAie_check(AieRC code, const char *note = "") {
    if (code != XAIE_OK) {
        throw test_XAie_error(code, note);
    }
}

const std::map<AieRC, std::string> test_XAie_error::AieRCNames = {
        std::make_pair(XAIE_OK, "XAIE_OK"),
        std::make_pair(XAIE_ERR, "XAIE_ERR"),
        std::make_pair(XAIE_INVALID_DEVICE, "XAIE_INVALID_DEVICE"),
        std::make_pair(XAIE_INVALID_RANGE, "XAIE_INVALID_RANGE"),
        std::make_pair(XAIE_INVALID_ARGS, "XAIE_INVALID_ARGS"),
        std::make_pair(XAIE_INVALID_TILE, "XAIE_INVALID_TILE"),
        std::make_pair(XAIE_ERR_STREAM_PORT, "XAIE_ERR_STREAM_PORT"),
        std::make_pair(XAIE_INVALID_DMA_TILE, "XAIE_INVALID_DMA_TILE"),
        std::make_pair(XAIE_INVALID_BD_NUM, "XAIE_INVALID_BD_NUM"),
        std::make_pair(XAIE_ERR_OUTOFBOUND, "XAIE_ERR_OUTOFBOUND"),
        std::make_pair(XAIE_INVALID_DATA_MEM_ADDR, "XAIE_INVALID_DATA_MEM_ADDR"),
        std::make_pair(XAIE_INVALID_ELF, "XAIE_INVALID_ELF"),
        std::make_pair(XAIE_CORE_STATUS_TIMEOUT, "XAIE_CORE_STATUS_TIMEOUT"),
        std::make_pair(XAIE_INVALID_CHANNEL_NUM, "XAIE_INVALID_CHANNEL_NUM"),
        std::make_pair(XAIE_INVALID_LOCK, "XAIE_INVALID_LOCK"),
        std::make_pair(XAIE_INVALID_DMA_DIRECTION, "XAIE_INVALID_DMA_DIRECTION"),
        std::make_pair(XAIE_INVALID_PLIF_WIDTH, "XAIE_INVALID_PLIF_WIDTH"),
        std::make_pair(XAIE_INVALID_LOCK_ID, "XAIE_INVALID_LOCK_ID"),
        std::make_pair(XAIE_INVALID_LOCK_VALUE, "XAIE_INVALID_LOCK_VALUE"),
        std::make_pair(XAIE_LOCK_RESULT_FAILED, "XAIE_LOCK_RESULT_FAILED"),
        std::make_pair(XAIE_INVALID_DMA_DESC, "XAIE_INVALID_DMA_DESC"),
        std::make_pair(XAIE_INVALID_ADDRESS, "XAIE_INVALID_ADDRESS"),
        std::make_pair(XAIE_FEATURE_NOT_SUPPORTED, "XAIE_FEATURE_NOT_SUPPORTED"),
        std::make_pair(XAIE_INVALID_BURST_LENGTH, "XAIE_INVALID_BURST_LENGTH"),
        std::make_pair(XAIE_INVALID_BACKEND, "XAIE_INVALID_BACKEND"),
        std::make_pair(XAIE_INSUFFICIENT_BUFFER_SIZE, "XAIE_INSUFFICIENT_BUFFER_SIZE"),
        std::make_pair(XAIE_INVALID_API_POINTER, "XAIE_INVALID_API_POINTER"),
        std::make_pair(XAIE_ERR_MAX, "XAIE_ERR_MAX"),
    };

#endif
