# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc.


# On Windows CMAKE_INSTALL_PREFIX includes AIEBU_BINARY_DIR hence we should
# not add the former only on Linux
set(PREDIR "")
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
  set(PREDIR "${AIEBU_BINARY_DIR}/")
endif()

set(MSVC_LEGACY_LINKING False)
if (MSVC AND (AIEBU_MSVC_LEGACY_LINKING))
  set(MSVC_LEGACY_LINKING True)
endif()

# Invoke nested cmake on a aiebu sample client application to verify that cmake find_package(aiebu) is working fine
# The sample has its own independent cmake which uses find_package(aiebu) to find aiebu exported headers and aiebu static library
add_test(NAME "aiebu_cmake"
  COMMAND cmake "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}" "-DCMAKE_PREFIX_PATH=${PREDIR}${CMAKE_INSTALL_PREFIX}/${AIEBU_INSTALL_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/sample" "-DAIEBU_MSVC_LEGACY_LINKING=${MSVC_LEGACY_LINKING}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Now build and run the sample test
add_test(NAME "aiebu_cmake_build"
  COMMAND cmake --build . --verbose --config ${CMAKE_BUILD_TYPE}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
