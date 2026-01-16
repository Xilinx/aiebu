# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc.

if (${Boost_VERSION} VERSION_GREATER_EQUAL "1.71.0")
  add_subdirectory(multi_graph)
  add_subdirectory(1col_preempt)
  add_subdirectory(multi_layer)
  add_subdirectory(move_ddr_to_memtile_ctrlpkt)
endif()
