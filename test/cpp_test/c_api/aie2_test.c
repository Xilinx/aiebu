/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved. */

#include <stdio.h>
#include <stdlib.h>
#include "aiebu.h"
#include "aie_test_common.h"

void usage_exit()
{
  printf("Usage: aie2_c.out <txn.bin> <control_packet.bin> <external_buffer_id.json>\n");
  exit(1);
}

int main(int argc, char **argv)
{
  if (argc != 2 && argc != 4)
    usage_exit();

  char* txn_buf = NULL;
  char* control_packet_buf = NULL;
  char* external_buffer_id_json_buf = NULL;
  char* elf_buf = NULL;

  size_t txn_buf_size, control_packet_buf_size = 0, external_buffer_id_json_buf_size = 0, elf_buf_size;

  txn_buf = aiebu_ReadFile(argv[1], (long *)&txn_buf_size);
  if (argc > 2)
  {
    control_packet_buf = aiebu_ReadFile(argv[2], (long *)&control_packet_buf_size);
    external_buffer_id_json_buf = aiebu_ReadFile(argv[3], (long *)&external_buffer_id_json_buf_size);
  }

  elf_buf_size = aiebu_assembler_get_elf(aiebu_assembler_buffer_type_blob_instr_transaction,
                                txn_buf, txn_buf_size,
                                control_packet_buf, control_packet_buf_size,
                                (void**)&elf_buf,
                                external_buffer_id_json_buf, external_buffer_id_json_buf_size,
                                "", "");
  if (elf_buf_size > 0)
  {
    free((void*)elf_buf);
    printf("Size returned :%zd\n", elf_buf_size);
  }
  return 0;
}
