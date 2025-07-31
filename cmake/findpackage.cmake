# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.
#
message("-- Preparing aiebu CMake find_package() support files")

if (CMAKE_VERSION VERSION_LESS "3.20.0")
  # cmake_path(...) function was introduced in 3.20.0
  message(WARNING "CMake version is less than 3.20.0, find_package is disabled")
  return()
endif()

# Provides write_basic_package_version_file
include(CMakePackageConfigHelpers)

string(TOLOWER ${PROJECT_NAME} LOWER_NAME)

# Must normalize CMAKE install dir in order for _IMPORT_PREFIX
# to be computed correctly.  CMake counts "./" as one level so
# must remove
cmake_path(SET AIEBU_INSTALL_CMAKE_DIR NORMALIZE "${AIEBU_INSTALL_CMAKE_DIR}")

# Generate aiebu-config.cmake
# For use by aiebu consumers (using cmake) to import aiebu libraries
configure_package_config_file (
  ${AIEBU_SOURCE_DIR}/cmake/config/aiebu.fp.in
  ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_NAME}-config.cmake
  INSTALL_DESTINATION ${AIEBU_INSTALL_CMAKE_DIR}
)

# Generate aiebu-config-version.cmake
# Consumers my require a particular version
# This enables version checking
write_basic_package_version_file (
  ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_NAME}-config-version.cmake
  VERSION ${AIEBU_VERSION_STRING}
  COMPATIBILITY AnyNewerVersion
)

# Install aiebu-config.cmake and aiebu-config-version.cmake
install (
  FILES ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_NAME}-config.cmake ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_NAME}-config-version.cmake
  DESTINATION ${AIEBU_INSTALL_CMAKE_DIR}
  COMPONENT ${AIEBU_DEV_COMPONENT}
)

# Generate and install aiebu-targets.cmake
# This will generate a file that details all targets we have marked for export
# as part of the aiebu-targets export group
# It will provide information such as the library file names and locations post install
install(
  EXPORT aiebu-targets
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${AIEBU_INSTALL_CMAKE_DIR}
  COMPONENT ${AIEBU_DEV_COMPONENT}
  )
