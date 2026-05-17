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

//Max sections is 65280, so max pages is 32640; because each page is 2 sections.
constexpr uint32_t max_pages = 32640;

class aie2ps_preprocessor: public preprocessor
{
  const std::string disable_dump_map = "disabledump";
  const std::string full_dump_map = "fulldump";
  std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> m_isa;
public:
  aie2ps_preprocessor() {}

  virtual std::string get_target_name() const { return "aie2ps"; }

  virtual std::shared_ptr<assembler_state> create_assembler_state(std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> isa,
                                                 std::vector<std::shared_ptr<asm_data>>& data,
                                                 std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpad,
                                                 std::map<std::string, uint32_t>& labelpageindex,
                                                 std::map<uint32_t, std::string>& ctrlpkt_id_map, uint32_t optimize_level, bool makeunique)
  {
    return std::make_shared<assembler_state_aie2ps>(isa, data, scratchpad, labelpageindex, ctrlpkt_id_map, optimize_level, makeunique, false);
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
          log_warn() << "Invalid log level flag: " << flag << ", ignored\n";
      }
      else
        log_warn() << "Invalid flag: " << flag << ", ignored\n";
    }
    
    std::shared_ptr<asm_parser> parser(new asm_parser(tinput->get_ctrlcode_data(), tinput->get_include_paths(), get_target_name(), tinput->get_artifacts()));

    parser->parse_lines();

    // Verify PREEMPT opcode count is equal across all columns in the control code.
    // All controllers must have the same number of preemption points to ensure consistent
    // in cert, as cert synchronize preemption points at runtime and mismatch may lead to hang.
    // The count from column 0 is used as the reference for comparison.
    if (parser->has_preempt()) {
      auto [success, expected_count, mismatch_col, mismatch_count] = parser->verify_preempt_count();
      if (!success) {
        throw error(error::error_code::invalid_asm,
          "Preempt opcode count mismatch: expected " + std::to_string(expected_count) +
          " preempt opcodes (from col 0), but controller " + std::to_string(mismatch_col) +
          " has " + std::to_string(mismatch_count) + " preempt opcodes\n");
      }
      log_info() << "Ctrlcode has " << expected_count << " preemption points\n";

      // cert relies on load_pdi (and possible load_cores / load_cores_cp) to recover
      // the last loaded PDI and cores at each preemption point.
      if (!parser->verify_preempt_requires_load_pdi())
        throw error(error::error_code::invalid_asm,
          "preempt opcode requires at least one load_pdi in the same control code elf\n");
    }

    // Verify load_pdi opcode count is equal across all columns in multi-UC control code.
    {
      auto [ok, exp, col, got] = parser->verify_load_pdi_count();
      if (!ok)
        throw error(error::error_code::invalid_asm,
          "load_pdi opcode count mismatch: expected " + std::to_string(exp) +
          " (from col 0), but controller " + std::to_string(col) +
          " has " + std::to_string(got) + " load_pdi opcodes\n");
    }

    // Verify load_cores opcode count is equal across all columns.
    {
      auto [ok, exp, col, got] = parser->verify_load_cores_count();
      if (!ok)
        throw error(error::error_code::invalid_asm,
          "load_cores opcode count mismatch: expected " + std::to_string(exp) +
          " (from col 0), but controller " + std::to_string(col) +
          " has " + std::to_string(got) + " load_cores opcodes\n");
    }

    // Verify load_cores_cp opcode count is equal across all columns.
    {
      auto [ok, exp, col, got] = parser->verify_load_cores_cp_count();
      if (!ok)
        throw error(error::error_code::invalid_asm,
          "load_cores_cp opcode count mismatch: expected " + std::to_string(exp) +
          " (from col 0), but controller " + std::to_string(col) +
          " has " + std::to_string(got) + " load_cores_cp opcodes\n");
    }

    // Verify start_cond_job_preempt opcode count is equal across all columns.
    {
      auto [ok, exp, col, got] = parser->verify_start_cond_job_preempt_count();
      if (!ok)
        throw error(error::error_code::invalid_asm,
          "start_cond_job_preempt opcode count mismatch: expected " + std::to_string(exp) +
          " (from col 0), but controller " + std::to_string(col) +
          " has " + std::to_string(got) + " start_cond_job_preempt opcodes\n");
    }

    // Verify .target directive matches the -t command line option
    auto target_info = parser->get_target_info();
    std::string expected_target = tinput->get_expected_target();
    if (target_info->is_set()) {
      std::string asm_target = target_info->get_arch();
      if (asm_target != expected_target) {
        throw error(error::error_code::invalid_asm,
          "Target mismatch: .target directive specifies '" + target_info->get_full_target() +
          "' but -t option specifies '" + expected_target + "'\n");
      }
      log_info() << "Target verification passed: " << target_info->get_full_target() << std::endl;
    }

    auto collist = parser->get_col_list();
    isa i;
    m_isa = i.get_isamap();
    auto toutput = std::make_shared<aie2ps_preprocessed_output>(parser->get_partition_info(),
                                                                parser->get_target_info(),
                                                                parser->get_aie_row_topology_info());
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
      uint32_t relative_page_index = 0;
      int pad_size = 0;
      auto& label_page_index = parser->getcollabelpageindex(col);
      auto& scratchpad = parser->getcolscratchpad(col);
      auto& coldata = parser->get_col_asmdata(col);
      for (auto label : parser->getLabelsforcol(col))
      {
        log_debug() << "[" << col << "]: Processing label: " << label << " Relative page index: " << relative_page_index<< std::endl;
        // create state
        std::vector<std::shared_ptr<asm_data>> data = coldata.get_label_asmdata(label);
        std::shared_ptr<assembler_state> state = create_assembler_state(m_isa, data, scratchpad, label_page_index, ctrlpkt_id_map, optimize, true);
      // create pages
        pager(PAGE_SIZE).pagify(*state, col, pages, relative_page_index);
        label_page_index[get_pagelabel(label)] = relative_page_index;
        log_debug() << "num pages: " << pages.size() - relative_page_index << std::endl;
        relative_page_index = static_cast<uint32_t>(pages.size());
      }

      for (auto& pad : scratchpad)
      {
        pad_size = (((pad_size + 3) >> 2) << 2); // round off to next multiple of 4
        pad.second->set_offset(pad_size);
        pad.second->set_base(PAGE_SIZE * relative_page_index);
        pad_size += pad.second->get_size();
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

    propagate_global_custom_sections(toutput, rinput);

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
