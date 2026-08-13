// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
//
// Standalone memory-consumption and timing report for ELF decompression.
//
// Takes a pre-compressed ELF file as input so the process starts clean —
// no assembly or any other large allocation precedes the RSS snapshot.
//
// Memory baseline model mirrors the XRT runtime code path:
//   XRT loads the compressed ELF (from xclbin) into memory, then calls
//   get_section_uncompressed_size() + copy_section_uncompressed_data() to decompress each section
//   directly into the destination BO buffer — one section at a time.
//   It never constructs a full decompressed ELF as an intermediate buffer.
//
//   read_file() here simulates the xclbin load XRT has already performed.
//   The RSS snapshot is taken BEFORE read_file() so the reported peak-RSS
//   delta covers the full working set:
//     compressed input bytes + ELFIO parse structures +
//     one section output buffer at a time.
//
// Exercises the per-section APIs from aiebu_decompress.h:
//   is_elf_compressed()      — verify the input is actually compressed
//   get_section_uncompressed_size()  — per-section uncompressed size via Elf_Chdr
//   copy_section_uncompressed_data()      — per-section decompression into caller buffer
//
// Usage:
//   decompress_memory_report <compressed.elf>
//
// The test is not a pass/fail correctness test — it always exits 0 on a
// valid compressed ELF.  It is intended to be run manually or as part of
// a benchmark/profiling suite, and its output is human-readable prose.

#include "aiebu/aiebu_decompress.h"
#include "aiebu/aiebu_error.h"
#include "common/metrics.h"

#include <elfio/elfio.hpp>

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static std::vector<char> read_file(const std::string& path)
{
  std::ifstream f(path, std::ios::binary);
  if (!f)
    throw std::runtime_error("cannot open: " + path);
  return {std::istreambuf_iterator<char>(f), {}};
}

template <typename Fn>
static double time_ms(Fn&& fn)
{
  const auto t0 = std::chrono::steady_clock::now();
  std::forward<Fn>(fn)();
  const auto t1 = std::chrono::steady_clock::now();
  return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

// ---------------------------------------------------------------------------
// Report
// ---------------------------------------------------------------------------

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <compressed.elf>\n";
    return 1;
  }

  const std::string input_file = argv[1];

  try {
    std::cout << std::fixed << std::setprecision(2);

    // -----------------------------------------------------------------------
    // Snapshot RSS before read_file() so the reported delta covers the full
    // XRT working set: compressed input bytes (simulates xclbin load) +
    // ELFIO parse structures + per-section output buffers.
    // -----------------------------------------------------------------------
    const auto rss_before = aiebu::get_process_metrics();

    const auto elf_bytes = read_file(input_file);

    if (!aiebu::is_elf_compressed(elf_bytes)) {
      std::cerr << "error: ELF has no SHF_COMPRESSED sections — "
                   "run with a compressed ELF (e.g. assembled with compress=zstd)\n"
                << "  input: " << input_file << " (" << elf_bytes.size() << " B)\n";
      return 1;
    }

    std::cout << "decompress_memory_report: " << input_file << "\n"
              << "  input size: " << elf_bytes.size() << " B\n\n";

    const auto wall_before = std::chrono::steady_clock::now();

    ELFIO::elfio elf;
    if (!elf.load(input_file))
      throw std::runtime_error("ELFIO failed to load: " + input_file);

    std::size_t total_stored  = 0;
    std::size_t total_uncomp  = 0;
    std::size_t section_count = 0;

    for (const auto& sec : elf.sections) {
      if (!(sec->get_flags() & ELFIO::SHF_COMPRESSED))
        continue;

      const std::size_t stored_sz = sec->get_size(); // sizeof(Chdr) + compressed payload
      const std::size_t data_sz   = aiebu::get_section_uncompressed_size(sec.get(), elf);

      std::vector<char> buf(data_sz);
      const double sec_ms = time_ms([&]{
        static_cast<void>(aiebu::copy_section_uncompressed_data(sec.get(), elf, buf.data(), buf.size()));
      });

      total_stored  += stored_sz;
      total_uncomp  += data_sz;
      ++section_count;

      const double ratio = (data_sz > 0)
          ? (100.0 * static_cast<double>(stored_sz) / static_cast<double>(data_sz))
          : 0.0;

      std::cout << "  [" << sec->get_name() << "]\n"
                << "    stored (Chdr+payload): " << stored_sz << " B\n"
                << "    uncompressed         : " << data_sz   << " B\n"
                << "    ratio                : " << ratio     << " %\n"
                << "    decompression time   : " << sec_ms    << " ms\n\n";
    }

    const auto wall_after = std::chrono::steady_clock::now();
    const auto rss_after  = aiebu::get_process_metrics();

    const double total_ms =
        std::chrono::duration<double, std::milli>(wall_after - wall_before).count();
    const std::uint64_t peak_kb =
        (rss_after.m_peak_kb > rss_before.m_peak_kb)
        ? (rss_after.m_peak_kb - rss_before.m_peak_kb) : 0;
    const double total_ratio = (total_uncomp > 0)
        ? (100.0 * static_cast<double>(total_stored) / static_cast<double>(total_uncomp))
        : 0.0;

    if (section_count == 0) {
      std::cerr << "warning: no SHF_COMPRESSED sections found by ELFIO "
                   "(is_elf_compressed() returned true — ELF may be malformed)\n";
    }

    std::cout << "  [summary]\n"
              << "    sections decompressed    : " << section_count << "\n"
              << "    total stored (Chdr+payld): " << total_stored  << " B\n"
              << "    total uncompressed       : " << total_uncomp  << " B\n"
              << "    overall ratio            : " << total_ratio   << " %\n"
              << "    total time (load+decomp) : " << total_ms      << " ms\n"
              << "    peak RSS delta           : " << peak_kb       << " KB\n"
              << "    (covers: compressed input + ELFIO + per-section output bufs)\n";

    return 0;
  }
  catch (const aiebu::error& ex) {
    std::cerr << "aiebu::error: " << ex.what() << " (code=" << ex.get_code() << ")\n";
    return 1;
  }
  catch (const std::exception& ex) {
    std::cerr << "error: " << ex.what() << "\n";
    return 1;
  }
}
