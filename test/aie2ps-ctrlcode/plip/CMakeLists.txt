# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

# Assemble wts_pl_ctrl ctrlcode to ELF (C++ assembler)
add_test(NAME "aie2ps_plip_asm"
  COMMAND aiebu-asm -t aie2ps -c "${CMAKE_CURRENT_SOURCE_DIR}/ctrlcode.asm" -o ctrlcode_plip.elf -f disabledump)

# Verify round-trip ELF is byte-identical to the original
add_test(NAME "aie2ps_plip_elf_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/ctrlcode_plip.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie2ps_plip_asm" PROPERTIES LABELS memcheck)
set_tests_properties("aie2ps_plip_elf_md5sum" PROPERTIES DEPENDS "aie2ps_plip_asm")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  # Convert C++ ELF to legacy text
  add_test(NAME "aie2ps_plip_cpp_elftolegacy"
    COMMAND "${AIEBU_SOURCE_DIR}/src/python/aiebu/elf_to_legacy.py"
    -o ctrlcode_plip_cpp.out.txt -i ctrlcode_plip.elf
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  add_test(NAME "aie2ps_plip_cpp_compare"
    COMMAND ${CMAKE_COMMAND} -E compare_files
    ctrlcode_plip_cpp.out.txt "${CMAKE_CURRENT_SOURCE_DIR}/ctrlcode.txt"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  set_tests_properties("aie2ps_plip_cpp_elftolegacy" PROPERTIES DEPENDS "aie2ps_plip_asm")
  set_tests_properties("aie2ps_plip_cpp_compare" PROPERTIES DEPENDS "aie2ps_plip_cpp_elftolegacy")
endif()
