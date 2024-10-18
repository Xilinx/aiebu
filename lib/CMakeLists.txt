# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc.

if (${AIEBU_AIE_RT_BIN_DIR} STREQUAL ${AIEBU_BINARY_DIR})
  message("-- Enabling build of aie-rt as submodule of aiebu")
  set(XAIENGINE_BUILD_SHARED OFF CACHE BOOL "Force static build of xaiengine library" FORCE)
#  if (MSVC)
 #   message("-- Suppressing warnings in aie-rt")
    #add_compile_options(/WX- /D_CRT_SECURE_NO_WARNINGS)
  #  add_compile_options(/Wd4703)
 # endif()
  if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_compile_options("-Wno-error=unused-parameter")
  endif()
  add_subdirectory(aie-rt/driver)
endif()

add_subdirectory(src)
