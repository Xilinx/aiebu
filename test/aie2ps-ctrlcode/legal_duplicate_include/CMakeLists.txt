# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

# Assemble a ctrlcode with mulitple inclusions of the same ASM file to ELF
add_test(NAME "aie2ps_legal_duplicate_include_asm"
  COMMAND aiebu-asm -t aie2ps -o aie_control.elf -c "${CMAKE_CURRENT_SOURCE_DIR}/ml_asm/merged_control.asm" -L "${CMAKE_CURRENT_SOURCE_DIR}/ml_asm" -L "${CMAKE_CURRENT_SOURCE_DIR}/asm" -j "${CMAKE_CURRENT_SOURCE_DIR}/external_buffer_id.json" -f disabledump)

# Compare the md5sum of ELF generated in duplicate_asm test with that of the golden file
add_test(NAME "aie2ps_legal_duplicate_include_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/aie_control.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie2ps_legal_duplicate_include_asm" PROPERTIES LABELS "benchmark")
set_tests_properties("aie2ps_legal_duplicate_include_md5sum" PROPERTIES DEPENDS "aie2ps_legal_duplicate_include_asm")
