// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.

#include "coredump_elfwriter.h"
#include "aie_elf_constants.h"

#include "elfio/elfio.hpp"

#include <cstdint>
#include <cstring>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace aiebu {

// ---------------------------------------------------------------------------
// elf_prpsinfo32 – mirrors the Linux kernel's elf_prpsinfo structure for
// 32-bit ELFs (ILP32).  Must be exactly 124 bytes.
// ---------------------------------------------------------------------------
constexpr size_t PRPSINFO_FNAME_LEN  = 16;   // pr_fname field width (Linux ABI)
constexpr size_t PRPSINFO_PSARGS_LEN = 80;   // pr_psargs field width (Linux ABI)
constexpr size_t PRPSINFO_STRUCT_SZ  = 124;  // total size of elf_prpsinfo32 (Linux ABI)

#pragma pack(push, 1)
struct elf_prpsinfo32 {
  uint8_t  pr_state;                    // 1   numeric process state
  char     pr_sname;                    // 1   process state letter
  uint8_t  pr_zomb;                     // 1   zombie
  uint8_t  pr_nice;                     // 1   nice value
  uint32_t pr_flag;                     // 4   process flags
  uint16_t pr_uid;                      // 2   effective uid
  uint16_t pr_gid;                      // 2   effective gid
  int32_t  pr_pid;                      // 4   pid
  int32_t  pr_ppid;                     // 4   parent pid
  int32_t  pr_pgrp;                     // 4   process group
  int32_t  pr_sid;                      // 4   session
  char     pr_fname[PRPSINFO_FNAME_LEN];   // NOLINT(cppcoreguidelines-avoid-c-arrays) — Linux ABI
  char     pr_psargs[PRPSINFO_PSARGS_LEN]; // NOLINT(cppcoreguidelines-avoid-c-arrays) — Linux ABI
};                                         // total = 124
#pragma pack(pop)

static_assert(sizeof(elf_prpsinfo32) == PRPSINFO_STRUCT_SZ,
              "elf_prpsinfo32 must be exactly 124 bytes");

// ---------------------------------------------------------------------------
// Descriptor-content helpers.
//
// These write the payload bytes of each note.  The AIE coredump descriptor
// wire format is defined as little-endian, so append_le is intentional here
// and not a portability concern.
// ---------------------------------------------------------------------------

template <typename T>
static void append_le(std::vector<char>& v, T value)
{
  const auto* p = reinterpret_cast<const char*>(&value);
  v.insert(v.end(), p, p + sizeof(T));
}

static void append_lp_string(std::vector<char>& v, const std::string& s)
{
  append_le<uint32_t>(v, static_cast<uint32_t>(s.size()));
  v.insert(v.end(), s.begin(), s.end());
}

// ---------------------------------------------------------------------------
// make_note — build one ELF note entry.
//
// Uses ELFIO's endianess_convertor for the namesz/descsz/type header fields
// so that the note framing is correct on both LE and BE hosts.
//
// ELF note layout:
//   uint32_t namesz   (length of name including NUL, endian-converted)
//   uint32_t descsz   (length of descriptor, endian-converted)
//   uint32_t type     (note type, endian-converted)
//   char     name[]   (namesz bytes, padded to 4-byte boundary)
//   char     desc[]   (descsz bytes, padded to 4-byte boundary)
// ---------------------------------------------------------------------------
static std::vector<char>
make_note(const ELFIO::endianess_convertor& conv,
          uint32_t                          type,
          const char*                       name,
          const std::vector<char>&          desc)
{
  // Number of padding bytes needed to align n to a 4-byte boundary.
  auto pad4 = [](uint32_t n) -> uint32_t { return (4U - (n & 3U)) & 3U; };

  const uint32_t namesz = static_cast<uint32_t>(std::strlen(name)) + 1U;
  const uint32_t descsz = static_cast<uint32_t>(desc.size());

  std::vector<char> entry;
  entry.reserve(3U * sizeof(uint32_t)
                + namesz + pad4(namesz)
                + descsz + pad4(descsz));

  // Write a uint32_t through the convertor (no-op on LE hosts).
  auto emit = [&entry, &conv](uint32_t v) {
    v = conv(v);
    const auto* p = reinterpret_cast<const char*>(&v);
    entry.insert(entry.end(), p, p + sizeof(v));
  };

  emit(namesz);
  emit(descsz);
  emit(type);

  // name + NUL terminator (namesz bytes total), then padding
  entry.insert(entry.end(), name, name + namesz);
  entry.insert(entry.end(), pad4(namesz), '\0');

  // descriptor, then padding
  entry.insert(entry.end(), desc.begin(), desc.end());
  entry.insert(entry.end(), pad4(descsz), '\0');

  return entry;
}

