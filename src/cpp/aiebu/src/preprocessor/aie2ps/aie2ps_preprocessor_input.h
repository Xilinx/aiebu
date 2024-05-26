// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_INPUT_H_
#define _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_INPUT_H_

#include "preprocessor_input.h"

namespace aiebu {

class aie2ps_preprocessor_input : public preprocessor_input
{
public:
  aie2ps_preprocessor_input() {}

  virtual void set_args(const std::vector<char>& control_code,
                        const std::vector<symbol>& patch_data,
                        const std::vector<char>& buffer2,
                        const std::vector<std::string>& /* libs */,
                        const std::vector<std::string>& /* libpaths */) override
  {
    m_data[std::string("control_code")] = control_code;
  }

  std::vector<char>& get_data()
  {
    return m_data["control_code"];
  }
};

}
#endif //_AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_INPUT_H_
