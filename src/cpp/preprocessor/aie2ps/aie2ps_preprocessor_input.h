// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_INPUT_H_
#define _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_INPUT_H_

#include <map>
#include "symbol.h"
#include "utils.h"
#include "file_utils.h"
#include "preprocessor_input.h"
#include <boost/format.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace aiebu {

class asm_preprocessor_input : public preprocessor_input
{
protected:
  constexpr static uint32_t MAX_ARG_INDEX = 512; // approximated value 512 to limit the number of arguments in XRT kernel call

  constexpr static uint64_t RANGE_32BIT = 0xFFFFFFFF; // Max value supported in 32bit elf supported
  // For transaction buffer flow. In Xclbin kernel argument, actual argument start from 3,
  // 0th is opcode, 1st is instruct buffer, 2nd is instruct buffer size.
  constexpr static uint32_t ARG_OFFSET = 0;

  std::vector<std::string> m_libpaths;
  std::vector<std::string> m_flags;
  uint32_t m_control_packet_index = 0xFFFFFFFF;
  uint32_t m_control_packet_offset_correction = 8;
  enum class offset_type {
    CONTROL_PACKET,
    COALESED_BUFFER
  };

  symbol::patch_schema control_packet_patching = symbol::patch_schema::control_packet_57;
  std::map<uint32_t, std::string> m_ctrlpkt_id_map;
 
  void aiecompiler_json_parser(const boost::property_tree::ptree& pt);
  void dmacompiler_json_parser(const boost::property_tree::ptree& pt);
  void readmetajson(std::istream& patch_json);
  void extract_control_packet_patch(const std::string& name, const uint32_t arg_index, const boost::property_tree::ptree& _pt);
  void extract_coalesed_buffers(const std::string& name, const boost::property_tree::ptree& _pt);
  void validate_json(uint32_t offset, uint32_t size, uint32_t arg_index, offset_type type) const;
  uint32_t get_32_bit_property(const boost::property_tree::ptree& pt, const std::string& property, bool defaultvalue = false) const;

public:
  asm_preprocessor_input() = default;
  asm_preprocessor_input(const asm_preprocessor_input& rhs) = default;
  asm_preprocessor_input& operator=(const asm_preprocessor_input& rhs) = default;
  asm_preprocessor_input(asm_preprocessor_input &&s) = default;
  asm_preprocessor_input& operator=(asm_preprocessor_input&& rhs) = default;

  const std::vector<std::string>& get_include_paths() const { return m_libpaths; }
  uint32_t get_control_packet_index() const { return m_control_packet_index; }
  const std::vector<std::string>& get_flags() const { return m_flags; }

  virtual void set_args(const std::vector<char>& control_code,
                        const std::vector<char>& patch_json,
                        const std::vector<char>& /*buffer2*/,
                        const std::vector<std::string>& flags,
                        const std::vector<std::string>& libpaths,
                        const std::map<uint32_t, std::vector<char> >& /*ctrlpkt*/) override
  {
    m_libpaths = libpaths;
    m_flags = flags;
    m_data["control_code"] = control_code;
    if (patch_json.size() !=0 )
    {
      vector_streambuf vsb(patch_json);
      std::istream elf_stream(&vsb);
      readmetajson(elf_stream);
    }
  }

  std::vector<char> &get_ctrlcode_data()
  {
    return get_data("control_code");
  }

  std::map<std::string, std::vector<char>>& get_controlpkt()
  {
    return m_controlpkt;
  }

  std::map<uint32_t, std::string>& get_ctrlpkt_id_map()
  {
    return m_ctrlpkt_id_map;
  }
};

class aie2ps_preprocessor_input : public asm_preprocessor_input
{
public:
  aie2ps_preprocessor_input() { control_packet_patching = symbol::patch_schema::control_packet_57;}

};

class asm_config_preprocessor_input : public preprocessor_input
{
protected: // NOLINT
  std::map<std::string, std::map<std::string, std::shared_ptr<asm_preprocessor_input>>> m_preprocessor_input;
public:
  const std::map<std::string, std::map<std::string, std::shared_ptr<asm_preprocessor_input>>>&
  get_kernel_map() const { return m_preprocessor_input; }

