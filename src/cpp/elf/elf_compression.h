// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_ELF_ELF_COMPRESSION_H_
#define AIEBU_ELF_ELF_COMPRESSION_H_

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace aiebu {

// Abstract base: takes raw ELF bytes, returns (possibly compressed) ELF bytes.
// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class ElfCompressor {
public:
  virtual ~ElfCompressor() = default;
  virtual std::vector<char> compress(std::vector<char> elf_bytes) const = 0;
};

// No-op compressor — returns bytes unchanged.
class NullElfCompressor : public ElfCompressor {
public:
  std::vector<char> compress(std::vector<char> elf_bytes) const override;
};

// Compresses .ctrltext* / .ctrldata* sections using zstd SHF_COMPRESSED.
// level: zstd compression level (1–22, or negative for fast modes).
//        Defaults to ZSTD_CLEVEL_DEFAULT (3) when constructed via make_elf_compressor.
class ZstdElfCompressor : public ElfCompressor {
public:
  explicit ZstdElfCompressor(int level);
  std::vector<char> compress(std::vector<char> elf_bytes) const override;
private:
  int m_level;
};

// Placeholder — throws std::invalid_argument (zlib not yet supported).
class ZlibElfCompressor : public ElfCompressor {
public:
  std::vector<char> compress(std::vector<char> elf_bytes) const override;
};

// Parse a flags vector for "compress=<algo>" and return the matching compressor.
// Returns NullElfCompressor if no compress= flag is present.
// Throws std::invalid_argument for unknown algorithm values.
std::unique_ptr<ElfCompressor> make_elf_compressor(
    const std::vector<std::string>& flags);

// ---------------------------------------------------------------------------
// ElfDecompressor hierarchy — separate from ElfCompressor by design.
//
// Compression is caller-directed (algorithm chosen via flags).
// Decompression is data-directed (algorithm read from ch_type in Elf_Chdr).
// These different contracts justify separate class hierarchies.
// ---------------------------------------------------------------------------

// Abstract base: takes (possibly compressed) ELF bytes, returns decompressed ELF bytes.
// Takes const& because the decompressor always builds a new output buffer — it never
// reuses or moves from the input.  The input only needs to stay alive during the call.
// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class ElfDecompressor {
public:
  virtual ~ElfDecompressor() = default;
  virtual std::vector<char> decompress(const std::vector<char>& elf_bytes) const = 0;
};

// Decompresses any SHF_COMPRESSED sections found in the ELF.
// Algorithm is auto-detected per section from ch_type in Elf_Chdr:
//   ch_type == 2 (ELFCOMPRESS_ZSTD) — decompressed with zstd.
// Sections without SHF_COMPRESSED are copied unchanged.
// If no SHF_COMPRESSED sections are found, returns a copy of the input.
// Throws std::runtime_error for corrupt data or unsupported ch_type.
class AutoElfDecompressor : public ElfDecompressor {
public:
  std::vector<char> decompress(const std::vector<char>& elf_bytes) const override;
};

// Returns an AutoElfDecompressor. No flags needed — the algorithm is in the ELF.
std::unique_ptr<ElfDecompressor> make_elf_decompressor();

// Raw section-header scan — no ELFIO, no heap allocation.
// Returns true if any section in the ELF has SHF_COMPRESSED set.
// Used by aiebu::is_elf_compressed() to short-circuit the full ELFIO
// parse for uncompressed ELFs on the XRT hot path.
bool is_elf_compressed_impl(const char* data, std::size_t size);

// ---------------------------------------------------------------------------
// Per-section decompression — deferred decompression support.
//
// These functions operate on a single SHF_COMPRESSED section's raw data
// (starting with an Elf_Chdr header).  They allow XRT to keep the ELF
// compressed inside ELFIO and decompress individual sections on demand,
// directly into a caller-provided buffer (e.g. a device BO).
// ---------------------------------------------------------------------------

// Read the uncompressed size (ch_size) from the Elf_Chdr header at the start
// of an SHF_COMPRESSED section.
// Returns 0 if the section data is too small to contain a valid Chdr.
// elf_class: ELFCLASS32 (1) or ELFCLASS64 (2).
std::size_t get_uncompressed_section_size_impl(
    const char* section_data, std::size_t section_size, unsigned char elf_class);

// Decompress a single SHF_COMPRESSED section directly into a caller-provided
// buffer.  section_data must start with an Elf_Chdr header.
// Writes exactly ch_size bytes to dest.  dest_size must be >= ch_size.
// Returns the number of bytes written (== ch_size on success).
// Throws std::runtime_error on corrupt data, unsupported ch_type, or
// dest_size too small.
std::size_t decompress_section_into_impl(
    const char* section_data, std::size_t section_size,
    void* dest, std::size_t dest_size, unsigned char elf_class);

// ---------------------------------------------------------------------------
// Per-call compression flags — populated by ZstdElfCompressor::compress().
// Used by callers to emit warnings about conditions that reduce compression
// effectiveness.  No timing fields — profiling is done externally.
// ---------------------------------------------------------------------------

struct compress_stats {
  bool has_dump_section      = false; // true if ELF contained .dump* sections (not compressed)
  bool has_unmerged_sections = false; // true if ELF uses legacy per-page .ctrldata* sections
};

// Returns the flags from the most recent ZstdElfCompressor::compress() call.
const compress_stats& get_last_compress_stats();

} // namespace aiebu

#endif // AIEBU_ELF_ELF_COMPRESSION_H_
