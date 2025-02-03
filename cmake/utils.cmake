# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc.

# aieby_add_subdirectory_diable_install_target subdir
#
# This function disables the install target for a subdirectory prior
# to calling add_subdirectory.  This is a work-around for a missing
# cmake feature.  While CMAKE_SKIP_INSTALL_RULES is key and prevents
# CMake from from creating subdir/cmake_install.cmake, it
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
