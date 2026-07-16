// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.
//
// Unit tests for the AIE Coredump ELF generation feature.
//
// Test 1 – assembler_coredump_no_meta:
//   Construct aiebu_assembler for AIE4 without metadata; verify a non-empty
//   ELF is returned and write to disk for manual inspection.
//
// Test 2 – assembler_coredump_with_meta:
//   Construct aiebu_assembler for AIE4 with full metadata; verify a non-empty
//   ELF is returned and write to disk for manual inspection.
//
// Test 3 – get_coredump_meta:
//   Full write→read round-trip: build a coredump ELF with known metadata,
//   read it back via get_coredump_meta(), verify all fields match.
//
// Test 4 – assembler_invalid_type:
//   Pass a non-coredump buffer_type to the coredump assembler constructor;
//   verify aiebu::error with error_code::invalid_buffer_type is thrown.

#include "aiebu/aiebu_assembler.h"
#include "aiebu/aiebu_error.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// helpers
// ---------------------------------------------------------------------------

struct result {
  bool        passed  = false;
  std::string message;
};

// Make a simple synthetic blob of `n` bytes filled with a fixed pattern byte.
static std::vector<char> make_blob(size_t n)
{
  constexpr char BLOB_FILL = static_cast<char>(0xAB);  // arbitrary non-zero fill pattern
  return std::vector<char>(n, BLOB_FILL);
}

// ---------------------------------------------------------------------------
// ELF32 LE field readers (offsets match our coredump_elfwriter layout)
// ---------------------------------------------------------------------------
static uint8_t  elf_u8 (const std::vector<char>& e, size_t off) { return static_cast<uint8_t>(e.at(off)); }
static uint16_t elf_u16(const std::vector<char>& e, size_t off) { uint16_t v = 0; std::memcpy(&v, e.data() + off, sizeof(v)); return v; }

// ELF32 header offsets
constexpr size_t  OFF_EI_OSABI  =  7;   // e_ident[EI_OSABI]
constexpr size_t  OFF_E_TYPE    = 16;   // e_type     (uint16_t)
constexpr size_t  OFF_E_PHNUM   = 44;   // e_phnum    (uint16_t)
constexpr size_t  OFF_E_SHNUM   = 48;   // e_shnum    (uint16_t)

// OS/ABI values for AIE architectures (mirrors aie_elf_constants.h, internal header)
constexpr uint8_t OSABI_AIE4    = 0x4BU;

// Synthetic timestamps used in tests (arbitrary values, chosen for distinctness)
constexpr uint64_t TS_WITH_META  = 1234567890123456789ULL;  // test_assembler_coredump_with_meta
constexpr uint64_t TS_ROUND_TRIP = 9876543210ULL;           // test_get_coredump_meta

static result check_elf_structure(const std::vector<char>& elf, uint8_t expected_osabi)
{
  constexpr uint16_t ET_CORE         = 4;
  constexpr uint16_t EXPECTED_PHNUM  = 2;  // PT_NOTE + PT_LOAD
  constexpr uint16_t EXPECTED_SHNUM  = 0;  // no section header table

  if (elf_u16(elf, OFF_E_TYPE) != ET_CORE)
    return {false, "e_type is not ET_CORE"};
  if (elf_u8(elf, OFF_EI_OSABI) != expected_osabi)
    return {false, "EI_OSABI mismatch"};
  if (elf_u16(elf, OFF_E_PHNUM) != EXPECTED_PHNUM)
    return {false, "e_phnum is not 2"};
  if (elf_u16(elf, OFF_E_SHNUM) != EXPECTED_SHNUM)
    return {false, "e_shnum is not 0 (section header table should be absent)"};
  return {true, ""};
}

static void write_elf(const std::vector<char>& elf, const std::string& path)
{
  std::ofstream f(path, std::ios::binary);
  f.write(elf.data(), static_cast<std::streamsize>(elf.size()));
  std::cout << "  wrote: " << path << " (" << elf.size() << " bytes)\n";
}

// ---------------------------------------------------------------------------
// Test 1: assembler coredump, no metadata
// ---------------------------------------------------------------------------
static result test_assembler_coredump_no_meta()
{
  try {
    const auto blob = make_blob(256);
    aiebu::aiebu_assembler as(aiebu::aiebu_assembler::buffer_type::coredump_aie4,
                              blob,
                              std::nullopt);
    const auto elf = as.get_elf();
    if (elf.empty())
      return {false, "get_elf() returned empty vector"};
    std::cout << "  ELF size (no meta): " << elf.size() << " bytes\n";

    // Verify ELF structure
    auto r = check_elf_structure(elf, OSABI_AIE4);
    if (!r.passed) return r;

    // No metadata supplied — AMDAIE_CORE note must be absent
    aiebu::aiebu_assembler reader(elf);
    if (reader.get_coredump_meta().has_value())
      return {false, "get_coredump_meta() should return nullopt when built without meta"};

    write_elf(elf, "coredump_no_meta.elf");
    return {true, ""};
  } catch (const std::exception& ex) {
    return {false, std::string("unexpected exception: ") + ex.what()};
  }
}

