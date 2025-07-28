# SPDX-License-Identifier: MIT
# Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.
cmake_minimum_required(VERSION 3.18...4.0)
project(AIEBU HOMEPAGE_URL https://github.com/Xilinx/aiebu VERSION 1.0)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
include(cmake/settings.cmake)
include(cmake/components.cmake)
include(cmake/version.cmake)

message("-- AIEBU_NATIVE_BUILD: ${AIEBU_NATIVE_BUILD}")
add_subdirectory(src/cpp)

add_subdirectory(specification)

add_subdirectory(lib)
add_subdirectory(test)

include(cmake/cpack.cmake)
