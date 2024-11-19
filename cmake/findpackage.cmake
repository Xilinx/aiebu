# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.
#
message("-- Preparing aiebu CMake find_package() support files")

# Provides write_basic_package_version_file
include(CMakePackageConfigHelpers)

string(TOLOWER ${PROJECT_NAME} LOWER_NAME)

# Generate aiebu-config.cmake
# For use by aiebu consumers (using cmake) to import aiebu libraries
configure_package_config_file (
  ${AIEBU_SOURCE_DIR}/cmake/config/aiebu.fp.in
  ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_NAME}-config.cmake
  INSTALL_DESTINATION ${AIEBU_INSTALL_DIR}/share/cmake/${PROJECT_NAME}
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
  DESTINATION ${AIEBU_INSTALL_DIR}/share/cmake/${PROJECT_NAME}
)

# Generate and install aiebu-targets.cmake
# This will generate a file that details all targets we have marked for export
# as part of the aiebu-targets export group
# It will provide information such as the library file names and locations post install
#install(
#  EXPORT aiebu-targets
#  NAMESPACE ${PROJECT_NAME}::
#  DESTINATION ${AIEBU_INSTALL_DIR}/share/cmake/${PROJECT_NAME}
#  )

install(TARGETS aiebu_static
        EXPORT aiebuTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(EXPORT aiebuTargets
        FILE aiebuTargets.cmake
        NAMESPACE AIEBU::
        DESTINATION ${AIEBU_INSTALL_DIR}/share/cmake/${PROJECT_NAME})