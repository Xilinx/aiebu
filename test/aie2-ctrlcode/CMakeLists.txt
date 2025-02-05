# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc.
add_subdirectory(save-restore)

# MD5 name-based uuid generation was corrected to be
# identical on all endian systems in 1.71.0 only.
if (${Boost_VERSION} VERSION_GREATER_EQUAL "1.71.0")
  add_subdirectory(basic)
endif()
