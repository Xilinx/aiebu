// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include "assembler.h"
#include "aie2_blob_preprocessor.h"
#include "aie2_blob_encoder.h"
#include "aie2_blob_elfwriter.h"
#include "aiebu_error.h"

#include "preprocessor.h"
#include "encoder.h"
#include "elfwriter.h"
#include "preprocessor_input.h"

namespace aiebu {

assembler::
assembler(const elf_type type)
{

  if (type == elf_type::aie2_dpu_blob)  {
    m_preprocessor = std::make_unique<aie2_blob_preprocessor>();
    m_enoder = std::make_unique<aie2_blob_encoder>();
    m_elfwriter = std::make_unique<aie2_blob_elf_writer>();
    m_ppi = std::make_shared<aie2_blob_dpu_preprocessor_input>();
  }
  else if (type == elf_type::aie2_transaction_blob)  {
    m_preprocessor = std::make_unique<aie2_blob_preprocessor>();
    m_enoder = std::make_unique<aie2_blob_encoder>();
    m_elfwriter = std::make_unique<aie2_blob_elf_writer>();
    m_ppi = std::make_shared<aie2_blob_transaction_preprocessor_input>();
  }
  else
    throw error(error::error_code::invalid_buffer_type ,"Invalid elf type!!!");
}

std::vector<char>
assembler::
process(const std::vector<char>& buffer1,
        const std::vector<std::string>& libs,
        const std::vector<std::string>& libpaths,
        const std::vector<char>& patch_json,
        const std::vector<char>& buffer2)
{
  m_ppi->set_args(buffer1, patch_json, buffer2, libs, libpaths);
  auto ppo = m_preprocessor->process(m_ppi);
  auto w = m_enoder->process(ppo);
  auto u = m_elfwriter->process(w);
  return u;
}

}
