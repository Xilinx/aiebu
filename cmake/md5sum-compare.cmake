# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc.

# Helper CMake script to generate md5sum of a give file and compare it with golden hash
# stored in another file, or compare md5sums of two files directly.
# Usage 1: cmake -P md5sum-compare.cmake <bin_file> <computed_checksum_gold_file_name>
# Usage 2: cmake -P md5sum-compare.cmake --compare-files <file1> <file2>
# e.g. cmake -P cmake/md5sum-compare.cmake build/Debug/test/aie2-ctrlcode/basic/basic.elf test/aie2-ctrlcode/basic/gold.md5
# e.g. cmake -P cmake/md5sum-compare.cmake --compare-files file1.bin file2.bin

cmake_minimum_required(VERSION 3.18)

# Check if we're in compare-files mode
if (CMAKE_ARGV3 STREQUAL "--compare-files")
  message(CHECK_START "-- Comparing md5sum of ${CMAKE_ARGV4} with ${CMAKE_ARGV5}")

  # Compute md5sum of both files
  file(MD5 ${CMAKE_ARGV4} file1_sum)
  file(MD5 ${CMAKE_ARGV5} file2_sum)

  string(COMPARE EQUAL ${file1_sum} ${file2_sum} RSUM)

  if (NOT RSUM)
    message(FATAL_ERROR "File md5sum mismatch: ${CMAKE_ARGV4} (${file1_sum}) vs ${CMAKE_ARGV5} (${file2_sum})")
  endif()

  message(CHECK_PASS "passed")
else()
  # Compare file with golden hash
  message(CHECK_START "-- Comparing checksum of ${CMAKE_ARGV3} with gold ${CMAKE_ARGV4}")

  # Compute md5sum of the binary file
  file(MD5 ${CMAKE_ARGV3} tsum)
  # Read the golden hash and strip out newlines/white spaces
  file(READ ${CMAKE_ARGV4} gsum)
  string(STRIP ${gsum} csum)

  string(COMPARE EQUAL ${tsum} ${csum} RSUM)

  if (NOT RSUM)
    message(FATAL_ERROR "${CMAKE_ARGV3} md5sum mismatch: ${tsum} vs ${csum}")
  endif()

  message(CHECK_PASS "passed")
endif()
