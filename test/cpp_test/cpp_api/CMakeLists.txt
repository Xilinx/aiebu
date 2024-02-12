# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2024 Xilinx, Inc. All rights reserved.


PROJECT(cpp_api)

set(AIE2_TESTNAME "aie2.out")
set(AIE2PS_TESTNAME "aie2ps.out")

add_executable(${AIE2_TESTNAME} aie2_test.cpp)
add_executable(${AIE2PS_TESTNAME} aie2ps_test.cpp)

target_link_libraries(${AIE2_TESTNAME}
  PRIVATE
  aiebu
  )

target_link_libraries(${AIE2PS_TESTNAME}
  PRIVATE
  aiebu
  )

target_include_directories(${AIE2_TESTNAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/cpp/src/include)
target_include_directories(${AIE2PS_TESTNAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/cpp/src/include)

if (NOT DEFINED ${INSTALL_DIR})
  set(INSTALL_DIR "/../../${CMAKE_INSTALL_PREFIX}/aiebu/bin/cpp_api")
endif()

install(TARGETS ${AIE2_TESTNAME}
  RUNTIME DESTINATION ${AIEBU_INSTALL_DIR}/bin/cpp_api)

install(TARGETS ${AIE2PS_TESTNAME}
  RUNTIME DESTINATION ${AIEBU_INSTALL_DIR}/bin/cpp_api)
