// SPDX-License-Identifier: MIT
// Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.

#include "common/build_info.h"
#include "version.h"

namespace aiebu {

build_info
get_build_info()
{
  return build_info{
    aiebu_build_version,
    aiebu_build_version_branch,
    aiebu_build_version_hash,
    aiebu_build_version_hash_date,
    aiebu_modified_files,
    aiebu_boost_version_string,
    aiebu_regex_engine,
    aiebu_cxx_compiler_id,
    aiebu_cxx_compiler_version,
    aiebu_build_type,
    static_cast<std::uint32_t>(AIEBU_VERSION_CODE),
    AIEBU_PATCH,
    AIEBU_HEAD_COMMITS,
    AIEBU_BRANCH_COMMITS,
  };
}

} // namespace aiebu
