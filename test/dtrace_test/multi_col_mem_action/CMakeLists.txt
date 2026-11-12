# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

# Test mem action across multiple lines and columns.
# Uses the DWARF ELF from dtrace_multi_col_generate_elf (DPU kernel, instance dpu).
add_test(NAME "dtrace_multi_col_mem_action"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col_mem_action"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi_col_mem_action.dat"
    "python"
    "elf" "${CMAKE_BINARY_DIR}/test/dtrace_test/multi_col/multi_col.elf" "DPU:dpu"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multi_col_mem_action" PROPERTIES DEPENDS "dtrace_multi_col_generate_elf")

add_test(NAME "dtrace_multi_col_mem_action_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi_col_mem_action.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col_mem_action/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multi_col_mem_action_md5sum" PROPERTIES DEPENDS "dtrace_multi_col_mem_action")

add_test(NAME "dtrace_multi_col_mem_action_result_md5sum"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi_col_mem_action.py"
  "${CMAKE_CURRENT_SOURCE_DIR}/gold_result.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multi_col_mem_action_result_md5sum" PROPERTIES DEPENDS "dtrace_multi_col_mem_action")
