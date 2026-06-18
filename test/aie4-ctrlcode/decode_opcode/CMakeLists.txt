# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

set(DECODE_OPCODE_TEST "aie4_decode_opcode")
add_executable(${DECODE_OPCODE_TEST} decode_opcode.cpp)
target_link_libraries(${DECODE_OPCODE_TEST} PRIVATE aiebu_static)
target_include_directories(${DECODE_OPCODE_TEST} PRIVATE
  ${AIEBU_SOURCE_DIR}/src/cpp/include
  ${AIEBU_SOURCE_DIR}/src/cpp/ELFIO
)
if (NOT WIN32)
  target_link_libraries(${DECODE_OPCODE_TEST} PRIVATE pthread)
endif()

# ---------------------------------------------------------------------------
# Test parameters for uc_idx=0, page_idx=2, offset=148
# Instruction: mask_poll_32  0x1400fdfc, 0x1, 0x1
#
# Dump path:    opcode_name + args_str + source_file + line are verified.
#               The dump "operation" field stores the full lowercased text
#               ("name args"); decode_from_dump splits it on the first space.
#
# ISA walk path: opcode_name + args_str are verified.
#               source_file and line are not available → skipped (hard-coded
#               to "none"/0 in the check_opcode call for this path).
# ---------------------------------------------------------------------------
set(KERNEL_NAME          "DPU:subgraph_0")
set(UC_IDX               "0")
set(PAGE_IDX             "2")
set(OFFSET               "16")


# ISA walk path expected values
# get_code_name() returns lowercase.
set(EXPECTED_OPCODE_ISA "start_job")
set(EXPECTED_ARGS_ISA   "0")

cmake_path(SET AIE4_ELF_IN NORMALIZE "${CMAKE_CURRENT_BINARY_DIR}/../multi_graph/multi-graph.elf")

# Assemble both ELFs and verify the decoded opcode, args, source file and line
add_test(NAME "aie4_decode_opcode"
  COMMAND ${DECODE_OPCODE_TEST}
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${AIE4_ELF_IN}"
    "${KERNEL_NAME}" "${UC_IDX}" "${PAGE_IDX}" "${OFFSET}"
    "${EXPECTED_OPCODE_ISA}"  "${EXPECTED_ARGS_ISA}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie4_decode_opcode" PROPERTIES DEPENDS "aie4_multi_graph_asm")
