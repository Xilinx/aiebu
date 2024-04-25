# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2023 Advanced Micro Devices, Inc.

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  # aie-rt main branch does not build on Windows because of
  # C:\scratch\XRT_aiebu\actions-runner\_work\aiebu\aiebu\lib\aie-rt\driver\src\routing\xaie_routing.c(296,30): error C2057: expected constant expression
add_subdirectory(aie-rt/driver)
add_subdirectory(src)
endif()
