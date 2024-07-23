// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_COMMON_WRITER_H_
#define _AIEBU_COMMON_WRITER_H_

#include <map>
#include <string>
#include <vector>
#include "symbol.h"
#include "code_section.h"

namespace aiebu {

// Class to hold sections name, data, symbols, type
class writer
{

  const std::string m_name;
  const code_section m_type;
  std::vector<uint8_t> m_data;
  std::vector<symbol> m_symbols;

public:
  writer(const std::string name, code_section type, std::vector<uint8_t>& data): m_name(name), m_type(type), m_data(std::move(data)) {}
  writer(const std::string name, code_section type): m_name(name), m_type(type) {}

  virtual void write_byte(uint8_t byte);

  virtual void write_word(uint32_t word);

  virtual offset_type tell() const;

  const std::vector<uint8_t>&
  get_data()
  {
    return m_data;
  }

  const std::string&
  get_name() const
  {
    return m_name;
  }

  code_section
  get_type() const
  {
    return m_type;
  }

  void set_data(std::vector<uint8_t> &data)
  {
    m_data = std::move(data);
  }

  const std::vector<symbol>&
  get_symbols() const
  {
    return m_symbols;
  }

  void add_symbol(symbol sym)
  {
    m_symbols.emplace_back(sym);
  }

  void add_symbols(std::vector<symbol>& syms)
  {
    m_symbols = std::move(syms);
  }

  bool hassymbols() const
  {
    return m_symbols.size();
  }

  void padding(offset_type size);
};
}
#endif //_AIEBU_COMMON_WRITER_H_
