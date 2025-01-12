// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSED_OUTPUT_H_
#define _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSED_OUTPUT_H_

#include "asm/page.h"
#include "preprocessed_output.h"

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
public:
  aie2ps_preprocessed_output() {}

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
};

}
#endif //_AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSED_OUTPUT_H_
