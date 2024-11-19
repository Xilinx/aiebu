# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.


PROJECT(cpp_api)

set(AIE2_TESTNAME "aie2_cpp.out")

add_executable(${AIE2_TESTNAME} aie2_test.cpp)

target_link_libraries(${AIE2_TESTNAME}
  PRIVATE
  aiebu
  )

target_include_directories(${AIE2_TESTNAME} PRIVATE ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/include)

if (AIEBU_FULL STREQUAL "ON")
  set(AIE2PS_TESTNAME "aie2ps_cpp.out")
  add_executable(${AIE2PS_TESTNAME} aie2ps_test.cpp)
  target_link_libraries(${AIE2PS_TESTNAME}
    PRIVATE
    aiebu
    )
  target_include_directories(${AIE2PS_TESTNAME} PRIVATE ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/include)
endif()
