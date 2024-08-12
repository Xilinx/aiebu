# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc.

if (NOT(DEFINED XRT_SOURCE_DIR))
  set(XAIENGINE_BUILD_SHARED OFF CACHE BOOL "Force static build of xaiengine library" FORCE)
  add_subdirectory(aie-rt/driver)
endif()

add_subdirectory(src)
