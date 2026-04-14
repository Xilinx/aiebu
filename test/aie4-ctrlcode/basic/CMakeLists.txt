# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

# Assemble a basic control code ASM with .target directive for into ELF
add_test(NAME "aie4_target_basic_asm"
  COMMAND aiebu-asm -t aie4 -c "${CMAKE_CURRENT_SOURCE_DIR}/aie4_target_basic.asm" -L "${CMAKE_CURRENT_SOURCE_DIR}/" -o aie4_target_basic.elf -f disabledump
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Compare the md5sum of ELF generated in aie4_target_basic_asm test with that of the golden file
add_test(NAME "aie4_target_basic_asm_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/aie4_target_basic.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


set_tests_properties("aie4_target_basic_asm_md5sum" PROPERTIES DEPENDS "aie4_target_basic_asm")
set_tests_properties("aie4_target_basic_asm" PROPERTIES LABELS memcheck)