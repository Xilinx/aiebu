// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
//
// Test: ELF compression + decompression round-trip via the AIEBU C++ API.
//
// Architectures covered:
//   aie4_config   (move_ddr_to_memtile)  — full compress/decompress suite
//   aie2ps_config (eff_net_coal)         — decompress no-op on plain ELF
//
// Usage: elf_compress_decompress_cpp <aie4_dir> <aie2ps_dir>
//   e.g. elf_compress_decompress_cpp test/cpp_test/aie4/move_ddr_to_memtile test/cpp_test/aie2ps/eff_net_coal

#include "aiebu/aiebu_assembler.h"
#include "aiebu/aiebu_error.h"

#include <elfio/elfio.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// File helpers
// ---------------------------------------------------------------------------

static void read_file(const std::string& path, std::vector<char>& buf)
{
  std::ifstream f(path, std::ios::binary);
  if (!f)
    throw std::runtime_error("cannot open: " + path);
  buf.assign(std::istreambuf_iterator<char>(f), {});
}

// ---------------------------------------------------------------------------
// ELF comparison helpers
// ---------------------------------------------------------------------------

// Load an ELF from an in-memory buffer via ELFIO — zero copy, fully seekable.
static ELFIO::elfio load_elf(const std::vector<char>& data)
{
  ELFIO::elfio elf;
  boost::interprocess::ibufferstream is(data.data(), data.size());
  if (!elf.load(is))
    throw std::runtime_error("ELFIO::load failed");
  return elf;
}

// Return true if the ELF has any SHF_COMPRESSED section.
static bool has_compressed_sections(const ELFIO::elfio& elf)
{
  for (ELFIO::Elf_Half i = 0; i < elf.sections.size(); ++i)
    if (elf.sections[i]->get_flags() & ELFIO::SHF_COMPRESSED)
      return true;
  return false;
}

// Compare section data of all .ctrltext* / .ctrldata* sections between two
// ELFs.  Returns true only if every such section found in `expected` exists
// in `actual` with identical data.
//
// We compare section data only (not sh_addr) because compression permanently
// zeroes sh_addr (standard Elf_Chdr has no field to preserve it), so a
// byte-for-byte ELF comparison would always fail after a compress/decompress
// round-trip.
static bool compare_ctrl_sections(const ELFIO::elfio& expected,
                                  const ELFIO::elfio& actual,
                                  const std::string& label)
{
  bool ok = true;
  for (ELFIO::Elf_Half i = 0; i < expected.sections.size(); ++i) {
    const auto* esec = expected.sections[i];
    const std::string& name = esec->get_name();

    // Only compare .ctrltext* and .ctrldata* sections.
    bool is_ctrl = (name.rfind(".ctrltext", 0) == 0 ||
                    name.rfind(".ctrldata", 0) == 0);
    if (!is_ctrl)
      continue;

    // Find same section in actual ELF.
    const ELFIO::section* asec = nullptr;
    for (ELFIO::Elf_Half j = 0; j < actual.sections.size(); ++j) {
      if (actual.sections[j]->get_name() == name) {
        asec = actual.sections[j];
        break;
      }
    }
    if (!asec) {
      std::cerr << "FAIL [" << label << "]: section '" << name
                << "' missing from actual ELF\n";
      ok = false;
      continue;
    }

    if (esec->get_size() != asec->get_size()) {
      std::cerr << "FAIL [" << label << "]: section '" << name
                << "' size mismatch: expected=" << esec->get_size()
                << " actual=" << asec->get_size() << "\n";
      ok = false;
      continue;
    }

    if (esec->get_size() > 0 &&
        std::memcmp(esec->get_data(), asec->get_data(), esec->get_size()) != 0) {
      std::cerr << "FAIL [" << label << "]: section '" << name
                << "' data mismatch\n";
      ok = false;
    }
  }
  return ok;
}

// ---------------------------------------------------------------------------
// AIE4 helpers
// ---------------------------------------------------------------------------

static std::vector<char> assemble_aie4(const std::string& dir,
                                       const std::vector<std::string>& flags)
{
  std::vector<char> config_json;
  read_file(dir + "/config.json", config_json);

  aiebu::file_artifact art;
  std::vector<char> buf;
  read_file(dir + "/test.asm",                buf); art.add_vfile("test.asm",                buf);
  read_file(dir + "/aie4_pdi.asm",            buf); art.add_vfile("aie4_pdi.asm",            buf);
  read_file(dir + "/external_buffer_id.json", buf); art.add_vfile("external_buffer_id.json", buf);

  aiebu::aiebu_assembler as(
      aiebu::aiebu_assembler::buffer_type::aie4_config,
      config_json, art, flags);
  return as.get_elf();
}

// ---------------------------------------------------------------------------
// AIE2PS helpers
// ---------------------------------------------------------------------------

