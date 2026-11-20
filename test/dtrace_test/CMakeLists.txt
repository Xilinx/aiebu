# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

project(dtrace_test)

set(DTRACE_TESTNAME "dtrace_test")

add_executable(${DTRACE_TESTNAME} dtrace_test.cpp)
add_dependencies(${DTRACE_TESTNAME} cert_dtrace_static aiebu_static)
target_link_libraries(${DTRACE_TESTNAME} PRIVATE cert_dtrace_static aiebu_static)
target_include_directories(${DTRACE_TESTNAME} PRIVATE
  ${AIEBU_SOURCE_DIR}/src/cpp
  ${AIEBU_ELFIO_SRC_DIR})

# functional test
# debug format dump
add_subdirectory(elf_handle_dump)
add_subdirectory(single_col_dump)
add_subdirectory(multi_col_dump)
# debug format dwarf
add_subdirectory(elf_handle)
add_subdirectory(single_col)
add_subdirectory(multi_col)
add_subdirectory(multi_col_annot)
add_subdirectory(multi_col_mem_action)
add_subdirectory(duplicate_filename)
add_subdirectory(empty_elf_debug)
add_subdirectory(profile)

# benchmark test
add_subdirectory(wayve_vit_encoder)
add_subdirectory(anduril_yolox)
