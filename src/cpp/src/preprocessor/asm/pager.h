// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_ASM_PAGER_H_
#define _AIEBU_PREPROCESSOR_ASM_PAGER_H_

#include <map>
#include "asm/asm_parser.h"
#include "assembler_state.h"
#include "asm/page.h"

namespace aiebu {

class pager
{
  uint32_t m_page_size;
  constexpr static offset_type PAGE_HEADER_SIZE = 16;
  constexpr static offset_type EOF_SIZE = 4;
  constexpr static offset_type DATA_SECTION_ALIGNMENT = 16;
  std::map<std::string , offset_type> ALIGNMAP = {{"uc_dma_bd", 16}, {"uc_dma_bd_shim", 16}, {".long", 4}};

  template <typename T>
  std::vector<T> union_of_lists_inorder(std::vector<T> &vec1, std::vector<T>& vec2);

  offset_type datasectionaligner(offset_type size);

  offset_type getdatasectionsize(assembler_state& state,
                                 std::vector<std::string>& labels_list);

  std::vector<jobid_type> extractjobs(assembler_state& state,
                                      std::shared_ptr<job> pjob);

  std::vector<std::string> extractlabels(assembler_state& state,
                                         std::shared_ptr<asm_data> token);
  
  offset_type extractjobsandlabels(assembler_state& state,
                                   std::shared_ptr<job> pjob,
                                   std::vector<jobid_type>& page_jobs,
                                   std::vector<jobid_type>& job_list,
                                   std::vector<std::string>& labels_list);

  std::vector<std::string> labelalignmentsorter(assembler_state& state,
                                                std::vector<std::string>& clist);

  void assignpagenumber(assembler_state& state, uint32_t colnum,
                        std::vector<jobid_type>& jobs,
                        std::vector<std::string> &labels,
                        uint32_t &pagenum,
                        std::vector<page> &pages,
                        bool islastpage);

public:
  pager(uint32_t page_size): m_page_size(page_size) {}  
  void pagify(assembler_state& state, uint32_t col, std::vector<page>& pages);

};

}
#endif //_AIEBU_PREPROCESSOR_ASM_PAGER_H_

