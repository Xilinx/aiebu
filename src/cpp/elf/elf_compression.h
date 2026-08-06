// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ELF_ELF_COMPRESSION_H_
#define _AIEBU_ELF_ELF_COMPRESSION_H_

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace aiebu {

// Abstract base: takes raw ELF bytes, returns (possibly compressed) ELF bytes.
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
// Pass-by-value enables callers to std::move large ELF buffers in — zero copy on input.
class ElfDecompressor {
public:
  virtual ~ElfDecompressor() = default;
  virtual std::vector<char> decompress(std::vector<char> elf_bytes) const = 0;
};

// Decompresses any SHF_COMPRESSED sections found in the ELF.
// Algorithm is auto-detected per section from ch_type in Elf_Chdr:
//   ch_type == 2 (ELFCOMPRESS_ZSTD) — decompressed with zstd.
// Sections without SHF_COMPRESSED are copied unchanged.
// If no SHF_COMPRESSED sections are found, returns the input buffer via move — no copy.
// Throws std::runtime_error for corrupt data or unsupported ch_type.
class AutoElfDecompressor : public ElfDecompressor {
public:
  std::vector<char> decompress(std::vector<char> elf_bytes) const override;
};

// Returns an AutoElfDecompressor. No flags needed — the algorithm is in the ELF.
std::unique_ptr<ElfDecompressor> make_elf_decompressor();

// Raw section-header scan — no ELFIO, no heap allocation.
// Returns true if any section in the ELF has SHF_COMPRESSED set.
// Used by aiebu_assembler::is_elf_compressed() to short-circuit the full ELFIO
// parse for uncompressed ELFs on the XRT hot path.
bool is_elf_compressed_impl(const char* data, std::size_t size);

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

#endif // _AIEBU_ELF_ELF_COMPRESSION_H_
