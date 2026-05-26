# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

# Assemble control code ASM with zero-hint_bitmap (hintmap_0 = 0x0) alongside a
# non-zero hintmap (hintmap_1 = 0xf).  The zero-hintmap PREEMPT points must
# produce minimal dummy save/restore jobs (NOP / LOAD_LAST_PDI) with no BD
# configuration or stream-switch routing.
add_test(NAME "aie4_1col_preempt_0hint_asm"
  COMMAND aiebu-asm -t aie4_config -j "${CMAKE_CURRENT_SOURCE_DIR}/config.json" -o 1col-preempt-0hint.elf -f disabledump
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Compare the md5sum of the generated ELF against the golden file
add_test(NAME "aie4_1col_preempt_0hint_asm_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/1col-preempt-0hint.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie4_1col_preempt_0hint_asm_md5sum" PROPERTIES DEPENDS "aie4_1col_preempt_0hint_asm")
set_tests_properties("aie4_1col_preempt_0hint_asm" PROPERTIES LABELS memcheck)
