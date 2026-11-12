# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

# Test yolox latency script
add_custom_command(OUTPUT anduril_yolox.elf
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/anduril_yolox.b64" anduril_yolox.elf
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/anduril_yolox.b64"
  COMMENT "Decoding base64 ELF to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
  )

add_custom_target(dtrace_anduril_yolox_elf ALL
  DEPENDS anduril_yolox.elf
  )

add_test(NAME "dtrace_anduril_yolox"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/anduril_yolox"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_anduril_yolox.dat"
    "python"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/anduril_yolox.elf" "DPU:test"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_anduril_yolox_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_anduril_yolox.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/anduril_yolox/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_anduril_yolox_md5sum" PROPERTIES DEPENDS "dtrace_anduril_yolox")

add_test(NAME "dtrace_anduril_yolox_result_md5sum"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_anduril_yolox.py"
  "${CMAKE_CURRENT_SOURCE_DIR}/gold_result.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_anduril_yolox_result_md5sum" PROPERTIES DEPENDS "dtrace_anduril_yolox")
