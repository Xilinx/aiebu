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
  /W4           # warning level
  /sdl          # enable security checks
  /ZH:SHA_256   # enable secure source code hashing
  /guard:cf     # enable compiler control guard feature (CFG) to prevent attackers from redirecting execution to unsafe locations
  )

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_options(/Qspectre)  # compile with the Spectre mitigations switch
endif()

# Release: explicit max-speed opts (/O2; /Ot favors fast code over smaller code) and
# no checked STL iterators. Base CMAKE_CXX_FLAGS_RELEASE from CMake is /O2 /Ob2 /DNDEBUG;
# we restate /O2 here so Release tuning stays explicit in this file.
# NOTE: Put /O2 and /Ot in separate genexes — a single "$<...:/O2;/Ot>" breaks because ';'
# splits the generator-expression parse and can leak "$<1:/O2" onto the cl command line.
add_compile_options("$<$<STREQUAL:$<CONFIG>,Release>:/O2>")
add_compile_options("$<$<STREQUAL:$<CONFIG>,Release>:/Ot>")
add_compile_definitions($<$<STREQUAL:$<CONFIG>,Release>:_ITERATOR_DEBUG_LEVEL=0>)

add_link_options(
  /guard:cf   # enable linker control guard feature (CFG) to prevent attackers from redirecting execution to unsafe locations
  )

set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>")

if (NOT ${CMAKE_CXX_COMPILER} MATCHES "(arm64|ARM64)")
    add_link_options(/CETCOMPAT) # enable Control-flow Enforcement Technology (CET) Shadow Stack mitigation
endif()

set(AIEBU_OS_SOURCE_DIR windows)
