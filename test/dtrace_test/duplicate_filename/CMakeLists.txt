# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

set (ADUMP $<TARGET_FILE:aiebu-dump>)

# Test aiebu-dump --private trace-probe with duplicate filename
add_test(NAME "aie4_elf_dump_trace_probe_duplicate_filename"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/run.cmake" -o trace_probe_output.txt -x 
  ${ADUMP} -- --private trace-probe ${CMAKE_CURRENT_SOURCE_DIR}/filename.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "aie4_elf_dump_trace_probe_duplicate_filename_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol 
  "${CMAKE_CURRENT_BINARY_DIR}/trace_probe_output.txt"
  "${CMAKE_CURRENT_SOURCE_DIR}/gold_trace_probe.txt"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("aie4_elf_dump_trace_probe_duplicate_filename_compare" PROPERTIES 
  DEPENDS "aie4_elf_dump_trace_probe_duplicate_filename")

# Test duplicate filename script
add_test(NAME "dtrace_duplicate_filename"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/duplicate_filename"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_duplicate_filename.dat"
    "python"
    "elf" "${AIEBU_SOURCE_DIR}/test/dtrace_test/duplicate_filename/filename.elf" "DPU:dpu"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

# control buffer md5sum
add_test(NAME "dtrace_duplicate_filename_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_duplicate_filename.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/duplicate_filename/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_duplicate_filename_md5sum" PROPERTIES DEPENDS "dtrace_duplicate_filename")
