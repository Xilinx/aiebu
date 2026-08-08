// SPDX-License-Identifier: MIT
// Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

// opcode / source lookup by PC and page
#include "tools/debug_tools.h"
#include "aiebu/aiebu_error.h"
#include "elf/aie_elf_constants.h"
#include "specification/aie2ps/isa.h"
#include "ops/ops.h"
#include "common/writer.h"
#include "common/disassembler_state.h"
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

      // The dump "operation" field stores the full lowercased instruction text:
      // "<opcode_name> <args>" (same format as get_line() in asm_parser.h).
      // Split on the first space to separate opcode name from args.
      const std::string operation = node.get<std::string>("operation", "");
      const auto space_pos = operation.find(' ');
      if (space_pos != std::string::npos) {
        result.opcode_name = operation.substr(0, space_pos);
        result.args_str    = operation.substr(space_pos + 1);
      } else {
        result.opcode_name = operation;
      }
      result.opcode_size = std::stoull(node.get<std::string>("opcode_size", "0"), nullptr, 0);
      result.page_offset = std::stoull(node.get<std::string>("page_offset",  "0"), nullptr, 0);
      result.line        = static_cast<uint32_t>(
                             std::stoul(node.get<std::string>("line", "0"), nullptr, 0));
      result.source_file = node.get<std::string>("file", "");
      result.found       = true;
      return result;
    }
  } catch (const std::exception& e) {
    result.diag_info = std::string("dump section format error: ") + e.what();
    return result;
  }

  return result;  // found = false
}

// ─── AIEDebug implementation ────────────────────────────────────────────────

AIEDebug::AIEDebug(const ELFIO::elfio& elf) : m_elf(elf) {}

// Returns the group index N for the given "kernel:instance" by walking the ELF symtab
// and finding the SHT_GROUP section that belongs to this instance.
// The returned N is the numeric suffix of the ".group.N" section name.
// This N is the same counter used by get_section_prefix(N) = "." + std::to_string(N),
// so section names like ".ctrltext.<col>.<page>.N" can be constructed from it.
//
// Algorithm:
//   Pass 1 — find the STT_FUNC symbol whose demangled name == kernel part.
//   Pass 2 — find the STT_OBJECT symbol whose raw name == instance part AND
//             whose st_shndx == the FUNC symbol's row index.
//   Pass 3 — find the SHT_GROUP section with sh_info == instance row index;
//             parse N from ".group.N" and return it.
//
// Returns UINT32_MAX on failure (single-instance ELF or lookup failed).
uint32_t
AIEDebug::resolve_group_name_id(const std::string& kernel_name) const
{
  if (kernel_name.empty())
    return UINT32_MAX;

  // kernel_name is either "kernel:instance" or just "kernel" (no instance).
  const auto colon = kernel_name.find(':');
  const std::string filter_kernel   = (colon != std::string::npos)
      ? kernel_name.substr(0, colon) : kernel_name;
  const std::string filter_instance = (colon != std::string::npos)
      ? kernel_name.substr(colon + 1) : "";

  if (filter_kernel.empty())
    return UINT32_MAX;

  const ELFIO::section* symtab = m_elf.sections[".symtab"];
  const ELFIO::section* strtab = m_elf.sections[".strtab"];
  if (!symtab || !strtab || !symtab->get_data() || !strtab->get_data())
    return UINT32_MAX;

  const size_t sym_count   = symtab->get_size() / sizeof(ELFIO::Elf32_Sym);
  const size_t strtab_size = strtab->get_size();

  // Pass 1: find STT_FUNC symbol whose demangled name == kernel part.
  // Kernel names in ELF are always mangled (e.g. "_Z4CTRLPcPc" → "CTRL").
  ELFIO::Elf_Word kernel_sym_idx = 0;
  for (size_t i = 0; i < sym_count; ++i) {
    const auto* sym = reinterpret_cast<const ELFIO::Elf32_Sym*>(
        symtab->get_data() + i * sizeof(ELFIO::Elf32_Sym));
    if (ELF_ST_TYPE(sym->st_info) != ELFIO::STT_FUNC || sym->st_name >= strtab_size)
      continue;
    const std::string sym_name(strtab->get_data() + sym->st_name);
    if (extract_kernel_name_from_mangled(sym_name) == filter_kernel) {
      kernel_sym_idx = static_cast<ELFIO::Elf_Word>(i);
      break;
    }
  }
  if (kernel_sym_idx == 0)
    return UINT32_MAX;

  // Pass 2: find STT_OBJECT symbol with st_shndx == kernel row.
  // If an instance name was provided, also require the symbol name to match.
  ELFIO::Elf_Word instance_sym_idx = 0;
  for (size_t i = 0; i < sym_count; ++i) {
    const auto* sym = reinterpret_cast<const ELFIO::Elf32_Sym*>(
        symtab->get_data() + i * sizeof(ELFIO::Elf32_Sym));
    if (ELF_ST_TYPE(sym->st_info) != ELFIO::STT_OBJECT || sym->st_name >= strtab_size)
      continue;
    if (sym->st_shndx != kernel_sym_idx)
      continue;
    if (!filter_instance.empty()) {
      const std::string sym_name(strtab->get_data() + sym->st_name);
      if (sym_name != filter_instance)
        continue;
    }
    instance_sym_idx = static_cast<ELFIO::Elf_Word>(i);
    break;
  }
  if (instance_sym_idx == 0)
    return UINT32_MAX;

  // Pass 3: find the SHT_GROUP section with sh_info == instance row index and
  //         parse N from its ".group.N" name.
  constexpr std::string_view group_prefix = ".group.";
  for (const auto& sec_ptr : m_elf.sections) {
    const ELFIO::section* sec = sec_ptr.get();
    if (sec->get_type() != ELFIO::SHT_GROUP || sec->get_info() != instance_sym_idx)
      continue;
    const std::string& sec_name = sec->get_name();
    if (sec_name.size() <= group_prefix.size() ||
        sec_name.compare(0, group_prefix.size(), group_prefix) != 0)
      continue;
    const std::string suffix = sec_name.substr(group_prefix.size());
    if (suffix.empty() || !std::isdigit(static_cast<unsigned char>(suffix[0])))
      continue;
    return static_cast<uint32_t>(std::stoul(suffix));
  }
  return UINT32_MAX;
}

