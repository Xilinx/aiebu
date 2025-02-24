# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc.

cmake_minimum_required(VERSION 3.18)
project(aiebu-cmake)

find_package(AIEBU REQUIRED)

message("-- aiebu version: ${AIEBU_VERSION}")

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
  add_compile_options(/WX /W2)
  if (NOT (AIEBU_MSVC_LEGACY_LINKING))
    add_compile_options(/MT$<$<CONFIG:Debug>:d>  # static linking with the CRT
      )
    add_link_options(
      /NODEFAULTLIB:libucrt$<$<CONFIG:Debug>:d>.lib  # Hybrid CRT
      /DEFAULTLIB:ucrt$<$<CONFIG:Debug>:d>.lib       # Hybrid
      )
  endif()
else()
  add_compile_options(-Wall -Wextra -Werror)
endif()

add_executable(aiebu-client aie2_test.cpp)
target_include_directories(aiebu-client PRIVATE ${AIEBU_INCLUDE_DIRS})
target_link_libraries(aiebu-client PRIVATE AIEBU::aiebu_static)

add_custom_target(aiebuclientrun ALL
  COMMAND "aiebu-client" "-h"
  DEPENDS aiebu-client)
