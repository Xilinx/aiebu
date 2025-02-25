// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include <map>
#include <string>
#include <regex>

#include "utils.h"

namespace aiebu {
static const std::map<fragment, const char *> fragment_table = {
  {fragment::begin_anchor_re, "^"},
  {fragment::end_anchor_re, "$"},
  {fragment::hex_re, "[[:space:]]*(0[xX][[:xdigit:]]+)[[:space:]]*"},
  {fragment::l_brack_re, "[[:space:]]*\\([[:space:]]*"},
  {fragment::r_brack_re, "[[:space:]]*\\)[[:space:]]*"},
  {fragment::equal_re, "[[:space:]]*==[[:space:]]*"},
  {fragment::index_re, "[[:space:]]*\\[[[:space:]]*([[:xdigit:]]+)[[:space:]]*\\][[:space:]]*"},
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

}