// Returns the content of the .dump section for the given instance.
// If name_id != UINT32_MAX, looks for ".dump.<name_id>" specifically.
// Otherwise returns the first ".dump"-prefixed section found (single-instance ELF).
std::string
AIEDebug::get_dump_json_from_elf(uint32_t name_id) const
{
  const std::string target_name = (name_id != UINT32_MAX)
      ? ".dump." + std::to_string(name_id)
      : "";

  constexpr std::string_view dump_prefix = ".dump";

  for (const auto& sec_ptr : m_elf.sections) {
    const ELFIO::section* sec = sec_ptr.get();
    if (sec->get_type() != ELFIO::SHT_PROGBITS)
      continue;
    const std::string& name = sec->get_name();
    if (name.size() < dump_prefix.size() ||
        name.compare(0, dump_prefix.size(), dump_prefix) != 0)
      continue;
    // With a group ELF, match only the specific ".dump.<N>" section
    if (!target_name.empty() && name != target_name)
      continue;
    if (!sec->get_data() || sec->get_size() == 0)
      continue;
    return {sec->get_data(), sec->get_size()};
  }
  return {};
}

// Decodes the opcode at (uc_idx, page_idx, offset) via ISA binary walk.
// Used as fallback when no .dump section is present.
// If name_id != UINT32_MAX, the ctrltext section name has a ".<name_id>" suffix
// (group ELF); otherwise matches by prefix alone (single-instance ELF).
opcode_information
AIEDebug::decode_opcode(uint32_t uc_idx, uint32_t page_idx,
                        uint32_t offset, uint32_t name_id) const
{
  opcode_information result{};
  const uint8_t os_abi      = m_elf.get_os_abi();
  const uint8_t abi_version = m_elf.get_abi_version();

  // Determine ELF layout from os_abi + abi_version:
  //   os_abi=0x46 (aie2ps_group), abi_version>=0x03  → per-page config ELF:
  //     one .ctrltext.<col>.<page>.<N> section per page.
  //   os_abi in {0x40,0x4B,0x56,0x69}, abi_version>=0x21 → merged-section config ELF:
  //     one .ctrltext.<col>.<N> section holds all pages concatenated.
  const bool is_merged = (os_abi != osabi_aie2ps_group) && (abi_version >= elf_version_config);

  // Build the expected section name.
  // For merged format: ".ctrltext.<uc_idx>[.<name_id>]"
  // For per-page format: ".ctrltext.<uc_idx>.<page_idx>[.<name_id>]"
  std::string expected_name = is_merged
      ? ".ctrltext." + std::to_string(uc_idx)
      : ".ctrltext." + std::to_string(uc_idx) + "." + std::to_string(page_idx);
  if (name_id != UINT32_MAX)
    expected_name += "." + std::to_string(name_id);

  // Find the matching ctrltext section by exact name (group ELF) or prefix (single-instance).
  const ELFIO::section* sec = nullptr;
  for (const auto& sec_ptr : m_elf.sections) {
    const ELFIO::section* s = sec_ptr.get();
    const std::string& name = s->get_name();
    if (name_id != UINT32_MAX) {
      // Group ELF: exact name match
      if (name == expected_name) {
        sec = s;
        break;
      }
    } else {
      // Single-instance ELF: prefix match
      // The char after the prefix must be '.' or end-of-string to avoid
      // matching ".ctrltext.<col+extra_digits>".
      if (name.size() < expected_name.size() ||
          name.compare(0, expected_name.size(), expected_name) != 0)
        continue;
      if (name.size() > expected_name.size() && name[expected_name.size()] != '.')
        continue;
      sec = s;
      break;
    }
  }

  if (!sec || !sec->get_data()) {
    result.diag_info = "section not found: " + expected_name;
    return result;
  }
  const std::string& section_name = sec->get_name();

  // If the section is compressed (SHF_COMPRESSED), decompress only this one section
  // into a local buffer. All other sections (.symtab, .strtab, .dump.*) are never
  // compressed and are read directly from ELFIO. This avoids a full ELF decompress.
  std::vector<char> decompressed_section_buf;
  const char* section_data = sec->get_data();
  std::size_t sec_size = sec->get_size();

  if (sec->get_flags() & ELFIO::SHF_COMPRESSED) {
    try {
      const std::size_t uncomp_size =
          aiebu_assembler::get_uncompressed_section_size(
              sec->get_data(), sec->get_size(),
              static_cast<unsigned char>(m_elf.get_class()));
      if (uncomp_size == 0) {
        result.diag_info = "compressed section " + section_name + " has invalid Chdr";
        return result;
      }
      decompressed_section_buf.resize(uncomp_size);
      aiebu_assembler::decompress_section_into(
          sec->get_data(), sec->get_size(),
          decompressed_section_buf.data(), uncomp_size,
          static_cast<unsigned char>(m_elf.get_class()));
      section_data = decompressed_section_buf.data();
      sec_size     = uncomp_size;
    }
    catch (const std::exception& e) {
      result.diag_info = "failed to decompress section "
                         + section_name + ": " + e.what();
      return result;
    }
  }

  // offset counts from page-slot start; bytes [0, k_binary_page_header_size) are
  // the page header — no ctrl-code instruction can reside there.
  if (offset < static_cast<uint32_t>(k_binary_page_header_size)) {
    result.diag_info = "offset " + std::to_string(offset)
                     + " is within page header of " + section_name;
    return result;
  }
  const size_t instr_offset = offset - static_cast<uint32_t>(k_binary_page_header_size);

  // instr_start : first instruction byte (absolute offset within the section).
  // region_end  : one past the last byte of the current page's region in the section.
  size_t instr_start = 0, region_end = 0;
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
  const char* instr_data = section_data + instr_start;

  isa_disassembler isa;
  const std::map<uint8_t, isa_op_disasm>* isa_map = isa.get_isa_map();

  const bool is_aie4 = (os_abi == osabi_aie4 || os_abi == osabi_aie4a || os_abi == osabi_aie4z);
  auto state = is_aie4
      ? std::static_pointer_cast<disassembler_state>(std::make_shared<disassembler_state_aie4>())
      : std::static_pointer_cast<disassembler_state>(std::make_shared<disassembler_state_aie2ps>());

  size_t pos = 0;
  while (pos < instr_size) {
    auto opbyte = static_cast<uint8_t>(instr_data[pos]);

    if (opbyte == k_align_opcode) {
      if (pos == instr_offset) {
        result.found       = true;
        result.opcode_name = "align";
        result.opcode_size = 1;
        return result;
      }
      state->increment_address(1);
      ++pos;
      continue;
    }

    const auto it = isa_map->find(opbyte);
    if (it == isa_map->end()) {
      result.diag_info += " unknown_opcode=" + ELFIO::to_hex_string(opbyte);
      return result;
    }

    // Compute instruction size directly from arg list — no heap allocation needed
    // for non-target instructions.
    size_t op_size = 2; // 1 opcode byte + 1 pad byte
    for (const auto& arg : it->second.get_args())
      op_size += arg.get_width() / byte_to_bits;

    if (pos + op_size > instr_offset) {
      // Found the target instruction — deserialize it now (only here) to get symbolic arg names.
      if (pos + op_size > instr_size) {
        result.diag_info += " truncated instruction at offset=" + std::to_string(pos);
        return result;
      }

      auto deserializer = it->second.create_deserializer();
      std::ostringstream oss;
      asm_writer writer(oss);
      try {
        deserializer->deserialize(writer, state, instr_data + pos);
      } catch (const std::exception& e) {
        result.diag_info += std::string(" deserialize error: ") + e.what();
        return result;
      }

      result.found       = true;
      result.opcode_name = it->second.get_code_name();
      result.opcode_size = op_size;

      // write_operation writes "    name\targ1, arg2\n" (current_label == label == "",
      // so no per-arg leading space; args separated by ", ").
      // Find the tab and take everything after it, then strip the trailing newline.
      const std::string line = oss.str();
      const auto tab = line.find('\t');
      if (tab != std::string::npos) {
        std::string args = line.substr(tab + 1);
        if (!args.empty() && args.back() == '\n')
          args.pop_back();
        // lowercase to match dump path format (dump stores lowercased source text)
        std::transform(args.begin(), args.end(), args.begin(),
                       [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        result.args_str = args;
      }
      return result;
    }

    // Not the target — advance past this instruction without deserializing.
    state->increment_address(static_cast<uint32_t>(op_size));
    pos += op_size;
  }
  return result;
}

/**
 * AIEDebug::get_opcode_information() - Decode the opcode at (uc_idx, page_idx, offset).
 *
 * Prefers the .dump section (richer output: source file, line number) and falls
 * back to ISA binary walk when no dump is present.
 * Returns a structured opcode_information; the caller formats and presents it.
 */
opcode_information
AIEDebug::get_opcode_information(const std::string& kernel_name,
                                  uint32_t uc_idx, uint32_t page_idx,
                                  uint32_t offset) const
{
  // Resolve the group index N for this kernel:instance.
  // UINT32_MAX → single-instance ELF (no group suffix applied).
  const uint32_t name_id = resolve_group_name_id(kernel_name);
  if (name_id == 0xffffffff)
    return {};

  const std::string dump_json = get_dump_json_from_elf(name_id);
  if (!dump_json.empty()) {
    const opcode_information result = decode_from_dump(dump_json, uc_idx, page_idx, offset);
    if (result.found)
      return result;
    // Dump present but entry not found — fall through to ISA walk and carry
    // the dump's diag_info forward so it isn't silently dropped.
    opcode_information isa_result = decode_opcode(uc_idx, page_idx, offset, name_id);
    if (!result.diag_info.empty())
      isa_result.diag_info = result.diag_info + "; " + isa_result.diag_info;
    return isa_result;
  }

  return decode_opcode(uc_idx, page_idx, offset, name_id);
}

} // namespace aiebu
