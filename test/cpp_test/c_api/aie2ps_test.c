#include <stdio.h>
#include <stdlib.h>
#include "aiebu.h"

char* ReadFile(char *name, long *s)
{
  FILE *file;
  char *buffer;
  unsigned long fileLen;

  //Open file
  file = fopen(name, "rb");
  if (!file)
  {
    fprintf(stderr, "Unable to open file %s", name);
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
    fprintf(stderr, "Memory error!");
    fclose(file);
    return NULL;
  }

  //Read file contents into buffer
  fread(buffer, fileLen, 1, file);
  fclose(file);
  return buffer;
}

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
