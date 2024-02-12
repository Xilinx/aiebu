// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ELF_ELF_WRITER_H_
#define _AIEBU_ELF_ELF_WRITER_H_

#include <sstream>
#include <iterator>
#include "writer.h"
#include "symbol.h"
#include "elfio/elfio.hpp"
using namespace ELFIO;

namespace aiebu {

constexpr int text_align = 16;
constexpr int data_align = 16;
constexpr int phdr_align = 8;
constexpr int program_header_static_count = 2;
constexpr int program_header_dynamic_count = 3;

class elf_section
{
  std::string m_name;
  std::vector<uint8_t> m_buffer;
  int m_type;
  int m_flags;
  int m_version;
  uint64_t m_size;
  uint64_t m_offset;
  uint64_t m_align;
  int m_info;
  std::string m_link;
public:
  HEADER_ACCESS_GET_SET(std::string, name);
  HEADER_ACCESS_GET_SET(int, type);
  HEADER_ACCESS_GET_SET(int, version);
  HEADER_ACCESS_GET_SET(int, flags);
  HEADER_ACCESS_GET_SET(int, info);
  HEADER_ACCESS_GET_SET(uint64_t, size);
  HEADER_ACCESS_GET_SET(uint64_t, offset);
  HEADER_ACCESS_GET_SET(uint64_t, align);
  HEADER_ACCESS_GET_SET(std::vector<uint8_t>,  buffer);
  HEADER_ACCESS_GET_SET(std::string, link);
  
};

class elf_segment
{
  int m_type;
  int m_flags;
  uint64_t m_vaddr = 0x0;
  uint64_t m_paddr = 0x0;
  int m_filesz;
  int m_memsz;
  uint64_t m_align;
  std::string m_link;
public:
  HEADER_ACCESS_GET_SET(int, type);
  HEADER_ACCESS_GET_SET(int, flags);
  HEADER_ACCESS_GET_SET(uint64_t, vaddr);
  HEADER_ACCESS_GET_SET(uint64_t, paddr);
  HEADER_ACCESS_GET_SET(int, filesz);
  HEADER_ACCESS_GET_SET(int, memsz);
  HEADER_ACCESS_GET_SET(uint64_t, align);
  HEADER_ACCESS_GET_SET(std::string, link);
};

class elf_writer
{
protected:
  elfio m_elfio;

  section* add_section(elf_section data);
  segment* add_segment(elf_segment data);
  string_section_accessor add_dynstr_section();
  void add_dynsym_section(string_section_accessor* stra, std::shared_ptr<writer> mwriter);
  void add_reldyn_section(std::shared_ptr<writer> mwriter);
  void add_dynamic_section_segment();
  std::vector<char> finalize();
  void add_text_data_section(std::shared_ptr<writer> mwriter);

  virtual std::string get_dataname(uint32_t colnum, uint32_t pagenum) = 0;
  virtual std::string get_textname(uint32_t colnum, uint32_t pagenum) = 0;
public:

  elf_writer(int abi, int version)
  {
    m_elfio.create(ELFCLASS32, ELFDATA2LSB);
    m_elfio.set_os_abi(abi);
    m_elfio.set_abi_version(version);
    m_elfio.set_type( ET_EXEC );
    m_elfio.set_machine( EM_M32 );
    m_elfio.set_flags(0x0);


    segment* seg = m_elfio.segments.add();
    seg->set_type( PT_PHDR );
    seg->set_virtual_address( 0x0 );
    seg->set_physical_address( 0x0 );
    seg->set_flags( PF_R );
    seg->set_file_size(0x0);
    seg->set_memory_size(0x0);
  }

  std::vector<char> process(std::shared_ptr<writer> mwriter);

};

}
#endif //_AIEBU_ELF_ELF_WRITER_H_
