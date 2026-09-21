# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.
#
# Negative (WILL_FAIL) tests that verify the aiebu assembler rejects TRACE
# in user control code.  TRACE is reserved for internal CERT use only.

macro(add_aie2ps_neg_test TEST_NAME ASM_FILE)
  add_test(NAME "${TEST_NAME}"
    COMMAND aiebu-asm -t aie2ps -o aie_control.elf -c "${CMAKE_CURRENT_SOURCE_DIR}/${ASM_FILE}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_tests_properties("${TEST_NAME}" PROPERTIES WILL_FAIL TRUE)
endmacro()

# TRACE must not appear in user control code — it is for CERT internal use.
add_aie2ps_neg_test("aie2ps_check_trace_in_ctrlcode" "trace_in_ctrlcode.asm")
