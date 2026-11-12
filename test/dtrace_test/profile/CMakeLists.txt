# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

# Test profile script for multi-graph ELF.
add_test(NAME "dtrace_profile"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/profile"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_profile.dat"
    "python"
    "elf" "${CMAKE_BINARY_DIR}/test/dtrace_test/elf_handle/multi-graph.elf" "DPU:subgraph_1"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_profile" PROPERTIES DEPENDS "dtrace_elf_handle_generate_elf")

add_test(NAME "dtrace_profile_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_profile.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/profile/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_profile_md5sum" PROPERTIES DEPENDS "dtrace_profile")
