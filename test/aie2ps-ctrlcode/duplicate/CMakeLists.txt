# SPDX-License-Identifier: MIT
# Copyright (C) 2026 Advanced Micro Devices, Inc.

# Assemble a ctrlcode with mulitple inclusions of the same ASM file to ELF
add_test(NAME "duplicate_asm"
  COMMAND aiebu-asm -t aie2ps_config -o aie_control.elf -c "${CMAKE_CURRENT_SOURCE_DIR}/ml_asm/merged_control.asm" -L "${CMAKE_CURRENT_SOURCE_DIR}/ml_asm" -L "${CMAKE_CURRENT_SOURCE_DIR}/asm" -j "${CMAKE_CURRENT_SOURCE_DIR}/external_buffer_id.json" -f disabledump)
  set_tests_properties("duplicate_asm" PROPERTIES LABELS "benchmark")
