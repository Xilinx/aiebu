// SPDX-License-Identifier: MIT
// Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

// opcode / source lookup by PC and page
#include "tools/debug_tools.h"
#include "aiebu/aiebu_error.h"
#include "elf/aie_elf_constants.h"
#include "specification/aie2ps/isa.h"
#include "ops/oparg.h"
#include "common/utils.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace aiebu {

constexpr uint64_t k_page_length = 0x2000;    // page length (8KB)
constexpr uint32_t k_max_page_index = 32640;  // maximum page index

// Additional page layout constants used by the ISA binary walk path
static constexpr size_t  k_binary_page_header_size = 16;   // header at the start of each merged-format page
static constexpr uint8_t k_align_opcode            = 0xA5; // alignment pseudo-instruction byte

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

// group_id passed in from XRT is the ELF section index of the .group.N section
// (the value stored in elf_impl::m_kernel_name_to_id_map / returned by
// get_ctrlcode_id()).  AIEBU section names use the numeric N from ".group.N"
// as their suffix, not the raw section index.  This helper resolves the index
// to N so callers can construct correct section names.
// Returns UINT32_MAX if resolution fails (treated as single-instance ELF).
static uint32_t
resolve_group_name_id(const ELFIO::elfio& elf, uint32_t group_id)
{
  const ELFIO::section* grp_sec = elf.sections[group_id];
  if (!grp_sec)
    return UINT32_MAX;

  // Name is expected to be ".group.<N>" — extract N.
  const std::string& name = grp_sec->get_name();
  constexpr std::string_view prefix = ".group.";
  if (name.size() <= prefix.size() || name.compare(0, prefix.size(), prefix) != 0)
    return UINT32_MAX;

  try {
    return static_cast<uint32_t>(std::stoul(name.substr(prefix.size())));
  } catch (...) {
    return UINT32_MAX;
  }
}

// Returns the content of the .dump or .dump.<group_id> section.
// group_id == UINT32_MAX means single-instance ELF (no group suffix).
static std::string
get_dump_json_from_elf(const ELFIO::elfio& elf, uint32_t group_id)
{
  const std::string section_name = (group_id == UINT32_MAX)
      ? ".dump" : ".dump." + std::to_string(group_id);

  const ELFIO::section* sec = elf.sections[section_name];
  if (!sec || sec->get_type() != ELFIO::SHT_PROGBITS || !sec->get_data() || sec->get_size() == 0)
    return {};
  return std::string(sec->get_data(), sec->get_size());
}

// Searches the dump JSON for the entry matching (uc_idx, page_idx, offset) and
// returns a populated opcode_information with source file and line number.
static opcode_information
decode_from_dump(const std::string& dump_json,
                 uint32_t uc_idx, uint32_t page_idx, uint32_t offset)
{
  opcode_information result{};
  const uint64_t page_offset = static_cast<uint64_t>(page_idx) * k_page_length + offset;

  std::istringstream json_stream(dump_json);
  boost::property_tree::ptree pt;
  try {
    boost::property_tree::read_json(json_stream, pt);
  } catch (const std::exception& e) {
    result.diag_info = std::string("dump section parse error: ") + e.what();
    return result;
  }

  try {
    for (const auto& item : pt.get_child("debug")) {
      const auto& node = item.second;
      const auto node_page_offset = std::stoull(node.get<std::string>("page_offset"), nullptr, 0);
      const auto node_page_index  = std::stoull(node.get<std::string>("page_index"),  nullptr, 0);
      const auto node_column      = static_cast<uint32_t>(
                                      std::stoul(node.get<std::string>("column"), nullptr, 0));
      if (node_page_offset != page_offset || node_page_index != page_idx || node_column != uc_idx)
        continue;

      result.found       = true;
      result.opcode_name = node.get<std::string>("operation",   "");
      result.opcode_size = std::stoull(node.get<std::string>("opcode_size", "0"), nullptr, 0);
      result.page_offset = std::stoull(node.get<std::string>("page_offset",  "0"), nullptr, 0);
      result.line        = static_cast<uint32_t>(
                             std::stoul(node.get<std::string>("line", "0"), nullptr, 0));
      result.source_file = node.get<std::string>("file", "");
      return result;
    }
  } catch (const std::exception& e) {
    result.diag_info = std::string("dump section format error: ") + e.what();
    return result;
  }

  return result;  // found = false
}

