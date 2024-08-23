# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc.

if (${AIEBU_AIE_RT_BIN_DIR} STREQUAL ${AIEBU_BINARY_DIR})
  set(XAIENGINE_BUILD_SHARED OFF CACHE BOOL "Force static build of xaiengine library" FORCE)
  if (MSVC)
    add_compile_options(/WX- /D_CRT_SECURE_NO_WARNINGS)
  endif()
  add_subdirectory(aie-rt/driver)
endif()

add_subdirectory(src)
