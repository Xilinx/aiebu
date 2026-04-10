# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

add_library(dtrace_library_objects OBJECT
  action/action_control.cpp
  action/break.cpp
  action/mask_write_register.cpp
  action/operation.cpp
  action/print.cpp
  action/printa.cpp
  action/profile.cpp
  action/read_mem.cpp
  action/read_register.cpp
  action/timestamp.cpp
  action/timestamp32.cpp
  action/timestamps.cpp
  action/timestamps32.cpp
  action/write_mem.cpp
  action/write_register.cpp
  action/write_handshake.cpp
  action/read_handshake.cpp
  action/host_timestamp.cpp
  action/host_timestamps.cpp
  action/sleep.cpp
  action/count.cpp
  probe/begin.cpp
  probe/end.cpp
  probe/jprobe.cpp
  probe/probe_control.cpp
  probe/profile.cpp
  probe/tracepoint.cpp
  parser/parser.cpp
  pager/pager.cpp
  control/control.cpp
  dtrace.cpp
  )

target_include_directories(dtrace_library_objects 
  PRIVATE
  ${AIEBU_SOURCE_DIR}/src/cpp
  ${Boost_INCLUDE_DIRS}
  )

set_target_properties(dtrace_library_objects PROPERTIES
  CXX_VISIBILITY_PRESET hidden
  POSITION_INDEPENDENT_CODE ON
  )

add_library(cert_dtrace SHARED
  $<TARGET_OBJECTS:dtrace_library_objects>
  )

add_library(cert_dtrace_static STATIC
  $<TARGET_OBJECTS:dtrace_library_objects>
  )

if (NOT WIN32)
  # On linux use cert_dtrace.a.  The target name is cert_dtrace_static but the
  # output name is cert_dtrace.
  set_target_properties(cert_dtrace_static PROPERTIES OUTPUT_NAME "cert_dtrace")
endif()

# As long as we support static linking of targets that depend on static cert,
# the cert static library must be released.
install(TARGETS cert_dtrace_static
  EXPORT aiebu-targets
  ARCHIVE DESTINATION ${AIEBU_INSTALL_LIB_DIR} COMPONENT ${AIEBU_DEV_COMPONENT}
  )
