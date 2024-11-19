// SPDX-License-Identifier: MIT
// Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

#include <fstream>
#include <iostream>
#include <boost/format.hpp>

#include "target.h"
#include "utils.h"

std::map<uint8_t, std::vector<char> >
aiebu::utilities::
target_aie2blob::parse_pmctrlpkt(const std::vector<std::string> pm_key_value_pairs)
{
  std::map<uint8_t, std::vector<char> > mappmctrl;

  for (const auto& kv : pm_key_value_pairs) {
    size_t pos = kv.find(':');
    if (pos == std::string::npos) {
      auto errMsg = boost::format("Invalid key:value pair: %s in pmctrl\n") % kv ;
      throw std::runtime_error(errMsg.str());
    }

    std::string key = kv.substr(0, pos);
    uint8_t ikey = std::stoi(key);
    std::string value = kv.substr(pos + 1);
    std::vector<char> buffer;
    readfile(value, buffer);
    mappmctrl[ikey] = buffer;
  }
  return mappmctrl;
}

bool
aiebu::utilities::
target_aie2blob::parseOption(const sub_cmd_options &_options)
{
  std::string input_file;
  std::string controlpkt_file;
  std::string external_buffers_file;
  std::vector<std::string> pm_key_value_pairs;
  cxxopts::Options all_options("Target aie2blob Options", m_description);

  try {
    all_options.add_options()
            ("o,outputelf", "ELF output file name", cxxopts::value<decltype(m_output_elffile)>())
            ("c,controlcode", "TXN control code binary", cxxopts::value<decltype(input_file)>())
            ("p,controlpkt", "Control packet binary", cxxopts::value<decltype(controlpkt_file)>())
            ("j,json", "control packet Patching json file", cxxopts::value<decltype(external_buffers_file)>())
            ("l,lib", "linked libs", cxxopts::value<decltype(m_libs)>())
            ("L,libpath", "libs path", cxxopts::value<decltype(m_libpaths)>())
            ("m,pmctrl", "pm ctrlpkt <id>:<file>", cxxopts::value<decltype(pm_key_value_pairs)>())
            ("r,report", "Generate Report", cxxopts::value<bool>()->default_value("false"))
            ("h,help", "show help message and exit", cxxopts::value<bool>()->default_value("false"))
    ;

    auto char_ver = aiebu::utilities::vector_of_string_to_vector_of_char(_options);

    auto result = all_options.parse(char_ver.size(), char_ver.data());

    if (result.count("help")) {
      std::cout << all_options.help({"", "Target aie2blob Options"});
      return false;
    }

    if (result.count("outputelf"))
      m_output_elffile = result["outputelf"].as<decltype(m_output_elffile)>();
    else
      throw std::runtime_error("the option '--outputelf' is required but missing\n");

    if (result.count("controlcode"))
      input_file = result["controlcode"].as<decltype(input_file)>();

    if (result.count("controlpkt"))
      controlpkt_file = result["controlpkt"].as<decltype(controlpkt_file)>();

    if (result.count("json"))
      external_buffers_file = result["json"].as<decltype(external_buffers_file)>();

    if (result.count("lib"))
      m_libs = result["lib"].as<decltype(m_libs)>();

    if (result.count("libpath"))
      m_libpaths = result["libpath"].as<decltype(m_libpaths)>();

    if (result.count("pmctrl"))
      pm_key_value_pairs = result["pmctrl"].as<decltype(pm_key_value_pairs)>();

    if (result.count("report"))
      m_print_report = result["report"].as<decltype(m_print_report)>();

  }
  catch (const cxxopts::exceptions::exception& e) {
    std::cout << all_options.help({"", "Target aie2blob Options"});
    auto errMsg = boost::format("Error parsing options: %s\n") % e.what() ;
    throw std::runtime_error(errMsg.str());
  }

  readfile(input_file, m_transaction_buffer);

  m_ctrlpkt = parse_pmctrlpkt(pm_key_value_pairs);

  if (!controlpkt_file.empty())
    readfile(controlpkt_file, m_control_packet_buffer);

  if (!external_buffers_file.empty())
    readfile(external_buffers_file, m_patch_data_buffer);

  return true;
}

void
aiebu::utilities::
target_aie2blob_dpu::assemble(const sub_cmd_options &_options)
{
  if (!parseOption(_options))
    return;

  try {
    aiebu::aiebu_assembler as(aiebu::aiebu_assembler::buffer_type::blob_instr_dpu,
                              m_transaction_buffer, m_control_packet_buffer, m_patch_data_buffer,
                              m_libs, m_libpaths);
    write_elf(as, m_output_elffile);
    if (m_print_report)
      as.get_report(std::cout);
  } catch (aiebu::error &ex) {
    auto errMsg = boost::format("Error: %s, code:%d\n") % ex.what() % ex.get_code() ;
    throw std::runtime_error(errMsg.str());
  }
}

