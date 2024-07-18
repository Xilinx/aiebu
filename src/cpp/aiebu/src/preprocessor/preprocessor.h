// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_PREPROCESSOR_H_
#define _AIEBU_PREPROCESSOR_PREPROCESSOR_H_

#include <memory>

#include "preprocessor_input.h"
#include "preprocessed_output.h"

namespace aiebu {

class preprocessor
{
public:
  preprocessor() {}

  virtual std::shared_ptr<preprocessed_output>
  process(std::shared_ptr<preprocessor_input> input) = 0;
  virtual ~preprocessor() {}
};

}
#endif //_AIEBU_PREPROCESSOR_PREPROCESSOR_H_
