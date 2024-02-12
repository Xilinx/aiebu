// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ENCODER_ENCODER_H_
#define _AIEBU_ENCODER_ENCODER_H_

#include <memory>

#include "preprocessed_output.h"
#include "writer.h"

namespace aiebu {

class encoder
{
public:
  encoder() {}

  virtual std::shared_ptr<writer>
  process(std::shared_ptr<preprocessed_output> input) = 0;
};

}
#endif //_AIEBU_ENCODER_ENCODER_H_
