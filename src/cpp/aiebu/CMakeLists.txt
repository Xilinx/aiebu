# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.
if (MSVC)
  add_compile_options(/WX /W2)
  # Use Hybrid linking on Windows
  if (NOT (AIEBU_MSVC_LEGACY_LINKING))
    add_compile_options(/MT$<$<CONFIG:Debug>:d>  # static linking with the CRT
      )
    add_link_options(
      /NODEFAULTLIB:libucrt$<$<CONFIG:Debug>:d>.lib  # Hybrid CRT
      /DEFAULTLIB:ucrt$<$<CONFIG:Debug>:d>.lib       # Hybrid CRT
      )
  endif()
else()
  add_compile_options(-Wall -Wextra -Werror)
endif()

add_subdirectory(src)
add_subdirectory(utils)
