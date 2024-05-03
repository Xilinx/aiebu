// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ENCODER_AIE2_BLOB_ENCODER_H_
#define _AIEBU_ENCODER_AIE2_BLOB_ENCODER_H_

#include <memory>

#include "encoder.h"
#include "aie2_blob_preprocessed_output.h"

namespace aiebu {

class aie2_blob_encoder: public encoder
{
public:
  aie2_blob_encoder() {}

  virtual std::shared_ptr<writer>process(std::shared_ptr<preprocessed_output> input) override
  {
    // encode : nothing to be done as blob is already encoded
    auto rinput = std::static_pointer_cast<aie2_blob_preprocessed_output>(input);
    std::shared_ptr<writer> rwriter = std::make_shared<writer>();

    auto &sym = rinput->get_symbols();
    for ( auto &s: sym)
      rwriter->add_symbol(std::move(s));

    rwriter->add_buffermap(0, std::make_pair(std::move(rinput->get_instruction_buffer()),
                           std::move(rinput->get_controlcode_buffer())));
    return rwriter;
  }
};

}
#endif //_AIEBU_ENCODER_AIE2_BLOB_ENCODER_H_
