/* SPDX-License-Identifier: Apache-2.0 */
/* Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved. */

#include <stdio.h>
#include <stdlib.h>
#include "aiebu.h"
#include "aie_test_common.h"

int main(int argc, char ** argv)
{
  char* v1;
  char* v2;
  char* v3;
  size_t vs1 = 0, vs2 = 0, ps = 0, vs3 = 0;
  struct aiebu_patch_info* patch_data;
  v1 = ReadFile(argv[1], (long *)&vs1);

  vs3 = aiebu_assembler_get_elf(aiebu_assembler_buffer_type_asm_aie2ps, v1, vs1, v2,
                                vs2, (void**)&v3, patch_data, ps);
  aiebu_assembler_free_elf(v3);
  printf("Size returned :%zd\n", vs3);
  return 0;
}
