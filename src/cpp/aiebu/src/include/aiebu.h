// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_H_
#define _AIEBU_H_

#ifdef __cplusplus
extern "C" {
#include <stddef.h>
#endif

#if defined(_WIN32)
#define DRIVER_DLLESPEC __declspec(dllexport)
#else
#define DRIVER_DLLESPEC __attribute__((visibility("default")))
#endif

enum aiebu_error_code {
  aiebu_invalid_asm = 1,
  aiebu_invalid_patch_schema,
  aiebu_invalid_batch_buffer_type,
  aiebu_invalid_buffer_type,
  aiebu_invalid_offset,
  aiebu_invalid_internal_error
};

enum aiebu_assembler_buffer_type {
  aiebu_assembler_buffer_type_blob_instr_dpu,
  aiebu_assembler_buffer_type_blob_instr_prepost,
  aiebu_assembler_buffer_type_blob_instr_transaction,
  aiebu_assembler_buffer_type_blob_control_packet
};

/*
 * This API takes buffer type, 2 buffers, their sizes and external_buffer_id json
 * it also allocate elf_buf and It fill elf content in it.
 * return, on success return return elf size, else posix error(negative).
 * User may pass any combination like
 * 1. type as aiebu_assembler_buffer_type_blob_instr_transaction, buffer1 as instruction buffer
 *    and buffer2 as control_packet: in this case it will package buffers in text and data
 *    section of elf respectively.
 * 2. type as aiebu_assembler_buffer_type_blob_instr_transaction, buffer1 as instruction buffer
 *    and buffer2 as null: in this case it will package buffer in text section.
 *
 * @type                buffer type
 * @instr_buf           first buffer
 * @instr_buf_size      first buffer size
 * @control_buf         second buffer
 * @control_buf_size    second buffer size
 * @elf_buf             elf buffer
 * @patch_json          external_buffer_id_json buffer.
 * @patch_json_size     patch_json array size
 * @libs                libs to be included, ";" separated.
 * @libpaths            paths to search for libs, ";" separated.
 */
DRIVER_DLLESPEC
int
aiebu_assembler_get_elf(enum aiebu_assembler_buffer_type type,
                        const char* buffer1,
                        size_t buffer1_size,
                        const char* buffer2,
                        size_t buffer2_size,
                        void** elf_buf,
                        const char* patch_json,
                        size_t patch_json_size,
                        const char* libs,
                        const char* libpaths);

#ifdef __cplusplus
}
#endif

#endif