  void add_instance(const std::string& kernel,
                    const boost::property_tree::ptree& pinstance,
                    const std::vector<std::string>& flags,
                    const std::vector<std::string>& paths)
  {

    for (const auto& [unused, pic] : pinstance)
    {
      auto tname = pic.get<std::string>("id");
      auto ccode_file = findFilePath(pic.get<std::string>("ctrl_code_file"), paths);
      auto ccode = readfile(ccode_file);

      std::vector<char> jdata;
      if (!pic.get<std::string>("patch_info_file", "").empty())
        jdata = readfile(pic.get<std::string>("patch_info_file"), paths);

      add_preprocessor_input(kernel, tname, ccode, jdata, flags, paths);
    }
  }

  void parse_config_json(std::istream& patch_json,
                         const std::vector<std::string>& flags,
                         const std::vector<std::string>& paths)
  {
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(patch_json, pt);

    const auto& pt_xrt_kernel_instance = pt.get_child_optional("xrt-kernels");
    if (!pt_xrt_kernel_instance) {
      std::cout << "xrt-kernels instance not found returning\n";
      return;
    }
    const auto& p_xrt_kernel_instance = pt_xrt_kernel_instance.get();
    for (const auto& [unused, ctrlcode] : p_xrt_kernel_instance)
    {
      //const auto& ctrlcode = pt_ctrlcode.second;
      //get mangled kernel name
      function func;
      func.name = ctrlcode.get<std::string>("name");
      for (const auto& item : ctrlcode.get_child("arguments")) {
        func.arguments.emplace_back(item.second.get<std::string>("name"),
                                    item.second.get<std::string>("type"),
                                    item.second.get<std::string>("offset"));
      }
      std::string mangled_name = mangle_function_name(func);
      //std::cout << "Mangled Function Name: " << mangled_name << std::endl;

      const auto& pt_pdis = ctrlcode.get_child_optional("PDIs");
      if (pt_pdis)
        throw error(error::error_code::invalid_asm, "PDIs section should not be present for json with controlcode in asm format\n");

      const auto& pt_instance = ctrlcode.get_child_optional("instance");
      if (pt_instance) {
        const auto& pinstance = pt_instance.get();
        add_instance(mangled_name, pinstance, flags, paths);
      } else {
        std::cout << "instance not found\n";
      }
    }
  }

  void set_args(const std::vector<char>& /*control_code*/,
                const std::vector<char>& patch_json,
                const std::vector<char>& /*buffer2*/,
                const std::vector<std::string>& libs,
                const std::vector<std::string>& libpaths,
                const std::map<uint32_t, std::vector<char> >& /*ctrlpkt*/) override
  {
    if (patch_json.size() !=0)
    {
      vector_streambuf vsb(patch_json);
      std::istream elf_stream(&vsb);
      parse_config_json(elf_stream, libs, libpaths);
    }
  }

  virtual void add_preprocessor_input(const std::string& /*kernel*/,
                                      const std::string& /*instance*/,
                                      const std::vector<char>& /*control_code*/,
                                      const std::vector<char>& /*patch_json*/,
                                      const std::vector<std::string>& /*flags*/,
                                      const std::vector<std::string>& /*paths*/) = 0;

  ~asm_config_preprocessor_input() override = default;
};

template <typename T>
class controlcode_config_preprocessor_input : public asm_config_preprocessor_input
{
public:
  void add_preprocessor_input(const std::string& kernel,
                              const std::string& instance,
                              const std::vector<char>& control_code,
                              const std::vector<char>& patch_json,
                              const std::vector<std::string>& flags,
                              const std::vector<std::string>& paths) override
  {
    auto input = std::make_shared<T>();
    input->set_args(control_code, patch_json, {}, flags, paths, {});
    m_preprocessor_input[kernel][instance] = input;
  }
};

}
#endif //_AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_INPUT_H_