static std::vector<char> assemble_aie2ps(const std::string& dir,
                                         const std::vector<std::string>& flags)
{
  std::vector<char> config_json;
  read_file(dir + "/config.json", config_json);

  // Build a file_artifact with every file the aie2ps_config preprocessor
  // may need for the eff_net_coal testcase.
  aiebu::file_artifact art;
  std::vector<char> buf;

  auto add = [&](const std::string& vname, const std::string& fpath) {
    read_file(fpath, buf);
    art.add_vfile(vname, buf);
  };

  // config.json references ctrl_code_file as "./ml_asm/merged_control.asm".
  // merged_control.asm includes siblings as "ml_asm/..." and "asm/..." (no leading "./").
  add("./ml_asm/merged_control.asm",    dir + "/ml_asm/merged_control.asm");
  add("ml_asm/aie_runtime_control.asm", dir + "/ml_asm/aie_runtime_control.asm");
  add("asm/pdi.asm",                    dir + "/asm/pdi.asm");
  add("asm/aie_asm_elfs.asm",           dir + "/asm/aie_asm_elfs.asm");
  add("asm/aie_asm_enable.asm",         dir + "/asm/aie_asm_enable.asm");
  add("asm/aie_asm_init.asm",           dir + "/asm/aie_asm_init.asm");

  aiebu::aiebu_assembler as(
      aiebu::aiebu_assembler::buffer_type::aie2ps_config,
      config_json, art, flags);
  return as.get_elf();
}

// ---------------------------------------------------------------------------
// Test cases — each returns true on pass, false on failure.
// ---------------------------------------------------------------------------

// Compression produces a smaller, distinct ELF.
static bool test_aie4_compression_shrinks(const std::string& dir)
{
  auto plain      = assemble_aie4(dir, {});
  auto compressed = assemble_aie4(dir, {"compress=zstd"});

  if (compressed == plain) {
    std::cerr << "FAIL [aie4_compression_shrinks]: compressed ELF identical to plain\n";
    return false;
  }
  if (compressed.size() >= plain.size()) {
    std::cerr << "FAIL [aie4_compression_shrinks]: compressed (" << compressed.size()
              << " B) not smaller than plain (" << plain.size() << " B)\n";
    return false;
  }
  std::cout << "PASS [aie4_compression_shrinks]: "
            << plain.size() << " B -> " << compressed.size() << " B ("
            << (100.0 * compressed.size() / plain.size()) << "%)\n";
  return true;
}

// compress=zstd round-trip: section data of decompress(compress(ELF)) matches plain ELF.
static bool test_aie4_roundtrip_zstd(const std::string& dir)
{
  auto plain      = assemble_aie4(dir, {});
  auto compressed = assemble_aie4(dir, {"compress=zstd"});

  ELFIO::elfio compressed_elf = load_elf(compressed);
  if (!has_compressed_sections(compressed_elf)) {
    std::cerr << "FAIL [aie4_roundtrip_zstd]: compressed ELF has no SHF_COMPRESSED sections\n";
    return false;
  }

  auto decompressed = aiebu::aiebu_assembler::decompress_elf(std::move(compressed));

  ELFIO::elfio decompressed_elf = load_elf(decompressed);
  if (has_compressed_sections(decompressed_elf)) {
    std::cerr << "FAIL [aie4_roundtrip_zstd]: decompressed ELF still has SHF_COMPRESSED sections\n";
    return false;
  }

  ELFIO::elfio plain_elf = load_elf(plain);
  if (!compare_ctrl_sections(plain_elf, decompressed_elf, "aie4_roundtrip_zstd"))
    return false;

  std::cout << "PASS [aie4_roundtrip_zstd]: " << decompressed.size() << " B, ctrl sections match\n";
  return true;
}

// Bare "compress" flag defaults to zstd and round-trips correctly.
static bool test_aie4_roundtrip_bare_flag(const std::string& dir)
{
  auto plain      = assemble_aie4(dir, {});
  auto compressed = assemble_aie4(dir, {"compress"});
  auto decompressed = aiebu::aiebu_assembler::decompress_elf(std::move(compressed));

  ELFIO::elfio plain_elf        = load_elf(plain);
  ELFIO::elfio decompressed_elf = load_elf(decompressed);

  if (has_compressed_sections(decompressed_elf)) {
    std::cerr << "FAIL [aie4_roundtrip_bare_flag]: SHF_COMPRESSED sections remain\n";
    return false;
  }
  if (!compare_ctrl_sections(plain_elf, decompressed_elf, "aie4_roundtrip_bare_flag"))
    return false;

  std::cout << "PASS [aie4_roundtrip_bare_flag]\n";
  return true;
}

