// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.
// CERT trace probe listing — DWARF v5 .debug_* path and legacy .dump path.

#include "tools/debug_tools.h"
#include "aiebu/aiebu_debug.h"
#include "aiebu/aiebu_error.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <map>
#include <sstream>
#include <string>
#include <utility>

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
// ─── DWARF path ─────────────────────────────────────────────────────────────

static void
write_trace_probes_from_dwarf(std::ostream& stream, const dwarf_reader& dr,
                              const std::string& kernel_instance,
                              const ELFIO::elfio& elf)
{
  // Extract the instance portion from "kernel:instance" for filtering.
  std::string filter_instance;
  const auto colon = kernel_instance.find(':');
  if (colon != std::string::npos)
    filter_instance = kernel_instance.substr(colon + 1);

  // Build the filtered view: pointers into dr's row vector (no copy).
  // If the filter matches nothing (e.g. wrong default for this ELF), fall back
  // to all rows so the tool remains useful rather than silently producing empty output.
  std::vector<const dwarf_debug_row*> all_rows;
  for (const auto& row : dr.get_all_rows()) {
    if (!filter_instance.empty()) {
      const std::string& cu = dr.cu_name_of(row);
      const auto cu_colon = cu.find(':');
      if (cu_colon == std::string::npos ||
          cu.compare(cu_colon + 1, std::string::npos, filter_instance) != 0)
        continue;
    }
    all_rows.push_back(&row);
  }
  if (all_rows.empty() && !filter_instance.empty()) {
    // Filter matched nothing — emit all instances so output is not silently empty.
    for (const auto& row : dr.get_all_rows())
      all_rows.push_back(&row);
  }

  // Resolve the group name_id once for the whole batch.  format_operation()
  // uses this to locate the correct .ctrltext section without repeating the
  // symbol-table scan for every row.
  const AIEDebug aie_debug(elf);
  const uint32_t name_id = aie_debug.resolve_group_name_id(kernel_instance);
  // Conflict-detection tracking: keyed by "basename:ucN".
  struct probe_tracking {
    bool        conflict  = false;
    std::string file_path; // full path for the first occurrence
  };
  std::map<std::string, probe_tracking> tracking_map;

  // Pass 1: detect filename conflicts across line rows (line > 0).
  for (const auto* row : all_rows) {
    if (row->line == 0)
      continue; // annotation rows carry no file/line
    const std::string file_name = std::filesystem::path(row->file).filename().string();
    const std::string key = file_name + ":uc" + std::to_string(row->column);
    auto it = tracking_map.find(key);
    if (it == tracking_map.end())
      tracking_map[key] = {false, row->file};
    else if (it->second.file_path != row->file)
      it->second.conflict = true;
  }

  // Build {column, address} → {display_file, operation} maps in one pass over
  // line rows. Column is part of the key because DWARF addresses are only unique
  // within a single control column. Annotation rows (Pass 3) share the same
  // (column, address) as their enclosing line row, so caching avoids a redundant
  // ISA binary walk per annotation probe.
  using col_addr = std::pair<uint32_t, uint32_t>; // {column, address}
  std::map<col_addr, std::string> addr_to_display;
  std::map<col_addr, std::string> addr_to_op;
  for (const auto* row : all_rows) {
    if (row->line == 0)
      continue;
    const col_addr ca{row->column, row->address};
    const std::string file_name = std::filesystem::path(row->file).filename().string();
    const std::string key = file_name + ":uc" + std::to_string(row->column);
    const bool has_conflict = tracking_map.at(key).conflict;
    addr_to_display.emplace(ca, has_conflict ? row->file : file_name);
    if (addr_to_op.find(ca) == addr_to_op.end())
      addr_to_op.emplace(ca,
                         aie_debug.format_operation(name_id, row->column,
                                                    row->page_index, row->page_offset));
  }

  // Pass 2: emit line probes.
  for (const auto* row : all_rows) {
    if (row->line == 0)
      continue;
    const col_addr ca{row->column, row->address};
    const std::string& display_file = addr_to_display.at(ca);
    const std::string& op           = addr_to_op.at(ca);

    stream << "jprobe:" << display_file
           << ":uc"     << row->column
           << ":line"   << row->line
           << " ( on " << (op.empty() ? "UNKNOWN" : op) << " )\n";
  }

  // Pass 3: emit annotation probes from DW_TAG_label rows (line == 0).
  // Resolve display_file and operation from the maps built above; both are
  // keyed by {column, address} and annotation rows always share both with their
  // enclosing line row.
  for (const auto* row : all_rows) {
    if (row->annotation_id.empty())
      continue; // only annotation rows have this set

    const col_addr ca{row->column, row->address};
    const auto disp_it = addr_to_display.find(ca);
    const std::string display_file =
        (disp_it != addr_to_display.end()) ? disp_it->second : std::to_string(row->address);

    const auto op_it = addr_to_op.find(ca);
    const std::string op = (op_it != addr_to_op.end()) ? op_it->second : std::string{};

    stream << "jprobe:" << display_file
           << ":uc"     << row->column
           << ":annotation" << row->annotation_id
           << " ( on " << (op.empty() ? "UNKNOWN" : op) << " )\n";
  }
}

// ─── Main entry point ────────────────────────────────────────────────────────

void
debug_tools::
write_trace_probes(std::ostream& stream) const
{
  // DWARF path: no .dump present
  if (has_dwarf()) {
    write_trace_probes_from_dwarf(stream, *m_dwarf, m_kernel_instance,
                                  m_transform_manager.get_elfio());
    return;
  }

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
