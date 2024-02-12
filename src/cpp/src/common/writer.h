// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_COMMON_WRITER_H_
#define _AIEBU_COMMON_WRITER_H_

#include <map>
#include <vector>
#include "symbol.h"
#include "code_section.h"

namespace aiebu {

class writer
{
  using buffermap_type = typename std::unordered_map<uint32_t, std::vector<std::pair<std::vector<uint8_t>,std::vector<uint8_t>>>>;
  buffermap_type m_buffermap;
  std::vector<symbol> m_symbols;

public:
  writer() {}

  virtual void write_byte(uint8_t byte, code_section sec, uint32_t colnum=0, pageid_type pagenum=0);

  virtual void write_word(uint32_t word, code_section sec, uint32_t colnum=0, pageid_type pagenum=0);

  virtual offset_type tell(code_section sec, uint32_t colnum=0, uint32_t pagenum=0);

  buffermap_type&
  get_buffermap()
  {
    return m_buffermap;
  }

  void add_buffermap(uint32_t col, std::pair<std::vector<uint8_t>,
                     std::vector<uint8_t>> p)
  {
    m_buffermap[col].emplace_back(p);
  }

  std::vector<symbol>& get_symbols()
  {
    return m_symbols;
  }

  void add_symbol(symbol sym)
  {
    m_symbols.emplace_back(sym);
  }

  void add_symbols(const std::vector<symbol> syms)
  {
    m_symbols = std::move(syms);
  }

  bool hassymbols()
  {
    return m_symbols.size();
  }

  void padding(uint32_t col, pageid_type pagenum, offset_type size);
};

}
#endif //_AIEBU_COMMON_WRITER_H_
