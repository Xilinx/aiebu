# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.
find_program(TREE_SITTER tree-sitter)
if (TREE_SITTER)
  message("-- tree-sitter found, basic tree-sitter parser will be generated (experimental)")
else()
  message("-- tree-sitter executable not found")
endif()
