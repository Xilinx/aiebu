// SPDX-License-Identifier: MIT
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

  virtual std::vector<writer> process(std::shared_ptr<preprocessed_output> input) override
  {
    // encode : nothing to be done as blob is already encoded
    auto rinput = std::static_pointer_cast<aie2_blob_preprocessed_output>(input);
    std::vector<writer> rwriter;

    for(auto key : rinput->get_keys())
      if ( !key.compare(".ctrltext") )
        rwriter.emplace_back(key, code_section::text, rinput->get_data(key));
      else
        rwriter.emplace_back(key, code_section::data, rinput->get_data(key));

    rwriter[0].add_symbols(rinput->get_symbols());

    return rwriter;
  }
};

}
#endif //_AIEBU_ENCODER_AIE2_BLOB_ENCODER_H_
