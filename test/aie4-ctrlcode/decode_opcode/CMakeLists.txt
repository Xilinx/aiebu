# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

set(DECODE_OPCODE_TEST "aie4_decode_opcode")
add_executable(${DECODE_OPCODE_TEST} decode_opcode.cpp)
target_link_libraries(${DECODE_OPCODE_TEST} PRIVATE aiebu_static)

# JSON input is parsed with Boost.PropertyTree (boost/property_tree/json_parser.hpp);
# Boost headers come from the top-level find_package(Boost) / include_directories.

target_include_directories(${DECODE_OPCODE_TEST} PRIVATE
  ${AIEBU_SOURCE_DIR}/src/cpp/include
  ${AIEBU_ELFIO_SRC_DIR}
)
if (NOT WIN32)
  target_link_libraries(${DECODE_OPCODE_TEST} PRIVATE pthread)
endif()

# Test vectors: see decode_opcode_input.json — object with a "cases" array;
# each case has kernel_name, uc_idx, page_idx, offset,
# expected_opcode_no_dump, expected_args_no_dump, optional label.

cmake_path(SET AIE4_ELF_IN NORMALIZE "${CMAKE_CURRENT_BINARY_DIR}/../multi_graph/multi-graph.elf")

add_test(NAME "aie4_decode_opcode"
  COMMAND ${DECODE_OPCODE_TEST}
    "${AIE4_ELF_IN}"
    "${CMAKE_CURRENT_SOURCE_DIR}/decode_opcode_input.json"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie4_decode_opcode" PROPERTIES DEPENDS "aie4_multi_graph_asm")
