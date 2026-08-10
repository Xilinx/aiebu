// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

#include "aiebu/elf.h"
#include "elf/aie_elf_constants.h"

#include "elfio/elfio.hpp"

#include <boost/interprocess/streams/bufferstream.hpp>

#include <array>
#include <cstdint>
#include <cstring>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace aiebu {

// Sentinel ctrl-code id for legacy ELFs with no .group sections
static constexpr uint32_t no_ctrl_code_id = UINT32_MAX;

// Section name patterns indexed by elf::buf_type
static constexpr std::array<std::string_view, 9> section_name_patterns = {
  ".ctrltext",       // ctrltext
  ".ctrldata",       // ctrldata
  ".preempt_save",   // preempt_save
  ".preempt_restore",// preempt_restore
  ".pdi",            // pdi
  ".ctrlpkt.pm",     // ctrlpkt_pm
  ".pad",            // pad
  ".dump",           // dump
  ".ctrlpkt"         // ctrlpkt
};

static constexpr std::string_view
section_pattern(elf::buf_type t)
{
  return section_name_patterns[static_cast<uint32_t>(t)];
}

// Generate the same key string that xrt_core::elf_patcher::generate_key_string produces,
// so xrt::elf_impl can look up patch points without change.
static std::string
make_key(const std::string& arg_name, elf::buf_type t)
{
  return arg_name + std::to_string(static_cast<uint32_t>(t));
}

// Strip trailing ".<group_id>" suffix from a section name to recover the symbol name
// used as the ctrlpkt identifier (mirrors get_symbol_name_from_section_name).
static std::string
strip_group_suffix(const std::string& section_name)
{
  auto pos = section_name.rfind('.');
  if (pos == std::string::npos)
    return section_name;
  // Only strip if the suffix is a decimal integer
  auto suffix = section_name.substr(pos + 1);
  if (!suffix.empty() && suffix.find_first_not_of("0123456789") == std::string::npos)
    return section_name.substr(0, pos);
  return section_name;
}

////////////////////////////////////////////////////////////////
// Kernel signature parsing helpers
// Ported from xrt_elf.cpp anonymous namespace
////////////////////////////////////////////////////////////////

static constexpr size_t mangled_prefix_length = 2;

static std::vector<std::string>
split(const std::string& s, char delim)
{
  std::vector<std::string> tokens;
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim))
    tokens.push_back(item);
  return tokens;
}

static std::string
get_demangle_type(char c)
{
  switch (c) {
  case 'v': return "void";
  case 'c': return "char";
  case 'i': return "int";
  default:
    throw std::runtime_error("Unknown type character in mangled name: " + std::string(1, c));
  }
}

static std::string
demangle(const std::string& mangled)
{
  if (mangled.size() <= mangled_prefix_length || mangled.substr(0, mangled_prefix_length) != "_Z")
    throw std::runtime_error("Doesn't have prefix _Z, not a mangled kernel name");

  size_t idx = 2;
  size_t len = 0;
  while (idx < mangled.size() && std::isdigit(mangled[idx]))
    len = len * 10 + (mangled[idx++] - '0');

  if (idx + len > mangled.size())
    throw std::runtime_error("Invalid mangled name, doesn't have expected kernel name length");

  std::string name = mangled.substr(idx, len);
  idx += len;

  std::vector<std::string> args;
  while (idx < mangled.size()) {
    int depth = 0;
    while (idx < mangled.size() && mangled[idx] == 'P') { ++depth; ++idx; }
    if (idx >= mangled.size())
      throw std::runtime_error("demangle arg index out of bounds");
    std::string type = get_demangle_type(mangled[idx++]);
    for (int i = 0; i < depth; ++i)
      type += "*";
    args.push_back(type);
  }

  std::string result = name + "(";
  for (size_t i = 0; i < args.size(); ++i) {
    if (i > 0) result += ", ";
    result += args[i];
  }
  result += ")";
  return result;
}

static std::string
extract_kernel_name(const std::string& signature)
{
  auto pos = signature.find('(');
  return (pos == std::string::npos) ? signature : signature.substr(0, pos);
}

static std::vector<elf::arg>
construct_kernel_args(const std::string& signature)
{
  std::vector<elf::arg> args;
  auto start = signature.find('(');
  if (start == std::string::npos)
    return args;
  auto end = signature.find(')', start);
  if (end == std::string::npos || start > end)
    throw std::runtime_error("Failed to construct kernel args");

  auto argstring = signature.substr(start + 1, end - start - 1);
  auto argstrings = split(argstring, ',');

  uint32_t idx = 0;
  for (const auto& s : argstrings) {
    bool is_global = (s.find('*') != std::string::npos);
    if (!is_global)
      throw std::runtime_error("scalar args are not yet supported for this kind of kernel");
    elf::arg a;
    a.name = "argv" + std::to_string(idx);
    a.data_type = s;
    a.index = idx;
    a.is_global = true;
    args.push_back(std::move(a));
    ++idx;
  }
  return args;
}

////////////////////////////////////////////////////////////////
// Column/page extraction for gen2plus section names
////////////////////////////////////////////////////////////////

