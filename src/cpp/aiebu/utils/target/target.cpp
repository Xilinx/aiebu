// SPDX-License-Identifier: MIT
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
  std::string controlpkt_file;
  std::string external_buffers_file;
  po::options_description common_options;
  common_options.add_options()
            ("outputelf,o", po::value<decltype(m_output_elffile)>(&m_output_elffile)->required(), "ELF output file name")
            ("controlcode,c", po::value<decltype(input_file)>(&input_file), "TXN control code binary")
            ("controlpkt,p", po::value<decltype(controlpkt_file)>(&controlpkt_file), "Control packet binary")
            ("json,j", po::value<decltype(external_buffers_file)>(&external_buffers_file), "control packet Patching json file")
            ("lib,l", po::value<decltype(m_libs)>(&m_libs)->multitoken(), "linked libs")
            ("libpath,L", po::value<decltype(m_libpaths)>(&m_libpaths)->multitoken(), "libs path")
            ("report,r", po::bool_switch(&m_print_report), "Generate Report")
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
                              m_transaction_buffer, m_control_packet_buffer, m_patch_data_buffer, m_libs, m_libpaths);
    write_elf(as, m_output_elffile);
    if (m_print_report)
      as.get_report(std::cout);
  } catch (aiebu::error &ex) {
    auto errMsg = boost::format("Error: %s, code:%d\n") % ex.what() % ex.get_code() ;
    throw std::runtime_error(errMsg.str());
  }
}
