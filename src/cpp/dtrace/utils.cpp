// SPDX-License-Identifier: MIT
// Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

#include "utils.h"
#include "aiebu/aiebu_debug.h"
#include "tools/dwarf_reader.h"

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

// ── DWARF-to-JSON helper ─────────────────────────────────────────────────────
// Synthesizes the same JSON structure that the legacy .dump section contained,
// from the rows decoded by dwarf_reader.  The parser expects:
//
//   { "debug": [
//       { "file": "path", "operation": "start_job 0", "page_index": "N", "page_offset": "N",
//         "column": "N", "line": "N" },          ← line rows (line > 0)
//       { ..., "annotation": { "id": "X" } },    ← annotation rows (annotation_id set)
//       ...
//   ]}
//
// "operation" is decoded via ISA binary walk (format_operation) so that the
// dtrace `operation` built-in variable is populated with the real mnemonic.
// "page_offset" is the absolute byte offset from the start of the .ctrltext section
// (page_index * PAGE_SIZE + relative_offset), matching the .dump convention.
// The dtrace parser subtracts page_index * page_size to recover the relative offset.
static std::string dwarf_rows_to_json(const std::vector<aiebu::dwarf_debug_row>& rows,
                                      const ELFIO::elfio& elf,
                                      const std::string& kernel_instance)
{
  if (rows.empty())
    return {};

  // Decode operations once per unique (column, address) pair to avoid redundant
  // ISA walks. Column is part of the key because DWARF addresses (page*PAGE_SIZE
  // + offset) are only unique within a single control column.
  // resolve_group_name_id() is called once; format_operation() once per pair.
  const aiebu::AIEDebug aie_debug(elf);
  const uint32_t name_id = aie_debug.resolve_group_name_id(kernel_instance);
  std::map<std::pair<uint32_t, uint32_t>, std::string> addr_to_op; // {col, addr} → op
  // Annotation rows (line == 0) share their address with a co-located line row.
  // Build a lookup so the annotation id can be merged into the line row's JSON
  // entry — matching the .dump convention that the dtrace parser expects.
  std::map<std::pair<uint32_t, uint32_t>, std::string> addr_to_ann; // {col, addr} → id
  for (const auto& row : rows) {
    if (row.line == 0) {
      if (!row.annotation_id.empty())
        addr_to_ann.emplace(std::make_pair(row.column, row.address), row.annotation_id);
      continue; // annotation rows share address with their enclosing line row
    }
    const auto cache_key = std::make_pair(row.column, row.address);
    if (addr_to_op.find(cache_key) == addr_to_op.end())
      addr_to_op.emplace(cache_key,
                         aie_debug.format_operation(name_id, row.column,
                                                    row.page_index, row.page_offset));
  }

  // Build JSON manually to avoid pulling in nlohmann or boost::json here.
  // Annotation-only rows (line == 0) are suppressed; their annotation_id is
  // merged into the co-located line row instead, matching the .dump format.
  std::ostringstream out;
  out << R"({"debug":[)";
  bool first = true;
  for (const auto& row : rows) {
    if (row.line == 0)
      continue; // merged into co-located line row below

    if (!first)
      out << ',';
    first = false;

    const auto op_it = addr_to_op.find({row.column, row.address});
    const std::string op = (op_it != addr_to_op.end()) ? op_it->second : std::string{};

    out << R"({"file":")" << row.file << '"'
        << R"(,"operation":")" << op << '"'
        << R"(,"page_index":")" << row.page_index << '"'
        << R"(,"page_offset":")" << row.address << '"'
        << R"(,"column":")" << row.column << '"'
        << R"(,"line":")" << row.line << '"';

    const auto ann_it = addr_to_ann.find({row.column, row.address});
    if (ann_it != addr_to_ann.end())
      out << R"(,"annotation":{"id":")" << ann_it->second << R"("})";

    out << '}';
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

    return {sec->get_data(), static_cast<size_t>(sec->get_size())};
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
 * @kernel_instance_filter may be "kernel:instance" or just "kernel" for
 * single-instance kernels; in the latter case the first matching instance
 * is accepted for both .dump and DWARF paths.
 */
std::string
elf_debug_map::
get_debug_section_json(const std::string& kernel_instance_filter) const
{
  // Parse once; pass pre-parsed values to both the .dump and DWARF paths.
  // If no ':' is present, treat the entire string as the kernel name and accept
  // the first matching instance — supports single-instance callers passing "DPU" vs "DPU:dpu".
  const size_t delimiter_pos = kernel_instance_filter.find(':');
  const std::string filter_kernel   = (delimiter_pos == std::string::npos)
      ? kernel_instance_filter
      : kernel_instance_filter.substr(0, delimiter_pos);
  const std::string filter_instance = (delimiter_pos == std::string::npos)
      ? std::string{}
      : kernel_instance_filter.substr(delimiter_pos + 1);

  const auto section_indices = get_filtered_section_indices(filter_kernel, filter_instance);

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

      return {sec->get_data(), static_cast<size_t>(sec->get_size())};
    }
  }

  // No .dump found (either no group or DWARF ELF) — try DWARF v5 .debug_* sections.
  const aiebu::dwarf_reader reader(m_elf);
  if (!reader.has_dwarf())
    return {};

  // Filter rows to those whose CU name matches this kernel(:instance).
  // CU names are stored as "mangled_kernel:instance" (e.g. "_Z3DPUPcPcPcPc:subgraph_0");
  // we demangle the kernel prefix for comparison.
  //
  // First pass: determine the matching CU name indices — O(unique CUs), not O(rows).
  // Demangling and substr are only called once per unique CU name, avoiding
  // repeated work when many rows share the same CU name (e.g. 29-subgraph ELFs).
  // The intern index (uint32_t) is used as the set key to avoid string copies.
  std::set<uint32_t> matching_cu_indices;
  std::set<uint32_t> seen_cu_indices;   // tracks which indices have already been evaluated
  std::string first_instance;
  for (const auto& row : reader.get_all_rows()) {
    if (row.cu_name_idx == UINT32_MAX) continue;
    if (!seen_cu_indices.insert(row.cu_name_idx).second)
      continue; // already evaluated this CU index
    const std::string& cu_name = reader.cu_name_of(row);
    const size_t cu_delim = cu_name.find(':');
    if (cu_delim == std::string::npos) continue;
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
    // No instance specified: accept first matching instance, error on second distinct
    if (first_instance.empty()) {
      first_instance = cu_instance;
      matching_cu_indices.insert(row.cu_name_idx);
    } else if (cu_instance != first_instance) {
      DTRACE_ERROR("DTRACE_KERNEL_INSTANCE_FILTER_FAILED", "kernel '" << filter_kernel
                   << "' has multiple instances; specify 'kernel:instance' for dtrace");
    }
  }

  // Second pass: collect all rows from matching CUs (integer set lookup per row).
  std::vector<aiebu::dwarf_debug_row> filtered;
  for (const auto& row : reader.get_all_rows())
    if (matching_cu_indices.count(row.cu_name_idx))
      filtered.push_back(row);

  return dwarf_rows_to_json(filtered, m_elf, kernel_instance_filter);
}

//-------------------------strip-------------------------//
/**
 * strip() - Removes leading and trailing whitespace characters, including spaces,
 *  newlines, carriage returns, and tabs, from a given string.
 *
 * @param token
 *  Input string.
 * @return
 *  Updated string with leading and trailing whitespace removed. If the input string
 *  contains only whitespace characters, an empty string is returned.
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