static std::pair<uint32_t, uint32_t>
get_column_and_page(const std::string& name, bool is_merged)
{
  constexpr size_t col_token_id  = 1;
  constexpr size_t page_token_id = 2;

  std::vector<std::string> tokens;
  std::stringstream ss(name);
  std::string token;
  while (std::getline(ss, token, '.')) {
    if (!token.empty())
      tokens.emplace_back(std::move(token));
  }

  try {
    if (tokens.size() <= col_token_id)
      return {0, 0};
    if (tokens.size() == col_token_id + 1)
      return {std::stoul(tokens[col_token_id]), 0};
    return {std::stoul(tokens[col_token_id]),
            is_merged ? 0 : std::stoul(tokens[page_token_id])};
  }
  catch (const std::exception&) {
    throw std::runtime_error("Invalid section name passed to parse col or page index");
  }
}

////////////////////////////////////////////////////////////////
// Owning buffer: zero-copy views over ELFIO section data,
// plus an optional padding allocation.
// Mirrors xrt::buf from elf_int.h so no XRT dependency is needed.
////////////////////////////////////////////////////////////////
struct section_buf
{
  std::vector<std::string_view>  views;
  std::vector<uint8_t>           padding;

  void
  append(const ELFIO::section* sec)
  {
    if (sec && sec->get_size() > 0)
      views.emplace_back(sec->get_data(), sec->get_size());
  }

  void
  pad_to(size_t target)
  {
    size_t current = size();
    if (target <= current)
      return;
    size_t n = target - current;
    padding.insert(padding.end(), n, 0);
    views.emplace_back(reinterpret_cast<const char*>(padding.data()), n);
  }

  size_t
  size() const
  {
    size_t total = 0;
    for (const auto& v : views) total += v.size();
    return total;
  }

  // Copy to a contiguous byte buffer
  std::vector<std::byte>
  to_bytes() const
  {
    std::vector<std::byte> out;
    out.reserve(size());
    for (const auto& v : views)
      for (char c : v)
        out.push_back(static_cast<std::byte>(c));
    return out;
  }
};

////////////////////////////////////////////////////////////////
// elf_reader — the actual implementation hidden behind elf's pimpl
////////////////////////////////////////////////////////////////
class elf_reader
{
public:
  ELFIO::elfio m_elfio;
  elf::platform m_platform;
  std::string m_path;

  // Group / section maps (same semantics as xrt::elf_impl)
  std::map<uint32_t, uint32_t>              m_section_to_group_map;
  std::map<uint32_t, std::vector<uint32_t>> m_group_to_sections_map;
  std::map<std::string, uint32_t>           m_kernel_name_to_id_map;
  std::map<std::string, std::vector<std::string>> m_kernel_to_subkernels_map;

  // Parsed kernels
  std::vector<elf::kernel>       m_kernels;
  // Working map cleared after parse_sections
  std::map<std::string, std::vector<elf::arg>> m_kernel_args_map;

  // Custom sections (type SHT_LOUSER+1)
  std::map<std::string, std::span<const std::byte>> m_custom_section_map;

  // Patch points grouped by ctrl-code-id then by key-string
  // key-string = arg_name + to_string(buf_type)
  std::map<uint32_t, std::map<std::string, std::vector<elf::patch_point>>>
    m_patch_points;

  // Addend encoding constants (same as xrt::elf_impl)
  static constexpr uint32_t addend_shift = 4;
  static constexpr uint32_t addend_mask  = ~((uint32_t)0) << addend_shift;
  static constexpr uint32_t schema_mask  = ~addend_mask;

  // AIE gen2 (aie2p) specific
  std::map<uint32_t, section_buf> m_instr_buf_map;
  std::map<uint32_t, section_buf> m_ctrl_packet_map;
  std::map<uint32_t, section_buf> m_save_buf_map;
  std::map<uint32_t, section_buf> m_restore_buf_map;
  std::map<std::string, section_buf> m_pdi_buf_map;
  std::map<uint32_t, std::unordered_set<std::string>> m_ctrl_pdi_map;
  std::map<std::string, section_buf> m_ctrlpkt_pm_bufs;
  std::set<std::string>           m_ctrlpkt_pm_dynsyms;
  size_t                          m_ctrl_scratch_pad_mem_size = 0;
  bool                            m_preemption_exist = false;

  // AIE gen2plus (aie2ps/aie4) specific
  static constexpr size_t elf_page_size = 8192;
  std::map<uint32_t, std::vector<section_buf>> m_ctrlcodes_map;
  std::map<uint32_t, std::map<std::string, section_buf>> m_ctrlpkt_buf_map;
  std::map<uint32_t, section_buf> m_dump_buf_map;

  // Materialised byte buffers for zero-copy span returns.
  // Keyed by section name or symbol name.
  mutable std::unordered_map<std::string, std::vector<std::byte>> m_byte_cache;

  ////////////////////////////////////////////////////////////////
  // Construction helpers
  ////////////////////////////////////////////////////////////////

  static ELFIO::elfio
  load(const std::string& fnm)
  {
    ELFIO::elfio e;
    if (!e.load(fnm))
      throw std::runtime_error(fnm + " is not found or is not a valid ELF file");
    return e;
  }

  static ELFIO::elfio
  load(std::istream& stream)
  {
    ELFIO::elfio e;
    if (!e.load(stream))
      throw std::runtime_error("not a valid ELF stream");
    return e;
  }

  static ELFIO::elfio
  load(const void* data, size_t size)
  {
    ELFIO::elfio e;
    boost::interprocess::ibufferstream istr(static_cast<const char*>(data), size);
    if (!e.load(istr))
      throw std::runtime_error("not valid ELF data");
    return e;
  }

