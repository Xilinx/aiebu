# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

# Helper CMake script for concatenating binary files
# You can manually run this script using the following synopsis:
# cmake -P concat.cmake input_file1 input_file2 -o output_file
# Manual command line for reference:
# e.g. cmake -P concat.cmake file1.bin file2.bin -o concatenated.bin

cmake_minimum_required(VERSION 3.18)

set(INPUT_FILE1 "${CMAKE_ARGV3}")
set(INPUT_FILE2 "${CMAKE_ARGV4}")

if (NOT "${CMAKE_ARGV5}" STREQUAL "-o")
  message(FATAL_ERROR "-o option is required before output file")
endif()

set(OUTPUT_FILE "${CMAKE_ARGV6}")

message("-- Concatenating files: ${INPUT_FILE1} ${INPUT_FILE2}")
message("-- Output file: ${OUTPUT_FILE}")

if (NOT EXISTS "${INPUT_FILE1}")
  message(FATAL_ERROR "Input file does not exist: ${INPUT_FILE1}")
endif()

if (NOT EXISTS "${INPUT_FILE2}")
  message(FATAL_ERROR "Input file does not exist: ${INPUT_FILE2}")
endif()

file(REMOVE "${OUTPUT_FILE}")

file(READ "${INPUT_FILE1}" file_content1 BINARY)
file(APPEND "${OUTPUT_FILE}" "${file_content1}")
message("-- Appended: ${INPUT_FILE1}")

file(READ "${INPUT_FILE2}" file_content2 BINARY)
file(APPEND "${OUTPUT_FILE}" "${file_content2}")
message("-- Appended: ${INPUT_FILE2}")

message("-- Concatenation complete: ${OUTPUT_FILE}")