// ---------------------------------------------------------------------------
// coredump_elf_writer
// ---------------------------------------------------------------------------

coredump_elf_writer::
coredump_elf_writer(unsigned char                    abi,
                    const std::vector<char>&         blob,
                    std::optional<aie_coredump_meta> meta)
  : m_abi(abi), m_blob(blob), m_meta(std::move(meta))
{}

std::vector<char>
coredump_elf_writer::
build_prpsinfo_desc() const
{
  elf_prpsinfo32 info{};
  std::memset(&info, 0, sizeof(info));

  info.pr_state = 0;
  info.pr_sname = 'R';
  info.pr_pid   = 1;

  static constexpr std::string_view fname = "aie_coredump";
  static_assert(fname.size() < PRPSINFO_FNAME_LEN,  "fname too long for pr_fname");
  static_assert(fname.size() < PRPSINFO_PSARGS_LEN, "fname too long for pr_psargs");
  std::memcpy(info.pr_fname,  fname.data(), fname.size());
  std::memcpy(info.pr_psargs, fname.data(), fname.size());

  const auto* p = reinterpret_cast<const char*>(&info);
  return {p, p + sizeof(info)};
}

std::vector<char>
coredump_elf_writer::
build_aie_dump_hdr_desc(const aie_coredump_meta& meta) const
{
  std::vector<char> desc;

  // AIE coredump wire format (all fields little-endian by definition):
  //   uint64_t  timestamp_ns
  //   uint32_t  context_status
  //   uint32_t  len(driver_version) + bytes
  //   uint32_t  len(fw_version)     + bytes
  //   uint32_t  len(device_info)    + bytes
  //   uint32_t  len(uuid)           + bytes
  append_le<uint64_t>(desc, meta.timestamp_ns);
  append_le<uint32_t>(desc, meta.context_status);
  append_lp_string(desc, meta.driver_version);
  append_lp_string(desc, meta.fw_version);
  append_lp_string(desc, meta.device_info);
  append_lp_string(desc, meta.uuid);

  return desc;
}

