# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

# Assemble a ctrlcode txn binary, a ctrlpkt and associated JSON to ELF
# Tests that addend > 32bit is handled correctly (warn + bake into BD) for shim_dma_48/control_packet_48
add_test(NAME "aie2_orca_2bit_greater_than_4gb"
  COMMAND aiebu-asm -t aie2txn
    -c "${CMAKE_CURRENT_SOURCE_DIR}/cluster_0_0instr_bo_fname.bin"
    -p "${CMAKE_CURRENT_SOURCE_DIR}/cluster_0_0super_instr_bo_fname.bin"
    -j "${CMAKE_CURRENT_SOURCE_DIR}/cluster_0_0ctrl_pkt_info.json"
    -o orca_2bit_greater_than_4gb.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie2_orca_2bit_greater_than_4gb" PROPERTIES LABELS memcheck)

# Compare the md5sum of the generated ELF with the golden file
add_test(NAME "aie2_orca_2bit_greater_than_4gb_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/orca_2bit_greater_than_4gb.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie2_orca_2bit_greater_than_4gb_md5sum" PROPERTIES DEPENDS "aie2_orca_2bit_greater_than_4gb")
