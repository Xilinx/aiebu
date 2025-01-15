# SPDX-License-Identifier: MIT
# Copyright (C) 2024-2025 Advanced Micro Devices, Inc.

add_custom_command(OUTPUT ml_txn.bin ctrl_pkt0.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/ml_txn.b64" ml_txn.bin
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/ctrl_pkt0.b64" ctrl_pkt0.bin
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ml_txn.b64"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ctrl_pkt0.b64"
  COMMENT "Decoding base64 ctrlcode and ctrlpkt to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM)

add_custom_target(aie2basicbins ALL
  DEPENDS ml_txn.bin ctrl_pkt0.bin)


add_test(NAME "aie2_basic"
  COMMAND aiebu-asm -r -t aie2txn -c ml_txn.bin -p ctrl_pkt0.bin -j "${CMAKE_CURRENT_SOURCE_DIR}/external_buffer_id.json" -o basic.elf)

add_test(NAME "aie2-basic1"
  COMMAND aiebu-asm -r -t aie2asm -c "${CMAKE_CURRENT_SOURCE_DIR}/ml_txn.ctrltext.asm" -p ctrl_pkt0.bin -j "${CMAKE_CURRENT_SOURCE_DIR}/external_buffer_id.json" -o ctrl-packet1.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2-basic2"
  COMMAND aiebu-asm -r -t aie2asm -c "ctrl-packet1.ctrltext.asm" -p ctrl_pkt0.bin -j "${CMAKE_CURRENT_SOURCE_DIR}/external_buffer_id.json" -o ctrl-packet2.elf
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2-basic_compare"
  COMMAND ${CMAKE_COMMAND} -E compare_files ctrl-packet1.ctrltext.asm ctrl-packet2.ctrltext.asm
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME "aie2_basic_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake" "${CMAKE_CURRENT_BINARY_DIR}/basic.elf" "${CMAKE_CURRENT_SOURCE_DIR}/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
