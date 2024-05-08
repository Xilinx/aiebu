// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ELF_AIE2PS_ELF_WRITER_H_
#define _AIEBU_ELF_AIE2PS_ELF_WRITER_H_

#include <elfwriter.h>

namespace aiebu {

class aie2ps_elf_writer: public elf_writer
{
  constexpr static int ob_abi = 0x40;
  constexpr static int version = 0x01;
public:
  aie2ps_elf_writer(): elf_writer(ob_abi, version)
  { }
};

}
#endif //_AIEBU_ELF_AIE2PS_ELF_WRITER_H_
