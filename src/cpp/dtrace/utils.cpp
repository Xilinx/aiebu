// SPDX-License-Identifier: MIT
// Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

#include "utils.h"

#include <cctype>
#include <string_view>

namespace dtrace {

//-------------------------ELF Dump Map Constructor-------------------------//
/**
 * elf_dump_map() - Constructor for the elf_dump_map class.
 *
 * @param map_data
 *  ELFIO elfio object.
 *
 * Initializes the elf_dump_map object with the provided ELFIO elfio object.
 */
elf_dump_map::
elf_dump_map(const ELFIO::elfio& elf)
  : m_elf(elf)
{}

//-------------------------elf_dump_map::extract_kernel_name_from_mangled-------------------------//
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
elf_dump_map::
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

//-------------------------elf_dump_map::get_filtered_section_indices-------------------------//
/**
 * get_filtered_section_indices() - Gets filtered section indices.
 *
 * @param kernel_instance_filter
 *  Kernel instance filter.
 *
 * Gets the filtered section indices from the provided kernel instance filter.
 */
std::set<ELFIO::Elf_Half>
elf_dump_map::
get_filtered_section_indices(const std::string& kernel_instance_filter) const
{
  const size_t delimiter_pos = kernel_instance_filter.find(':');
  if (delimiter_pos == std::string::npos)
    return {};

  const std::string filter_kernel = kernel_instance_filter.substr(0, delimiter_pos);
  const std::string filter_instance = kernel_instance_filter.substr(delimiter_pos + 1);

  const ELFIO::section* symtab = m_elf.sections[".symtab"];
  const ELFIO::section* strtab = m_elf.sections[".strtab"];
  if (!symtab || !strtab)
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

//-------------------------elf_dump_map::get_dump_section_json-------------------------//
/**
 * get_dump_section_json() - Gets dump section JSON.
 *
 * @return
 *  Dump section JSON.
 *
 * Gets the dump section JSON from the provided ELFIO elfio object.
 */
std::string
elf_dump_map::
get_dump_section_json() const
{
  static constexpr std::string_view dump_prefix = ".dump";

  for (const auto& section_ptr : m_elf.sections) {
    const ELFIO::section* sec = section_ptr.get();
    if (sec->get_type() != ELFIO::SHT_PROGBITS)
      continue;

    const std::string& name = sec->get_name();
    if (name.size() < dump_prefix.size()
        || name.compare(0, dump_prefix.size(), dump_prefix) != 0)
      continue;

    return std::string(sec->get_data(), static_cast<size_t>(sec->get_size()));
  }

  return {};
}

//-------------------------elf_dump_map::get_dump_section_json-------------------------//
/**
 * get_dump_section_json() - Gets dump section JSON with kernel instance filter.
 *
 * @param kernel_instance_filter
 *  Kernel instance filter.
 *
 * Gets the dump section JSON with kernel instance filter from the provided ELFIO elfio object.
 */
std::string
elf_dump_map::
get_dump_section_json(const std::string& kernel_instance_filter) const
{
  const auto section_indices = get_filtered_section_indices(kernel_instance_filter);
  if (section_indices.empty())
    return {};

  static constexpr std::string_view dump_prefix = ".dump";

  for (const auto& section_ptr : m_elf.sections) {
    const ELFIO::section* sec = section_ptr.get();
    if (sec->get_type() != ELFIO::SHT_PROGBITS)
      continue;
    if (section_indices.find(sec->get_index()) == section_indices.end())
      continue;

    const std::string& name = sec->get_name();
    if (name.size() < dump_prefix.size()
        || name.compare(0, dump_prefix.size(), dump_prefix) != 0)
      continue;

    return std::string(sec->get_data(), static_cast<size_t>(sec->get_size()));
  }

  return {};
}

} // namespace dtrace
