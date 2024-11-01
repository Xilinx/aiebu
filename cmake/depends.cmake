# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc.

# Helper CMake script to generate dependencies of an executable  binary
# A manual command line for reference is following--
# cmake -P depends.cmake <exe_name> <computed_checksum_file_name>
# e.g. cmake -P depends.cmake Debug/src/cpp/aiebu/utils/asm/aiebu-asm Debug/depends.txt

cmake_minimum_required(VERSION 3.18)

set(DEPENDS_TOOL "ldd")

set(DEPENDS_TOOL_SWITCH1 "")
set(DEPENDS_TOOL_SWITCH2 "")

if (WIN32)
  set(DEPENDS_TOOL "dumpbin")
  set(DEPENDS_TOOL_SWITCH1 "/nologo")
  set(DEPENDS_TOOL_SWITCH2 "/dependents")
endif()

find_program(DEPENDS_TOOL "${DEPENDS_TOOL}")
if(NOT DEPENDS_TOOL)
  message(FATAL_ERROR "${DEPENDS_TOOL} not found, cannot determine binary dependency")
endif()

message("-- Generating dependency file ${CMAKE_ARGV4} for binary ${CMAKE_ARGV3}")
message("-- Working directory ${CMAKE_CURRENT_BINARY_DIR}")

file(REMOVE "${CMAKE_ARGV4}")

execute_process(
  COMMAND ${DEPENDS_TOOL} ${DEPENDS_TOOL_SWITCH1} ${DEPENDS_TOOL_SWITCH2} ${CMAKE_ARGV3}
  OUTPUT_VARIABLE DEPENDS_OUT
)

message("${CMAKE_ARGV3}:\n${DEPENDS_OUT}")
file(WRITE ${CMAKE_ARGV4} "${CMAKE_ARGV3}:\n")
file(APPEND ${CMAKE_ARGV4} "${DEPENDS_OUT}")
