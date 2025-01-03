# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc.

# Helper CMake script for base64 encode/decode functions
# You can manually run this script using the following synopsys:
# A manual command line for reference is following--
# cmake -P b64.cmake <-e|-d> <input_file> <output_file>
# e.g. cmake -P b64.cmake -e ctrl_pkt0.bin ctrl_pkt0.b64
# e.g. cmake -P b64.cmake -d ctrl_pkt0.b64 ctrl_pkt0.bin

cmake_minimum_required(VERSION 3.18)

set(DECODE_SWITCH "-d")
set(ENCODE_SWITCH "")

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
  find_program (B64TOOL base64 REQUIRED)
else()
  find_program (B64TOOL certutil REQUIRED)
  set(DECODE_SWITCH "-decode")
  set(ENCODE_SWITCH "-encode")
endif()

set(B64SWITCH "")

if (${CMAKE_ARGV3} STREQUAL "-e")
  message("-- Encoding binary file ${CMAKE_ARGV4} to ascii file ${CMAKE_ARGV5}")
  set(B64SWITCH ${ENCODE_SWITCH})
elseif(${CMAKE_ARGV3} STREQUAL "-d")
  message("-- Decoding ascii file ${CMAKE_ARGV4} to binary file ${CMAKE_ARGV5}")
  set(B64SWITCH ${DECODE_SWITCH})
else()
  message(FATAL_ERROR "-- Invalid switch ${CMAKE_ARGV3}, only -e or -d allowed")
endif()

file(REMOVE "${CMAKE_ARGV5}")

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
  execute_process(COMMAND ${B64TOOL} ${B64SWITCH} ${CMAKE_ARGV4} OUTPUT_FILE ${CMAKE_ARGV5})
else()
  execute_process(COMMAND ${B64TOOL} ${B64SWITCH} ${CMAKE_ARGV4} ${CMAKE_ARGV5})
endif()
