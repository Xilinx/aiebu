# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.
if(POLICY CMP0144)
  cmake_policy(SET CMP0144 NEW)
endif()

# We use header only libraries for most components
# Boost.Regex is header-only from 1.76.0+ (when using C++11 or later)
# Require minimum Boost 1.76.0 for header-only Boost.Regex support
if(${CMAKE_VERSION} VERSION_LESS "3.30")
  find_package(Boost 1.76.0 REQUIRED COMPONENTS regex)
else()
  find_package(Boost 1.76.0 CONFIG REQUIRED COMPONENTS regex)
endif()

message(STATUS "Boost version: ${Boost_VERSION}")
message(STATUS "Boost include dir: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
message(STATUS "Boost.Regex: using header-only mode (Boost >= 1.76.0)")

# Some later versions of boost spews warnings form property_tree
# but can be disabled with this setting
add_compile_options("-DBOOST_BIND_GLOBAL_PLACEHOLDERS")
include_directories(${Boost_INCLUDE_DIRS})