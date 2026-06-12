# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

set (ADUMP $<TARGET_FILE:aiebu-dump>)

# Test aiebu-dump --private trace-probe with duplicate filename
add_test(NAME "aie4_elf_dump_trace_probe_duplicate_filename"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/run.cmake" -o trace_probe_output.txt -x 
  ${ADUMP} -- --private trace-probe ${CMAKE_CURRENT_SOURCE_DIR}/filename.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie4_elf_dump_trace_probe_duplicate_filename_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol 
  "${CMAKE_CURRENT_BINARY_DIR}/trace_probe_output.txt"
  "${CMAKE_CURRENT_SOURCE_DIR}/gold_trace_probe.txt"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie4_elf_dump_trace_probe_duplicate_filename_compare" PROPERTIES DEPENDS "aie4_elf_dump_trace_probe_duplicate_filename")
