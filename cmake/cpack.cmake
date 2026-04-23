# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.
set(PACKAGE_KIND "TGZ")
set(CPACK_GENERATOR "TGZ")
message("-- ${CMAKE_BUILD_TYPE} ${PACKAGE_KIND} package")

set(CPACK_PACKAGE_VENDOR "Advanced Micro Devices Inc.")
set(CPACK_PACKAGE_CONTACT "runtimeca39d@amd.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "AMD XDNA binutils package")
set(CPACK_RESOURCE_FILE_LICENSE "${AIEBU_SOURCE_DIR}/LICENSE")

# AIEBU by default installs only static library for client
# linking and only if requested.  The variable checked here is
# default ON, but is set to OFF by upstream builds where static
# libraries are not allowed.  This option doesn't scale too
# well; if aiebu-targets is used differently this conditional
# include must be revisted
if(AIEBU_INSTALL_STATIC_LIBRARY)
  include(cmake/findpackage.cmake)
endif()

include(CPack)
