# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.

# Test camo facedetection (compute_io_bound core tile counters) script
add_custom_command(OUTPUT camo_facedetection.elf
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/camo_facedetection.b64" camo_facedetection.elf
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/camo_facedetection.b64"
  COMMENT "Decoding base64 ELF to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
  )

add_custom_target(dtrace_camo_facedetection_elf ALL
  DEPENDS camo_facedetection.elf
  )

add_test(NAME "dtrace_camo_facedetection"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/camo_facedetection"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_camo_facedetection.dat"
    "json"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/camo_facedetection.elf" "facedetection_1_3_0_fp32_vaiml_par_0"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_camo_facedetection_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_camo_facedetection.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/camo_facedetection/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_camo_facedetection_md5sum" PROPERTIES DEPENDS "dtrace_camo_facedetection")

add_test(NAME "dtrace_camo_facedetection_result_md5sum"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_camo_facedetection.json"
  "${CMAKE_CURRENT_SOURCE_DIR}/gold_result.json"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_camo_facedetection_result_md5sum" PROPERTIES DEPENDS "dtrace_camo_facedetection")
