# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc.

# Helper CMake script to generate md5sum of ctrlcode files in a given directory
# during build time (not CMake configure time). Inspired from approach suggested here--
# https://discourse.cmake.org/t/how-to-create-md5-file-within-add-custom-target/5156/4
# This CMake script is automatically run by cmake after the genartion of ctrlcode.
# You can manually run this script using the following synopsys:
# A manual command line for reference is following--
# cmake -P md5sum.cmake <dir_with_ctrlcode_bin_files> <computed_checksum_file_name>
# e.g. cmake -P md5sum.cmake Debug/lib/gen Debug/lib/gen/checksums.txt

cmake_minimum_required(VERSION 3.18)

file(GLOB tfiles "${CMAKE_ARGV3}/*.bin")

message("-- Generating checksum file ${CMAKE_ARGV4} from ctrlcode files in ${CMAKE_ARGV3}")

file(REMOVE "${CMAKE_ARGV4}")

foreach(tfile ${tfiles})

  cmake_path(GET tfile STEM sample)
  cmake_path(GET tfile FILENAME samplebin)
  file(MD5 "${tfile}" tsum)
  file(APPEND "${CMAKE_ARGV4}" "${tsum}  ${samplebin}\n")

endforeach()
