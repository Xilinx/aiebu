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

# Test mem action across multiple lines and columns
add_test(NAME "dtrace_mem_action"
  COMMAND ${DTRACE_TESTNAME} "${AIEBU_SOURCE_DIR}/test/dtrace_test/mem_action"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_mem_action.dat"
  "python"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_mem_action_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_mem_action.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/mem_action/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_mem_action_md5sum" PROPERTIES DEPENDS "dtrace_mem_action")

add_test(NAME "dtrace_mem_action_result_md5sum"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_mem_action.py"
  "${CMAKE_CURRENT_SOURCE_DIR}/mem_action/gold_result.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_mem_action_result_md5sum" PROPERTIES DEPENDS "dtrace_mem_action")

# Test large compile-time generated jprobe script
add_test(NAME "dtrace_compile_time"
  COMMAND ${DTRACE_TESTNAME} "${AIEBU_SOURCE_DIR}/test/dtrace_test/compile_time"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_compile_time.dat"
  "python"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_compile_time_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_compile_time.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/compile_time/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_compile_time_md5sum" PROPERTIES DEPENDS "dtrace_compile_time")

add_test(NAME "dtrace_compile_time_result_md5sum"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_compile_time.py"
  "${CMAKE_CURRENT_SOURCE_DIR}/compile_time/gold_result.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_compile_time_result_md5sum" PROPERTIES DEPENDS "dtrace_compile_time")

# Test yolox latency script with extracted aie_control.elf dump map
add_test(NAME "dtrace_yolox_latency"
  COMMAND ${DTRACE_TESTNAME} "${AIEBU_SOURCE_DIR}/test/dtrace_test/yolox_latency"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_yolox_latency.dat"
  "python"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_yolox_latency" PROPERTIES TIMEOUT 180)

add_test(NAME "dtrace_yolox_latency_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_yolox_latency.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/yolox_latency/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_yolox_latency_md5sum" PROPERTIES DEPENDS "dtrace_yolox_latency")

add_test(NAME "dtrace_yolox_latency_result_md5sum"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol 
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_yolox_latency.py"
  "${CMAKE_CURRENT_SOURCE_DIR}/yolox_latency/gold_result.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_yolox_latency_result_md5sum" PROPERTIES DEPENDS "dtrace_yolox_latency")

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

# Test full elf with kernel:instance (explicit instance name)
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

# Test full elf with kernel only (no instance suffix) — single-instance kernel support
add_test(NAME "dtrace_elf_with_kernel_no_instance"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/elf_handle"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_elf_with_kernel_no_instance.dat"
    "python"
    "elf" "${AIEBU_SOURCE_DIR}/test/dtrace_test/elf_handle/test.elf" "CTRL"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_elf_with_kernel_no_instance_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_elf_with_kernel_no_instance.dat"
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/elf_handle/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_elf_with_kernel_no_instance_md5sum" PROPERTIES DEPENDS "dtrace_elf_with_kernel_no_instance")

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
# The result script is verified against gold_result_dwarf.py; operation is
# decoded via ISA binary walk (format_operation) and matches the lowercased mnemonic.

# Generate multi-graph-dwarf.elf from source (avoids storing a large binary in the repo)
add_test(NAME "dtrace_generate_dwarf_elf"
  COMMAND aiebu-asm
    -t aie4_config
    -j "${AIEBU_SOURCE_DIR}/test/aie4-ctrlcode/multi_graph/config.json"
    -o "${CMAKE_CURRENT_BINARY_DIR}/multi-graph-dwarf.elf"
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

set_tests_properties("dtrace_dwarf_elf" PROPERTIES DEPENDS "dtrace_generate_dwarf_elf")

# Verify the DWARF result script matches the expected gold.
# operation is decoded from the ISA binary via format_operation (lowercased mnemonic + args).
add_test(NAME "dtrace_dwarf_elf_result_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_dwarf_elf.py"
    "${CMAKE_CURRENT_SOURCE_DIR}/dwarf_elf/gold_result_dwarf.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

set_tests_properties("dtrace_dwarf_elf_result_compare" PROPERTIES
  DEPENDS "dtrace_dwarf_elf")

# DWARF ELF multi-column annotation dtrace test — validates that the {column, address} cache key
# in write_trace_probes_from_dwarf() and dwarf_rows_to_json() correctly distinguishes
# rows from different AIE columns (uc0 on column 0, uc2 on column 2).
# ELF is generated from test/aie4-ctrlcode/multicol_annot_dwarf (two .attach_to_group sections).
# jprobes target line and annotation probes on both columns:
#   uc0:line7, uc0:annotation0, uc0:annotation1 (start_job 0 / uc_dma_write_des_sync / end_job)
#   uc2:line7, uc2:annotation0, uc2:annotation1 (start_job 1 / uc_dma_write_des_sync / end_job)

add_test(NAME "dtrace_generate_multicol_annot_dwarf"
  COMMAND aiebu-asm
    -t aie4_config
    -j "${AIEBU_SOURCE_DIR}/test/aie4-ctrlcode/multicol_annot_dwarf/config.json"
    -o "${CMAKE_CURRENT_BINARY_DIR}/multicol_annot_dwarf.elf"
    -f fulldump
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_multicol_annot_dwarf"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/multicol_annot_dwarf"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multicol_annot_dwarf.dat"
    "python"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/multicol_annot_dwarf.elf" "DPU:dpu"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multicol_annot_dwarf" PROPERTIES
  DEPENDS "dtrace_generate_multicol_annot_dwarf")

add_test(NAME "dtrace_multicol_annot_dwarf_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multicol_annot_dwarf.dat"
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/multicol_annot_dwarf/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multicol_annot_dwarf_md5sum" PROPERTIES
  DEPENDS "dtrace_multicol_annot_dwarf")

add_test(NAME "dtrace_multicol_annot_dwarf_result_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multicol_annot_dwarf.py"
    "${CMAKE_CURRENT_SOURCE_DIR}/multicol_annot_dwarf/gold_result_dwarf_multicol.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multicol_annot_dwarf_result_compare" PROPERTIES
  DEPENDS "dtrace_multicol_annot_dwarf")

# DWARF ELF multi-column dtrace test (aie2ps_config) — real multi-column workload
# with 6 AIE columns (uc0, uc4, uc8, uc12, uc16, uc20) and complex script.ct exercising
# jprobe, tracepoint, read_mem, write_mem, read_reg, mask_write_reg across columns.
# ELF is generated from test/dtrace_test/multi_col_dwarf/Work/aiebu_config.json in-place;
# ASM files are resolved relative to the config file by aiebu-asm.
# Output format: json (result file: control_buffer_multi_col_dwarf.json).
# kernel instance: DPU:dpu

add_test(NAME "dtrace_generate_multi_col_dwarf"
  COMMAND aiebu-asm
    -t aie2ps_config
    -j "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col_dwarf/Work/aiebu_config.json"
    -o "${CMAKE_CURRENT_BINARY_DIR}/multi_col_dwarf.elf"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_multi_col_dwarf"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col_dwarf"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi_col_dwarf.dat"
    "json"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/multi_col_dwarf.elf" "DPU:dpu"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multi_col_dwarf" PROPERTIES
  DEPENDS "dtrace_generate_multi_col_dwarf")

add_test(NAME "dtrace_multi_col_dwarf_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi_col_dwarf.dat"
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col_dwarf/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multi_col_dwarf_md5sum" PROPERTIES
  DEPENDS "dtrace_multi_col_dwarf")

add_test(NAME "dtrace_multi_col_dwarf_result_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi_col_dwarf.json"
    "${CMAKE_CURRENT_SOURCE_DIR}/multi_col_dwarf/gold_result_multi_col_dwarf.json"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multi_col_dwarf_result_compare" PROPERTIES
  DEPENDS "dtrace_multi_col_dwarf")

# DWARF ELF single-column dtrace test (aie2ps_config) — single-column workload (uc0)
# with jprobe, tracepoint, read_mem, write_mem, read_reg, mask_write_reg probes.
# ELF is generated from test/dtrace_test/single_col_dwarf/Work/aiebu_config.json in-place.
# Output format: json (result file: control_buffer_single_col_dwarf.json).
# kernel instance: DPU:dpu

add_test(NAME "dtrace_generate_single_col_dwarf"
  COMMAND aiebu-asm
    -t aie2ps_config
    -j "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col_dwarf/Work/aiebu_config.json"
    -o "${CMAKE_CURRENT_BINARY_DIR}/single_col_dwarf.elf"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "dtrace_single_col_dwarf"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col_dwarf"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_single_col_dwarf.dat"
    "json"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/single_col_dwarf.elf" "DPU:dpu"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties("dtrace_single_col_dwarf" PROPERTIES
  DEPENDS "dtrace_generate_single_col_dwarf")

add_test(NAME "dtrace_single_col_dwarf_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_single_col_dwarf.dat"
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col_dwarf/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties("dtrace_single_col_dwarf_md5sum" PROPERTIES
  DEPENDS "dtrace_single_col_dwarf")

add_test(NAME "dtrace_single_col_dwarf_result_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_single_col_dwarf.json"
    "${CMAKE_CURRENT_SOURCE_DIR}/single_col_dwarf/gold_result_single_col_dwarf.json"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties("dtrace_single_col_dwarf_result_compare" PROPERTIES
  DEPENDS "dtrace_single_col_dwarf")

add_test(NAME "dtrace_single_col_dwarf_py"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col_dwarf"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_single_col_dwarf_py.dat"
    "python"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/single_col_dwarf.elf" "DPU:dpu"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties("dtrace_single_col_dwarf_py" PROPERTIES
  DEPENDS "dtrace_generate_single_col_dwarf")

# Validate DWARF sections of generated ELFs with llvm-dwarfdump --verify (Linux only).
# This provides an independent check that .debug_abbrev, .debug_info, .debug_line, and
# .debug_str are spec-compliant — something the internal dwarf_reader cannot verify since
# it uses a hardcoded schema and does not parse .debug_abbrev at all.
find_program(LLVM_DWARFDUMP llvm-dwarfdump)
if (LLVM_DWARFDUMP)
  add_test(NAME "dwarf_verify_single_instance"
    COMMAND ${LLVM_DWARFDUMP} --verify "${CMAKE_CURRENT_BINARY_DIR}/multi-graph-dwarf.elf"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_tests_properties("dwarf_verify_single_instance" PROPERTIES
    DEPENDS "dtrace_generate_dwarf_elf")

  add_test(NAME "dwarf_verify_multicol_annot_dwarf"
    COMMAND ${LLVM_DWARFDUMP} --verify "${CMAKE_CURRENT_BINARY_DIR}/multicol_annot_dwarf.elf"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_tests_properties("dwarf_verify_multicol_annot_dwarf" PROPERTIES
    DEPENDS "dtrace_generate_multicol_annot_dwarf")

  add_test(NAME "dwarf_verify_multi_col_dwarf"
    COMMAND ${LLVM_DWARFDUMP} --verify "${CMAKE_CURRENT_BINARY_DIR}/multi_col_dwarf.elf"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_tests_properties("dwarf_verify_multi_col_dwarf" PROPERTIES
    DEPENDS "dtrace_generate_multi_col_dwarf")

  add_test(NAME "dwarf_verify_single_col_dwarf"
    COMMAND ${LLVM_DWARFDUMP} --verify "${CMAKE_CURRENT_BINARY_DIR}/single_col_dwarf.elf"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_tests_properties("dwarf_verify_single_col_dwarf" PROPERTIES
    DEPENDS "dtrace_generate_single_col_dwarf")
endif()

add_subdirectory(duplicate_filename)
