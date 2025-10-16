# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc.

set (ATRANSFORM $<TARGET_FILE:aiebu-transform>)
#get_target_property(AIEBU_DUMP_PATH aiebu-dump LOCATION)

# Run aiebu-dump on a ctrlcode binary
add_test(NAME "aie2_basic_transform"
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/run.cmake" -o disassemble-basic.txt -x ${ATRANSFORM} -- -j ${CMAKE_CURRENT_SOURCE_DIR}/t1.json -o basic-transform.elf ../basic/basic.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2_basic_transform_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol disassemble-basic.txt "${CMAKE_CURRENT_SOURCE_DIR}/disassemble-basic.txt"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2_fused_transform"
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/run.cmake" -o disassemble-fused.txt -x ${ATRANSFORM} -- -j ${CMAKE_CURRENT_SOURCE_DIR}/t1.json -o fused-transform.elf ../fused/fused.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2_fused_transform_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol disassemble-fused.txt "${CMAKE_CURRENT_SOURCE_DIR}/disassemble-fused.txt"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Test interdependencies use ful when running ctest with -j
set_tests_properties("aie2_basic_transform" PROPERTIES DEPENDS "aie2_basic_txn")
set_tests_properties("aie2_basic_transform_compare" PROPERTIES DEPENDS "aie2_basic_transform")
set_tests_properties("aie2_fused_transform" PROPERTIES DEPENDS "aie2_fused_txn")
set_tests_properties("aie2_fused_transform_compare" PROPERTIES DEPENDS "aie2_fused_transform")
