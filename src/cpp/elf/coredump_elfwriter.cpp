// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.

#include "coredump_elfwriter.h"
#include "aie_elf_constants.h"

#include "elfio/elfio.hpp"

#include "aiebu/aiebu_error.h"

#include <boost/endian/conversion.hpp>
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
constexpr size_t prpsinfo_fname_len  = 16;   // pr_fname field width (Linux ABI)
constexpr size_t prpsinfo_psargs_len = 80;   // pr_psargs field width (Linux ABI)
constexpr size_t prpsinfo_struct_sz  = 124;  // total size of elf_prpsinfo32 (Linux ABI)

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
  char     pr_fname[prpsinfo_fname_len];   // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays) — Linux ABI
  char     pr_psargs[prpsinfo_psargs_len]; // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays) — Linux ABI
};                                         // total = 124
#pragma pack(pop)

static_assert(sizeof(elf_prpsinfo32) == prpsinfo_struct_sz,
              "elf_prpsinfo32 must be exactly 124 bytes");

// ---------------------------------------------------------------------------
// Descriptor-content helpers.
//
// append_le converts a host value to LE wire format and appends its bytes.
// append_lp_string writes a 4-byte LE length prefix then the string bytes.
// ---------------------------------------------------------------------------

#ifdef __GNUC__
#pragma GCC diagnostic push
#if defined(__has_warning)
#if __has_warning("-Wstringop-overflow")
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
#else
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
#endif
template <typename T>
static void append_le(std::vector<char>& v, T value)
{
  value = boost::endian::native_to_little(value);
  const auto* p = reinterpret_cast<const char*>(&value);
  v.insert(v.end(), p, p + sizeof(T));
}
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

static void append_lp_string(std::vector<char>& v, const std::string& s)
{
  append_le<uint32_t>(v, static_cast<uint32_t>(s.size()));
  v.insert(v.end(), s.begin(), s.end());
}

// ---------------------------------------------------------------------------
// make_note — build one ELF note entry.
//
// Note header fields (namesz, descsz, type) are written in LE byte order
// via append_le() so that the framing is correct on both LE and BE hosts.
//
// ELF note layout:
//   uint32_t namesz   (length of name including NUL, LE)
//   uint32_t descsz   (length of descriptor, LE)
//   uint32_t type     (note type, LE)
//   char     name[]   (namesz bytes, padded to 4-byte boundary)
//   char     desc[]   (descsz bytes, padded to 4-byte boundary)
// ---------------------------------------------------------------------------
static std::vector<char>
make_note(uint32_t                 type,
          const char*              name,
          const std::vector<char>& desc)
{
  // Number of padding bytes needed to align n to a 4-byte boundary.
  auto pad4 = [](uint32_t n) -> uint32_t { return (4U - (n & 3U)) & 3U; };

  const auto namesz = static_cast<uint32_t>(std::strlen(name)) + 1U;
  const auto descsz = static_cast<uint32_t>(desc.size());

  std::vector<char> entry;
  entry.reserve(3U * sizeof(uint32_t)
                + namesz + pad4(namesz)
                + descsz + pad4(descsz));

  // Write a uint32_t in LE byte order.
  auto emit = [&entry](uint32_t v) { append_le<uint32_t>(entry, v); };

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

// Map OS/ABI byte to the maximum expected coredump blob size for that architecture.
static uint32_t max_blob_size_for_abi(unsigned char abi)
{
  if (abi == osabi_aie2p)
    return coredump_load_size_aie2p;
  if (abi == osabi_aie2ps)
    return coredump_load_size_aie2ps;
  if (abi == osabi_aie4  ||
      abi == osabi_aie4a ||
      abi == osabi_aie4z)
    return coredump_load_size_aie4;
  throw error(error::error_code::invalid_buffer_type,
              "Unknown OS/ABI for coredump blob size validation");
}

coredump_elf_writer::
coredump_elf_writer(unsigned char                    abi,
                    const std::vector<char>&         blob,
                    std::optional<aie_coredump_meta> meta)
  : m_abi(abi), m_blob(blob), m_meta(std::move(meta))
{
  if (blob.size() > max_blob_size_for_abi(abi))
    throw error(error::error_code::invalid_input,
                "coredump blob size exceeds maximum for architecture");
}

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
  static_assert(fname.size() < prpsinfo_fname_len,  "fname too long for pr_fname");
  static_assert(fname.size() < prpsinfo_psargs_len, "fname too long for pr_psargs");
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
  append_le<uint32_t>(desc, static_cast<uint32_t>(meta.context_status));
  append_lp_string(desc, meta.driver_version);
  append_lp_string(desc, meta.fw_version);
  append_lp_string(desc, meta.device_info);
  append_lp_string(desc, meta.uuid);

  return desc;
}

