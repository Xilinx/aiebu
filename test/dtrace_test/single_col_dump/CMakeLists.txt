# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

# Test single uc script with debug format dump

add_custom_command(OUTPUT simple_int8_orig.elf
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/simple_int8_orig.b64" simple_int8_orig.elf
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/simple_int8_orig.b64"
  COMMENT "Decoding base64 ELF to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM)

add_custom_target(dtrace_single_col_dump_elf ALL
  DEPENDS simple_int8_orig.elf)

add_test(NAME "dtrace_single_col_dump"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col_dump"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_single.dat"
    "python"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/simple_int8_orig.elf" "none"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

# control buffer md5sum
add_test(NAME "dtrace_single_col_dump_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_single.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col_dump/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_single_col_dump_md5sum" PROPERTIES DEPENDS "dtrace_single_col_dump")

# result python file md5sum
add_test(NAME "dtrace_single_col_dump_result_md5sum"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_single.py"
  "${CMAKE_CURRENT_SOURCE_DIR}/gold_result.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_single_col_dump_result_md5sum" PROPERTIES DEPENDS "dtrace_single_col_dump")
