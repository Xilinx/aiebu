// SPDX-License-Identifier: MIT
// Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

#ifndef __AIEBU_UTILITIES_UTILS_H_
#define __AIEBU_UTILITIES_UTILS_H_

// Include files
// Please keep these to the bare minimum

#include <string>
#include <vector>
#include <boost/program_options.hpp>

#include "target.h"
#include "aiebu_assembler.h"

namespace po = boost::program_options;
namespace aiebu::utilities {
  void
  report_commands_help(const std::string& executable,
                       const std::string& description,
                       const po::options_description& option_description,
                       const target_collection& sub_targets);

  void
  report_target_help(const std::string& executable_name,
                     const std::string& starget,
                     const std::string& description,
                     const po::options_description& option_description,
                     bool remove_long_opt_dashes = false);

  void
  report_option_help(const std::string & group_name,
                     const po::options_description& option_description,
                     const bool report_parameter = true,
                     const bool remove_long_opt_dashes = false);

  std::string
  create_usage_string(const po::options_description &od,
                      const po::positional_options_description &pod,
                      bool remove_long_opt_dashes = false);

  bool
  is_escape_codes_disabled();

  std::string
  wrap_paragraphs(const std::string & unformattedString,
                  unsigned int indentWidth,
                  unsigned int columnWidth,
                  bool indentFirstLine);

  std::vector<std::string>
  process_arguments(po::variables_map& vm,
                    po::command_line_parser& parser,
                    const po::options_description& options,
                    bool validate_arguments);

}

#endif //__AIEBU_UTILITIES_UTILS_H_
