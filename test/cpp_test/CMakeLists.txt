# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.


if (MSVC AND (NOT (AIEBU_MSVC_LEGACY_LINKING)))
  # Use Hybrid linking on Windows
  add_compile_options(/MT$<$<CONFIG:Debug>:d>  # static linking with the CRT
    )
endif()

add_subdirectory(cpp_api)

if (NOT MSVC)
  add_subdirectory(c_api)
endif()
