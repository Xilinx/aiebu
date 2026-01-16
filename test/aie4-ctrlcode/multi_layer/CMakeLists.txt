# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

set (ADUMP $<TARGET_FILE:aiebu-dump>)

# Assemble a ctrlcode txn binary to ELF
add_test(NAME "aie4_asm"
  COMMAND aiebu-asm -t aie4 -c "${CMAKE_CURRENT_SOURCE_DIR}/ctrlcode.asm" -L "${CMAKE_CURRENT_SOURCE_DIR}/" -o ctrlcode_asm.elf -f disabledump)

add_test(NAME "aie4_disassemble"
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/run.cmake" -o disassembled_ctrlcode.asm -x ${ADUMP} -- -m aie4 -d ./ctrlcode_asm.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Reassemble disassembled ASM
add_test(NAME "aie4_disassemble_elf"
  COMMAND aiebu-asm -t aie4 -c disassembled_ctrlcode.asm -o disassembled_ctrlcode.elf -f disabledump)

# Compare the md5sum of ELF generated in aie4_asm test with that of the ELF generated from aie4_disassemble_elf
add_test(NAME "aie4_disassemble_elf_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" --compare-files "${CMAKE_CURRENT_BINARY_DIR}/disassembled_ctrlcode.elf" "${CMAKE_CURRENT_BINARY_DIR}/ctrlcode_asm.elf"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Set properties for the test
set_tests_properties("aie4_asm" PROPERTIES LABELS memcheck)
set_tests_properties("aie4_disassemble" PROPERTIES LABELS memcheck)
set_tests_properties("aie4_disassemble_elf" PROPERTIES LABELS memcheck)
set_tests_properties("aie4_disassemble_elf_md5sum" PROPERTIES LABELS memcheck)

# Test interdependencies use full when running ctest with -j
set_tests_properties("aie4_disassemble" PROPERTIES DEPENDS "aie4_asm")
set_tests_properties("aie4_disassemble_elf" PROPERTIES DEPENDS "aie4_disassemble")
set_tests_properties("aie4_disassemble_elf_md5sum" PROPERTIES DEPENDS "aie4_disassemble_elf")
