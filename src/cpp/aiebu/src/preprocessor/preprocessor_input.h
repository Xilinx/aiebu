// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_PREPROCESSOR_INPUT_H_
#define _AIEBU_PREPROCESSOR_PREPROCESSOR_INPUT_H_

#include <vector>
#include "symbol.h"

namespace aiebu {

class preprocessor_input
{
public:
  preprocessor_input() {}

  virtual void set_args(const std::vector<char>& buffer1,
                       const std::vector<symbol>& patch_data,
                       const std::vector<char>& buffer2) = 0;
};

}
#endif //_AIEBU_PREPROCESSOR_PREPROCESSOR_INPUT_H_
