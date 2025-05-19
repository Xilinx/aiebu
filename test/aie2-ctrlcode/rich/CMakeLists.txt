# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc.

add_custom_command(OUTPUT ml_txn.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/ml_txn.b64" ml_txn.bin
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ml_txn.b64"
  COMMENT "Decoding base64 ctrlcode and ctrlpkt to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM)

add_custom_target(aie2richbins ALL
  DEPENDS ml_txn.bin)

# Assemble a ctrlcode txn binary, a ctrlpkt and associated JSON to ELF
add_test(NAME "aie2_rich_txn"
  COMMAND aiebu-asm -r -t aie2txn -c ml_txn.bin -o rich.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Assemble a ctrlcode ASM text, a ctrlpkt and associated JSON to ELF
add_test(NAME "aie2_rich_asm"
  COMMAND aiebu-asm -r -t aie2asm -c "${CMAKE_CURRENT_SOURCE_DIR}/rich.ctrltext.asm" -o ctrl1.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Compile a ctrlcode ASM text to binary and then decompile it into ASM text
add_test(NAME "aie2_rich_asm_redo"
  COMMAND aiebu-asm -r -t aie2asm -c "ctrl1.ctrltext.asm" -o ctrl2.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Compile the original ctrlcode ASM text to decompiled ASM text
add_test(NAME "aie2_rich_asm_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files ctrl1.ctrltext.asm ctrl2.ctrltext.asm
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Compare the md5sum of ELF generated in aie2_rich_txn test with that of the golden file
add_test(NAME "aie2_rich_txn_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/rich.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Test interdependencies use ful when running ctest with -j
set_tests_properties("aie2_rich_asm_redo" PROPERTIES DEPENDS "aie2_rich_asm")
set_tests_properties("aie2_rich_asm_compare" PROPERTIES DEPENDS "aie2_rich_asm;aie2_rich_asm_redo")
set_tests_properties("aie2_rich_txn_md5sum" PROPERTIES DEPENDS "aie2_rich_txn")

set_tests_properties("aie2_rich_txn" PROPERTIES LABELS memcheck)
set_tests_properties("aie2_rich_asm" PROPERTIES LABELS memcheck)
