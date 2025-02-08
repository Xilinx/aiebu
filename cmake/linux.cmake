# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.
find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBELF REQUIRED libelf)
message("-- Libelf version: ${LIBELF_VERSION}")

if (AIEBU_FULL STREQUAL "ON")
  find_program (READELF eu-readelf REQUIRED)
  message("-- Readelf path: ${READELF}")
endif()

add_compile_options(-Wall -Wextra -Werror)
