# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.


PROJECT(cpp_api)

set(AIE2_TESTNAME "aie2_cpp")

add_executable(${AIE2_TESTNAME} aie2_test.cpp)
target_link_libraries(${AIE2_TESTNAME}
  PRIVATE
  aiebu_static
  )
target_include_directories(${AIE2_TESTNAME} PRIVATE ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/include)

if (AIEBU_FULL STREQUAL "ON")
  set(AIE2PS_TESTNAME "aie2ps_cpp")
  set(CHECKSUMS_FILE "${CMAKE_CURRENT_BINARY_DIR}/checksums.txt")
  add_executable(${AIE2PS_TESTNAME} aie2ps_test.cpp)
  target_link_libraries(${AIE2PS_TESTNAME}
    PRIVATE
    aiebu_static
    )
  target_include_directories(${AIE2PS_TESTNAME} PRIVATE ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/include)

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

  # add_test(NAME "aie2ps_cpp_eff_net_ctrlpacket"
  #   COMMAND ${AIE2PS_TESTNAME} "eff_net_ctrlpacket" "${AIEBU_SOURCE_DIR}/test/cpp_test/aie2ps/eff_net_ctrlpacket/"
  #   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  add_test(NAME "aie2ps_cpp_eff_net_coal_md5sum"
    COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/eff_net_coal.elf" "${AIEBU_SOURCE_DIR}/test/cpp_test/aie2ps/eff_net_coal/gold.md5"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  # add_test(NAME "aie2ps_cpp_eff_net_ctrlpacket_md5sum"
  #   COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/eff_net_ctrlpacket.elf" "${AIEBU_SOURCE_DIR}/test/cpp_test/aie2ps/eff_net_ctrlpacket/gold.md5"
  #   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

endif()

add_test(NAME "aie2_cpp_4x4"
  COMMAND ${AIE2_TESTNAME} "${AIEBU_BINARY_DIR}/lib/gen/preempt_restore_stx_4x4.bin"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2_cpp_4x8"
  COMMAND ${AIE2_TESTNAME} "${AIEBU_BINARY_DIR}/lib/gen/preempt_restore_stx_4x8.bin"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie2_cpp_4x4" PROPERTIES LABELS memcheck)
set_tests_properties("aie2_cpp_4x8" PROPERTIES LABELS memcheck)