// GCC 15 emits a false-positive -Wfree-nonheap-object when aggressive
// inlining of vector::push_back confuses its alias analysis.
#if defined(__GNUC__) && __GNUC__ >= 15
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfree-nonheap-object"
#endif
std::vector<char>
coredump_elf_writer::
finalize() const
{
  // ------------------------------------------------------------------
  // Build the ET_CORE ELF binary directly as raw bytes — no ELFIO
  // object, no section header table, no intermediate serialization.
  //
  // All ELF structure fields (header, phdrs, note framing) are written
  // in LE byte order via append_le() so byte order is correct on both
  // LE and BE hosts.
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

  // ---- build note entries ----
  constexpr ELFIO::Elf_Word nt_prpsinfo = 3U;

  auto prpsinfo_note = make_note(nt_prpsinfo,
                                 nt_name_core,
                                 build_prpsinfo_desc());

  std::vector<char> aie_note;
  if (m_meta.has_value())
    aie_note = make_note(static_cast<uint32_t>(nt_aie_dump_hdr),
                         nt_name_amdaie_core,
                         build_aie_dump_hdr_desc(*m_meta));

  // ---- compute file layout ----
  constexpr uint32_t elf_hdr_sz  = 52U;
  constexpr uint32_t phdr_sz     = 32U;
  constexpr uint32_t num_phdrs   = 2U;
  constexpr uint32_t note_off    = elf_hdr_sz + num_phdrs * phdr_sz;  // 0x74
  constexpr uint32_t load_align  = 0x1000U;
  constexpr uint32_t ei_pad_sz   = 7U;  // e_ident padding bytes (ELF spec, offsets 9–15)

  const auto note_size = static_cast<uint32_t>(prpsinfo_note.size()
                                               + aie_note.size());
  const uint32_t load_off  = (note_off + note_size + load_align - 1U)
                             & ~(load_align - 1U);
  const auto load_size = static_cast<uint32_t>(m_blob.size());

  std::vector<char> out;
  out.reserve(load_off + load_size);

  // Emit helpers — convert to LE then append bytes.
  auto append16 = [&out](uint16_t v) { append_le<uint16_t>(out, v); };
  auto append32 = [&out](uint32_t v) { append_le<uint32_t>(out, v); };

  // ---- ELF32 header ----
  // e_ident (16 bytes): single bytes, no endian conversion needed.
  out.push_back('\x7f'); out.push_back('E'); out.push_back('L'); out.push_back('F');
  out.push_back('\x01');                                // ELFCLASS32
  out.push_back('\x01');                                // ELFDATA2LSB
  out.push_back('\x01');                                // EV_CURRENT
  out.push_back(static_cast<char>(m_abi));              // EI_OSABI
  out.push_back(static_cast<char>(elf_version_config)); // EI_ABIVERSION
  out.insert(out.end(), ei_pad_sz, '\0');               // EI_PAD

  append16(static_cast<uint16_t>(ELFIO::ET_CORE));        // e_type
  append16(static_cast<uint16_t>(em_aiectrlcode));         // e_machine
  append32(1U);                                            // e_version (EV_CURRENT)
  append32(0U);                                            // e_entry
  append32(elf_hdr_sz);                                    // e_phoff
  append32(0U);                                            // e_shoff  = 0 (no SHT)
  append32(0U);                                            // e_flags
  append16(static_cast<uint16_t>(elf_hdr_sz));             // e_ehsize
  append16(static_cast<uint16_t>(phdr_sz));                // e_phentsize
  append16(static_cast<uint16_t>(num_phdrs));              // e_phnum
  append16(0U);                                            // e_shentsize = 0 (no SHT)
  append16(0U);                                            // e_shnum     = 0 (no SHT)
  append16(0U);                                            // e_shstrndx  = 0 (no SHT)

  // ---- PT_NOTE program header ----
  append32(static_cast<uint32_t>(ELFIO::PT_NOTE));  // p_type
  append32(note_off);                                // p_offset
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
  append32(load_align);                              // p_align

  // ---- note blob ----
  out.insert(out.end(), prpsinfo_note.begin(), prpsinfo_note.end());
  out.insert(out.end(), aie_note.begin(),      aie_note.end());

  // ---- zero pad to page boundary ----
  out.insert(out.end(), load_off - static_cast<uint32_t>(out.size()), '\0');

  // ---- raw AIE dump payload ----
  out.insert(out.end(), m_blob.begin(), m_blob.end());

  if (out.size() != load_off + load_size)
    throw error(error::error_code::internal_error,
                "ELF output size mismatch");

  return out;
}
#if defined(__GNUC__) && __GNUC__ >= 15
#pragma GCC diagnostic pop
#endif

