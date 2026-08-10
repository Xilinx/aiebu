// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

#include "aiebu/elf.h"
#include "elf/aie_elf_constants.h"

#include "elfio/elfio.hpp"

#include <boost/interprocess/streams/bufferstream.hpp>

#include <array>
#include <cstdint>
#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

namespace aiebu {

// Sentinel ctrl-code id for legacy ELFs with no .group sections
static constexpr uint32_t no_ctrl_code_id = UINT32_MAX;

static constexpr std::array<std::string_view, 9> section_name_patterns = {
  ".ctrltext",        // ctrltext
  ".ctrldata",        // ctrldata
  ".preempt_save",    // preempt_save
  ".preempt_restore", // preempt_restore
  ".pdi",             // pdi
  ".ctrlpkt.pm",      // ctrlpkt_pm
  ".pad",             // pad
  ".dump",            // dump
  ".ctrlpkt"          // ctrlpkt
};

static constexpr std::string_view
section_pattern(elf::buf_type t)
{
  return section_name_patterns[static_cast<uint32_t>(t)];
}

// Generates the same key string as xrt_core::elf_patcher::generate_key_string
// so xrt::elf_impl can look up patch points without change.
static std::string
make_key(const std::string& arg_name, elf::buf_type t)
{
  return arg_name + std::to_string(static_cast<uint32_t>(t));
}

// Strip trailing ".<integer>" group-id suffix from a ctrlpkt section name,
// mirroring xrt_core::elf_patcher::get_symbol_name_from_section_name.
static std::string
strip_group_suffix(const std::string& name)
{
  auto pos = name.rfind('.');
  if (pos == std::string::npos)
    return name;

  auto suffix = name.substr(pos + 1);
  if (!suffix.empty() && suffix.find_first_not_of("0123456789") == std::string::npos)
    return name.substr(0, pos);

  return name;
}

////////////////////////////////////////////////////////////////
// Kernel signature parsing — ported from xrt_elf.cpp
////////////////////////////////////////////////////////////////

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
  case 'v':
    return "void";
  case 'c':
    return "char";
  case 'i':
    return "int";
  default:
    throw std::runtime_error("Unknown type in mangled name: " + std::string(1, c));
  }
}

static std::string
demangle(const std::string& mangled)
{
  static constexpr size_t prefix_len = 2;
  if (mangled.size() <= prefix_len || mangled.substr(0, prefix_len) != "_Z")
    throw std::runtime_error("Not a mangled kernel name: " + mangled);

  size_t idx = prefix_len;
  size_t len = 0;
  while (idx < mangled.size() && std::isdigit(mangled[idx]))
    len = len * 10 + (mangled[idx++] - '0');

  if (idx + len > mangled.size())
    throw std::runtime_error("Invalid mangled name length");

  std::string name = mangled.substr(idx, len);
  idx += len;

  std::vector<std::string> args;
  while (idx < mangled.size()) {
    int depth = 0;
    while (idx < mangled.size() && mangled[idx] == 'P') {
      ++depth;
      ++idx;
    }

    if (idx >= mangled.size())
      throw std::runtime_error("demangle arg index out of bounds");

    std::string type = get_demangle_type(mangled[idx++]);
    for (int i = 0; i < depth; ++i)
      type += "*";

    args.push_back(type);
  }

  std::string result = name + "(";
  for (size_t i = 0; i < args.size(); ++i) {
    if (i > 0)
      result += ", ";

    result += args[i];
  }
  return result + ")";
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

  auto parts = split(signature.substr(start + 1, end - start - 1), ',');
  uint32_t idx = 0;
  for (const auto& s : parts) {
    if (s.find('*') == std::string::npos)
      throw std::runtime_error("scalar args not yet supported");

    elf::arg a;
    a.name      = "argv" + std::to_string(idx);
    a.data_type = s;
    a.index     = idx;
    a.is_global = true;
    args.push_back(std::move(a));
    ++idx;
  }
  return args;
}

////////////////////////////////////////////////////////////////
// section_buf — zero-copy views over ELFIO section data
// with optional padding.  Mirrors xrt::buf from elf_int.h.
////////////////////////////////////////////////////////////////
struct section_buf
{
  std::vector<std::string_view> views;
  std::vector<uint8_t>          padding;

  void
  append(const ELFIO::section* sec)
  {
    if (sec && sec->get_size() > 0)
      views.emplace_back(sec->get_data(), sec->get_size());
  }

  void
  pad_to(size_t target)
  {
    size_t cur = size();
    if (target <= cur)
      return;

    size_t n = target - cur;
    padding.insert(padding.end(), n, 0);
    views.emplace_back(reinterpret_cast<const char*>(padding.data()), n);
  }

  size_t
  size() const
  {
    size_t total = 0;
    for (const auto& v : views)
      total += v.size();

    return total;
  }

  // Copy all views into dest in order.  dest.size() must be >= size().
  void
  copy_to(std::span<std::byte> dest) const
  {
    if (dest.size() < size())
      throw std::runtime_error("destination buffer too small for section_buf::copy_to");

    auto* p = dest.data();
    for (const auto& v : views) {
      std::memcpy(p, v.data(), v.size());
      p += v.size();
    }
  }
};

