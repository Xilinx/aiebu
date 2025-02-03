# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc.

if (${AIEBU_AIE_RT_BIN_DIR} STREQUAL ${AIEBU_BINARY_DIR})
  message("-- Enabling build of aie-rt as submodule of aiebu")
  set(XAIENGINE_BUILD_SHARED OFF CACHE BOOL "Force static build of xaiengine library" FORCE)
  if (MSVC)
     if (NOT (AIEBU_MSVC_LEGACY_LINKING))
       add_compile_options(/MT$<$<CONFIG:Debug>:d>  # static linking with the CRT
         )
    endif()
  endif()
  if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_compile_options("-Wno-error=unused-parameter")
  endif()
  aiebu_add_subdirectory_disable_install_target(aie-rt/driver)
endif()

add_subdirectory(src)
