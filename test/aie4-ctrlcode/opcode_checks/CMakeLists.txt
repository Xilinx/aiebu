# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.
#
# Negative (WILL_FAIL) and positive tests that verify the aiebu assembler
# enforces the opcode restrictions documented in the aiebu spec for:
#   load_pdi, load_cores, load_cores_cp, preempt, start_cond_job_preempt
#
# All tests run via "aiebu-asm -t aie4_config -j <config.json>" so they
# exercise the full aie4_config pipeline including aie4_preprocessor checks.

# ---------------------------------------------------------------------------
# Helper macro: register one aie4_config negative test (expected to fail).
# ---------------------------------------------------------------------------
macro(add_aie4_config_neg_test TEST_NAME CONFIG_FILE)
  add_test(NAME "${TEST_NAME}"
    COMMAND aiebu-asm -t aie4_config -j "${CMAKE_CURRENT_SOURCE_DIR}/${CONFIG_FILE}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_tests_properties("${TEST_NAME}" PROPERTIES WILL_FAIL TRUE)
endmacro()

# ---------------------------------------------------------------------------
# Negative tests
# ---------------------------------------------------------------------------

# load_pdi: two instructions in same column point to the same @label (address)
add_aie4_config_neg_test("aie4_check_dup_load_pdi_addr"
  "config_dup_load_pdi_addr.json")

# load_cores: two instructions in same column point to the same @label
add_aie4_config_neg_test("aie4_check_dup_load_cores_addr"
  "config_dup_load_cores_addr.json")

# load_pdi: count differs across columns (col0=2, col1=1)
add_aie4_config_neg_test("aie4_check_load_pdi_count_mismatch"
  "config_load_pdi_count_mismatch.json")

# load_cores: count differs across columns (col0=1, col1=0)
add_aie4_config_neg_test("aie4_check_load_cores_count_mismatch"
  "config_load_cores_count_mismatch.json")

# load_cores_cp: count differs across columns (col0=1, col1=0)
add_aie4_config_neg_test("aie4_check_load_cores_cp_count_mismatch"
  "config_load_cores_cp_count_mismatch.json")

# preempt: used without any load_pdi in the same control code elf
add_aie4_config_neg_test("aie4_check_preempt_no_load_pdi"
  "config_preempt_no_load_pdi.json")

# start_cond_job_preempt: appears before any preempt in the same column
add_aie4_config_neg_test("aie4_check_start_cond_before_preempt"
  "config_start_cond_before_preempt.json")

# start_cond_job_preempt: count differs across columns (col0=1, col2=0)
add_aie4_config_neg_test("aie4_check_start_cond_count_mismatch"
  "config_start_cond_count_mismatch.json")
