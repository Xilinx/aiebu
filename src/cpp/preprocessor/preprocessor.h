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
  preprocessor() = default;

  virtual std::shared_ptr<preprocessed_output>
  process(std::shared_ptr<preprocessor_input> input) = 0;
  virtual ~preprocessor() = default;
  preprocessor(const preprocessor &) = delete;
  preprocessor(preprocessor &&) = delete;
  preprocessor& operator =(preprocessor const&) = delete;
  preprocessor& operator =(preprocessor &&) = delete;
};

}
#endif //_AIEBU_PREPROCESSOR_PREPROCESSOR_H_
