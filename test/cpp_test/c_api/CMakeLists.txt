# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.


PROJECT(c_api)

set(AIE2_TESTNAME "aie2_c.out")

add_executable(${AIE2_TESTNAME} aie2_test.c)

target_link_libraries(${AIE2_TESTNAME}
  PRIVATE
  aiebu_static
  )


target_include_directories(${AIE2_TESTNAME} PRIVATE ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/include)

if (AIEBU_FULL STREQUAL "ON")
  set(AIE2PS_TESTNAME "aie2ps_c.out")
  add_executable(${AIE2PS_TESTNAME} aie2ps_test.c)
  target_link_libraries(${AIE2PS_TESTNAME}
    PRIVATE
    aiebu_static
    )

  target_include_directories(${AIE2PS_TESTNAME} PRIVATE ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/include)
endif()
