# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

set(Boost_USE_STATIC_LIBS ON)
INCLUDE (FindBoost)
message("-- Boost version: ${Boost_VERSION}")
find_package(Boost REQUIRED COMPONENTS program_options)

if (MSVC)
  # Cannot use /W4 since ELFIO has three warnings that cause build failure with /WX
  add_compile_options(/WX /W2)
else()
  add_compile_options(-Wall -Wextra -Werror)
endif()

add_subdirectory(src)
add_subdirectory(utils)
