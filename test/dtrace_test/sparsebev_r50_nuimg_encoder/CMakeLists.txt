# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.

# Test sparsebev r50 nuimg encoder script
add_custom_command(OUTPUT sparsebev_r50_nuimg_encoder.elf
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/sparsebev_r50_nuimg_encoder.b64" sparsebev_r50_nuimg_encoder.elf
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/sparsebev_r50_nuimg_encoder.b64"
  COMMENT "Decoding base64 ELF to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
  )

add_custom_target(dtrace_sparsebev_r50_nuimg_encoder_elf ALL
  DEPENDS sparsebev_r50_nuimg_encoder.elf
  )

add_test(NAME "dtrace_sparsebev_r50_nuimg_encoder"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/sparsebev_r50_nuimg_encoder"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_sparsebev_r50_nuimg_encoder.dat"
    "json"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/sparsebev_r50_nuimg_encoder.elf" "DPU:test"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_sparsebev_r50_nuimg_encoder_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_sparsebev_r50_nuimg_encoder.dat"
  "${AIEBU_SOURCE_DIR}/test/dtrace_test/sparsebev_r50_nuimg_encoder/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_sparsebev_r50_nuimg_encoder_md5sum" PROPERTIES DEPENDS "dtrace_sparsebev_r50_nuimg_encoder")

add_test(NAME "dtrace_sparsebev_r50_nuimg_encoder_result_md5sum"
  COMMAND ${CMAKE_COMMAND} -E compare_files --ignore-eol
  "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_sparsebev_r50_nuimg_encoder.json"
  "${CMAKE_CURRENT_SOURCE_DIR}/gold_result.json"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_sparsebev_r50_nuimg_encoder_result_md5sum" PROPERTIES DEPENDS "dtrace_sparsebev_r50_nuimg_encoder")
