// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_ENCODER_AIE2PS_PAGE_BUSY_POLL_H_
#define AIEBU_ENCODER_AIE2PS_PAGE_BUSY_POLL_H_

#include "assembler_state.h"

#include <vector>

namespace aiebu {

// Byte offset in the 16-byte merged ctrltext page header (CERT busy-poll hint).
constexpr size_t PAGE_HEADER_BUSY_POLL_HINT_BYTE = 12;

inline bool
opcode_makes_job_special(const std::string& op_name)
{
  return op_name == "load_pdi" || op_name == "load_cores" || op_name == "load_cores_cp"
         || op_name == "preempt" || op_name == "start_cond_job_preempt";
}

inline bool
job_is_special(const assembler_state& state, const std::shared_ptr<job>& j)
{
  for (uint32_t idx = j->get_start_index(); idx <= j->get_end_index(); ++idx) {
    const std::string& op_name = state.m_data[idx]->get_operation().get_name();
    if (opcode_makes_job_special(op_name))
      return true;
  }
  return false;
}

// Returns 1 when CERT may busy-poll for poll/mask_poll/uc_dma_write_des_sync on this page.
inline uint8_t
compute_page_busy_poll_hint(assembler_state& state, const std::vector<jobid_type>& jobs)
{
  std::vector<jobid_type> ordered;
  ordered.reserve(jobs.size());
  for (const auto& jid : jobs) {
    if (jid == EOF_ID || jid.rfind(EOP_ID, 0) == 0)
      continue;
    ordered.push_back(jid);
  }
  if (ordered.empty())
    return 1;

  unsigned normal_count = 0;
  unsigned special_count = 0;
  unsigned deferred_count = 0;
  unsigned consecutive_normal = 0;

  for (const auto& jid : ordered) {
    const auto& j = state.m_jobmap.at(jid);
    const bool special = job_is_special(state, j);
    const bool deferred = j->is_deferred();
    if (deferred)
      deferred_count++;
    if (special) {
      special_count++;
      consecutive_normal = 0;
    } else if (deferred) {
      consecutive_normal = 0;
    } else {
      normal_count++;
      consecutive_normal++;
      if (consecutive_normal > 1)
        return 0;
    }
  }

  if (special_count > 1 && normal_count == 0)
    return 0;

  if (deferred_count >= 1 && normal_count >= 1)
    return 0;

  return 1;
}

} // namespace aiebu

#endif // AIEBU_ENCODER_AIE2PS_PAGE_BUSY_POLL_H_
