# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2025 Advanced Micro Devices, Inc.

project(tests CXX C)

add_subdirectory(aie2ps-ctrlcode)
add_subdirectory(aie4-ctrlcode)


if (AIEBU_NATIVE_BUILD)
 add_subdirectory(aie2-ctrlcode)
endif()
add_subdirectory(cpp_test)

# Whomever added this test, please make it work when
# AIEBU is built as a submodule from XRT
if (NOT AIEBU_SUBMODULE)
  add_subdirectory(cmake-test)
  if (NOT CMAKE_CXX_COMPILER MATCHES "(arm64|ARM64)")
    add_subdirectory(dtrace_test)
  endif()
endif()
