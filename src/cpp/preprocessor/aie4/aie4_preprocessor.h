// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_PREPROCESSOR_AIE4_PREPROCESSOR_H_
#define AIEBU_PREPROCESSOR_AIE4_PREPROCESSOR_H_

#include <string>
#include "aie2ps_preprocessor.h"
#include "preprocessor/aie4/aie4_preprocessor_input.h"

namespace aiebu {

class aie4_preprocessor: public aie2ps_preprocessor
{
public:
  aie4_preprocessor(): aie2ps_preprocessor() {}
  
  std::shared_ptr<preprocessed_output>
  process(std::shared_ptr<preprocessor_input> input) override
  {
    // do preprocessing,
    // separate out asm data colnum wise
    // create pages
    return process_internal(std::dynamic_pointer_cast<aie4_preprocessor_input>(input));
  }
  std::shared_ptr<assembler_state> create_assembler_state(std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> isa,
                                         std::vector<std::shared_ptr<asm_data>>& data,
                                         std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpad,
                                         std::map<std::string, uint32_t>& labelpageindex,
                                         std::map<uint32_t, std::string>& ctrlpkt_id_map, uint32_t optimize_level, bool makeunique) override
  {
    return std::make_shared<assembler_state_aie4>(isa, data, scratchpad, labelpageindex, ctrlpkt_id_map, optimize_level, makeunique);
  }
};

}
#endif //AIEBU_PREPROCESSOR_AIE4_PREPROCESSOR_H_
