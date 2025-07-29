# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

add_library(aiebu_dump_objects OBJECT
  dump.cpp
  )

# Remove globle includes in case of submodule XRT as there are conflicts
# in version.h and utils.h
set_target_properties(aiebu_dump_objects PROPERTIES
  INCLUDE_DIRECTORIES ""  # Clear inherited dirs
  )

target_include_directories(aiebu_dump_objects
  PRIVATE
  ${AIEBU_SOURCE_DIR}/src/cpp/include
  ${AIEBU_SOURCE_DIR}/src/cpp/cxxopts/include
  ${AIEBU_SOURCE_DIR}/src/cpp
  ${AIEBU_SOURCE_DIR}/src/cpp/ELFIO
  ${Boost_INCLUDE_DIRS}
  )

add_executable(aiebu-dump $<TARGET_OBJECTS:aiebu_dump_objects>)
target_link_libraries(aiebu-dump PRIVATE aiebu_static)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  target_link_libraries(aiebu-dump PRIVATE pthread)
  target_link_options(aiebu-dump PRIVATE "-static")
  set_target_properties(aiebu-dump PROPERTIES INSTALL_RPATH "" BUILD_RPATH "")

  # Create a dynamically linked executable. aiebu-dump-dyn, on Linux for running
  # valgrind, etc. This binary is not released for deployment but only used for
  # internal testing. Note that valgrind known to report many glibc related
  # errors when running with fully statically linked executables, hence the
  # dynamically linked binary for testing
  add_executable(aiebu-dump-dyn $<TARGET_OBJECTS:aiebu_dump_objects>)
  target_link_libraries(aiebu-dump-dyn PRIVATE aiebu_static)
endif()

# This custom target fails if aiebu-dump has any dynamic dependencies
add_custom_target(dump_check_dynamic_deps ALL
  COMMAND ${CMAKE_COMMAND} -E echo "Checking for dynamic dependencies ..."
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/depends.cmake" $<TARGET_FILE:aiebu-dump> aiebu-dump_depends.txt
  DEPENDS aiebu-dump
  )

install(TARGETS aiebu-dump
  RUNTIME DESTINATION ${AIEBU_INSTALL_BIN_DIR}
  CONFIGURATIONS Debug Release
  COMPONENT ${AIEBU_COMPONENT}
  )
