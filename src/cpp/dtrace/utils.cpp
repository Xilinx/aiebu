// SPDX-License-Identifier: MIT
// Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

#include "utils.h"
#include "aiebu/aiebu_debug.h"
#include "tools/dwarf_reader.h"
#include "json/nlohmann/json.hpp"

#include <cctype>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string_view>
#include <utility>

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
get_filtered_section_indices(const std::string& filter_kernel,
                             const std::string& filter_instance) const
{
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

  // When no instance is specified, accept the first instance but error out
  // if a second one is found. empty kernel instance is not allowed if there
  // are multiple kernel instances
  ELFIO::Elf_Word instance_symbol_index = 0;
  for (size_t i = 0; i < sym_count; ++i) {
    const auto* sym = reinterpret_cast<const ELFIO::Elf32_Sym*>(
        symtab->get_data() + i * sizeof(ELFIO::Elf32_Sym));
    const unsigned char sym_type = ELF_ST_TYPE(sym->st_info);
    if (sym_type != ELFIO::STT_OBJECT || sym->st_name >= strtab_size)
      continue;

    const char* sym_name = strtab->get_data() + sym->st_name;
    if (sym->st_shndx != kernel_symbol_index)
      continue;

    if (!filter_instance.empty()) {
      if (std::string(sym_name) == filter_instance) {
        instance_symbol_index = static_cast<ELFIO::Elf_Word>(i);
        break;  // exact match — no need to scan further
      }
      continue;
    }

    // filter_instance is empty: accept first matching instance, error on second distinct one
    if (instance_symbol_index == 0)
      instance_symbol_index = static_cast<ELFIO::Elf_Word>(i);
    else
      DTRACE_ERROR("DTRACE_KERNEL_INSTANCE_FILTER_FAILED", "kernel '" << filter_kernel
                   << "' has multiple instances; specify 'kernel:instance' for dtrace");
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

//-------------------------elf_debug_map::parse_kernel_instance-------------------------//
/**
 * parse_kernel_instance() - Parses the kernel instance from the provided filter.
 *
 * @param filter
 *  Filter string containing the kernel instance.
 * @param kernel
 *  Kernel name.
 * @param instance
 *  Instance name.
 *
 * Parses the kernel instance from the provided filter.
 * It returns the kernel name and instance name if the filter is valid, otherwise it returns an empty string.
 */
void
elf_debug_map::
parse_kernel_instance(const std::string& filter, std::string& kernel, std::string& instance)
{
  const size_t pos = filter.find(':');
  if (pos == std::string::npos) {
    kernel   = filter;
    instance = {};
  } else {
    kernel   = filter.substr(0, pos);
    instance = filter.substr(pos + 1);
  }
}

//-------------------------elf_debug_map::find_dump_section-------------------------//
/**
 * find_dump_section() - Finds the dump section from the provided section indices.
 *
 * @param section_indices
 *  Section indices.
 *
 * Finds the dump section from the provided section indices.
 * It returns the dump section if it is found, otherwise it returns nullptr.
 */
const ELFIO::section*
elf_debug_map::
find_dump_section(const std::set<ELFIO::Elf_Half>& section_indices) const
{
  static constexpr std::string_view dump_prefix = ".dump";
  for (const auto& sec_ptr : m_elf.sections) {
    const ELFIO::section* sec = sec_ptr.get();
    if (sec->get_type() != ELFIO::SHT_PROGBITS)
      continue;

    if (section_indices.find(sec->get_index()) == section_indices.end())
      continue;

    const std::string& name = sec->get_name();
    if (name.size() >= dump_prefix.size() &&
        name.compare(0, dump_prefix.size(), dump_prefix) == 0)
      return sec;

  }
  return nullptr;
}

//-------------------------elf_debug_map::get_filtered_dwarf_rows-------------------------//
/**
 * get_filtered_dwarf_rows() - Gets the filtered dwarf rows from the provided filter.
 *
 * @param filter_kernel
 *  Filter kernel name.
 * @param filter_instance
 *  Filter instance name.
 *
 * Gets the filtered dwarf rows from the provided filter.
 * It returns the filtered rows and the reader if the filter is valid, otherwise it returns an empty pair.
 */
std::pair<std::vector<aiebu::dwarf_debug_row>, std::unique_ptr<aiebu::dwarf_reader>>
elf_debug_map::
get_filtered_dwarf_rows(const std::string& filter_kernel,
                        const std::string& filter_instance) const
{
  auto reader = std::make_unique<aiebu::dwarf_reader>(m_elf);
  if (!reader->has_dwarf())
    return {};

  // First pass: determine the matching CU name indices — O(unique CUs)
  // Demangling and substr are only called once per unique CU name
  std::set<uint32_t> matching_cu_indices;
  std::set<uint32_t> seen_cu_indices;
  std::string first_instance;
  for (const auto& row : reader->get_all_rows()) {
    if (row.cu_name_idx == UINT32_MAX) 
      continue;

    if (!seen_cu_indices.insert(row.cu_name_idx).second) 
      continue;

    const std::string& cu_name = reader->cu_name_of(row);
    const size_t cu_delim = cu_name.find(':');
    if (cu_delim == std::string::npos) 
      continue;

    const std::string cu_kernel   = cu_name.substr(0, cu_delim);
    const std::string cu_instance = cu_name.substr(cu_delim + 1);
    const std::string demangled   = extract_kernel_name_from_mangled(cu_kernel);
    if (demangled != filter_kernel && cu_kernel != filter_kernel)
      continue;

    if (!filter_instance.empty()) {
      if (cu_instance == filter_instance)
        matching_cu_indices.insert(row.cu_name_idx);
      continue;
    }

    // No instance specified: accept first matching instance, error on second distinct.
    if (first_instance.empty()) {
      first_instance = cu_instance;
      matching_cu_indices.insert(row.cu_name_idx);
    } else if (cu_instance != first_instance) {
      DTRACE_ERROR("DTRACE_KERNEL_INSTANCE_FILTER_FAILED", "kernel '" << filter_kernel
                   << "' has multiple instances; specify 'kernel:instance' for dtrace");
    }
  }

  // Second pass: collect all rows from matching CUs.
  std::vector<aiebu::dwarf_debug_row> filtered;
  for (const auto& row : reader->get_all_rows())
    if (matching_cu_indices.count(row.cu_name_idx))
      filtered.push_back(row);

  // m_rows is no longer needed — the reader is returned only so callers can pass
  reader->release_rows();

  return {std::move(filtered), std::move(reader)};
}

//-------------------------elf_debug_map::parse_debug_json-------------------------//
/**
 * parse_debug_json() - Parses a .dump JSON string into a debug_map_t.
 *
 * @param json_str
 *  JSON string containing the debug map.
 *
 * Parses the .dump JSON string into a debug_map_t.
 * It returns the debug_map_t if the JSON string is valid, otherwise it returns an empty debug_map_t.
 */
static 
debug_map_t 
parse_debug_json(const std::string& json_str)
{
  if (json_str.empty())
    return {};

  auto pt = nlohmann::json::parse(json_str);
  if (!pt.contains("debug") || !pt["debug"].is_array())
    return {};

  debug_map_t result;
  result.reserve(pt["debug"].size());
  for (const auto& item : pt["debug"]) {
    if (!item.contains("line"))
      continue;

    debug_entry entry;
    entry.file      = item.value("file",      std::string{});
    entry.operation = item.value("operation", std::string{});

    // All .dump numeric fields are bounded by the 32-bit AIE ELF layout
    // Guard against malformed sections where a value exceeds uint32_t range.
    auto to_u32 = [](const nlohmann::json& json_item) -> uint32_t {
      const std::string str_item = json_item.is_string() ? json_item.get<std::string>() : json_item.dump();
      const auto num_item = std::stoull(str_item);
      if (num_item > std::numeric_limits<uint32_t>::max())
        throw std::out_of_range("debug map field exceeds uint32_t range: " + str_item);

      return static_cast<uint32_t>(num_item);
    };

    entry.page_index  = to_u32(item.at("page_index"));
    entry.page_offset = to_u32(item.at("page_offset"));
    entry.column      = to_u32(item.at("column"));
    entry.line        = to_u32(item.at("line"));

    if (item.contains("annotation") && item["annotation"].contains("id")) {
      const auto& id = item["annotation"]["id"];
      entry.annotation_id = id.is_string() ? id.get<std::string>() : id.dump();
    }
    result.push_back(std::move(entry));
  }
  return result;
}

//-------------------------elf_debug_map::build_debug_map_from_dwarf-------------------------//
/**
 * build_debug_map_from_dwarf() - Builds a debug_map_t directly from DWARF rows.
 *
 * @param rows
 *  Dwarf rows.
 * @param elf
 *  ELFIO elfio object.
 * @param kernel_instance
 *  Kernel instance.
 * @param shared_dwarf
 *  Shared dwarf reader.
 *
 * Builds a debug_map_t directly from DWARF rows.
 * It returns the debug_map_t if the rows are valid, otherwise it returns an empty debug_map_t.
 */
static 
debug_map_t 
build_debug_map_from_dwarf(const std::vector<aiebu::dwarf_debug_row>& rows,
  const ELFIO::elfio& elf, const std::string& kernel_instance,
  const aiebu::dwarf_reader* shared_dwarf)
{
  if (rows.empty())
    return {};

  const aiebu::AIEDebug aie_debug(elf, shared_dwarf);
  const uint32_t name_id = aie_debug.resolve_group_name_id(kernel_instance);

  // Annotation rows (line == 0) share addresses with line rows; collect them
  // first so each debug_entry can include the annotation_id inline.
  using col_addr = std::pair<uint32_t, uint32_t>; // {column, address}
  std::map<col_addr, std::string> annotation;
  for (const auto& row : rows) {
    if (row.line == 0 && !row.annotation_id.empty())
    annotation.emplace(col_addr{row.column, row.address}, row.annotation_id);
  }

  debug_map_t result;
  result.reserve(rows.size());
  for (const auto& row : rows) {
    if (row.line == 0)
      continue;

    debug_entry entry;
    entry.file        = row.file;
    entry.column      = row.column;
    entry.line        = row.line;
    entry.page_index  = row.page_index;
    entry.page_offset = row.address; // absolute offset — matches .dump page_offset convention
    entry.operation   = aie_debug.format_operation(name_id, row.column, row.page_index, row.page_offset);

    const auto annotation_it = annotation.find({row.column, row.address});
    if (annotation_it != annotation.end())
      entry.annotation_id = annotation_it->second;

    result.push_back(std::move(entry));
  }
  return result;
}

//-------------------------elf_debug_map::get_debug_map-------------------------//
/**
 * get_debug_map() - Gets the debug map from the provided elfio object.
 *
 * @return
 *  Debug map.
 *
 * Gets the debug map from the provided elfio object.
 * It returns the debug map if the elfio object is valid, otherwise it returns an empty debug_map_t.
 */
debug_map_t
elf_debug_map::
get_debug_map() const
{
  static constexpr std::string_view dump_prefix = ".dump";
  for (const auto& sec_ptr : m_elf.sections) {
    const ELFIO::section* sec = sec_ptr.get();
    if (sec->get_type() != ELFIO::SHT_PROGBITS)
      continue;

    const std::string& name = sec->get_name();
    if (name.size() >= dump_prefix.size() &&
        name.compare(0, dump_prefix.size(), dump_prefix) == 0)
      return parse_debug_json({sec->get_data(), static_cast<size_t>(sec->get_size())});
  }
  return {};
}

//-------------------------elf_debug_map::get_debug_map-------------------------//
/**
 * get_debug_map() - Gets the debug map from the provided kernel instance filter.
 *
 * @param kernel_instance_filter
 *  Kernel instance filter.
 *
 * Gets the debug map from the provided kernel instance filter.
 * It returns the debug map if the kernel instance filter is valid, otherwise it returns an empty debug_map_t.
 */
debug_map_t
elf_debug_map::
get_debug_map(const std::string& kernel_instance_filter) const
{
  std::string filter_kernel, filter_instance;
  parse_kernel_instance(kernel_instance_filter, filter_kernel, filter_instance);

  const auto section_indices = get_filtered_section_indices(filter_kernel, filter_instance);
  if (!section_indices.empty()) {
    const ELFIO::section* sec = find_dump_section(section_indices);
    if (sec)
      return parse_debug_json({sec->get_data(), static_cast<size_t>(sec->get_size())});
  }

  // No .dump found — build debug_map_t directly from DWARF v5 rows (no JSON).
  auto [filtered, reader] = get_filtered_dwarf_rows(filter_kernel, filter_instance);
  if (filtered.empty())
    return {};

  return build_debug_map_from_dwarf(filtered, m_elf, kernel_instance_filter, reader.get());
}

//-------------------------strip-------------------------//
/**
 * strip() - Removes leading and trailing whitespace characters, including spaces,
 *  newlines, carriage returns, and tabs, from a given string.
 *
 * @param token: Input string.
 * @return: Updated string or
 *          empty string if the input string contains only whitespace characters
 */
std::string
strip(const std::string& token)
{
  size_t first = token.find_first_not_of(" \n\r\t");
  if (first == std::string::npos)
    return "";

  size_t last = token.find_last_not_of(" \n\r\t");
  return token.substr(first, last - first + 1);
}

//-------------------------getline-------------------------//
/**
 * getline() - Splits a token on a delimiter and strips every field.
 *
 * @param token
 *  Token to split.
 * @param delimiter
 * @param fields
 *  Stripped fields of the token.
 */
void
getline(const std::string& token, char delimiter, std::vector<std::string>& fields)
{
  fields.clear();
  size_t field_start = 0;
  while (field_start < token.size())
  {
    size_t field_end = token.find(delimiter, field_start);
    if (field_end == std::string::npos)
      field_end = token.size();

    fields.push_back(strip(token.substr(field_start, field_end - field_start)));
    field_start = field_end + 1;
  }
}

//-------------------------match-------------------------//
/**
 * match() - Splits an action call into action name and arguments.
 *
 * @param token
 *  Action call to match: name(arguments)
 * @param name
 *  Action name of the call.
 * @param arguments
 *  Argument string of the call, without the brackets.
 * @return
 *  true if the token has the form name(arguments).
 */
bool
match(const std::string& token, std::string& name, std::string& arguments)
{
  if (token.empty() || token.back() != ')')
    return false;

  size_t open_bracket = token.find('(');
  if (open_bracket == 0 || open_bracket == std::string::npos)
    return false;

  name = token.substr(0, open_bracket);
  arguments = token.substr(open_bracket + 1, token.size() - open_bracket - 2);
  return true;
}

} // namespace dtrace
