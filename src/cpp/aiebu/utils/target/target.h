// SPDX-License-Identifier: MIT
// Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

#ifndef __AIEBU_UTILITIES_TARGET_H_
#define __AIEBU_UTILITIES_TARGET_H_

#include <fstream>
#include <filesystem>

#include "aiebu_assembler.h"
#include "aiebu_error.h"

namespace aiebu::utilities {

class target;

using target_collection = std::vector<std::shared_ptr<target>>;

class target
{
  protected:
  const std::string m_executable;
  const std::string m_sub_target_name;
  const std::string m_description;

  inline bool file_exists(const std::string& name) const {
    return std::filesystem::exists(name);
  }

  inline void readfile(const std::string& filename, std::vector<char>& buffer)
  {
    if (!file_exists(filename))
      throw std::runtime_error("file:" + filename + " not found\n");

    std::ifstream input(filename, std::ios::in | std::ios::binary);
    auto file_size = std::filesystem::file_size(filename);
    buffer.resize(file_size);
    input.read(buffer.data(), file_size);
  }

  inline void write_elf(const aiebu::aiebu_assembler& as, const std::string& outfile)
  {
    auto e = as.get_elf();
    std::cout << "elf size:" << e.size() << "\n";
    std::ofstream output_file(outfile, std::ios_base::binary);
    output_file.write(e.data(), e.size());
  }

  public:
  using sub_cmd_options = std::vector<std::string>;
  virtual void assemble(const sub_cmd_options &_options) = 0;
  const std::string &get_name() const { return m_sub_target_name; }
  const std::string &get_nescription() const { return m_description; }

  target(const std::string& exename, const std::string& name, const std::string& description)
    : m_executable(exename),
      m_sub_target_name(name),
      m_description(description)
  {}
  virtual ~target() = default;

};

#ifdef AIEBU_FULL
class target_aie2ps: public target
{
  public:
  virtual void assemble(const sub_cmd_options &_options);

  target_aie2ps(const std::string& name): target(name, "aie2ps", "aie2ps asm assembler") {}
};

#endif

class target_aie2blob: public target
{
  protected:
  std::string m_transaction_file;
  std::string m_controlpkt_file;
  std::string m_external_buffers_file;
  std::vector<char> m_transaction_buffer;
  std::vector<char> m_control_packet_buffer;
  std::vector<char> m_patch_data_buffer;
  std::vector<std::string> m_libs;
  std::vector<std::string> m_libpaths;
  std::map<uint8_t, std::vector<char> > m_ctrlpkt;
  std::string m_output_elffile;
  bool m_print_report = false;
  target_aie2blob(const std::string& exename, const std::string& name, const std::string& description)
    : target(exename, name, description) {}
  bool parseOption(const sub_cmd_options &_options);

  std::map<uint8_t, std::vector<char> >
  parse_pmctrlpkt(std::vector<std::string> pm_key_value_pairs);
};

class target_aie2blob_transaction: public target_aie2blob
{
  public:
  target_aie2blob_transaction(const std::string& exename, const std::string& name = "aie2txn",
                              const std::string& description = "aie2 txn blob assembler")
    : target_aie2blob(exename, name, description) {}
  virtual void assemble(const sub_cmd_options &_options);
};

class target_aie2: public target_aie2blob_transaction
{
  public:
  virtual void assemble(const sub_cmd_options &_options);
  target_aie2(const std::string& exename): target_aie2blob_transaction(exename, "aie2asm", "aie2 asm assembler") {}
};

class target_aie2blob_dpu: public target_aie2blob
{
  public:
  target_aie2blob_dpu(const std::string& exename)
    : target_aie2blob(exename, "aie2dpu", "aie2 dpu blob assembler") {}
  virtual void assemble(const sub_cmd_options &_options);
};

} //namespace aiebu::utilities

#endif //__AIEBU_UTILITIES_TARGET_H_
