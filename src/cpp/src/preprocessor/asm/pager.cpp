// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include "asm/pager.h"
#include "aiebu_error.h"
#include "utils.h"

namespace aiebu {

template <typename T>
std::vector<T>
pager::
union_of_lists_inorder(std::vector<T>& vec1, std::vector<T>& vec2)
{
  // Template to merge unique elements of 2 vector
  std::vector<T> vec3;
  std::merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), std::back_inserter(vec3));
  auto pte = std::unique(vec3.begin(), vec3.end());
  vec3.erase(pte, vec3.end());
  return std::move(vec3);
}

offset_type
pager::
datasectionaligner(offset_type size)
{
  // calculate allignment bytes between text and data section
  return (((size + EOF_SIZE) % DATA_SECTION_ALIGNMENT) > 0) ?
         (DATA_SECTION_ALIGNMENT - ((size + EOF_SIZE) % DATA_SECTION_ALIGNMENT)) : 0;
}

offset_type
pager::
getdatasectionsize(assembler_state& state, std::vector<std::string>& labels_list)
{
  // get data section size for label list
  offset_type dsize16 = 0;
  offset_type dsize4 = 0;
  for (auto &lb : labels_list)
  {
    auto token = state.m_data[state.m_labelmap[lb]->get_index() + 1];
    auto &name = token->get_operation()->get_name();
    if (ALIGNMAP.count(name) > 0)
      if (16 == ALIGNMAP.at(name))
        dsize16 += state.m_labelmap[lb]->get_size();
      else if (4 == ALIGNMAP.at(name))
        dsize16 += state.m_labelmap[lb]->get_size();
      else
        throw error(error::error_code::internal_error, name +" found in ALIGNMAP with allignment " + std::to_string(ALIGNMAP.at(name)));
    else
      throw error(error::error_code::internal_error, lb +" not found in ALIGNMAP!!!");
  }
  return dsize4 + dsize16;
}

std::vector<jobid_type>
pager::
extractjobs(assembler_state& state, std::shared_ptr<job> pjob)
{
  // Extract all jobs depending on jobid
  std::vector<jobid_type> jobids;
  jobids.push_back(pjob->get_jobid());
  std::vector<barrierid_type> barriers = pjob->m_barrierids;
  auto eopnum = pjob->get_eopnum();
  uint32_t i = 0;
  while( i < barriers.size())
  {
    auto lbid = barriers[i];
    auto jj = state.m_localbarriermap[lbid];
    for (auto j : jj)
    {
      if (eopnum != state.m_jobmap[j]->get_eopnum())
        throw error(error::error_code::internal_error, "Job " + std::to_string(j)
                    + " having dependency with Job " + std::to_string(jobids[0]) +
                    " are not expected to be on same page because of .eop");
      barriers = union_of_lists_inorder<barrierid_type>(barriers, state.m_jobmap[j]->m_barrierids);
    }
    jobids = union_of_lists_inorder<jobid_type>(jobids, state.m_localbarriermap[lbid]);
    ++i;
  }
  // Explicit dependencies (e.g. launch_job relation)
  jobids = union_of_lists_inorder<jobid_type>(jobids, pjob->m_dependentjobs);
  return std::move(jobids); 
}

std::vector<std::string>
pager::
extractlabels(assembler_state& state, std::shared_ptr<asm_data> token)
{
  // Extract all labels connected to token
  std::vector<std::string> labels;
  if (token->isLabel())
    return std::move(labels);

  for (auto &arg : token->get_operation()->get_args())
  {
    if (arg.rfind("@") == 0)
    {
      auto lb = arg.substr(1);
      if (state.m_labelmap.find(lb) == state.m_labelmap.end())
        throw error(error::error_code::internal_error, "Label not found " + lb);

      std::vector<std::string> vlb {lb};
      labels = union_of_lists_inorder<std::string>(labels, vlb);
      auto index = state.m_labelmap[lb]->get_index();
      for (auto i = 0; i <= state.m_labelmap[lb]->get_count(); ++i)
      {
        auto elb = extractlabels(state, state.m_data[index+i]);
        labels = union_of_lists_inorder<std::string>(labels, elb);
      }
    }
  }
  return std::move(labels);
}

offset_type
pager::
extractjobsandlabels(assembler_state& state, std::shared_ptr<job> pjob,
                     std::vector<jobid_type>& page_jobs,
                     std::vector<jobid_type>& job_list,
                     std::vector<std::string>& labels_list)
{
  /*
   * extract all releated(jobs which share local barrier with current jobid) jobs
   * and labels with jobid also return size of complete text section in that page
   */

  job_list = extractjobs(state, pjob);
  offset_type tsize = 0;
  for (auto djid : job_list)
  {
    tsize +=  pjob->get_end() - pjob->get_start();
    for (auto j = state.m_jobmap[djid]->get_start_index(); j < state.m_jobmap[djid]->get_end_index() + 1; ++j)
    {
      auto token = state.m_data[j];
      auto elb = extractlabels(state, token);
      labels_list = union_of_lists_inorder<std::string>(labels_list, elb);
    }
  }
  return tsize;
}