std::vector<char>
coredump_elf_writer::
finalize() const
{
  // ------------------------------------------------------------------
  // Build the ET_CORE ELF binary directly as raw bytes — no ELFIO
  // object, no section header table, no intermediate serialization.
  //
  // ELFIO's endianess_convertor is used for all ELF structure fields
  // (header + program headers + note framing) so byte order is correct
  // on both LE and BE hosts.
  //
  // Layout (ELF32 LE, e_shoff = e_shnum = e_shentsize = 0):
  //
  //   [0x000]  ELF32 header      (52 bytes)
  //   [0x034]  PT_NOTE phdr      (32 bytes)
  //   [0x054]  PT_LOAD phdr      (32 bytes)
  //   [0x074]  note blob         (variable, internally 4-byte padded)
  //   [???  ]  zero pad to next 0x1000 boundary
  //   [???  ]  raw AIE dump      (m_blob.size() bytes)
  // ------------------------------------------------------------------

  // Endianness convertor: no-op on LE hosts, swaps on BE hosts.
  ELFIO::endianess_convertor conv;
  conv.setup(ELFIO::ELFDATA2LSB);

  // ---- build note entries ----
  constexpr ELFIO::Elf_Word NT_PRPSINFO = 3u;

  auto prpsinfo_note = make_note(conv, NT_PRPSINFO,
                                 nt_name_core,
                                 build_prpsinfo_desc());

  std::vector<char> aie_note;
  if (m_meta.has_value())
    aie_note = make_note(conv, static_cast<uint32_t>(nt_aie_dump_hdr),
                         nt_name_amdaie_core,
                         build_aie_dump_hdr_desc(*m_meta));

  // ---- compute file layout ----
  constexpr uint32_t ELF_HDR_SZ  = 52u;
  constexpr uint32_t PHDR_SZ     = 32u;
  constexpr uint32_t NUM_PHDRS   = 2u;
  constexpr uint32_t NOTE_OFF    = ELF_HDR_SZ + NUM_PHDRS * PHDR_SZ;  // 0x74
  constexpr uint32_t LOAD_ALIGN  = 0x1000u;

  const uint32_t note_size = static_cast<uint32_t>(prpsinfo_note.size()
                                                    + aie_note.size());
  const uint32_t load_off  = (NOTE_OFF + note_size + LOAD_ALIGN - 1u)
                             & ~(LOAD_ALIGN - 1u);
  const uint32_t load_size = static_cast<uint32_t>(m_blob.size());

  std::vector<char> out;
  out.reserve(load_off + load_size);

  // Emit helpers — apply convertor then append bytes.
  auto append16 = [&out, &conv](uint16_t v) {
    v = conv(v);
    const auto* p = reinterpret_cast<const char*>(&v);
    out.insert(out.end(), p, p + sizeof(v));
  };
  auto append32 = [&out, &conv](uint32_t v) {
    v = conv(v);
    const auto* p = reinterpret_cast<const char*>(&v);
    out.insert(out.end(), p, p + sizeof(v));
  };

  // ---- ELF32 header ----
  // e_ident (16 bytes): single bytes, no endian conversion needed.
  out.push_back('\x7f'); out.push_back('E'); out.push_back('L'); out.push_back('F');
  out.push_back('\x01');                                // ELFCLASS32
  out.push_back('\x01');                                // ELFDATA2LSB
  out.push_back('\x01');                                // EV_CURRENT
  out.push_back(static_cast<char>(m_abi));              // EI_OSABI
  out.push_back(static_cast<char>(elf_version_config)); // EI_ABIVERSION
  out.insert(out.end(), 7, '\0');                       // EI_PAD

  append16(static_cast<uint16_t>(ELFIO::ET_CORE));        // e_type
  append16(static_cast<uint16_t>(em_aiectrlcode));         // e_machine
  append32(1U);                                            // e_version (EV_CURRENT)
  append32(0U);                                            // e_entry
  append32(ELF_HDR_SZ);                                   // e_phoff
  append32(0U);                                            // e_shoff  = 0 (no SHT)
  append32(0U);                                            // e_flags
  append16(static_cast<uint16_t>(ELF_HDR_SZ));            // e_ehsize
  append16(static_cast<uint16_t>(PHDR_SZ));               // e_phentsize
  append16(static_cast<uint16_t>(NUM_PHDRS));              // e_phnum
  append16(0U);                                            // e_shentsize = 0 (no SHT)
  append16(0U);                                            // e_shnum     = 0 (no SHT)
  append16(0U);                                            // e_shstrndx  = 0 (no SHT)

  // ---- PT_NOTE program header ----
  append32(static_cast<uint32_t>(ELFIO::PT_NOTE));  // p_type
  append32(NOTE_OFF);                                // p_offset
  append32(0U);                                      // p_vaddr
  append32(0U);                                      // p_paddr
  append32(note_size);                               // p_filesz
  append32(0U);                                      // p_memsz  = 0 (notes not loaded)
  append32(static_cast<uint32_t>(ELFIO::PF_R));     // p_flags
  append32(4U);                                      // p_align

  // ---- PT_LOAD program header ----
  append32(static_cast<uint32_t>(ELFIO::PT_LOAD));  // p_type
  append32(load_off);                                // p_offset
  append32(0U);                                      // p_vaddr
  append32(0U);                                      // p_paddr
  append32(load_size);                               // p_filesz
  append32(load_size);                               // p_memsz
  append32(static_cast<uint32_t>(ELFIO::PF_R));     // p_flags
  append32(LOAD_ALIGN);                              // p_align

  // ---- note blob ----
  out.insert(out.end(), prpsinfo_note.begin(), prpsinfo_note.end());
  out.insert(out.end(), aie_note.begin(),      aie_note.end());

  // ---- zero pad to page boundary ----
  out.insert(out.end(), load_off - static_cast<uint32_t>(out.size()), '\0');

  // ---- raw AIE dump payload ----
  out.insert(out.end(), m_blob.begin(), m_blob.end());

  return out;
}

