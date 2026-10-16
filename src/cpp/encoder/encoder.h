// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ENCODER_ENCODER_H_
#define _AIEBU_ENCODER_ENCODER_H_

#include <memory>
#include <string>

#include "preprocessed_output.h"
#include "writer.h"

namespace aiebu {

class encoder
{
public:
  encoder() = default;

  virtual std::vector<std::shared_ptr<writer>>
  process(std::shared_ptr<preprocessed_output> input) = 0;

  /** Merged single-section .ctrltext.<col>; no-op except aie2ps / asm_config encoders. */
  virtual void set_merged_ctrltext_elf(bool /*merged*/) {}

  virtual ~encoder() = default;
};

}
#endif //_AIEBU_ENCODER_ENCODER_H_
