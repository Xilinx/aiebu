# SPDX-License-Identifier: MIT
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

target_include_directories(${AIE2_TESTNAME} PRIVATE ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/include)
target_include_directories(${AIE2PS_TESTNAME} PRIVATE ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/include)
