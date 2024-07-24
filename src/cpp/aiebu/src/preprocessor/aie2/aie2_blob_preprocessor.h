// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSOR_H_
#define _AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSOR_H_

#include "preprocessor.h"
#include "aie2_blob_preprocessor_input.h"
#include "aie2_blob_preprocessed_output.h"

namespace aiebu {

class aie2_blob_preprocessor: public preprocessor
{  

public:
  aie2_blob_preprocessor() {}

  std::vector<uint8_t> transform(const std::vector<char>& in)
  {
    // transform vector<char> to vector<uint8_t>
    std::vector<uint8_t> out;
    out.resize(in.size());
    std::transform(in.begin(), in.end(), out.begin(), [](char c) {return static_cast<uint8_t>(c);});
    return out;
  }

  virtual std::shared_ptr<preprocessed_output>
  process(std::shared_ptr<preprocessor_input> input) override
  {
    // preporcess : nothing to be done.
    auto rinput = std::static_pointer_cast<aie2_blob_preprocessor_input>(input);
    auto routput = std::make_shared<aie2_blob_preprocessed_output>();

    for(auto key : rinput->get_keys())
      routput->add_data(key, transform(rinput->get_data(key)));

    routput->add_symbols(rinput->get_symbols());
    return routput;
  }
};

}
#endif //_AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSOR_H_
