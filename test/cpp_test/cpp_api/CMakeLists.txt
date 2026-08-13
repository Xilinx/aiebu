# SPDX-License-Identifier: MIT
# Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.
PROJECT(cpp_api)

set(AIE2_TESTNAME "aie2_cpp")
add_executable(${AIE2_TESTNAME} aie2_test.cpp)
target_link_libraries(${AIE2_TESTNAME} PRIVATE aiebu_static)
target_include_directories(${AIE2_TESTNAME} PRIVATE ${AIEBU_SOURCE_DIR}/src/cpp/include)

set(AIE2PS_TESTNAME "aie2ps_cpp")
set(CHECKSUMS_FILE "${CMAKE_CURRENT_BINARY_DIR}/checksums.txt")
add_executable(${AIE2PS_TESTNAME} aie2ps_test.cpp)
target_link_libraries(${AIE2PS_TESTNAME} PRIVATE aiebu_static)
target_include_directories(${AIE2PS_TESTNAME} PRIVATE ${AIEBU_SOURCE_DIR}/src/cpp/include)

set(AIE2_FULLELF_TESTNAME "aie2_full_elf_cpp")
add_executable(${AIE2_FULLELF_TESTNAME} aie2_full_elf_test.cpp)
target_link_libraries(${AIE2_FULLELF_TESTNAME} PRIVATE aiebu_static)
target_include_directories(${AIE2_FULLELF_TESTNAME} PRIVATE ${AIEBU_SOURCE_DIR}/src/cpp/include)

set(AIE4_SAVETIMESTAMPS_TESTNAME "aie4_savetimestamps_cpp")
add_executable(${AIE4_SAVETIMESTAMPS_TESTNAME} aie4_savetimestamps_test.cpp)
target_link_libraries(${AIE4_SAVETIMESTAMPS_TESTNAME} PRIVATE aiebu_static)
target_include_directories(${AIE4_SAVETIMESTAMPS_TESTNAME} PRIVATE
  ${AIEBU_SOURCE_DIR}/src/cpp/include
  ${AIEBU_ELFIO_SRC_DIR})
if (NOT WIN32)
  target_link_libraries(${AIE4_SAVETIMESTAMPS_TESTNAME} PRIVATE pthread)
endif()

set(AIEBU_ERROR_TESTNAME "aiebu_error_cpp")
add_executable(${AIEBU_ERROR_TESTNAME} aiebu_error_test.cpp)
target_link_libraries(${AIEBU_ERROR_TESTNAME} PRIVATE aiebu_static)
target_include_directories(${AIEBU_ERROR_TESTNAME} PRIVATE ${AIEBU_SOURCE_DIR}/src/cpp/include)
if (NOT WIN32)
  target_link_libraries(${AIEBU_ERROR_TESTNAME} PRIVATE pthread)
endif()

set(AIE_COREDUMP_TESTNAME "aie_coredump_cpp")
add_executable(${AIE_COREDUMP_TESTNAME} aie_coredump_test.cpp)
target_link_libraries(${AIE_COREDUMP_TESTNAME} PRIVATE aiebu_static)
target_include_directories(${AIE_COREDUMP_TESTNAME} PRIVATE ${AIEBU_SOURCE_DIR}/src/cpp/include)
if (NOT WIN32)
  target_link_libraries(${AIE_COREDUMP_TESTNAME} PRIVATE pthread)
endif()

set(ELF_COMPRESS_DECOMPRESS_TESTNAME "elf_compress_decompress_cpp")
add_executable(${ELF_COMPRESS_DECOMPRESS_TESTNAME} elf_compress_decompress_test.cpp)
target_link_libraries(${ELF_COMPRESS_DECOMPRESS_TESTNAME} PRIVATE aiebu_static)
target_include_directories(${ELF_COMPRESS_DECOMPRESS_TESTNAME} PRIVATE
  ${AIEBU_SOURCE_DIR}/src/cpp/include
  ${AIEBU_ELFIO_SRC_DIR})
