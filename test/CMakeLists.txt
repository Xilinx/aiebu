# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2023 Advanced Micro Devices, Inc.

project(tests CXX C)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  add_subdirectory(control-code)
endif()
add_subdirectory(aie2-ctrlcode)
