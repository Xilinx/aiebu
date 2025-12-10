// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.
#include "assembler.h"
#include "elfwriter.h"
#include "encoder.h"
#include "preprocessor.h"
#include "preprocessor_input.h"
#include "reporter.h"
#include "symbol.h"
#include "utils.h"
#include "file_utils.h"
#include "transform_manager.h"
#include "logger.h"
#include "aiebu/aiebu.h"
#include "aiebu/aiebu_assembler.h"
#include "aiebu/aiebu_error.h"

#include <map>
#include <string>


namespace aiebu {

aiebu_assembler::
aiebu_assembler(buffer_type type,
                const std::vector<char>& buffer,
                const std::vector<std::string>& libs,
                const std::vector<std::string>& libpaths,
                const std::vector<char>& patch_json)
                : aiebu_assembler(type, buffer, {}, patch_json, libs, libpaths, {})
{ }

aiebu_assembler::
aiebu_assembler(buffer_type type,
                const std::vector<char>& buffer1,
                const std::vector<char>& buffer2,
                const std::vector<char>& patch_json,
                const std::vector<std::string>& libs,
                const std::vector<std::string>& libpaths,
                const std::map<uint32_t, std::vector<char> >& ctrlpkt) : m_type(type)
{
  if (type == buffer_type::blob_instr_dpu)
  {
    aiebu::assembler a(assembler::elf_type::aie2_dpu_blob);
    elf_data = a.process(buffer1, libs, libpaths, patch_json, buffer2);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie2;
  }
  else if (type == buffer_type::blob_instr_transaction)
  {
    aiebu::assembler a(assembler::elf_type::aie2_transaction_blob);
    elf_data = a.process(buffer1, libs, libpaths, patch_json, buffer2, ctrlpkt);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie2;
  }
  else if (type == buffer_type::asm_aie2)
  {
    aiebu::assembler a(assembler::elf_type::aie2_asm);
    elf_data = a.process(buffer1, libs, libpaths, patch_json, buffer2, ctrlpkt);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie2;
  }
  else if (type == buffer_type::asm_aie2ps)
  {
    aiebu::assembler a(assembler::elf_type::aie2ps_asm);
    elf_data = a.process(buffer1, libs, libpaths, patch_json);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie2ps;
  }
  else if (type == buffer_type::aie2_config)
  {
    aiebu::assembler a(assembler::elf_type::aie2_config);
    elf_data = a.process(buffer1, libs, libpaths, patch_json, buffer2);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie2_config;
  }
  else if (type == buffer_type::asm_aie4)
  {
    aiebu::assembler a(assembler::elf_type::aie4_asm);
    elf_data = a.process(buffer1, libs, libpaths, patch_json);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie4;
  }
  else if (type == buffer_type::aie2ps_config)
  {
    aiebu::assembler a(assembler::elf_type::aie2ps_config);
    elf_data = a.process(buffer1, libs, libpaths, patch_json, buffer2);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie2ps_config;
  }
  else if (type == buffer_type::aie4_config)
  {
    aiebu::assembler a(assembler::elf_type::aie4_config);
    elf_data = a.process(buffer1, libs, libpaths, patch_json, buffer2);
    m_output_type = aiebu::aiebu_assembler::buffer_type::elf_aie4_config;
  }
  else {
    throw error(error::error_code::invalid_buffer_type, "Buffer_type not supported !!!");
  }
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
  if (!stream) {
    throw error(error::error_code::invalid_input,
                "The given stream is not writable or has failed.");
  }
  reporter rep(m_output_type, elf_data);
  rep.elf_summary(stream);
  rep.ctrlcode_summary(stream);
}

void
aiebu_assembler::
disassemble(const std::filesystem::path &root) const
{
  reporter rep(m_output_type, elf_data);
  rep.disassemble(root, true);
}

class aiebu_assembler::argtbl_impl
{
  std::vector<arginfo>& m_tbl;

