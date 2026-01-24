# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

add_library(aiebu_transform_objects OBJECT
  transform.cpp
  )

# Remove globle includes in case of submodule XRT as there are conflicts
# in version.h and utils.h
set_target_properties(aiebu_transform_objects PROPERTIES
  INCLUDE_DIRECTORIES ""  # Clear inherited dirs
  )

target_include_directories(aiebu_transform_objects
  PRIVATE
  ${AIEBU_ELFIO_SRC_DIR}
  ${AIEBU_SOURCE_DIR}/src/cpp/include
  ${AIEBU_SOURCE_DIR}/src/cpp/cxxopts/include
  ${AIEBU_SOURCE_DIR}/src/cpp
  ${Boost_INCLUDE_DIRS}
  )

add_executable(aiebu-transform $<TARGET_OBJECTS:aiebu_transform_objects>)
target_link_libraries(aiebu-transform PRIVATE aiebu_static)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_link_libraries(aiebu-transform PRIVATE pthread)
  # Upstreaming does not allow statically linked executables.  AIEBU_UPSTREAM is
  # set by projects that are built for upstreaming and includes AIEBU as a
  # submodule
  if (NOT AIEBU_UPSTREAM)
    target_link_options(aiebu-transform PRIVATE "-static")
    set_target_properties(aiebu-transform PROPERTIES INSTALL_RPATH "" BUILD_RPATH "")

    # Create a dynamically linked executable. aiebu-transform-dyn, on Linux for
    # running valgrind, etc. This binary is not released for deployment but only
    # used for internal testing. Note that valgrind known to report many glibc
    # related errors when running with fully statically linked executables,
    # hence the dynamically linked binary for testing
    add_executable(aiebu-transform-dyn $<TARGET_OBJECTS:aiebu_transform_objects>)
    target_link_libraries(aiebu-transform-dyn PRIVATE aiebu_static)
  endif()
endif()

# This custom target fails if aiebu-transform has any dynamic dependencies
if (NOT AIEBU_UPSTREAM)
  add_custom_target(tranform_check_dynamic_deps ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Checking for dynamic dependencies ..."
    COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/depends.cmake" $<TARGET_FILE:aiebu-transform> aiebu-transform_depends.txt
    DEPENDS aiebu-transform
    )
endif()

install(TARGETS aiebu-transform
  RUNTIME DESTINATION ${AIEBU_INSTALL_BIN_DIR}
  CONFIGURATIONS Debug Release
  COMPONENT ${AIEBU_COMPONENT}
  )
