# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

# This flag is set to enable legacy linking in windows
# If this is not set, aiebu will have hybrid linking
message("-- AIEBU_MSVC_LEGACY_LINKING=${AIEBU_MSVC_LEGACY_LINKING}")

if (NOT AIEBU_MSVC_LEGACY_LINKING)
  add_compile_options(/MT$<$<CONFIG:Debug>:d>  # static linking with the CRT
    )
  add_link_options(
    /NODEFAULTLIB:libucrt$<$<CONFIG:Debug>:d>.lib  # Hybrid CRT
    /DEFAULTLIB:ucrt$<$<CONFIG:Debug>:d>.lib       # Hybrid
    )
endif()

add_compile_options(
  /Zc:__cplusplus
  /WX           # treat warnings as errors
  /W2           # warning level
  /Zi           # generate pdb files even in release mode
  /sdl          # enable security checks
  /Qspectre     # compile with the Spectre mitigations switch
  /ZH:SHA_256   # enable secure source code hashing
  /guard:cf     # enable compiler control guard feature (CFG) to prevent attackers from redirecting execution to unsafe locations
  )
add_link_options(
  /DEBUG      # instruct linker to create debugging info
  /guard:cf   # enable linker control guard feature (CFG) to prevent attackers from redirecting execution to unsafe locations
  /CETCOMPAT  # enable Control-flow Enforcement Technology (CET) Shadow Stack mitigation
  )
