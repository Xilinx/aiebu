// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_COMMOM_SYMBOL_H_
#define _AIEBU_COMMOM_SYMBOL_H_

#include "utils.h"
#include <elfio/elfio.hpp>

namespace aiebu {

// rela->addend have addend info along with schema
// [0:2] bit are used for schema, [3:31] used for addend
constexpr uint32_t ADDEND_SHIFT = 3;
constexpr uint32_t ADDEND_MASK = 0xF8;
constexpr uint32_t SCHEMA_MASK = 0x07;

class symbol
{
public:

  enum class patch_schema : uint8_t
  {
    uc_dma_remote_ptr_symbol = 1,
    shim_dma_57 = 2,
    scaler_32 = 3,
    control_packet_48 = 4,
    shim_dma_48 = 5,
    unknown = 8,
  };

private:
  std::string m_name;
  patch_schema m_schema;
  offset_type m_pos;
  uint32_t m_colnum;
  uint32_t m_pagenum;
  uint32_t m_addend;
  std::string m_section_name;
  ELFIO::Elf_Word m_index;

public:

  symbol(const std::string& name, uint32_t pos, uint32_t colnum, uint32_t pagenum, uint32_t addend,
         const std::string& section_name, patch_schema schema=patch_schema::unknown)
         :m_name(name), m_schema(schema), m_pos(pos), m_colnum(colnum),
          m_pagenum(pagenum), m_addend(addend), m_section_name(section_name)  { }

  symbol(const symbol *s)
  {
    m_name = s->m_name;
    m_schema = s->m_schema;
    m_pos = s->m_pos;
    m_colnum = s->m_colnum;
    m_pagenum = s->m_pagenum;
    m_addend = s->m_addend;
    m_section_name = s->m_section_name;
  }

  HEADER_ACCESS_GET(std::string&, name);
  HEADER_ACCESS_GET_SET(patch_schema, schema);
  HEADER_ACCESS_GET_SET(offset_type, pos);
  HEADER_ACCESS_GET_SET(uint32_t, addend);
  HEADER_ACCESS_GET_SET(std::string, section_name);
  HEADER_ACCESS_GET_SET(ELFIO::Elf_Word, index);
};
}
#endif //_AIEBU_COMMOM_SYMBOL_H_
