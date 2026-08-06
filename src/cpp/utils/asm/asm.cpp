// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026 Advanced Micro Devices, Inc. All rights reserved.

#include <boost/format.hpp>
#include <cxxopts.hpp>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>

#include "target.h"
#include "utils.h"
#include "elf/elf_compression.h"

namespace aiebu::utilities {

// Returns true if subcmd_options contains a compress= flag requesting actual
// compression.
static bool has_compress_flag(const std::vector<std::string>& options)
{
  for (const auto& opt : options) {
    if (opt == "compress" || opt.rfind("compress=", 0) == 0) {
      if (opt != "compress=none")
        return true;
    }
  }
  return false;
}

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
      ("t,target", "supported targets aie2ps/aie2asm/aie2txn/aie2dpu/aie2_config/aie4/aie2ps_config/aie4_config (specific aie4 variant determined from .target directive in ASM)", cxxopts::value<decltype(target_name)>())
      ("v,version", "show version and exit", cxxopts::value<bool>()->default_value("false"))
      ;

    auto result = global_options.parse(argc, argv);

    subcmd_options = result.unmatched();

    if (result.count("help"))
      bhelp = result["help"].as<bool>();

    if (result.count("target"))
      target_name = result["target"].as<decltype(target_name)>();

    if (result.count("version")) {
      std::cout << version_string();
      return;
    }
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
    subcmd_options.emplace_back("--help");

  subcmd_options.insert(subcmd_options.begin(), _executable);

  starget->assemble(subcmd_options);

  // Compression warnings — printed whenever a compress= flag is active.
  if (has_compress_flag(subcmd_options)) {
    const aiebu::compress_stats& cs = aiebu::get_last_compress_stats();
    if (cs.has_unmerged_sections)
      std::cerr << "Warning: ELF uses legacy per-page sections (.ctrltext/<col>/<page>, "
                   ".ctrldata/<col>/<page>).\n"
                   "         Compression is less effective without merged sections. \n";
    if (cs.has_dump_section)
      std::cerr << "Warning: ELF contains .dump section(s) which are not compressed.\n"
                   "         The compression ratio reflects the full ELF size including "
                   "uncompressed .dump data.\n";
  }
}

} //namespace aiebu::utilities

int main( int argc, char** argv )
{
  aiebu::utilities::target_collection targets;
  const std::string executable = "aiebu-asm";

  {
    targets.emplace_back(std::make_shared<aiebu::utilities::target_aie2ps>(executable));
    targets.emplace_back(std::make_shared<aiebu::utilities::target_aie2>(executable));
    targets.emplace_back(std::make_shared<aiebu::utilities::target_aie2blob_transaction>(executable));
    targets.emplace_back(std::make_shared<aiebu::utilities::target_aie2blob_dpu>(executable));
    targets.emplace_back(std::make_shared<aiebu::utilities::target_aie2_config>(executable));
    targets.emplace_back(std::make_shared<aiebu::utilities::target_aie4>(executable));
    targets.emplace_back(std::make_shared<aiebu::utilities::target_aie2ps_config>(executable));
    targets.emplace_back(std::make_shared<aiebu::utilities::target_aie4_config>(executable));
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
