# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.
find_package(PkgConfig REQUIRED)

if (AIEBU_PYTHON STREQUAL "ON")
  pkg_check_modules(LIBELF REQUIRED libelf)
  message("-- Libelf version: ${LIBELF_VERSION}")

  find_program (READELF eu-readelf REQUIRED)
endif()
add_compile_options(-Wall -Wextra -Werror -fvisibility=hidden)

if (WIN32)
    set(AIEBU_OS_SOURCE_DIR windows)
else()
    set(AIEBU_OS_SOURCE_DIR linux)
endif()