////////////////////////////////////////////////////////////////
// elf_reader — abstract base
//
// Owns the ELFIO object, platform identity, all maps that are
// shared between platforms (group maps, kernel metadata, custom
// sections, patch points, byte cache), and the common parsing
// logic.  Platform-specific buffer init is deferred to subclasses.
////////////////////////////////////////////////////////////////
class elf_reader
{
public:
  ELFIO::elfio  m_elfio;
  elf::platform m_platform;
  std::string   m_path;  // file path this ELF was loaded from; empty if loaded from stream/buffer

  // section index -> group index (no_ctrl_code_id for legacy ELFs with no .group sections)
  std::map<uint32_t, uint32_t>              m_section_to_group_map;

  // group index (ctrl-code-id) -> member section indices
  std::map<uint32_t, std::vector<uint32_t>> m_group_to_sections_map;

  // "kernel_name + subkernel_name" -> group index; used by get_ctrlcode_id()
  std::map<std::string, uint32_t>           m_kernel_name_to_id_map;

  // kernel name -> vector of subkernel/instance names
  std::map<std::string, std::vector<std::string>> m_kernel_to_subkernels_map;

  // Final kernel objects exposed through aiebu::elf::get_kernels()
  std::vector<elf::kernel>                        m_kernels;

  // Custom sections (ELF type SHT_LOUSER+1); key = section name, value = zero-copy span
  // into ELFIO-owned memory — valid for the lifetime of m_elfio.
  std::map<std::string, std::span<const std::byte>> m_custom_section_map;

  // Patch points grouped by ctrl-code-id then by key-string.
  // key-string = arg_name + to_string(buf_type), matching xrt_core::elf_patcher convention.
  std::map<uint32_t, std::map<std::string, std::vector<elf::patch_point>>>
    m_patch_points;

  // rela->r_addend encoding for ABI version 1:
  //   bits [0:3]  = patching schema
  //   bits [4:31] = base-bo address offset
  static constexpr uint32_t addend_shift = 4;
  static constexpr uint32_t addend_mask  = ~((uint32_t)0) << addend_shift;
  static constexpr uint32_t schema_mask  = ~addend_mask;

  virtual ~elf_reader() = default;

