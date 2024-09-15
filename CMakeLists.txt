# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

cmake_minimum_required(VERSION 3.18.0)

project(AIEBU HOMEPAGE_URL https://gitenterprise.xilinx.com/XRT/aiebu)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(CMAKE_INSTALL_PREFIX "/opt/xilinx")
else()
  set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/xilinx" CACHE PATH "..." FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(AIEBU_INSTALL_DIR "aiebu")

message("-- CMAKE_SYSTEM=${CMAKE_SYSTEM}")
message("-- CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(AIEBU_SPECIFICATION_INSTALL_DIR "aiebu/share/specification")

find_program (PYLINT pylint REQUIRED)
message("-- Pylint path: ${PYLINT}")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  find_package(PkgConfig REQUIRED)

  pkg_check_modules(LIBELF REQUIRED libelf)
  message("-- Libelf version: ${LIBELF_VERSION}")

  find_program (READELF eu-readelf REQUIRED)
  message("-- Readelf path: ${READELF}")

  find_package(OpenSSL REQUIRED)
  message("-- OpenSSL path: ${OPENSSL_LIBRARIES}")
endif()

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

add_subdirectory(src/python)
add_subdirectory(src/cpp/aiebu)
add_subdirectory(specification)
add_subdirectory(scripts)

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