  static elf::platform
  detect_platform(const ELFIO::elfio& elfio)
  {
    auto osabi = elfio.get_os_abi();
    switch (osabi) {
    case osabi_aie2p:        return elf::platform::aie2p;
    case osabi_aie2ps:       return elf::platform::aie2ps;
    case osabi_aie2ps_group: return elf::platform::aie2ps_legacy;
    case osabi_aie4:         return elf::platform::aie4;
    case osabi_aie4a:        return elf::platform::aie4a;
    case osabi_aie4z:        return elf::platform::aie4z;
    default:
      throw std::runtime_error("ELF contains unsupported platform OS/ABI: " +
                               std::to_string(static_cast<int>(osabi)));
    }
  }

  ////////////////////////////////////////////////////////////////
  // Version helpers
  ////////////////////////////////////////////////////////////////

  std::pair<uint8_t, uint8_t>
  abi_version() const
  {
    constexpr uint8_t major_mask = 0xF0;
    constexpr uint8_t minor_mask = 0x0F;
    constexpr uint8_t shift = 4;
    auto v = m_elfio.get_abi_version();
    return { static_cast<uint8_t>((v & major_mask) >> shift),
             static_cast<uint8_t>(v & minor_mask) };
  }

  bool
  is_group_elf_aie2p() const
  {
    auto [major, minor] = abi_version();
    return major >= 1;
  }

  bool
  is_group_elf_gen2plus() const
  {
    auto [major, minor] = abi_version();
    return (major > 0) || (major == 0 && minor >= 3);
  }

  bool
  is_group_elf() const
  {
    return (m_platform == elf::platform::aie2p)
      ? is_group_elf_aie2p()
      : is_group_elf_gen2plus();
  }

  bool
  is_merged_format() const
  {
    return m_elfio.get_abi_version() == 0x21; // NOLINT
  }

  ////////////////////////////////////////////////////////////////
  // .symtab helpers
  ////////////////////////////////////////////////////////////////

  struct symbol_info {
    std::string name;
    unsigned char type = 0;
    ELFIO::Elf_Half section_index = UINT16_MAX;
  };

  symbol_info
  get_symbol_from_symtab(uint32_t sym_index) const
  {
    auto* symtab = m_elfio.sections[".symtab"];
    if (!symtab)
      throw std::runtime_error("No .symtab section found");

    ELFIO::symbol_section_accessor symbols(m_elfio, symtab);
    symbol_info info;
    ELFIO::Elf64_Addr value{};
    ELFIO::Elf_Xword size{};
    unsigned char bind{}, other{};
    if (!symbols.get_symbol(sym_index, info.name, value, size, bind,
                            info.type, info.section_index, other))
      throw std::runtime_error("Unable to find symbol in .symtab at index: " +
                               std::to_string(sym_index));
    return info;
  }

  std::pair<std::string, std::string>
  get_kernel_subkernel_from_symtab(uint32_t sym_index)
  {
    auto sub = get_symbol_from_symtab(sym_index);
    if (sub.type != ELFIO::STT_OBJECT)
      throw std::runtime_error("Symbol doesn't point to subkernel entry (expected STT_OBJECT)");

    auto kern = get_symbol_from_symtab(sub.section_index);
    if (kern.type != ELFIO::STT_FUNC)
      throw std::runtime_error("Subkernel doesn't point to kernel entry (expected STT_FUNC)");

    auto signature = demangle(kern.name);
    auto kernel_name = extract_kernel_name(signature);

    if (m_kernel_args_map.find(kernel_name) == m_kernel_args_map.end())
      m_kernel_args_map[kernel_name] = construct_kernel_args(signature);

    return {kernel_name, sub.name};
  }

  ////////////////////////////////////////////////////////////////
  // Section map building
  ////////////////////////////////////////////////////////////////

  void
  init_legacy_section_maps()
  {
    std::vector<uint32_t> all_ids;
    for (const auto& sec : m_elfio.sections) {
      auto id = sec->get_index();
      all_ids.push_back(id);
      m_section_to_group_map[id] = no_ctrl_code_id;
    }
    m_kernel_name_to_id_map[""] = no_ctrl_code_id;
    m_group_to_sections_map.emplace(no_ctrl_code_id, std::move(all_ids));
  }

  void
  parse_single_group_section(const ELFIO::section* sec)
  {
    const auto* data = sec->get_data();
    const auto  sz   = sec->get_size();
    auto group_id    = sec->get_index();

    if (!data || sz < sizeof(ELFIO::Elf_Word))
      return;

    auto [kernel_name, subkernel_name] =
      get_kernel_subkernel_from_symtab(sec->get_info());

    m_kernel_to_subkernels_map[kernel_name].push_back(subkernel_name);
    m_kernel_name_to_id_map[kernel_name + subkernel_name] = group_id;

    const auto* words = reinterpret_cast<const ELFIO::Elf_Word*>(data);
    auto word_count = sz / sizeof(ELFIO::Elf_Word);

    std::vector<uint32_t> members;
    for (size_t i = 1; i < word_count; ++i) {
      auto mid = words[i];
      members.push_back(mid);
      m_section_to_group_map[mid] = group_id;
    }
    m_group_to_sections_map.emplace(group_id, std::move(members));
  }

  void
  parse_custom_sections(const std::vector<uint32_t>& ids)
  {
    for (auto id : ids) {
      auto* sec = m_elfio.sections[id];
      if (!sec) continue;
      auto& cached = m_byte_cache[sec->get_name()];
      cached.resize(sec->get_size());
      std::memcpy(cached.data(), sec->get_data(), sec->get_size());
      m_custom_section_map[sec->get_name()] =
        std::span<const std::byte>(cached.data(), cached.size());
    }
  }

