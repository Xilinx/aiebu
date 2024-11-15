// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_PREPROCESSOR_INPUT_H_
#define _AIEBU_PREPROCESSOR_PREPROCESSOR_INPUT_H_

#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "symbol.h"
#include "aiebu_error.h"

namespace aiebu {

class preprocessor_input
{
protected:
  std::map<std::string, std::vector<char>> m_data;
  std::vector<symbol> m_sym;
public:
  preprocessor_input() {}
  virtual ~preprocessor_input() = default;

  virtual void set_args(const std::vector<char>&,
                        const std::vector<char>& patch_json,
                        const std::vector<char>&,
                        const std::vector<std::string>&,
                        const std::vector<std::string>&,
                        const std::map<uint8_t, std::vector<char> >& ctrlpkt) = 0;

  const std::vector<std::string> get_keys()
  {
    std::vector<std::string> keys(m_data.size());
    auto key_selector = [](auto pair){return pair.first;};
    transform(m_data.begin(), m_data.end(), keys.begin(), key_selector);
    return keys;
  }

  virtual std::vector<char>& get_data(std::string& key)
  {
    auto it = m_data.find(key);
    if (it == m_data.end())
      throw error(error::error_code::internal_error, "Key (" + key  + ") not found!!!");
    return m_data[key];
  }

  std::vector<symbol>& get_symbols()
  {
    return m_sym;
  }

  void add_symbol(const symbol buf)
  {
    m_sym.emplace_back(buf);
  }

  void add_symbols(const std::vector<symbol>& syms)
  {
    m_sym.insert( m_sym.end(), syms.begin(), syms.end());
  }
};

}
#endif //_AIEBU_PREPROCESSOR_PREPROCESSOR_INPUT_H_
