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
  add_executable(${AIE2PS_TESTNAME} aie2ps_test.cpp)
  target_link_libraries(${AIE2PS_TESTNAME}
    PRIVATE
    aiebu_static
    )
  target_include_directories(${AIE2PS_TESTNAME} PRIVATE ${AIEBU_SOURCE_DIR}/src/cpp/aiebu/src/include)
endif()

add_test(NAME "aie2_cpp_4x4"
  COMMAND ${AIE2_TESTNAME} "${AIEBU_BINARY_DIR}/lib/gen/preempt_restore_stx_4x4.bin"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2_cpp_4x8"
  COMMAND ${AIE2_TESTNAME} "${AIEBU_BINARY_DIR}/lib/gen/preempt_restore_stx_4x8.bin"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
