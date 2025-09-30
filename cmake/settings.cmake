# SPDX-License-Identifier: MIT
# Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(POLICY CMP0177)
  cmake_policy(SET CMP0177 NEW)
endif()

################################################################
# Version settings
################################################################
set(AIEBU_VERSION_RELEASE 202510)
SET(AIEBU_VERSION_MAJOR 1)
SET(AIEBU_VERSION_MINOR 0)

message("-- Host system processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message("-- Target system processor is ${CMAKE_SYSTEM_PROCESSOR}")

set(AIEBU_NATIVE_BUILD TRUE)
if (NOT ${CMAKE_SYSTEM_PROCESSOR} STREQUAL ${CMAKE_HOST_SYSTEM_PROCESSOR})
  set(AIEBU_NATIVE_BUILD FALSE)
endif()

if (DEFINED $ENV{AIEBU_VERSION_PATCH})
  SET(AIEBU_VERSION_PATCH $ENV{AIEBU_VERSION_PATCH})
else(DEFINED $ENV{AIEBU_VERSION_PATCH})
  SET(AIEBU_VERSION_PATCH 0)
endif(DEFINED $ENV{AIEBU_VERSION_PATCH})

# Also update cache to set version for external plug-in .so
set(AIEBU_SOVERSION ${AIEBU_VERSION_MAJOR} CACHE INTERNAL "")
set(AIEBU_VERSION_STRING ${AIEBU_VERSION_MAJOR}.${AIEBU_VERSION_MINOR}.${AIEBU_VERSION_PATCH} CACHE INTERNAL "")

################################################################
# Standard code snippet to identify parent project if we are a submodule
################################################################
set(AIEBU_GIT_SUBMODULE FALSE)

find_package(Git)
if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --show-superproject-working-tree
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_SUPERPROJECT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
  )

  if(GIT_SUPERPROJECT)
    message("-- Building AIEBU as a submodule of ${GIT_SUPERPROJECT}")
    set(AIEBU_GIT_SUBMODULE TRUE)
  endif()
endif()
################################################################

################################################################
# Install directories
################################################################
message("-- CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")

include (GNUInstallDirs)
message("-- CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
message("-- CMAKE_INSTALL_BINDIR=${CMAKE_INSTALL_BINDIR}")
message("-- CMAKE_INSTALL_LIBDIR=${CMAKE_INSTALL_LIBDIR}")
message("-- CMAKE_INSTALL_INCLUDEDIR=${CMAKE_INSTALL_INCLUDEDIR}")

set(AIEBU_INSTALL_DIR ".")

if (WIN32)
  set(AIEBU_INSTALL_BIN_DIR         ${AIEBU_INSTALL_DIR})
  # On windows the build script uses multi-config build, meaning same
  # binary dir is used for Release and Debug.  At install time,
  # artifacts are staged into Release/ and Debug/ subdirectories
  set(AIEBU_INSTALL_STAGING_DIR     ${CMAKE_BINARY_DIR}/$<CONFIG>/xilinx/aiebu)
else()
  set(AIEBU_INSTALL_BIN_DIR         ${AIEBU_INSTALL_DIR}/${CMAKE_INSTALL_BINDIR})
  # On Linux the build script uses separate binary dir for Release or Debug
  set(AIEBU_INSTALL_STAGING_DIR     ${CMAKE_BINARY_DIR}/opt/xilinx/aiebu)
endif()

set(AIEBU_INSTALL_LIB_DIR           ${AIEBU_INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR})
set(AIEBU_INSTALL_INCLUDE_DIR       ${AIEBU_INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR})
set(AIEBU_INSTALL_CMAKE_DIR         ${AIEBU_INSTALL_DIR}/share/cmake/${PROJECT_NAME})
set(AIEBU_PYTHON_INSTALL_DIR        ${AIEBU_INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}/python3)
set(AIEBU_SPECIFICATION_INSTALL_DIR ${AIEBU_INSTALL_DIR}/share/specification)
set(AIEBU_GEN_DIR                   ${AIEBU_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/gen)

# If this repository is used as a submodule, the parent repository may
# set the following variables in CMake to make aiebu point to the
# parents copy of ELFIO and/or AIE-RT.
#
# For example, XRT parent repository can set the following in its
# CMake for aiebu to inherit the XRT build of aie-rt:
#   set(AIEBU_AIE-RT_BIN_DIR ${XRT_BINARY_DIR}/src/runtime_src/aie-rt/driver/src)
#   set(AIEBU_AIE-RT_HEADER_DIR ${AIEBU_AIE-RT_BIN_DIR}/include)
# or it can set the source dir from which AIEBU should build aie-rt:
#   set(AIEBU_AIE-RT_SRC_DIR ${XRT_SOURCE_DIR}/src/runtime_src/aie-rt)
# If no AIEBU_AIE-RT_* variables are set, aiebu will build aie-rt
# from its own submodule aie-rt in lib/aie-rt.
#
# For using XRT's copy of ELFIO, XRT can set the following in its
# CMake for aiebu to inherit:
#   set(AIEBU_ELFIO_SRC_DIR ${XRT_SOURCE_DIR}/src/runtime_src/core/common/elf)
if (DEFINED AIEBU_AIE-RT_BIN_DIR AND DEFINED AIEBU_AIE-RT_HEADER_DIR)
  message("-- AIEBU uses aie-rt binaries from ${AIEBU_AIE-RT_BIN_DIR}")
  message("-- AIEBU uses aie-rt headers from ${AIEBU_AIE-RT_HEADER_DIR}")
elseif (NOT DEFINED AIEBU_AIE-RT_SRC_DIR)
  set(AIEBU_AIE-RT_SRC_DIR ${AIEBU_SOURCE_DIR}/lib/aie-rt)
endif()

if (DEFINED AIEBU_AIE-RT_SRC_DIR)
  set(AIEBU_AIE-RT_HEADER_DIR ${AIEBU_BINARY_DIR}/lib/aie-rt/driver/driver-src/include)
  message("-- AIEBU builds aie-rt binaries from ${AIEBU_AIE-RT_SRC_DIR}")
  message("-- AIEBU uses aie-rt headers from ${AIEBU_AIE-RT_HEADER_DIR}")
endif()

if (NOT DEFINED AIEBU_ELFIO_SRC_DIR)
  set(AIEBU_ELFIO_SRC_DIR "${AIEBU_SOURCE_DIR}/src/cpp/ELFIO")
endif()
message("-- AIEBU is using ELFIO from ${AIEBU_ELFIO_SRC_DIR}")
################################################################
# Global compile options
################################################################
if (MSVC)
  include(cmake/windows.cmake)
else()
  include(cmake/linux.cmake)
endif()

include(cmake/utils.cmake)
include(cmake/boost.cmake)
include(cmake/clang-tidy.cmake)
if (AIEBU_PYTHON STREQUAL "ON")
  include(cmake/python.cmake)
endif()
include(cmake/test.cmake)
