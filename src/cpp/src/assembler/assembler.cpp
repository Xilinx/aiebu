// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include "assembler.h"
#include "aie2_blob_preprocessor.h"
#include "aie2_blob_encoder.h"
#include "aie2_blob_elfwriter.h"
#include "aie2ps_preprocessor.h"
#include "aie2ps_encoder.h"
#include "aie2ps_elfwriter.h"
#include "aiebu_error.h"

namespace aiebu {

assembler::
assembler(const elf_type type)
{

  if (type == elf_type::aie2_dpu_blob)  {
    m_preprocessor = std::make_unique<aie2_blob_preprocessor>();
    m_enoder = std::make_unique<aie2_blob_encoder>();
    m_elfwriter = std::make_unique<aie2_blob_elf_writer>();
    m_ppi = std::make_shared<aie2_blob_preprocessor_input>();

    // copy data in input buffer
    //ppi->set_instruction_buffer(buffer1);
    //ppi->set_controlcode_buffer(buffer2);
    //for (auto s: patch_data)
    //  ppi->add_symbol(s);
    //m_ppi = ppi;
  }
  else if (type == elf_type::aie2ps_asm)
  {
    m_preprocessor = std::make_unique<aie2ps_preprocessor>();
    m_enoder = std::make_unique<aie2ps_encoder>();
    m_elfwriter = std::make_unique<aie2ps_elf_writer>();
    m_ppi = std::make_shared<aie2ps_preprocessor_input>();

    // copy data in input buffer
    //ppi->set_data(buffer1);
    //m_ppi = ppi;
  }
  else
    throw error(error::error_code::invalid_buffer_type ,"Invalid elf type!!!");
}

std::vector<char>
assembler::
process(const std::vector<char>& buffer1,
        const std::vector<symbol>& patch_data,
        const std::vector<char>& buffer2)
{
  m_ppi->set_args(buffer1, patch_data, buffer2);
  auto ppo = m_preprocessor->process(m_ppi);
  auto w = m_enoder->process(ppo);
  auto u = m_elfwriter->process(w);
  return std::move(u);
}

}
