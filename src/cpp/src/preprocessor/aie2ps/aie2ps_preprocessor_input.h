// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_INPUT_H_
#define _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_INPUT_H_

namespace aiebu {

class aie2ps_preprocessor_input : public preprocessor_input
{
  std::vector<char> m_data;
public:
  aie2ps_preprocessor_input() {}

  virtual void set_args(const std::vector<char>& buffer1,
                       const std::vector<symbol>& patch_data,
                       const std::vector<char>& buffer2)
  {
    set_data(buffer1);
  }

  void set_data(const std::vector<char> data)
  {
    m_data = std::move(data);
  }

  const std::vector<char>& get_data() const
  {
    return m_data;
  }
};

}
#endif //_AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_INPUT_H_