  void
  parse_sections()
  {
    if (!is_group_elf()) {
      init_legacy_section_maps();
      finalize_kernels();
      m_kernel_args_map.clear();
      return;
    }

    std::vector<uint32_t> custom_ids;
    constexpr ELFIO::Elf_Word custom_type = ELFIO::SHT_LOUSER + 1;
    for (const auto& sec : m_elfio.sections) {
      if (!sec) continue;
      if (sec->get_type() == ELFIO::SHT_GROUP)
        parse_single_group_section(sec.get());
      else if (sec->get_type() == custom_type)
        custom_ids.push_back(sec->get_index());
    }
    finalize_kernels();
    parse_custom_sections(custom_ids);
    m_kernel_args_map.clear();
  }

  void
  finalize_kernels()
  {
    for (const auto& [kname, args] : m_kernel_args_map) {
      elf::kernel k;
      k.name = kname;
      k.args = args;
      if (auto it = m_kernel_to_subkernels_map.find(kname);
          it != m_kernel_to_subkernels_map.end())
        k.instances = it->second;
      m_kernels.push_back(std::move(k));
    }
  }

  ////////////////////////////////////////////////////////////////
  // AIE gen2 (aie2p) buffer initialization
  ////////////////////////////////////////////////////////////////

  void
  gen2_init_simple_buf_map(elf::buf_type type,
                           std::map<uint32_t, section_buf>& out)
  {
    auto pattern = section_pattern(type);
    for (const auto& sec : m_elfio.sections) {
      if (sec->get_name().find(pattern) == std::string::npos)
        continue;
      auto grp = m_section_to_group_map[sec->get_index()];
      out[grp].append(sec.get());
    }
  }

  void
  gen2_init_save_restore()
  {
    auto save_pat    = section_pattern(elf::buf_type::preempt_save);
    auto restore_pat = section_pattern(elf::buf_type::preempt_restore);

    for (const auto& [gid, sec_ids] : m_group_to_sections_map) {
      bool has_save = false, has_restore = false;
      for (auto idx : sec_ids) {
        auto* sec = m_elfio.sections[idx];
        auto nm = sec->get_name();
        if (nm.find(save_pat) != std::string::npos) {
          m_save_buf_map[gid].append(sec);
          has_save = true;
        }
        else if (nm.find(restore_pat) != std::string::npos) {
          m_restore_buf_map[gid].append(sec);
          has_restore = true;
        }
      }
      if (has_save != has_restore)
        throw std::runtime_error("Invalid ELF: preempt save and restore sections are not paired");
      if (has_save && has_restore)
        m_preemption_exist = true;
    }
  }

  void
  gen2_init_pdi()
  {
    auto pdi_pat = section_pattern(elf::buf_type::pdi);
    for (const auto& sec : m_elfio.sections) {
      if (sec->get_name().find(pdi_pat) == std::string::npos)
        continue;
      m_pdi_buf_map[sec->get_name()].append(sec.get());
    }
  }

  void
  gen2_init_ctrlpkt_pm()
  {
    auto pm_pat = section_pattern(elf::buf_type::ctrlpkt_pm);
    for (const auto& sec : m_elfio.sections) {
      if (sec->get_name().find(pm_pat) == std::string::npos)
        continue;
      m_ctrlpkt_pm_bufs[sec->get_name()].append(sec.get());
    }
  }

