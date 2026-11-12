# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

# Test single uc script with debug format dwarf
# generate elf file
add_test(NAME "dtrace_single_col_generate_elf"
  COMMAND aiebu-asm
    -t aie2ps_config
    -j "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col/Work/aiebu_config.json"
    -o "${CMAKE_CURRENT_BINARY_DIR}/single_col.elf"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_single_col"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_single_col.dat"
    "json"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/single_col.elf" "DPU:dpu"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_single_col" PROPERTIES
  DEPENDS "dtrace_single_col_generate_elf")

# control buffer md5sum
add_test(NAME "dtrace_single_col_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_single_col.dat"
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_single_col_md5sum" PROPERTIES
  DEPENDS "dtrace_single_col")

# result json file md5sum
add_test(NAME "dtrace_single_col_result_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_single_col.json"
    "${CMAKE_CURRENT_SOURCE_DIR}/gold_result.json"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_single_col_result_compare" PROPERTIES
  DEPENDS "dtrace_single_col")
