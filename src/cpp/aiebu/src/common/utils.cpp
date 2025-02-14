// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include <map>
#include <string>
#include <regex>

#include "utils.h"

namespace aiebu {
static const std::map<fragment, const char *> fragment_table = {
  {fragment::HEX_RE, "[[:space:]]*(0[xX][[:xdigit:]]+)[[:space:]]*"},
  {fragment::L_BRACK_RE, "[[:space:]]*\\([[:space:]]*"},
  {fragment::R_BRACK_RE, "[[:space:]]*\\)[[:space:]]*"},
};

const std::string HEX_RE("[[:space:]]*(0[xX][[:xdigit:]]+)[[:space:]]*");
const std::string L_BRACK_RE("[[:space:]]*\\([[:space:]]*");
const std::string R_BRACK_RE("[[:space:]]*\\)[[:space:]]*");

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
