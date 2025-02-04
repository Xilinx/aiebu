# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2025 Advanced Micro Devices, Inc.

project(tests CXX C)

if (AIEBU_FULL STREQUAL "ON")
  if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_subdirectory(control-code)
  endif()
endif()
add_subdirectory(aie2-ctrlcode)
add_subdirectory(cpp_test)


# Whomever added this test, please make it work when
# AIEBU is built as a submodule from XRT
if (NOT AIEBU_SUBMODULE)
#  add_subdirectory(cmake-test)
endif()
