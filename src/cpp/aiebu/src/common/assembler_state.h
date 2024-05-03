// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_COMMON_ASSEMBLER_STATE_H_
#define _AIEBU_COMMON_ASSEMBLER_STATE_H_

#include "asm/asm_parser.h"
#include "oparg.h"
#include "utils.h"
#include "symbol.h"
#include "ops.h"
#include <iostream>
#include <memory>
#include <map>

namespace aiebu {

class isa_op;

class symbol_exception : public std::exception
{

};

class job
{
  jobid_type m_jobid;
  offset_type m_start;
  offset_type m_end;
  uint32_t m_start_index;
  uint32_t m_end_index;
  pageid_type m_pagenum;
  uint32_t m_eopnum;
  bool m_isdeferred;

public:
  std::vector<barrierid_type> m_barrierids;
  std::vector<jobid_type> m_dependentjobs;

  job(jobid_type jobid, offset_type pos, uint32_t index, uint32_t eopnum, bool isdeferred)
  :m_jobid(jobid), m_start(pos), m_end(pos), m_start_index(index), m_end_index(index),
   m_pagenum(NO_PAGE), m_isdeferred(isdeferred), m_eopnum(eopnum) {}

  job(job* j)
  {
    j->m_jobid = m_jobid;
    j->m_start = m_start;
    j->m_end = m_end;
    j->m_start_index = m_start_index;
    j->m_end_index = m_end_index;
    j->m_pagenum = m_pagenum;
    j->m_isdeferred = m_isdeferred;
    j->m_eopnum = m_eopnum;
  }

  HEADER_ACCESS_GET_SET(jobid_type, jobid);
  HEADER_ACCESS_GET_SET(pageid_type, pagenum);
  HEADER_ACCESS_GET_SET(uint32_t, eopnum);
  HEADER_ACCESS_GET_SET(offset_type, start);
  HEADER_ACCESS_GET_SET(offset_type, end);
  HEADER_ACCESS_GET_SET(uint32_t, start_index);
  HEADER_ACCESS_GET_SET(uint32_t, end_index);
  offset_type get_size()
  {
    return m_end - m_start;
  }
};

class label
{
  std::string m_name;
  offset_type m_pos;
  uint32_t m_index;
  int32_t m_count = -1;
  uint64_t m_size = 0;
  pageid_type m_pagenum;

public:

  label(std::string& name, offset_type pos, uint32_t index):m_name(name), m_pos(pos), m_index(index) {}
  label(label *t)
  {
    t->m_name = m_name;
    t->m_pos = m_pos;
    t->m_index = m_index;
    t->m_pagenum = m_pagenum;
  }

  HEADER_ACCESS_GET_SET(pageid_type, pagenum);
  HEADER_ACCESS_GET_SET(uint64_t, size);
  HEADER_ACCESS_GET_SET(int32_t, count);
  HEADER_ACCESS_GET_SET(uint32_t, index);
  HEADER_ACCESS_GET_SET(offset_type, pos);
  const std::string& get_name() { return m_name; }
  void increment_size(uint64_t size)
  {
    m_size += size;
  }

  void increment_count(int32_t count)
  {
    m_count += count;
  }
};

class assembler_state
{
  offset_type m_pos = 0;
public:
  std::vector<std::shared_ptr<asm_data>>& m_data;
  std::vector<jobid_type> m_jobids;
  std::unordered_map<jobid_type, std::shared_ptr<job>> m_jobmap;
  std::unordered_map<std::string, std::shared_ptr<label>> m_labelmap; 
  std::unordered_map<barrierid_type, std::vector<jobid_type>> m_localbarriermap;
  std::unordered_map<jobid_type, std::vector<jobid_type>> m_joblaunchmap;
  std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> m_isa;


  assembler_state(std::shared_ptr<std::map<std::string,
                  std::shared_ptr<isa_op>>> isa,
                  std::vector<std::shared_ptr<asm_data>>& data);

  HEADER_ACCESS_GET_SET(offset_type, pos);

  void printstate() const;

  bool is_number(const std::string& s) const {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
  }

  uint32_t parse_num_arg(const std::string& str);

  void process();

  const std::vector<jobid_type> get_job_list() const
  {
    // get list of jobs
    std::vector<jobid_type> keys;
    std::transform(
      m_jobmap.begin(),
      m_jobmap.end(),
      std::back_inserter(keys),
      [](const std::unordered_map<jobid_type, std::shared_ptr<job>>::value_type &pair){return pair.first;});
    return std::move(keys);
  }

};

}
#endif //_AIEBU_COMMON_ASSEMBLER_STATE_H_
