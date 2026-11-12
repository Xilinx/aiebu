# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

# Test multi uc script and multi page control block with debug format dump

add_custom_command(OUTPUT multi_uc_6.elf
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/multi_uc_6.b64" multi_uc_6.elf
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/multi_uc_6.b64"
  COMMENT "Decoding base64 ELF to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM)

add_custom_target(dtrace_multi_col_dump_elf ALL
  DEPENDS multi_uc_6.elf)

add_test(NAME "dtrace_multi_col_dump"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col_dump"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi.dat"
    "json"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/multi_uc_6.elf" "none"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

# control buffer md5sum
add_test(NAME "dtrace_multi_col_dump_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col_dump/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multi_col_dump_md5sum" PROPERTIES DEPENDS "dtrace_multi_col_dump")

# result json file md5sum
add_test(NAME "dtrace_multi_col_dump_result_md5sum"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi.json"
  "${CMAKE_CURRENT_SOURCE_DIR}/gold_result.json"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multi_col_dump_result_md5sum" PROPERTIES DEPENDS "dtrace_multi_col_dump")
