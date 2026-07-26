# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.


if (MSVC AND (NOT (AIEBU_MSVC_LEGACY_LINKING)) AND (NOT ENABLE_ASAN))
  # Use Hybrid linking on Windows.
  # Explicit /MT only on the non-ASAN path. Under ASAN the CRT is dynamic (/MD),
  # driven by CMAKE_MSVC_RUNTIME_LIBRARY, so these test exes match the /MD
  # aiebu_static they link against. An explicit /MT here would be mapped into
  # <RuntimeLibrary> by the VS generator and force the test objects back to /MT,
  # mismatching the instrumented /MD aiebu_static (LNK2038/LNK1319).
  add_compile_options(/MT$<$<CONFIG:Debug>:d>  # static linking with the CRT
    )
endif()

add_subdirectory(cpp_api)

if (NOT MSVC)
  add_subdirectory(c_api)
endif()