  ////////////////////////////////////////////////////////////////
  // Static load helpers — construct an ELFIO object from various sources
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
    switch (elfio.get_os_abi()) {
    case osabi_aie2p:
      return elf::platform::aie2p;
    case osabi_aie2ps:
      return elf::platform::aie2ps;
    case osabi_aie2ps_group:
      return elf::platform::aie2ps_legacy;
    case osabi_aie4:
      return elf::platform::aie4;
    case osabi_aie4a:
      return elf::platform::aie4a;
    case osabi_aie4z:
      return elf::platform::aie4z;
    default:
      throw std::runtime_error("Unsupported ELF OS/ABI: " +
                               std::to_string(static_cast<int>(elfio.get_os_abi())));
    }
  }

  ////////////////////////////////////////////////////////////////
  // Version helpers
  ////////////////////////////////////////////////////////////////

  // Returns ELF ABI version as (major, minor).
  // The version byte encodes: upper nibble = major, lower nibble = minor.
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

  virtual bool is_group_elf() const = 0;

  virtual uint32_t
  get_ctrlcode_id(const std::string& name) const = 0;

  ////////////////////////////////////////////////////////////////
  // .symtab helpers — used during parse_sections(), shared by both platforms
  ////////////////////////////////////////////////////////////////

  // Symbol information extracted from the .symtab section
  struct symbol_info {
    std::string     name;
    unsigned char   type = 0;            // STT_FUNC, STT_OBJECT, etc.
    ELFIO::Elf_Half section_index = UINT16_MAX;  // st_shndx field
  };

  // Retrieve symbol at sym_index from .symtab; throws if not found
  symbol_info
  get_symbol_from_symtab(uint32_t sym_index) const
  {
    auto* symtab = m_elfio.sections[".symtab"];
    if (!symtab)
      throw std::runtime_error("No .symtab section found");

    ELFIO::symbol_section_accessor symbols(m_elfio, symtab);
    symbol_info info;
    ELFIO::Elf64_Addr value{};
    ELFIO::Elf_Xword  size{};
    unsigned char     bind{}, other{};
    if (!symbols.get_symbol(sym_index, info.name, value, size, bind,
                            info.type, info.section_index, other))
      throw std::runtime_error("Symbol not found in .symtab at index: " +
                               std::to_string(sym_index));

    return info;
  }

  // Transient: kernel name -> arg list, populated during .group section parsing
  // and used to build m_kernels.  Cleared by parse_sections() when done.
  std::map<std::string, std::vector<elf::arg>> m_kernel_args_map;

  // Follow the .group info field through .symtab to recover the kernel name
  // (from the STT_FUNC symbol) and subkernel name (from the STT_OBJECT symbol).
  // Also populates m_kernel_args_map on first encounter of a kernel name.
  std::pair<std::string, std::string>
  get_kernel_subkernel_from_symtab(uint32_t sym_index)
  {
    auto sub = get_symbol_from_symtab(sym_index);
    if (sub.type != ELFIO::STT_OBJECT)
      throw std::runtime_error("Expected STT_OBJECT for subkernel symbol");

    auto kern = get_symbol_from_symtab(sub.section_index);
    if (kern.type != ELFIO::STT_FUNC)
      throw std::runtime_error("Expected STT_FUNC for kernel symbol");

    auto signature   = demangle(kern.name);
    auto kernel_name = extract_kernel_name(signature);
    if (m_kernel_args_map.find(kernel_name) == m_kernel_args_map.end())
      m_kernel_args_map[kernel_name] = construct_kernel_args(signature);

    return {kernel_name, sub.name};
  }

  ////////////////////////////////////////////////////////////////
  // Section map building — shared by both platform subclasses
  ////////////////////////////////////////////////////////////////

  // Populate group/section maps for legacy ELFs that have no .group sections.
  // All sections are assigned to no_ctrl_code_id; the empty string is used
  // as the kernel name so get_ctrlcode_id("") returns a valid id.
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

  // Parse one .group section: resolve the kernel/subkernel names from .symtab,
  // update m_kernel_name_to_id_map and m_kernel_to_subkernels_map, and record
  // all member section indices in m_section_to_group_map / m_group_to_sections_map.
  void
  parse_single_group_section(const ELFIO::section* sec)
  {
    const auto* data = sec->get_data();
    auto sz          = sec->get_size();
    auto group_id    = sec->get_index();

    if (!data || sz < sizeof(ELFIO::Elf_Word))
      return;

    auto [kernel_name, subkernel_name] =
      get_kernel_subkernel_from_symtab(sec->get_info());

    m_kernel_to_subkernels_map[kernel_name].push_back(subkernel_name);
    m_kernel_name_to_id_map[kernel_name + subkernel_name] = group_id;

    const auto* words = reinterpret_cast<const ELFIO::Elf_Word*>(data);
    auto word_count   = sz / sizeof(ELFIO::Elf_Word);
    std::vector<uint32_t> members;
    for (size_t i = 1; i < word_count; ++i) {
      auto mid = words[i];
      members.push_back(mid);
      m_section_to_group_map[mid] = group_id;
    }
    m_group_to_sections_map.emplace(group_id, std::move(members));
  }

  // Record zero-copy spans into ELFIO-owned memory for each custom section
  // (type SHT_LOUSER+1), keyed by section name.
  void
  parse_custom_sections(const std::vector<uint32_t>& ids)
  {
    for (auto id : ids) {
      auto* sec = m_elfio.sections[id];
      if (!sec)
        continue;

      m_custom_section_map[sec->get_name()] =
        std::span<const std::byte>(
          reinterpret_cast<const std::byte*>(sec->get_data()),
          sec->get_size());
    }
  }

  // Build m_kernels from m_kernel_args_map and m_kernel_to_subkernels_map.
  // Called after all .group sections have been parsed.
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
      if (!sec)
        continue;

      if (sec->get_type() == ELFIO::SHT_GROUP)
        parse_single_group_section(sec.get());
      else if (sec->get_type() == custom_type)
        custom_ids.push_back(sec->get_index());
    }
    finalize_kernels();
    parse_custom_sections(custom_ids);
    m_kernel_args_map.clear();
  }

  ////////////////////////////////////////////////////////////////
  // Relocation helpers
  ////////////////////////////////////////////////////////////////

  // Decode the r_addend field of a .rela.dyn entry into (schema, base_bo_offset).
  // ABI version 1 packs both into one 32-bit word (schema in bits [0:3],
  // base-bo offset in bits [4:31]); all other versions use r_type for schema
  // and r_addend directly for the offset.
  std::pair<elf::patch_schema, uint32_t>
  decode_addend(uint32_t rtype, int32_t r_addend, uint16_t abi_ver) const
  {
    if (abi_ver != 1)
      return { static_cast<elf::patch_schema>(rtype),
               static_cast<uint32_t>(r_addend) };

    // ABI version 1: schema in low bits, base-bo offset in high bits
    auto addend = static_cast<uint32_t>(r_addend);
    return { static_cast<elf::patch_schema>(addend & schema_mask),
             (addend & addend_mask) >> addend_shift };
  }

  ////////////////////////////////////////////////////////////////
  // Ctrl-code id resolution
  ////////////////////////////////////////////////////////////////

  // Look up the group index (ctrl-code-id) for a kernel name.
  // Accepts "kernel:subkernel" format or a bare kernel name when only one
  // subkernel exists.  has_ctrlcode is a platform-supplied predicate that
  // confirms the resolved id actually has a control code buffer.
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
      throw std::runtime_error("Multiple sub kernels, cannot choose: " + name);
    }

    throw std::runtime_error("Cannot get ctrlcode id from kernel name: " + name);
  }
};

////////////////////////////////////////////////////////////////
// elf_reader_aie2p — AIE2P (gen2) platform
////////////////////////////////////////////////////////////////
class elf_reader_aie2p : public elf_reader
{
public:
  // ctrl-code-id -> instruction buffer (.ctrltext.* sections)
  std::map<uint32_t, section_buf>            m_instr_buf_map;

  // ctrl-code-id -> control packet buffer (.ctrldata.* sections)
  std::map<uint32_t, section_buf>            m_ctrl_packet_map;

  // ctrl-code-id -> preemption save buffer (.preempt_save.* sections)
  std::map<uint32_t, section_buf>            m_save_buf_map;

