# SPDX-License-Identifier: MIT
# Copyright (C) 2024-2026 Advanced Micro Devices, Inc. All rights reserved.
cmake_minimum_required(VERSION 3.18...4.0)
project(AIEBU HOMEPAGE_URL https://github.com/Xilinx/aiebu VERSION 1.0)

if (WIN32 AND CMAKE_VERSION VERSION_LESS 3.24)
  message(FATAL_ERROR "CMake 3.24 or higher is required on Windows")
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
include(cmake/settings.cmake)
include(cmake/components.cmake)
include(cmake/version.cmake)

message("-- AIEBU_NATIVE_BUILD: ${AIEBU_NATIVE_BUILD}")
add_subdirectory(src/cpp)

add_subdirectory(specification)

if (AIEBU_CTRLCODE_CODEGEN)
  add_subdirectory(lib)
endif()

if (AIEBU_NATIVE_BUILD)
  add_subdirectory(test)
endif()

include(cmake/cpack.cmake)
