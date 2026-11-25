# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

# Test multi uc script with debug format dwarf
add_custom_command(OUTPUT multi_col.elf
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/multi_col.b64" multi_col.elf
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/multi_col.b64"
  COMMENT "Decoding base64 ELF to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
  )

add_custom_target(dtrace_multi_col_elf ALL
  DEPENDS multi_col.elf
  )

add_test(NAME "dtrace_multi_col"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi_col.dat"
    "python"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/multi_col.elf" "DPU:dpu"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

# control buffer md5sum
add_test(NAME "dtrace_multi_col_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi_col.dat"
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multi_col_md5sum" PROPERTIES
  DEPENDS "dtrace_multi_col")

# result python file md5sum
add_test(NAME "dtrace_multi_col_result_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi_col.py"
    "${CMAKE_CURRENT_SOURCE_DIR}/gold_result.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multi_col_result_compare" PROPERTIES
  DEPENDS "dtrace_multi_col")

# verify elf file
find_program(LLVM_DWARFDUMP llvm-dwarfdump)
if (LLVM_DWARFDUMP)
  add_test(NAME "dwarf_verify_multi_col"
    COMMAND ${LLVM_DWARFDUMP} --verify "${CMAKE_CURRENT_BINARY_DIR}/multi_col.elf"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()
