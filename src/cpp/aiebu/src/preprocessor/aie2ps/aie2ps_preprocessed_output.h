// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSED_OUTPUT_H_
#define _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSED_OUTPUT_H_

#include "asm/page.h"
#include "preprocessed_output.h"

namespace aiebu {

class aie2ps_preprocessed_output : public preprocessed_output
{
  std::map<uint32_t, std::vector<page>> m_coldata;
public:
  aie2ps_preprocessed_output() {}

  void set_coldata(const uint32_t col, const std::vector<page> &pages)
  {
    m_coldata[col] = std::move(pages);
  }

  const std::map<uint32_t, std::vector<page>>& get_coldata() const
  {
    return m_coldata;
  }
};

}
#endif //_AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSED_OUTPUT_H_
