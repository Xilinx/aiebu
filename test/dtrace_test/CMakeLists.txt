# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

project(dtrace_test)

set(DTRACE_TESTNAME "dtrace_test")

add_executable(${DTRACE_TESTNAME} dtrace_test.cpp)
add_dependencies(${DTRACE_TESTNAME} cert_dtrace_static)
target_link_libraries(${DTRACE_TESTNAME} PRIVATE cert_dtrace_static)
target_include_directories(${DTRACE_TESTNAME} PRIVATE
  ${AIEBU_SOURCE_DIR}/src/cpp
  ${AIEBU_ELFIO_SRC_DIR})

# Test single uc script
add_test(NAME "dtrace_single_column"
  COMMAND ${DTRACE_TESTNAME} "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_single.dat"
  "python"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_single_column_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_single.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_single_column_md5sum" PROPERTIES DEPENDS "dtrace_single_column")

add_test(NAME "dtrace_single_column_result_md5sum"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol 
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_single.py"
  "${CMAKE_CURRENT_SOURCE_DIR}/single_col/gold_result.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_single_column_result_md5sum" PROPERTIES DEPENDS "dtrace_single_column")

# Test multi uc script and multi page control block
add_test(NAME "dtrace_multi_column"
  COMMAND ${DTRACE_TESTNAME} "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi.dat"
  "json"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_multi_column_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multi_column_md5sum" PROPERTIES DEPENDS "dtrace_multi_column")

add_test(NAME "dtrace_multi_column_result_md5sum"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol 
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi.json"
  "${CMAKE_CURRENT_SOURCE_DIR}/multi_col/gold_result.json"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multi_column_result_md5sum" PROPERTIES DEPENDS "dtrace_multi_column")

# Test profile script
add_test(NAME "dtrace_profile"
  COMMAND ${DTRACE_TESTNAME} "${AIEBU_SOURCE_DIR}/test/dtrace_test/profile"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_profile.dat"
  "python"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_profile_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_profile.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/profile/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_profile_md5sum" PROPERTIES DEPENDS "dtrace_profile")

# Test duplicate filename script
add_test(NAME "dtrace_duplicate_filename"
  COMMAND ${DTRACE_TESTNAME} "${AIEBU_SOURCE_DIR}/test/dtrace_test/duplicate_filename"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_duplicate_filename.dat"
  "python"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_duplicate_filename_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_duplicate_filename.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/duplicate_filename/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_duplicate_filename_md5sum" PROPERTIES DEPENDS "dtrace_duplicate_filename")

# Test empty elf dump section
add_test(NAME "dtrace_empty_elf_dump"
  COMMAND ${DTRACE_TESTNAME} "${AIEBU_SOURCE_DIR}/test/dtrace_test/empty_elf_dump"
  "${CMAKE_CURRENT_BINARY_DIR}/control_empty_elf_dump.dat"
  "python"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_empty_elf_dump_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_empty_elf_dump.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/empty_elf_dump/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_empty_elf_dump_md5sum" PROPERTIES DEPENDS "dtrace_empty_elf_dump")

# Test full elf with kernel
add_test(NAME "dtrace_elf_with_kernel"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/elf_handle"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_elf_with_kernel.dat"
    "python"
    "elf" "${AIEBU_SOURCE_DIR}/test/dtrace_test/elf_handle/test.elf" "CTRL:inst0"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_elf_with_kernel_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_elf_with_kernel.dat"
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/elf_handle/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_elf_with_kernel_md5sum" PROPERTIES DEPENDS "dtrace_elf_with_kernel")

# Test partial elf without kernel
add_test(NAME "dtrace_elf_no_kernel"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/elf_handle"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_elf_no_kernel.dat"
    "python"
    "elf" "${AIEBU_SOURCE_DIR}/test/dtrace_test/elf_handle/test_partial.elf" "none"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_elf_no_kernel_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_elf_no_kernel.dat"
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/elf_handle/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_elf_no_kernel_md5sum" PROPERTIES DEPENDS "dtrace_elf_no_kernel")

# DWARF ELF dtrace test — multi-graph-dwarf.elf is generated at test time from
# test/aie4-ctrlcode/multi_graph/config.json using aiebu-asm (aie4_config, fulldump).
# It is a multi-graph ELF (DPU kernel, 29 subgraph instances) with DWARF v5
# .debug_* sections. Filtered to kernel instance "DPU:subgraph_0",
# jprobe target: control.asm:uc0:line5.
# The result script is verified against gold_result_dwarf.py; operation="" is
# expected since DWARF .debug_line does not store instruction mnemonics.

# Generate multi-graph-dwarf.elf from source (avoids storing a large binary in the repo)
add_test(NAME "generate_dwarf_elf"
  COMMAND aiebu-asm
    -t aie4_config
    -j "${AIEBU_SOURCE_DIR}/test/aie4-ctrlcode/multi_graph/config.json"
    -o "${CMAKE_CURRENT_BINARY_DIR}/multi-graph-dwarf.elf"
    -f fulldump
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

# Run dtrace against the generated DWARF ELF (instance DPU:subgraph_0)
add_test(NAME "dtrace_dwarf_elf"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/dwarf_elf"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_dwarf_elf.dat"
    "python"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/multi-graph-dwarf.elf" "DPU:subgraph_0"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

set_tests_properties("dtrace_dwarf_elf" PROPERTIES DEPENDS "generate_dwarf_elf")

# Verify the DWARF result script matches the expected gold.
# operation="" is expected: DWARF .debug_line does not store instruction mnemonics.
add_test(NAME "dtrace_dwarf_elf_result_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_dwarf_elf.py"
    "${CMAKE_CURRENT_SOURCE_DIR}/dwarf_elf/gold_result_dwarf.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

set_tests_properties("dtrace_dwarf_elf_result_compare" PROPERTIES
  DEPENDS "dtrace_dwarf_elf")

add_subdirectory(duplicate_filename)