  // gen2 patcher: mirrors elf_aie_gen2::initialize_arg_patchers
  void
  gen2_init_patchers()
  {
    static constexpr const char* scratch_pad_sym = "scratch-pad-ctrl";
    static constexpr const char* ctrlpkt_pm_sym  = "ctrlpkt-pm";

    auto* dynsym = m_elfio.sections[".dynsym"];
    auto* dynstr = m_elfio.sections[".dynstr"];
    auto* dynsec = m_elfio.sections[".rela.dyn"];
    if (!dynsym || !dynstr || !dynsec)
      return;

    auto abi_ver = static_cast<uint16_t>(m_elfio.get_abi_version());
    auto begin = reinterpret_cast<const ELFIO::Elf32_Rela*>(dynsec->get_data());
    auto end   = begin + dynsec->get_size() / sizeof(ELFIO::Elf32_Rela);

    for (auto rela = begin; rela != end; ++rela) {
      auto symidx = ELFIO::get_sym_and_type<ELFIO::Elf32_Rela>::get_r_sym(rela->r_info);
      auto rtype  = ELFIO::get_sym_and_type<ELFIO::Elf32_Rela>::get_r_type(rela->r_info);

      auto dsym_off = symidx * sizeof(ELFIO::Elf32_Sym);
      if (dsym_off >= dynsym->get_size())
        throw std::runtime_error("Invalid symbol index " + std::to_string(symidx));

      auto* sym = reinterpret_cast<const ELFIO::Elf32_Sym*>(dynsym->get_data() + dsym_off);
      if (sym->st_name >= dynstr->get_size())
        throw std::runtime_error("Invalid symbol name offset " + std::to_string(sym->st_name));

      const char* symname = dynstr->get_data() + sym->st_name;

      if (!m_ctrl_scratch_pad_mem_size && std::strcmp(symname, scratch_pad_sym) == 0)
        m_ctrl_scratch_pad_mem_size = static_cast<size_t>(sym->st_size);

      if (std::string(symname).find(ctrlpkt_pm_sym) != std::string::npos)
        m_ctrlpkt_pm_dynsyms.emplace(symname);

      auto* patch_sec = m_elfio.sections[sym->st_shndx];
      if (!patch_sec)
        throw std::runtime_error("Invalid section index " + std::to_string(sym->st_shndx));

      auto sec_name = patch_sec->get_name();
      auto sec_idx  = patch_sec->get_index();
      auto grp_idx  = m_section_to_group_map[sec_idx];

      // Determine which buf type and total accumulated size this section belongs to
      elf::buf_type btype = elf::buf_type::ctrltext;
      size_t        sec_size = 0;

      auto resolve_section_type = [&]() -> std::pair<size_t, elf::buf_type> {
        auto check = [&](elf::buf_type t, auto& bmap) -> bool {
          if (sec_name.find(section_pattern(t)) != std::string::npos) {
            auto it = bmap.find(grp_idx);
            if (it == bmap.end())
              throw std::runtime_error("Section info not cached for group " + std::to_string(grp_idx));
            return true;
          }
          return false;
        };
        if (check(elf::buf_type::ctrltext, m_instr_buf_map))
          return {m_instr_buf_map[grp_idx].size(), elf::buf_type::ctrltext};
        if (!m_ctrl_packet_map.empty() &&
            check(elf::buf_type::ctrldata, m_ctrl_packet_map))
          return {m_ctrl_packet_map[grp_idx].size(), elf::buf_type::ctrldata};
        if (check(elf::buf_type::preempt_save, m_save_buf_map))
          return {m_save_buf_map[grp_idx].size(), elf::buf_type::preempt_save};
        if (check(elf::buf_type::preempt_restore, m_restore_buf_map))
          return {m_restore_buf_map[grp_idx].size(), elf::buf_type::preempt_restore};
        if (!m_pdi_buf_map.empty() &&
            sec_name.find(section_pattern(elf::buf_type::pdi)) != std::string::npos) {
          auto it = m_pdi_buf_map.find(sec_name);
          if (it == m_pdi_buf_map.end())
            throw std::runtime_error("PDI section not cached: " + sec_name);
          return {it->second.size(), elf::buf_type::pdi};
        }
        throw std::runtime_error("Invalid section passed: " + sec_name);
      };

      auto [resolved_size, resolved_type] = resolve_section_type();
      btype    = resolved_type;
      sec_size = resolved_size;

      auto offset = static_cast<uint64_t>(rela->r_offset);
      if (offset >= sec_size)
        throw std::runtime_error("Invalid offset " + std::to_string(offset));

      if (sec_name.find("pdi") != std::string::npos)
        m_ctrl_pdi_map[grp_idx].insert(symname);

      elf::patch_schema schema = elf::patch_schema::scalar_32bit;
      uint32_t add_end_addr = 0;
      if (abi_ver != 1) {
        add_end_addr = static_cast<uint32_t>(rela->r_addend);
        schema = static_cast<elf::patch_schema>(rtype);
      }
      else {
        add_end_addr = (static_cast<uint32_t>(rela->r_addend) & addend_mask) >> addend_shift;
        schema = static_cast<elf::patch_schema>(
          static_cast<uint32_t>(rela->r_addend) & schema_mask);
      }

      uint32_t mask = (schema == elf::patch_schema::scalar_32bit)
        ? static_cast<uint32_t>(sym->st_size) : 0;

      std::string argnm{symname, symname + std::min(std::strlen(symname), dynstr->get_size())};
      auto key = make_key(argnm, btype);

      elf::patch_point pp{argnm, schema, btype, offset, add_end_addr, mask};
      m_patch_points[grp_idx][key].push_back(std::move(pp));
    }
  }

  void
  gen2_init_all()
  {
    gen2_init_simple_buf_map(elf::buf_type::ctrltext,  m_instr_buf_map);
    gen2_init_simple_buf_map(elf::buf_type::ctrldata,  m_ctrl_packet_map);
    gen2_init_save_restore();
    gen2_init_pdi();
    gen2_init_ctrlpkt_pm();
    gen2_init_patchers();
  }

  ////////////////////////////////////////////////////////////////
  // AIE gen2plus (aie2ps/aie4) buffer initialization
  ////////////////////////////////////////////////////////////////