// ---------------------------------------------------------------------------
// parse_coredump_meta — inverse of build_aie_dump_hdr_desc
// ---------------------------------------------------------------------------

// Deserialize one NT_AIE_DUMP_HDR descriptor blob into aie_coredump_meta.
// Throws std::out_of_range if the descriptor is truncated.
static aie_coredump_meta
parse_aie_dump_hdr(const char* desc, uint32_t descsz)
{
  // Descriptor is always little-endian on the wire; little_to_native converts to host.
  size_t off = 0;

  auto consume_u64 = [&]() -> uint64_t {
    if (off + sizeof(uint64_t) > descsz) throw std::out_of_range("NT_AIE_DUMP_HDR descriptor truncated");
    uint64_t v = 0; std::memcpy(&v, desc + off, sizeof(uint64_t)); off += sizeof(uint64_t);
    return boost::endian::little_to_native(v);
  };
  auto consume_u32 = [&]() -> uint32_t {
    if (off + sizeof(uint32_t) > descsz) throw std::out_of_range("NT_AIE_DUMP_HDR descriptor truncated");
    uint32_t v = 0; std::memcpy(&v, desc + off, sizeof(uint32_t)); off += sizeof(uint32_t);
    return boost::endian::little_to_native(v);
  };
  auto consume_str = [&]() -> std::string {
    uint32_t len = consume_u32();
    if (off + len > descsz) throw std::out_of_range("NT_AIE_DUMP_HDR descriptor truncated");
    std::string s(desc + off, len); off += len;
    return s;
  };

  aie_coredump_meta meta;
  meta.timestamp_ns   = consume_u64();
  meta.context_status = static_cast<aie_context_status>(consume_u32());
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

  // AIE coredump ELFs are always LE on the wire; little_to_native converts to host.
  auto pad4 = [](uint32_t n) -> uint32_t { return (4U - (n & 3U)) & 3U; };

  // Reject oversized descriptors before allocating any string memory.
  // Actual max: 8 (timestamp) + 4 (context_status)
  //           + 4 strings × (4-byte length prefix + content):
  //               driver_version ~256 + fw_version ~256 + device_info ~256 + uuid 36
  //           ≈ 832 bytes.  2048 gives ~2× headroom for future fields.
  constexpr uint32_t max_aie_dump_hdr_desc_sz = 2048U;

  for (const auto& seg : elf.segments) {
    if (seg->get_type() != ELFIO::PT_NOTE)
      continue;

    const char* data = seg->get_data();
    const auto  size = static_cast<size_t>(seg->get_file_size());
    if (data == nullptr || size == 0)
      continue;
    size_t off = 0;

    // ELF note header: namesz (4) + descsz (4) + type (4) = 12 bytes.
    constexpr size_t note_hdr_fields = 3;  // namesz, descsz, type
    constexpr size_t note_hdr_sz = note_hdr_fields * sizeof(uint32_t);

    while (off + note_hdr_sz <= size) {
      uint32_t namesz = 0, descsz = 0, type = 0;
      std::memcpy(&namesz, data + off,                        sizeof(uint32_t));
      std::memcpy(&descsz, data + off +     sizeof(uint32_t), sizeof(uint32_t));
      std::memcpy(&type,   data + off + 2 * sizeof(uint32_t), sizeof(uint32_t));
      namesz = boost::endian::little_to_native(namesz);
      descsz = boost::endian::little_to_native(descsz);
      type   = boost::endian::little_to_native(type);
      off += note_hdr_sz;

      // namesz == 0 is malformed
      // namesz extending past the segment is also malformed.
      if (namesz == 0 || off + namesz > size)
        break;

      // Read name (namesz bytes including NUL terminator), strip NUL.
      std::string name(data + off, namesz - 1);

      // Advance past name + 4-byte padding; guard against uint32_t overflow.
      const uint32_t name_advance = namesz + pad4(namesz);
      if (name_advance < namesz || off + name_advance > size)
        break;
      off += name_advance;

      if (name == nt_name_amdaie_core &&
          type == static_cast<uint32_t>(nt_aie_dump_hdr) &&
          off + descsz <= size) {
        if (descsz > max_aie_dump_hdr_desc_sz)
          return std::nullopt;  // reject oversized descriptor — possible heap exhaustion attack
        try {
          return parse_aie_dump_hdr(data + off, descsz);
        } catch (const std::out_of_range&) {
          return std::nullopt;  // malformed descriptor
        }
      }

      // Advance past descriptor + 4-byte padding; guard against uint32_t overflow
      // and running past the segment end.
      const uint32_t desc_advance = descsz + pad4(descsz);
      if (desc_advance < descsz || off + desc_advance > size)
        break;
      off += desc_advance;
    }
  }
  return std::nullopt;
}

} // namespace aiebu
