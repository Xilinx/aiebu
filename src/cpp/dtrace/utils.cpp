// SPDX-License-Identifier: MIT
// Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

#include "utils.h"
#include "tools/dwarf_reader.h"

#include <cctype>
#include <sstream>
#include <string_view>

namespace dtrace {

//-------------------------ELF Debug Map Constructor-------------------------//
/**
 * elf_debug_map() - Constructor for the elf_debug_map class.
 *
 * @param elf
 *  ELFIO elfio object.
 *
 * Initializes the elf_debug_map object with the provided ELFIO elfio object.
 */
elf_debug_map::
elf_debug_map(const ELFIO::elfio& elf)
  : m_elf(elf)
{}


//-------------------------elf_debug_map::is_group_elf-------------------------//
/**
 * is_group_elf() - Checks if the ELF file is a group ELF.
 *
 * @param elf
 *  ELFIO elfio object.
 *
 * @return
 *  True if the ELF file is a group ELF, false otherwise.
 *
 * Checks if the provided ELFIO elfio object represents a group ELF file.
 */
bool
elf_debug_map::
is_group_elf(const ELFIO::elfio& elf)
{
  const auto abi_version = static_cast<uint8_t>(elf.get_abi_version());
  constexpr uint8_t major_ver_mask = 0xF0;
  constexpr uint8_t minor_ver_mask = 0x0F;
  constexpr uint8_t shift = 4;

  const auto major = static_cast<uint8_t>((abi_version & major_ver_mask) >> shift);
  const auto minor = static_cast<uint8_t>(abi_version & minor_ver_mask);

  constexpr uint8_t group_elf_major_version = 0;
  constexpr uint8_t group_elf_minor_version = 3;

  // Version >= 0.3: major > 0 OR (major == 0 AND minor >= 3)
  return (major > group_elf_major_version) ||
         (major == group_elf_major_version && minor >= group_elf_minor_version);
}

//-------------------------elf_debug_map::extract_kernel_name_from_mangled-------------------------//
/**
 * extract_kernel_name_from_mangled() - Extracts kernel name from mangled symbol name.
 *
 * @param symbol_name
 *  Mangled symbol name.
 *
 * @return
 *  Kernel name.
 *
 * Extracts the kernel name from the provided mangled symbol name.
 * It returns the kernel name if the mangled symbol name is valid, otherwise it returns an empty string.
 */
std::string
elf_debug_map::
extract_kernel_name_from_mangled(const std::string& symbol_name)
{
  if (symbol_name.size() <= 3 || symbol_name[0] != '_' || symbol_name[1] != 'Z'
      || !std::isdigit(static_cast<unsigned char>(symbol_name[2])))
    return {};

  size_t length_end = 2;
  while (length_end < symbol_name.size()
         && std::isdigit(static_cast<unsigned char>(symbol_name[length_end])))
    ++length_end;

  const size_t name_length = std::stoul(symbol_name.substr(2, length_end - 2));
  const size_t name_start = length_end;
  if (name_start + name_length > symbol_name.size())
    return {};

  return symbol_name.substr(name_start, name_length);
}

//-------------------------elf_debug_map::get_filtered_section_indices-------------------------//
/**
 * get_filtered_section_indices() - Gets filtered section indices.
 *
 * @param kernel_instance_filter
 *  Kernel instance filter.
 *
 * Gets the filtered section indices from the provided kernel instance filter.
 */
std::set<ELFIO::Elf_Half>
elf_debug_map::
get_filtered_section_indices(const std::string& kernel_instance_filter) const
{
  const size_t delimiter_pos = kernel_instance_filter.find(':');
  if (delimiter_pos == std::string::npos)
    return {};

  const std::string filter_kernel = kernel_instance_filter.substr(0, delimiter_pos);
  const std::string filter_instance = kernel_instance_filter.substr(delimiter_pos + 1);

  const ELFIO::section* symtab = m_elf.sections[".symtab"];
  const ELFIO::section* strtab = m_elf.sections[".strtab"];
  if (!symtab || !strtab || !symtab->get_data() || !strtab->get_data())
    return {};

  const auto symtab_size = symtab->get_size();
  const auto strtab_size = strtab->get_size();
  const auto sym_count = symtab_size / sizeof(ELFIO::Elf32_Sym);

  ELFIO::Elf_Word kernel_symbol_index = 0;
  for (size_t i = 0; i < sym_count; ++i) {
    const auto* sym = reinterpret_cast<const ELFIO::Elf32_Sym*>(
        symtab->get_data() + i * sizeof(ELFIO::Elf32_Sym));
    const unsigned char sym_type = ELF_ST_TYPE(sym->st_info);
    if (sym_type != ELFIO::STT_FUNC || sym->st_name >= strtab_size)
      continue;

    const char* sym_name = strtab->get_data() + sym->st_name;
    if (extract_kernel_name_from_mangled(sym_name) == filter_kernel) {
      kernel_symbol_index = static_cast<ELFIO::Elf_Word>(i);
      break;
    }
  }
  if (kernel_symbol_index == 0)
    return {};

  ELFIO::Elf_Word instance_symbol_index = 0;
  for (size_t i = 0; i < sym_count; ++i) {
    const auto* sym = reinterpret_cast<const ELFIO::Elf32_Sym*>(
        symtab->get_data() + i * sizeof(ELFIO::Elf32_Sym));
    const unsigned char sym_type = ELF_ST_TYPE(sym->st_info);
    if (sym_type != ELFIO::STT_OBJECT || sym->st_name >= strtab_size)
      continue;

    const char* sym_name = strtab->get_data() + sym->st_name;
    if (std::string(sym_name) == filter_instance && sym->st_shndx == kernel_symbol_index) {
      instance_symbol_index = static_cast<ELFIO::Elf_Word>(i);
      break;
    }
  }
  if (instance_symbol_index == 0)
    return {};

  std::set<ELFIO::Elf_Half> section_indices;
  for (const auto& section_ptr : m_elf.sections) {
    const ELFIO::section* section = section_ptr.get();
    if (section->get_type() != ELFIO::SHT_GROUP)
      continue;
    if (section->get_info() != instance_symbol_index)
      continue;

    const auto* group_data = reinterpret_cast<const uint32_t*>(section->get_data());
    const auto group_size = section->get_size();
    const auto num_entries = group_size / sizeof(uint32_t);

    for (size_t j = 1; j < num_entries; ++j)
      section_indices.insert(static_cast<ELFIO::Elf_Half>(group_data[j]));
  }

  return section_indices;
}

// ── DWARF-to-JSON helper ─────────────────────────────────────────────────────
// Synthesizes the same JSON structure that the legacy .dump section contained,
// from the rows decoded by dwarf_reader.  The parser expects:
//
//   { "debug": [
//       { "file": "path", "operation": "", "page_index": "N", "page_offset": "N",
//         "column": "N", "line": "N" },          ← line rows (line > 0)
//       { ..., "annotation": { "id": "X" } },    ← annotation rows (annotation_id set)
//       ...
//   ]}
//
// "operation" is left empty: DWARF .debug_line does not store instruction text.
// "page_offset" is the raw byte offset within the page (not the absolute address).
static std::string dwarf_rows_to_json(const std::vector<aiebu::dwarf_debug_row>& rows)
{
  if (rows.empty())
    return {};

  // Build JSON manually to avoid pulling in nlohmann or boost::json here.
  std::ostringstream out;
  out << "{\"debug\":[";
  bool first = true;
  for (const auto& row : rows) {
    if (!first) out << ',';
    first = false;

    out << "{\"file\":\"" << row.file << "\""
        << ",\"operation\":\"\""
        << ",\"page_index\":\"" << row.page_index << "\""
        << ",\"page_offset\":\"" << row.page_offset << "\""
        << ",\"column\":\"" << row.column << "\"";

    if (row.line > 0) {
      out << ",\"line\":\"" << row.line << "\"";
    }

    if (!row.annotation_id.empty()) {
      out << ",\"annotation\":{\"id\":\"" << row.annotation_id << "\"}";
    }

    out << "}";
  }
  out << "]}";
  return out.str();
}

//-------------------------elf_debug_map::get_debug_section_json-------------------------//
/**
 * get_debug_section_json() - Gets debug section JSON.
 *
 * @return
 *  Debug section JSON.
 *
 * Returns the raw .dump section JSON for partial ELFs (single kernel, no
 * group sections). DWARF is not applicable to partial ELFs.
 */
std::string
elf_debug_map::
get_debug_section_json() const
{
  static constexpr std::string_view debug_prefix = ".dump";

  for (const auto& section_ptr : m_elf.sections) {
    const ELFIO::section* sec = section_ptr.get();
    if (sec->get_type() != ELFIO::SHT_PROGBITS)
      continue;

    const std::string& name = sec->get_name();
    if (name.size() < debug_prefix.size()
        || name.compare(0, debug_prefix.size(), debug_prefix) != 0)
      continue;

    return std::string(sec->get_data(), static_cast<size_t>(sec->get_size()));
  }

  return {};
}

//-------------------------elf_debug_map::get_debug_section_json-------------------------//
/**
 * get_debug_section_json() - Gets debug section JSON with kernel instance filter.
 *
 * @param kernel_instance_filter
 *  Kernel instance filter.
 *
 * Returns the raw .dump section JSON (group-filtered) if present; otherwise
 * synthesizes an equivalent JSON from DWARF v5 .debug_* sections.
 * For DWARF ELFs the group filter is not applied: the DWARF CU already
 * contains only the data for the specified kernel instance (the cu_name
 * DW_AT_name matches "kernel:instance").
 */
std::string
elf_debug_map::
get_debug_section_json(const std::string& kernel_instance_filter) const
{
  const auto section_indices = get_filtered_section_indices(kernel_instance_filter);

  static constexpr std::string_view debug_prefix = ".dump";

  if (!section_indices.empty()) {
    // Group ELF with .dump sections — look for the filtered one.
    for (const auto& section_ptr : m_elf.sections) {
      const ELFIO::section* sec = section_ptr.get();
      if (sec->get_type() != ELFIO::SHT_PROGBITS)
        continue;
      if (section_indices.find(sec->get_index()) == section_indices.end())
        continue;

      const std::string& name = sec->get_name();
      if (name.size() < debug_prefix.size()
          || name.compare(0, debug_prefix.size(), debug_prefix) != 0)
        continue;

      return std::string(sec->get_data(), static_cast<size_t>(sec->get_size()));
    }
  }

  // No .dump found (either no group or DWARF ELF) — try DWARF v5 .debug_* sections.
  const aiebu::dwarf_reader reader(m_elf);
  if (!reader.has_dwarf())
    return {};

  const size_t delimiter_pos = kernel_instance_filter.find(':');
  if (delimiter_pos == std::string::npos)
    return dwarf_rows_to_json(reader.get_all_rows());

  const std::string filter_kernel   = kernel_instance_filter.substr(0, delimiter_pos);
  const std::string filter_instance = kernel_instance_filter.substr(delimiter_pos + 1);

  // Filter rows to those whose CU name matches this kernel:instance.
  // CU names are stored as "mangled_kernel:instance" (e.g. "_Z3DPUPcPcPcPc:subgraph_0");
  // we demangle the kernel prefix for comparison.
  std::vector<aiebu::dwarf_debug_row> filtered;
  for (const auto& row : reader.get_all_rows()) {
    const size_t cu_delim = row.cu_name.find(':');
    if (cu_delim == std::string::npos) continue;
    const std::string cu_kernel   = row.cu_name.substr(0, cu_delim);
    const std::string cu_instance = row.cu_name.substr(cu_delim + 1);
    const std::string demangled   = extract_kernel_name_from_mangled(cu_kernel);
    const bool kernel_match = (demangled == filter_kernel) || (cu_kernel == filter_kernel);
    if (kernel_match && cu_instance == filter_instance)
      filtered.push_back(row);
  }
  return dwarf_rows_to_json(filtered);
}

} // namespace dtrace
