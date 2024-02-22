/* SPDX-License-Identifier: Apache-2.0 */
/* Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved. */

#include <stdio.h>
#include <stdlib.h>
#include "aiebu.h"
#include "aie_test_common.h"

int main(int argc, char **argv)
{
  char* v1;
  char* v2;
  char* v3;
  size_t vs1 = 0, vs2 = 0, ps = 0, vs3 = 0;
  struct aiebu_patch_info patch_data[3];
  patch_data[0].symbol[0] = 'a';
  patch_data[0].symbol[1] = 'b';
  patch_data[0].symbol[2] = 'c';
  patch_data[0].symbol[3] = '\0';

  patch_data[0].buf_type = aiebu_patch_buffer_type_instruct;
  patch_data[0].schema = aiebu_patch_schema_shim_dma_48;
  patch_data[0].offsets = (unsigned int*) malloc( sizeof(unsigned int)* 3);
  patch_data[0].offsets[0] = 0x100;
  patch_data[0].offsets[1] = 0x200;
  patch_data[0].offsets[2] = 0x300;
  patch_data[0].offsets_size = 3;

  patch_data[1].symbol[0] = 'd';
  patch_data[1].symbol[1] = 'f';
  patch_data[1].symbol[2] = 'g';
  patch_data[1].symbol[3] = '\0';
  patch_data[1].buf_type = aiebu_patch_buffer_type_instruct;
  patch_data[1].schema = aiebu_patch_schema_shim_dma_48;
  patch_data[1].offsets = (unsigned int*) malloc( sizeof(unsigned int)* 2);
  patch_data[1].offsets[0] = 0x2100;
  patch_data[1].offsets[1] = 0x2200;
  patch_data[1].offsets_size = 2;

  patch_data[2].symbol[0] = 'x';
  patch_data[2].symbol[1] = 'y';
  patch_data[2].symbol[2] = 'z';
  patch_data[2].symbol[3] = '\0';
  patch_data[2].buf_type = aiebu_patch_buffer_type_control_packet;
  patch_data[2].schema = aiebu_patch_schema_shim_dma_48;
  patch_data[2].offsets = (unsigned int*) malloc( sizeof(unsigned int)* 4);
  patch_data[2].offsets[0] = 0x3100;
  patch_data[2].offsets[1] = 0x3200;
  patch_data[2].offsets[2] = 0x3300;
  patch_data[2].offsets[3] = 0x3400;
  patch_data[2].offsets_size = 4;
  ps = 3;
  v1 = ReadFile(argv[1], (long *)&vs1);
  if ( argc > 2)
      v2 = ReadFile(argv[2], (long *)&vs2);

  vs3 = aiebu_assembler_get_elf(aiebu_assembler_buffer_type_blob_instr_dpu, v1, vs1, v2, vs2, (void**)&v3, patch_data, ps);
  aiebu_assembler_free_elf(v3);
  printf("Size returned :%zd\n", vs3);
  return 0;
}
