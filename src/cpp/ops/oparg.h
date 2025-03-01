// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _ADSM_OPS_OPARG_H_
#define _ADSM_OPS_OPARG_H_

#include <vector>
#include <string>

namespace aiebu {

class opArg
{
public:
  enum class optype: uint8_t
  {
    CONST = 0,
    REG = 1,
    PAD = 2,
    JOBSIZE = 3,
    BARRIER = 4,
    PAGE_ID = 5,
  };

  std::string m_name;
  optype m_type;
  uint8_t m_width;

  opArg(std::string name, optype type, uint8_t width): m_name(name), m_type(type), m_width(width) { }
  HEADER_ACCESS_GET_SET(std::string, name);
  HEADER_ACCESS_GET_SET(optype, type);
  HEADER_ACCESS_GET_SET(uint8_t, width);
};

}

#endif //_ADSM_OPS_OPARG_H_
