/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved. */

#ifndef _AIE_TEST_COMMON_H_
#define _AIE_TEST_COMMON_H_

#include <stdio.h>
#include <stdlib.h>

char* ReadFile(char *name, long *s)
{
  FILE *file;
  char *buffer;
  unsigned long fileLen;

  //Open file
  file = fopen(name, "rb");
  if (!file)
  {
    printf("Unable to open file %s", name);
    return NULL;
  }

  //Get file length
  fseek(file, 0, SEEK_END);
  fileLen=ftell(file);
  fseek(file, 0, SEEK_SET);

  //Allocate memory
  buffer=(char *)malloc(fileLen);
  *s = fileLen;
  if (!buffer)
  {
    printf("Memory error!");
    fclose(file);
    return NULL;
  }

  //Read file contents into buffer
  fread(buffer, fileLen, 1, file);
  fclose(file);
  return buffer;
}

#endif