std::vector<std::string>
pager::
labelalignmentsorter(assembler_state& state, std::vector<std::string>& clist)
{
  // sort labels in order of there align
  std::vector<std::string> labels;
  for (auto lb : clist)
  {
    auto index = state.m_labelmap[lb]->get_index();
    auto token = state.m_data[index+1];
    if (16 == ALIGNMAP.at(token->get_operation()->get_name()))
      labels.emplace_back(lb);
  }

  for (auto lb : clist)
  {
    auto index = state.m_labelmap[lb]->get_index();
    auto token = state.m_data[index+1];
    if (4 == ALIGNMAP.at(token->get_operation()->get_name()))
      labels.emplace_back(lb);
  }

  if (clist.size() != labels.size())
    throw error(error::error_code::internal_error, "clist size" + std::to_string(clist.size()) + " != " +
          " labels size" + std::to_string(labels.size()));

  return std::move(labels);
}

void
pager::
assignpagenumber(assembler_state& state, uint32_t colnum,
                 std::vector<jobid_type>& jobs,
                 std::vector<std::string> &labels,
                 uint32_t &pagenum, std::vector<page> &pages,
                 bool islastpage)
{
  // assign pagenumber to all instruction and create text and data section lists
  page lpage;
  lpage.set_colnum(colnum);
  lpage.set_pagenum(pagenum);
  lpage.set_islastpage(islastpage);

  if (jobs.size() == 0)
    return;

  if (jobs.size() == 1 && jobs[0] == EOF_ID)
    return;

  // sort labels according to label alignment
  labels = labelalignmentsorter(state, labels);
  for (auto &djid : jobs)
  {
    state.m_jobmap[djid]->set_pagenum(pagenum);
    for (auto j = state.m_jobmap[djid]->get_start_index();
         j < state.m_jobmap[djid]->get_end_index() + 1; ++j)
    {
      state.m_data[j]->set_pagenum(pagenum);
      lpage.m_text.emplace_back(state.m_data[j]);
    }
  }
  for (auto &lb : labels)
  {
    state.m_labelmap[lb]->set_pagenum(pagenum);
    auto index = state.m_labelmap[lb]->get_index();
    for (auto i = 0; i <= state.m_labelmap[lb]->get_count(); ++i)
    {
      state.m_data[index+i]->set_pagenum(pagenum);
      lpage.m_data.emplace_back(state.m_data[index+i]);
    }
  }
  // add eof at end of page
  lpage.m_text.emplace_back(state.m_data[state.m_jobmap[EOF_ID]->get_start_index()]);
  // add align for data section
  lpage.m_text.emplace_back(std::make_shared<asm_data>(std::make_shared<operation>(".align", "16"),
                                                    operation_type::op, code_section::text, 0,
                                                    (uint32_t)-1, 0, ".align 16"));
  pages.emplace_back(std::move(lpage));
  ++pagenum;
}

void
pager::
pagify(assembler_state& state, uint32_t col, std::vector<page>& pages)
{
  // pagify the content
  offset_type pos = PAGE_HEADER_SIZE;
  offset_type page_tsize = 0;
  offset_type page_dsize = 0;
  uint32_t page_index = 0;
  std::vector<jobid_type> page_jobs;
  std::vector<std::string> page_labels;
  auto joblist = state.get_job_list();

  for( auto jobid : state.m_jobids)
  {
    auto pjob = state.m_jobmap[jobid];
    std::vector<jobid_type> job_list;
    std::vector<std::string> labels_list;
    auto it = std::find(page_jobs.begin(), page_jobs.end(), jobid);
    // job already added if job have page assigned or job taken in current page or jodid is 'eof'
    if (pjob->get_pagenum() != NO_PAGE || it != page_jobs.end() || jobid == EOF_ID)
      continue;

    if (jobid <= EOP_ID)
    {
      // conclude the page
      assignpagenumber(state, col, page_jobs, page_labels, page_index, pages, false);
      page_tsize = page_dsize = 0;
      page_jobs.clear();
      page_labels.clear();
      continue;
    }

    // get total text section size, jobs and labels releated to jobid(current job) which are not already taken in current page
    // don't return jobs/labels for a job which is already in this page a second time (note that multiple jobs might depend on
    // the same job)
    auto tsize = extractjobsandlabels(state, pjob, page_jobs, job_list, labels_list);

    // calculate alignment bytes needed bet text and data section
    // NOTE: data section is always 16 Byte aligned
    auto dsectionaligner = datasectionaligner(page_tsize + tsize);

    // get data section size for jobs(related to current job)
    auto dsize = getdatasectionsize(state, labels_list);

    auto jobdsectionaligner = datasectionaligner(tsize);

    //check if job can fit in one page
    if ((tsize + jobdsectionaligner + dsize + EOF_SIZE + PAGE_HEADER_SIZE) > m_page_size)
      throw error(error::error_code::internal_error, "text and data section size " +
                  std::to_string(tsize + jobdsectionaligner + dsize + EOF_SIZE + PAGE_HEADER_SIZE)
                  + " > pagesize(" + std::to_string(m_page_size) + ")");

    // check if current job fit in current page along with other jobs already in page
    if ((page_tsize + page_dsize + tsize + dsectionaligner + dsize + EOF_SIZE + PAGE_HEADER_SIZE) > m_page_size)
    {
      // if current job dont fit in current page, conclude the page
      assignpagenumber(state, col, page_jobs, page_labels, page_index, pages, false);
      page_tsize = page_dsize = 0;
      page_jobs.clear();
      page_labels.clear();
    }

    page_tsize += tsize;
    page_dsize += dsize;
    page_jobs.insert( page_jobs.end(), job_list.begin(), job_list.end() );
    page_labels.insert( page_labels.end(), labels_list.begin(), labels_list.end() );
  }

  if (page_jobs.size())
    assignpagenumber(state, col, page_jobs, page_labels, page_index, pages, true);
  else
    pages[page_index-1].set_islastpage(true);
}

}
