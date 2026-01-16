# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

add_custom_command(OUTPUT ctrlpkt0.bin external_buffer_id.json config.json test.asm aie4_pdi.asm aie4_ctrlpkt.asm
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/ctrlpkt0.b64" ctrlpkt0.bin
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/config.json" "${CMAKE_CURRENT_BINARY_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/test.asm" "${CMAKE_CURRENT_BINARY_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/aie4_pdi.asm" "${CMAKE_CURRENT_BINARY_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/aie4_ctrlpkt.asm" "${CMAKE_CURRENT_BINARY_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/external_buffer_id.json" "${CMAKE_CURRENT_BINARY_DIR}"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ctrlpkt0.b64"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/config.json"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test.asm"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/aie4_pdi.asm"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/aie4_ctrlpkt.asm"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/external_buffer_id.json"
  COMMENT "Decoding base64 ctrlcode and control packets to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM)

add_custom_target(aie4_ctrlpkt ALL
  DEPENDS ctrlpkt0.bin external_buffer_id.json config.json test.asm aie4_pdi.asm aie4_ctrlpkt.asm)

add_test(NAME "aie4_ctrlpkt_elf"
  COMMAND aiebu-asm -t aie4_config -j "${CMAKE_CURRENT_BINARY_DIR}/config.json" -o aie4_ctrlpkt.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie4_ctrlpkt_elf_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/aie4_ctrlpkt.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set_tests_properties("aie4_ctrlpkt_elf_md5sum" PROPERTIES DEPENDS "aie4_ctrlpkt_elf")
