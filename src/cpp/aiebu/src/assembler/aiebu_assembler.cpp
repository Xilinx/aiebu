// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include <map>
#include <string>
#include "assembler.h"
#include "aiebu_assembler.h"
#include "aiebu.h"
#include "aiebu_error.h"
#include "symbol.h"
#include "utils.h"
#include "preprocessor.h"
#include "encoder.h"
#include "elfwriter.h"
#include "preprocessor_input.h"

#include "reporter.h"

namespace aiebu {

aiebu_assembler::
aiebu_assembler(buffer_type type,
                const std::vector<char>& buffer,
                const std::vector<std::string>& libs,
                const std::vector<std::string>& libpaths,
                const std::vector<char>& patch_json)
                : aiebu_assembler(type, buffer, {}, patch_json, libs, libpaths)
{ }

aiebu_assembler::
aiebu_assembler(buffer_type type,
                const std::vector<char>& buffer1,
                const std::vector<char>& buffer2,
                const std::vector<char>& patch_json,
                const std::vector<std::string>& libs,
                const std::vector<std::string>& libpaths) : _type(type)
{
  if (type == buffer_type::blob_instr_dpu)
  {
    aiebu::assembler a(assembler::elf_type::aie2_dpu_blob);
    elf_data = a.process(buffer1, libs, libpaths, patch_json, buffer2);
  }
  else if (type == buffer_type::blob_instr_transaction)
  {
    aiebu::assembler a(assembler::elf_type::aie2_transaction_blob);
    elf_data = a.process(buffer1, libs, libpaths, patch_json, buffer2);
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


void
aiebu_assembler::
get_report(std::ostream &stream) const
{
    reporter rep(_type, elf_data);
    rep.elf_summary(stream);
    rep.ctrlcode_summary(stream);
    rep.ctrlcode_detail_summary(stream);
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
                        const char* patch_json,
                        size_t patch_json_size,
                        const char* libs,
                        const char* libpaths)
{
  int ret = 0;
  if (buffer2 == NULL && buffer2_size != 0)
  {
    std::cout << "ERROR: Invalid buffer2 size" << std::endl;
    return -(static_cast<int>(aiebu::error::error_code::internal_error));
  }

  if (patch_json == NULL && patch_json_size !=0)
  {
    std::cout << "ERROR: Invalid patch json size" << std::endl;
    return -(static_cast<int>(aiebu::error::error_code::internal_error));
  }

  try
  {
    std::vector<char> v1, v2, v3;
    std::vector<char> velf;

    v1.assign(buffer1, buffer1+buffer1_size);
    v2.assign(buffer2, buffer2+buffer2_size);
    v3.assign(patch_json, patch_json+patch_json_size);

    std::vector<std::string> vlibs;
    if (libs)
      vlibs = aiebu::splitoption(libs);

    std::vector<std::string> vlibpaths;
    if (libpaths)
      vlibpaths = aiebu::splitoption(libpaths);

    aiebu::aiebu_assembler handler((aiebu::aiebu_assembler::buffer_type)type, v1, v2, v3, vlibs, vlibpaths);
    velf = handler.get_elf();
    char *aelf = static_cast<char*>(std::malloc(sizeof(char)*velf.size()));
    std::copy(velf.begin(), velf.end(), aelf);
    *elf_buf = (void*)aelf;
    ret = static_cast<int>(velf.size());
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