if (NOT WIN32)
  target_link_libraries(${ELF_COMPRESS_DECOMPRESS_TESTNAME} PRIVATE pthread)
endif()

set(DECOMPRESS_MEM_REPORT_NAME "decompress_memory_report")
add_executable(${DECOMPRESS_MEM_REPORT_NAME} decompress_memory_report.cpp)
target_link_libraries(${DECOMPRESS_MEM_REPORT_NAME} PRIVATE aiebu_static)
target_include_directories(${DECOMPRESS_MEM_REPORT_NAME} PRIVATE
  ${AIEBU_SOURCE_DIR}/src/cpp/include
  ${AIEBU_SOURCE_DIR}/src/cpp
  ${AIEBU_ELFIO_SRC_DIR})
if (NOT WIN32)
  target_link_libraries(${DECOMPRESS_MEM_REPORT_NAME} PRIVATE pthread)
endif()

# ELF compression ratio reporter — not a CTest test (requires a user-supplied directory).
# Usage: elf_compression_ratio <dir> --arch <aie4|aie2ps> --mode <plain|compress|decompress>
#        elf_compression_ratio --mode decompress-file --input <compressed.elf> --output <plain.elf>
# For accurate per-section peak RSS measurement use decompress_memory_report instead.
set(ELF_COMPRESSION_RATIO_NAME "elf_compression_ratio")
add_executable(${ELF_COMPRESSION_RATIO_NAME} elf_compression_ratio.cpp)
target_link_libraries(${ELF_COMPRESSION_RATIO_NAME} PRIVATE aiebu_static)
target_include_directories(${ELF_COMPRESSION_RATIO_NAME} PRIVATE
  ${AIEBU_SOURCE_DIR}/src/cpp/include
  ${AIEBU_SOURCE_DIR}/src/cpp)
if (NOT WIN32)
  target_link_libraries(${ELF_COMPRESSION_RATIO_NAME} PRIVATE pthread)
endif()


if (NOT WIN32)
  target_link_libraries(${AIE2_TESTNAME} PRIVATE pthread)
  target_link_libraries(${AIE2PS_TESTNAME} PRIVATE pthread)
endif()

add_custom_command(OUTPUT ctrl_pkt0.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${AIEBU_SOURCE_DIR}/test/cpp_test/aie2ps/eff_net_ctrlpacket/ctrl_pkt/ctrl_pkt0.b64" ctrl_pkt0.bin
  # COMMAND ${CMAKE_COMMAND} -E copy "ctrl_pkt0.bin" "${AIEBU_SOURCE_DIR}/test/cpp_test/aie2ps/eff_net_ctrlpacket/ctrl_pkt/"
  DEPENDS "${AIEBU_SOURCE_DIR}/test/cpp_test/aie2ps/eff_net_ctrlpacket/ctrl_pkt/ctrl_pkt0.b64"
  COMMENT "Decoding base64 ctrlcode and ctrlpkt to binary in ${AIEBU_SOURCE_DIR}/test/cpp_test/aie2ps/eff_net_ctrlpacket"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM)

add_custom_target(aie2psbins ALL
  DEPENDS ctrl_pkt0.bin)

