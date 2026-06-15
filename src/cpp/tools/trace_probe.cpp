// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.
// CERT trace probe listing from .dump (elf_map_reader.py).

#include "tools/debug_tools.h"
#include "aiebu/aiebu_error.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <algorithm>
#include <cctype>
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
  * "jprobe:<file_name/file_path>:uc<column number>:line<line_number> ( on <operation> )". 
  * If there are any annotations associated with the probe, 
  * additional lines are written for each annotation in the format 
  * "jprobe:<file_name/file_path>:uc<column_number>:annotation<annotation_id> ( on <operation> )".
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

  // Track probe keys to detect filename conflicts during two-pass processing
  struct probe_tracking {
    bool conflict = false;                  // conflict detected for the tracking key
    std::string file_path;                  // file path associated with the tracking key
  };
  std::map<std::string, probe_tracking> tracking_map;

  // First pass: collect file paths for each tracking key and detect conflicts
  for (const auto& item : pt.get_child("debug")) {
    const auto& node = item.second;
    const std::string file_path = node.get<std::string>("file", "");
    const std::string file_name = std::filesystem::path(file_path).filename().string();
    if (!node.get_child_optional("column") || !node.get_child_optional("line") ||
        !node.get_child_optional("operation"))
      continue;

    // Skip invalid operations for jprobes
    auto operation = node.get<std::string>("operation");
    std::transform(operation.begin(), operation.end(), operation.begin(),
                  [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (operation == "eof" || operation.find(".align") != std::string::npos ||
        operation.find(".long") != std::string::npos)
      continue;

    const auto column = node.get<std::string>("column");
    const std::string tracking_key = file_name + ":uc" + column;

    // Check if tracking key exists in tracking map
    if (tracking_map.find(tracking_key) == tracking_map.end()) {
      tracking_map[tracking_key] = probe_tracking();
      tracking_map[tracking_key].file_path = file_path;
    } else {
      // If tracking key already exists, check for conflict
      auto& tracking = tracking_map[tracking_key];
      if (tracking.file_path != file_path)
        tracking.conflict = true;
    }
  }

  // Second pass: output probes in order with conflict information
  for (const auto& item : pt.get_child("debug")) {
    const auto& node = item.second;
    const std::string file_path = node.get<std::string>("file", "");
    const std::string file_name = std::filesystem::path(file_path).filename().string();
    if (!node.get_child_optional("column") || !node.get_child_optional("line") ||
        !node.get_child_optional("operation"))
      continue;

    // Skip invalid operations for jprobes
    auto operation = node.get<std::string>("operation");
    std::transform(operation.begin(), operation.end(), operation.begin(),
                  [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (operation == "eof" || operation.find(".align") != std::string::npos)
      continue;

    // Extract the column / line / operation from the debug node
    const auto column = node.get<std::string>("column");
    const auto line = node.get<std::string>("line");
    const auto operation_orig = node.get<std::string>("operation");
    const std::string tracking_key = file_name + ":uc" + column;

    // Determine if conflict exists for this tracking_key
    const bool has_conflict = tracking_map[tracking_key].conflict;

    std::string probe_line_file_path = "jprobe:" + (has_conflict ? file_path : file_name) 
                                     + ":uc" + column 
                                     + ":line" + line;
    stream << probe_line_file_path << " ( on " << operation_orig << " )" << '\n';

    // If there are annotations associated with the probe, print additional lines for each annotation
    if (auto annotation = node.get_child_optional("annotation")) {
      const auto annotation_id = annotation->get<std::string>("id");
      std::string annotation_probe_line = "jprobe:" + (has_conflict ? file_path : file_name) 
                                        + ":uc" + column 
                                        + ":annotation" + annotation_id;
      stream << annotation_probe_line << " ( on " << operation_orig << " )" << '\n';
    }
  }
}

} // namespace aiebu
