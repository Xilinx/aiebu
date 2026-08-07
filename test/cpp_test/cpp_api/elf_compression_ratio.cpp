// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
//
// Tool: ELF compression size/ratio reporter.
//
// Each --mode performs exactly one timed operation.  Run modes as separate
// processes so each starts without cache warming from a previous run.
//
// Usage:
//   elf_compression_ratio <directory> --arch <aie4|aie2ps> --mode <mode>
//                         [--algorithm zstd] [--level <N>] [-f <flag>]
//
// Modes:
//   plain      assemble without compression — reports ELF size + assembly time
//   compress   assemble with compression    — reports sizes, ratio + assembly time
//              (plain assembly is done untimed to obtain the uncompressed size)
//   decompress time decompress_elf() only   — reports sizes + decompression time
//              (assemble+compress done untimed to produce the compressed input)
//
// Examples:
//   elf_compression_ratio <dir> --arch aie4 --mode plain
//   elf_compression_ratio <dir> --arch aie4 --mode compress --level 9
//   elf_compression_ratio <dir> --arch aie4 --mode decompress -f disabledump

#include "aiebu/aiebu_assembler.h"
#include "aiebu/aiebu_error.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// ---------------------------------------------------------------------------
// Timing helper
// ---------------------------------------------------------------------------

