// SPDX-License-Identifier: MIT
// Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.
#ifndef AIEBU_COMMON_BUILD_INFO_H_
#define AIEBU_COMMON_BUILD_INFO_H_

#include <cstdint>
#include <string_view>

namespace aiebu {

struct build_info {
  std::string_view build_version;
  std::string_view build_version_branch;
  std::string_view build_version_hash;
  std::string_view build_version_hash_date;
  std::string_view modified_files;
  std::string_view boost_version_string;
  std::string_view regex_engine;
  std::string_view cxx_compiler_id;
  std::string_view cxx_compiler_version;
  std::string_view build_type;
  std::uint32_t version_code;
  int patch;
  int head_commits;
  int branch_commits;
};

build_info get_build_info();

} // namespace aiebu

#endif // AIEBU_COMMON_BUILD_INFO_H_