// compress=zstd:<level> round-trips at various levels.
static bool test_aie4_roundtrip_levels(const std::string& dir)
{
  auto plain     = assemble_aie4(dir, {});
  ELFIO::elfio plain_elf = load_elf(plain);

  for (int lvl : {1, 3, 9, 19}) {
    auto compressed   = assemble_aie4(dir, {"compress=zstd:" + std::to_string(lvl)});
    auto decompressed = aiebu::aiebu_assembler::decompress_elf(std::move(compressed));

    ELFIO::elfio decompressed_elf = load_elf(decompressed);
    if (has_compressed_sections(decompressed_elf)) {
      std::cerr << "FAIL [aie4_roundtrip_levels]: level=" << lvl
                << " SHF_COMPRESSED sections remain\n";
      return false;
    }
    if (!compare_ctrl_sections(plain_elf, decompressed_elf,
                               "aie4_roundtrip_levels_L" + std::to_string(lvl)))
      return false;

    std::cout << "PASS [aie4_roundtrip_levels]: level=" << lvl << "\n";
  }
  return true;
}

// decompress_elf on an already-uncompressed ELF returns it unchanged (move, no copy).
static bool test_aie4_decompress_noop(const std::string& dir)
{
  auto plain      = assemble_aie4(dir, {});
  auto plain_copy = plain;

  auto result = aiebu::aiebu_assembler::decompress_elf(std::move(plain_copy));

  if (result != plain) {
    std::cerr << "FAIL [aie4_decompress_noop]: result differs from input\n";
    return false;
  }
  std::cout << "PASS [aie4_decompress_noop]: uncompressed ELF returned unchanged\n";
  return true;
}

// AIE2PS: compression round-trip — ctrl section data matches plain ELF.
static bool test_aie2ps_roundtrip_zstd(const std::string& dir)
{
  auto plain      = assemble_aie2ps(dir, {});
  auto compressed = assemble_aie2ps(dir, {"compress=zstd"});

  ELFIO::elfio compressed_elf = load_elf(compressed);
  if (!has_compressed_sections(compressed_elf)) {
    std::cerr << "FAIL [aie2ps_roundtrip_zstd]: compressed ELF has no SHF_COMPRESSED sections\n";
    return false;
  }

  auto decompressed = aiebu::aiebu_assembler::decompress_elf(std::move(compressed));

  ELFIO::elfio decompressed_elf = load_elf(decompressed);
  if (has_compressed_sections(decompressed_elf)) {
    std::cerr << "FAIL [aie2ps_roundtrip_zstd]: decompressed ELF still has SHF_COMPRESSED sections\n";
    return false;
  }

  ELFIO::elfio plain_elf = load_elf(plain);
  if (!compare_ctrl_sections(plain_elf, decompressed_elf, "aie2ps_roundtrip_zstd"))
    return false;

  std::cout << "PASS [aie2ps_roundtrip_zstd]: " << decompressed.size() << " B, ctrl sections match\n";
  return true;
}

// AIE2PS: decompress_elf on an uncompressed ELF returns it unchanged.
static bool test_aie2ps_decompress_noop(const std::string& dir)
{
  auto plain      = assemble_aie2ps(dir, {});
  auto plain_copy = plain;

  auto result = aiebu::aiebu_assembler::decompress_elf(std::move(plain_copy));

  if (result != plain) {
    std::cerr << "FAIL [aie2ps_decompress_noop]: result differs from input\n";
    return false;
  }
  std::cout << "PASS [aie2ps_decompress_noop]: uncompressed ELF returned unchanged\n";
  return true;
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

int main(int argc, char** argv)
{
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0]
              << " <aie4_testcase_path> <aie2ps_testcase_path>\n";
    return 1;
  }

  const std::string aie4_dir   = argv[1];
  const std::string aie2ps_dir = argv[2];

  try {
    bool ok = true;

    // AIE4 tests
    ok = test_aie4_compression_shrinks(aie4_dir) && ok;
    ok = test_aie4_roundtrip_zstd(aie4_dir)      && ok;
    ok = test_aie4_roundtrip_bare_flag(aie4_dir)  && ok;
    ok = test_aie4_roundtrip_levels(aie4_dir)     && ok;
    ok = test_aie4_decompress_noop(aie4_dir)      && ok;

    // AIE2PS tests
    ok = test_aie2ps_roundtrip_zstd(aie2ps_dir)   && ok;
    ok = test_aie2ps_decompress_noop(aie2ps_dir)   && ok;

    if (!ok) {
      std::cerr << "One or more tests FAILED\n";
      return 1;
    }
    std::cout << "All tests PASSED\n";
    return 0;
  }
  catch (const aiebu::error& ex) {
    std::cerr << "aiebu::error: " << ex.what() << " (code=" << ex.get_code() << ")\n";
    return 1;
  }
  catch (const std::exception& ex) {
    std::cerr << "exception: " << ex.what() << "\n";
    return 1;
  }
}