// Decodes the opcode at (uc_idx, page_idx, offset) via ISA binary walk.
// Used as fallback when no .dump section is present.
// group_id == UINT32_MAX means single-instance ELF (no group suffix in section names).
static opcode_information
decode_opcode(const ELFIO::elfio& elf, uint32_t uc_idx, uint32_t page_idx,
              uint32_t offset, uint32_t group_id)
{
  opcode_information result{};
  const uint8_t os_abi      = elf.get_os_abi();
  const uint8_t abi_version = elf.get_abi_version();

  // Determine ELF layout from os_abi + abi_version:
  //   os_abi=0x46 (aie2ps_group), abi_version>=0x03  → per-page config ELF:
  //     one .ctrltext.<col>.<page>[.<N>] section per page.
  //   os_abi in {0x40,0x4B,0x56,0x69}, abi_version>=0x21 → merged-section config ELF:
  //     one .ctrltext.<col>[.<N>] section holds all pages concatenated.
  const bool is_merged = (os_abi != osabi_aie2ps_group) && (abi_version >= elf_version_config);

  const std::string grp_suffix = (group_id == UINT32_MAX) ? "" : ("." + std::to_string(group_id));
  const std::string section_name = is_merged
      ? ".ctrltext." + std::to_string(uc_idx) + grp_suffix
      : ".ctrltext." + std::to_string(uc_idx) + "." + std::to_string(page_idx) + grp_suffix;

  const ELFIO::section* sec = elf.sections[section_name];
  if (!sec || !sec->get_data()) {
    result.diag_info = "section not found: " + section_name;
    return result;
  }

  const size_t sec_size = sec->get_size();
  // Both merged and per-page page slots begin with a 16-byte header followed by
  // instruction data.  The firmware PC (offset) counts from page slot start, so
  // the target within the instruction area is offset minus the header size.
  if (offset < k_binary_page_header_size) {
    result.diag_info = "offset " + std::to_string(offset)
                     + " is within page header of " + section_name;
    return result;
  }
  const size_t instr_offset = offset - k_binary_page_header_size;

  // instr_start : first instruction byte (absolute offset within the section).
  // region_end  : one past the last byte of the current page's region in the section.
  size_t instr_start, region_end;
  if (is_merged) {
    const size_t page_base = static_cast<size_t>(page_idx) * k_page_length;
    instr_start = page_base + k_binary_page_header_size;
    region_end  = std::min(sec_size, page_base + k_page_length);
  } else {
    instr_start = k_binary_page_header_size;
    region_end  = sec_size;
  }

  const size_t opcode_pos = instr_start + instr_offset;
  const size_t instr_size = (region_end > instr_start) ? (region_end - instr_start) : 0;

  result.diag_info = "section=" + section_name + " size=" + std::to_string(sec_size)
                   + " pos=" + std::to_string(opcode_pos);

  // Use instr_size (not sec_size) so we also catch the merged case where opcode_pos
  // is within the section but past this page's region_end.
  if (instr_offset >= instr_size)
    return result;

  // Walk instructions from the start of the page region to find the one spanning instr_offset
  const char* instr_data = sec->get_data() + instr_start;

  isa_disassembler isa;
  const std::map<uint8_t, isa_op_disasm>* isa_map = isa.get_isa_map();

  size_t pos = 0;
  while (pos < instr_size) {
    const uint8_t opbyte = static_cast<uint8_t>(instr_data[pos]);

    if (opbyte == k_align_opcode) {
      if (pos == instr_offset) {
        result.found       = true;
        result.opcode_name = "align";
        result.opcode_size = 1;
        return result;
      }
      ++pos;
      continue;
    }

    const auto it = isa_map->find(opbyte);
    if (it == isa_map->end()) {
      result.diag_info += " unknown_opcode=" + ELFIO::to_hex_string(opbyte);
      return result;
    }

    // Instruction size: 1B opcode + 1B pad + arg bytes
    size_t op_size = 2;
    for (const auto& arg : it->second.get_args())
      op_size += arg.get_width() / byte_to_bits;

    if (op_size == 0)
      break;

    if (pos + op_size > instr_offset) {
      result.found       = true;
      result.opcode_name = it->second.get_code_name();
      result.opcode_size = op_size;

      // Read argument values (little-endian, skip PAD args)
      size_t arg_pos = pos + 2;
      std::ostringstream args_ss;
      const char* sep = "";
      for (const auto& arg : it->second.get_args()) {
        const size_t arg_bytes = arg.get_width() / byte_to_bits;
        if (arg.get_type() == opArg::optype::PAD) {
          arg_pos += arg_bytes;
          continue;
        }
        if (arg_pos + arg_bytes > instr_size)
          break;
        uint32_t val = 0;
        for (size_t b = 0; b < arg_bytes; ++b)
          val |= static_cast<uint32_t>(static_cast<uint8_t>(instr_data[arg_pos + b])) << (b * 8);
        arg_pos += arg_bytes;
        args_ss << sep << "0x" << std::hex << val;
        sep = ", ";
      }
      result.args_str = args_ss.str();
      return result;
    }

    pos += op_size;
  }
  return result;
}

/**
 * get_opcode_information() - Decode the opcode at (uc_idx, page_idx, offset).
 *
 * Declared in aiebu_debug.h. Prefers the .dump section (richer output: source
 * file, line number) and falls back to ISA binary walk when no dump is present.
 * Returns a structured opcode_information; the caller formats and presents it.
 */
opcode_information
get_opcode_information(const ELFIO::elfio& elf, uint32_t group_id,
                       uint32_t uc_idx, uint32_t page_idx, uint32_t offset)
{
  // group_id is the ELF section index from XRT; resolve to the N used in
  // AIEBU section name suffixes (e.g. ".group.32" → 32).
  const uint32_t name_id = (group_id == UINT32_MAX)
      ? UINT32_MAX : resolve_group_name_id(elf, group_id);

  const std::string dump_section_name = (name_id == UINT32_MAX)
      ? ".dump" : ".dump." + std::to_string(name_id);
  const std::string dump_json = get_dump_json_from_elf(elf, name_id);
  if (!dump_json.empty()) {
    const opcode_information result = decode_from_dump(dump_json, uc_idx, page_idx, offset);
    if (result.found)
      return result;
  }

  return decode_opcode(elf, uc_idx, page_idx, offset, name_id);
}

} // namespace aiebu
