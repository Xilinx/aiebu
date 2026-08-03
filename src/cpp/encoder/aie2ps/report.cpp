// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.

#include <iostream>
#include "report.h"

namespace aiebu {

void
asm_report::
addpage(page& lpage,  std::shared_ptr<assembler_state> page_state, offset_type textsize, offset_type datasize, offset_type padsize)
{
  std::vector<jobid_type> &jobs = page_state->m_jobids;
  jobs.erase(std::remove(jobs.begin(), jobs.end(), "EOF"), jobs.end());
  m_colpages[lpage.get_colnum()].emplace_back(lpage.get_colnum(), lpage.get_pagenum(), textsize, datasize, padsize, jobs, page_state->m_localbarriermap, page_state->m_joblaunchmap);
}

void
asm_report::
summary(std::ostream& output) {
  output << "************************** ASSEMBLER REPORT **************************" << std::endl;
  output << "BUILD ID: " << m_build_id << std::endl;

  for (const auto& [col, pages] : m_colpages) {
    output << "COLUMN: " << col << std::endl;
    for (const auto& page : pages) {
      output << "\tPAGE NO.:          " << page.m_pagenum << std::endl;
      output << "\tJOBS:              ";
      for (const auto& j : page.m_jobids)
        output << j << ",";
      output << "\n\tSECTIONS:          .ctrltext." << page.m_colnum << "." << page.m_pagenum
             << ", .ctrldata." << page.m_colnum << "." << page.m_pagenum << std::endl;
      output << "\tTEXT SECTION SIZE: " << page.m_textsize << std::endl;
      output << "\tDATA SECTION SIZE: " << page.m_datasize << std::endl;
      output << "\tPAD SIZE: " << page.m_padsize << std::endl;
      output << "\tGRAPH:\n";

      for (const auto& [barrier, ids] : page.m_localbarriermap) {
        output << "\t      local barrier {" << barrier << "}: jobids ";
        for (const auto& id : ids)
          output << id << ",";
        output << std::endl;
      }

      for (const auto& [launcher, jobs] : page.m_joblaunchmap) {
        output << "\t      job launchers for {" << launcher << "}: jobids ";
        for (const auto& id : jobs)
          output << id << ",";
        output << std::endl;
      }
      output << std::endl;
    }
  }
}
}
