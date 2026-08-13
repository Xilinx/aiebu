// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

#include "elf_compression.h"

#include <cstdint>
#include <cstring>
#include <limits>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "elfio/elfio.hpp"
#include <zstd.h>

#include <boost/endian/conversion.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>

namespace {

// ---------------------------------------------------------------------------
// Section name helpers
// ---------------------------------------------------------------------------

constexpr std::string_view kCtrltext = ".ctrltext";
constexpr std::string_view kCtrldata = ".ctrldata";
constexpr std::string_view kCtrlpkt  = ".ctrlpkt";
constexpr std::string_view kDump     = ".dump";

inline bool starts_with(std::string_view value, std::string_view prefix)
{
  return value.size() >= prefix.size()
      && value.compare(0, prefix.size(), prefix) == 0;
}

inline bool is_compressible_section(std::string_view name)
{
  return starts_with(name, kCtrltext)
      || starts_with(name, kCtrldata)
      || starts_with(name, kCtrlpkt);
}


// ---------------------------------------------------------------------------
// ELF compression constants
// ---------------------------------------------------------------------------

constexpr ELFIO::Elf_Word ELFCOMPRESS_ZSTD_VAL = 2;

// ---------------------------------------------------------------------------
// zstd compression
// ---------------------------------------------------------------------------

std::vector<std::uint8_t> compress_buffer_zstd(
    const std::uint8_t* data, std::size_t size, int level)
{
  const std::size_t bound = ZSTD_compressBound(size);
  std::vector<std::uint8_t> output(bound);

  const std::size_t written = ZSTD_compress(
      output.data(), output.size(), data, size, level);
  if (ZSTD_isError(written)) {
    throw std::runtime_error(
        std::string("zstd compress failed: ") + ZSTD_getErrorName(written));
  }

  output.resize(written);
  return output;
}

// ---------------------------------------------------------------------------
// ELF Chdr wrapper
// ---------------------------------------------------------------------------

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
std::vector<std::uint8_t> wrap_elf_compressed(
    unsigned char elf_class,
    ELFIO::Elf_Word ch_type,
    std::uint64_t uncompressed_size,
    ELFIO::Elf_Xword addralign,
    const std::vector<std::uint8_t>& compressed)
// NOLINTEND(bugprone-easily-swappable-parameters)
{
  std::vector<std::uint8_t> result;

  if (elf_class == ELFIO::ELFCLASS64) {
    ELFIO::Elf64_Chdr header{};
    header.ch_type      = ch_type;
    header.ch_reserved  = 0;
    header.ch_size      = uncompressed_size;
    header.ch_addralign = addralign;

    result.resize(sizeof(header) + compressed.size());
    std::memcpy(result.data(), &header, sizeof(header));
    std::memcpy(result.data() + sizeof(header), compressed.data(), compressed.size());
    return result;
  }

  if (elf_class == ELFIO::ELFCLASS32) {
    constexpr std::uint64_t kMax32 = std::numeric_limits<std::uint32_t>::max();
    if (uncompressed_size > kMax32)
      throw std::runtime_error("uncompressed section too large for Elf32_Chdr");
    if (addralign > kMax32)
      throw std::runtime_error("section alignment too large for Elf32_Chdr");

    ELFIO::Elf32_Chdr header{};
    header.ch_type      = ch_type;
    header.ch_size      = static_cast<ELFIO::Elf32_Word>(uncompressed_size);
    header.ch_addralign = static_cast<ELFIO::Elf32_Word>(addralign);

    result.resize(sizeof(header) + compressed.size());
    std::memcpy(result.data(), &header, sizeof(header));
    std::memcpy(result.data() + sizeof(header), compressed.data(), compressed.size());
    return result;
  }

  throw std::runtime_error("unsupported ELF class");
}

// ---------------------------------------------------------------------------
// Section flag helpers
// ---------------------------------------------------------------------------

ELFIO::Elf_Xword compressed_section_flags(ELFIO::Elf_Xword original_flags)
{
  ELFIO::Elf_Xword flags = original_flags;
  flags &= ~ELFIO::SHF_ALLOC;
  flags &= ~ELFIO::SHF_EXECINSTR;
  flags |=  ELFIO::SHF_COMPRESSED;
  return flags;
}

// ---------------------------------------------------------------------------
// Section link/info remapping.
//
// Called after all sections have been added to the writer.  For each section
// the caller first copies sh_link and sh_info verbatim from src to dst; this
// function then overwrites the fields that hold section indices (not symbol
// counts or other values) with the remapped destination indices.
//
// sh_info semantics by section type:
//   SHT_REL / SHT_RELA       — sh_info = section index of the section to
//                              which relocations apply → must remap
//   SHT_HASH / SHT_GROUP /
//   SHT_SYMTAB_SHNDX         — sh_info = section index → must remap
//   SHT_SYMTAB / SHT_DYNSYM  — sh_info = one-past-last-local-symbol count,
//                              NOT a section index → verbatim copy is correct,
//                              no remap needed (handled by caller's pre-copy)
//   everything else           — sh_info not used or zero → no action needed
// ---------------------------------------------------------------------------

void remap_section_references(
    const ELFIO::section* src,
    ELFIO::section* dst,
    const std::map<ELFIO::Elf_Half, ELFIO::Elf_Half>& index_map)
{
  const auto remap = [&](ELFIO::Elf_Word index) -> ELFIO::Elf_Word {
    const auto it = index_map.find(static_cast<ELFIO::Elf_Half>(index));
    if (it == index_map.end())
      throw std::runtime_error(
          "missing section index remap for link/info reference: "
          + std::to_string(index));
    return it->second;
  };

  // sh_link is always a section index when non-zero.
  const ELFIO::Elf_Word link = src->get_link();
  if (link != 0 && link != ELFIO::SHN_UNDEF)
    dst->set_link(remap(link));

  // sh_info is a section index only for the types listed above.
  const ELFIO::Elf_Word info = src->get_info();
  switch (src->get_type()) {
  case ELFIO::SHT_REL:
  case ELFIO::SHT_RELA:
  case ELFIO::SHT_HASH:
  case ELFIO::SHT_GROUP:
  case ELFIO::SHT_SYMTAB_SHNDX:
    if (info != 0 && info != ELFIO::SHN_UNDEF)
      dst->set_info(remap(info));
    break;
  default:
    // All other types: sh_info was already copied verbatim by the caller.
    break;
  }
}

// ---------------------------------------------------------------------------
// Shared helper: recreate program headers in the writer.
//
// Instead of copying stale file_size / memory_size / offset values from the
// reader (which become wrong after section sizes change due to compression or
// decompression), this function creates fresh PT_LOAD segments from the
// writer's own sections — one per SHF_ALLOC section, matching the original
// elfwriter.cpp pattern (1:1 section-to-segment mapping).  ELFIO computes
// correct offsets and sizes from the linked sections when saving.
//
// Compressed sections have SHF_ALLOC cleared (standard for SHF_COMPRESSED),
// so they correctly get no PT_LOAD segment.  Decompressed sections have
// SHF_ALLOC restored, so their PT_LOAD segments are recreated.
// ---------------------------------------------------------------------------

void recreate_segments(ELFIO::elfio& writer)
{
  for (const auto& sec_ptr : writer.sections) {
    const ELFIO::section* sec = sec_ptr.get();
    if (!(sec->get_flags() & ELFIO::SHF_ALLOC))
      continue;

    ELFIO::segment* seg = writer.segments.add();
    seg->set_type(ELFIO::PT_LOAD);
    if (sec->get_flags() & ELFIO::SHF_EXECINSTR)
      seg->set_flags(ELFIO::PF_X | ELFIO::PF_R);
    else
      seg->set_flags(ELFIO::PF_W | ELFIO::PF_R);
    seg->set_align(sec->get_addr_align());
    seg->set_virtual_address(sec->get_address());
    seg->set_physical_address(0);
    seg->add_section_index(sec->get_index(), sec->get_addr_align());
  }
}

// ---------------------------------------------------------------------------
// Core zstd ELF compression pipeline — rebuild approach.
// Loads the ELF into a reader, compresses eligible sections, then writes a
// fresh ELFIO writer so ELFIO recomputes all section offsets from scratch.
// ---------------------------------------------------------------------------

std::pair<std::vector<char>, aiebu::compress_stats>
compress_elf_zstd(const std::vector<char>& elf_bytes, int level)
{
  boost::interprocess::ibufferstream iss(elf_bytes.data(), elf_bytes.size());
  ELFIO::elfio reader;
  if (!reader.load(iss))
    throw std::runtime_error("ZstdElfCompressor: failed to parse ELF bytes");

  const unsigned char elf_class = reader.get_class();

  // Detect legacy (unmerged) format: abi_version 0x21 = merged, anything else = per-page.
  constexpr ELFIO::Elf_Word kAbiVersionMerged = 0x21;
  aiebu::compress_stats stats;
  stats.has_unmerged_sections = (reader.get_abi_version() != kAbiVersionMerged);

  // Compress eligible sections into a name-keyed map.
  // Also detect .dump* sections in the same pass — no extra ELF parse.
  std::map<std::string, std::vector<std::uint8_t>> compressed;

  for (const auto& sec_ptr : reader.sections) {
    ELFIO::section* sec = sec_ptr.get();
    const std::string& name = sec->get_name();
    if (starts_with(name, kDump))
      stats.has_dump_section = true;
    if (!is_compressible_section(name))
      continue;

    if (sec->get_type() == ELFIO::SHT_NOBITS)
      throw std::runtime_error(name + " is SHT_NOBITS and has no file data");

    const char* data = sec->get_data();
    const std::size_t size = sec->get_size();
    if (data == nullptr || size == 0)
      throw std::runtime_error(name + " is empty");

    compressed.emplace(name,
        wrap_elf_compressed(elf_class, ELFCOMPRESS_ZSTD_VAL, size,
                            sec->get_addr_align(),
                            compress_buffer_zstd(
                                reinterpret_cast<const std::uint8_t*>(data),
                                size, level)));
  }

  if (compressed.empty())
    return {elf_bytes, stats};  // nothing to compress — return unchanged with stats

  // Build output ELF with a fresh writer so offsets are computed from scratch.
  ELFIO::elfio writer;
  writer.create(elf_class, reader.get_encoding());
  writer.set_os_abi(reader.get_os_abi());
  writer.set_abi_version(reader.get_abi_version());
  writer.set_type(reader.get_type());
  writer.set_machine(reader.get_machine());
  writer.set_flags(reader.get_flags());
  writer.set_entry(reader.get_entry());

  // index_map: source section index → destination section index.
  // Null section (0) is always 0→0; .shstrtab is auto-created by writer at 1.
  std::map<ELFIO::Elf_Half, ELFIO::Elf_Half> index_map;
  index_map[0] = 0;

  struct PendingSection { ELFIO::section* src; ELFIO::section* dst; };
  std::vector<PendingSection> pending;

  // NOLINT(modernize-loop-convert): index i needed for index_map and null-section skip
  for (ELFIO::Elf_Half i = 0; i < reader.sections.size(); ++i) { // NOLINT(modernize-loop-convert)
    ELFIO::section* src = reader.sections[i];
    const std::string& name = src->get_name();

    if (i == 0)
      continue;  // null section already seeded
    if (name == ".shstrtab") {
      index_map[i] = 1;  // auto-created by writer
      continue;
    }

    ELFIO::section* dst = writer.sections.add(name);
    index_map[i] = dst->get_index();

    dst->set_type(src->get_type());
    dst->set_flags(src->get_flags());
    dst->set_addr_align(src->get_addr_align());
    dst->set_entry_size(src->get_entry_size());
    dst->set_address(src->get_address());

    // Verbatim copy of sh_link and sh_info; remap_section_references will
    // overwrite the fields that hold section indices with remapped values.
    dst->set_link(src->get_link());
    dst->set_info(src->get_info());

    if (src->get_type() == ELFIO::SHT_NOBITS) {
      dst->set_size(src->get_size());
    }

    const auto it = compressed.find(name);
    if (it != compressed.end()) {
      dst->set_flags(compressed_section_flags(src->get_flags()));
      dst->set_address(0);
      dst->set_addr_align(1);
      dst->append_data(reinterpret_cast<const char*>(it->second.data()),
                       static_cast<ELFIO::Elf_Word>(it->second.size()));
    } else if (src->get_type() != ELFIO::SHT_NOBITS) {
      const char* d = src->get_data();
      const std::size_t sz = src->get_size();
      if (d != nullptr && sz > 0)
        dst->append_data(d, static_cast<ELFIO::Elf_Word>(sz));
    }

    pending.push_back({src, dst});
  }

  for (const auto& p : pending)
    remap_section_references(p.src, p.dst, index_map);

  recreate_segments(writer);

  std::ostringstream oss;
  if (!writer.save(oss))
    throw std::runtime_error("ZstdElfCompressor: failed to save output ELF");

  std::string bytes = oss.str();
  std::vector<char> out{std::make_move_iterator(bytes.begin()),
                        std::make_move_iterator(bytes.end())};
  return {std::move(out), stats};
}

// ---------------------------------------------------------------------------
// zstd decompression — pre-allocates exact output using ch_size from Elf_Chdr.
// Avoids the over-allocation and resize that ZSTD_compressBound requires on
// the compression path.
// ---------------------------------------------------------------------------

std::vector<char> decompress_buffer_zstd(
    const char* data, std::size_t compressed_size, std::uint64_t uncompressed_size)
{
  std::vector<char> output(uncompressed_size);  // exact allocation — no resize needed
  const std::size_t result = ZSTD_decompress(
      output.data(), output.size(), data, compressed_size);
  if (ZSTD_isError(result))
    throw std::runtime_error(
        std::string("zstd decompress failed: ") + ZSTD_getErrorName(result));
  if (result != uncompressed_size)
    throw std::runtime_error("zstd decompress: output size mismatch");
  return output;
}

// ---------------------------------------------------------------------------
// Restore ELF section flags after decompression.
// Compression cleared SHF_ALLOC and SHF_EXECINSTR and set SHF_COMPRESSED.
// Decompression reverses this: clears SHF_COMPRESSED and restores the original
// flags from the section name (the only source of truth left in the ELF).
// ---------------------------------------------------------------------------

ELFIO::Elf_Xword decompressed_section_flags(
    ELFIO::Elf_Xword compressed_flags, std::string_view name)
{
  ELFIO::Elf_Xword flags = compressed_flags;
  flags &= ~ELFIO::SHF_COMPRESSED;
  flags |=  ELFIO::SHF_ALLOC;
  if (starts_with(name, kCtrltext))
    flags |= ELFIO::SHF_EXECINSTR;
  return flags;
}

// ---------------------------------------------------------------------------
// Core ELF decompression pipeline — rebuild approach.
// ---------------------------------------------------------------------------

std::vector<char> decompress_elf_impl(const std::vector<char>& elf_bytes)
{
  boost::interprocess::ibufferstream is(elf_bytes.data(), elf_bytes.size());
  ELFIO::elfio reader;
  if (!reader.load(is))
    throw std::runtime_error("AutoElfDecompressor: failed to parse ELF");

  const unsigned char elf_class = reader.get_class();

  // Early exit if nothing is compressed.
  // Callers should pre-check with is_elf_compressed() to avoid the ELFIO parse
  // cost above.  This is a safety net for direct callers.
  bool has_compressed = false;
  for (const auto& sec_ptr : reader.sections) {
    if (sec_ptr->get_flags() & ELFIO::SHF_COMPRESSED) {
      has_compressed = true;
      break;
    }
  }
  if (!has_compressed)
    return {elf_bytes.begin(), elf_bytes.end()};

  // Build output ELF with a fresh writer so offsets are computed from scratch.
  ELFIO::elfio writer;
  writer.create(elf_class, reader.get_encoding());
  writer.set_os_abi(reader.get_os_abi());
  writer.set_abi_version(reader.get_abi_version());
  writer.set_type(reader.get_type());
  writer.set_machine(reader.get_machine());
  writer.set_flags(reader.get_flags());
  writer.set_entry(reader.get_entry());

  std::map<ELFIO::Elf_Half, ELFIO::Elf_Half> index_map;
  index_map[0] = 0;

  struct PendingSection { ELFIO::section* src; ELFIO::section* dst; };
  std::vector<PendingSection> pending;

  for (ELFIO::Elf_Half i = 0; i < reader.sections.size(); ++i) { // NOLINT(modernize-loop-convert)
    ELFIO::section* src = reader.sections[i];
    const std::string& name = src->get_name();

    if (i == 0)
      continue;  // null section already seeded
    if (name == ".shstrtab") {
      index_map[i] = 1;
      continue;
    }

    ELFIO::section* dst = writer.sections.add(name);
    index_map[i] = dst->get_index();

    dst->set_type(src->get_type());
    dst->set_entry_size(src->get_entry_size());

    // Verbatim copy of sh_link and sh_info; remap_section_references will
    // overwrite the fields that hold section indices with remapped values.
    dst->set_link(src->get_link());
    dst->set_info(src->get_info());

    const ELFIO::Elf_Xword flags = src->get_flags();

    if (flags & ELFIO::SHF_COMPRESSED) {
      // Parse Elf_Chdr to get ch_type, uncompressed size, original alignment.
      const char* raw      = src->get_data();
      const std::size_t sz = src->get_size();

      ELFIO::Elf_Word  ch_type      = 0;
      std::uint64_t    ch_size      = 0;
      ELFIO::Elf_Xword ch_addralign = 0;
      std::size_t      chdr_sz      = 0;

      if (elf_class == ELFIO::ELFCLASS64) {
        if (sz < sizeof(ELFIO::Elf64_Chdr))
          throw std::runtime_error(name + ": too small for Elf64_Chdr");
        ELFIO::Elf64_Chdr hdr{};
        std::memcpy(&hdr, raw, sizeof(hdr));
        ch_type      = hdr.ch_type;
        ch_size      = hdr.ch_size;
        ch_addralign = hdr.ch_addralign;
        chdr_sz      = sizeof(hdr);
      } else {
        if (sz < sizeof(ELFIO::Elf32_Chdr))
          throw std::runtime_error(name + ": too small for Elf32_Chdr");
        ELFIO::Elf32_Chdr hdr{};
        std::memcpy(&hdr, raw, sizeof(hdr));
        ch_type      = hdr.ch_type;
        ch_size      = hdr.ch_size;
        ch_addralign = hdr.ch_addralign;
        chdr_sz      = sizeof(hdr);
      }

      if (ch_type != ELFCOMPRESS_ZSTD_VAL)
        throw std::runtime_error(
            name + ": unsupported ch_type " + std::to_string(ch_type)
            + " (only ELFCOMPRESS_ZSTD=2 is supported)");

      // Decompress — ch_size is exact; pre-allocated, no resize needed.
      auto uncompressed = decompress_buffer_zstd(
          raw + chdr_sz, sz - chdr_sz, ch_size);

      dst->set_flags(decompressed_section_flags(flags, name));
      dst->set_addr_align(ch_addralign);  // restored from Chdr
      dst->set_address(src->get_address());
      dst->append_data(uncompressed.data(), static_cast<ELFIO::Elf_Word>(uncompressed.size()));
    } else {
      dst->set_flags(flags);
      dst->set_addr_align(src->get_addr_align());
      dst->set_address(src->get_address());

      if (src->get_type() == ELFIO::SHT_NOBITS) {
        dst->set_size(src->get_size());
      } else {
        const char* d = src->get_data();
        const std::size_t sz = src->get_size();
        if (d != nullptr && sz > 0)
          dst->append_data(d, static_cast<ELFIO::Elf_Word>(sz));
      }
    }

    pending.push_back({src, dst});
  }

  for (const auto& p : pending)
    remap_section_references(p.src, p.dst, index_map);

  recreate_segments(writer);

  std::ostringstream oss;
  if (!writer.save(oss))
    throw std::runtime_error("AutoElfDecompressor: failed to save output ELF");

  std::string bytes = oss.str();
  return {std::make_move_iterator(bytes.begin()),
          std::make_move_iterator(bytes.end())};
}

} // namespace

