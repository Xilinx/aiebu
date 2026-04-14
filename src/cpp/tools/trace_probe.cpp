// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.
// CERT trace probe listing from .dump (elf_map_reader.py).

#include "tools/debug_tools.h"
#include "aiebu/aiebu_error.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <filesystem>
#include <sstream>
#include <string>

namespace aiebu {

/**
  * write_trace_probes() - Writes trace probe information to the provided output stream
  * 
  * @param stream
  *   Output stream to which the trace probe information will be written.
  * 
  * This function extracts the debug section from the ELF buffer, parses it as JSON, 
  * and iterates through the debug information to find trace probe details. 
  * For each trace probe found, trace probe information is formatted as 
  * "jprobe:<file_name>:uc<column number>:line<line_number> ( on <operation> )". 
  * If there are any annotations associated with the probe, 
  * additional lines are written for each annotation in the format 
  * "jprobe:<file_name>:uc<column_number>:annotation<annotation_id> ( on <operation> )".
  */  
void
debug_tools::
write_trace_probes(std::ostream& stream) const
{
  // Extract .dump section from ELF buffer
  const auto& debug_data = get_dump_data();
  if (debug_data.empty())
    throw error(error::error_code::invalid_input, "No debug information found in the ELF file");

  // Parse the .dump section as JSON
  std::istringstream data(debug_data);
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(data, pt);

  // Iterate through the debug information to find trace probe details
  for (const auto& item : pt.get_child("debug")) {
    const auto& node = item.second;
    const std::string file_path = node.get<std::string>("file", "");
    const std::string file_name = std::filesystem::path(file_path).filename().string();
    if (!node.get_child_optional("column") || !node.get_child_optional("line") ||
        !node.get_child_optional("operation"))
      continue;
    
    // Extract the column / line / operation from the debug node
    const auto column = static_cast<uint32_t>(std::stoul(node.get<std::string>("column"), nullptr, 0));
    const auto line = static_cast<uint32_t>(std::stoul(node.get<std::string>("line"), nullptr, 0));
    const auto operation = node.get<std::string>("operation");
    
    // Format and print the trace probe line from the debug information
    std::string probe_line =  "jprobe:" + file_name + 
                              ":uc" + std::to_string(column) + 
                              ":line" + std::to_string(line);
    stream << probe_line << " ( on " << operation << " )" << '\n';

    // If there are annotations associated with the probe, print additional lines for each annotation
    if (auto annotation = node.get_child_optional("annotation")) {
      const auto annotation_id = annotation->get<std::string>("id");
      std::string annotation_probe_line = "jprobe:" + file_name + 
                                          ":uc" + std::to_string(column) + 
                                          ":annotation" + annotation_id;
      stream << annotation_probe_line << " ( on " << operation << " )" << '\n';
    }
  }
}

} // namespace aiebu
