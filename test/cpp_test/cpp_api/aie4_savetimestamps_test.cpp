// SPDX-License-Identifier: MIT
// Copyright (C) 2026 Advanced Micro Devices, Inc.

// Sample test for aiebu_assembler::get_op_locations()
//
// Two modes, selected by the first argument:
//
//   config <path>   — Assemble a config ELF (aie4_config, config.json driven)
//                     and call get_op_locations(0x1c, "DPU").
//                     The directory must contain:
//                       config.json, test.asm, aie_runtime_control.asm,
//                       pdi.asm, aie_asm_elfs.asm, aie_asm_enable.asm,
//                       aie_asm_init.asm
//
//   target <path>   — Assemble a standalone target ELF (asm_aie4, single ASM
//                     buffer) and call get_op_locations(0x1c).
//                     The directory must contain test.asm (and its includes).

#include "aiebu/aiebu_assembler.h"
#include "aiebu/aiebu_error.h"

#include <elfio/elfio.hpp>

#include <cstddef>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

static void read_file(const std::string& path, std::vector<char>& buf)
{
  if (!std::filesystem::exists(path))
    throw std::runtime_error("file not found: " + path);
  std::ifstream f(path, std::ios::binary);
  auto sz = std::filesystem::file_size(path);
  buf.resize(sz);
  f.read(buf.data(), static_cast<std::streamsize>(sz));
}

static ELFIO::elfio load_elfio_from_buffer(const std::vector<char>& elf_data)
{
  ELFIO::elfio elf;
  std::istringstream iss(std::string(elf_data.begin(), elf_data.end()),
                         std::ios::in | std::ios::binary);
  if (!elf.load(iss))
    throw std::runtime_error("ELFIO::load failed for assembled ELF");
  return elf;
}

// Counts every {line, file} entry under all instances/columns (innermost entries).
static std::size_t count_total_line_entries(
    const std::vector<aiebu::aiebu_assembler::op_loc>& locs)
{
  std::size_t n = 0;
  for (const auto& loc : locs) {
    for (const auto& li : loc.line_info)
      n += li.entries.size();
  }
  return n;
}

// Prints opcode locations; returns total line entry count (0 if none).
static std::size_t print_results(const aiebu::aiebu_assembler::op_tbl& tbl,
                                 const std::string& label)
{
  const auto& locs = tbl.get_line_info();
  const std::size_t total = count_total_line_entries(locs);

  if (total == 0) {
    std::cout << "No opcodes found" << (label.empty() ? "" : " for " + label) << "\n";
    return 0;
  }

  std::cout << "total line entries: " << total << "\n";
  std::cout << "opcode locations" << (label.empty() ? "" : " for " + label) << ":\n";
  for (const auto& loc : locs) {
    if (!loc.inst_name.empty())
      std::cout << "  instance: " << loc.inst_name << "\n";
    for (const auto& li : loc.line_info) {
      std::cout << "    col=" << li.col << "\n";
      for (const auto& [linenum, filename] : li.entries)
        std::cout << "      line=" << linenum << "  file=" << filename << "\n";
    }
  }
  return total;
}

// ---------------------------------------------------------------------------
// config mode: aie4_config assembled from config.json + file search paths
// ---------------------------------------------------------------------------
static int test_config(const std::string& dir)
{
  std::vector<char> config_json;
  read_file(dir + "/config_target.json", config_json);

  // No "disabledump" flag → .dump section is included in the ELF, allowing
  // get_save_timestamps() to read timestamp locations from it.
  aiebu::aiebu_assembler as_build(
      aiebu::aiebu_assembler::buffer_type::aie4_config,
      {},
      {} /* no flags */,
      {dir},
      config_json);

  ELFIO::elfio elf = load_elfio_from_buffer(as_build.get_elf());
  aiebu::aiebu_assembler as(&elf);

  auto tbl = as.get_op_locations(0x1c, "DPU");  // 0x1c = SAVE_TIMESTAMPS
  const std::size_t n = print_results(tbl, "kernel DPU");
  if (n == 0) {
    std::cerr << "FAIL: expected at least one line entry for save_timestamps (kernel DPU)\n";
    return 1;
  }
  return 0;
}

// ---------------------------------------------------------------------------
// target mode: standalone asm_aie4 ELF from a single ASM buffer
// ---------------------------------------------------------------------------
static int test_target(const std::string& dir)
{
  std::vector<char> asm_buf;
  read_file(dir + "/test.asm", asm_buf);

  // No "disabledump" flag → .dump section is written.
  // libpaths contains the test directory so that .include directives inside
  // test.asm resolve correctly (aie_runtime_control.asm, pdi.asm, etc.).
  aiebu::aiebu_assembler as_build(
      aiebu::aiebu_assembler::buffer_type::asm_aie4,
      asm_buf,
      std::vector<std::string>{} /* no flags */,
      std::vector<std::string>{dir} /* search path for .include */);

  ELFIO::elfio elf = load_elfio_from_buffer(as_build.get_elf());
  aiebu::aiebu_assembler as(&elf);

  // No kernel name → non-config ELF path: scans .dump section directly
  // without group-based kernel:instance filtering.
  auto tbl = as.get_op_locations(0x1c);  // 0x1c = SAVE_TIMESTAMPS
  const std::size_t n = print_results(tbl, "");
  if (n == 0) {
    std::cerr << "FAIL: expected at least one line entry for save_timestamps\n";
    return 1;
  }
  return 0;
}

int main(int argc, char** argv)
{
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <config|target> <testcase_path>\n";
    return 1;
  }

  const std::string mode = argv[1];
  const std::string dir  = argv[2];

  try {
    if (mode == "config")
      return test_config(dir);
    if (mode == "target")
      return test_target(dir);

    std::cerr << "Unknown mode '" << mode << "'. Use 'config' or 'target'.\n";
    return 1;
  }
  catch (const aiebu::error& ex) {
    std::cerr << "aiebu error: " << ex.what() << " (code=" << ex.get_code() << ")\n";
    return 1;
  }
  catch (const std::exception& ex) {
    std::cerr << "error: " << ex.what() << "\n";
    return 1;
  }
}
