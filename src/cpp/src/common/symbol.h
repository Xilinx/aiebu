// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_COMMOM_SYMBOL_H_
#define _AIEBU_COMMOM_SYMBOL_H_

#include "utils.h"
#include <elfio/elfio.hpp>

namespace aiebu {

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
    unknown = 6,
  };

private:
  std::string m_name;
  patch_schema m_schema;
  offset_type m_pos;
  uint32_t m_colnum;
  uint32_t m_pagenum;
  uint32_t m_section_index;
  ELFIO::Elf_Word m_index;

public:

  symbol(std::string name, uint32_t pos, uint32_t colnum, uint32_t pagenum,
         uint32_t section_index, patch_schema schema=patch_schema::unknown)
         :m_name(name), m_schema(schema), m_pos(pos), m_colnum(colnum),
          m_pagenum(pagenum), m_section_index(section_index)  { }

  symbol(const symbol *s)
  {
    m_name = s->m_name;
    m_schema = s->m_schema;
    m_pos = s->m_pos;
    m_colnum = s->m_colnum;
    m_pagenum = s->m_pagenum;
    m_section_index = s->m_section_index;
  }

  HEADER_ACCESS_GET_SET(std::string, name);
  HEADER_ACCESS_GET_SET(patch_schema, schema);
  HEADER_ACCESS_GET_SET(offset_type, pos);
  HEADER_ACCESS_GET_SET(uint32_t, colnum);
  HEADER_ACCESS_GET_SET(pageid_type, pagenum);
  HEADER_ACCESS_GET_SET(uint32_t, section_index);
  HEADER_ACCESS_GET_SET(ELFIO::Elf_Word, index);
};
}
#endif //_AIEBU_COMMOM_SYMBOL_H_
