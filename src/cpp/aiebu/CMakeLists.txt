# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

set(Boost_USE_STATIC_LIBS ON)
INCLUDE (FindBoost)
message("-- Boost version: ${Boost_VERSION}")
find_package(Boost REQUIRED COMPONENTS program_options)

if (MSVC)
  # Cannot use /W4 since ELFIO has three warnings that cause build failure with /WX
  add_compile_options(/WX /W2)
  add_compile_options($<$<CONFIG:Debug>:/MTd>
                      $<$<CONFIG:Release>:/MT>)
  add_link_options($<$<CONFIG:Debug>:/NODEFAULTLIB:libucrtd.lib>
                   $<$<CONFIG:Debug>:/DEFAULTLIB:ucrtd.lib>
                   $<$<CONFIG:Release>:/NODEFAULTLIB:libucrt.lib>
                   $<$<CONFIG:Release>:/DEFAULTLIB:ucrt.lib>)
else()
  add_compile_options(-Wall -Wextra -Werror)
endif()

add_subdirectory(src)
add_subdirectory(utils)
