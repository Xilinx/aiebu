# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

# 3-column hintmap regression (chunk c -> word[c/32] bit c%32; 5 words x 32 bits):
#   PREEMPT 0 — full home memtile per UC (48 chunks each)
#   PREEMPT 1 — middle 16 chunks of home memtile
#   PREEMPT 2 — partial own/cross coverage (24 chunks)
#   PREEMPT 3 — cross-UC 16+16+16 on 1-based col1 / memtile 1_1 (chunks 48-95)
#   PREEMPT 4 — cross-UC 16+16+16 on 1-based col2 / memtile 2_1 (chunks 96-143)
#   PREEMPT 5 — cross-UC 16+16+16 on 1-based col0 / memtile 0_1 (chunks 0-47)
#     uc0: first 16, uc2: middle 16, uc4: last 16

add_test(NAME "aie4_3col_preempt_hint_flavor_asm"
  COMMAND aiebu-asm -t aie4_config -j "${CMAKE_CURRENT_SOURCE_DIR}/config.json" -o 3col-preempt-hint-flavor.elf -f disabledump
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Compare the md5sum of the generated ELF against the golden file
add_test(NAME "aie4_3col_preempt_hint_flavor_asm_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/3col-preempt-hint-flavor.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie4_3col_preempt_hint_flavor_asm_md5sum" PROPERTIES DEPENDS "aie4_3col_preempt_hint_flavor_asm")
set_tests_properties("aie4_3col_preempt_hint_flavor_asm" PROPERTIES LABELS memcheck)