  void
  gen2plus_init_column_ctrlcode(std::map<uint32_t, std::vector<size_t>>& pad_offsets)
  {
    auto ctrltext_pat = section_pattern(elf::buf_type::ctrltext);
    auto ctrldata_pat = section_pattern(elf::buf_type::ctrldata);
    auto pad_pat      = section_pattern(elf::buf_type::pad);

    struct elf_page { ELFIO::section* ctrltext = nullptr; ELFIO::section* ctrldata = nullptr; };
    using page_map = std::map<uint32_t, elf_page>;
    using uc_map   = std::map<uint32_t, page_map>;
    std::map<uint32_t, uc_map> ctrl_map;

    bool merged = is_merged_format();

    for (const auto& [id, sec_ids] : m_group_to_sections_map) {
      for (auto sidx : sec_ids) {
        auto* sec = m_elfio.sections[sidx];
        auto nm = sec->get_name();
        if (nm.find(ctrltext_pat) != std::string::npos) {
          auto [col, page] = get_column_and_page(nm, merged);
          ctrl_map[id][col][page].ctrltext = sec;
        }
        else if (nm.find(ctrldata_pat) != std::string::npos) {
          auto [col, page] = get_column_and_page(nm, merged);
          ctrl_map[id][col][page].ctrldata = sec;
        }
      }
    }

    for (const auto& [id, uc_sec] : ctrl_map) {
      auto sz = uc_sec.empty() ? 0 : uc_sec.rbegin()->first + 1;
      m_ctrlcodes_map[id].resize(sz);
      pad_offsets[id].resize(sz);
      for (const auto& [ucidx, pages] : uc_sec) {
        if (merged) {
          auto& pg = pages.begin()->second;
          if (pg.ctrltext)
            m_ctrlcodes_map[id][ucidx].append(pg.ctrltext);
        }
        else {
          for (const auto& [page, pg] : pages) {
            if (pg.ctrltext) m_ctrlcodes_map[id][ucidx].append(pg.ctrltext);
            if (pg.ctrldata) m_ctrlcodes_map[id][ucidx].append(pg.ctrldata);
            auto cur    = m_ctrlcodes_map[id][ucidx].size();
            auto target = (page + 1) * elf_page_size;
            if (cur < target)
              m_ctrlcodes_map[id][ucidx].pad_to(target);
          }
        }
        pad_offsets[id][ucidx] = m_ctrlcodes_map[id][ucidx].size();
      }
    }

    // Append .pad sections
    for (const auto& [id, sec_ids] : m_group_to_sections_map) {
      for (auto sidx : sec_ids) {
        auto* sec = m_elfio.sections[sidx];
        if (sec->get_name().find(pad_pat) == std::string::npos)
          continue;
        auto [col, page] = get_column_and_page(sec->get_name(), merged);
        m_ctrlcodes_map[id][col].append(sec);
      }
    }
  }

  void
  gen2plus_init_ctrlpkt()
  {
    auto pat = section_pattern(elf::buf_type::ctrlpkt);
    for (const auto& sec : m_elfio.sections) {
      if (sec->get_name().find(pat) == std::string::npos)
        continue;
      auto grp = m_section_to_group_map[sec->get_index()];
      m_ctrlpkt_buf_map[grp][sec->get_name()].append(sec.get());
    }
  }

  void
  gen2plus_init_dump()
  {
    auto pat = section_pattern(elf::buf_type::dump);
    for (const auto& sec : m_elfio.sections) {
      if (sec->get_name().find(pat) == std::string::npos)
        continue;
      auto ctrl_id = m_section_to_group_map[sec->get_index()];
      m_dump_buf_map[ctrl_id].append(sec.get());
    }
  }

  void
  gen2plus_init_patchers(const std::map<uint32_t, std::vector<size_t>>& pad_offsets)
  {
    auto pad_pat     = section_pattern(elf::buf_type::pad);
    auto ctrlpkt_pat = section_pattern(elf::buf_type::ctrlpkt);

    auto* dynsym = m_elfio.sections[".dynsym"];
    auto* dynstr = m_elfio.sections[".dynstr"];
    auto* dynsec = m_elfio.sections[".rela.dyn"];
    if (!dynsym || !dynstr || !dynsec)
      return;

    bool merged = is_merged_format();
    auto abi_ver = static_cast<uint16_t>(m_elfio.get_abi_version());
    auto begin = reinterpret_cast<const ELFIO::Elf32_Rela*>(dynsec->get_data());
    auto end   = begin + dynsec->get_size() / sizeof(ELFIO::Elf32_Rela);

    for (auto rela = begin; rela != end; ++rela) {
      auto symidx = ELFIO::get_sym_and_type<ELFIO::Elf32_Rela>::get_r_sym(rela->r_info);
      auto rtype  = ELFIO::get_sym_and_type<ELFIO::Elf32_Rela>::get_r_type(rela->r_info);

      auto dsym_off = symidx * sizeof(ELFIO::Elf32_Sym);
      if (dsym_off >= dynsym->get_size())
        throw std::runtime_error("Invalid symbol index " + std::to_string(symidx));

      auto* sym = reinterpret_cast<const ELFIO::Elf32_Sym*>(dynsym->get_data() + dsym_off);
      if (sym->st_name >= dynstr->get_size())
        throw std::runtime_error("Invalid symbol name offset " + std::to_string(sym->st_name));

      const char* symname = dynstr->get_data() + sym->st_name;
      std::string argnm{symname, symname + std::min(std::strlen(symname), dynstr->get_size())};

      auto* patch_sec = m_elfio.sections[sym->st_shndx];
      if (!patch_sec)
        throw std::runtime_error("Invalid section index " + std::to_string(sym->st_shndx));

      auto patch_sec_name = patch_sec->get_name();
      auto [col, page]    = get_column_and_page(patch_sec_name, merged);
      auto sec_idx        = patch_sec->get_index();
      auto grp_idx        = m_section_to_group_map[sec_idx];

      if (m_ctrlcodes_map.find(grp_idx) == m_ctrlcodes_map.end())
        throw std::runtime_error("Unable to fetch ctrlcode for symbol: " + argnm);

      const auto& ctrlcodes = m_ctrlcodes_map[grp_idx];
      uint64_t abs_offset = 0;
      elf::buf_type btype = elf::buf_type::buf_type_count;

      if (patch_sec_name.find(pad_pat) != std::string::npos) {
        for (uint32_t i = 0; i < col; ++i)
          abs_offset += ctrlcodes[i].size();
        abs_offset += pad_offsets.at(grp_idx)[col];
        abs_offset += rela->r_offset;
        btype = elf::buf_type::pad;
      }
      else if (patch_sec_name.find(ctrlpkt_pat) != std::string::npos) {
        abs_offset = rela->r_offset;
        btype = elf::buf_type::ctrlpkt;
        argnm += strip_group_suffix(patch_sec_name);
      }
      else {
        auto col_size  = ctrlcodes.at(col).size();
        auto sec_off   = page * elf_page_size + rela->r_offset + 16; // NOLINT
        if (sec_off >= col_size)
          throw std::runtime_error("Invalid ctrlcode offset " + std::to_string(sec_off));
        for (uint32_t i = 0; i < col; ++i)
          abs_offset += ctrlcodes.at(i).size();
        abs_offset += sec_off;
        btype = elf::buf_type::ctrltext;
      }

      elf::patch_schema schema = elf::patch_schema::unknown;
      uint32_t add_end_addr = 0;
      if (abi_ver != 1) {
        add_end_addr = static_cast<uint32_t>(rela->r_addend);
        schema = static_cast<elf::patch_schema>(rtype);
      }
      else {
        add_end_addr = (static_cast<uint32_t>(rela->r_addend) & addend_mask) >> addend_shift;
        schema = static_cast<elf::patch_schema>(
          static_cast<uint32_t>(rela->r_addend) & schema_mask);
      }

      auto key = make_key(argnm, btype);
      elf::patch_point pp{argnm, schema, btype, abs_offset, add_end_addr, 0};
      m_patch_points[grp_idx][key].push_back(std::move(pp));
    }
  }

