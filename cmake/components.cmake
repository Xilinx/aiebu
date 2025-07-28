# SPDX-License-Identifier: MIT
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.
if (NOT WIN32)
  if (${LINUX_FLAVOR} MATCHES "^(ubuntu|debian)")
    set (AIEBU_DEV_COMPONENT_SUFFIX "-dev")
  elseif (${LINUX_FLAVOR} MATCHES "^(rhel|centos|fedora)")
    set (AIEBU_DEV_COMPONENT_SUFFIX "-devel")
  endif()
endif()

# NSIS packager cannot handle '-' in component names
if (WIN32)
  set (AIEBU_DEV_COMPONENT_SUFFIX "_dev")
endif()

if (NOT AIEBU_GIT_SUBMODULE)
  set (CMAKE_INSTALL_DEFAULT_COMPONENT_NAME "aiebu")
  set (AIEBU_COMPONENT "aiebu")
  set (AIEBU_DEV_COMPONENT "aiebu${AIEBU_DEV_COMPONENT_SUFFIX}")
endif()