  // ctrl-code-id -> preemption restore buffer (.preempt_restore.* sections)
  std::map<uint32_t, section_buf>            m_restore_buf_map;

  // PDI section name -> buffer (.pdi.* sections); keyed by name not ctrl-code-id
  // because multiple ctrl codes may reference the same PDI symbol
  std::map<std::string, section_buf>         m_pdi_buf_map;

  // ctrl-code-id -> set of PDI symbol names that need patching in that ctrl code
  std::map<uint32_t, std::unordered_set<std::string>> m_ctrl_pdi_map;

  // preemption ctrl-pkt section name -> buffer (.ctrlpkt.pm.* sections)
  std::map<std::string, section_buf>         m_ctrlpkt_pm_bufs;

  // set of .dynsym names that contain "ctrlpkt-pm"; used by the patching path
  // to identify which symbols require preemption ctrl-pkt patching
  std::set<std::string>                      m_ctrlpkt_pm_dynsyms;

  // size in bytes of the control scratch-pad memory region, derived from
  // the "scratch-pad-ctrl" dynsym st_size field; 0 if not present
  size_t                                     m_ctrl_scratch_pad_mem_size = 0;

  // true when at least one ctrl-code group has both .preempt_save and
  // .preempt_restore sections; drives ERT opcode selection
  bool                                       m_preemption_exist = false;

  bool
  is_group_elf() const override
  {
    auto [major, minor] = abi_version();
    return major >= 1;
  }

  uint32_t
  get_ctrlcode_id(const std::string& name) const override
  {
    return resolve_ctrlcode_id(name, [&](uint32_t id) {
      return m_instr_buf_map.count(id) || m_ctrl_packet_map.count(id);
    });
  }

  ////////////////////////////////////////////////////////////////
  // Buffer initialisation
  ////////////////////////////////////////////////////////////////

  // Collect all sections whose name contains the pattern for buf_type into out,
  // keyed by the group index of each section.
  void
  init_simple_buf_map(elf::buf_type type, std::map<uint32_t, section_buf>& out)
  {
    auto pat = section_pattern(type);
    for (const auto& sec : m_elfio.sections) {
      if (sec->get_name().find(pat) == std::string::npos)
        continue;

      out[m_section_to_group_map[sec->get_index()]].append(sec.get());
    }
  }

