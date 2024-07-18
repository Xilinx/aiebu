// SPDX-License-Identifier: MIT
// Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>

#include "target.h"
#include "utils.h"

namespace po = boost::program_options;
namespace aiebu::utilities {

void main_helper(int argc, char** argv,
                 const std::string & _executable,
                 const std::string & _description,
                 const target_collection& _targets)
{

  bool bhelp = false;
  std::string target_name;

  po::options_description global_options("Global Options");
  global_options.add_options()
    ("help,h", po::bool_switch(&bhelp), "show help message and exit")
    ("target,t", po::value<decltype(target_name)>(&target_name), "supported targets aie2txn/aie2dpu")
  ;

  po::variables_map vm;
  po::command_line_parser parser(argc, argv);
  auto subcmd_options = aiebu::utilities::process_arguments(vm, parser, global_options, false);

  // Search for the target (case sensitive)
  std::shared_ptr<target> starget;
  for (auto & target_entry : _targets) {
    if (target_name.compare(target_entry->get_name()) == 0) {
      starget = target_entry;
      break;
    }
  }

  if (!starget) {
    if (bhelp)
      std::cerr << "ERROR: " << "Unknown target: '" << target_name << "'" << std::endl;
    aiebu::utilities::report_commands_help(_executable, _description, global_options, _targets);
    return;
  }

  if (bhelp)
    subcmd_options.push_back("--help");

  starget->assemble(subcmd_options);
}

} //namespace aiebu::utilities

int main( int argc, char** argv )
{
  aiebu::utilities::target_collection targets;
  const std::string executable = "aiebu-asm";

  {
    targets.emplace_back(std::make_shared<aiebu::utilities::target_aie2blob_transaction>(executable));
    targets.emplace_back(std::make_shared<aiebu::utilities::target_aie2blob_dpu>(executable));
  }

  // -- Program Description
  const std::string description = 
  "AIEBU Assembling utils (aiebu-asm)";

  try {
    aiebu::utilities::main_helper( argc, argv, executable, description, targets);
    return 0;
  } catch (const std::exception& e) {
    std::cout << e.what();
  }

  return 1;
}
