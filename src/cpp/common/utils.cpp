// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include <map>
#include <string>

#include "utils.h"
#include "version.h"
#include "metrics.h"

namespace aiebu {

static const std::map<fragment, std::string> fragment_table = { // NOLINT
    {fragment::begin_anchor_re, "^"},
    {fragment::end_anchor_re, "$"},
    {fragment::hex_re, "[[:space:]]*(0[xX][[:xdigit:]]+)[[:space:]]*"},
    {fragment::dec_re, "[[:space:]]*([[:digit:]]+)[[:space:]]*"},
    {fragment::add_dec_re, "[[:space:]]*\\+([[:digit:]]+)[[:space:]]*"},
    {fragment::l_brack_re, "[[:space:]]*\\([[:space:]]*"},
    {fragment::r_brack_re, "[[:space:]]*\\)[[:space:]]*"},
    {fragment::equal_re, "[[:space:]]*==[[:space:]]*"},
    {fragment::index_re, "[[:space:]]*\\[[[:space:]]*([[:xdigit:]]+)[[:space:]]*\\][[:space:]]*"},
    {fragment::l_curly_re, "[[:space:]]*\\{[[:space:]]*"},
    {fragment::r_curly_re, "[[:space:]]*\\}[[:space:]]*"},
    {fragment::column, "[[:space:]]*c\\[([[:digit:]]+)\\][[:space:]]*"},
    {fragment::row, "[[:space:]]*r\\[([[:digit:]]+)\\][[:space:]]*"},
};

aiebu::regex
get_regex(const std::vector<fragment>& pattern)
{
  std::string composite;
  for (auto frag : pattern) {
    composite += fragment_table.at(frag);
  }
  return aiebu::regex(composite);
}

std::string
version_string()
{
  std::stringstream stream;
  version::print(stream);
  return stream.str();
}

std::string
metrics_report()
{
  std::stringstream stream;
  const process_metrics met = get_process_metrics();
  stream << "CPU " << met.m_cpu_ms << " ms, Memory " << met.m_peak_kb << " KB";
  return stream.str();
}
}
