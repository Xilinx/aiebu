// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSED_OUTPUT_H_
#define _AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSED_OUTPUT_H_

#include "symbol.h"

namespace aiebu {

class aie2_blob_preprocessed_output : public preprocessed_output
{

  std::vector<uint8_t> m_instruction_buffer;
  std::vector<uint8_t> m_controlcode_buffer;
  std::vector<symbol> m_sym;

public:
  aie2_blob_preprocessed_output() {}
  const std::vector<uint8_t>& get_instruction_buffer() const
  {
    return m_instruction_buffer;
  }

  const std::vector<uint8_t>& get_controlcode_buffer() const
  {
    return m_controlcode_buffer;
  }

  const std::vector<symbol>& get_symbols() const
  {
    return m_sym;
  }

  void set_instruction_buffer(const std::vector<uint8_t> buf)
  {
    m_instruction_buffer = std::move(buf);
  }

  void set_controlcode_buffer(const std::vector<uint8_t> buf)
  {
    m_controlcode_buffer = std::move(buf);
  }

  void add_symbol(const symbol buf)
  {
    m_sym.emplace_back(buf);
  }
};

}
#endif //_AIEBU_PREPROCESSOR_AIE2_BLOB_PREPROCESSED_OUTPUT_H_
