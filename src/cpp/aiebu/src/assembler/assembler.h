// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_COMMOM_ASSEMBLER_H_
#define _AIEBU_COMMOM_ASSEMBLER_H_

#include <memory>
#include <vector>

#include "symbol.h"

namespace aiebu {

class preprocessor;
class encoder;
class elf_writer;
class preprocessor_input;

class assembler
{
  std::unique_ptr<preprocessor> m_preprocessor;
  std::unique_ptr<encoder> m_enoder;
  std::unique_ptr<elf_writer> m_elfwriter;
  std::shared_ptr<preprocessor_input> m_ppi;
public:
  enum class elf_type
  {
    aie2_transaction_blob,
    aie2_dpu_blob
  };

  explicit assembler(const elf_type type);

  std::vector<char> process(const std::vector<char>& buffer1,
                            const std::vector<std::string>& libs = {},
                            const std::vector<std::string>& libpaths = {},
                            const std::vector<char>& patch_json = {},
                            const std::vector<char>& buffer2 = {});

};

}

#endif //_AIEBU_COMMOM_ASSEMBLER_H_