  // Populate m_save_buf_map and m_restore_buf_map per ctrl-code group.
  // Also validates that save and restore sections are always paired and sets
  // m_preemption_exist if any group has them.
  void
  init_save_restore()
  {
    auto save_pat    = section_pattern(elf::buf_type::preempt_save);
    auto restore_pat = section_pattern(elf::buf_type::preempt_restore);

    for (const auto& [gid, sec_ids] : m_group_to_sections_map) {
      bool has_save = false, has_restore = false;
      for (auto idx : sec_ids) {
        auto* sec = m_elfio.sections[idx];
        auto nm   = sec->get_name();
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
        throw std::runtime_error("Preempt save and restore sections are not paired");

      if (has_save && has_restore)
        m_preemption_exist = true;
    }
  }

  void
  init_pdi()
  {
    auto pat = section_pattern(elf::buf_type::pdi);
    for (const auto& sec : m_elfio.sections) {
      if (sec->get_name().find(pat) == std::string::npos)
        continue;

      m_pdi_buf_map[sec->get_name()].append(sec.get());
    }
  }

  void
  init_ctrlpkt_pm()
  {
    auto pat = section_pattern(elf::buf_type::ctrlpkt_pm);
    for (const auto& sec : m_elfio.sections) {
      if (sec->get_name().find(pat) == std::string::npos)
        continue;

      m_ctrlpkt_pm_bufs[sec->get_name()].append(sec.get());
    }
  }

  // Walk .rela.dyn relocations and build m_patch_points for each argument symbol.
  // Also extracts m_ctrl_scratch_pad_mem_size and m_ctrlpkt_pm_dynsyms as side-effects.
  void
  init_patchers()
  {
    static constexpr const char* scratch_pad_sym = "scratch-pad-ctrl";
    static constexpr const char* ctrlpkt_pm_sym  = "ctrlpkt-pm";

    auto* dynsym = m_elfio.sections[".dynsym"];
    auto* dynstr = m_elfio.sections[".dynstr"];
    auto* dynsec = m_elfio.sections[".rela.dyn"];
    if (!dynsym || !dynstr || !dynsec)
      return;

    auto abi_ver = static_cast<uint16_t>(m_elfio.get_abi_version());
    auto begin   = reinterpret_cast<const ELFIO::Elf32_Rela*>(dynsec->get_data());
    auto end     = begin + dynsec->get_size() / sizeof(ELFIO::Elf32_Rela);

    for (auto rela = begin; rela != end; ++rela) {
      auto symidx = ELFIO::get_sym_and_type<ELFIO::Elf32_Rela>::get_r_sym(rela->r_info);
      auto rtype  = ELFIO::get_sym_and_type<ELFIO::Elf32_Rela>::get_r_type(rela->r_info);

      auto dsym_off = symidx * sizeof(ELFIO::Elf32_Sym);
      if (dsym_off >= dynsym->get_size())
        throw std::runtime_error("Invalid symbol index " + std::to_string(symidx));

      auto* sym = reinterpret_cast<const ELFIO::Elf32_Sym*>(dynsym->get_data() + dsym_off);
      if (sym->st_name >= dynstr->get_size())
        throw std::runtime_error("Invalid symbol name offset");

      const char* symname = dynstr->get_data() + sym->st_name;

      if (!m_ctrl_scratch_pad_mem_size && std::strcmp(symname, scratch_pad_sym) == 0)
        m_ctrl_scratch_pad_mem_size = static_cast<size_t>(sym->st_size);

      if (std::string(symname).find(ctrlpkt_pm_sym) != std::string::npos)
        m_ctrlpkt_pm_dynsyms.emplace(symname);

      auto* patch_sec = m_elfio.sections[sym->st_shndx];
      if (!patch_sec)
        throw std::runtime_error("Invalid section index " + std::to_string(sym->st_shndx));

      auto sec_name = patch_sec->get_name();
      auto grp_idx  = m_section_to_group_map[patch_sec->get_index()];

      // Resolve buf type and accumulated section size
      auto [sec_size, btype] = [&]() -> std::pair<size_t, elf::buf_type> {
        auto match = [&](elf::buf_type t, auto& bmap) {
          return sec_name.find(section_pattern(t)) != std::string::npos
              && bmap.count(grp_idx);
        };
        if (match(elf::buf_type::ctrltext, m_instr_buf_map))
          return {m_instr_buf_map.at(grp_idx).size(), elf::buf_type::ctrltext};

        if (!m_ctrl_packet_map.empty() && match(elf::buf_type::ctrldata, m_ctrl_packet_map))
          return {m_ctrl_packet_map.at(grp_idx).size(), elf::buf_type::ctrldata};

        if (match(elf::buf_type::preempt_save, m_save_buf_map))
          return {m_save_buf_map.at(grp_idx).size(), elf::buf_type::preempt_save};

        if (match(elf::buf_type::preempt_restore, m_restore_buf_map))
          return {m_restore_buf_map.at(grp_idx).size(), elf::buf_type::preempt_restore};

        if (!m_pdi_buf_map.empty() &&
            sec_name.find(section_pattern(elf::buf_type::pdi)) != std::string::npos) {
          auto it = m_pdi_buf_map.find(sec_name);
          if (it == m_pdi_buf_map.end())
            throw std::runtime_error("PDI section not cached: " + sec_name);

          return {it->second.size(), elf::buf_type::pdi};
        }
        throw std::runtime_error("Unrecognised section in gen2 patcher: " + sec_name);
      }();

      auto offset = static_cast<uint64_t>(rela->r_offset);
      if (offset >= sec_size)
        throw std::runtime_error("Invalid relocation offset " + std::to_string(offset));

      if (sec_name.find("pdi") != std::string::npos)
        m_ctrl_pdi_map[grp_idx].insert(symname);

      auto [schema, add_end_addr] = decode_addend(rtype, rela->r_addend, abi_ver);
      uint32_t mask = (schema == elf::patch_schema::scalar_32bit)
        ? static_cast<uint32_t>(sym->st_size) : 0;

      std::string argnm{symname, symname + std::min(std::strlen(symname), dynstr->get_size())};
      elf::patch_point pp{argnm, schema, btype, offset, add_end_addr, mask};
      m_patch_points[grp_idx][make_key(argnm, btype)].push_back(std::move(pp));
    }
  }

  void
  init_all()
  {
    init_simple_buf_map(elf::buf_type::ctrltext, m_instr_buf_map);
    init_simple_buf_map(elf::buf_type::ctrldata, m_ctrl_packet_map);
    init_save_restore();
    init_pdi();
    init_ctrlpkt_pm();
    init_patchers();
  }
};

////////////////////////////////////////////////////////////////
// elf_reader_gen2plus — AIE2PS / AIE4 family
////////////////////////////////////////////////////////////////
class elf_reader_gen2plus : public elf_reader
{
public:
  // AIE column page size in bytes; ctrlcode sections are padded to this boundary
  static constexpr size_t elf_page_size = 8192;

  // ctrl-code-id -> vector of per-column control code buffers.
  // Index into the vector is the column (uC) index.
  // Each buffer contains ctrltext + ctrldata pages, padded to elf_page_size,
  // followed by any .pad section data.
  std::map<uint32_t, std::vector<section_buf>>           m_ctrlcodes_map;

  // ctrl-code-id -> (section name -> control packet buffer) for .ctrlpkt.* sections
  std::map<uint32_t, std::map<std::string, section_buf>> m_ctrlpkt_buf_map;

  // ctrl-code-id -> dump buffer (.dump.* sections); used for debug/trace decoding
  std::map<uint32_t, section_buf>                        m_dump_buf_map;

  bool
  is_group_elf() const override
  {
    auto [major, minor] = abi_version();
    return (major > 0) || (major == 0 && minor >= 3);
  }

  uint32_t
  get_ctrlcode_id(const std::string& name) const override
  {
    return resolve_ctrlcode_id(name, [&](uint32_t id) {
      return m_ctrlcodes_map.count(id) > 0;
    });
  }