  void
  gen2plus_init_all()
  {
    std::map<uint32_t, std::vector<size_t>> pad_offsets;
    gen2plus_init_column_ctrlcode(pad_offsets);
    gen2plus_init_ctrlpkt();
    gen2plus_init_dump();
    gen2plus_init_patchers(pad_offsets);
  }

  ////////////////////////////////////////////////////////////////
  // Span helpers — materialise a section_buf into the byte cache
  // so we can return a stable span.
  ////////////////////////////////////////////////////////////////

  std::span<const std::byte>
  materialise(const std::string& key, const section_buf& buf) const
  {
    auto& cached = m_byte_cache[key];
    if (cached.empty() && buf.size() > 0)
      cached = buf.to_bytes();
    return std::span<const std::byte>(cached.data(), cached.size());
  }

  ////////////////////////////////////////////////////////////////
  // get_ctrlcode_id  (shared logic for both platforms)
  ////////////////////////////////////////////////////////////////

  uint32_t
  resolve_ctrlcode_id(const std::string& name,
                      const std::function<bool(uint32_t)>& has_ctrlcode) const
  {
    if (auto pos = name.find(':'); pos != std::string::npos) {
      auto key = name.substr(0, pos) + name.substr(pos + 1);
      auto it  = m_kernel_name_to_id_map.find(key);
      if (it == m_kernel_name_to_id_map.end())
        throw std::runtime_error("Unable to find group idx for kernel: " + name);
      auto id = it->second;
      if (!has_ctrlcode(id))
        throw std::runtime_error("Unable to find ctrlcode entry for kernel: " + name);
      return id;
    }

    if (auto entry = m_kernel_to_subkernels_map.find(name);
        entry != m_kernel_to_subkernels_map.end()) {
      if (entry->second.size() == 1) {
        auto key = name + entry->second.front();
        auto it  = m_kernel_name_to_id_map.find(key);
        if (it == m_kernel_name_to_id_map.end())
          throw std::runtime_error("Unable to find group idx for kernel: " + key);
        auto id = it->second;
        if (!has_ctrlcode(id))
          throw std::runtime_error("Unable to find ctrlcode entry for kernel: " + name);
        return id;
      }
      throw std::runtime_error("Multiple sub kernels present, cannot choose sub kernel");
    }

    throw std::runtime_error("cannot get ctrlcode id from kernel name: " + name);
  }
};

////////////////////////////////////////////////////////////////
// aiebu::elf method implementations
////////////////////////////////////////////////////////////////

elf::
elf(const std::string& filename)
  : m_reader{std::make_unique<elf_reader>()}
{
  m_reader->m_elfio    = elf_reader::load(filename);
  m_reader->m_path     = filename;
  m_reader->m_platform = elf_reader::detect_platform(m_reader->m_elfio);
  m_reader->parse_sections();
  if (m_reader->m_platform == elf::platform::aie2p)
    m_reader->gen2_init_all();
  else
    m_reader->gen2plus_init_all();
}

elf::
elf(std::istream& stream)
  : m_reader{std::make_unique<elf_reader>()}
{
  m_reader->m_elfio    = elf_reader::load(stream);
  m_reader->m_platform = elf_reader::detect_platform(m_reader->m_elfio);
  m_reader->parse_sections();
  if (m_reader->m_platform == elf::platform::aie2p)
    m_reader->gen2_init_all();
  else
    m_reader->gen2plus_init_all();
}

elf::
elf(const void* data, size_t size)
  : m_reader{std::make_unique<elf_reader>()}
{
  m_reader->m_elfio    = elf_reader::load(data, size);
  m_reader->m_platform = elf_reader::detect_platform(m_reader->m_elfio);
  m_reader->parse_sections();
  if (m_reader->m_platform == elf::platform::aie2p)
    m_reader->gen2_init_all();
  else
    m_reader->gen2plus_init_all();
}

elf::
elf(std::string_view data)
  : elf(data.data(), data.size())
{}

elf::~elf() = default;
elf::elf(elf&&) noexcept = default;
elf& elf::operator=(elf&&) noexcept = default;

elf::platform
elf::
get_platform() const
{
  return m_reader->m_platform;
}

