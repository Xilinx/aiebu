// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ELF_ELF_WRITER_H_
#define _AIEBU_ELF_ELF_WRITER_H_

#include <sstream>
#include <iterator>
#include <mutex>
#include "writer.h"
#include "symbol.h"
#include "elfio/elfio.hpp"
#include "uid_md5.h"

namespace aiebu {

constexpr int align = 16;
constexpr int text_align = 16;
constexpr int data_align = 16;
constexpr int phdr_align = 8;
constexpr int program_header_static_count = 2;
constexpr int program_header_dynamic_count = 3;
constexpr int SHT_CUSTOM_SECTION = ELFIO::SHT_LOUSER + 1;
// elfio support max 0xff00 section, beyond that extended section support is needed
// which elfio dont support.
constexpr ELFIO::Elf_Word max_sections = 65279; //65280 - 1;

constexpr ELFIO::Elf_Word NT_XRT_UID = 4;
constexpr ELFIO::Elf_Word NT_XRT_UUID       = 5;
constexpr ELFIO::Elf_Word NT_XRT_PARTITION_SIZE = 6;

class elf_section
{
  std::string m_name;
  std::vector<uint8_t> m_buffer;
  int m_type;
  int m_flags;
  int m_version;
  uint64_t m_align;
  int m_info;
  std::string m_link;
  uint64_t m_addr = 0x0;
public:
  HEADER_ACCESS_GET_SET(std::string, name);
  HEADER_ACCESS_GET_SET(int, type);
  HEADER_ACCESS_GET_SET(int, version);
  HEADER_ACCESS_GET_SET(int, flags);
  HEADER_ACCESS_GET_SET(int, info);
  HEADER_ACCESS_GET_SET(uint64_t, align);
  HEADER_ACCESS_GET_SET(std::vector<uint8_t>,  buffer);
  HEADER_ACCESS_GET_SET(std::string, link);
  HEADER_ACCESS_GET_SET(uint64_t, addr);

};

class elf_segment
{
  int m_type;
  int m_flags;
  uint64_t m_vaddr = 0x0;
  uint64_t m_paddr = 0x0;
  int m_filesz = 0;
  int m_memsz = 0;
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
protected:  // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
  ELFIO::elfio m_elfio;
  ELFIO::section* str_sec = nullptr;
  ELFIO::section* sym_sec = nullptr;

  ELFIO::section* dstr_sec = nullptr;
  ELFIO::section* dsym_sec = nullptr;
  ELFIO::section* rel_sec = nullptr;
  ELFIO::section* dynamic_sec = nullptr;
  std::once_flag dynamic_flag;
  uid_md5 m_uid;
  uint64_t prev_virtual_addr = 0;
  uint64_t cur_addr = 0;
  uint64_t prev_seg_size = 0;

  ELFIO::section* add_section(const elf_section& data);
  ELFIO::segment* add_segment(const elf_segment& data);
  ELFIO::string_section_accessor add_dynstr_section();
  void add_dynsym_section(ELFIO::string_section_accessor* stra, std::vector<symbol>& syms, const std::string& index_string);
  void add_reldyn_section(std::vector<symbol>& syms);
  void add_dynamic_section();
  std::vector<char> finalize();
  std::vector<uint32_t> add_text_data_section(const std::vector<std::shared_ptr<writer>>& mwriter, std::vector<symbol>& syms, const std::string& index_string);
  void add_note(ELFIO::Elf_Word type, const std::string& name, const std::vector<char>& dec);
  ELFIO::Elf_Word add_symtab(const std::string& name);
  ELFIO::Elf_Word add_symtab_section(const std::string& name, ELFIO::Elf_Word index);
  void init_symtab();
  void init_dynamic_sections();
  std::vector<uint32_t> process_common_helper(const std::vector<std::shared_ptr<writer>>& mwriter, const std::string& index_string);
  void process_global_custom_sections_if_any(const std::vector<std::shared_ptr<writer>>& global_sections);
  std::string get_group_name(uint32_t index) {return ".group."+ std::to_string(index); }
  std::string get_section_prefix(uint32_t index) {return "."+ std::to_string(index); }
  void add_group(const std::string& name, const std::vector<uint32_t>& member, ELFIO::Elf_Word info_index);
  uint64_t get_virtual_addr(uint64_t in_prev_virtual_addr, uint64_t in_prev_seg_size);
  uint64_t align_address(uint64_t address);
  // Helper function to return ELFIO::section* and take section name as input
  ELFIO::section* add_section_by_name(const std::string& section_name) {
    if (m_elfio.sections.size() > max_sections)
      throw error(error::error_code::invalid_asm, "Maximum number of sections reached");

    return m_elfio.sections.add(section_name);
  }

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
    //seg->set_type( ELFIO::PT_LOAD );
    seg->set_virtual_address( 0x0 );
    seg->set_physical_address( 0x0 );
    seg->set_flags( ELFIO::PF_R );
    //seg->set_flags( ELFIO::PF_R | ELFIO::PF_X);
    seg->set_file_size(0x0);
    seg->set_memory_size(0x0);

  }

  // Methods to update OS ABI and version after construction (for .target directive support)
  void set_os_abi(unsigned char abi) { m_elfio.set_os_abi(abi); }
  void set_abi_version(unsigned char version) { m_elfio.set_abi_version(version); }
  unsigned char get_os_abi() const { return m_elfio.get_os_abi(); }
  unsigned char get_abi_version() const { return m_elfio.get_abi_version(); }

  virtual std::vector<char> process(std::vector<std::shared_ptr<writer>>& mwriter);

  virtual ~elf_writer() = default;

};

}
#endif //_AIEBU_ELF_ELF_WRITER_H_
