// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

#include <fstream>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "target.h"
#include "utils.h"

bool
aiebu::utilities::
target_aie2blob::parseOption(const sub_cmd_options &_options)
{
  bool bhelp;
  std::string input_file;
  std::string meta_file;
  po::options_description common_options;
  common_options.add_options()
            ("outputelf,o", po::value<decltype(m_output_elffile)>(&m_output_elffile)->required(), "ELF output file name")
            ("controlcode,c", po::value<decltype(input_file)>(&input_file), "TXN control code binary")
            ("meta,m", po::value<decltype(meta_file)>(&meta_file), "Patching Meta json file")
            ("help,h", po::bool_switch(&bhelp), "show help message and exit")
  ;

  po::options_description all_options("All Options");
  all_options.add(common_options);

  if (std::find(_options.begin(), _options.end(), "--help") != _options.end()) {
    aiebu::utilities::report_target_help(m_executable, m_sub_target_name, m_description, common_options);
    return false;
  }

  po::variables_map vm;
  po::command_line_parser parser(_options);

  aiebu::utilities::process_arguments(vm, parser, all_options, true);

  readfile(input_file, m_transaction_buffer);

  if (!meta_file.empty())
    readmetajson(meta_file);

  return true;
}


/*
sample json
{
    "control-packet": {
        "name": "control-packet-0",
        "path": "path/to/ctrl_pkt0.bin"
    },
    "symbols": [
        {
            "name": "sym",
            "patching": [
                {
                    "name": "3",
                    "buf_type": 0,
                    "offsets": 72,
                    "schema": 7,
                    "addend": 0
                },
                }
                    "name": "3",
                    "buf_type": 0,
                    "offsets": 72,
                    "schema": 7,
                    "addend": 0
                }
            ]
       }
    ]
}
*/
void
aiebu::utilities::
target_aie2blob::readmetajson(std::string& metafile)
{
  boost::property_tree::ptree _pt;
  boost::property_tree::read_json(metafile, _pt);

  auto _pt_control_packet = _pt.get_child_optional("control-packet");
  if (_pt_control_packet)
  {
    auto path = _pt_control_packet.get().get<std::string>("path");
    readfile(path, m_control_packet_buffer);
  }

  auto _pt_symbols = _pt.get_child_optional("symbols");
  if (!_pt_symbols)
    return;

  auto syms = _pt_symbols.get();
  for (auto sym : syms)
  {
    auto _pt_patching = sym.second.get_child_optional("patching");
    if (_pt_patching)
    {
      auto patchs = _pt_patching.get();
      for (auto pat : patchs)
      {
        auto patch = pat.second;
        aiebu::patch_info sym;
        sym.symbol = patch.get<std::string>("name");
        sym.buf_type = (aiebu::patch_buffer_type)patch.get<uint32_t>("buf_type");
        sym.schema = (aiebu::patch_schema)patch.get<uint32_t>("schema");
        sym.offset = (uint32_t)patch.get<uint32_t>("offsets");
        m_patch_data.push_back(sym);
      }
    }
  }
}

void
aiebu::utilities::
target_aie2blob_dpu::assemble(const sub_cmd_options &_options)
{
  if (!parseOption(_options))
    return;

  try {
    aiebu::aiebu_assembler as(aiebu::aiebu_assembler::buffer_type::blob_instr_dpu, m_transaction_buffer, m_control_packet_buffer, m_patch_data);
    write_elf(as, m_output_elffile);
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
    aiebu::aiebu_assembler as(aiebu::aiebu_assembler::buffer_type::blob_instr_transaction, m_transaction_buffer, m_control_packet_buffer, m_patch_data);
    write_elf(as, m_output_elffile);
  } catch (aiebu::error &ex) {
    auto errMsg = boost::format("Error: %s, code:%d\n") % ex.what() % ex.get_code() ;
    throw std::runtime_error(errMsg.str());
  }
}

void
aiebu::utilities::
target_aie2::assemble(const sub_cmd_options &_options)
{
  bool bhelp;
  std::string output_elffile;
  std::string input_file;
  po::options_description common_options;
  common_options.add_options()
            ("outputelf,o", boost::program_options::value<decltype(output_elffile)>(&output_elffile)->required(), "ELF output file name")
            ("asm,c", boost::program_options::value<decltype(input_file)>(&input_file)->required(), "ASM File")
            ("help,h",    boost::program_options::bool_switch(&bhelp), "show help message and exit")
  ;

  if (std::find(_options.begin(), _options.end(), "--help") != _options.end()) {
    aiebu::utilities::report_target_help(m_executable, m_sub_target_name, m_description, common_options);
    return;
  }

  po::variables_map vm;
  po::command_line_parser parser(_options);

  aiebu::utilities::process_arguments(vm, parser, common_options, true);

  auto errMsg = boost::format("Error: Not supported target\n");
  throw std::runtime_error(errMsg.str());
}

void
aiebu::utilities::
target_aie2ps::assemble(const sub_cmd_options &_options)
{
  bool bhelp;
  std::string output_elffile;
  std::string input_file;
  po::options_description common_options;
  common_options.add_options()
            ("outputelf,o", po::value<decltype(output_elffile)>(&output_elffile)->required(), "ELF output file name")
            ("asm,c", po::value<decltype(input_file)>(&input_file)->required(), "ASM File")
            ("help,h", po::bool_switch(&bhelp), "show help message and exit")
  ;

  if (std::find(_options.begin(), _options.end(), "--help") != _options.end()) {
    aiebu::utilities::report_target_help(m_executable, m_sub_target_name, m_description, common_options);
    return;
  }

  po::variables_map vm;
  po::command_line_parser parser(_options);

  aiebu::utilities::process_arguments(vm, parser, common_options, true);

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
