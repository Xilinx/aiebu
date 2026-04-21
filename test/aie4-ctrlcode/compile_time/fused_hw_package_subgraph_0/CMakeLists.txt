# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

#if(CMAKE_BUILD_TYPE STREQUAL "Release")
# Assemble control code ASM for very large asm of total size 286 MB arranged into 10K files
add_test(NAME "aie4_compile_time"
  COMMAND aiebu-asm -t aie4_config -j "${CMAKE_CURRENT_SOURCE_DIR}/config.json" -o control.elf -f disabledump
  CONFIGURATIONS Release
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Compare the md5sum of ELF generated in aie4_compile_time test with that of the golden file
add_test(NAME "aie4_compile_time_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/control.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  CONFIGURATIONS Release
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie4_compile_time_md5sum" PROPERTIES DEPENDS "aie4_compile_time")
#endif()