  // ABI version 0x21 uses merged format: one .ctrltext.<col> section per column
  // contains all pages laid out at page*PAGE_SIZE offsets, with no separate
  // .ctrldata sections.  Earlier versions use per-page .ctrltext.<col>.<page> sections.
  bool
  is_merged_format() const
  {
    return m_elfio.get_abi_version() == 0x21; // NOLINT
  }

  ////////////////////////////////////////////////////////////////
  // Buffer initialisation
  ////////////////////////////////////////////////////////////////

  // Build m_ctrlcodes_map: for each ctrl-code group, assemble one section_buf
  // per column from its .ctrltext (and .ctrldata for per-page format) sections,
  // padding each page to elf_page_size, then appending any .pad sections.
  // pad_offsets[id][col] records the byte offset where .pad data begins within
  // that column's buffer; used by init_patchers() to compute absolute offsets.
  void
  init_column_ctrlcode(std::map<uint32_t, std::vector<size_t>>& pad_offsets)
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
        auto  nm  = sec->get_name();
        if (nm.find(ctrltext_pat) != std::string::npos) {
          auto [col, page] = get_col_page(nm, merged);
          ctrl_map[id][col][page].ctrltext = sec;
        }
        else if (nm.find(ctrldata_pat) != std::string::npos) {
          auto [col, page] = get_col_page(nm, merged);
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
            if (pg.ctrltext)
              m_ctrlcodes_map[id][ucidx].append(pg.ctrltext);

            if (pg.ctrldata)
              m_ctrlcodes_map[id][ucidx].append(pg.ctrldata);

            auto target = (page + 1) * elf_page_size;
            if (m_ctrlcodes_map[id][ucidx].size() < target)
              m_ctrlcodes_map[id][ucidx].pad_to(target);
          }
        }
        pad_offsets[id][ucidx] = m_ctrlcodes_map[id][ucidx].size();
      }
    }

    // Append .pad sections after main ctrlcode
    for (const auto& [id, sec_ids] : m_group_to_sections_map) {
      for (auto sidx : sec_ids) {
        auto* sec = m_elfio.sections[sidx];
        if (sec->get_name().find(pad_pat) == std::string::npos)
          continue;

        auto [col, page] = get_col_page(sec->get_name(), merged);
        m_ctrlcodes_map[id][col].append(sec);
      }
    }
  }

  void
  init_ctrlpkt()
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
  init_dump()
  {
    auto pat = section_pattern(elf::buf_type::dump);
    for (const auto& sec : m_elfio.sections) {
      if (sec->get_name().find(pat) == std::string::npos)
        continue;

      m_dump_buf_map[m_section_to_group_map[sec->get_index()]].append(sec.get());
    }
  }

  // Walk .rela.dyn and build m_patch_points.  Absolute offsets are computed
  // differently depending on whether the relocation targets a .pad section,
  // a .ctrlpkt section, or a .ctrltext (ctrlcode) section:
  //   pad:     sum of preceding column sizes + pad_offsets[grp][col] + r_offset
  //   ctrlpkt: r_offset directly into the ctrlpkt buffer
  //   ctrltext: sum of preceding column sizes + page*PAGE_SIZE + r_offset + 16
  //             (the +16 skips the per-page header)
  void
  init_patchers(const std::map<uint32_t, std::vector<size_t>>& pad_offsets)
  {
    auto pad_pat     = section_pattern(elf::buf_type::pad);
    auto ctrlpkt_pat = section_pattern(elf::buf_type::ctrlpkt);

    auto* dynsym = m_elfio.sections[".dynsym"];
    auto* dynstr = m_elfio.sections[".dynstr"];
    auto* dynsec = m_elfio.sections[".rela.dyn"];
    if (!dynsym || !dynstr || !dynsec)
      return;

    bool  merged  = is_merged_format();
    auto  abi_ver = static_cast<uint16_t>(m_elfio.get_abi_version());
    auto  begin   = reinterpret_cast<const ELFIO::Elf32_Rela*>(dynsec->get_data());
    auto  end     = begin + dynsec->get_size() / sizeof(ELFIO::Elf32_Rela);

    for (auto rela = begin; rela != end; ++rela) {
      auto symidx = ELFIO::get_sym_and_type<ELFIO::Elf32_Rela>::get_r_sym(rela->r_info);
      auto rtype  = ELFIO::get_sym_and_type<ELFIO::Elf32_Rela>::get_r_type(rela->r_info);

      auto dsym_off = symidx * sizeof(ELFIO::Elf32_Sym);
      if (dsym_off >= dynsym->get_size())
        throw std::runtime_error("Invalid symbol index " + std::to_string(symidx));

      auto* sym = reinterpret_cast<const ELFIO::Elf32_Sym*>(dynsym->get_data() + dsym_off);
      if (sym->st_name >= dynstr->get_size())
        throw std::runtime_error("Invalid symbol name offset");

      const char* symname = dynstr->get_data() + sym->st_name;
      std::string argnm{symname, symname + std::min(std::strlen(symname), dynstr->get_size())};

      auto* patch_sec = m_elfio.sections[sym->st_shndx];
      if (!patch_sec)
        throw std::runtime_error("Invalid section index " + std::to_string(sym->st_shndx));

      auto patch_sec_name = patch_sec->get_name();
      auto [col, page]    = get_col_page(patch_sec_name, merged);
      auto grp_idx        = m_section_to_group_map[patch_sec->get_index()];

      if (!m_ctrlcodes_map.count(grp_idx))
        throw std::runtime_error("No ctrlcode for symbol: " + argnm);

      const auto&   ctrlcodes  = m_ctrlcodes_map[grp_idx];
      uint64_t      abs_offset = 0;
      elf::buf_type btype      = elf::buf_type::buf_type_count;

      if (patch_sec_name.find(pad_pat) != std::string::npos) {
        for (uint32_t i = 0; i < col; ++i)
          abs_offset += ctrlcodes[i].size();

        abs_offset += pad_offsets.at(grp_idx)[col] + rela->r_offset;
        btype = elf::buf_type::pad;
      }
      else if (patch_sec_name.find(ctrlpkt_pat) != std::string::npos) {
        abs_offset = rela->r_offset;
        btype      = elf::buf_type::ctrlpkt;
        argnm     += strip_group_suffix(patch_sec_name);
      }
      else {
        auto col_size = ctrlcodes.at(col).size();
        auto sec_off  = page * elf_page_size + rela->r_offset + 16; // NOLINT
        if (sec_off >= col_size)
          throw std::runtime_error("Invalid ctrlcode offset " + std::to_string(sec_off));

        for (uint32_t i = 0; i < col; ++i)
          abs_offset += ctrlcodes.at(i).size();

        abs_offset += sec_off;
        btype = elf::buf_type::ctrltext;
      }

      auto [schema, add_end_addr] = decode_addend(rtype, rela->r_addend, abi_ver);
      elf::patch_point pp{argnm, schema, btype, abs_offset, add_end_addr, 0};
      m_patch_points[grp_idx][make_key(argnm, btype)].push_back(std::move(pp));
    }
  }

  void
  init_all()
  {
    std::map<uint32_t, std::vector<size_t>> pad_offsets;
    init_column_ctrlcode(pad_offsets);
    init_ctrlpkt();
    init_dump();
    init_patchers(pad_offsets);
  }

