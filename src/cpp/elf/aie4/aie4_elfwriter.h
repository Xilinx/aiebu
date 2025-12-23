// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_ELF_AIE4_ELF_WRITER_H_
#define AIEBU_ELF_AIE4_ELF_WRITER_H_

#include <elfwriter.h>

namespace aiebu {

// OS/ABI values for AIE architectures
constexpr unsigned char OSABI_AIE2PS = 0x46;  // 70
constexpr unsigned char OSABI_AIE4   = 0x47;  // 71
constexpr unsigned char OSABI_AIE4A  = 0x48;  // 72
constexpr unsigned char OSABI_AIEZ   = 0x49;  // 73

class aie4_elf_writer: public elf_writer
{
  constexpr static unsigned char version = 0x02;
public:
  aie4_elf_writer(): elf_writer(OSABI_AIE4, version)
  { }
};

class aie4_config_elf_writer: public elf_writer
{
  constexpr static unsigned char version = 0x03;
public:
  aie4_config_elf_writer(): elf_writer(OSABI_AIE4, version)
  { }
};

class aie4a_elf_writer: public elf_writer
{
  constexpr static unsigned char version = 0x02;
public:
  aie4a_elf_writer(): elf_writer(OSABI_AIE4A, version)
  { }
};

class aie4a_config_elf_writer: public elf_writer
{
  constexpr static unsigned char version = 0x03;
public:
  aie4a_config_elf_writer(): elf_writer(OSABI_AIE4A, version)
  { }
};

class aiez_elf_writer: public elf_writer
{
  constexpr static unsigned char version = 0x02;
public:
  aiez_elf_writer(): elf_writer(OSABI_AIEZ, version)
  { }
};

class aiez_config_elf_writer: public elf_writer
{
  constexpr static unsigned char version = 0x03;
public:
  aiez_config_elf_writer(): elf_writer(OSABI_AIEZ, version)
  { }
};

}
#endif //AIEBU_ELF_AIE4_ELF_WRITER_H_
