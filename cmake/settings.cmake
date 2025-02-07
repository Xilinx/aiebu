# SPDX-License-Identifier: MIT
# Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

################################################################
# Version settings
################################################################
set(AIEBU_VERSION_RELEASE 202510)
SET(AIEBU_VERSION_MAJOR 1)
SET(AIEBU_VERSION_MINOR 0)

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
if (NOT(AIEBU_GIT_SUBMODULE))
  if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(CMAKE_INSTALL_PREFIX "/opt/xilinx")
  else()
    set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/xilinx" CACHE PATH "..." FORCE)
  endif()
endif()

set(AIEBU_INSTALL_DIR "aiebu")
set(AIEBU_INSTALL_BIN_DIR           "${AIEBU_INSTALL_DIR}/bin")
set(AIEBU_INSTALL_LIB_DIR           "${AIEBU_INSTALL_DIR}/lib")
set(AIEBU_INSTALL_INCLUDE_DIR       "${AIEBU_INSTALL_DIR}/include")
set(AIEBU_PYTHON_INSTALL_DIR        "${AIEBU_INSTALL_DIR}/lib/python3")
set(AIEBU_SPECIFICATION_INSTALL_DIR "${AIEBU_INSTALL_DIR}/share/specification")

# If this repository is used as a submodule, the parent repository may
# set the following variables in CMake to make aiebu point to the
# parents copy of ELFIO and/or AIE-RT. e.g, XRT parent repository can
# set the following in its CMake for aiebu to inherit it:
# set(AIEBU_AIE_RT_BIN_DIR ${XRT_BINARY_DIR})
# set(AIEBU_ELFIO_SRC_DIR"${XRT_SOURCE_DIR}/src/runtime_src/core/common/elf")

# These variables may be defined by the parent project as it may also
# include AIE-RT and or ELFIO as a submodule AIEBU_AIE_RT_BIN_DIR,
# AIEBU_AIE_RT_HEADER_DIR and AIEBU_ELFIO_SRC_DIR
if (NOT (DEFINED AIEBU_AIE_RT_BIN_DIR))
  set(AIEBU_AIE_RT_BIN_DIR ${AIEBU_BINARY_DIR})
endif()

if (NOT (DEFINED AIEBU_AIE_RT_HEADER_DIR))
  set(AIEBU_AIE_RT_HEADER_DIR "${AIEBU_BINARY_DIR}/lib/aie-rt/driver/driver-src/include")
endif()

message("-- Using aie-rt headers from ${AIEBU_AIE_RT_HEADER_DIR}")
message("-- Using aie-rt build from ${AIEBU_AIE_RT_BIN_DIR}")

if (NOT (DEFINED AIEBU_ELFIO_SRC_DIR))
  set(AIEBU_ELFIO_SRC_DIR "${AIEBU_SOURCE_DIR}/src/cpp/ELFIO")
endif()

message("-- Using ELFIO from ${AIEBU_ELFIO_SRC_DIR}")
################################################################
# Global compile options
################################################################
if (AIEBU_FULL STREQUAL "ON")
  add_compile_options(-DAIEBU_FULL)
endif()

if (MSVC)
  include(cmake/windows.cmake)
else()
  include(cmake/linux.cmake)
endif()

include(cmake/utils.cmake)
include(cmake/boost.cmake)
include(cmake/clang-tidy.cmake)
include(cmake/python.cmake)
include(cmake/test.cmake)
