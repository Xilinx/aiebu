# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc.

# xrt_add_subdirectory_diable_install_target subdir
#
# This function disables the install target for a subdirectory prior
# to calling add_subdirectory.  This is a work-around for a missing
# cmake feature.  While CMAKE_SKIP_INSTALL_RULES is key and prevents
# the CMake from from creating subdir/cmake_install.cmake, it
# unfortunately doesn't prevent CMake from still wanting to include
# subdir/cmake_install.cmake.  This function just creates an empty
# subdir/cmake_install.cmake file.
function(aiebu_add_subdirectory_disable_install_target subdir)
  set(CMAKE_SKIP_INSTALL_RULES TRUE)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${subdir})
  file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/${subdir}/cmake_install.cmake)
  add_subdirectory(${ARGV})
  set(CMAKE_SKIP_INSTALL_RULES FALSE)
endfunction()

if (${AIEBU_AIE_RT_BIN_DIR} STREQUAL ${AIEBU_BINARY_DIR})
  message("-- Enabling build of aie-rt as submodule of aiebu")
  set(XAIENGINE_BUILD_SHARED OFF CACHE BOOL "Force static build of xaiengine library" FORCE)
  if (MSVC)
#    message("-- Suppressing warnings in aie-rt")
#    add_compile_options(/WX- /D_CRT_SECURE_NO_WARNINGS)
     add_compile_options($<$<CONFIG:Debug>:/MTd>
                         $<$<CONFIG:Release>:/MT>)
  endif()
  if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_compile_options("-Wno-error=unused-parameter")
  endif()
  aiebu_add_subdirectory_disable_install_target(aie-rt/driver)
endif()

add_subdirectory(src)
