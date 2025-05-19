# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc.

set (ADUMP $<TARGET_FILE:aiebu-dump>)
#get_target_property(AIEBU_DUMP_PATH aiebu-dump LOCATION)

# Run aiebu-dump on a ctrlcode binary
add_test(NAME "aie2_pmctrlpkt_txn_dump"
  COMMAND aiebu-dump ../pmctrlpkt/mc_code_txn.bin
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2_pmctrlpkt_elf_dump"
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/run.cmake" -o d.txt -x ${ADUMP} -- -d ../pmctrlpkt/mc_code_txn.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2_pmctrlpkt_elf_dump_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files d.txt "${CMAKE_CURRENT_SOURCE_DIR}/d.txt"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2_pmctrlpkt_elf_dump_all"
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/run.cmake" -o D.txt -x ${ADUMP} -- -D ../pmctrlpkt/mc_code_txn.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2_pmctrlpkt_elf_dump_all_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files D.txt "${CMAKE_CURRENT_SOURCE_DIR}/D.txt"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2_pmctrlpkt_ctrlpkt_dump0"
  COMMAND aiebu-dump ../pmctrlpkt/ctrl_pkt.bin
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2_pmctrlpkt_ctrlpkt_dump1"
  COMMAND aiebu-dump ../pmctrlpkt/pm_ctrlpkt_0.bin
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie2_pmctrlpkt_elf_dump_all" PROPERTIES LABELS memcheck)
