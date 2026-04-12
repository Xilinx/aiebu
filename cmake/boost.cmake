# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.
if(POLICY CMP0144)
  cmake_policy(SET CMP0144 NEW)
endif()

# First, find Boost to determine the version (minimum 1.65 for property_tree support)
# Use MODULE mode (default) which is more flexible in finding Boost installations
find_package(Boost 1.65.0 REQUIRED)

message(STATUS "Boost version: ${Boost_VERSION}")
message(STATUS "Boost include dir: ${Boost_INCLUDE_DIRS}")

# Determine which regex library to use based on Boost version
# - Boost < 1.76.0: use std::regex (avoid linking libboost_regex)
# - Boost >= 1.76.0: use boost::regex (header-only)
set(USE_BOOST_REGEX OFF)

if(Boost_VERSION_STRING VERSION_GREATER_EQUAL "1.76.0")
  # Boost >= 1.76.0: use boost::regex (header-only, no linking needed)
  set(USE_BOOST_REGEX ON)
  set(AIEBU_REGEX_ENGINE "boost::regex")
  message(STATUS "Boost ${Boost_VERSION_STRING} >= 1.76.0 - Using boost::regex (header-only)")
else()
  # Boost < 1.76.0: use std::regex (avoids needing libboost_regex)
  set(AIEBU_REGEX_ENGINE "std::regex")
  message(STATUS "Boost ${Boost_VERSION_STRING} < 1.76.0 - Using std::regex")
endif()
message(STATUS "AIEBU_REGEX_ENGINE: ${AIEBU_REGEX_ENGINE}")

# Configure regex library preprocessor macro
if(USE_BOOST_REGEX)
  # Define preprocessor macro to enable boost::regex in wrapper
  add_compile_definitions(USE_BOOST_REGEX)
endif()

# Some later versions of boost spews warnings form property_tree
# but can be disabled with this setting
add_compile_options("-DBOOST_BIND_GLOBAL_PLACEHOLDERS")
include_directories(${Boost_INCLUDE_DIRS})
