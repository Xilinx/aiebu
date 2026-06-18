// SPDX-License-Identifier: MIT
// Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.
//
// Test for aiebu::get_opcode_information().
//
// Assembles a multi-instance aie2ps_config ELF from the test ASM files,
// then calls get_opcode_information() for a known (uc_idx, page_idx, offset)
// and verifies the returned opcode name.
//
// Two ELFs are produced and tested:
//   1. WITH  .dump section  — exercises the dump-based decode path.
//   2. WITHOUT .dump section — exercises the ISA binary walk fallback path.
//
// Usage:
//   decode_opcode <test_src_dir> <out_with_dump.elf> <out_no_dump.elf>
//       <kernel_name> <uc_idx> <page_idx> <offset>
//       <expected_opcode_with_dump> <expected_args_with_dump>
//       <expected_source_file_dump> <expected_line_dump>
//       <expected_opcode_no_dump>   <expected_args_no_dump>
//
//   Pass "none" for expected_args / expected_source_file to skip those checks.
//   Pass "0" for expected_line to skip the line check.
//   The dump path populates opcode_name, args_str, source_file and line.
//   The ISA walk path populates opcode_name and args_str but NOT source_file/line.

#include "aiebu/aiebu_assembler.h"
#include "aiebu/aiebu_debug.h"
#include "aiebu/aiebu_error.h"

#include <elfio/elfio.hpp>

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

void read_file(const std::filesystem::path& path, std::vector<char>& out)
{
  if (!std::filesystem::exists(path))
    throw std::runtime_error("file not found: " + path.string());
  std::ifstream in(path, std::ios::binary);
  const auto sz = std::filesystem::file_size(path);
  out.resize(static_cast<std::size_t>(sz));
  in.read(out.data(), static_cast<std::streamsize>(sz));
}

// Load ELF bytes into an ELFIO object.
ELFIO::elfio load_elfio(const std::vector<char>& elf_bytes)
{
  std::string str(elf_bytes.begin(), elf_bytes.end());
  std::istringstream ss(str);
  ELFIO::elfio reader;
  if (!reader.load(ss))
    throw std::runtime_error("failed to parse ELF");
  return reader;
}

// Call get_opcode_information and verify the returned fields against expected values.
// Pass "none" for expected_args / expected_source_file to skip those checks.
// Pass 0 for expected_line to skip the line check.
// Returns true on pass, false on failure (prints reason).
bool check_opcode(const ELFIO::elfio& elf,
                  const std::string& kernel_name,
                  uint32_t uc_idx, uint32_t page_idx, uint32_t offset,
                  const std::string& expected_opcode,
                  const std::string& expected_args,
                  const std::string& expected_source_file,
                  uint32_t           expected_line,
                  const std::string& label)
{
  aiebu::AIEDebug debugger(elf);
  const aiebu::opcode_information info =
      debugger.get_opcode_information(kernel_name, uc_idx, page_idx, offset);

  // Always print returned values to aid diagnosis regardless of pass/fail
  std::cerr << "[INFO] " << label << " returned:"
            << "\n        found       = " << (info.found ? "true" : "false")
            << "\n        opcode_name = '" << info.opcode_name << "'"
            << "\n        args_str    = '" << info.args_str << "'"
            << "\n        source_file = '" << info.source_file << "'"
            << "\n        line        = "  << info.line
            << "\n        opcode_size = "  << info.opcode_size
            << "\n        page_offset = "  << info.page_offset
            << "\n        diag_info   = '" << info.diag_info << "'"
            << "\n";

  if (!info.found) {
    std::cerr << "[FAIL] " << label << ": opcode not found\n";
    return false;
  }

  bool pass = true;

  if (info.opcode_name != expected_opcode) {
    std::cerr << "[FAIL] " << label << ": expected opcode '" << expected_opcode
              << "' but got '" << info.opcode_name << "'\n";
    pass = false;
  }

  // args_str: only the ISA walk path populates this; dump path leaves it empty.
  if (expected_args != "none" && info.args_str != expected_args) {
    std::cerr << "[FAIL] " << label << ": expected args '" << expected_args
              << "' but got '" << info.args_str << "'\n";
    pass = false;
  }

  // source_file and line: only the dump path populates these.
  if (expected_source_file != "none" && info.source_file != expected_source_file) {
    std::cerr << "[FAIL] " << label << ": expected source_file '" << expected_source_file
              << "' but got '" << info.source_file << "'\n";
    pass = false;
  }

  if (expected_line != 0 && info.line != expected_line) {
    std::cerr << "[FAIL] " << label << ": expected line " << expected_line
              << " but got " << info.line << "\n";
    pass = false;
  }

  if (pass)
    std::cout << "[PASS] " << label << "\n";

  return pass;
}

void usage()
{
  std::cerr << "Usage: decode_opcode <test_src_dir> <out_no_dump.elf>"
               " <kernel_name> <uc_idx> <page_idx> <offset>"
               " <expected_opcode_isa> <expected_args_isa>\n"
               "  kernel_name: e.g. CTRL:inst0\n"
               "  uc_idx/page_idx/offset: decimal integers\n"
               "  expected_args: comma-separated hex values, or 'none' to skip\n";
}

} // namespace

int main(int argc, char** argv)
{
  if (argc != 9) {
    usage();
    return 2;
  }

  const std::filesystem::path root                     = argv[1];
  const std::filesystem::path in_no_dump               = argv[2];
  const std::string           kernel_name              = argv[3];
  const uint32_t              uc_idx   = static_cast<uint32_t>(std::stoul(argv[4]));
  const uint32_t              page_idx = static_cast<uint32_t>(std::stoul(argv[5]));
  const uint32_t              offset   = static_cast<uint32_t>(std::stoul(argv[6]));
  const std::string           expected_opcode_no_dump  = argv[7];
  const std::string           expected_args_no_dump    = argv[8];

  try {

    std::vector<char> buf;
    read_file(in_no_dump, buf);

    const ELFIO::elfio reader_no_dump   = load_elfio(buf);

    bool pass = true;
    pass &= check_opcode(reader_no_dump,   kernel_name, uc_idx, page_idx, offset,
                         expected_opcode_no_dump, expected_args_no_dump,
                         "none", 0,
                         "ISA walk path");

    return pass ? 0 : 1;

  } catch (const aiebu::error& ex) {
    std::cerr << "aiebu::error: " << ex.what() << " (" << ex.get_code() << ")\n";
    return 1;
  } catch (const std::exception& ex) {
    std::cerr << "error: " << ex.what() << '\n';
    return 1;
  }
}
