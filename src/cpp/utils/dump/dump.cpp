// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include <boost/format.hpp>
#include <cxxopts.hpp>
#include <set>
#include <string>
#include <iostream>

#include "analyzer/reporter.h"
#include "common/utils.h"

namespace aiebu {

static const std::set<std::string> targets = { //NOLINT
    "aie2ps",
    "aie2asm",
    "aie2txn",
    "aie2dpu",
    "unspecified"
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
    global_options.allow_unrecognised_options().add_options()
      ("a,archive-headers", "Display archive header information", cxxopts::value<bool>()->default_value("false"))
      ("f,file-headers", "Display the contents of the overall file header", cxxopts::value<bool>()->default_value("false"))
      ("x,all-headers", "Display contents of all elf headers", cxxopts::value<bool>()->default_value("false"))
      ("d,disassemble", "Display assembler contents of ctrltext sections", cxxopts::value<bool>()->default_value("false"))
      ("H,help", "show help message and exit", cxxopts::value<bool>()->default_value("false"))
      ("m,architecture", "Specify the target architecture as MACHINE (aie2ps/aie2asm/aie2txn/aie2dpu)", cxxopts::value<decltype(target_name)>()->default_value("unspecified"))
      ("D,disassemble-all", "Display assembler contents of all sections", cxxopts::value<bool>()->default_value("false"))
      ("t,syms", "Display contents of the symbols table(s)", cxxopts::value<bool>()->default_value("false"))
      ("r,reloc", "Display relocation entries in the file", cxxopts::value<bool>()->default_value("false"))
      ("filename", "Input file name", cxxopts::value<decltype(filename)>());

    auto result = global_options.parse(argc, argv);

    subcmd_options = result.unmatched();

    if (result.count("help"))
      bhelp = result["help"].as<bool>();

    if (result.count("target"))
      target_name = result["target"].as<decltype(target_name)>();
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
  const std::string description = "aiebu dumping utility (aiebu-dump)";

  cxxopts::ParseResult result;

  try {
    result = aiebu::main_helper(argc, argv, executable, description);
    return 0;
  } catch (const std::exception& e) {
    std::cout << e.what();
  }

  aiebu::reporter reporter(aiebu::aiebu_assembler::buffer_type::unspecified,
                           aiebu::readfile(result["filename"].as<std::string>()));

  return 1;
}
