# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc.

set(MSVC_LEGACY_LINKING False)
if (MSVC AND (AIEBU_MSVC_LEGACY_LINKING))
  set(MSVC_LEGACY_LINKING True)
endif()

# Invoke nested cmake on a aiebu sample client application to verify
# that cmake find_package(aiebu) is working fine. The sample has its
# own independent cmake which uses find_package(aiebu) to find aiebu
# exported headers and aiebu static library

# The test relies on setting CMAKE_PREFIX_PATH to the install staging
# area, but this area is user (build script) chosen and independent of
# CMAKE_INSTALL_PREFIX.  These tests are enabled only if the staging area
# can be found, otherwise skipped.

if (WIN32)
  set(AIEBU_CHECK_AND_RUN_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/check_and_run.bat)
else()
  set(AIEBU_CHECK_AND_RUN_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/check_and_run.sh)
endif()

add_test(NAME "aiebu_cmake"
  COMMAND ${AIEBU_CHECK_AND_RUN_SCRIPT} ${AIEBU_INSTALL_STAGING_DIR}
  cmake 
  "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}" 
  "-DCMAKE_PREFIX_PATH=${AIEBU_INSTALL_STAGING_DIR}" 
  "-DAIEBU_MSVC_LEGACY_LINKING=${MSVC_LEGACY_LINKING}" 
  "${CMAKE_CURRENT_SOURCE_DIR}/sample"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Now build and run the sample test
add_test(NAME "aiebu_cmake_build"
  COMMAND ${AIEBU_CHECK_AND_RUN_SCRIPT} ${AIEBU_INSTALL_STAGING_DIR}
  cmake --build . --verbose --config $<CONFIG>
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties(aiebu_cmake PROPERTIES SKIP_RETURN_CODE 77)
set_tests_properties(aiebu_cmake_build PROPERTIES SKIP_RETURN_CODE 77)
