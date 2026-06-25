// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSED_OUTPUT_H_
#define _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSED_OUTPUT_H_

#include "asm/page.h"
#include "symbol.h"
#include "preprocessed_output.h"
#include <utility>

namespace aiebu {

class aie2ps_preprocessed_output : public preprocessed_output
{
  class coldata
  {
    public:
    std::vector<page> m_pages;
    std::map<std::string, std::shared_ptr<scratchpad_info>> m_scratchpad;
    std::map<std::string, uint32_t> m_labelpageindex;
    uint32_t m_control_packet_index = 0xFFFFFFFF; // default value if control packet not present
    coldata(std::vector<page> pages, std::map<std::string, std::shared_ptr<scratchpad_info>> scratchpad, std::map<std::string, uint32_t> labelpageindex, uint32_t control_packet_index): m_pages(std::move(pages)), m_scratchpad(std::move(scratchpad)), m_labelpageindex(std::move(labelpageindex)), m_control_packet_index(control_packet_index) {}
  };
  std::map<uint32_t, std::shared_ptr<coldata>> m_coldata;
  std::vector<symbol> m_sym;
  std::vector<annotation_type> m_annotation_list;
  asm_dump_flag isdebug = asm_dump_flag::text;
  uint32_t m_optimization_level = 0;
  std::shared_ptr<const partition_info> m_partition;
  std::shared_ptr<const target_info> m_target;
  std::shared_ptr<const aie_row_topology_info> m_aie_row_topology;
  std::map<std::string, std::vector<char>> m_ctrlpkt;
  std::map<uint32_t, std::string> m_ctrlpkt_id_map;
  std::shared_ptr<const detail::filename_table> m_filename_table;
public:

  explicit aie2ps_preprocessed_output(std::shared_ptr<const partition_info> partition,
                                      std::shared_ptr<const target_info> target = nullptr,
                                      std::shared_ptr<const aie_row_topology_info> aie_row_topology = nullptr)
    : m_partition(std::move(partition)), m_target(std::move(target)), m_aie_row_topology(std::move(aie_row_topology)) {}

  std::shared_ptr<const partition_info> get_partition_info() const { return m_partition; }
  std::shared_ptr<const target_info> get_target_info() const { return m_target; }
  std::shared_ptr<const aie_row_topology_info> get_aie_row_topology_info() const { return m_aie_row_topology; }

  void set_coldata(const uint32_t col, const std::vector<page> &pages, std::map<std::string, std::shared_ptr<scratchpad_info>> &scratchpad, std::map<std::string, uint32_t>& labelpageindex, uint32_t control_packet_index)
  {
    m_coldata[col] = std::make_shared<coldata>(pages, scratchpad, labelpageindex, control_packet_index);
  }

  const std::map<uint32_t, std::shared_ptr<coldata>>& get_coldata() const
  {
    return m_coldata;
  }

  std::vector<symbol>& get_symbols()
  {
    return m_sym;
  }

  void add_symbols(std::vector<symbol>& syms)
  {
    m_sym = std::move(syms);
  }

  void set_annotations(std::vector<annotation_type> annotation_list)
  {
    m_annotation_list = std::move(annotation_list);
  }

  std::vector<annotation_type> get_annotations() { return std::move(m_annotation_list); }

  void set_debug(asm_dump_flag flag)
  {
    isdebug = flag;
  }

  asm_dump_flag get_debug() const
  {
    return isdebug;
  }
  void set_optmization(uint32_t value)
  {
    m_optimization_level = value;
  }
  uint32_t get_optimization_level()
  {
    return m_optimization_level;
  }

  void set_ctrlpkt(std::map<std::string, std::vector<char>>& ctrlpkt)
  {
    m_ctrlpkt = ctrlpkt;
  }

  std::map<std::string, std::vector<char>>& get_ctrlpkt()
  {
    return m_ctrlpkt;
  }

  std::map<uint32_t, std::string>& get_ctrlpkt_id_map()
  {
    return m_ctrlpkt_id_map;
  }

  void set_ctrlpkt_id_map( std::map<uint32_t, std::string>& ctrlpkt_id_map)
  {
    m_ctrlpkt_id_map = ctrlpkt_id_map;
  }

  void set_filename_table(std::shared_ptr<const detail::filename_table> table) {
    m_filename_table = std::move(table);
  }

  const std::shared_ptr<const detail::filename_table>& get_filename_table() const {
    return m_filename_table;
  }
};

template <typename T>
class asm_config_preprocessed_output: public config_preprocessed_output_base
{
  std::map<std::string, std::map<std::string, std::shared_ptr<T>>> m_output;

public:
  const std::map<std::string, std::map<std::string, std::shared_ptr<T>>>&
  get_kernel_map() const { return m_output; }

  void add_kernel_map(const std::string& kernel, const std::string& instance, std::shared_ptr<T> val) {
    m_output[kernel][instance] = val;
  }
};

}
#endif //_AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSED_OUTPUT_H_
