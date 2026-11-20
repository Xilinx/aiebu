# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

# DWARF ELF multi-column annotation dtrace test — validates that the {column, address} cache key
# in write_trace_probes_from_dwarf() and dwarf_rows_to_json() correctly distinguishes
# rows from different AIE columns (uc0 on column 0, uc2 on column 2).
# ELF is generated from test/dtrace_test/multi_col_annot/Work/aiebu_config.json.
# jprobes target line and annotation probes on both columns:
#   uc0:line7, uc0:annotation0, uc0:annotation1 (start_job 0 / uc_dma_write_des_sync / end_job)
#   uc2:line7, uc2:annotation0, uc2:annotation1 (start_job 1 / uc_dma_write_des_sync / end_job)

# generate elf file
add_test(NAME "dtrace_multi_col_annot_generate_elf"
  COMMAND aiebu-asm
    -t aie4_config
    -j "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col_annot/Work/aiebu_config.json"
    -o "${CMAKE_CURRENT_BINARY_DIR}/multi_col_annot.elf"
    -f fulldump
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_multi_col_annot"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col_annot"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi_col_annot.dat"
    "python"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/multi_col_annot.elf" "DPU:dpu"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multi_col_annot" PROPERTIES
  DEPENDS "dtrace_multi_col_annot_generate_elf")

# control buffer md5sum
add_test(NAME "dtrace_multi_col_annot_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi_col_annot.dat"
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col_annot/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multi_col_annot_md5sum" PROPERTIES
  DEPENDS "dtrace_multi_col_annot")

# result python file md5sum
add_test(NAME "dtrace_multi_col_annot_result_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi_col_annot.py"
    "${CMAKE_CURRENT_SOURCE_DIR}/gold_result.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multi_col_annot_result_compare" PROPERTIES
  DEPENDS "dtrace_multi_col_annot")
  
# verify elf file
find_program(LLVM_DWARFDUMP llvm-dwarfdump)
if (LLVM_DWARFDUMP)
  add_test(NAME "dwarf_verify_multi_col_annot"
    COMMAND ${LLVM_DWARFDUMP} --verify "${CMAKE_CURRENT_BINARY_DIR}/multi_col_annot.elf"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_tests_properties("dwarf_verify_multi_col_annot" PROPERTIES
    DEPENDS "dtrace_multi_col_annot_generate_elf")
endif()
