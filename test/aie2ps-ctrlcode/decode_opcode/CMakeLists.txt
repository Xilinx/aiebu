# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

set(DECODE_OPCODE_TEST "aie2ps_decode_opcode")
add_executable(${DECODE_OPCODE_TEST} decode_opcode.cpp)
target_link_libraries(${DECODE_OPCODE_TEST} PRIVATE aiebu_static)
target_include_directories(${DECODE_OPCODE_TEST} PRIVATE
  ${AIEBU_SOURCE_DIR}/src/cpp/include
  ${AIEBU_ELFIO_SRC_DIR}
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
set(KERNEL_NAME          "CTRL:inst0")
set(UC_IDX               "0")
set(PAGE_IDX             "2")
set(OFFSET               "148")

# Dump path expected values
# "operation" in dump JSON is lowercased full text "name args"; decode_from_dump splits on first space.
set(EXPECTED_OPCODE_DUMP "mask_poll_32")
set(EXPECTED_ARGS_DUMP   "0x1400fdfc, 0x1, 0x1")
# dump stores the vfile key verbatim, which includes the "../" prefix
set(EXPECTED_SOURCE_FILE "../ml_asm/aie_runtime_control0.asm")
set(EXPECTED_LINE        "22")

# ISA walk path expected values
# get_code_name() returns lowercase.
# handle_generic_const_arg formats "address"-named args as uppercase hex,
# and small "mask"/"value" fields (<= 0xFFFF) as decimal.
set(EXPECTED_OPCODE_ISA  "mask_poll_32")
set(EXPECTED_ARGS_ISA    "0x1400fdfc, 1, 1")

# Assemble both ELFs and verify the decoded opcode, args, source file and line
add_test(NAME "aie2ps_decode_opcode_assemble"
  COMMAND ${DECODE_OPCODE_TEST}
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}/decode_opcode_with_dump.elf"
    "${CMAKE_CURRENT_BINARY_DIR}/decode_opcode_no_dump.elf"
    "${KERNEL_NAME}" "${UC_IDX}" "${PAGE_IDX}" "${OFFSET}"
    "${EXPECTED_OPCODE_DUMP}" "${EXPECTED_ARGS_DUMP}"
    "${EXPECTED_SOURCE_FILE}" "${EXPECTED_LINE}"
    "${EXPECTED_OPCODE_ISA}"  "${EXPECTED_ARGS_ISA}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie2ps_decode_opcode_assemble" PROPERTIES LABELS memcheck)
