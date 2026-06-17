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

// Minimal xrt-kernels config: one kernel ("CTRL"), one instance ("inst0").
constexpr char k_config_json[] = R"json({
  "xrt-kernels": [
    {
      "name": "CTRL",
      "arguments": [
        { "name": "arg0", "type": "char *", "offset": "0x00" },
        { "name": "arg1", "type": "char *", "offset": "0x08" },
        { "name": "arg2", "type": "char *", "offset": "0x10" },
        { "name": "arg3", "type": "char *", "offset": "0x18" }
      ],
      "instance": [
        {
          "id": "inst0",
          "ctrl_code_file": "./ml_asm/merged_control.asm",
          "patch_info_file": "./external_buffer_id.json"
        }
      ]
    }
  ]
}
)json";

void read_file(const std::filesystem::path& path, std::vector<char>& out)
{
  if (!std::filesystem::exists(path))
    throw std::runtime_error("file not found: " + path.string());
  std::ifstream in(path, std::ios::binary);
  const auto sz = std::filesystem::file_size(path);
  out.resize(static_cast<std::size_t>(sz));
  in.read(out.data(), static_cast<std::streamsize>(sz));
}

void add_asm_tree(const std::filesystem::path& root, aiebu::file_artifact& artifact)
{
  const std::pair<const char*, const char*> dirs[] = {
      {"../ml_asm/", "ml_asm"},
      {"../asm/",    "asm"},
  };
  for (const auto& [key_prefix, dir_name] : dirs) {
    const std::filesystem::path dir = root / dir_name;
    if (!std::filesystem::is_directory(dir))
      throw std::runtime_error("not a directory: " + dir.string());
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
      if (!entry.is_regular_file() || entry.path().extension() != ".asm")
        continue;
      std::vector<char> buf;
      read_file(entry.path(), buf);
      artifact.add_vfile(std::string(key_prefix) + entry.path().filename().string(), buf);
    }
  }
}

// Assemble and write an ELF. flags controls whether .dump is included.
std::vector<char> assemble(const std::filesystem::path& root,
                           const std::vector<std::string>& flags)
{
  std::vector<char> merged_asm;
  read_file(root / "ml_asm" / "merged_control.asm", merged_asm);

  std::vector<char> patch_json;
  read_file(root / "external_buffer_id.json", patch_json);

  std::vector<char> config_json;
  config_json.assign(k_config_json, k_config_json + std::strlen(k_config_json));

  aiebu::file_artifact artifact;
  artifact.add_vfile("./ml_asm/merged_control.asm", merged_asm);
  artifact.add_vfile("./external_buffer_id.json",   patch_json);
  add_asm_tree(root, artifact);

  aiebu::aiebu_assembler as(aiebu::aiebu_assembler::buffer_type::aie2ps_config,
                            config_json, artifact, flags);
  return as.get_elf();
}

void write_elf(const std::filesystem::path& path, const std::vector<char>& elf)
{
  std::ofstream out(path, std::ios::binary);
  out.write(elf.data(), static_cast<std::streamsize>(elf.size()));
  if (!out)
    throw std::runtime_error("failed to write " + path.string());
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
  std::cerr << "Usage: decode_opcode <test_src_dir> <out_with_dump.elf> <out_no_dump.elf>"
               " <kernel_name> <uc_idx> <page_idx> <offset>"
               " <expected_opcode_dump> <expected_args_dump>"
               " <expected_source_file_dump> <expected_line_dump>"
               " <expected_opcode_isa> <expected_args_isa>\n"
               "  kernel_name: e.g. CTRL:inst0\n"
               "  uc_idx/page_idx/offset: decimal integers\n"
               "  expected_args: comma-separated hex values, or 'none' to skip\n"
               "  expected_source_file: path string, or 'none' to skip\n"
               "  expected_line: integer, or '0' to skip\n";
}

} // namespace

int main(int argc, char** argv)
{
  if (argc != 14) {
    usage();
    return 2;
  }

  const std::filesystem::path root                     = argv[1];
  const std::filesystem::path out_with_dump            = argv[2];
  const std::filesystem::path out_no_dump              = argv[3];
  const std::string           kernel_name              = argv[4];
  const uint32_t              uc_idx   = static_cast<uint32_t>(std::stoul(argv[5]));
  const uint32_t              page_idx = static_cast<uint32_t>(std::stoul(argv[6]));
  const uint32_t              offset   = static_cast<uint32_t>(std::stoul(argv[7]));
  const std::string           expected_opcode_dump     = argv[8];
  const std::string           expected_args_dump       = argv[9];   // "none" → skip
  const std::string           expected_source_file     = argv[10];  // "none" → skip
  const uint32_t              expected_line            = static_cast<uint32_t>(std::stoul(argv[11])); // 0 → skip
  const std::string           expected_opcode_no_dump  = argv[12];
  const std::string           expected_args_no_dump    = argv[13];

  try {
    // Assemble with dump (primary path: .dump section lookup)
    const std::vector<char> elf_with_dump = assemble(root, {});
    write_elf(out_with_dump, elf_with_dump);

    // Assemble without dump (fallback path: ISA binary walk)
    const std::vector<char> elf_no_dump = assemble(root, {"disabledump"});
    write_elf(out_no_dump, elf_no_dump);

    const ELFIO::elfio reader_with_dump = load_elfio(elf_with_dump);
    const ELFIO::elfio reader_no_dump   = load_elfio(elf_no_dump);

    bool pass = true;
    pass &= check_opcode(reader_with_dump, kernel_name, uc_idx, page_idx, offset,
                         expected_opcode_dump, expected_args_dump,
                         expected_source_file, expected_line,
                         "dump path");
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
