# SPDX-License-Identifier: MIT
# Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.
cmake_minimum_required(VERSION 3.18.0)
project(AIEBU HOMEPAGE_URL https://gitenterprise.xilinx.com/XRT/aiebu)

include(cmake/settings.cmake)

add_subdirectory(src/cpp/aiebu)

if (AIEBU_FULL STREQUAL "ON")
  add_subdirectory(src/python)
  add_subdirectory(specification)
  add_subdirectory(scripts)
endif()

add_subdirectory(lib)
add_subdirectory(test)

include(cmake/cpack.cmake)