// ---------------------------------------------------------------------------
// Test 2: assembler coredump, with metadata
// ---------------------------------------------------------------------------
static result test_assembler_coredump_with_meta()
{
  try {
    const auto blob = make_blob(512);

    aiebu::aie_coredump_meta meta;
    meta.timestamp_ns    = TS_WITH_META;
    meta.driver_version  = "amdxdna-1.2.3";
    meta.fw_version      = "fw-0.9.0";
    meta.device_info     = "NPU Medusa";
    meta.context_status  = aiebu::aie_context_status::timeout;
    meta.uuid            = "a1b2c3d4-e5f6-7890-abcd-ef1234567890";

    aiebu::aiebu_assembler as(aiebu::aiebu_assembler::buffer_type::coredump_aie4,
                              blob,
                              std::make_optional(meta));
    const auto elf = as.get_elf();
    if (elf.empty())
      return {false, "get_elf() returned empty vector"};
    std::cout << "  ELF size (with meta): " << elf.size() << " bytes\n";

    // Verify ELF structure
    auto r = check_elf_structure(elf, OSABI_AIE4);
    if (!r.passed) return r;

    write_elf(elf, "coredump_with_meta.elf");
    return {true, ""};
  } catch (const std::exception& ex) {
    return {false, std::string("unexpected exception: ") + ex.what()};
  }
}

// ---------------------------------------------------------------------------
// Test 3: round-trip — write coredump ELF then read metadata back
// ---------------------------------------------------------------------------
static result test_get_coredump_meta()
{
  try {
    const auto blob = make_blob(256);

    aiebu::aie_coredump_meta in_meta;
    in_meta.timestamp_ns   = TS_ROUND_TRIP;
    in_meta.driver_version = "amdxdna-2.0.0";
    in_meta.fw_version     = "fw-1.0.0";
    in_meta.device_info    = "NPU Strix";
    in_meta.context_status = aiebu::aie_context_status::error;
    in_meta.uuid           = "deadbeef-dead-beef-dead-beefdeadbeef";

    // Write
    aiebu::aiebu_assembler writer(aiebu::aiebu_assembler::buffer_type::coredump_aie4,
                                  blob, std::make_optional(in_meta));
    const auto elf = writer.get_elf();

    // Read back
    aiebu::aiebu_assembler reader(elf);
    const auto out_meta = reader.get_coredump_meta();
    if (!out_meta)
      return {false, "get_coredump_meta() returned nullopt"};
    if (out_meta->timestamp_ns   != in_meta.timestamp_ns)
      return {false, "timestamp_ns mismatch"};
    if (out_meta->driver_version != in_meta.driver_version)
      return {false, "driver_version mismatch"};
    if (out_meta->fw_version     != in_meta.fw_version)
      return {false, "fw_version mismatch"};
    if (out_meta->device_info    != in_meta.device_info)
      return {false, "device_info mismatch"};
    if (out_meta->context_status != in_meta.context_status)
      return {false, "context_status mismatch"};
    if (out_meta->uuid           != in_meta.uuid)
      return {false, "uuid mismatch"};
    std::cout << "  uuid: " << out_meta->uuid << "\n";
    return {true, ""};
  } catch (const std::exception& ex) {
    return {false, std::string("unexpected exception: ") + ex.what()};
  }
}

// ---------------------------------------------------------------------------
// Test 4: invalid buffer_type passed to coredump assembler constructor
// ---------------------------------------------------------------------------
static result test_assembler_invalid_type()
{
  const auto blob = make_blob(64);
  try {
    // blob_instr_dpu is not a coredump type — must throw.
    aiebu::aiebu_assembler as(aiebu::aiebu_assembler::buffer_type::blob_instr_dpu,
                              blob,
                              std::nullopt);
    return {false, "no exception thrown for invalid buffer_type"};
  } catch (const aiebu::error& ex) {
    constexpr int EXPECTED = aiebu_invalid_buffer_type;
    if (ex.get_code() != EXPECTED)
      return {false, "wrong error code " + std::to_string(ex.get_code())
                     + " (expected " + std::to_string(EXPECTED) + ")"};
    std::cout << "  expected error thrown: " << ex.what() << "\n";
    return {true, ""};
  } catch (const std::exception& ex) {
    return {false, std::string("unexpected exception type: ") + ex.what()};
  }
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main()
{
  struct tc { const char* name; result (*fn)(); };
  const std::array<tc, 4> tests = {{
    {"assembler_coredump_no_meta",   test_assembler_coredump_no_meta},
    {"assembler_coredump_with_meta", test_assembler_coredump_with_meta},
    {"get_coredump_meta",            test_get_coredump_meta},
    {"assembler_invalid_type",       test_assembler_invalid_type},
  }};

  int failures = 0;
  for (const auto& t : tests) {
    const result r = t.fn();
    if (r.passed) {
      std::cout << "PASS: " << t.name << "\n";
    } else {
      std::cout << "FAIL: " << t.name << " – " << r.message << "\n";
      ++failures;
    }
  }
  return failures > 0 ? 1 : 0;
}
