# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

# Running command to decode the base64 ctrlcode txn binary with control packet having PM Loading but no pm control packet
add_custom_command(OUTPUT dma_txn.bin dma_ctrl_pkt.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/ml_txn.b64" dma_txn.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/ctrl_pkt.b64" dma_ctrl_pkt.bin
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ml_txn.b64"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ctrl_pkt.b64"
  COMMENT "Decoding base64 ctrlcode and ctrlpkt to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM)

# Add a custom target to ensure the binary is built before running tests
add_custom_target(aie2_dma_txn_ctrlpkt_pmload_bins ALL
  DEPENDS dma_txn.bin dma_ctrl_pkt.bin)

# Assemble a ctrlcode txn binary, a ctrlpkt and associated JSON to ELF
add_test(NAME "aie2_dma_txn_ctrlpkt_pmload"
COMMAND aiebu-asm -r -t aie2txn -c dma_txn.bin -p dma_ctrl_pkt.bin -j "${CMAKE_CURRENT_SOURCE_DIR}/external_buffer_id.json" -o dma_txn_ctrlpkt_pmload.elf)

# Compare the md5sum of ELF generated in aie2_dma_txn_ctrlpkt_pmload test with that of the golden file
add_test(NAME "aie2_dma_txn_ctrlpkt_pmload_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/dma_txn_ctrlpkt_pmload.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Set properties for the test
set_tests_properties("aie2_dma_txn_ctrlpkt_pmload" PROPERTIES LABELS memcheck)

# Test interdependencies use full when running ctest with -j
set_tests_properties("aie2_dma_txn_ctrlpkt_pmload_md5sum" PROPERTIES DEPENDS "aie2_dma_txn_ctrlpkt_pmload")
