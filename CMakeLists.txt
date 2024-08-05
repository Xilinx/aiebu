# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

cmake_minimum_required(VERSION 3.18.0)

project(AIEBU HOMEPAGE_URL https://github.com/Xilinx/aiebu)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(CMAKE_INSTALL_PREFIX "/opt/xilinx")
else()
  set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/xilinx" CACHE PATH "..." FORCE)
endif()


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(AIEBU_INSTALL_DIR "aiebu")

message("-- CMAKE_SYSTEM=${CMAKE_SYSTEM}")
message("-- CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")

find_package(Git REQUIRED)

if (DEFINED XRT_SOURCE_DIR)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --show-superproject-working-tree
    WORKING_DIRECTORY ${AIEBU_SOURCE_DIR}
    OUTPUT_VARIABLE PARENT_PROJECT_ROOT
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  message("-- Compiling AIEBU as a submodule of: ${PARENT_PROJECT_ROOT}")
  # Make AIEBU .clangd configuration for compile_commands.json point back to the compile_commands.json
  # in the top level repository
  SET(DEBUG_DIR_NAME "Debug")
  if (CMAKE_HOST_WIN32)
    SET(DEBUG_DIR_NAME "WDebug")
  endif()
  file(CREATE_LINK "${PARENT_PROJECT_ROOT}/build/${DEBUG_DIR_NAME}" "${AIEBU_SOURCE_DIR}/build/${DEBUG_DIR_NAME}" SYMBOLIC)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
message("-- Python version: ${Python3_VERSION}")

if (XRT_CLANG_TIDY STREQUAL "ON")
  find_program(CLANG_TIDY "clang-tidy")
  if(NOT CLANG_TIDY)
    message(FATAL_ERROR "clang-tidy not found, cannot enable static analysis")
  else()
    message("-- Enabling clang-tidy")
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
  endif()
endif()

enable_testing()

add_subdirectory(src/cpp/aiebu)

add_subdirectory(lib)

add_subdirectory(test/cpp_test)
add_subdirectory(test)

SET(PACKAGE_KIND "TGZ")
SET(CPACK_GENERATOR "TGZ")
message("-- ${CMAKE_BUILD_TYPE} ${PACKAGE_KIND} package")

SET(CPACK_PACKAGE_VENDOR "Advanced Micro Devices Inc.")
SET(CPACK_PACKAGE_CONTACT "runtimeca39d@amd.com")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "AMD XDNA binutils package")
SET(CPACK_RESOURCE_FILE_LICENSE "${AIEBU_SOURCE_DIR}/LICENSE")

INCLUDE(CPack)