// ---------------------------------------------------------------------------
// parse_coredump_meta — inverse of build_aie_dump_hdr_desc
// ---------------------------------------------------------------------------

// Deserialize one NT_AIE_DUMP_HDR descriptor blob into aie_coredump_meta.
// Throws std::out_of_range if the descriptor is truncated.
static aie_coredump_meta
parse_aie_dump_hdr(const char* desc, uint32_t descsz)
{
  // Descriptor is always little-endian by definition.
  ELFIO::endianess_convertor conv;
  conv.setup(ELFIO::ELFDATA2LSB);

  size_t off = 0;

  auto consume_u64 = [&]() -> uint64_t {
    if (off + 8 > descsz) throw std::out_of_range("NT_AIE_DUMP_HDR descriptor truncated");
    uint64_t v; std::memcpy(&v, desc + off, 8); off += 8;
    return conv(v);
  };
  auto consume_u32 = [&]() -> uint32_t {
    if (off + 4 > descsz) throw std::out_of_range("NT_AIE_DUMP_HDR descriptor truncated");
    uint32_t v; std::memcpy(&v, desc + off, 4); off += 4;
    return conv(v);
  };
  auto consume_str = [&]() -> std::string {
    uint32_t len = consume_u32();
    if (off + len > descsz) throw std::out_of_range("NT_AIE_DUMP_HDR descriptor truncated");
    std::string s(desc + off, len); off += len;
    return s;
  };

  aie_coredump_meta meta;
  meta.timestamp_ns   = consume_u64();
  meta.context_status = consume_u32();
  meta.driver_version = consume_str();
  meta.fw_version     = consume_str();
  meta.device_info    = consume_str();
  meta.uuid           = consume_str();
  return meta;
}

std::optional<aie_coredump_meta>
parse_coredump_meta(const std::vector<char>& elf_bytes)
{
  ELFIO::elfio elf;
  std::string s(elf_bytes.begin(), elf_bytes.end());
  std::istringstream stream(s);
  if (!elf.load(stream))
    return std::nullopt;
  if (elf.get_type() != ELFIO::ET_CORE)
    return std::nullopt;

  // ELF note header fields use the ELF's own encoding.
  ELFIO::endianess_convertor hdr_conv;
  hdr_conv.setup(elf.get_encoding());

  auto pad4 = [](uint32_t n) -> uint32_t { return (4u - (n & 3u)) & 3u; };

  for (ELFIO::Elf_Half i = 0; i < elf.segments.size(); ++i) {
    const auto* seg = elf.segments[i];
    if (seg->get_type() != ELFIO::PT_NOTE)
      continue;

    const char*  data = seg->get_data();
    const size_t size = static_cast<size_t>(seg->get_file_size());
    size_t off = 0;

    while (off + 12 <= size) {
      uint32_t namesz, descsz, type;
      std::memcpy(&namesz, data + off,     4);
      std::memcpy(&descsz, data + off + 4, 4);
      std::memcpy(&type,   data + off + 8, 4);
      namesz = hdr_conv(namesz);
      descsz = hdr_conv(descsz);
      type   = hdr_conv(type);
      off += 12;

      // Read name (namesz bytes including NUL terminator), strip NUL.
      std::string name;
      if (namesz > 0 && off + namesz <= size)
        name.assign(data + off, namesz - 1);
      off += namesz + pad4(namesz);

      if (name == nt_name_amdaie_core &&
          type == static_cast<uint32_t>(nt_aie_dump_hdr) &&
          off + descsz <= size) {
        try {
          return parse_aie_dump_hdr(data + off, descsz);
        } catch (const std::out_of_range&) {
          return std::nullopt;  // malformed descriptor
        }
      }
      off += descsz + pad4(descsz);
    }
  }
  return std::nullopt;
}

} // namespace aiebu
