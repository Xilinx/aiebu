// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include "assembler_state.h"
#include "aiebu_error.h"
#include "utils.h"
#include <unordered_map>
#include <functional>

namespace aiebu {

assembler_state::
assembler_state(std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> isa,
                std::vector<std::shared_ptr<asm_data>>& data,
                std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpad,
                std::map<std::string, uint32_t>& labelpageindex, uint32_t control_packet_index, bool makeunique)
                : m_isa(std::move(isa)), m_data(data), m_scratchpad(scratchpad),
                  m_labelpageindex(labelpageindex), m_control_packet_index(control_packet_index)
{
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
  for (auto data : m_data)
  {

    if (data->isLabel())
    {
      csection = code_section::data;
      clabelname = gen_label_name(makeunique, data);

      data->set_size(0);
      if (m_labelmap.find(clabelname) != m_labelmap.end())
        throw error(error::error_code::invalid_asm, "Label " + clabelname + " present multiple time in asm\n");
      m_labelmap[clabelname] = std::make_shared<label>(clabelname, m_pos, index);
    } else if (data->isOpcode()){
      std::string name = data->get_operation()->get_name();
      if (!name.compare("start_job") || !name.compare("start_job_deferred"))
      {
        clabelname.clear();
        cjob_id = gen_job_name(makeunique, data);
        if (m_jobmap.find(cjob_id) != m_jobmap.end())
          throw error(error::error_code::invalid_asm, "Job " + cjob_id + " present multiple time in asm\n");
        m_jobmap[cjob_id] = std::make_shared<job>(cjob_id, m_pos, index, eopnum, !name.compare("start_job_deferred"));
        m_jobids.push_back(cjob_id);
      }

      if (!name.compare("eof"))
      {
        m_jobmap[EOF_ID] = std::make_shared<job>(EOF_ID, m_pos, index, eopnum, false);
        m_jobids.push_back(EOF_ID);
      }

      if ((*m_isa).count(name) > 0)
      {
        offset_type size = (*m_isa)[name]->serializer(data->get_operation()->get_args())->size(*this);
        m_pos += size;
        data->set_size(size);
        if (!name.compare("eof"))
        {
          m_jobmap[EOF_ID]->set_end(m_pos);
          m_jobmap[EOF_ID]->set_end_index(index);
          cjob_id = -1;
        }
      } else if (!name.compare(".eop")) {
        m_jobmap[gen_eop_name(eopnum)] = std::make_shared<job>(gen_eop_name(eopnum), m_pos, index, eopnum, false);
        m_jobids.push_back(gen_eop_name(eopnum));
        ++eopnum;
      } else
        throw error(error::error_code::internal_error, "Invalid operation:" + name);

      if (!name.compare("local_barrier"))
      {
        barrierid_type lbid = parse_barrier(data->get_operation()->get_args()[0]);
        auto it = m_localbarriermap.find(lbid);
        if (it == m_localbarriermap.end())
          m_localbarriermap[lbid] = std::vector<jobid_type>();
        m_jobmap[cjob_id].get()->m_barrierids.push_back(lbid);
        m_localbarriermap[lbid].push_back(cjob_id);
      }

      if (!name.compare("launch_job"))
      {
        jobid_type launchjobid;
        launchjobid = gen_job_name(makeunique, data);
        m_jobmap[cjob_id]->m_dependentjobs.push_back(launchjobid);
        auto it = m_joblaunchmap.find(launchjobid);
        if (it == m_joblaunchmap.end())
          m_joblaunchmap[launchjobid] = std::vector<jobid_type>();
        m_joblaunchmap[launchjobid].push_back(cjob_id);
      }

      if (!name.compare("end_job"))
      {
        //TODO : assert if cjob_id not in m_jobmap
        m_jobmap[cjob_id]->set_end(m_pos);
        m_jobmap[cjob_id]->set_end_index(index);
        cjob_id.clear();
      }

    } else {
      throw error(error::error_code::internal_error, "Unknown type found!!!");
    }

    if (!clabelname.empty() && data->get_operation()->get_name().compare(".align") && data->get_operation()->get_name().compare(".eop"))
    {
      m_labelmap[clabelname]->increment_count(1);
      m_labelmap[clabelname]->increment_size(data->get_size());
    }
    ++index;
    data->set_section(csection);
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
uint32_t assembler_state::parse_num_arg(const std::string& str) {
  static const std::unordered_map<std::string, std::function<uint32_t(const std::string&)>> handlers = {
    {"@", [this](const std::string& s) -> uint32_t {
          //If string start with '@': it can be either pad name or label name
          auto key = s.substr(1);
          if (auto it = m_scratchpad.find(key); it != m_scratchpad.end())
            return it->second->get_base() + it->second->get_offset();
          if (auto it = m_labelmap.find(key); it != m_labelmap.end())
            return it->second->get_pos();
          throw error(error::error_code::invalid_asm, "Label " + key + " not present in label map\n");
    }},
    {"s2mm_", [this](const std::string& s) -> uint32_t { return get_actor("s2mm", s); }},
    {"mm2s_", [this](const std::string& s) -> uint32_t { return get_actor("mm2s", s); }},
    {"mem_s2mm_", [this](const std::string& s) -> uint32_t { return get_actor("mem_s2mm", s); }},
    {"mem_mm2s_", [this](const std::string& s) -> uint32_t { return get_actor("mem_mm2s", s); }},
    {"shim_s2mm_", [this](const std::string& s) -> uint32_t { return get_actor("shim_s2mm", s); }},
    {"shim_mm2s_", [this](const std::string& s) -> uint32_t { return get_actor("shim_mm2s", s); }},
    {"tile_s2mm_", [this](const std::string& s) -> uint32_t { return get_actor("tile_s2mm", s); }},
    {"tile_mm2s_", [this](const std::string& s) -> uint32_t { return get_actor("tile_mm2s", s); }}
  };

  // check if its pad/label/actor
  for (const auto& [prefix, handler] : handlers) {
    if (str.rfind(prefix) == 0) {
      return handler(str);
    }
  }

  if (str.rfind("tile_") == 0)
  {
    // Parse and return perticular col and row 32bit base address eg: tile_0_1
    constexpr static size_t col_start = 5;
    constexpr static size_t len_of_underscore = 1;
    constexpr static size_t row_mask = 0x1F;
    constexpr static size_t col_mask = 0x7F;
    constexpr static size_t col_shift = 5;
    size_t row_start = col_start + len_of_underscore + str.substr(col_start).rfind("_");
    uint32_t col = std::stoi(str.substr(col_start));
    uint32_t row = std::stoi(str.substr(row_start));
    return (((col & col_mask) << col_shift) | (row & row_mask));
  } else if (str.rfind("0x") == 0)
  { //parse hex string
    return std::stol(str.substr(2), nullptr , HEX_BASE);
  } else if (is_number(str))
  { //parse numeric string
    return std::stol(str);
  } else
    throw symbol_exception();
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
