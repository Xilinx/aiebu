# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

set(Boost_USE_STATIC_LIBS ON)
INCLUDE (FindBoost)
message("-- Boost version: ${Boost_VERSION}")
find_package(Boost REQUIRED COMPONENTS program_options)

add_subdirectory(src)
add_subdirectory(utils)
