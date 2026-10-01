// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_ENCODER_AIE2PS_PAGE_BUSY_POLL_H_
#define AIEBU_ENCODER_AIE2PS_PAGE_BUSY_POLL_H_

#include "assembler_state.h"
#include "specification/aie2ps/isa.h"

#include <vector>

namespace aiebu {

// Byte offset in the 16-byte merged ctrltext page header (CERT busy-poll hint).
constexpr size_t PAGE_HEADER_BUSY_POLL_HINT_BYTE = 12;

inline bool
opcode_implies_load(uint8_t opcode)
{
  switch (opcode) {
  case OPCODE_LOAD_PDI:
  case OPCODE_LOAD_CORES:
  case OPCODE_LOAD_CORES_CP:
  case OPCODE_PREEMPT:
  case OPCODE_START_COND_JOB_PREEMPT:
    return true;
  default:
    return false;
  }
}

inline bool
job_is_special(const assembler_state& state, const std::shared_ptr<job>& j)
{
  for (uint32_t idx = j->get_start_index(); idx <= j->get_end_index(); ++idx) {
    const std::string& op_name = state.m_data[idx]->get_operation().get_name();
    const auto isa_it = state.m_isa->find(op_name);
    if (isa_it == state.m_isa->end())
      continue;
    if (opcode_implies_load(isa_it->second->get_code()))
      return true;
  }
  return false;
}

// Returns 1 when CERT may busy-poll for poll/mask_poll/uc_dma_write_des_sync on this page.
// conditions:
// 1. just one normal job = 1
// 2. one special job = 1
// 3. consecutive special job (no normal on page/ with no consecutive normal job) = 1
// 4. consecutive normal job = 0
// 5. no consecutive normal job = 1
// 6. even one deferred job = 0
inline uint8_t
compute_page_busy_poll_hint(const assembler_state& state, const std::vector<jobid_type>& jobs)
{
  std::vector<jobid_type> ordered;
  ordered.reserve(jobs.size());
  for (const auto& jid : jobs) {
    if (jid == EOF_ID || jid.rfind(EOP_ID, 0) == 0)
      continue;
    ordered.push_back(jid);
  }
  // if we have 1 job in page it can be normal or special job so we return 1
  // as we cant have deferred job without a normal job which call launch_job.
  if (ordered.size() < 2)
    return 1;

  unsigned consecutive_normal = 0;

  for (const auto& jid : ordered) {
    const auto& j = state.m_jobmap.at(jid);
    if (j->is_deferred())
      return 0;
    if (job_is_special(state, j)) {
      consecutive_normal = 0;
    } else {
      consecutive_normal++;
      if (consecutive_normal > 1)
        return 0;
    }
  }

  return 1;
}

} // namespace aiebu

#endif // AIEBU_ENCODER_AIE2PS_PAGE_BUSY_POLL_H_
