// SPDX-License-Identifier: MIT
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
    //auto keys = tinput->get_keys();
    std::shared_ptr<asm_parser> parser(new asm_parser(tinput->get_data(), tinput->get_include_paths()));
    parser->parse_lines();
    auto collist = parser->get_col_list();
    isa i;
    m_isa = i.get_isamap();

    for (auto col: collist)
    {
      std::vector<page> pages;
      int relative_page_index = 0;
      int pad_size = 0;
      auto& label_page_index = parser->getcollabelpageindex(col);
      auto& scratchpad = parser->getcolscratchpad(col);
      auto& coldata = parser->get_col_asmdata(col);
      for (auto label : parser->getLabelsforcol(col))
      {
        // create state
        std::vector<std::shared_ptr<asm_data>> data = coldata.get_label_asmdata(label);
        assembler_state state = assembler_state(m_isa, data, scratchpad, label_page_index, 0, true);
        // create pages
        pager(PAGE_SIZE).pagify(state, col, pages, relative_page_index);
        label_page_index[get_pagelabel(label)] = relative_page_index;
        relative_page_index = pages.size();
      }

      for (auto& pad : scratchpad)
      {
        pad_size = (((pad_size + 3) >> 2) << 2); // round off to next multiple of 4
        pad.second->set_offset(pad_size);
        pad.second->set_base(PAGE_SIZE * relative_page_index);
        pad_size += pad.second->get_size();
      }

      for (auto&page : pages)
      {
        auto &ooo = page.getout_of_order_page();
        if (ooo.size() > 2)
          throw error(error::error_code::invalid_asm, "Only 2 out of order branching supported\n");

        if (ooo.size() == 2)
          if (label_page_index.find(get_pagelabel(ooo[1])) == label_page_index.end())
            throw error(error::error_code::invalid_asm, "Label " + get_pagelabel(ooo[1]) + " not present in col:" + std::to_string(col) + "\n");

        if (ooo.size() == 1)
          if (label_page_index.find(get_pagelabel(ooo[0])) == label_page_index.end())
            throw error(error::error_code::invalid_asm, "Label " + get_pagelabel(ooo[0]) + " not present in col:" + std::to_string(col) + "\n");

        auto ooo_page_len_1 = ooo.size() ? pages[label_page_index.at(get_pagelabel(ooo[0]))].get_cur_page_len() : 0;
        auto ooo_page_len_2 = (ooo.size() == 2) ? pages[label_page_index.at(get_pagelabel(ooo[1]))].get_cur_page_len() : 0;
        page.set_ooo_page_len(ooo_page_len_1,ooo_page_len_2);
      }
      toutput->set_coldata(col, pages, scratchpad, label_page_index, tinput->get_control_packet_index());
    }
    toutput->add_symbols(tinput->get_symbols());
    return toutput;
  }
};

}
#endif //_AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_H_
