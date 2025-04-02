// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include <boost/format.hpp>
#include <cxxopts.hpp>
#include <set>
#include <string>
#include <iostream>
#include <map>

#include "analyzer/reporter.h"
#include "common/file_utils.h"

namespace aiebu {

static const std::set<std::string>
targets = { //NOLINT
    "aie2ps",
    "aie2asm",
    "aie2txn",
    "aie2dpu",
    "unspecified"
};

static const std::map<aiebu::aiebu_assembler::buffer_type, std::string>
buffer_type_table = { // NOLINT
  {aiebu::aiebu_assembler::buffer_type::blob_instr_dpu, "aie2-dpu-ctrlcode"},
  {aiebu::aiebu_assembler::buffer_type::blob_instr_prepost, "aie2-ctrlpkt"},
  {aiebu::aiebu_assembler::buffer_type::blob_instr_transaction, "aie2-ctrlcode"},
  {aiebu::aiebu_assembler::buffer_type::blob_control_packet, "aie2-ctrlpkt"},
  {aiebu::aiebu_assembler::buffer_type::asm_aie2ps, "aie2ps-ctrlcode-asm"},
  {aiebu::aiebu_assembler::buffer_type::asm_aie2, "aie2-ctrlcode-asm"},
  {aiebu::aiebu_assembler::buffer_type::elf_aie2, "aie2-elf"},
  {aiebu::aiebu_assembler::buffer_type::elf_aie2ps, "aie2ps-elf"},
  {aiebu::aiebu_assembler::buffer_type::pdi_aie2, "aie2-pdi"},
  {aiebu::aiebu_assembler::buffer_type::pdi_aie2ps, "aie2ps-pdi"},
  {aiebu::aiebu_assembler::buffer_type::unspecified, "unknown"},
};


cxxopts::ParseResult main_helper(int argc, const char* const *argv,
                                 const std::string & executable,
                                 const std::string & description)
{
  bool bhelp = false;
  std::string target_name;
  std::string filename;
  std::vector<std::string> subcmd_options;
  cxxopts::Options global_options(executable, description);

  try {
    global_options.add_options()
      ("a,archive-headers", "Display archive header information", cxxopts::value<bool>()->default_value("false"))
      ("f,file-headers", "Display the contents of the overall file header", cxxopts::value<bool>()->default_value("false"))
      ("x,all-headers", "Display contents of all elf headers", cxxopts::value<bool>()->default_value("false"))
      ("d,disassemble", "Display assembler contents of ctrltext sections", cxxopts::value<bool>()->default_value("false"))
      ("H,help", "show help message and exit", cxxopts::value<bool>()->default_value("false"))
      ("m,architecture", "Specify the target architecture as MACHINE (aie2ps/aie2asm/aie2txn/aie2dpu)", cxxopts::value<std::string>()->default_value("unspecified"))
      ("D,disassemble-all", "Display assembler contents of all sections", cxxopts::value<bool>()->default_value("false"))
      ("t,syms", "Display contents of the symbols table(s)", cxxopts::value<bool>()->default_value("false"))
      ("r,reloc", "Display relocation entries in the file", cxxopts::value<bool>()->default_value("false"))
      ("filename", "Input file name", cxxopts::value<std::string>());

    global_options.parse_positional({"filename"});

    auto result = global_options.parse(argc, argv);

    subcmd_options = result.unmatched();

    if (result.count("help"))
      bhelp = result["help"].as<bool>();

    target_name = result["architecture"].as<std::string>();

    if (targets.find(target_name) == targets.end())
      throw cxxopts::exceptions::incorrect_argument_type(target_name);

    if (!result.count("filename"))
      throw cxxopts::exceptions::missing_argument("filename");

    filename = result["filename"].as<decltype(filename)>();

    if (bhelp) {
      std::cout << global_options.help({"", executable}) << std::endl;
      return {};
    }

    return result;
  }
  catch (const cxxopts::exceptions::exception& e) {
    auto errMsg = boost::format("Error parsing options: %s\n") % e.what() ;
    throw std::runtime_error(errMsg.str());
  }
}

} //namespace aiebu::utilities

int main(int argc, char* argv[])
{
  const std::string executable = "aiebu-dump";
  // -- Program Description
  const std::string description = "aiebu dumping utility (aiebu-dump) for aie binaries";

  cxxopts::ParseResult result;

  try {
    result = aiebu::main_helper(argc, argv, executable, description);
  } catch (const std::exception& e) {
    std::cout << e.what();
    return 1;
  }

  const std::vector<char> buffer = aiebu::readfile(result["filename"].as<std::string>());
  aiebu::aiebu_assembler::buffer_type type = aiebu::identify_buffer_type(buffer);

  std::cout << aiebu::buffer_type_table.at(type) << std::endl;
  if (type == aiebu::aiebu_assembler::buffer_type::elf_aie2) {
    aiebu::reporter rep(aiebu::aiebu_assembler::buffer_type::elf_aie2, buffer);
    if (result["all-headers"].as<bool>()) {
      rep.elf_summary(std::cout);
      rep.ctrlcode_summary(std::cout);
    }
    if (result["disassemble"].as<bool>()) {
      rep.ctrlcode_detail_summary(std::cout);
    }
  }

  return 0;
}
