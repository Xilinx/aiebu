# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

# Declared here so a standalone aiebu build has the flag; as an XRT submodule
# the parent's cache option carries through and this is a no-op.
option(ENABLE_ASAN "Build with AddressSanitizer (/fsanitize=address). Requires MSVC v145 (VS 2026) for ARM64." OFF)

# This flag is set to enable legacy linking in windows
# If this is not set, aiebu will have hybrid linking
message("-- AIEBU_MSVC_LEGACY_LINKING=${AIEBU_MSVC_LEGACY_LINKING}")

if (NOT AIEBU_MSVC_LEGACY_LINKING)
  if (NOT ENABLE_ASAN)
    # Explicit /MT only on the non-ASAN path. Under ASAN the CRT is dynamic (/MD),
    # driven by CMAKE_MSVC_RUNTIME_LIBRARY, so cert_dtrace_static/aiebu_static match
    # the /MD xrt_coreutil.dll they are linked into. An explicit /MT here would be
    # mapped into <RuntimeLibrary> by the VS generator and force these static libs
    # back to /MT, mismatching the instrumented /MD runtime (LNK2038/LNK1319).
    add_compile_options(/MT$<$<CONFIG:Debug>:d>  # static linking with the CRT
      )
  endif()
  add_link_options(
    /NODEFAULTLIB:libucrt$<$<CONFIG:Debug>:d>.lib  # Hybrid CRT
    /DEFAULTLIB:ucrt$<$<CONFIG:Debug>:d>.lib       # Hybrid
    )
endif()

option(AIEBU_EXPLICIT_DEBUG_FLAGS
  "Set /Zi and /DEBUG explicitly for all build types. Parent projects may set to OFF to control debug info format externally."
  ON)

add_compile_options(
  /Zc:__cplusplus
  /WX           # treat warnings as errors
  /W4           # warning level
  /sdl          # enable security checks
  /ZH:SHA_256   # enable secure source code hashing
  /guard:cf     # enable compiler control guard feature (CFG) to prevent attackers from redirecting execution to unsafe locations
  )

if(AIEBU_EXPLICIT_DEBUG_FLAGS)
  add_compile_options(/Zi)
endif()

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

if(AIEBU_EXPLICIT_DEBUG_FLAGS)
  add_link_options(/DEBUG)
endif()

add_link_options(
  /guard:cf   # enable linker control guard feature (CFG) to prevent attackers from redirecting execution to unsafe locations
  )

if(NOT AIEBU_EXPLICIT_DEBUG_FLAGS)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
    "$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>")
endif()

if (NOT ${CMAKE_CXX_COMPILER} MATCHES "(arm64|ARM64)")
    add_link_options(/CETCOMPAT) # enable Control-flow Enforcement Technology (CET) Shadow Stack mitigation
endif()

set(AIEBU_OS_SOURCE_DIR windows)
