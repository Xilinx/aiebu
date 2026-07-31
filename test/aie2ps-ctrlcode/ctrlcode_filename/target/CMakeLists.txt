# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.
#
# aie2ps target test: eof entries carry "default" (no ctrl_code_file in target ELF).

add_test(NAME "aie2ps_ctrlcode_filename_target_assemble"
  COMMAND aiebu-asm -t aie2ps
    -c "${CMAKE_CURRENT_SOURCE_DIR}/../ml_asm/merged_control.asm"
    -o "${CMAKE_CURRENT_BINARY_DIR}/ctrlcode_filename_target.elf"
    -f fulldump
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2ps_ctrlcode_filename_target_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files
    "${CMAKE_CURRENT_BINARY_DIR}/debug_map.json"
    "${CMAKE_CURRENT_SOURCE_DIR}/gold_debug_map.json"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie2ps_ctrlcode_filename_target_compare"
  PROPERTIES DEPENDS "aie2ps_ctrlcode_filename_target_assemble")