uint8_t
elf::
get_os_abi() const
{
  return m_reader->m_elfio.get_os_abi();
}

std::pair<uint8_t, uint8_t>
elf::
get_abi_version() const
{
  return m_reader->abi_version();
}

bool
elf::
is_full_elf() const
{
  return m_reader->m_elfio.sections[".note.xrt.configuration"] != nullptr;
}

bool
elf::
is_group_elf() const
{
  return m_reader->is_group_elf();
}

std::array<uint8_t, 16>
elf::
get_cfg_uuid() const
{
  constexpr size_t uuid_size = 16;
  auto* sec = m_reader->m_elfio.sections[".note.xrt.UID"];
  if (!sec)
    throw std::runtime_error("ELF is missing .note.xrt.UID section");

  // NOLINTNEXTLINE - ELFIO API requires non-const pointer
  ELFIO::note_section_accessor accessor(m_reader->m_elfio, const_cast<ELFIO::section*>(sec));
  ELFIO::Elf_Word type = 0;
  std::string name;
  char* desc = nullptr;
  ELFIO::Elf_Word desc_size = 0;
  if (!accessor.get_note(0, type, name, desc, desc_size))
    throw std::runtime_error("Failed to get UUID note");
  if (desc_size != uuid_size)
    throw std::runtime_error("Invalid UUID size: expected 16, got " + std::to_string(desc_size));

  std::array<uint8_t, 16> uuid{};
  std::memcpy(uuid.data(), desc, uuid_size);
  return uuid;
}

uint32_t
elf::
get_partition_size() const
{
  auto* sec = m_reader->m_elfio.sections[".note.xrt.configuration"];
  if (!sec)
    throw std::runtime_error("ELF is missing xrt configuration info");

  // NOLINTNEXTLINE
  ELFIO::note_section_accessor accessor(m_reader->m_elfio, const_cast<ELFIO::section*>(sec));
  ELFIO::Elf_Word type = 0;
  std::string name;
  char* desc = nullptr;
  ELFIO::Elf_Word desc_size = 0;
  if (!accessor.get_note(0, type, name, desc, desc_size))
    throw std::runtime_error("Failed to get configuration note");

  uint32_t value = 0;
  std::memcpy(&value, desc, std::min(static_cast<size_t>(desc_size), sizeof(uint32_t)));
  return value;
}

std::vector<elf::kernel>
elf::
get_kernels() const
{
  return m_reader->m_kernels;
}

std::span<const std::byte>
elf::
get_section(std::string_view name) const
{
  // Check custom section map first
  auto nm = std::string(name);
  if (auto it = m_reader->m_custom_section_map.find(nm);
      it != m_reader->m_custom_section_map.end())
    return it->second;

  // Fall back to any named section
  auto* sec = m_reader->m_elfio.sections[nm];
  if (!sec)
    return {};

  auto& cached = m_reader->m_byte_cache[nm];
  if (cached.empty() && sec->get_size() > 0) {
    cached.resize(sec->get_size());
    std::memcpy(cached.data(), sec->get_data(), sec->get_size());
  }
  return std::span<const std::byte>(cached.data(), cached.size());
}

void
elf::
save(std::ostream& stream) const
{
  // NOLINTNEXTLINE - ELFIO save takes non-const stream
  const_cast<ELFIO::elfio&>(m_reader->m_elfio).save(stream);
}

std::map<uint32_t, uint32_t>
elf::
get_section_to_group_map() const
{
  return m_reader->m_section_to_group_map;
}

std::map<uint32_t, std::vector<uint32_t>>
elf::
get_group_to_sections_map() const
{
  return m_reader->m_group_to_sections_map;
}

std::map<std::string, uint32_t>
elf::
get_kernel_name_to_id_map() const
{
  return m_reader->m_kernel_name_to_id_map;
}

uint32_t
elf::
get_ctrlcode_id(const std::string& name) const
{
  if (m_reader->m_platform == elf::platform::aie2p) {
    return m_reader->resolve_ctrlcode_id(name, [&](uint32_t id) {
      return m_reader->m_instr_buf_map.count(id) || m_reader->m_ctrl_packet_map.count(id);
    });
  }
  return m_reader->resolve_ctrlcode_id(name, [&](uint32_t id) {
    return m_reader->m_ctrlcodes_map.count(id) > 0;
  });
}

std::span<const std::byte>
elf::
get_pdi(const std::string& symbol_name) const
{
  auto it = m_reader->m_pdi_buf_map.find(symbol_name);
  if (it == m_reader->m_pdi_buf_map.end())
    return {};
  return m_reader->materialise("pdi:" + symbol_name, it->second);
}

std::set<std::string>
elf::
get_ctrlpkt_pm_dynsyms() const
{
  return m_reader->m_ctrlpkt_pm_dynsyms;
}

std::span<const std::byte>
elf::
get_ctrlpkt_pm_buf(const std::string& symbol_name) const
{
  auto it = m_reader->m_ctrlpkt_pm_bufs.find(symbol_name);
  if (it == m_reader->m_ctrlpkt_pm_bufs.end())
    return {};
  return m_reader->materialise("ctrlpkt_pm:" + symbol_name, it->second);
}

std::map<uint32_t, std::map<std::string, std::vector<elf::patch_point>>>
elf::
get_patch_points() const
{
  return m_reader->m_patch_points;
}

size_t
elf::
get_ctrl_scratch_pad_mem_size() const
{
  return m_reader->m_ctrl_scratch_pad_mem_size;
}

} // namespace aiebu
