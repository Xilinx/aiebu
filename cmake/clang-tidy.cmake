# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.
if (XRT_CLANG_TIDY STREQUAL "ON")
  find_program(CLANG_TIDY "clang-tidy")
  if(NOT CLANG_TIDY)
    message("clang-tidy not found, cannot enable static analysis")
  else()
    message("-- Enabling clang-tidy")
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
  endif()
endif()
