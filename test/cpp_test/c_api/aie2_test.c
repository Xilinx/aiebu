/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved. */

#include <stdio.h>
#include <stdlib.h>
#include "aiebu.h"
#include "aie_test_common.h"

int main(int argc, char **argv)
{
  char* v1 = NULL;
  char* v2 = NULL;
  char* v3 = NULL;
  size_t vs1 = 0, vs2 = 0, ps = 0, vs3 = 0;
  struct aiebu_patch_info patch_data[3];
  patch_data[0].symbol[0] = 'i';
  patch_data[0].symbol[1] = 'f';
  patch_data[0].symbol[2] = 'm';
  patch_data[0].symbol[3] = '\0';

  patch_data[0].buf_type = aiebu_patch_buffer_type_instruct;
  patch_data[0].schema = aiebu_patch_schema_shim_dma_48;
  patch_data[0].offset = 0x100;

  patch_data[1].symbol[0] = 'd';
  patch_data[1].symbol[1] = 'f';
  patch_data[1].symbol[2] = 'g';
  patch_data[1].symbol[3] = '\0';
  patch_data[1].buf_type = aiebu_patch_buffer_type_instruct;
  patch_data[1].schema = aiebu_patch_schema_shim_dma_48;
  patch_data[1].offset = 0x2100;

  patch_data[2].symbol[0] = 'x';
  patch_data[2].symbol[1] = 'y';
  patch_data[2].symbol[2] = 'z';
  patch_data[2].symbol[3] = '\0';
  patch_data[2].buf_type = aiebu_patch_buffer_type_control_packet;
  patch_data[2].schema = aiebu_patch_schema_shim_dma_48;
  patch_data[2].offset = 0x3100;
  ps = 3;
  v1 = ReadFile(argv[1], (long *)&vs1);
  if ( argc > 2)
      v2 = ReadFile(argv[2], (long *)&vs2);

  vs3 = aiebu_assembler_get_elf(aiebu_assembler_buffer_type_blob_instr_dpu, v1, vs1, v2, vs2, (void**)&v3,
                                patch_data, ps, "", "");
  free((void*)vs3);
  printf("Size returned :%zd\n", vs3);
  return 0;
}
