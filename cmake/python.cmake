# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.
if (AIEBU_FULL STREQUAL "ON")
  find_program (PYLINT pylint REQUIRED)
  message("-- Pylint path: ${PYLINT}")

  find_package(Python3 COMPONENTS Interpreter REQUIRED)
  message("-- Python version: ${Python3_VERSION}")
endif()
