// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.
#include "pager.h"

#include "aiebu/aiebu_error.h"

#include <unordered_set>

namespace aiebu {
namespace {

inline bool
opcode_is_out_of_order_list(const std::string& op_name)
{
  return op_name == "load_pdi" || op_name == "preempt" || op_name == "load_cores";
}

void
extractlabels_accum(assembler_state& state,
                    const std::shared_ptr<asm_data>& token,
                    std::vector<std::string>& out,
                    std::unordered_set<std::string>& seen,
                    const std::map<std::string, std::vector<std::string>>& dependent_labelmap)
{
  if (!token || token->isLabel())
    return;
  if (opcode_is_out_of_order_list(token->get_operation().get_name()))
    return;

  const std::vector<std::string> args = token->get_operation().get_args();
  for (const auto& arg : args) {
    if (arg.empty() || arg[0] != '@')
      continue;

    std::string lb = arg.substr(1);
    if (state.containscratchpads(lb))
      continue;
    lb = token->get_qualify_label(lb);

    const auto lit = state.m_labelmap.find(lb);
    if (lit == state.m_labelmap.end())
      throw error(error::error_code::internal_error, "Label not found " + lb);

    if (!seen.insert(lb).second)
      continue;

    out.push_back(lb);

    const auto dit = dependent_labelmap.find(lb);
    if (dit != dependent_labelmap.end()) {
      for (const auto& dep : dit->second) {
        if (seen.insert(dep).second)
          out.push_back(dep);
      }
    }

    const uint32_t index = lit->second->get_index();
    const int32_t count = lit->second->get_count();
    for (int32_t i = 0; i <= count; ++i) {
      extractlabels_accum(state,
                            state.m_data[static_cast<size_t>(index) + static_cast<size_t>(i)],
                            out,
                            seen,
                            dependent_labelmap);
    }
  }
}

void
extract_externallabels_accum(const std::shared_ptr<asm_data>& token,
                             std::vector<std::string>& out,
                             std::unordered_set<std::string>& seen)
{
  if (!token || token->isLabel())
    return;
  if (!opcode_is_out_of_order_list(token->get_operation().get_name()))
    return;

  const std::vector<std::string> args = token->get_operation().get_args();
  for (const auto& arg : args) {
    if (arg.empty() || arg[0] != '@')
      continue;
    std::string lb = arg.substr(1);
    lb = token->get_qualify_label(lb);
    if (seen.insert(lb).second)
      out.push_back(lb);
  }
}

} // namespace

template <typename T>
std::vector<T>
pager::
union_of_lists_inorder(std::vector<T>& vec1, std::vector<T>& vec2)
{
  // order of insertion is important
  std::unordered_set<T> seen;
  std::vector<T> vec3;

  // Insert elements from the first vector
  for (const auto& str : vec1) {
    if (seen.insert(str).second) {
      vec3.push_back(str);
    }
  }

  // Insert elements from the second vector
  for (const auto& str : vec2) {
    if (seen.insert(str).second) {
      vec3.push_back(str);
    }
  }
  return vec3;
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
    auto &name = token->get_operation().get_name();
    if (ALIGNMAP.count(name) > 0)
    {
      if (ALIGNMENT_16 == ALIGNMAP.at(name))
        dsize16 += state.m_labelmap[lb]->get_size();
      else if (ALIGNMENT_4 == ALIGNMAP.at(name))
        dsize4 += state.m_labelmap[lb]->get_size();
      else
        throw error(error::error_code::internal_error, name +" found in ALIGNMAP with allignment " + std::to_string(ALIGNMAP.at(name)));
    }
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
        throw error(error::error_code::internal_error, "Job " + j
                    + " having dependency with Job " + jobids[0] +
                    " are not expected to be on same page because of .eop");
      barriers = union_of_lists_inorder<barrierid_type>(barriers, state.m_jobmap[j]->m_barrierids);
    }
    jobids = union_of_lists_inorder<jobid_type>(jobids, state.m_localbarriermap[lbid]);
    ++i;
  }
  // Explicit dependencies (e.g. launch_job relation)
  jobids = union_of_lists_inorder<jobid_type>(jobids, pjob->m_dependentjobs);
  for (auto& dep_job:pjob->m_dependentjobs)
  {
    auto dep_job_list =  extractjobs(state, state.m_jobmap[dep_job]);
    jobids = union_of_lists_inorder<jobid_type>(jobids, dep_job_list);
  }
  return jobids;
}

