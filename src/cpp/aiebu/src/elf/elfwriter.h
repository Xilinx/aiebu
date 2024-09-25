// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ELF_ELF_WRITER_H_
#define _AIEBU_ELF_ELF_WRITER_H_

#include <sstream>
#include <iterator>
#include "writer.h"
#include "symbol.h"
#include "uid_md5.h"

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4245)
#include "elfio/elfio.hpp"
#pragma warning(pop)
#endif

namespace aiebu {

constexpr int align = 16;
constexpr int text_align = 16;
constexpr int data_align = 16;
constexpr int phdr_align = 8;
constexpr int program_header_static_count = 2;
constexpr int program_header_dynamic_count = 3;

constexpr ELFIO::Elf_Word NT_XRT_UID = 4;

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
  ELFIO::elfio m_elfio;
  uid_md5 m_uid;

  ELFIO::section* add_section(elf_section data);
  ELFIO::segment* add_segment(elf_segment data);
  ELFIO::string_section_accessor add_dynstr_section();
  void add_dynsym_section(ELFIO::string_section_accessor* stra, std::vector<symbol>& syms);
  void add_reldyn_section(std::vector<symbol>& syms);
  void add_dynamic_section_segment();
  std::vector<char> finalize();
  void add_text_data_section(std::vector<writer>& mwriter, std::vector<symbol>& syms);
  void add_note(ELFIO::Elf_Word type, std::string name, std::string dec);

public:

  elf_writer(unsigned char abi, unsigned char version)
  {
    m_elfio.create(ELFIO::ELFCLASS32, ELFIO::ELFDATA2LSB);
    m_elfio.set_os_abi(abi);
    m_elfio.set_abi_version(version);
    m_elfio.set_type( ELFIO::ET_EXEC );
    m_elfio.set_machine( ELFIO::EM_M32 );
    m_elfio.set_flags(0x0);


    ELFIO::segment* seg = m_elfio.segments.add();
    seg->set_type( ELFIO::PT_PHDR );
    seg->set_virtual_address( 0x0 );
    seg->set_physical_address( 0x0 );
    seg->set_flags( ELFIO::PF_R );
    seg->set_file_size(0x0);
    seg->set_memory_size(0x0);

  }

  std::vector<char> process(std::vector<writer> mwriter);

  virtual ~elf_writer() = default;

};

}
#endif //_AIEBU_ELF_ELF_WRITER_H_
