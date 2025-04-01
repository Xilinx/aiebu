# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc.

# Run aiebu-dump on a ctrlcode binary
add_test(NAME "aie2_pmctrlpkt_txn_dump"
  COMMAND aiebu-dump ../pmctrlpkt/mc_code_txn.bin
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2_pmctrlpkt_elf_dump"
  COMMAND aiebu-dump ../pmctrlpkt/mc_code_txn.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie2_pmctrlpkt_txn_dump" PROPERTIES LABELS memcheck)