namespace aiebu {

// ---------------------------------------------------------------------------
// NullElfCompressor
// ---------------------------------------------------------------------------

std::vector<char> NullElfCompressor::compress(std::vector<char> elf_bytes) const
{
  return elf_bytes;
}

// ---------------------------------------------------------------------------
// ZstdElfCompressor
// ---------------------------------------------------------------------------

ZstdElfCompressor::ZstdElfCompressor(int level)
  : m_level(level)
{
}

// Returns a mutable reference to the function-local static compress_stats.
// Using a function-local static avoids a non-const global variable.
static compress_stats& mutable_last_compress_stats()
{
  static compress_stats s;
  return s;
}

std::vector<char> ZstdElfCompressor::compress(std::vector<char> elf_bytes) const
{
  auto [result, stats] = compress_elf_zstd(elf_bytes, m_level);
  mutable_last_compress_stats() = stats;
  return result;
}

const compress_stats& get_last_compress_stats()
{
  return mutable_last_compress_stats();
}

// ---------------------------------------------------------------------------
// ZlibElfCompressor
// ---------------------------------------------------------------------------

std::vector<char> ZlibElfCompressor::compress(std::vector<char> /*elf_bytes*/) const
{
  throw std::invalid_argument("compress=zlib is not yet supported");
}

// ---------------------------------------------------------------------------
// make_elf_compressor
// ---------------------------------------------------------------------------

std::unique_ptr<ElfCompressor> make_elf_compressor(const std::vector<std::string>& flags)
{
  constexpr std::string_view bare_flag = "compress";
  constexpr std::string_view prefix    = "compress=";
  constexpr std::string_view zstd_key  = "zstd";
  constexpr std::string_view zstd_level_sep = "zstd:";

  const std::string* found = nullptr;
  for (const auto& flag : flags) {
    const bool is_bare = (flag == bare_flag);
    const bool is_keyed = starts_with(std::string_view(flag), prefix);
    if (is_bare || is_keyed) {
      if (found) {
        throw std::invalid_argument(
            "multiple compress flags specified: '" + *found + "' and '" + flag + "'");
      }
      found = &flag;
    }
  }

  if (!found)
    return std::make_unique<NullElfCompressor>();

  // Bare "compress" flag — default to zstd at default level
  if (*found == bare_flag)
    return std::make_unique<ZstdElfCompressor>(ZSTD_CLEVEL_DEFAULT);

  const std::string_view algo(found->data() + prefix.size(),
                               found->size()  - prefix.size());

  // compress=none
  if (algo == "none")
    return std::make_unique<NullElfCompressor>();

  // compress=zlib (placeholder)
  if (algo == "zlib")
    return std::make_unique<ZlibElfCompressor>();

  // compress=zstd  or  compress=zstd:<level>
  if (algo == zstd_key || starts_with(algo, zstd_level_sep)) {
    int level = ZSTD_CLEVEL_DEFAULT;

    if (starts_with(algo, zstd_level_sep)) {
      const std::string level_str(algo.data() + zstd_level_sep.size(),
                                   algo.size()  - zstd_level_sep.size());
      if (level_str.empty()) {
        throw std::invalid_argument(
            "compress=zstd: requires a level value, e.g. compress=zstd:3");
      }

      std::size_t pos = 0;
      try {
        level = std::stoi(level_str, &pos);
      } catch (const std::exception&) {
        throw std::invalid_argument(
            "compress=zstd: invalid level '" + level_str + "' — must be an integer");
      }
      if (pos != level_str.size()) {
        throw std::invalid_argument(
            "compress=zstd: invalid level '" + level_str + "' — must be an integer");
      }

      const int min_level = ZSTD_minCLevel();
      const int max_level = ZSTD_maxCLevel();
      if (level < min_level || level > max_level) {
        throw std::invalid_argument(
            "compress=zstd: level " + std::to_string(level)
            + " out of range [" + std::to_string(min_level)
            + ", " + std::to_string(max_level) + "]");
      }
    }

    return std::make_unique<ZstdElfCompressor>(level);
  }

  throw std::invalid_argument(
      "unknown compression algorithm in flag '" + *found + "'");
}

// ---------------------------------------------------------------------------
// AutoElfDecompressor
// ---------------------------------------------------------------------------

std::vector<char> AutoElfDecompressor::decompress(const std::vector<char>& elf_bytes) const
{
  return decompress_elf_impl(elf_bytes);
}

// ---------------------------------------------------------------------------
// make_elf_decompressor
// ---------------------------------------------------------------------------

std::unique_ptr<ElfDecompressor> make_elf_decompressor()
{
  return std::make_unique<AutoElfDecompressor>();
}

// ---------------------------------------------------------------------------
// Per-section decompression — parse Chdr and decompress into caller buffer.
// Extracted from AutoElfDecompressor::decompress() Chdr parsing (lines above)
// into reusable functions for deferred per-section decompression in XRT.
// ---------------------------------------------------------------------------

// Internal helper: parse the Elf_Chdr at the start of a compressed section.
// Returns true on success, populating ch_type, ch_size, ch_addralign, chdr_sz.
// Returns false if section_data is too small for the Chdr.
namespace {

struct chdr_info {
  ELFIO::Elf_Word  ch_type      = 0;
  std::uint64_t    ch_size      = 0; // uncompressed size
  ELFIO::Elf_Xword ch_addralign = 0;
  std::size_t      chdr_sz      = 0; // sizeof(Elf32_Chdr) or sizeof(Elf64_Chdr)
};

bool parse_chdr(const char* data, std::size_t size,
                unsigned char elf_class, chdr_info& out)
{
  if (elf_class == ELFIO::ELFCLASS64) {
    if (size < sizeof(ELFIO::Elf64_Chdr))
      return false;
    ELFIO::Elf64_Chdr hdr{};
    std::memcpy(&hdr, data, sizeof(hdr));
    out.ch_type      = hdr.ch_type;
    out.ch_size      = hdr.ch_size;
    out.ch_addralign = hdr.ch_addralign;
    out.chdr_sz      = sizeof(hdr);
    return true;
  }

  if (elf_class == ELFIO::ELFCLASS32) {
    if (size < sizeof(ELFIO::Elf32_Chdr))
      return false;
    ELFIO::Elf32_Chdr hdr{};
    std::memcpy(&hdr, data, sizeof(hdr));
    out.ch_type      = hdr.ch_type;
    out.ch_size      = hdr.ch_size;
    out.ch_addralign = hdr.ch_addralign;
    out.chdr_sz      = sizeof(hdr);
    return true;
  }

  return false;
}

} // anonymous namespace

std::size_t get_uncompressed_section_size_impl(
    const char* section_data, std::size_t section_size, unsigned char elf_class)
{
  chdr_info info{};
  if (!parse_chdr(section_data, section_size, elf_class, info))
    return 0;
  return static_cast<std::size_t>(info.ch_size);
}

std::size_t decompress_section_into_impl(
    const char* section_data, std::size_t section_size,
    void* dest, std::size_t dest_size, unsigned char elf_class)
{
  chdr_info info{};
  if (!parse_chdr(section_data, section_size, elf_class, info))
    throw std::runtime_error(
        "decompress_section_into: section too small for Elf_Chdr");

  if (info.ch_type != ELFCOMPRESS_ZSTD_VAL) {
    throw std::runtime_error(
        "decompress_section_into: unsupported ch_type "
        + std::to_string(info.ch_type)
        + " (only ELFCOMPRESS_ZSTD=2 is supported)");
  }

  const auto uncompressed_size = static_cast<std::size_t>(info.ch_size);
  if (dest_size < uncompressed_size) {
    throw std::runtime_error(
        "decompress_section_into: dest_size ("
        + std::to_string(dest_size) + ") < uncompressed size ("
        + std::to_string(uncompressed_size) + ")");
  }

  const char* compressed_data = section_data + info.chdr_sz;
  const std::size_t compressed_size = section_size - info.chdr_sz;

  const std::size_t result = ZSTD_decompress(
      dest, dest_size, compressed_data, compressed_size);
  if (ZSTD_isError(result))
    throw std::runtime_error(
        std::string("decompress_section_into: zstd failed: ")
        + ZSTD_getErrorName(result));
  if (result != uncompressed_size)
    throw std::runtime_error(
        "decompress_section_into: output size mismatch");

  return uncompressed_size;
}

// ---------------------------------------------------------------------------
// is_elf_compressed_impl — raw section-header scan, no ELFIO, no heap alloc.
// Checks sh_flags for SHF_COMPRESSED without parsing the full ELF structure.
// ---------------------------------------------------------------------------

bool is_elf_compressed_impl(const char* data, std::size_t size)
{
  constexpr std::size_t kEIIdent      = 16;
  constexpr unsigned char kElfMag0    = 0x7f;
  constexpr unsigned char kElfClass32 = 1;
  constexpr unsigned char kElfClass64 = 2;
  constexpr uint32_t kShfCompressed   = 0x800U;

  if (size < kEIIdent)
    return false;
  if (static_cast<unsigned char>(data[0]) != kElfMag0 ||
      data[1] != 'E' || data[2] != 'L' || data[3] != 'F')
    return false;

  // Unaligned little-endian reads via boost::endian — explicit endianness contract,
  // compiles to a single load instruction on x86_64.
  // reinterpret_cast: boost::endian requires unsigned char const*, data is const char*.
  auto rd16 = [&](std::size_t off) -> uint16_t {
    if (off + 2 > size) return 0;
    return boost::endian::load_little_u16(reinterpret_cast<const unsigned char*>(data + off));
  };
  auto rd32 = [&](std::size_t off) -> uint32_t {
    if (off + 4 > size) return 0;
    return boost::endian::load_little_u32(reinterpret_cast<const unsigned char*>(data + off));
  };
  auto rd64 = [&](std::size_t off) -> uint64_t {
    constexpr std::size_t kU64Size = 8;
    if (off + kU64Size > size) return 0;
    return boost::endian::load_little_u64(reinterpret_cast<const unsigned char*>(data + off));
  };

  const auto ei_class = static_cast<unsigned char>(data[4]);

  if (ei_class == kElfClass32) {
    // Elf32_Ehdr: e_shoff@32(4), e_shentsize@46(2), e_shnum@48(2)
    constexpr std::size_t kEhdr32ShOff     = 32;
    constexpr std::size_t kEhdr32ShEntSize = 46;
    constexpr std::size_t kEhdr32ShNum     = 48;
    // Elf32_Shdr: sh_flags field offset within entry
    constexpr std::size_t kShdr32FlagsOff  = 8;
    constexpr uint16_t    kShdr32MinSize   = 12;
    const uint32_t shoff     = rd32(kEhdr32ShOff);
    const uint16_t shentsize = rd16(kEhdr32ShEntSize);
    const uint16_t shnum     = rd16(kEhdr32ShNum);
    if (shoff == 0 || shentsize < kShdr32MinSize) return false;
    for (uint16_t i = 0; i < shnum; ++i) {
      if (rd32(shoff + std::size_t(i) * shentsize + kShdr32FlagsOff) & kShfCompressed)
        return true;
    }
  } else if (ei_class == kElfClass64) {
    // Elf64_Ehdr: e_shoff@40(8), e_shentsize@58(2), e_shnum@60(2)
    constexpr std::size_t kEhdr64ShOff     = 40;
    constexpr std::size_t kEhdr64ShEntSize = 58;
    constexpr std::size_t kEhdr64ShNum     = 60;
    // Elf64_Shdr: sh_flags field offset within entry
    constexpr std::size_t kShdr64FlagsOff  = 8;
    constexpr uint16_t    kShdr64MinSize   = 16;
    const uint64_t shoff     = rd64(kEhdr64ShOff);
    const uint16_t shentsize = rd16(kEhdr64ShEntSize);
    const uint16_t shnum     = rd16(kEhdr64ShNum);
    if (shoff == 0 || shentsize < kShdr64MinSize) return false;
    for (uint16_t i = 0; i < shnum; ++i) {
      if (rd64(std::size_t(shoff) + std::size_t(i) * shentsize + kShdr64FlagsOff) & kShfCompressed)
        return true;
    }
  }

  return false;
}

} // namespace aiebu
