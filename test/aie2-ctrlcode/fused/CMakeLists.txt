# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc.

add_custom_command(OUTPUT fused.bin ctrlpkt.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/fused.b64" fused.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/ctrlpkt.b64" ctrlpkt.bin
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/fused.b64"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ctrlpkt.b64"
  COMMENT "Decoding base64 ctrlcode and ctrlpkt to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM)

add_custom_target(aie2fusedbins ALL
  DEPENDS fused.bin)

# Assemble a ctrlcode txn binary, a ctrlpkt and associated JSON to ELF
add_test(NAME "aie2_fused_txn"
  COMMAND aiebu-asm -r -t aie2txn -c fused.bin -p ctrlpkt.bin -j "${CMAKE_CURRENT_SOURCE_DIR}/fused.json" -o fused.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Assemble a ctrlcode ASM text, a ctrlpkt and associated JSON to ELF
add_test(NAME "aie2_fused_asm"
  COMMAND aiebu-asm -r -t aie2asm -c "${CMAKE_CURRENT_SOURCE_DIR}/fused.ctrltext.asm" -p ctrlpkt.bin -j "${CMAKE_CURRENT_SOURCE_DIR}/fused.json" -o ctrl1.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Compile a ctrlcode ASM text to binary and then decompile it into ASM text
add_test(NAME "aie2_fused_asm_redo"
  COMMAND aiebu-asm -r -t aie2asm -c "ctrl1.ctrltext.asm" -p ctrlpkt.bin -j "${CMAKE_CURRENT_SOURCE_DIR}/fused.json" -o ctrl2.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Compile the original ctrlcode ASM text to decompiled ASM text
add_test(NAME "aie2_fused_asm_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files ctrl1.ctrltext.asm ctrl2.ctrltext.asm
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Compare the md5sum of ELF generated in aie2_fused_txn test with that of the golden file
add_test(NAME "aie2_fused_txn_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/fused.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie2_fused_txn" PROPERTIES LABELS memcheck)
set_tests_properties("aie2_fused_asm" PROPERTIES LABELS memcheck)
