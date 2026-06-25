# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.
#
# Negative test: assembling a config whose .include chain causes the same file
# (dup.asm) to be included twice must fail with "duplicate asm file name".
#
# Include chain:
#   main.asm  --includes-->  test.asm  --includes-->  dup.asm  (1st inclusion)
#   main.asm  --includes-->  dup.asm                           (2nd inclusion → error)
#
# PASS_REGULAR_EXPRESSION makes CTest pass when the expected error string
# appears in the tool output (regardless of exit code).
add_test(NAME "aie4_config_duplicate_include_error"
  COMMAND aiebu-asm -t aie4_config
          -j "${CMAKE_CURRENT_SOURCE_DIR}/config.json"
          -o duplicate_include.elf
          -f disabledump
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie4_config_duplicate_include_error" PROPERTIES
  PASS_REGULAR_EXPRESSION "duplicate asm file name")
