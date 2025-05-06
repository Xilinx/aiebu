// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include <map>
#include <string>
#include <regex>

#include "utils.h"
#include "version.h"

namespace aiebu {

static const std::map<fragment, std::string> fragment_table = {
    // NOLINT
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

std::regex
get_regex(const std::vector<fragment>& pattern)
{
  std::string composite;
  for (auto frag : pattern) {
    composite += fragment_table.at(frag);
  }
  return std::regex(composite);
}

std::string
version_string()
{
  std::stringstream stream;
  version::print(stream);
  return stream.str();
}

}