void
aiebu::utilities::
target_aie2blob_transaction::assemble(const sub_cmd_options &_options)
{
  if (!parseOption(_options))
    return;

  try {
    aiebu::aiebu_assembler as(aiebu::aiebu_assembler::buffer_type::blob_instr_transaction,
                              m_transaction_buffer, m_control_packet_buffer, m_patch_data_buffer, m_libs, m_libpaths, m_ctrlpkt);
    write_elf(as, m_output_elffile);
    if (m_print_report)
      as.get_report(std::cout);
  } catch (aiebu::error &ex) {
    auto errMsg = boost::format("Error: %s, code:%d\n") % ex.what() % ex.get_code() ;
    throw std::runtime_error(errMsg.str());
  }
}

#ifdef AIEBU_FULL
void
aiebu::utilities::
target_aie2::assemble(const sub_cmd_options &_options)
{
  std::string output_elffile;
  std::string input_file;
  cxxopts::Options all_options("Target aie2 Options", m_description);

  try {
    all_options.add_options()
            ("outputelf,o", "ELF output file name", cxxopts::value<decltype(output_elffile)>())
            ("asm,c", "ASM File", cxxopts::value<decltype(input_file)>())
            ("help,h", "show help message and exit", cxxopts::value<bool>()->default_value("false"))
    ;
    auto char_ver = aiebu::utilities::vector_of_string_to_vector_of_char(_options);
    auto result = all_options.parse(char_ver.size(), char_ver.data());

    if (result.count("help")) {
      std::cout << all_options.help({"", "Target aie2 Options"});
      return;
    }

    if (result.count("outputelf"))
      output_elffile = result["outputelf"].as<decltype(output_elffile)>();
    else
    {
      throw std::runtime_error("the option '--outputelf' is required but missing\n");
    }

    if (result.count("asm"))
      input_file = result["asm"].as<decltype(input_file)>();
    else
    {
      throw std::runtime_error("the option '--asm' is required but missing\n");
    }
  }
  catch (const cxxopts::exceptions::exception& e) {
    std::cout << all_options.help({"", "Target aie2 Options"});
    auto errMsg = boost::format("Error parsing options: %s\n") % e.what() ;
    throw std::runtime_error(errMsg.str());
  }

  auto errMsg = boost::format("Error: Not supported target\n");
  throw std::runtime_error(errMsg.str());
}

void
aiebu::utilities::
target_aie2ps::assemble(const sub_cmd_options &_options)
{
  std::string output_elffile;
  std::string input_file;

  cxxopts::Options all_options("Target aie2ps Options", m_description);

  try {
    all_options.add_options()
            ("outputelf,o", "ELF output file name", cxxopts::value<decltype(output_elffile)>())
            ("asm,c", "ASM File", cxxopts::value<decltype(input_file)>())
            ("help,h", "show help message and exit", cxxopts::value<bool>()->default_value("false"))
    ;

    auto char_ver = aiebu::utilities::vector_of_string_to_vector_of_char(_options);
    auto result = all_options.parse(char_ver.size(), char_ver.data());

    if (result.count("help")) {
      std::cout << all_options.help({"", "Target aie2ps Options"});
      return;
    }

    if (result.count("outputelf"))
      output_elffile = result["outputelf"].as<decltype(output_elffile)>();
    else
    {
      throw std::runtime_error("the option '--outputelf' is required but missing\n");
    }

    if (result.count("asm"))
      input_file = result["asm"].as<decltype(input_file)>();
    else
    {
      throw std::runtime_error("the option '--asm' is required but missing\n");
    }
  }
  catch (const cxxopts::exceptions::exception& e) {
    std::cout << all_options.help({"", "Target aie2ps Options"});
    auto errMsg = boost::format("Error parsing options: %s\n") % e.what() ;
    throw std::runtime_error(errMsg.str());
  }

  std::vector<char> asmBuffer;
  readfile(input_file, asmBuffer);

  try {
    aiebu::aiebu_assembler as(aiebu::aiebu_assembler::buffer_type::asm_aie2ps, asmBuffer);
    write_elf(as, output_elffile);
  } catch (aiebu::error &ex) {
    auto errMsg = boost::format("Error: %s, code:%d\n") % ex.what() % ex.get_code() ;
    throw std::runtime_error(errMsg.str());
  }
}
#endif
