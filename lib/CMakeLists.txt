# SPDX-License-Identifier: MIT
# Copyright (C) 2024-2025 Advanced Micro Devices, Inc.
set(XAIENGINE_BUILD_SHARED OFF CACHE BOOL "Force static build of xaiengine library" FORCE)
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  add_compile_options("-Wno-error=unused-parameter")
endif()
aiebu_add_subdirectory_disable_install_target(${AIEBU_AIE-RT_SRC_DIR}/driver ${AIEBU_BINARY_DIR}/lib/aie-rt/driver)

if (AIEBU_NATIVE_BUILD)
  add_subdirectory(src)
endif()
