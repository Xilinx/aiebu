# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.


if (MSVC AND (NOT (AIEBU_MSVC_LEGACY_LINKING)))
  # Use Hybrid linking on Windows
  add_compile_options($<$<CONFIG:Debug>:/MTd>
                      $<$<CONFIG:Release>:/MT>)
endif()

add_subdirectory(cpp_api)
add_subdirectory(c_api)