private:
  // Extract column and page indices from a section name of the form
  // .ctrltext.<col>[.<page>[.<grp_id>]].  In merged format the third token
  // is a group id, not a page index, so page is always 0.
  static std::pair<uint32_t, uint32_t>
  get_col_page(const std::string& name, bool merged)
  {
    constexpr size_t col_token  = 1;
    constexpr size_t page_token = 2;

    std::vector<std::string> tokens;
    std::stringstream ss(name);
    std::string tok;
    while (std::getline(ss, tok, '.'))
      if (!tok.empty())
        tokens.emplace_back(std::move(tok));

    try {
      if (tokens.size() <= col_token)
        return {0, 0};

      if (tokens.size() == col_token + 1)
        return {std::stoul(tokens[col_token]), 0};

      return {std::stoul(tokens[col_token]),
              merged ? 0 : std::stoul(tokens[page_token])};
    }
    catch (const std::exception&) {
      throw std::runtime_error("Invalid section name for col/page parse: " + name);
    }
  }
};

////////////////////////////////////////////////////////////////
// Factory — returns the correct subclass for the detected platform
////////////////////////////////////////////////////////////////
static std::unique_ptr<elf_reader>
make_reader(ELFIO::elfio elfio, std::string path)
{
  auto platform = elf_reader::detect_platform(elfio);

  if (platform == elf::platform::aie2p) {
    auto r        = std::make_unique<elf_reader_aie2p>();
    r->m_elfio    = std::move(elfio);
    r->m_path     = std::move(path);
    r->m_platform = platform;
    r->parse_sections();
    r->init_all();
    return r;
  }

  auto r        = std::make_unique<elf_reader_gen2plus>();
  r->m_elfio    = std::move(elfio);
  r->m_path     = std::move(path);
  r->m_platform = platform;
  r->parse_sections();
  r->init_all();
  return r;
}

////////////////////////////////////////////////////////////////
// aiebu::elf method implementations
////////////////////////////////////////////////////////////////

elf::elf(const std::string& filename)
  : m_reader{make_reader(elf_reader::load(filename), filename)}
{}

elf::elf(std::istream& stream)
  : m_reader{make_reader(elf_reader::load(stream), {})}
{}

elf::elf(const void* data, size_t size)
  : m_reader{make_reader(elf_reader::load(data, size), {})}
{}

elf::elf(std::string_view data)
  : elf(data.data(), data.size())
{}

elf::~elf() = default;
elf::elf(elf&&) noexcept = default;
elf& elf::operator=(elf&&) noexcept = default;

elf::platform
elf::get_platform() const { return m_reader->m_platform; }

uint8_t
elf::get_os_abi() const { return m_reader->m_elfio.get_os_abi(); }

std::pair<uint8_t, uint8_t>
elf::get_abi_version() const { return m_reader->abi_version(); }

bool
elf::is_full_elf() const
{
  return m_reader->m_elfio.sections[".note.xrt.configuration"] != nullptr;
}

bool
elf::is_group_elf() const { return m_reader->is_group_elf(); }

