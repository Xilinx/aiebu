# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc.

cmake_minimum_required(VERSION 3.18)
project(aiebu-cmake)

find_package(AIEBU REQUIRED)

message("-- aiebu version: ${AIEBU_VERSION}")

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
  add_compile_options(/WX /W2)
  # Use Hybrid linking on Windows
  if (NOT (AIEBU_MSVC_LEGACY_LINKING))
    add_compile_options($<$<CONFIG:Debug>:/MTd>
                        $<$<CONFIG:Release>:/MT>)
    add_link_options($<$<CONFIG:Debug>:/NODEFAULTLIB:libucrtd.lib>
                     $<$<CONFIG:Debug>:/DEFAULTLIB:ucrtd.lib>
                     $<$<CONFIG:Release>:/NODEFAULTLIB:libucrt.lib>
                     $<$<CONFIG:Release>:/DEFAULTLIB:ucrt.lib>)
  endif()
else()
  add_compile_options(-Wall -Wextra -Werror)
endif()

add_executable(aiebu-client aie2_test.cpp)

target_link_directories(aiebu-client PRIVATE ${AIEBU_LINK_DIRS})

target_link_libraries(aiebu-client PRIVATE AIEBU::aiebu_static)

target_include_directories(aiebu-client PRIVATE ${AIEBU_INCLUDE_DIRS})

add_custom_target(aiebuclientrun ALL
  COMMAND "aiebu-client" "-h"
  DEPENDS aiebu-client)
