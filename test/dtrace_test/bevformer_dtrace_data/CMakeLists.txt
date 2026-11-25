# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.

# Test bevformer script
add_custom_command(OUTPUT bevformer_dtrace_data.elf
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/bevformer_dtrace_data.b64" bevformer_dtrace_data.elf
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/bevformer_dtrace_data.b64"
  COMMENT "Decoding base64 ELF to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
  )

add_custom_target(dtrace_bevformer_dtrace_data_elf ALL
  DEPENDS bevformer_dtrace_data.elf
  )

add_test(NAME "dtrace_bevformer_dtrace_data"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/bevformer_dtrace_data"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_bevformer_dtrace_data.dat"
    "python"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/bevformer_dtrace_data.elf" "DPU:test"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_bevformer_dtrace_data_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_bevformer_dtrace_data.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/bevformer_dtrace_data/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_bevformer_dtrace_data_md5sum" PROPERTIES DEPENDS "dtrace_bevformer_dtrace_data")

add_test(NAME "dtrace_bevformer_dtrace_data_result_md5sum"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_bevformer_dtrace_data.py"
  "${CMAKE_CURRENT_SOURCE_DIR}/gold_result.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_bevformer_dtrace_data_result_md5sum" PROPERTIES DEPENDS "dtrace_bevformer_dtrace_data")