std::array<uint8_t, 16>
elf::get_cfg_uuid() const
{
  constexpr size_t uuid_size = 16;
  auto* sec = m_reader->m_elfio.sections[".note.xrt.UID"];
  if (!sec)
    throw std::runtime_error("ELF is missing .note.xrt.UID section");

  // NOLINTNEXTLINE
  ELFIO::note_section_accessor acc(m_reader->m_elfio, const_cast<ELFIO::section*>(sec));
  ELFIO::Elf_Word type = 0;
  std::string name;
  char* desc = nullptr;
  ELFIO::Elf_Word desc_size = 0;
  if (!acc.get_note(0, type, name, desc, desc_size))
    throw std::runtime_error("Failed to read UUID note");

  if (desc_size != uuid_size)
    throw std::runtime_error("UUID note wrong size: " + std::to_string(desc_size));

  std::array<uint8_t, 16> uuid{};
  std::memcpy(uuid.data(), desc, uuid_size);
  return uuid;
}

uint32_t
elf::get_partition_size() const
{
  auto* sec = m_reader->m_elfio.sections[".note.xrt.configuration"];
  if (!sec)
    throw std::runtime_error("ELF is missing xrt configuration info");

  // NOLINTNEXTLINE
  ELFIO::note_section_accessor acc(m_reader->m_elfio, const_cast<ELFIO::section*>(sec));
  ELFIO::Elf_Word type = 0;
  std::string name;
  char* desc = nullptr;
  ELFIO::Elf_Word desc_size = 0;
  if (!acc.get_note(0, type, name, desc, desc_size))
    throw std::runtime_error("Failed to read configuration note");

  uint32_t value = 0;
  std::memcpy(&value, desc, std::min(static_cast<size_t>(desc_size), sizeof(uint32_t)));
  return value;
}

std::vector<elf::kernel>
elf::get_kernels() const { return m_reader->m_kernels; }

std::span<const std::byte>
elf::get_section(std::string_view name) const
{
  auto nm = std::string(name);

  if (auto it = m_reader->m_custom_section_map.find(nm);
      it != m_reader->m_custom_section_map.end())
    return it->second;

  auto* sec = m_reader->m_elfio.sections[nm];
  if (!sec)
    return {};

  return std::span<const std::byte>(
    reinterpret_cast<const std::byte*>(sec->get_data()),
    sec->get_size());
}

void
elf::save(std::ostream& stream) const
{
  // NOLINTNEXTLINE
  const_cast<ELFIO::elfio&>(m_reader->m_elfio).save(stream);
}

std::map<uint32_t, uint32_t>
elf::get_section_to_group_map() const { return m_reader->m_section_to_group_map; }

std::map<uint32_t, std::vector<uint32_t>>
elf::get_group_to_sections_map() const { return m_reader->m_group_to_sections_map; }

std::map<std::string, uint32_t>
elf::get_kernel_name_to_id_map() const { return m_reader->m_kernel_name_to_id_map; }

uint32_t
elf::get_ctrlcode_id(const std::string& name) const
{
  return m_reader->get_ctrlcode_id(name);
}

size_t
elf::get_pdi_size(const std::string& symbol_name) const
{
  auto* r = dynamic_cast<elf_reader_aie2p*>(m_reader.get());
  if (!r)
    return 0;

  auto it = r->m_pdi_buf_map.find(symbol_name);
  return (it != r->m_pdi_buf_map.end()) ? it->second.size() : 0;
}

void
elf::copy_pdi(const std::string& symbol_name, std::span<std::byte> dest) const
{
  auto* r = dynamic_cast<elf_reader_aie2p*>(m_reader.get());
  if (!r)
    return;

  auto it = r->m_pdi_buf_map.find(symbol_name);
  if (it != r->m_pdi_buf_map.end())
    it->second.copy_to(dest);
}

std::set<std::string>
elf::get_ctrlpkt_pm_dynsyms() const
{
  auto* r = dynamic_cast<elf_reader_aie2p*>(m_reader.get());
  return r ? r->m_ctrlpkt_pm_dynsyms : std::set<std::string>{};
}

size_t
elf::get_ctrlpkt_pm_buf_size(const std::string& symbol_name) const
{
  auto* r = dynamic_cast<elf_reader_aie2p*>(m_reader.get());
  if (!r)
    return 0;

  auto it = r->m_ctrlpkt_pm_bufs.find(symbol_name);
  return (it != r->m_ctrlpkt_pm_bufs.end()) ? it->second.size() : 0;
}

void
elf::copy_ctrlpkt_pm_buf(const std::string& symbol_name, std::span<std::byte> dest) const
{
  auto* r = dynamic_cast<elf_reader_aie2p*>(m_reader.get());
  if (!r)
    return;

  auto it = r->m_ctrlpkt_pm_bufs.find(symbol_name);
  if (it != r->m_ctrlpkt_pm_bufs.end())
    it->second.copy_to(dest);
}

std::map<uint32_t, std::map<std::string, std::vector<elf::patch_point>>>
elf::get_patch_points() const { return m_reader->m_patch_points; }

size_t
elf::get_ctrl_scratch_pad_mem_size() const
{
  auto* r = dynamic_cast<elf_reader_aie2p*>(m_reader.get());
  return r ? r->m_ctrl_scratch_pad_mem_size : 0;
}

} // namespace aiebu
