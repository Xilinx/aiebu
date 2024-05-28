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
    readmetajson(external_buffers_file);

  return true;
}


/*
sample json
{
    "external_buffers": {
        "buffer0": {
            "xrt_id": 1,
            "size_in_bytes": 345088,
            "name": "coalesed_weights",
            "coalesed_buffers": [
                {
                    "logical_id": 0,
                    "offset_in_bytes": 0,
                    "name": "compute_graph.resnet_layers[0].wts_ddr",
                    "control_packet_patch_locations": [
                        {
                            "offset": 17420,
                            "size": 6,
                            "operation": "read_add_write"
                        },
                        {
                            "offset": 17484,
                            "size": 6,
                            "operation": "read_add_write"
                        },
                        {
                            "offset": 17548,
                            "size": 6,
                            "operation": "read_add_write"
                        },
                        {
                            "offset": 17612,
                            "size": 6,
                            "operation": "read_add_write"
                        }
                    ]
                },
                {
                    "logical_id": 1,
                    "offset_in_bytes": 37888,
                    "name": "compute_graph.resnet_layers[1].wts_ddr",
                    "control_packet_patch_locations": [
                        {
                            "offset": 19404,
                            "size": 6,
                            "operation": "read_add_write"
                        },
                        {
                            "offset": 19468,
                            "size": 6,
                            "operation": "read_add_write"
                        },
                        {
                            "offset": 19532,
                            "size": 6,
                            "operation": "read_add_write"
                        },
                        {
                            "offset": 19596,
                            "size": 6,
                            "operation": "read_add_write"
                        }
                    ]
                },
                {
                    "logical_id": 2,
                    "offset_in_bytes": 195584,
                    "name": "compute_graph.resnet_layers[2].wts_ddr",
                    "control_packet_patch_locations": [
                        {
                            "offset": 40012,
                            "size": 6,
                            "operation": "read_add_write"
                        },
                        {
                            "offset": 40076,
                            "size": 6,
                            "operation": "read_add_write"
                        },
                        {
                            "offset": 40140,
                            "size": 6,
                            "operation": "read_add_write"
                        },
                        {
                            "offset": 40204,
                            "size": 6,
                            "operation": "read_add_write"
                        }
                    ]
                }
            ]
        },
        "buffer1": {
            "xrt_id": 2,
            "logical_id": 3,
            "size_in_bytes": 802816,
            "name": "compute_graph.ifm_ddr",
            "control_packet_patch_locations": [
                {
                    "offset": 12,
                    "size": 6,
                    "operation": "read_add_write"
                },
                {
                    "offset": 76,
                    "size": 6,
                    "operation": "read_add_write"
                }
            ]
        },
        "buffer2": {
            "xrt_id": 3,
            "logical_id": 4,
            "size_in_bytes": 458752,
            "name": "compute_graph.ofm_ddr",
            "control_packet_patch_locations": [
                {
                    "offset": 60428,
                    "size": 6,
                    "operation": "read_add_write"
                },
                {
                    "offset": 60492,
                    "size": 6,
                    "operation": "read_add_write"
                }
            ]
        },
        "buffer3": {
            "xrt_id": 0,
            "logical_id": -1,
            "size_in_bytes": 60736,
            "ctrl_pkt_buffer": 1,
            "name": "runtime_control_packet"
        }
    }
}
*/
void
aiebu::utilities::
target_aie2blob::extract_coalesed_buffers(const std::string& name,
                                          const boost::property_tree::ptree& _pt)
{
  const auto _pt_coalesed_buffers = _pt.get_child_optional("coalesed_buffers");
  if (_pt_coalesed_buffers)
  {
    const auto coalesed_buffers = _pt_coalesed_buffers.get();
    for (auto coalesed_buffer : coalesed_buffers)
      extract_control_packet_patch(name,
                                   coalesed_buffer.second.get<uint32_t>("offset_in_bytes", 0),
                                   coalesed_buffer.second);
  }
}

void
aiebu::utilities::
target_aie2blob::extract_control_packet_patch(const std::string& name,
                                              const uint32_t addend,
                                              const boost::property_tree::ptree& _pt)
{
  const auto _pt_control_packet_patch = _pt.get_child_optional("control_packet_patch_locations");
  if (_pt_control_packet_patch)
  {
    const auto patchs = _pt_control_packet_patch.get();
    for (auto pat : patchs)
    {
      auto patch = pat.second;
      // move 8 bytes(header) up for unifying the patching scheme between DPU sequence and transaction-buffer
      uint32_t offset = patch.get<uint32_t>("offset") - 8;
      m_patch_data.push_back({name, aiebu::patch_buffer_type::control_packet,
                                aiebu::patch_schema::control_packet_48, offset, addend});
    }
  }
}

void
aiebu::utilities::
target_aie2blob::readmetajson(const std::string& external_buffers_file)
{
  // For transaction buffer flow. In Xclbin kernel argument, actual argument start from 3,
  // 0th is opcode, 1st is instruct buffer, 2nd is instruct buffer size.
  constexpr static uint32_t ARG_OFFSET = 3;
  boost::property_tree::ptree _pt;
  boost::property_tree::read_json(external_buffers_file, _pt);

  const auto _pt_external_buffers = _pt.get_child_optional("external_buffers");
  if (!_pt_external_buffers)
    return;

  const auto external_buffers = _pt_external_buffers.get();
  for (auto& external_buffer : external_buffers)
  {
    const auto _pt_coalesed_buffers = external_buffer.second.get_child_optional("coalesed_buffers");

    // added ARG_OFFSET to argidx to match with kernel argument index in xclbin
    std::string name = std::to_string(external_buffer.second.get<uint32_t>("xrt_id") + ARG_OFFSET);

    if (_pt_coalesed_buffers)
      extract_coalesed_buffers(name, external_buffer.second);
    else
      extract_control_packet_patch(name,
                                   external_buffer.second.get<uint32_t>("offset_in_bytes", 0),
                                   external_buffer.second);
  }
}

void
aiebu::utilities::
target_aie2blob_dpu::assemble(const sub_cmd_options &_options)
{
  if (!parseOption(_options))
    return;

  try {
    aiebu::aiebu_assembler as(aiebu::aiebu_assembler::buffer_type::blob_instr_dpu,
                              m_transaction_buffer, m_control_packet_buffer, m_patch_data,
                              m_libs, m_libpaths);
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
    aiebu::aiebu_assembler as(aiebu::aiebu_assembler::buffer_type::blob_instr_transaction,
                              m_transaction_buffer, m_control_packet_buffer, m_patch_data, m_libs, m_libpaths);
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
