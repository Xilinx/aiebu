// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.
#include "assembler_state.h"

#include "utils.h"

#include "regex_wrapper.h"
#include "aiebu/aiebu_error.h"

#include <unordered_map>

namespace aiebu {

assembler_state::
assembler_state(std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> isa,
                std::vector<std::shared_ptr<asm_data>>& data,
                std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpad,
                std::map<std::string, uint32_t>& labelpageindex, std::map<uint32_t, std::string>& ctrlpkt_id_map,
                uint32_t optimize_level, bool makeunique, bool merged_ctrltext_elf)
                : m_isa(std::move(isa)), m_data(data), m_scratchpad(scratchpad),
                  m_labelpageindex(labelpageindex), m_ctrlpkt_id_map(ctrlpkt_id_map),
                  m_merged_ctrltext_elf(merged_ctrltext_elf)
{
  process_optimization(optimize_level);
  process(makeunique);
  //printstate();
}

// makeunique: make the job and label name unique by adding file name with it,
//             this is only needed before paging as different file can have same
//             job number and label but after pafing its not needed as on a page
//             we will have only unique job and label
void
assembler_state::
process(bool makeunique)
{
  code_section csection = code_section::text;
  uint32_t index = 0;
  uint32_t eopnum = 0;
  std::string clabelname;
  jobid_type cjob_id = "";
  std::map<std::string, uint32_t> apply_label_map;
  for (const auto& data : m_data)
  {
    if (data->isLabel())
    {
      csection = code_section::data;
      clabelname = gen_label_name(makeunique, data);

      data->set_size(0);
      auto lb_it = m_labelmap.lower_bound(clabelname);
      if (lb_it != m_labelmap.end() && lb_it->first == clabelname)
        throw error(error::error_code::invalid_asm, "Label " + clabelname + " present multiple time in asm\n");
      m_labelmap.emplace_hint(lb_it, clabelname, std::make_shared<label>(clabelname, m_pos, index));
      m_labellist.emplace_back(clabelname);
    } else if (data->isOpcode()){
      const std::string& name = data->get_operation().get_name();
      if (!name.compare("start_job") || !name.compare("start_job_deferred") || !name.compare("start_cond_job_preempt"))
      {
        clabelname.clear();
        cjob_id = gen_job_name(makeunique, data, eopnum);
        auto j_it = m_jobmap.lower_bound(cjob_id);
        if (j_it != m_jobmap.end() && j_it->first == cjob_id)
          throw error(error::error_code::invalid_asm, "Job " + cjob_id + " present multiple time in asm\n");
        m_jobmap.emplace_hint(j_it, cjob_id, std::make_shared<job>(cjob_id, m_pos, index, eopnum, !name.compare("start_job_deferred")));
        m_jobids.push_back(cjob_id);
      }

      if (!name.compare("eof"))
      {
        m_jobmap[EOF_ID] = std::make_shared<job>(EOF_ID, m_pos, index, eopnum, false);
        m_jobids.push_back(EOF_ID);
      }

      const auto isa_it = m_isa->find(name);
      if (isa_it != m_isa->end())
      {
        const auto& op_args = data->get_operation().get_args();
        offset_type size = isa_it->second->encoded_size_in_text(*this, op_args);
        m_pos += size;
        data->set_size(size);
        if (!name.compare("eof"))
        {
          m_jobmap[EOF_ID]->set_end(m_pos);
          m_jobmap[EOF_ID]->set_end_index(index);
          cjob_id = std::to_string(-1);
        }
      } else if (!name.compare(".eop")) {
        const std::string eop_name = gen_eop_name(eopnum);
        m_jobmap[eop_name] = std::make_shared<job>(eop_name, m_pos, index, eopnum, false);
        m_jobids.push_back(eop_name);
        ++eopnum;
      } else {
        throw error(error::error_code::internal_error, "Invalid operation:" + name);
      }

      if (!name.compare("local_barrier"))
      {
        barrierid_type lbid = parse_barrier(data->get_operation().get_args()[0]);
        m_jobmap[cjob_id]->m_barrierids.push_back(lbid);
        m_localbarriermap[lbid].push_back(cjob_id);
      }

      if (!name.compare("launch_job"))
      {
        jobid_type launchjobid;
        launchjobid = gen_job_name(makeunique, data, eopnum);
        m_jobmap[cjob_id]->m_dependentjobs.push_back(launchjobid);
        m_joblaunchmap[launchjobid].push_back(cjob_id);
      }

      if (!name.compare("end_job"))
      {
        //TODO : assert if cjob_id not in m_jobmap
        m_jobmap[cjob_id]->set_end(m_pos);
        m_jobmap[cjob_id]->set_end_index(index);
        cjob_id.clear();
      }
      // maintain map of label and num_entries for apply_offset_57 / apply_offset_pl
      // only used in pager
      // since label are after control code ops we need to record intermediately and resolve it later
      if ((!name.compare("apply_offset_57") || !name.compare("apply_offset_pl")) && makeunique)
        apply_label_map[data->get_qualify_label(data->get_operation().get_args()[0].substr(1))]
                 = !name.compare("apply_offset_pl") ? 1 : std::stoul(data->get_operation().get_args()[1]);
    } else {
      throw error(error::error_code::internal_error, "Unknown type found!!!");
    }

    if (!clabelname.empty())
    {
      const std::string& opnm = data->get_operation().get_name();
      if (opnm != ".align" && opnm != ".eop" && opnm != "eof")
      {
        m_labelmap[clabelname]->increment_count(1);
        m_labelmap[clabelname]->increment_size(data->get_size());
      }
    }
    ++index;
    data->set_section(csection);
  }

  //convert label and num_entries to map of label and dependent labels
  std::unordered_map<std::string, size_t> label_index;
  for (size_t li = 0; li < m_labellist.size(); ++li)
    label_index.emplace(m_labellist[li], li);

  for (const auto& pair : apply_label_map)
  {
    if (pair.second == 1)
      continue;
    auto lit = label_index.find(pair.first);
    if (lit == label_index.end())
      throw error(error::error_code::internal_error, "label " + pair.first + " not found in label list!!!");
    const size_t label_idx = lit->second;
    for (uint32_t i = 0; i < pair.second; ++i)
    {
      auto label = get_label_at(label_idx+i);
      m_dependent_labelmap[pair.first].push_back(label.substr(1));
    }
  }
  //TODO launch job id sanity check
}

/*
 * This fuction parse different kind of input
 * 1. If string start with '@': it can be either pad name or label name
 * 2. If string start with s2mm_/mm2s_/mem_s2mm_/mem_mm2s_/shim_s2mm_/shim_mm2s_/tile_mm2s_/tile_s2mm_: it return actor number
 * 3. If string start with tile_: it return perticular col and row 32bit base address
 * 3. If string is hex number string (start with "0x"): it return decimal equavalent
 * 4. If string is numeric string: it will convert to decimal
 */
uint32_t assembler_state::parse_num_arg(const std::string &str) const {
  // check if its pad/label/actor
  for (const auto& [prefix, handler] : handlers) {
    if (str.rfind(prefix) == 0) {
      return handler(str);
    }
  }

  static const regex kTileRe{R"(^tile_(\d+)_(\d+)$)"};
  smatch m;
  if ((str[0] == 't') && regex_match(str, m, kTileRe) && (m.size() == 3))
  {
    constexpr static size_t row_mask = 0x1F;
    constexpr static size_t col_mask = 0x7F;
    constexpr static size_t col_shift = 5;
    const uint32_t col = std::stoi(m[1]);
    const uint32_t row = std::stoi(m[2]);
    return (((col & col_mask) << col_shift) | (row & row_mask));
  }
  else if (str.rfind("0x") == 0)
  { //parse hex string
    return std::stoul(str, nullptr , HEX_BASE);
  }
  else if (is_number(str))
  { //parse numeric string
    return std::stoul(str);
  }
  else {
    throw symbol_exception();
  }
}

void
assembler_state::
printstate() const
{
  //print state object
  //JOBS
  for (auto &it : m_jobmap)
  {
    std::cout << "JOB[" << it.first << "] =>\tm_jobid:" << it.second->get_jobid()
              << "  m_start:" << it.second->get_start() << "  m_end:"
              << it.second->get_end() << "  m_start_index:" << it.second->get_start_index()
              << "  m_end_index:" << it.second->get_end_index() << "  m_eopnum:"
              << it.second->get_eopnum() << '\n';
  }
  std::cout<<"\n";

  //LOCAL BARRIERS
  for (auto it : m_localbarriermap)
  {
    std::cout << "LBMAP[" << it.first << "] =>\t";
    for( auto v : it.second)
      std::cout << v << ", ";
    std::cout<<"\n";
  }
  std::cout<<"\n";

  //LABELS
  for (auto it : m_labelmap)
  {
    std::cout << "LABELS[" << it.first << "] =>\tm_name:" << it.second->get_name()
              << "  m_pos:" << it.second->get_pos() << "  m_index:"
              << it.second->get_index() << "  m_count:" << it.second->get_count()
              << "  m_size:" << it.second->get_size() << '\n';
  }
  std::cout<<"\n";
}

}
