# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

# 3-column merged control test with cross-word hintmap holes.
# col0 hintmap_00: word0=0x0000000f (bits 0-3) + word3=0x0f000000 (bits 120-123)
#                  set=8, span=124, holes=116
# col2: no hintmap -> fixed region (whole column)
# col4 hintmap_40: word0=0x0000f000 (bits 12-15)
#                  set=4, span=4, holes=0
# col0 has a massive cross-word hole; col4's span [12,15] does not overlap col0's
# span [0,123]; but col0's huge span overlaps the fixed col2 region, triggering
# span-only redistribution for the hintmap controllers while col2 is unchanged.

add_test(NAME "aie4_3col_preempt_hintmap_cross_word_hole_asm"
  COMMAND aiebu-asm -t aie4_config -j "${CMAKE_CURRENT_SOURCE_DIR}/config.json" -o 3col-preempt-hintmap-cross-word-hole.elf -f disabledump
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie4_3col_preempt_hintmap_cross_word_hole_asm_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/3col-preempt-hintmap-cross-word-hole.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie4_3col_preempt_hintmap_cross_word_hole_asm_md5sum" PROPERTIES DEPENDS "aie4_3col_preempt_hintmap_cross_word_hole_asm")
set_tests_properties("aie4_3col_preempt_hintmap_cross_word_hole_asm" PROPERTIES LABELS memcheck)

# Verify read_bd.py output against the golden table
add_test(NAME "aie4_3col_preempt_hintmap_cross_word_hole_read_bd"
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/run.cmake" -o read_bd_output.txt
          -x ${Python3_EXECUTABLE} --
          "${AIEBU_SOURCE_DIR}/src/python/aiebu/read_bd.py"
          3col-preempt-hintmap-cross-word-hole.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie4_3col_preempt_hintmap_cross_word_hole_read_bd_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
          "${CMAKE_CURRENT_BINARY_DIR}/read_bd_output.txt"
          "${CMAKE_CURRENT_SOURCE_DIR}/gold_read_bd.txt"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie4_3col_preempt_hintmap_cross_word_hole_read_bd" PROPERTIES DEPENDS "aie4_3col_preempt_hintmap_cross_word_hole_asm")
set_tests_properties("aie4_3col_preempt_hintmap_cross_word_hole_read_bd_compare" PROPERTIES DEPENDS "aie4_3col_preempt_hintmap_cross_word_hole_read_bd")
