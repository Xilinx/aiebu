# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc.

add_custom_command(OUTPUT mc_code_txn.bin ctrl_pkt.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/mc_code_txn.b64" mc_code_txn.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/ctrl_pkt.b64" ctrl_pkt.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/pm_ctrlpkt_0.b64" pm_ctrlpkt_0.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/pm_ctrlpkt_1.b64" pm_ctrlpkt_1.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/pm_ctrlpkt_2.b64" pm_ctrlpkt_2.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/pm_ctrlpkt_3.b64" pm_ctrlpkt_3.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/pm_ctrlpkt_4.b64" pm_ctrlpkt_4.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/pm_ctrlpkt_5.b64" pm_ctrlpkt_5.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/pm_ctrlpkt_6.b64" pm_ctrlpkt_6.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/pm_ctrlpkt_7.b64" pm_ctrlpkt_7.bin
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/mc_code_txn.b64"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ctrl_pkt.b64"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/pm_ctrlpkt_0.b64"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/pm_ctrlpkt_1.b64"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/pm_ctrlpkt_2.b64"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/pm_ctrlpkt_3.b64"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/pm_ctrlpkt_4.b64"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/pm_ctrlpkt_5.b64"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/pm_ctrlpkt_6.b64"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/pm_ctrlpkt_7.b64"
  COMMENT "Decoding base64 ctrlcode and control packets to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM)

add_custom_target(aie2_pmctrlpkt ALL
  DEPENDS mc_code_txn.bin ctrl_pkt.bin)

# Assemble a ctrlcode txn binary, a ctrl_pkt and associated JSON to ELF
add_test(NAME "aie2_pmctrlpkt_txn"
  COMMAND aiebu-asm -r -t aie2txn -c mc_code_txn.bin -p ctrl_pkt.bin -m 0:pm_ctrlpkt_0.bin -m 1:pm_ctrlpkt_1.bin -m 2:pm_ctrlpkt_2.bin -m 3:pm_ctrlpkt_3.bin -m 4:pm_ctrlpkt_4.bin -m 5:pm_ctrlpkt_5.bin -m 6:pm_ctrlpkt_6.bin -m 7:pm_ctrlpkt_7.bin -j "${CMAKE_CURRENT_SOURCE_DIR}/mc_code_txn.json" -o mc_code_txn.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Assemble a ctrlcode ASM text, a ctrl_pkt and associated JSON to ELF
add_test(NAME "aie2_pmctrlpkt_asm"
  COMMAND aiebu-asm -r -t aie2asm -c "${CMAKE_CURRENT_SOURCE_DIR}/mc_code_txn.ctrltext.asm" -p ctrl_pkt.bin -m 0:pm_ctrlpkt_0.bin -m 1:pm_ctrlpkt_1.bin -m 2:pm_ctrlpkt_2.bin -m 3:pm_ctrlpkt_3.bin -m 4:pm_ctrlpkt_4.bin -m 5:pm_ctrlpkt_5.bin -m 6:pm_ctrlpkt_6.bin -m 7:pm_ctrlpkt_7.bin -j "${CMAKE_CURRENT_SOURCE_DIR}/mc_code_txn.json" -o ctrl1.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties(
  "aie2_pmctrlpkt_asm"
  PROPERTIES
  TIMEOUT 3500
)

# Compile a ctrlcode ASM text to binary and then decompile it into ASM text
add_test(NAME "aie2_pmctrlpkt_asm_redo"
  COMMAND aiebu-asm -r -t aie2asm -c "ctrl1.ctrltext.asm" -p ctrl_pkt.bin -m 0:pm_ctrlpkt_0.bin -m 1:pm_ctrlpkt_1.bin -m 2:pm_ctrlpkt_2.bin -m 3:pm_ctrlpkt_3.bin -m 4:pm_ctrlpkt_4.bin -m 5:pm_ctrlpkt_5.bin -m 6:pm_ctrlpkt_6.bin -m 7:pm_ctrlpkt_7.bin -j "${CMAKE_CURRENT_SOURCE_DIR}/mc_code_txn.json" -o ctrl2.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties(
  "aie2_pmctrlpkt_asm_redo"
  PROPERTIES
  TIMEOUT 3500
)

# Compare the two ctrlcodes: ASM text to decompiled ASM text
add_test(NAME "aie2_pmctrlpkt_asm_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol ctrl1.ctrltext.asm ctrl2.ctrltext.asm
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties(
  "aie2_pmctrlpkt_asm_compare"
  PROPERTIES
  TIMEOUT 3500
)

# Compare the md5sum of ELF generated in aie2_mc_code_txn_txn test with that of the golden file
add_test(NAME "aie2_pmctrlpkt_txn_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/mc_code_txn.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2_pmctrlpkt_compare_ctrldata"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol mc_code_txn.ctrldata.ctrl "${CMAKE_CURRENT_SOURCE_DIR}/mc_code_txn.ctrldata.ctrl"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

foreach(index RANGE 7)
  set(tfile "mc_code_txn.ctrlpkt.pm.${index}.ctrl")
  set(tname "aie2_pmctrlpkt_compare_ctrlpkt_${index}")
  add_test(NAME "${tname}"
    COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol ${tfile} "${CMAKE_CURRENT_SOURCE_DIR}/${tfile}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_tests_properties("${tname}" PROPERTIES DEPENDS "aie2_pmctrlpkt_txn")
endforeach()

# Test interdependencies used by cmake when running ctest with -j
set_tests_properties("aie2_pmctrlpkt_asm_redo" PROPERTIES DEPENDS "aie2_pmctrlpkt_asm")
set_tests_properties("aie2_pmctrlpkt_asm_compare" PROPERTIES DEPENDS "aie2_pmctrlpkt_asm;aie2_pmctrlpkt_asm_redo")
set_tests_properties("aie2_pmctrlpkt_txn_md5sum" PROPERTIES DEPENDS "aie2_pmctrlpkt_txn")
set_tests_properties("aie2_pmctrlpkt_compare_ctrldata" PROPERTIES DEPENDS "aie2_pmctrlpkt_txn")

set_tests_properties("aie2_pmctrlpkt_txn" PROPERTIES LABELS memcheck)
set_tests_properties("aie2_pmctrlpkt_asm" PROPERTIES LABELS memcheck)
