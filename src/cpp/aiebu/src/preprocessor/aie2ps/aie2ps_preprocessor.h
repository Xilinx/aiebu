// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_H_
#define _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_H_

#include <string>
#include "preprocessor.h"
#include "asm/asm_parser.h"
#include "assembler_state.h"
#include "asm/pager.h"
#include "aie2ps_preprocessor_input.h"
#include "aie2ps_preprocessed_output.h"
#include "aie2ps/isa.h"

namespace aiebu {

class aie2ps_preprocessor: public preprocessor
{  
  std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> m_isa;
public:
  aie2ps_preprocessor() {}

  virtual std::shared_ptr<preprocessed_output>
  process(std::shared_ptr<preprocessor_input> input) override
  {
    // do preprocessing,
    // separate out asm data colnum wise
    // create pages
    auto tinput = std::static_pointer_cast<aie2ps_preprocessor_input>(input);
    auto toutput = std::make_shared<aie2ps_preprocessed_output>();
    asm_parser a(tinput->get_data());
    auto collist = a.get_col_list();
    isa i;
    m_isa = i.get_isamap();

    for (auto col: collist)
    {
      std::vector<page> pages;
      // create state
      assembler_state state = assembler_state(m_isa, a.get_col_asmdata(col));
      // create pages
      pager(PAGE_SIZE).pagify(state, col, pages);
      toutput->set_coldata(col, pages);     
    }
    return toutput;
  }
};

}
#endif //_AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_H_
