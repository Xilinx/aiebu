// SPDX-License-Identifier: MIT
// Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.
#include <map>
#include <string>
#include <sstream>

#include "utils.h"
#include "common/build_info.h"
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
  const build_info bi = get_build_info();
  std::stringstream output;
  output << "     AIEBU Build Version: " << bi.build_version << '\n';
  output << "    Build Version Branch: " << bi.build_version_branch << '\n';
  output << "      Build Version Hash: " << bi.build_version_hash << '\n';
  output << " Build Version Hash Date: " << bi.build_version_hash_date << '\n';
  output << "           Boost version: " << bi.boost_version_string << '\n';
  output << "            Regex engine: " << bi.regex_engine << '\n';
  output << "         CXX compiler ID: " << bi.cxx_compiler_id << '\n';
  output << "    CXX compiler version: " << bi.cxx_compiler_version << '\n';
  output << "              Build type: " << bi.build_type << '\n';
  output << "            Version code: " << bi.version_code << '\n';

  std::string modified_files(bi.modified_files);
  if (modified_files.empty())
    return output.str();

  const std::string& delimiters = ",";      // Our delimiter
  std::string::size_type last_pos = 0;
  int running_index = 1;
  while (last_pos < modified_files.length() + 1) {
    if (running_index == 1)
      output << "  Current Modified Files: ";
    else
      output << "                          ";

    output << running_index++ << ") ";

    auto pos = modified_files.find_first_of(delimiters, last_pos);

    if (pos == std::string::npos)
      pos = modified_files.length();

    output << modified_files.substr(last_pos, pos - last_pos) << '\n';

    last_pos = pos + 1;
  }
  return output.str();
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
