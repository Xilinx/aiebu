# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

# Test ELF with empty debug sections.
add_test(NAME "dtrace_empty_elf_debug_generate_elf"
  COMMAND aiebu-asm
    -t aie2ps_config
    -j "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col/Work/aiebu_config.json"
    -o "${CMAKE_CURRENT_BINARY_DIR}/empty_elf_debug.elf"
    -f disabledump
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_empty_elf_debug"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/empty_elf_debug"
    "${CMAKE_CURRENT_BINARY_DIR}/control_empty_elf_debug.dat"
    "python"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/empty_elf_debug.elf" "DPU:dpu"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_empty_elf_debug" PROPERTIES DEPENDS "dtrace_empty_elf_debug_generate_elf")

add_test(NAME "dtrace_empty_elf_debug_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_empty_elf_debug.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/empty_elf_debug/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_empty_elf_debug_md5sum" PROPERTIES DEPENDS "dtrace_empty_elf_debug")
