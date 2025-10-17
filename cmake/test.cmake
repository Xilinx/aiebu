# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.
if (AIEBU_GIT_SUBMODULE)
  enable_testing()
else()
  # This enables valgrind but we should not include (CTest) when we
  # are a submodule as it may conflict with configurations of the
  # parent project
  set(CTEST_TEST_TIMEOUT 3600)
  include (CTest)
  set(CMAKE_CTEST_ARGUMENTS "--output-on-failure")
endif()