  public:
    explicit argtbl_impl(std::vector<arginfo>& in_tbl)
      : m_tbl(in_tbl) { }

    std::vector<arginfo>&
    dump() const
    {
      return m_tbl;
    }
};

aiebu_assembler::argtbl::
argtbl(std::shared_ptr<argtbl_impl> in_impl)
  : handle(std::move(in_impl))
{}

std::vector<arginfo>&
aiebu_assembler::argtbl::
dump() const
{
  return handle->dump();
}

aiebu_assembler::argtbl
aiebu_assembler::
get_argtbl()
{
  transform_manager trans(elf_data);
  arginfo_tbl = trans.extract_rela_sections();
  return argtbl{std::make_shared<argtbl_impl>(arginfo_tbl)};
}

void
aiebu_assembler::
flush_argtbl(const argtbl& arg_table)
{
  const auto& table = arg_table.dump();
  if (table.size() != arginfo_tbl.size())
    throw error(error::error_code::invalid_input,
                "Table size mismatch: got " + std::to_string(table.size())
                + ", expected " + std::to_string(arginfo_tbl.size()));

  transform_manager trans(elf_data);
  elf_data = trans.update_rela_sections(table);
}

aiebu_assembler::
aiebu_assembler(const std::string& elf_fnm)
{
  elf_data = readfile(elf_fnm);
}

aiebu_assembler::
aiebu_assembler(const std::vector<char>& buffer): elf_data(buffer) { }

}

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
                        const char* libpaths,
                        struct pm_ctrlpkt* pm_ctrlpkts,
                        size_t pm_ctrlpkt_size)
{
  int ret = 0;

  if (buffer1 == nullptr && buffer1_size != 0)
  {
    aiebu::log_error() << "Invalid buffer1 size";
    return -(static_cast<int>(aiebu::error::error_code::invalid_input));
  }

  if (buffer2 == nullptr && buffer2_size != 0)
  {
    aiebu::log_error() << "Invalid buffer2 size";
    return -(static_cast<int>(aiebu::error::error_code::invalid_input));
  }

  if (patch_json == nullptr && patch_json_size !=0)
  {
    aiebu::log_error() << "Invalid patch json size";
    return -(static_cast<int>(aiebu::error::error_code::invalid_input));
  }

  try
  {
    std::vector<char> v1, v2, v3;
    std::vector<char> velf;

    v1.assign(buffer1, buffer1+buffer1_size);
    v2.assign(buffer2, buffer2+buffer2_size);
    v3.assign(patch_json, patch_json+patch_json_size);
    std::map<uint32_t, std::vector<char> > mctrlpkt;

    std::vector<std::string> vlibs;
    if (libs)
      vlibs = aiebu::splitoption(libs);

    std::vector<std::string> vlibpaths;
    if (libpaths)
      vlibpaths = aiebu::splitoption(libpaths);

    for (auto i=0ul; i < pm_ctrlpkt_size; i++)
    {
      std::vector<char> v(pm_ctrlpkts[i].pm_buffer, pm_ctrlpkts[i].pm_buffer + pm_ctrlpkts[i].pm_buffer_size);
      mctrlpkt[pm_ctrlpkts[i].pm_id] = std::move(v);
    }

    aiebu::aiebu_assembler handler((aiebu::aiebu_assembler::buffer_type)type, v1, v2, v3, vlibs, vlibpaths, mctrlpkt);
    velf = handler.get_elf();
    char *aelf = static_cast<char*>(std::malloc(sizeof(char)*velf.size()));
    std::copy(velf.begin(), velf.end(), aelf);
    *elf_buf = (void*)aelf;
    ret =  static_cast<int>(velf.size());
  }
  catch (aiebu::error &ex)
  {
    aiebu::log_error() << ex.what();
    ret = -(ex.get_code());
  }
  catch (std::exception &ex)
  {
    aiebu::log_error() << ex.what();
    ret = -(static_cast<int>(aiebu::error::error_code::internal_error));
  }
  return ret;
}
