# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc.

# Assemble a ctrlcode to ELF
add_test(NAME "aie2ps_eff_net_coal_asm"
  COMMAND aiebu-asm -t aie2ps_config -j "${CMAKE_CURRENT_SOURCE_DIR}/config.json" -o eff_net_coal.elf -f disabledump)
  set_tests_properties("aie2ps_eff_net_coal_asm" PROPERTIES LABELS "benchmark")

if ((${CMAKE_SYSTEM_NAME} STREQUAL "Linux"))
  add_test(NAME "aie2ps_eff_net_coal_readelf"
    COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/../run-readelf.sh" "eff_net_coal.elf" "eff_net_coal.txt"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  add_test(NAME "aie2ps_eff_net_coal_compareelf"
    COMMAND ${CMAKE_COMMAND} -E compare_files "eff_net_coal.txt" "${CMAKE_CURRENT_SOURCE_DIR}/eff_net_coal.gold"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  set_tests_properties("aie2ps_eff_net_coal_readelf" PROPERTIES DEPENDS "aie2ps_eff_net_coal_asm")
  set_tests_properties("aie2ps_eff_net_coal_compareelf" PROPERTIES DEPENDS "aie2ps_eff_net_coal_readelf")

  add_test(NAME "aie2ps_eff_net_coal_md5sum"
    COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/eff_net_coal.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  set_tests_properties("aie2ps_eff_net_coal_md5sum" PROPERTIES DEPENDS "aie2ps_eff_net_coal_asm")
endif()
