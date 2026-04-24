# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

project(dtrace_test)

set(DTRACE_TESTNAME "dtrace_test")

add_executable(${DTRACE_TESTNAME} dtrace_test.cpp)
add_dependencies(${DTRACE_TESTNAME} cert_dtrace_static)
target_link_libraries(${DTRACE_TESTNAME} PRIVATE cert_dtrace_static)
target_include_directories(${DTRACE_TESTNAME} PRIVATE ${AIEBU_SOURCE_DIR}/src/cpp/dtrace)

add_test(NAME "dtrace_single_column"
  COMMAND ${DTRACE_TESTNAME} "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col" "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_single.dat"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_single_column_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_single.dat" "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_single_column_md5sum" PROPERTIES DEPENDS "dtrace_single_column")

add_test(NAME "dtrace_multi_column"
  COMMAND ${DTRACE_TESTNAME} "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col" "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi.dat"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_multi_column_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi.dat" "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_multi_column_md5sum" PROPERTIES DEPENDS "dtrace_multi_column")

add_test(NAME "dtrace_profile"
  COMMAND ${DTRACE_TESTNAME} "${AIEBU_SOURCE_DIR}/test/dtrace_test/profile" "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_profile.dat"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_profile_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_profile.dat" "${AIEBU_SOURCE_DIR}/test/dtrace_test/profile/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_profile_md5sum" PROPERTIES DEPENDS "dtrace_profile")
