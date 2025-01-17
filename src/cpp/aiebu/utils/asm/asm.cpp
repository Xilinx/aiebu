// SPDX-License-Identifier: MIT
// Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

#include <boost/format.hpp>
#include <cxxopts.hpp>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>

#include "target.h"

namespace aiebu::utilities {

void main_helper(int argc, char** argv,
                 const std::string & _executable,
                 const std::string & _description,
                 const target_collection& _targets)
{

  bool bhelp = false;
  std::string target_name;
  std::vector<std::string> subcmd_options;
  cxxopts::Options global_options(_executable, _description);

  try {
    global_options
      .allow_unrecognised_options()
      .add_options()
      ("h,help", "show help message and exit", cxxopts::value<bool>()->default_value("false"))
#ifdef AIEBU_FULL
      ("t,target", "supported targets aie2ps/aie2asm/aie2txn/aie2dpu", cxxopts::value<decltype(target_name)>())
#else
      ("t,target", "supported targets aie2txn/aie2dpu/aie2asm", cxxopts::value<decltype(target_name)>())
#endif
    ;

    auto result = global_options.parse(argc, argv);

    subcmd_options = result.unmatched();

    if (result.count("help"))
      bhelp = result["help"].as<bool>();

    if (result.count("target"))
      target_name = result["target"].as<decltype(target_name)>();
  }
  catch (const cxxopts::exceptions::exception& e) {
    auto errMsg = boost::format("Error parsing options: %s\n") % e.what() ;
    throw std::runtime_error(errMsg.str());
  }


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
    std::cout << global_options.help({"", _executable}) << std::endl;
    return;
  }

  if (bhelp || subcmd_options.size() == 0)
    subcmd_options.push_back("--help");

  subcmd_options.insert(subcmd_options.begin(), _executable);
  starget->assemble(subcmd_options);
}

} //namespace aiebu::utilities

int main( int argc, char** argv )
{
  aiebu::utilities::target_collection targets;
  const std::string executable = "aiebu-asm";

  {
#ifdef AIEBU_FULL
    targets.emplace_back(std::make_shared<aiebu::utilities::target_aie2ps>(executable));
#endif
    targets.emplace_back(std::make_shared<aiebu::utilities::target_aie2>(executable));
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
