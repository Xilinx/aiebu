# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

# Test single uc script with debug format dwarf
add_custom_command(OUTPUT single_col.elf
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/single_col.b64" single_col.elf
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/single_col.b64"
  COMMENT "Decoding base64 ELF to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
  )

add_custom_target(dtrace_single_col_elf ALL
  DEPENDS single_col.elf
  )

add_test(NAME "dtrace_single_col"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_single_col.dat"
    "json"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/single_col.elf" "DPU:dpu"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

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
