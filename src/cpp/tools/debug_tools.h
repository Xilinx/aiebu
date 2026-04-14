// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_DEBUG_TOOLS_H_
#define AIEBU_DEBUG_TOOLS_H_

#include "aiebu/aiebu_assembler.h"
#include "analyzer/transform_manager.h"

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

namespace aiebu {

/**
 * @class debug_tools
 *
 * @brief
 * debug_tools provides utility functions for extracting and writing debug information
 * from elf binary buffers.
 *
 * @details
 * This class is responsible for parsing the ELF buffer, extracting the .dump section, and providing
 * functions to write trace probe information and opcode information based on the extracted debug data.
 * It uses the transform_manager to handle ELF parsing and data extraction.
 */
class debug_tools 
{
private:
  transform_manager m_transform_manager;
  const aiebu_assembler::buffer_type m_buffer_type;
  std::string m_debug_json;
  static transform_manager make_transform(
    aiebu_assembler::buffer_type type,  const std::vector<char>& buffer);
  void get_dump_section();

public:
  // Constructor
  debug_tools(aiebu_assembler::buffer_type type, const std::vector<char>& buffer);

  // Delete copy and move constructors and assignment operators
  debug_tools(const debug_tools&) = delete;               // Copy constructor
  debug_tools& operator=(const debug_tools&) = delete;    // Copy assignment operator
  debug_tools(debug_tools&&) = delete;                    // Move constructor
  debug_tools& operator=(debug_tools&&) = delete;         // Move assignment operator

  // Destructor
  ~debug_tools() = default;                               // Default destructor

  // Member functions
  const std::string& get_dump_data() const { return m_debug_json; }
  void write_trace_probes(std::ostream &stream) const;
  void write_opcode_information(std::ostream &stream, const std::string& filename,
    const std::string& pc_str, const std::string& page_str, const std::string& uc_str) const;
};

} // namespace aiebu
#endif // AIEBU_DEBUG_TOOLS_H_
