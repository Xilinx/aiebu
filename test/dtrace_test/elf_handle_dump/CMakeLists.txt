# SPDX-License-Identifier: MIT
# Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

# Test elf handle with debug format dump.

add_custom_command(OUTPUT test.elf test_partial.elf
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/test.b64" test.elf
  COMMAND ${CMAKE_COMMAND} -P "${AIEBU_SOURCE_DIR}/cmake/b64.cmake" -d "${CMAKE_CURRENT_SOURCE_DIR}/test_partial.b64" test_partial.elf
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test.b64"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test_partial.b64"
  COMMENT "Decoding base64 ELFs to binary in ${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM)

add_custom_target(dtrace_elf_handle_dump_elf ALL
  DEPENDS test.elf test_partial.elf)

# Test full elf with kernel:instance (explicit instance name)
add_test(NAME "dtrace_elf_handle_dump_with_kernel"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/elf_handle_dump"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_elf_with_kernel.dat"
    "python"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/test.elf" "CTRL:inst0"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_elf_handle_dump_with_kernel_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_elf_with_kernel.dat"
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/elf_handle_dump/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_elf_handle_dump_with_kernel_md5sum" PROPERTIES 
  DEPENDS "dtrace_elf_handle_dump_with_kernel")

# Test full elf with kernel only (no instance suffix) — single-instance kernel support
add_test(NAME "dtrace_elf_handle_dump_with_kernel_no_instance"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/elf_handle_dump"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_elf_with_kernel_no_instance.dat"
    "python"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/test.elf" "CTRL"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_elf_handle_dump_with_kernel_no_instance_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_elf_with_kernel_no_instance.dat"
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/elf_handle_dump/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_elf_handle_dump_with_kernel_no_instance_md5sum" PROPERTIES 
  DEPENDS "dtrace_elf_handle_dump_with_kernel_no_instance")

# Test partial elf without kernel
add_test(NAME "dtrace_elf_handle_dump_no_kernel"
  COMMAND ${DTRACE_TESTNAME}
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/elf_handle_dump"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_elf_no_kernel.dat"
    "python"
    "elf" "${CMAKE_CURRENT_BINARY_DIR}/test_partial.elf" "none"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_test(NAME "dtrace_elf_handle_dump_no_kernel_md5sum"
  COMMAND cmake -P "${AIEBU_SOURCE_DIR}/cmake/md5sum-compare.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/control_buffer_elf_no_kernel.dat"
    "${AIEBU_SOURCE_DIR}/test/dtrace_test/elf_handle_dump/gold.md5"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
set_tests_properties("dtrace_elf_handle_dump_no_kernel_md5sum" PROPERTIES 
  DEPENDS "dtrace_elf_handle_dump_no_kernel")
