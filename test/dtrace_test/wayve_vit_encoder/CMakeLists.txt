# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

# Test wayve vit encoder script
add_custom_command(OUTPUT wayve_vit_encoder.elf
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/wayve_vit_encoder.b64" wayve_vit_encoder.elf
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/wayve_vit_encoder.b64"
  COMMENT "Decoding base64 ELF to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
  )

add_custom_target(dtrace_wayve_vit_encoder_elf ALL
  DEPENDS wayve_vit_encoder.elf
  )

add_test(NAME "dtrace_wayve_vit_encoder"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/wayve_vit_encoder"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_wayve_vit_encoder.dat"
    "python"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/wayve_vit_encoder.elf" "DPU:test"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_wayve_vit_encoder_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_wayve_vit_encoder.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/wayve_vit_encoder/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_wayve_vit_encoder_md5sum" PROPERTIES DEPENDS "dtrace_wayve_vit_encoder")

add_test(NAME "dtrace_wayve_vit_encoder_result_md5sum"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_wayve_vit_encoder.py"
  "${CMAKE_CURRENT_SOURCE_DIR}/gold_result.py"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_wayve_vit_encoder_result_md5sum" PROPERTIES DEPENDS "dtrace_wayve_vit_encoder")
