# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc.

# Assemble control code ASM for single col preemption model into ELF
add_test(NAME "aie4_1col_preempt_asm"
  COMMAND aiebu-asm -t aie4_config -j "${CMAKE_CURRENT_SOURCE_DIR}/config.json" -o 1col-preempt.elf -f disabledump
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Compare the md5sum of ELF generated in aie4_1col_preempt_asm test with that of the golden file
add_test(NAME "aie4_1col_preempt_asm_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/1col-preempt.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Assemble control code ASM with manually added ".target aie4" directive for single col preemption model into ELF
add_test(NAME "aie4_1col_preempt_asm_target"
  COMMAND aiebu-asm -t aie4_config -j "${CMAKE_CURRENT_SOURCE_DIR}/config_target.json" -o 1col-preempt_target.elf -f disabledump
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Compare the md5sum of ELF generated in aie4_1col_preempt_asm_target test with that of the golden file
add_test(NAME "aie4_1col_preempt_asm_target_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/1col-preempt_target.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold_target.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


set_tests_properties("aie4_1col_preempt_asm_md5sum" PROPERTIES DEPENDS "aie4_1col_preempt_asm")
set_tests_properties("aie4_1col_preempt_asm" PROPERTIES LABELS memcheck)
set_tests_properties("aie4_1col_preempt_asm_target_md5sum" PROPERTIES DEPENDS "aie4_1col_preempt_asm_target")
set_tests_properties("aie4_1col_preempt_asm_target" PROPERTIES LABELS memcheck)