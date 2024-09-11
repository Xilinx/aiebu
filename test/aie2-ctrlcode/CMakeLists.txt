# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc.
# Validate checksums of generated ctrlcode
add_test(NAME "aie2_ctrlcode_md5sum"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol "${AIEBU_BINARY_DIR}/lib/gen/checksums.txt" "${CMAKE_CURRENT_SOURCE_DIR}/checksums.txt")
