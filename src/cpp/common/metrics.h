// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_COMMOM_METRICS_H_
#define AIEBU_COMMOM_METRICS_H_

#include <cstdint>

namespace aiebu {

struct process_metrics {
  std::uint64_t m_peak_kb;  // peak resident set size
  double        m_cpu_ms;   // total user + system CPU time
};

process_metrics get_process_metrics();

}
#endif
