/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved. */

#ifndef _AIE_TEST_COMMON_H_
#define _AIE_TEST_COMMON_H_

#ifdef _WIN32
//The _CRT_XXX does not work :-(
//Instead use the explicit pragma to disable the windows compiler warnings
//about insecure fopen, etc.
//#define _CRT_SECURE_NO_WARNINGS
//#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

#include <stdio.h>
#include <stdlib.h>

char* aiebu_ReadFile(char *name, long *s)
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
  size_t count = fread(buffer, fileLen, 1, file);
  if (count != fileLen) {
    free(buffer);
    buffer = NULL;
  }
  fclose(file);
  return buffer;
}
#ifdef _WIN32
#pragma warning(pop)
#endif

#endif
