# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

# Test ELF with empty debug sections.
add_custom_command(OUTPUT empty_elf_debug.elf
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/empty_elf_debug.b64" empty_elf_debug.elf
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/empty_elf_debug.b64"
  COMMENT "Decoding base64 ELF to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
  )

add_custom_target(dtrace_empty_elf_debug_elf ALL
  DEPENDS empty_elf_debug.elf
  )

add_test(NAME "dtrace_empty_elf_debug"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/empty_elf_debug"
    "${CMAKE_CURRENT_BINARY_DIR}/control_empty_elf_debug.dat"
    "python"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/empty_elf_debug.elf" "DPU:dpu"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_empty_elf_debug_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_empty_elf_debug.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/empty_elf_debug/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_empty_elf_debug_md5sum" PROPERTIES DEPENDS "dtrace_empty_elf_debug")
