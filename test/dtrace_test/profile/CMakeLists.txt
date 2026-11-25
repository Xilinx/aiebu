# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

# Test profile script
add_custom_command(OUTPUT single_col.elf
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col/single_col.b64" single_col.elf
  DEPENDS "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col/single_col.b64"
  COMMENT "Decoding base64 ELF to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
  )

add_custom_target(dtrace_profile_elf ALL
  DEPENDS single_col.elf
  )

add_test(NAME "dtrace_profile"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/profile"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_profile.dat"
    "python"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/single_col.elf" "DPU:dpu"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_profile_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_profile.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/profile/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_profile_md5sum" PROPERTIES DEPENDS "dtrace_profile")
