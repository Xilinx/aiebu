# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

if (MSVC)
  add_compile_options(/WX /W4)
else()
  add_compile_options(-Wall -Wextra -Werror)
endif()

add_subdirectory(cpp_api)
add_subdirectory(c_api)
