// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_AIE2_ASM_PREPROCESSOR_H_
#define _AIEBU_PREPROCESSOR_AIE2_ASM_PREPROCESSOR_H_

#include "aie2_blob_preprocessor.h"
#include "aie2_blob_preprocessor_input.h"
#include "aie2_blob_preprocessed_output.h"

namespace aiebu {

// Derived from the aie2_blob as the overall flow is same as aie2_blob
// The major difference is that the ASM input is preprocessed into blob format
// so we can reuse the rest of the blob infrastructure.
class aie2_asm_preprocessor: public aie2_blob_preprocessor
{
public:
  aie2_asm_preprocessor() = default;

  virtual std::shared_ptr<preprocessed_output>
  process(std::shared_ptr<preprocessor_input> input) override
  {
    auto rinput = std::static_pointer_cast<aie2_asm_preprocessor_input>(input);
    // Behave the same way as aie2_blob_preprocessor::process()
    // TODO: explore if this process() can simply call the other process().
    auto routput = std::make_shared<aie2_blob_preprocessed_output>();

    for(auto key : rinput->get_keys())
      routput->add_data(key, transform(rinput->get_data(key)));

    routput->add_symbols(rinput->get_symbols());
    return routput;
  }
};

}
#endif //_AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSOR_H_
