// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include <boost/format.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <cxxopts.hpp>
#include <string>
#include <iostream>

#include "analyzer/passmanager.h"
#include "common/file_utils.h"
#include "common/utils.h"

namespace aiebu {

cxxopts::ParseResult main_helper(int argc, const char* const *argv,
                                 const std::string & executable,
                                 const std::string & description)
{
  std::vector<std::string> subcmd_options;
  cxxopts::Options global_options(executable, description);

  try {
    global_options.add_options()(
        "j,transform",
        "Name of JSON file with requested ctrlcode transform patterns",
        cxxopts::value<std::string>()->default_value("unspecified.json"))(
        "o,output", "Name of the output ELF file",
        cxxopts::value<std::string>()->default_value("unspecified.elf"))(
        "filename", "Input file name", cxxopts::value<std::string>())(
        "help,h", "show help message and exit",
        cxxopts::value<bool>()->default_value("false"))
      ("d,debug", "dump ctrlcode after each transform", cxxopts::value<bool>()->default_value("false"))
      ("v,version", "show version and exit", cxxopts::value<bool>()->default_value("false"));
    global_options.parse_positional({"filename"});

    auto result = global_options.parse(argc, argv);

    subcmd_options = result.unmatched();

    if (result.count("version")) {
      std::cout << version_string();
      return {};
    }

    if (result.count("help")) {
      std::cout << global_options.help({"", executable}) << std::endl;
      return {};
    }

    if (!result.count("filename"))
      throw cxxopts::exceptions::missing_argument("filename");

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
  const std::string executable = "aiebu-transform";
  // -- Program Description
  const std::string description = "aiebu ctrlcode transforming utility (aiebu-transform) for aie binaries";

  cxxopts::ParseResult result;

  try {
    result = aiebu::main_helper(argc, argv, executable, description);
  } catch (const std::exception& e) {
    std::cout << e.what();
    return 1;
  }

  if (!result.arguments().size())
    return 1;

  const std::vector<char> buffer = aiebu::readfile(result["filename"].as<std::string>());
  if (aiebu::identify_buffer_type(buffer) !=
      aiebu::aiebu_assembler::buffer_type::elf_aie2)
    return 1;

  ELFIO::elfio ebin;
  if (!ebin.load(result["filename"].as<std::string>()))
    return 1;

  boost::property_tree::ptree spec;
  boost::property_tree::read_json(result["transform"].as<std::string>(), spec);

  // Run the transforms
  aiebu::passmanager passm(ebin, spec, result["debug"].as<bool>());
  passm.run_transforms();

  // Now save the ELF with transformed ctrlcode
  if(!ebin.save(result["output"].as<std::string>()))
    return 1;

  return 0;
}