std::vector<std::string>
pager::
extractlabels(assembler_state& state, std::shared_ptr<asm_data> token)
{
  std::vector<std::string> labels;
  std::unordered_set<std::string> seen;
  extractlabels_accum(state, token, labels, seen, state.get_dependent_labelmap());
  return labels;
}

std::vector<std::string>
pager::
extract_externallabels(assembler_state& /*state*/, std::shared_ptr<asm_data> token)
{
  std::vector<std::string> labels;
  std::unordered_set<std::string> seen;
  extract_externallabels_accum(token, labels, seen);
  return labels;
}

offset_type
pager::
extractjobsandlabels(assembler_state& state, std::shared_ptr<job> pjob,
                     std::vector<jobid_type>& job_list,
                     std::vector<std::string>& labels_list,
                     std::vector<std::string>& external_labels_list)
{
  /*
   * extract all releated(jobs which share local barrier with current jobid) jobs
   * and labels with jobid also return size of complete text section in that page
   */

  job_list = extractjobs(state, pjob);
  offset_type tsize = 0;
  const auto& dependent_labelmap = state.get_dependent_labelmap();
  std::unordered_set<std::string> labels_seen;
  std::unordered_set<std::string> external_seen;
  for (auto djid : job_list)
  {
    tsize +=  state.m_jobmap[djid]->get_end() - state.m_jobmap[djid]->get_start();
    for (auto j = state.m_jobmap[djid]->get_start_index(); j < state.m_jobmap[djid]->get_end_index() + 1; ++j)
    {
      const auto& token = state.m_data[j];
      extractlabels_accum(state, token, labels_list, labels_seen, dependent_labelmap);
      extract_externallabels_accum(token, external_labels_list, external_seen);
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
    if (ALIGNMENT_16 == ALIGNMAP.at(token->get_operation().get_name()))
      labels.emplace_back(lb);
  }

  for (auto lb : clist)
  {
    auto index = state.m_labelmap[lb]->get_index();
    auto token = state.m_data[index+1];
    if (ALIGNMENT_4 == ALIGNMAP.at(token->get_operation().get_name()))
      labels.emplace_back(lb);
  }

  if (clist.size() != labels.size())
    throw error(error::error_code::internal_error, "clist size" + std::to_string(clist.size()) + " != " +
          " labels size" + std::to_string(labels.size()));

  return labels;
}

//TODO: break this function to page constructor and book keeping helper.
void
pager::
assignpagenumber(assembler_state& state, uint32_t colnum,
                 std::vector<jobid_type>& jobs,
                 std::vector<std::string> &labels,
                 std::vector<std::string> &externallabels,
                 uint32_t &pagenum, std::vector<page> &pages,
                 bool islastpage, uint32_t tsize, uint32_t dsize,
                 uint32_t relative_page_index)
{
  // assign pagenumber to all instruction and create text and data section lists
  page lpage;
  lpage.set_colnum(colnum);
  lpage.set_pagenum(pagenum);
  lpage.set_islastpage(islastpage);

  if (jobs.size() == 0)
    return;

  if (jobs.size() == 1 && !jobs[0].compare(EOF_ID))
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
  if (state.m_jobmap.find(EOF_ID) == state.m_jobmap.end())
    throw error(error::error_code::invalid_asm , "INVALID ASM, EOF job not found");

  lpage.m_text.emplace_back(state.m_data[state.m_jobmap[EOF_ID]->get_start_index()]);
  auto aligner = dsize ? datasectionaligner(tsize) : 0;
  // add align for data section
  if (aligner)
    lpage.m_text.emplace_back(std::make_shared<asm_data>(operation(".align", "16"),
                                                    operation_type::op, code_section::text, 0,
                                                    (uint32_t)-1, 0, m_default_file_idx));

  uint32_t cur_page_len = PAGE_HEADER_SIZE + tsize + EOF_SIZE + aligner + dsize;
  cur_page_len = (((cur_page_len + 3) >> 2) << 2); // round off to next multiple of 4
  lpage.set_cur_page_len(cur_page_len);
  lpage.set_in_order_page_len(0);
  lpage.set_externallabels(externallabels);
  pages.emplace_back(std::move(lpage));
  if (pages.size() > 1 && pages.size()-2 >= relative_page_index)
    pages[pages.size() - 2].set_in_order_page_len(cur_page_len);
  ++pagenum;
}

uint32_t
pager::
pagify(assembler_state& state, uint32_t col, std::vector<page>& pages, uint32_t relative_page_index)
{
  // pagify the content
  offset_type page_tsize = 0;
  offset_type page_dsize = 0;
  uint32_t page_index = relative_page_index;
  std::vector<jobid_type> page_jobs;
  std::vector<std::string> page_labels;
  std::vector<std::string> page_external_labels;
  auto joblist = state.get_job_list();

  for( auto jobid : state.m_jobids)
  {
    auto pjob = state.m_jobmap[jobid];
    std::vector<jobid_type> job_list;
    std::vector<std::string> labels_list;
    std::vector<std::string> external_labels_list;
    auto it = std::find(page_jobs.begin(), page_jobs.end(), jobid);
    // job already added if job have page assigned or job taken in current page or jodid is 'eof'
    if (pjob->get_pagenum() != NO_PAGE || it != page_jobs.end() || !jobid.compare(EOF_ID))
      continue;

    if (is_eop(jobid))
    {
      if (!page_jobs.size())
        continue;
      // conclude the page
      assignpagenumber(state, col, page_jobs, page_labels, page_external_labels, page_index, pages, false, page_tsize, page_dsize, relative_page_index);
      page_tsize = page_dsize = 0;
      page_jobs.clear();
      page_labels.clear();
      page_external_labels.clear();
      continue;
    }

    // get total text section size, jobs and labels releated to jobid(current job) which are not already taken in current page
    // don't return jobs/labels for a job which is already in this page a second time (note that multiple jobs might depend on
    // the same job)
    offset_type tsize = extractjobsandlabels(state, pjob, job_list, labels_list, external_labels_list);
    // calculate alignment bytes needed bet text and data section
    // NOTE: data section is always 16 Byte aligned
    offset_type dsectionaligner = datasectionaligner(page_tsize + tsize);
    // get data section size for jobs(related to current job)
    offset_type dsize = getdatasectionsize(state, labels_list);

    offset_type jobdsectionaligner = datasectionaligner(tsize);

    //check if job can fit in one page
    if ((tsize + jobdsectionaligner + dsize + EOF_SIZE + PAGE_HEADER_SIZE) > m_page_size)
      throw error(error::error_code::internal_error, "text and data section size " +
                  std::to_string(tsize + jobdsectionaligner + dsize + EOF_SIZE + PAGE_HEADER_SIZE)
                  + " > pagesize(" + std::to_string(m_page_size) + ")");

    // check if current job fit in current page along with other jobs already in page
    if ((page_tsize + page_dsize + tsize + dsectionaligner + dsize + EOF_SIZE + PAGE_HEADER_SIZE) > m_page_size)
    {
      // if current job dont fit in current page, conclude the page
      assignpagenumber(state, col, page_jobs, page_labels, page_external_labels, page_index, pages, false, page_tsize, page_dsize, relative_page_index);
      page_tsize = page_dsize = 0;
      page_jobs.clear();
      page_labels.clear();
      page_external_labels.clear();
    }

    page_tsize += tsize;
    page_dsize += dsize;
    page_jobs.insert( page_jobs.end(), job_list.begin(), job_list.end() );
    page_labels = union_of_lists_inorder<std::string>(page_labels, labels_list);
    page_external_labels = union_of_lists_inorder<std::string>(page_external_labels, external_labels_list);
  }

  if (page_jobs.size())
  {
    assignpagenumber(state, col, page_jobs, page_labels, page_external_labels, page_index, pages, true, page_tsize, page_dsize, relative_page_index);
    page_index += 1;
  } else if (page_index > relative_page_index)
    pages[page_index - relative_page_index - 1].set_islastpage(true);
  return page_index;
}

}
