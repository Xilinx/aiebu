# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

# Test elf handle with debug format dwarf. It is a multi-graph ELF (DPU kernel, 2 subgraph instances).
# Filtered to kernel instance "DPU:subgraph_0".

# generate elf file
add_test(NAME "dtrace_elf_handle_generate_elf"  
  COMMAND aiebu-asm
    -t aie4_config
    -j "${AIEBU_SOURCE_DIR}/test/aie4-ctrlcode/multi_graph/aiebu_config.json"
    -o "${CMAKE_CURRENT_BINARY_DIR}/multi-graph.elf"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

# Test full elf with kernel:instance (explicit instance name)
add_test(NAME "dtrace_elf_handle"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/elf_handle"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_elf_handle.dat"
    "python"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/multi-graph.elf" "DPU:subgraph_0"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_elf_handle" PROPERTIES DEPENDS "dtrace_elf_handle_generate_elf")

# result python file md5sum
add_test(NAME "dtrace_elf_handle_result_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_elf_handle.py"
    "${CMAKE_CURRENT_SOURCE_DIR}/gold_result.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_elf_handle_result_compare" PROPERTIES
  DEPENDS "dtrace_elf_handle")

# verify elf file
# Validate DWARF sections of the generated ELF with llvm-dwarfdump --verify.
# This provides an independent check that .debug_abbrev, .debug_info, .debug_line, and
# .debug_str are spec-compliant — something the internal dwarf_reader cannot verify since
# it uses a hardcoded schema and does not parse .debug_abbrev at all.
find_program(LLVM_DWARFDUMP llvm-dwarfdump)
if (LLVM_DWARFDUMP)
  add_test(NAME "dwarf_verify_single_instance"
    COMMAND ${LLVM_DWARFDUMP} --verify "${CMAKE_CURRENT_BINARY_DIR}/multi-graph.elf"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_tests_properties("dwarf_verify_single_instance" PROPERTIES
    DEPENDS "dtrace_elf_handle_generate_elf")
endif()
