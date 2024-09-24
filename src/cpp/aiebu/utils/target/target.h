// SPDX-License-Identifier: MIT
// Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

#ifndef __AIEBU_UTILITIES_TARGET_H_
#define __AIEBU_UTILITIES_TARGET_H_

#include <fstream>
#include <filesystem>
#include <boost/property_tree/ptree.hpp>

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

class target_aie2blob: public target
{
protected:
  std::vector<char> m_transaction_buffer;
  std::vector<char> m_control_packet_buffer;
  std::vector<char> m_patch_data_buffer;
  std::vector<std::string> m_libs;
  std::vector<std::string> m_libpaths;
  std::string m_output_elffile;
  bool m_print_report;
  target_aie2blob(const std::string& exename, const std::string& name, const std::string& description)
    : target(exename, name, description) {}

  void extract_coalesed_buffers(const std::string& name,
                                const boost::property_tree::ptree& _pt);
  void extract_control_packet_patch(const std::string& name,
                                    const uint32_t addend,
                                    const boost::property_tree::ptree& _pt);
  void readmetajson(const std::string& metafile);
  bool parseOption(const sub_cmd_options &_options);
};

class target_aie2blob_transaction: public target_aie2blob
{
public:
  target_aie2blob_transaction(const std::string& name)
    : target_aie2blob(name, "aie2txn", "aie2 txn blob assembler") {}
  virtual void assemble(const sub_cmd_options &_options);
};

class target_aie2blob_dpu: public target_aie2blob
{
public:
  target_aie2blob_dpu(const std::string& name)
    : target_aie2blob(name, "aie2dpu", "aie2 dpu blob assembler") {}
  virtual void assemble(const sub_cmd_options &_options);
};

} //namespace aiebu::utilities

#endif //__AIEBU_UTILITIES_TARGET_H_
