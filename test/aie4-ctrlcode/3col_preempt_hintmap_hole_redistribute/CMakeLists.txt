# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

# 3-column span-overlap-due-to-holes redistribution test.
# col0 hintmap: bits 0,3,6,9,12,15  (word0 = 0x00009249)
# col2 hintmap: bits 1,4,7,10,13,16 (word0 = 0x00012492)
# col4 hintmap: bits 2,5,8,11,14,17 (word0 = 0x00024924)
# All bits are disjoint; all three spans overlap due to interleaving (holes).
# Expected redistribution (greedy, midpoint tie-break):
#   col0 -> chunks  0-3  [0x000000, 0x040000)
#   col2 -> chunks  4-8  [0x040000, 0x090000)
#   col4 -> chunks 9-17  [0x090000, 0x120000)

add_test(NAME "aie4_3col_preempt_hintmap_hole_redistribute_asm"
  COMMAND aiebu-asm -t aie4_config -j "${CMAKE_CURRENT_SOURCE_DIR}/config.json" -o 3col-preempt-hintmap-hole-redistribute.elf -f disabledump
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie4_3col_preempt_hintmap_hole_redistribute_asm_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/3col-preempt-hintmap-hole-redistribute.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie4_3col_preempt_hintmap_hole_redistribute_asm_md5sum" PROPERTIES DEPENDS "aie4_3col_preempt_hintmap_hole_redistribute_asm")
set_tests_properties("aie4_3col_preempt_hintmap_hole_redistribute_asm" PROPERTIES LABELS memcheck)

# ---------------------------------------------------------------------------
# Real-world control code test (merged_control_3col.asm).
# col0 hintmap_00: 0x00000f0f → bits 0-3,8-11 (8 set, span [0,11])
# col2: no hintmap → fixed region (whole column)
# col4 hintmap_40: 0x0000f000 + 0xffffffff in word3 → bits 12-15 + 96-127
#                  (36 set, span [12,127])
# col0 and col4 both have hintmaps; col4's span [12,127] overlaps col0's
# fixed-col2 region, triggering span-only redistribution for the two hintmap
# controllers while col2's fixed region is left unchanged.
# Expected redistribution:
#   col0 -> [0x000000, 0x100000)  (set=12, span=16, holes=4)
#   col4 -> [0x600000, 0x800000)  (set=32, span=32, holes=0)
# ---------------------------------------------------------------------------
add_test(NAME "aie4_3col_merged_control_asm"
  COMMAND aiebu-asm -t aie4_config -j "${CMAKE_CURRENT_SOURCE_DIR}/config_merged.json" -o merged-control-3col.elf -f disabledump
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie4_3col_merged_control_asm_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/merged-control-3col.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold_merged.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie4_3col_merged_control_asm_md5sum" PROPERTIES DEPENDS "aie4_3col_merged_control_asm")
set_tests_properties("aie4_3col_merged_control_asm" PROPERTIES LABELS memcheck)
