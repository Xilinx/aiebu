// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSED_OUTPUT_H_
#define _AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSED_OUTPUT_H_

#include <string>
#include "symbol.h"
#include "preprocessed_output.h"

namespace aiebu {

class aie2_blob_preprocessed_output : public preprocessed_output
{

  std::unordered_map<std::string, std::vector<uint8_t>> m_data;
  std::vector<symbol> m_sym;

public:
  aie2_blob_preprocessed_output() {}

  std::vector<symbol>& get_symbols()
  {
    return m_sym;
  }

  void add_data(const std::string& name, const std::vector<uint8_t>& buf)
  {
    m_data[name] = buf;
  }

  void add_symbol(const symbol buf)
  {
    m_sym.emplace_back(buf);
  }

  void add_symbols(std::vector<symbol>& syms)
  {
    m_sym = std::move(syms);
  }

  const std::vector<std::string> get_keys()
  {
    std::vector<std::string> keys(m_data.size());
    auto key_selector = [](auto pair){return pair.first;};
    transform(m_data.begin(), m_data.end(), keys.begin(), key_selector);
    return keys;
  }

  std::vector<uint8_t>& get_data(std::string& key)
  {
    auto it = m_data.find(key);
    if (it == m_data.end())
      throw error(error::error_code::internal_error, "Key (" + key + ") not found!!!");
    return m_data[key];
  }
};

}
#endif //_AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSED_OUTPUT_H_
