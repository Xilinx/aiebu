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
#include "aiebu/aiebu_decompress.h"
#include "aiebu/aiebu_error.h"

#include <elfio/elfio.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>

#include <cstring>
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
  for (const auto& sec : elf.sections)
    if (sec->get_flags() & ELFIO::SHF_COMPRESSED)
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
  for (const auto& esec : expected.sections) {
    const std::string& name = esec->get_name();

    // Only compare .ctrltext* and .ctrldata* sections.
    bool is_ctrl = (name.rfind(".ctrltext", 0) == 0 ||
                    name.rfind(".ctrldata", 0) == 0);
    if (!is_ctrl)
      continue;

    // Find same section in actual ELF.
    const ELFIO::section* asec = nullptr;
    for (const auto& sec : actual.sections) {
      if (sec->get_name() == name) {
        asec = sec.get();
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

// Compare metadata sections (.group, .rela.dyn, .dynsym, .dynstr, .symtab,
// .strtab) between two ELFs to verify compression does not break patching,
// kernel names, or instance mapping.
static bool compare_metadata_sections(const ELFIO::elfio& expected,
                                      const ELFIO::elfio& actual,
                                      const std::string& label)
{
  // Sections whose data must survive a compress/decompress round-trip unchanged.
  static const std::vector<std::string> metadata_names = {
    ".dynsym", ".dynstr", ".symtab", ".strtab", ".rela.dyn",
  };
  // Sections matched by prefix (there may be multiple .group.* or .rela.* sections).
  static const std::vector<std::string> metadata_prefixes = {
    ".group", ".rela.",
  };

  auto is_metadata = [&](const std::string& name) -> bool {
    for (const auto& n : metadata_names)
      if (name == n)
        return true;
    for (const auto& p : metadata_prefixes)
      if (name.rfind(p, 0) == 0)
        return true;
    return false;
  };

  bool ok = true;
  for (const auto& esec : expected.sections) {
    const std::string& name = esec->get_name();

    if (!is_metadata(name))
      continue;

    const ELFIO::section* asec = nullptr;
    for (const auto& sec : actual.sections) {
      if (sec->get_name() == name) {
        asec = sec.get();
        break;
      }
    }
    if (!asec) {
      std::cerr << "FAIL [" << label << "]: metadata section '" << name
                << "' missing from actual ELF\n";
      ok = false;
      continue;
    }

    if (esec->get_type() != asec->get_type()) {
      std::cerr << "FAIL [" << label << "]: section '" << name
                << "' type mismatch: expected=" << esec->get_type()
                << " actual=" << asec->get_type() << "\n";
      ok = false;
    }

    if (esec->get_size() != asec->get_size()) {
      std::cerr << "FAIL [" << label << "]: section '" << name
                << "' size mismatch: expected=" << esec->get_size()
                << " actual=" << asec->get_size() << "\n";
      ok = false;
      continue;
    }

    if (esec->get_size() > 0 && esec->get_data() && asec->get_data() &&
        std::memcmp(esec->get_data(), asec->get_data(), esec->get_size()) != 0) {
      std::cerr << "FAIL [" << label << "]: section '" << name
                << "' data mismatch\n";
      ok = false;
    }
  }

  // Also verify section count hasn't changed (no sections lost or duplicated).
  if (expected.sections.size() != actual.sections.size()) {
    std::cerr << "FAIL [" << label << "]: section count mismatch: expected="
              << expected.sections.size() << " actual="
              << actual.sections.size() << "\n";
    ok = false;
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
            << (100.0 * static_cast<double>(compressed.size()) / static_cast<double>(plain.size())) << "%)\n";
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

  auto decompressed = aiebu::decompress_elf(compressed);

  ELFIO::elfio decompressed_elf = load_elf(decompressed);
  if (has_compressed_sections(decompressed_elf)) {
    std::cerr << "FAIL [aie4_roundtrip_zstd]: decompressed ELF still has SHF_COMPRESSED sections\n";
    return false;
  }

  ELFIO::elfio plain_elf = load_elf(plain);
  if (!compare_ctrl_sections(plain_elf, decompressed_elf, "aie4_roundtrip_zstd"))
    return false;
  if (!compare_metadata_sections(plain_elf, decompressed_elf, "aie4_roundtrip_zstd"))
    return false;

  std::cout << "PASS [aie4_roundtrip_zstd]: " << decompressed.size()
            << " B, ctrl + metadata sections match\n";
  return true;
}

// Bare "compress" flag defaults to zstd and round-trips correctly.
static bool test_aie4_roundtrip_bare_flag(const std::string& dir)
{
  auto plain      = assemble_aie4(dir, {});
  auto compressed = assemble_aie4(dir, {"compress"});
  auto decompressed = aiebu::decompress_elf(compressed);

  ELFIO::elfio plain_elf        = load_elf(plain);
  ELFIO::elfio decompressed_elf = load_elf(decompressed);

  if (has_compressed_sections(decompressed_elf)) {
    std::cerr << "FAIL [aie4_roundtrip_bare_flag]: SHF_COMPRESSED sections remain\n";
    return false;
  }
  if (!compare_ctrl_sections(plain_elf, decompressed_elf, "aie4_roundtrip_bare_flag"))
    return false;
  if (!compare_metadata_sections(plain_elf, decompressed_elf, "aie4_roundtrip_bare_flag"))
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
    auto decompressed = aiebu::decompress_elf(compressed);

    ELFIO::elfio decompressed_elf = load_elf(decompressed);
    if (has_compressed_sections(decompressed_elf)) {
      std::cerr << "FAIL [aie4_roundtrip_levels]: level=" << lvl
                << " SHF_COMPRESSED sections remain\n";
      return false;
    }
    std::string lvl_label = "aie4_roundtrip_levels_L" + std::to_string(lvl);
    if (!compare_ctrl_sections(plain_elf, decompressed_elf, lvl_label))
      return false;
    if (!compare_metadata_sections(plain_elf, decompressed_elf, lvl_label))
      return false;

    std::cout << "PASS [aie4_roundtrip_levels]: level=" << lvl << "\n";
  }
  return true;
}

// decompress_elf on an already-uncompressed ELF returns it unchanged.
static bool test_aie4_decompress_noop(const std::string& dir)
{
  auto plain      = assemble_aie4(dir, {});
  auto plain_copy = plain;

  auto result = aiebu::decompress_elf(plain_copy);

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

  auto decompressed = aiebu::decompress_elf(compressed);

  ELFIO::elfio decompressed_elf = load_elf(decompressed);
  if (has_compressed_sections(decompressed_elf)) {
    std::cerr << "FAIL [aie2ps_roundtrip_zstd]: decompressed ELF still has SHF_COMPRESSED sections\n";
    return false;
  }

  ELFIO::elfio plain_elf = load_elf(plain);
  if (!compare_ctrl_sections(plain_elf, decompressed_elf, "aie2ps_roundtrip_zstd"))
    return false;
  if (!compare_metadata_sections(plain_elf, decompressed_elf, "aie2ps_roundtrip_zstd"))
    return false;

  std::cout << "PASS [aie2ps_roundtrip_zstd]: " << decompressed.size()
            << " B, ctrl + metadata sections match\n";
  return true;
}

// AIE2PS: decompress_elf on an uncompressed ELF returns it unchanged.
static bool test_aie2ps_decompress_noop(const std::string& dir)
{
  auto plain      = assemble_aie2ps(dir, {});
  auto plain_copy = plain;

  auto result = aiebu::decompress_elf(plain_copy);

  if (result != plain) {
    std::cerr << "FAIL [aie2ps_decompress_noop]: result differs from input\n";
    return false;
  }
  std::cout << "PASS [aie2ps_decompress_noop]: uncompressed ELF returned unchanged\n";
  return true;
}

// ---------------------------------------------------------------------------
// Decompress API coverage
// ---------------------------------------------------------------------------

// Correctness test for every API in aiebu_decompress.h:
//   is_elf_compressed()     — true for compressed, false for plain
//   decompress_elf()        — round-trip produces correct output
//   get_section_uncompressed_size() — returns uncompressed size (> stored size)
//   copy_section_uncompressed_data()     — decompresses into caller buffer; bytes match plain
//
// Peak RSS measurement is intentionally omitted here: assembly runs in this
// same process before any snapshot could be taken, inflating the baseline and
// making any delta meaningless.  Accurate memory reporting is done by the
// standalone decompress_memory_report binary which takes a pre-compressed
// ELF file as input and starts with a clean process baseline.
static bool test_decompress_api_coverage(const std::string& dir)
{
  auto compressed = assemble_aie4(dir, {"compress=zstd"});
  auto plain      = assemble_aie4(dir, {});

  // --- is_elf_compressed() ---
  if (!aiebu::is_elf_compressed(compressed)) {
    std::cerr << "FAIL [decompress_api_coverage]: is_elf_compressed(compressed) returned false\n";
    return false;
  }
  if (aiebu::is_elf_compressed(plain)) {
    std::cerr << "FAIL [decompress_api_coverage]: is_elf_compressed(plain) returned true\n";
    return false;
  }

  // --- decompress_elf() ---
  auto decompressed = aiebu::decompress_elf(compressed);

  // --- get_section_uncompressed_size() and copy_section_uncompressed_data() ---
  ELFIO::elfio compressed_elfio = load_elf(compressed);
  ELFIO::elfio plain_elfio      = load_elf(plain);
  bool section_ok = true;

  for (const auto& sec : compressed_elfio.sections) {
    if (!(sec->get_flags() & ELFIO::SHF_COMPRESSED))
      continue;

    // get_section_uncompressed_size() returns ch_size (the original uncompressed size
    // from the Elf_Chdr header).  For small sections zstd can expand the data,
    // so stored size (sizeof(Chdr)+compressed_payload) may exceed ch_size —
    // the only invariant is that ch_size is positive.
    const std::size_t data_sz = aiebu::get_section_uncompressed_size(sec.get(), compressed_elfio);
    if (data_sz == 0) {
      std::cerr << "FAIL [decompress_api_coverage]: get_section_uncompressed_size('"
                << sec->get_name() << "') returned 0\n";
      section_ok = false;
      continue;
    }

    // copy_section_uncompressed_data() must decompress into a caller buffer.
    std::vector<char> buf(data_sz);
    const std::size_t written =
        aiebu::copy_section_uncompressed_data(sec.get(), compressed_elfio, buf.data(), buf.size());
    if (written != data_sz) {
      std::cerr << "FAIL [decompress_api_coverage]: copy_section_uncompressed_data('"
                << sec->get_name() << "') wrote " << written
                << " B, expected " << data_sz << " B\n";
      section_ok = false;
      continue;
    }

    // Verify bytes match the plain ELF section.
    const ELFIO::section* plain_sec = nullptr;
    for (const auto& ps : plain_elfio.sections) {
      if (ps->get_name() == sec->get_name()) {
        plain_sec = ps.get();
        break;
      }
    }
    if (!plain_sec) {
      std::cerr << "FAIL [decompress_api_coverage]: section '"
                << sec->get_name() << "' missing from plain ELF\n";
      section_ok = false;
    } else if (plain_sec->get_size() != data_sz) {
      std::cerr << "FAIL [decompress_api_coverage]: copy_section_uncompressed_data('"
                << sec->get_name() << "') size " << data_sz
                << " != plain ELF section size " << plain_sec->get_size() << "\n";
      section_ok = false;
    } else if (std::memcmp(buf.data(), plain_sec->get_data(), data_sz) != 0) {
      std::cerr << "FAIL [decompress_api_coverage]: copy_section_uncompressed_data('"
                << sec->get_name() << "') data mismatch vs plain ELF\n";
      section_ok = false;
    }
  }

  if (!section_ok)
    return false;

  std::cout << "PASS [decompress_api_coverage]: all decompress APIs verified\n";
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

    // Decompress API correctness coverage.
    ok = test_decompress_api_coverage(aie4_dir)   && ok;

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