add_test(NAME "aie2ps_cpp_eff_net_coal"
  COMMAND ${AIE2PS_TESTNAME} "eff_net_coal" "${AIEBU_SOURCE_DIR}/test/cpp_test/aie2ps/eff_net_coal/"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

#add_test(NAME "aie4_move_ddr_to_memtile_ctrlpkt"
#  COMMAND ${AIE2PS_TESTNAME} "move_ddr_to_memtile_ctrlpkt" "${AIEBU_SOURCE_DIR}/test/cpp_test/aie4/move_ddr_to_memtile_ctrlpkt/"
#  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie4_move_ddr_to_memtile"
  COMMAND ${AIE2PS_TESTNAME} "move_ddr_to_memtile" "${AIEBU_SOURCE_DIR}/test/cpp_test/aie4/move_ddr_to_memtile/"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "elf_compress_decompress"
  COMMAND ${ELF_COMPRESS_DECOMPRESS_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/cpp_test/aie4/move_ddr_to_memtile"
    "${AIEBU_SOURCE_DIR}/test/cpp_test/aie2ps/eff_net_coal"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# add_test(NAME "aie2ps_cpp_eff_net_ctrlpacket"
#   COMMAND ${AIE2PS_TESTNAME} "eff_net_ctrlpacket" "${AIEBU_SOURCE_DIR}/test/cpp_test/aie2ps/eff_net_ctrlpacket/"
#   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Check checksum only then boost > 1.70 else there is mismatch in uid because of endianness issue
# https://github.com/boostorg/uuid/pull/109
# https://github.com/boostorg/uuid/issues/111
if(Boost_VERSION VERSION_GREATER 1.70)
  add_test(NAME "aie2ps_cpp_eff_net_coal_md5sum"
    COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/eff_net_coal.elf" "${AIEBU_SOURCE_DIR}/test/cpp_test/aie2ps/eff_net_coal/gold.md5"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_tests_properties("aie2ps_cpp_eff_net_coal_md5sum" PROPERTIES DEPENDS "aie2ps_cpp_eff_net_coal")

  add_test(NAME "aie2ps_cpp_eff_net_coal_modified_md5sum"
    COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/eff_net_coal_modified.elf" "${AIEBU_SOURCE_DIR}/test/cpp_test/aie2ps/eff_net_coal/gold_modified.md5"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_tests_properties("aie2ps_cpp_eff_net_coal_modified_md5sum" PROPERTIES DEPENDS "aie2ps_cpp_eff_net_coal")

# add_test(NAME "aie2ps_cpp_eff_net_ctrlpacket_md5sum"
#   COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/eff_net_ctrlpacket.elf" "${AIEBU_SOURCE_DIR}/test/cpp_test/aie2ps/eff_net_ctrlpacket/gold.md5"
#   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

add_test(NAME "aie2_cpp_4x4"
  COMMAND ${AIE2_TESTNAME} "${AIEBU_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/gen/preempt_restore_stx_4x4.bin"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2_cpp_4x8"
  COMMAND ${AIE2_TESTNAME} "${AIEBU_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/gen/preempt_restore_stx_4x8.bin"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

#add_test(NAME "aie2_tinyyolo_preempt"
#  COMMAND ${AIE2_FULLELF_TESTNAME} "tinyyolo_preempt" "${AIEBU_SOURCE_DIR}/test/cpp_test/aie2/tinyyolo_preempt/"
#  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

#add_test(NAME "aie2_tinyyolo_preempt_cp"
#  COMMAND ${AIE2_FULLELF_TESTNAME} "tinyyolo_preempt_cp" "${AIEBU_SOURCE_DIR}/test/cpp_test/aie2/tinyyolo_preempt_cp/"
#  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie2_cpp_4x4" PROPERTIES LABELS memcheck)
set_tests_properties("aie2_cpp_4x8" PROPERTIES LABELS memcheck)

add_test(NAME "aie4_1col_preempt_savetimestamps_config"
  COMMAND ${AIE4_SAVETIMESTAMPS_TESTNAME} "config" "${AIEBU_SOURCE_DIR}/test/aie4-ctrlcode/1col_preempt"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie4_1col_preempt_savetimestamps_target"
  COMMAND ${AIE4_SAVETIMESTAMPS_TESTNAME} "target" "${AIEBU_SOURCE_DIR}/test/aie4-ctrlcode/1col_preempt"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Duplicate ASM file name error tests (both cases run in the same binary;
# exit code 0 means every test case reported PASS).
add_test(NAME "aiebu_error_duplicate_asm_filename"
  COMMAND ${AIEBU_ERROR_TESTNAME}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# AIE Coredump ELF generation tests.
add_test(NAME "aie_coredump_elf_generation"
  COMMAND ${AIE_COREDUMP_TESTNAME}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

################################################################
# aiebu::elf reader tests (step 1.1 exit criterion)
#
# One binary, driven with different ELF files and expected platform
# names.  Checked-in ELFs cover aie2ps_legacy, aie2ps, and aie4.
# Assembler-produced ELFs (from aie2ps_cpp / aie4 tests above)
# cover aie2ps and aie4 with group-format ELFs.
################################################################
set(ELF_READER_TESTNAME "elf_reader_cpp")
add_executable(${ELF_READER_TESTNAME} elf_reader_test.cpp)
target_link_libraries(${ELF_READER_TESTNAME} PRIVATE aiebu_static)
target_include_directories(${ELF_READER_TESTNAME} PRIVATE ${AIEBU_SOURCE_DIR}/src/cpp/include)
if (NOT WIN32)
  target_link_libraries(${ELF_READER_TESTNAME} PRIVATE pthread)
endif()

# Checked-in ELFs — always available, no assembler dependency
add_test(NAME "elf_reader_aie2ps_legacy"
  COMMAND ${ELF_READER_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col/multi_uc_6.elf"
    aie2ps_legacy
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "elf_reader_aie2ps"
  COMMAND ${ELF_READER_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col/simple_int8_orig.elf"
    aie2ps
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "elf_reader_aie4_merged"
  COMMAND ${ELF_READER_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/duplicate_filename/filename.elf"
    aie4
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# aie2p fixture — exercises the aie2p-only guards in elf_reader_test.cpp:
#   test_has_pdi_value()        (F1: has_pdi() is section-based, drives ERT opcode)
#   test_pdi_symbols_crosscheck (F2: get_pdi_symbols() == PDI patch points)
# The 4th argument "pdi"/"nopdi" asserts the expected has_pdi() value.
#
# The test/aie2-ctrlcode/* tests assemble aie2p ELFs (aiebu-asm -t aie2txn/aie2asm
# -> elf_aie2 -> osabi_aie2p 0x45).  "basic" produces basic.elf via the
# "aie2_basic_txn" test.  That ELF has no .pdi section, so expect "nopdi";
# this fully exercises F2 and the false branch of F1.  To also guard the F1
# true branch (.pdi present but no relocation), supply a PDI-bearing aie2p ELF
# and pass "pdi".
#
# NOTE: aie2_basic_txn writes basic.elf into ITS OWN binary dir
# (test/aie2-ctrlcode/basic), not this one.  Reference it by absolute build
# path and add the DEPENDS below when enabling.  Adjust if the fixture moves.
# add_test(NAME "elf_reader_aie2p"
#   COMMAND ${ELF_READER_TESTNAME}
#     "${CMAKE_BINARY_DIR}/test/aie2-ctrlcode/basic/basic.elf"
#     aie2p
#     nopdi
#   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
# set_tests_properties("elf_reader_aie2p" PROPERTIES DEPENDS "aie2_basic_txn")

# Assembler-produced ELFs — depend on the assembler tests having run first
add_test(NAME "elf_reader_aie2ps_group"
  COMMAND ${ELF_READER_TESTNAME}
    "${CMAKE_CURRENT_BINARY_DIR}/eff_net_coal.elf"
    aie2ps_legacy
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties("elf_reader_aie2ps_group" PROPERTIES DEPENDS "aie2ps_cpp_eff_net_coal")

add_test(NAME "elf_reader_aie4_group"
  COMMAND ${ELF_READER_TESTNAME}
    "${CMAKE_CURRENT_BINARY_DIR}/move_ddr_to_memtile_in_mem.elf"
    aie2ps_legacy
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties("elf_reader_aie4_group" PROPERTIES DEPENDS "aie4_move_ddr_to_memtile")
