// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_INPUT_H_
#define _AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_INPUT_H_

#include <map>
#include "symbol.h"
#include "utils.h"
#include "preprocessor_input.h"
#include <boost/format.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace aiebu {

class aie2ps_preprocessor_input : public preprocessor_input
{
  constexpr static uint32_t MAX_ARG_INDEX = 32; // approximated value 24 to limit the number of arguments in XRT kernel call

  constexpr static uint64_t RANGE_32BIT = 0xFFFFFFFF; // Max value supported in 32bit elf supported
  // For transaction buffer flow. In Xclbin kernel argument, actual argument start from 3,
  // 0th is opcode, 1st is instruct buffer, 2nd is instruct buffer size.
  constexpr static uint32_t ARG_OFFSET = 0;

  std::vector<std::string> m_libpaths;
  uint32_t m_control_packet_index = 0xFFFFFFFF;
  enum class offset_type {
    CONTROL_PACKET,
    COALESED_BUFFER
  };

  void aiecompiler_json_parser(const boost::property_tree::ptree& pt);
  void dmacompiler_json_parser(const boost::property_tree::ptree& pt);
  void readmetajson(std::istream& patch_json);
  void extract_control_packet_patch(const std::string& name, const uint32_t arg_index, const boost::property_tree::ptree& _pt);
  void extract_coalesed_buffers(const std::string& name, const boost::property_tree::ptree& _pt);
  void validate_json(uint32_t offset, uint32_t size, uint32_t arg_index, offset_type type) const;
  uint32_t get_32_bit_property(const boost::property_tree::ptree& pt, const std::string& property, bool defaultvalue = false) const;

public:
  aie2ps_preprocessor_input() {}

  const std::vector<std::string>& get_include_paths() const { return m_libpaths; }
  uint32_t get_control_packet_index() const { return m_control_packet_index; }

  virtual void set_args(const std::vector<char>& control_code,
                        const std::vector<char>& patch_json,
                        const std::vector<char>& /*buffer2*/,
                        const std::vector<std::string>& /* libs */,
                        const std::vector<std::string>& libpaths,
                        const std::map<uint8_t, std::vector<char> >& /*ctrlpkt*/) override
  {
    m_libpaths = libpaths;
    m_data["control_code"] = control_code;
    if (patch_json.size() !=0 )
    {
      vector_streambuf vsb(patch_json);
      std::istream elf_stream(&vsb);
      readmetajson(elf_stream);
    }
  }

  const std::vector<std::string> get_keys()
  {
    std::vector<std::string> keys(m_data.size());
    auto key_selector = [](auto pair){return pair.first;};
    transform(m_data.begin(), m_data.end(), keys.begin(), key_selector);
    return keys;
  }

  std::vector<char>& get_data()
  {
    return m_data["control_code"];
  }
};

}
#endif //_AIEBU_PREPROCESSOR_AIE2PS_PREPROCESSOR_INPUT_H_
