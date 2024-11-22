/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved. */

#include <stdio.h>
#include <stdlib.h>
#include "aiebu.h"
#include "aie_test_common.h"

void usage_exit()
{
  printf("Usage: aie2ps_c.out <control_code.asm>\n");
  exit(1);
}

int main(int argc, char ** argv)
{
  if (argc != 2)
    usage_exit();

  char* control_code_buf;
  char* elf_buf;
  size_t control_code_buf_size, elf_buf_size;
  control_code_buf = aiebu_ReadFile(argv[1], &control_code_buf_size);

  elf_buf_size = aiebu_assembler_get_elf(aiebu_assembler_buffer_type_asm_aie2ps,
                                         control_code_buf, control_code_buf_size,
                                         NULL, 0, (void**)&elf_buf, NULL, 0, "", "", NULL, 0);
  if (elf_buf_size > 0)
  {
    free((void*)elf_buf);
    printf("Size returned :%zd\n", elf_buf_size);
  }
  return 0;
}
