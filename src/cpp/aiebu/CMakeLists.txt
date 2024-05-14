# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2024 Xilinx, Inc. All rights reserved.

INCLUDE (FindBoost)
message("-- Boost version: ${Boost_VERSION}")
find_package(Boost REQUIRED COMPONENTS program_options)

add_subdirectory(src)
add_subdirectory(utils)
