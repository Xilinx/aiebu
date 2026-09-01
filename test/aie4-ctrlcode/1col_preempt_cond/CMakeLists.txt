# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

add_test(NAME "aie4_1col_preempt_cond_asm"
  COMMAND aiebu-asm -t aie4_config -j "${CMAKE_CURRENT_SOURCE_DIR}/config.json" -o 1col_preempt_cond.elf -f disabledump
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Compare the md5sum of the generated ELF against the golden file
add_test(NAME "aie4_1col_preempt_cond_asm_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/1col_preempt_cond.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie4_1col_preempt_cond_asm_md5sum" PROPERTIES DEPENDS "aie4_1col_preempt_cond_asm")
set_tests_properties("aie4_1col_preempt_cond_asm" PROPERTIES LABELS memcheck)
