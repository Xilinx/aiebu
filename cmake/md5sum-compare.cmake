# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc.

# Helper CMake script to generate md5sum of a give file and compare it with golden hash
# stored in another file.
# cmake -P md5sum-compare.cmake <bin_file> <computed_checksum_gold_file_name>
# e.g. cmake -P cmake/md5sum-compare.cmake build/Debug/test/aie2-ctrlcode/basic/basic.elf test/aie2-ctrlcode/basic/gold.md5

cmake_minimum_required(VERSION 3.18)

message(CHECK_START "-- Comparing checksum of ${CMAKE_ARGV3} with gold ${CMAKE_ARGV4}")

# Compute md5sum of the binary file
file(MD5 ${CMAKE_ARGV3} tsum)
# Read the golden hash and strip out newlines/white spaces
file(READ ${CMAKE_ARGV4} gsum)
string(STRIP ${gsum} csum)

string(COMPARE EQUAL ${tsum} ${csum} RSUM)

if (NOT RSUM)
  message(FATAL_ERROR "${CMAKE_ARGV3} md5sum mismatch: ${tsum} vs ${gsum}")
endif()
