// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_H_
#define _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_H_

#include <string>
#include "preprocessor.h"
#include "asm/asm_parser.h"
#include "assembler_state.h"
#include "asm/pager.h"
#include "aie2ps_preprocessor_input.h"
#include "aie2ps_preprocessed_output.h"
#include "specification/aie2ps/isa.h"
#include "logger.h"

namespace aiebu {

class aie2ps_preprocessor: public preprocessor
{
  const std::string disable_dump_map = "disabledump";
  const std::string full_dump_map = "fulldump";
  std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> m_isa;
public:
  aie2ps_preprocessor() {}
  virtual std::shared_ptr<assembler_state> create_assembler_state(std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> isa,
                                                 std::vector<std::shared_ptr<asm_data>>& data,
                                                 std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpad,
                                                 std::map<std::string, uint32_t>& labelpageindex,
                                                 std::map<uint32_t, std::string>& ctrlpkt_id_map, uint32_t optimize_level, bool makeunique)
  {
    return std::make_shared<assembler_state_aie2ps>(isa, data, scratchpad, labelpageindex, ctrlpkt_id_map, optimize_level, makeunique);
  }

  virtual std::shared_ptr<preprocessed_output>
  process(std::shared_ptr<preprocessor_input> input) override
  {
    // do preprocessing,
    // separate out asm data colnum wise
    // create pages
    return process_internal(std::dynamic_pointer_cast<aie2ps_preprocessor_input>(input));
  }

  std::shared_ptr<preprocessed_output>
  process_internal(std::shared_ptr<asm_preprocessor_input> tinput)
  {
    //auto keys = tinput->get_keys();
    const std::string prefix = "opt_level_";

    // Process flags before parsing
    auto flags = tinput->get_flags();
    uint32_t optimize = 0;
    asm_dump_flag debug_flag = asm_dump_flag::text;
    const std::string loglevel_prefix = "loglevel_";
    for (const auto& flag: flags)
    {
      if (flag == disable_dump_map)
        debug_flag = asm_dump_flag::disable;
      else if (flag == full_dump_map)
        debug_flag = asm_dump_flag::full;
      else if (flag.find(prefix) == 0)
        optimize = std::stoi(flag.substr(prefix.size()));
      else if (flag.find(loglevel_prefix) == 0) {
        // Process log level for library API users
        std::string log_level_str = flag.substr(loglevel_prefix.size());
        if (log_level_str == "error")
          set_log_level(log_level::error);
        else if (log_level_str == "warn")
          set_log_level(log_level::warn);
        else if (log_level_str == "info")
          set_log_level(log_level::info);
        else if (log_level_str == "debug")
          set_log_level(log_level::debug);
        else
          log_warn() << "Invalid log level flag: " << flag << ", ignored";
      }
      else
        log_warn() << "Invalid flag: " << flag << ", ignored";
    }
    
    std::shared_ptr<asm_parser> parser(new asm_parser(tinput->get_ctrlcode_data(), tinput->get_include_paths(), tinput->get_artifacts()));

    parser->parse_lines();

    auto collist = parser->get_col_list();
    isa i;
    m_isa = i.get_isamap();
    auto toutput = std::make_shared<aie2ps_preprocessed_output>(parser->get_partition_info());
    toutput->set_debug(debug_flag);
    auto& controlpkts = tinput->get_controlpkt();
    auto& ctrlpkt_id_map = tinput->get_ctrlpkt_id_map();
    toutput->set_optmization(optimize);
    toutput->set_ctrlpkt(controlpkts);
    toutput->set_ctrlpkt_id_map(ctrlpkt_id_map);
    toutput->set_annotations(parser->get_annotations());

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
        std::shared_ptr<assembler_state> state = create_assembler_state(m_isa, data, scratchpad, label_page_index, ctrlpkt_id_map, optimize, true);
      // create pages
        pager(PAGE_SIZE).pagify(*state, col, pages, relative_page_index);
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

template <typename preprocessor_template, typename input_tamplete, typename output_tamplete>
class asm_config_preprocessor: public preprocessor
{

public:
  std::shared_ptr<preprocessed_output>
  process(std::shared_ptr<preprocessor_input> input) override
  {
    preprocessor_template m_preprocessor;
    auto rinput = std::dynamic_pointer_cast<asm_config_preprocessor_input>(input);
    auto toutput = std::make_shared<asm_config_preprocessed_output<output_tamplete>>();

    for (auto& [kernel, instances] : rinput->get_kernel_map()) {
      for(auto& [iname, instance] : instances)
      {
        auto val = std::dynamic_pointer_cast<output_tamplete>(m_preprocessor.process(instance));
        toutput->add_kernel_map(kernel, iname, val);
      }
    }
    return toutput;
  }
};
}
#endif //_AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_H_
