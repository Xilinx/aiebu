// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ELF_AIE2_BLOB_ELF_WRITER_H_
#define _AIEBU_ELF_AIE2_BLOB_ELF_WRITER_H_

#include <elfwriter.h>

namespace aiebu {

class aie2_blob_elf_writer: public elf_writer
{
  constexpr static unsigned char ob_abi = 0x45;
  constexpr static unsigned char version = 0x01;
public:
  aie2_blob_elf_writer(): elf_writer(ob_abi, version)
  { }
};

}
#endif //_AIEBU_ELF_AIE2_BLOB_ELF_WRITER_H_
