// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include <map>
#include "assembler.h"
#include "aiebu_assembler.h"
#include "aiebu.h"
#include "aiebu_error.h"
#include "symbol.h"

namespace aiebu {

static
symbol::patch_schema
symbol_schema_transform(const aiebu::patch_schema schema)
{
  static std::map<patch_schema, symbol::patch_schema> convert_symbol_patch_schema = {
    {patch_schema::scaler_32,         symbol::patch_schema::scaler_32},
    {patch_schema::control_packet_48, symbol::patch_schema::control_packet_48},
    {patch_schema::shim_dma_57,       symbol::patch_schema::shim_dma_57},
    {patch_schema::shim_dma_48,       symbol::patch_schema::shim_dma_48},
  };

  auto it = convert_symbol_patch_schema.find(schema);
  if ( it == convert_symbol_patch_schema.end() )
    throw error(error::error_code::invalid_patch_schema, "Invalid patch schema !!!");

  return convert_symbol_patch_schema[schema];
}

aiebu_assembler::
aiebu_assembler(buffer_type type,
                const std::vector<char>& buffer,
                const std::vector<patch_info>& patch_data)
                : aiebu_assembler(type, buffer, {}, patch_data)
{ }

aiebu_assembler::
aiebu_assembler(buffer_type type,
                const std::vector<char>& buffer1,
                const std::vector<char>& buffer2,
                const std::vector<patch_info>& patch_data)
{
  std::vector<symbol> symbols;
  // in aie2ps patch data is not used, its extracted from asm
  if (type != buffer_type::asm_aie2ps)
  {
    for (auto s : patch_data)
      symbols.emplace_back(s.symbol, s.offset, 0, 0, s.addend,
                           s.buf_type == aiebu::patch_buffer_type::instruct ? ".ctrltext" : ".ctrldata",
                           symbol_schema_transform(s.schema));
  }

  if (type == buffer_type::blob_instr_dpu)
  {
    aiebu::assembler a(assembler::elf_type::aie2_dpu_blob);
    elf_data = a.process(buffer1, symbols, buffer2);
  }
  else if (type == buffer_type::blob_instr_transaction)
  {
    aiebu::assembler a(assembler::elf_type::aie2_transaction_blob);
    elf_data = a.process(buffer1, symbols, buffer2);
  }
  else if (type == buffer_type::asm_aie2ps)
  {
    aiebu::assembler a(assembler::elf_type::aie2ps_asm);
    elf_data = a.process(buffer1);
  }
  else
    throw error(error::error_code::invalid_buffer_type, "Buffer_type not supported !!!");
}

std::vector<char>
aiebu_assembler::
get_elf() const
{
  return elf_data;
}

static
std::vector<aiebu::patch_info>
convert_patchdata(const struct aiebu_patch_info* patch_data,
                  const size_t patch_data_size)
{
  std::map<aiebu_patch_buffer_type, patch_buffer_type> convert_patch_buffer_type = {
    { aiebu_patch_buffer_type_instruct, patch_buffer_type::instruct},
    { aiebu_patch_buffer_type_control_packet, patch_buffer_type::control_packet}
  };

  std::map<aiebu_patch_schema, patch_schema> convert_schema = {
    { aiebu_patch_schema_scaler_32,   patch_schema::scaler_32},
    { aiebu_patch_schema_shim_dma_48, patch_schema::shim_dma_48},
    { aiebu_patch_schema_shim_dma_57, patch_schema::shim_dma_57},
    { aiebu_patch_schema_control_packet_48, patch_schema::control_packet_48}
  };

  std::vector<aiebu::patch_info> vpatch;
  for (int i=0; i< patch_data_size; ++i)
  {
    aiebu::patch_info p;
    p.symbol = patch_data[i].symbol;

    auto it = convert_patch_buffer_type.find(patch_data[i].buf_type);
    if ( it == convert_patch_buffer_type.end())
      throw error(error::error_code::invalid_buffer_type, "Buffer_type not supported !!!");

    p.buf_type = convert_patch_buffer_type[patch_data[i].buf_type];

    auto it1 = convert_schema.find(patch_data[i].schema);
    if ( it1 == convert_schema.end())
      throw error(error::error_code::invalid_patch_schema, "Patch Schema not supported !!!");

    p.schema = convert_schema[patch_data[i].schema];
    p.offset = patch_data[i].offset;
    p.addend = patch_data[i].addend;

    vpatch.emplace_back(p);
  }

  return vpatch;
}
}

DRIVER_DLLESPEC
int
aiebu_assembler_get_elf(enum aiebu_assembler_buffer_type type,
                        const char* buffer1,
                        size_t buffer1_size,
                        const char* buffer2,
                        size_t buffer2_size,
                        void** elf_buf,
                        const struct aiebu_patch_info* patch_data,
                        size_t patch_data_size)
{
  int ret = 0;
  try
  {
    std::vector<char> v1, v2;
    std::vector<char> velf;
    std::vector<aiebu::patch_info> vpatch = aiebu::convert_patchdata(patch_data, patch_data_size);
    v1.assign(buffer1, buffer1+buffer1_size);
    v2.assign(buffer2, buffer2+buffer2_size);

    aiebu::aiebu_assembler handler((aiebu::aiebu_assembler::buffer_type)type, v1, v2, vpatch);
    velf = handler.get_elf();
    char *aelf = static_cast<char*>(std::malloc(sizeof(char)*velf.size()));
    std::copy(velf.begin(), velf.end(), aelf);
    *elf_buf = (void*)aelf;
    ret = velf.size();
  }
  catch (aiebu::error &ex)
  {
    std::cout << "ERROR: " <<  ex.what() << std::endl;
    ret = -(ex.get_code());
  }
  catch (std::exception &ex)
  {
    std::cout << "ERROR: " <<  ex.what() << std::endl;
    ret = -(static_cast<int>(aiebu::error::error_code::internal_error));
  }
  return ret;
}
