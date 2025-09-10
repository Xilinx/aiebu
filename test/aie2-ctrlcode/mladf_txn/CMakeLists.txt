# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

# Running command to decode the base64 ctrlcode txn binary with no control packet
add_custom_command(OUTPUT ml_txn.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/ml_txn.b64" ml_txn.bin
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ml_txn.b64"
  COMMENT "Decoding base64 ctrlcode to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM)

# Add a custom target to ensure the binary is built before running tests
add_custom_target(aie2_mladf_txn_bins ALL
  DEPENDS ml_txn.bin)

# Assemble a ctrlcode txn binary to ELF
add_test(NAME "aie2_mladf_txn"
  COMMAND aiebu-asm -r -t aie2txn -c ml_txn.bin -o ml_txn.elf)

# Compare the md5sum of ELF generated in aie2_mladf_txn test with that of the golden file
add_test(NAME "aie2_mladf_txn_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/ml_txn.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Set properties for the test
set_tests_properties("aie2_mladf_txn" PROPERTIES LABELS memcheck)

# Test interdependencies use full when running ctest with -j
set_tests_properties("aie2_mladf_txn_md5sum" PROPERTIES DEPENDS "aie2_mladf_txn")
