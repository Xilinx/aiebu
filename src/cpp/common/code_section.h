// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_COMMOM_CODE_SECTION_H_
#define _AIEBU_COMMOM_CODE_SECTION_H_

#include <cstdint>

namespace aiebu {

enum class code_section
{
  text = 1,
  data = 2,
  unknown = 3
};

constexpr uint8_t AIE2P_OPT_MAJOR_VER = 1;
constexpr uint8_t AIE2P_OPT_MINOR_VER = 0;

}
#endif //_AIEBU_COMMOM_CODE_SECTION_H_
