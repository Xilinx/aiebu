# SPDX-License-Identifier: MIT
# Copyright (C) 2024-2026 Advanced Micro Devices, Inc. All rights reserved.
cmake_minimum_required(VERSION 3.18...4.0)
project(AIEBU HOMEPAGE_URL https://github.com/Xilinx/aiebu VERSION 1.0)

if (WIN32 AND CMAKE_VERSION VERSION_LESS 3.24)
  message(FATAL_ERROR "CMake 3.24 or higher is required on Windows")
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
include(cmake/settings.cmake)
include(cmake/components.cmake)
include(cmake/version.cmake)

# ---------------------------------------------------------------------------
# zstd — external dependency managed as a git submodule (src/cpp/zstd).
#
# Both ELFIO and zstd are submodules, but they are consumed differently:
#   ELFIO  — header-only; integrated via include path only, no build step.
#   zstd   — compiled C library; sources are globbed directly (Detours-style)
#            rather than going through zstd's own CMakeLists.txt.
#
# Why glob sources rather than add_subdirectory(zstd/build/cmake)?
#   zstd's CMakeLists installs its own cmake export set (zstdExports /
#   zstdTargets_static.cmake). CMake forbids a target from belonging to two
#   export sets, so add_subdirectory creates an unsolvable conflict when
#   aiebu_static also references libzstd_static via its own aiebu-targets.cmake.
#   Globbing the sources directly defines a local-only target (aiebu_zstd)
#   that is never exported, sidestepping the conflict entirely and producing
#   a fully self-contained aiebu_static with no consumer-visible zstd dependency.
#
# Submodule rather than find_package: using find_package(zstd) would require
# zstd to be pre-installed on every build machine (developer workstations,
# CI runners, new team setups). The submodule avoids that burden — cloning
# the repo is sufficient for a working build, with no separate install step.
# ---------------------------------------------------------------------------
set(AIEBU_ZSTD_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/zstd/lib")

file(GLOB AIEBU_ZSTD_SOURCES
  "${AIEBU_ZSTD_LIB_DIR}/common/*.c"
  "${AIEBU_ZSTD_LIB_DIR}/compress/*.c"
  "${AIEBU_ZSTD_LIB_DIR}/decompress/*.c"
)

add_library(aiebu_zstd STATIC ${AIEBU_ZSTD_SOURCES})
target_include_directories(aiebu_zstd PUBLIC "${AIEBU_ZSTD_LIB_DIR}")
set_target_properties(aiebu_zstd PROPERTIES
  POSITION_INDEPENDENT_CODE ON
  C_VISIBILITY_PRESET hidden
)
if (MSVC)
  target_compile_options(aiebu_zstd PRIVATE /wd4267 /wd4244 /wd4146 /wd4245)
else()
  target_compile_options(aiebu_zstd PRIVATE -w)
endif()
# Disable the hand-written x86-64 assembly fast-path in zstd's HUF decompressor.
# The assembly symbols (huf_decompress_amd64.S) cannot be included in the glob
# without conflicting with C_VISIBILITY_PRESET hidden, which hides them at link
# time.  The C fallback path is correct and fully adequate for one-shot ELF
# decompression — the asm fast-path only matters for streaming/throughput workloads.
target_compile_definitions(aiebu_zstd PRIVATE ZSTD_DISABLE_ASM)

message("-- AIEBU_NATIVE_BUILD: ${AIEBU_NATIVE_BUILD}")
add_subdirectory(src/cpp)

add_subdirectory(specification)

if (AIEBU_CTRLCODE_CODEGEN)
  add_subdirectory(lib)
endif()

option(AIEBU_BUILD_TESTS "Build aiebu's own test suite" ON)
if (AIEBU_NATIVE_BUILD AND AIEBU_BUILD_TESTS)
  add_subdirectory(test)
endif()

include(cmake/cpack.cmake)
