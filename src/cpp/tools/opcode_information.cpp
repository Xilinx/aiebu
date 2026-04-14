// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

// opcode / source lookup by PC and page
#include "tools/debug_tools.h"
#include "aiebu/aiebu_error.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <sstream>
#include <string>

namespace aiebu {

constexpr uint64_t k_page_length = 0x2000;    // page length (8KB)
constexpr uint32_t k_max_page_index = 32640;  // maximum page index

/**
 * format_hex() - Helper function to format uint64_t values as hexadecimal strings
 */
static
std::string
format_hex(uint64_t value)
{
  std::ostringstream output;
  output << "0x" << std::hex << value;
  return output.str();
}

/**
 * write_opcode_information() - Writes opcode information to the provided output stream
 * 
 * @param stream
 *  Output stream to which the opcode information will be written.
 * @param filename
 *  Name of the ELF file from which the debug information was extracted.
 * @param pc_str
 *  Program counter value for which the opcode information is being queried.
 * @param page_str
 *  Page index value for which the opcode information is being queried.
 * @param uc_str
 *  Microcontroller (uC) index for which the opcode information is being queried.
 * 
 * This function extracts the debug section from the ELF buffer, parses it as JSON, 
 * and iterates through the debug information to find opcode details matching the 
 * input PC, page index, and uC index. If a matching entry is found, the opcode 
 * information is formatted and written to the output stream. 
 * If no matching entry is found, a "Not found" message is written to the output stream.
 * 
 */
void
debug_tools::
write_opcode_information(std::ostream& stream, const std::string& filename,
  const std::string& pc_str, const std::string& page_str, const std::string& uc_str) const
{
  // Validate the required parameters and check format
  if (pc_str == "unspecified" || page_str == "unspecified")
    throw error(error::error_code::invalid_input,
                "Parameters --pc and --page-index are required for opcode-info");

  const uint64_t pc = std::stoull(pc_str, nullptr, 0);
  const uint64_t page_index = std::stoull(page_str, nullptr, 0);

  uint32_t uc_index = 0;
  if (uc_str != "unspecified" && !uc_str.empty())
    uc_index = static_cast<uint32_t>(std::stoull(uc_str, nullptr, 0));

  // Validate input parameters 
  if (page_index > k_max_page_index)
    throw error(error::error_code::invalid_input, "Page index overflow when computing page offset");

  // Compute the page length and validate that it does not overflow
  const uint64_t page_length = page_index * k_page_length;
  if (pc > k_max_page_index * k_page_length - page_length)
    throw error(error::error_code::invalid_input, "PC and page index overflow when computing page offset");

  // Extract .dump section from ELF buffer
  const auto& debug_data = get_dump_data();
  if (debug_data.empty())
    throw error(error::error_code::invalid_input, "No debug information found in the ELF file");

  // Parse the .dump section as JSON
  std::istringstream data(debug_data);
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(data, pt);

  // Compute the page offset from the page length and the PC
  const uint64_t page_offset = page_length + pc;

  bool found = false;
  boost::property_tree::ptree opcode_info;
  for (const auto& item : pt.get_child("debug")) {
    const auto& node = item.second;
    const auto data_page_offset = std::stoull(node.get<std::string>("page_offset"), nullptr, 0);
    const auto data_page_index = std::stoull(node.get<std::string>("page_index"), nullptr, 0);
    const auto data_column = static_cast<uint32_t>(std::stoul(node.get<std::string>("column"), nullptr, 0));
    if (data_page_offset != page_offset || data_page_index != page_index || data_column != uc_index)
      continue;

    opcode_info = node;
    found = true;
    break;
  }

  // Write the ELF / PC information to the output stream
  stream << "ELF File:       " << filename << '\n';
  stream << "PC:             " << format_hex(pc) << '\n';
  stream << "Opcode Information:\n";

  // If no matching entry is found, write "Not found" message to the output stream
  if (!found) {
    stream << "Not found!\n";
    return;
  }

  // If a matching entry is found, format and write the opcode information to the output stream
  const std::string operation = opcode_info.get<std::string>("operation", "");
  const auto opcode_size = std::stoull(opcode_info.get<std::string>("opcode_size", "0"), nullptr, 0);
  const auto column = static_cast<uint32_t>(std::stoul(opcode_info.get<std::string>("column", "0"), nullptr, 0));
  const auto data_page_offset = std::stoull(opcode_info.get<std::string>("page_offset", "0"), nullptr, 0);
  const auto data_page_index = std::stoull(opcode_info.get<std::string>("page_index", "0"), nullptr, 0);
  const auto line = static_cast<uint32_t>(std::stoul(opcode_info.get<std::string>("line", "0"), nullptr, 0));
  const std::string file = opcode_info.get<std::string>("file", "");

  stream << "Opcode:         " << operation << '\n';
  stream << "Opcode Size:    " << format_hex(opcode_size) << '\n';
  stream << "uC Index:       " << format_hex(static_cast<uint64_t>(column)) << '\n';
  stream << "Page Index:     " << format_hex(data_page_index) << '\n';
  stream << "Page Offset:    " << format_hex(data_page_offset) << '\n';
  stream << "Line:           " << line << '\n';
  stream << "File:           " << file << '\n';

  return;
}

} // namespace aiebu
