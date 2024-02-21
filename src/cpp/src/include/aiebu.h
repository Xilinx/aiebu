// SPDX-License-Identifier: Apache-2.0
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
  aiebu_assembler_buffer_type_blob_control_packet,
  aiebu_assembler_buffer_type_asm_aie2ps
};

enum aiebu_patch_buffer_type {
  aiebu_patch_buffer_type_instruct,
  aiebu_patch_buffer_type_control_packet
};

enum aiebu_patch_schema {
  aiebu_patch_schema_scaler_32,
  aiebu_patch_schema_shim_dma_48,
  aiebu_patch_schema_shim_dma_57,
  aiebu_patch_schema_control_packet_48
};

struct aiebu_patch_info {
  char symbol[128];
  enum aiebu_patch_buffer_type  buf_type;
  enum aiebu_patch_schema  schema;
  unsigned int* offsets;
  unsigned int offsets_size;
};

/*
 * This API takes buffer type, 2 buffers, their sizes and a array of symbols with their
 * patching information as input. it als allocate elf_buf and It fill elf content in it.
 * return, on success return return elf size, else posix error(negative).
 * User may pass any combination like
 * 1. type as aiebu_assembler_buffer_type_blob_instr_dpu, buffer1 as instruction buffer
 *    and buffer2 as control_packet: in this case it will package buffers in text and data
 *    section of elf respectively.
 * 2. type as aiebu_assembler_buffer_type_blob_instr_dpu, buffer1 as instruction buffer
 *    and buffer2 as null: in this case it will package buffer in text section.
 * 3. type as aiebu_assembler_buffer_type_asm_aie2ps, buffer1 as asm buffer and buffer2
 *    as null: in this case it will assemble the asm code and package in elf.
 *
 * @type                buffer type
 * @instr_buf           first buffer
 * @instr_buf_size      first buffer size
 * @control_buf         second buffer
 * @control_buf_size    second buffer size
 * @elf_buf             elf buffer
 * @patch_data          relocatable information.
 * @patch_data_size     patch data array size
 */
DRIVER_DLLESPEC
int
aiebu_assembler_get_elf(enum aiebu_assembler_buffer_type type,
                        const char* buffer1,
                        size_t buffer1_size,
                        const char* buffer2,
                        size_t buffer2_size,
                        void** elf_buf,
                        const struct aiebu_patch_info* patch_data,
                        size_t patch_data_size);
/*
 * This API is used to free the elf buffer.
 *
 * @mem                 buffer to free
 */
DRIVER_DLLESPEC
void
aiebu_assembler_free_elf(void* mem);

#ifdef __cplusplus
}
#endif

#endif
