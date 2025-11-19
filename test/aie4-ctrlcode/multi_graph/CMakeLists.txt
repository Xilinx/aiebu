# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc.

# Assemble control code ASM for multigraph model into ELF
add_test(NAME "aie4_multi_graph_asm"
  COMMAND aiebu-asm -t aie4_config -j "${CMAKE_CURRENT_SOURCE_DIR}/config.json" -o multi-graph.elf -f disabledump
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Compare the md5sum of ELF generated in aie4_multi_graph_asm test with that of the golden file
add_test(NAME "aie4_multi_graph_asm_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/multi-graph.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie4_multi_graph_asm_md5sum" PROPERTIES DEPENDS "aie4_multi_graph_asm")
set_tests_properties("aie4_multi_graph_asm" PROPERTIES LABELS memcheck)
