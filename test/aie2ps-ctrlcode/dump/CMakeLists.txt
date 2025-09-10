# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc.

set (ADUMP $<TARGET_FILE:aiebu-dump>)

# Assemble a ctrlcode txn binary to ELF
add_test(NAME "aie2ps_asm"
  COMMAND aiebu-asm -t aie2ps -c "${CMAKE_CURRENT_SOURCE_DIR}/ctrlcode.asm" -o ctrlcode_asm.elf -f disabledump)

add_test(NAME "aie2ps_disassemble"
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/run.cmake" -o disassembled_ctrlcode.asm -x ${ADUMP} -- -D ./ctrlcode_asm.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2ps_disassemble_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol disassembled_ctrlcode.asm "${CMAKE_CURRENT_SOURCE_DIR}/disassemble_golden.asm"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Assemble a ctrlcode txn binary to ELF
add_test(NAME "aie2ps_disassembled_elf"
  COMMAND aiebu-asm -t aie2ps -f disabledump -c disassembled_ctrlcode.asm -o disassembled_ctrlcode.elf)

# Compare the md5sum of ELF generated in aie2ps_asm test with that of the ELF generated from aie2ps_disassembled_elf
add_test(NAME "aie2ps_disassemble_elf_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" --compare-files "${CMAKE_CURRENT_BINARY_DIR}/disassembled_ctrlcode.elf" "${CMAKE_CURRENT_BINARY_DIR}/ctrlcode_asm.elf"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Set properties for the test
set_tests_properties("aie2ps_asm" PROPERTIES LABELS memcheck)
set_tests_properties("aie2ps_disassemble" PROPERTIES LABELS memcheck)
set_tests_properties("aie2ps_disassemble_compare" PROPERTIES LABELS memcheck)
set_tests_properties("aie2ps_disassembled_elf" PROPERTIES LABELS memcheck)
set_tests_properties("aie2ps_disassemble_elf_md5sum" PROPERTIES LABELS memcheck)

# Test interdependencies use full when running ctest with -j
set_tests_properties("aie2ps_disassemble_compare" PROPERTIES DEPENDS "aie2ps_disassemble")
set_tests_properties("aie2ps_disassembled_elf" PROPERTIES DEPENDS "aie2ps_disassemble")
set_tests_properties("aie2ps_disassemble_elf_md5sum" PROPERTIES DEPENDS "aie2ps_disassembled_elf;aie2ps_asm")
