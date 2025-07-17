# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

project(dtrace_test)

set(DTRACE_TESTNAME "dtrace_test")

add_executable(${DTRACE_TESTNAME} dtrace_test.cpp)

add_dependencies(${DTRACE_TESTNAME} cert_dtrace)

add_custom_command(OUTPUT control_buffer_single.dat
  COMMAND ${DTRACE_TESTNAME} "${AIEBU_BINARY_DIR}/src/cpp/dtrace/libcert_dtrace.so" "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col/script.ct" "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col/map.json" "control_buffer_single.dat"
  DEPENDS "${DTRACE_TESTNAME}"
  COMMENT "Control block for single column dtrace test"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_custom_target(dtracesinglecolumn ALL
  DEPENDS control_buffer_single.dat
  )

add_test(NAME "dtrace_single_column"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_single.dat" "${AIEBU_SOURCE_DIR}/test/dtrace_test/single_col/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_custom_command(OUTPUT control_buffer_multi.dat
  COMMAND ${DTRACE_TESTNAME} "${AIEBU_BINARY_DIR}/src/cpp/dtrace/libcert_dtrace.so" "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col/script.ct" "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col/map.json" "control_buffer_multi.dat"
  DEPENDS "${DTRACE_TESTNAME}"
  COMMENT "Control block for multi column dtrace test"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_custom_target(dtracemulticolumn ALL
  DEPENDS control_buffer_multi.dat
  )

add_test(NAME "dtrace_multi_column"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_multi.dat" "${AIEBU_SOURCE_DIR}/test/dtrace_test/multi_col/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_custom_command(OUTPUT control_buffer_profile.dat
  COMMAND ${DTRACE_TESTNAME} "${AIEBU_BINARY_DIR}/src/cpp/dtrace/libcert_dtrace.so" "${AIEBU_SOURCE_DIR}/test/dtrace_test/profile/script.ct" "${AIEBU_SOURCE_DIR}/test/dtrace_test/profile/map.json" "control_buffer_profile.dat"
  DEPENDS "${DTRACE_TESTNAME}"
  COMMENT "Control block for profile dtrace test"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_custom_target(dtracprofile ALL
  DEPENDS control_buffer_profile.dat
  )

add_test(NAME "dtrace_profile"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_profile.dat" "${AIEBU_SOURCE_DIR}/test/dtrace_test/profile/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
