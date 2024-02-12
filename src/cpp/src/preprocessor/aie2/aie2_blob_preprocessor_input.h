// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSOR_INPUT_H_
#define _AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSOR_INPUT_H_

#include "symbol.h"

namespace aiebu {

class aie2_blob_preprocessor_input : public preprocessor_input
{

  std::vector<char> m_instruction_buffer;
  std::vector<char> m_controlcode_buffer;
  std::vector<symbol> m_sym;

public:
  aie2_blob_preprocessor_input() {}

  virtual void set_args(const std::vector<char>& buffer1,
                       const std::vector<symbol>& patch_data,
                       const std::vector<char>& buffer2)
  {
    set_instruction_buffer(buffer1);
    set_controlcode_buffer(buffer2);
    add_symbols(patch_data);
  }

  const std::vector<char>& get_instruction_buffer() const
  {
    return m_instruction_buffer;
  }

  const std::vector<char>& get_controlcode_buffer() const
  {
    return m_controlcode_buffer;
  }

  const std::vector<symbol>& get_symbols() const
  {
    return m_sym;
  }

  void set_instruction_buffer(const std::vector<char> buf)
  {
    m_instruction_buffer = std::move(buf);
  }

  void set_controlcode_buffer(const std::vector<char> buf)
  {
    m_controlcode_buffer = std::move(buf);
  }

  void add_symbol(const symbol buf)
  {
    m_sym.emplace_back(buf);
  }

  void add_symbols(const std::vector<symbol> syms)
  {
    m_sym = std::move(syms);
  }
};

}
#endif //_AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSOR_INPUT_H_