template <typename Fn>
static double time_ms(Fn&& fn)
{
  const auto t0 = std::chrono::steady_clock::now();
  fn();
  const auto t1 = std::chrono::steady_clock::now();
  return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

// ---------------------------------------------------------------------------
// File helper
// ---------------------------------------------------------------------------

static std::vector<char> read_file(const std::string& path)
{
  std::ifstream f(path, std::ios::binary);
  if (!f)
    throw std::runtime_error("cannot open: " + path);
  return std::vector<char>(std::istreambuf_iterator<char>(f), {});
}

// ---------------------------------------------------------------------------
// Assembly helper — auto-discovers all artifacts in the directory.
// Files are registered under both "rel/path" and "./rel/path" so that
// config.json references work regardless of whether they use a leading "./".
// ---------------------------------------------------------------------------

static std::vector<char> assemble(const std::string& dir,
                                  aiebu::aiebu_assembler::buffer_type type,
                                  const std::vector<std::string>& flags)
{
  auto config_json = read_file(dir + "/config.json");

  aiebu::file_artifact art;
  const fs::path base(dir);

  for (const auto& entry : fs::recursive_directory_iterator(base)) {
    if (!entry.is_regular_file())
      continue;
    if (entry.path().filename() == "config.json")
      continue;

    const std::string vname =
        entry.path().lexically_relative(base).generic_string();
    auto buf = read_file(entry.path().string());
    art.add_vfile(vname, buf);
    art.add_vfile("./" + vname, buf);
  }

  aiebu::aiebu_assembler as(type, config_json, art, flags);
  return as.get_elf();
}

// ---------------------------------------------------------------------------
// Modes
// ---------------------------------------------------------------------------

static int mode_plain(const std::string& dir,
                      aiebu::aiebu_assembler::buffer_type type,
                      const std::vector<std::string>& extra_flags,
                      const std::string& arch)
{
  std::vector<char> elf;
  const double t_ms = time_ms([&]{ elf = assemble(dir, type, extra_flags); });

  std::cout << std::fixed << std::setprecision(2)
            << "[plain assembly]\n"
            << "  architecture  : " << arch        << "\n"
            << "  ELF size      : " << elf.size()  << " B\n"
            << "  assembly time : " << t_ms        << " ms\n";
  return 0;
}

static int mode_compress(const std::string& dir,
                         aiebu::aiebu_assembler::buffer_type type,
                         const std::vector<std::string>& extra_flags,
                         const std::string& arch,
                         const std::string& algorithm,
                         int level,
                         const std::string& compress_flag)
{
  // Plain assembly — untimed, for size comparison only.
  const auto plain = assemble(dir, type, extra_flags);

  std::vector<std::string> comp_flags = extra_flags;
  comp_flags.emplace_back(compress_flag);

  std::vector<char> compressed;
  const double t_ms = time_ms([&]{ compressed = assemble(dir, type, comp_flags); });

  const std::size_t plain_size = plain.size();
  const std::size_t comp_size  = compressed.size();
  const double ratio = (plain_size > 0)
      ? (100.0 * comp_size / plain_size)
      : 0.0;

  std::cout << std::fixed << std::setprecision(2)
            << "[compressed assembly]\n"
            << "  architecture  : " << arch      << "\n"
            << "  algorithm     : " << algorithm << " (level " << level << ")\n"
            << "  uncompressed  : " << plain_size << " B\n"
            << "  compressed    : " << comp_size  << " B\n"
            << "  ratio         : " << ratio      << " %"
               "  (compressed / uncompressed * 100)\n"
            << "  assembly time : " << t_ms       << " ms\n";
  return 0;
}

static int mode_decompress_file(const std::string& input_file,
                               const std::string& output_file)
{
  auto compressed = read_file(input_file);
  const std::size_t comp_size = compressed.size();

  if (!aiebu::aiebu_assembler::is_elf_compressed(compressed)) {
    std::cerr << "ELF is not compressed, nothing to do.\n"
              << "  input  : " << input_file << " (" << comp_size << " B)\n";
    // Still write the output so the user gets a usable file
    std::ofstream out(output_file, std::ios::binary);
    if (!out)
      throw std::runtime_error("cannot open output: " + output_file);
    out.write(compressed.data(), compressed.size());
    return 0;
  }

  auto decompressed = aiebu::aiebu_assembler::decompress_elf(std::move(compressed));

  std::ofstream out(output_file, std::ios::binary);
  if (!out)
    throw std::runtime_error("cannot open output: " + output_file);
  out.write(decompressed.data(), decompressed.size());

  std::cout << std::fixed << std::setprecision(2)
            << "[decompress-file]\n"
            << "  input           : " << input_file           << "\n"
            << "  output          : " << output_file          << "\n"
            << "  compressed size : " << comp_size             << " B\n"
            << "  decompressed    : " << decompressed.size()   << " B\n"
            << "  ratio           : "
            << (100.0 * comp_size / decompressed.size()) << " %\n";
  return 0;
}

static int mode_decompress(const std::string& dir,
                           aiebu::aiebu_assembler::buffer_type type,
                           const std::vector<std::string>& extra_flags,
                           const std::string& arch,
                           const std::string& algorithm,
                           int level,
                           const std::string& compress_flag)
{
  // Assemble + compress — untimed, produces the compressed input.
  std::vector<std::string> comp_flags = extra_flags;
  comp_flags.emplace_back(compress_flag);
  auto compressed = assemble(dir, type, comp_flags);

  const std::size_t comp_size = compressed.size();

  std::vector<char> decompressed;
  const double t_ms = time_ms([&]{
    decompressed = aiebu::aiebu_assembler::decompress_elf(std::move(compressed));
  });

  std::cout << std::fixed << std::setprecision(2)
            << "[decompression]\n"
            << "  architecture      : " << arch              << "\n"
            << "  algorithm         : " << algorithm << " (level " << level << ")\n"
            << "  compressed size   : " << comp_size          << " B\n"
            << "  decompressed size : " << decompressed.size() << " B\n"
            << "  decompression time: " << t_ms               << " ms\n";
  return 0;
}

static void print_usage(const char* prog)
{
  std::cerr
    << "Usage:\n"
    << "  Assembly modes (require --arch):\n"
    << "    " << prog
    << " <directory> --arch <aie4|aie2ps> --mode <plain|compress|decompress>"
       " [--algorithm zstd] [--level <N>] [-f <flag>]\n\n"
    << "  File mode (no --arch needed):\n"
    << "    " << prog
    << " --mode decompress-file --input <compressed.elf> --output <decompressed.elf>\n\n"
    << "  directory   directory containing config.json and artifact files\n"
    << "  --arch      target architecture: aie4 or aie2ps\n"
    << "  --mode      plain            : time plain assembly, report ELF size\n"
    << "              compress         : time compressed assembly, report sizes + ratio\n"
    << "              decompress       : time decompress_elf() only, report sizes\n"
    << "              decompress-file  : decompress an ELF file and write output\n"
    << "  --input     input ELF file (for decompress-file mode)\n"
    << "  --output    output ELF file (for decompress-file mode)\n"
    << "  --algorithm compression algorithm (default: zstd)\n"
    << "  --level     compression level (default: 3)\n"
    << "  -f <flag>   extra assembler flag (may be repeated), e.g. -f disabledump\n";
}

int main(int argc, char** argv)
{
  std::string dir;
  std::string arch;
  std::string mode;
  std::string algorithm = "zstd";
  std::string input_file;
  std::string output_file;
  int level = 3;
  std::vector<std::string> extra_flags;

  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "--arch" && i + 1 < argc)
      arch = argv[++i];
    else if (arg == "--mode" && i + 1 < argc)
      mode = argv[++i];
    else if (arg == "--algorithm" && i + 1 < argc)
      algorithm = argv[++i];
    else if (arg == "--level" && i + 1 < argc)
      level = std::stoi(argv[++i]);
    else if (arg == "--input" && i + 1 < argc)
      input_file = argv[++i];
    else if (arg == "--output" && i + 1 < argc)
      output_file = argv[++i];
    else if (arg == "-f" && i + 1 < argc)
      extra_flags.emplace_back(argv[++i]);
    else if (arg == "-h" || arg == "--help") {
      print_usage(argv[0]);
      return 0;
    }
    else if (dir.empty() && arg[0] != '-')
      dir = arg;
    else {
      std::cerr << "Unknown argument: " << arg << "\n";
      print_usage(argv[0]);
      return 1;
    }
  }

  if (mode.empty()) {
    print_usage(argv[0]);
    return 1;
  }

  // decompress-file mode: only needs --input and --output
  if (mode == "decompress-file") {
    if (input_file.empty() || output_file.empty()) {
      std::cerr << "Error: decompress-file mode requires --input and --output\n";
      print_usage(argv[0]);
      return 1;
    }
  }
  else {
    // Assembly modes need dir and arch
    if (dir.empty() || arch.empty()) {
      print_usage(argv[0]);
      return 1;
    }
    if (arch != "aie4" && arch != "aie2ps") {
      std::cerr << "Error: --arch must be aie4 or aie2ps\n";
      return 1;
    }
    if (mode != "plain" && mode != "compress" && mode != "decompress") {
      std::cerr << "Error: --mode must be plain, compress, decompress, or decompress-file\n";
      return 1;
    }
  }

  try {
    if (mode == "decompress-file")
      return mode_decompress_file(input_file, output_file);

    const auto type = (arch == "aie4")
        ? aiebu::aiebu_assembler::buffer_type::aie4_config
        : aiebu::aiebu_assembler::buffer_type::aie2ps_config;

    const std::string compress_flag =
        "compress=" + algorithm + ":" + std::to_string(level);

    if (mode == "plain")
      return mode_plain(dir, type, extra_flags, arch);
    if (mode == "compress")
      return mode_compress(dir, type, extra_flags, arch, algorithm, level, compress_flag);
    if (mode == "decompress")
      return mode_decompress(dir, type, extra_flags, arch, algorithm, level, compress_flag);
  }
  catch (const aiebu::error& ex) {
    std::cerr << "aiebu::error: " << ex.what()
              << " (code=" << ex.get_code() << ")\n";
    return 1;
  }
  catch (const std::exception& ex) {
    std::cerr << "error: " << ex.what() << "\n";
    return 1;
  }

  return 0;
}
