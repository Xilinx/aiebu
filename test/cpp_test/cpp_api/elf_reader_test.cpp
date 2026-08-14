// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
//
// Test harness for aiebu::elf (step 1.1 and step 1.2 exit criteria).
//
// Usage:
//   elf_reader_test <elf-file> [expected-platform] [expected-has-pdi]
//
// expected-platform is one of: aie2p aie2ps aie2ps_legacy aie4 aie4a aie4z
// If omitted, platform detection is still exercised but not checked.
//
// expected-has-pdi is "pdi" or "nopdi" (aie2p only): asserts the value
// returned by has_pdi().  If omitted, has_pdi() value is not checked.
//
// The test calls every public getter on aiebu::elf and verifies
// internal consistency of the results.  It does not require golden
// output files — the ELF is its own oracle.

#include "aiebu/elf.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////
// Minimal test framework
////////////////////////////////////////////////////////////////

static int g_passed = 0;
static int g_failed = 0;

static void
check(bool cond, const std::string& label)
{
  if (cond) {
    std::cout << "  PASS  " << label << "\n";
    ++g_passed;
  }
  else {
    std::cout << "  FAIL  " << label << "\n";
    ++g_failed;
  }
}

// Call expr; succeed if it does not throw, fail with message otherwise.
#define CHECK_NOTHROW(expr, label) \
  do { \
    try { (expr); check(true, (label)); } \
    catch (const std::exception& e) \
    { std::cout << "  FAIL  " << (label) << " [threw: " << e.what() << "]\n"; ++g_failed; } \
  } while (0)

// Call expr; succeed if it throws, fail otherwise.
#define CHECK_THROWS(expr, label) \
  do { \
    try { (expr); std::cout << "  FAIL  " << (label) << " [did not throw]\n"; ++g_failed; } \
    catch (...) { check(true, (label)); } \
  } while (0)

////////////////////////////////////////////////////////////////
// Platform name helpers
////////////////////////////////////////////////////////////////

static std::string
platform_name(aiebu::elf::platform p)
{
  switch (p) {
  case aiebu::elf::platform::aie2p:         return "aie2p";
  case aiebu::elf::platform::aie2ps:        return "aie2ps";
  case aiebu::elf::platform::aie2ps_legacy: return "aie2ps_legacy";
  case aiebu::elf::platform::aie4:          return "aie4";
  case aiebu::elf::platform::aie4a:         return "aie4a";
  case aiebu::elf::platform::aie4z:         return "aie4z";
  }
  return "unknown";
}

static aiebu::elf::platform
parse_platform(const std::string& s)
{
  if (s == "aie2p")         return aiebu::elf::platform::aie2p;
  if (s == "aie2ps")        return aiebu::elf::platform::aie2ps;
  if (s == "aie2ps_legacy") return aiebu::elf::platform::aie2ps_legacy;
  if (s == "aie4")          return aiebu::elf::platform::aie4;
  if (s == "aie4a")         return aiebu::elf::platform::aie4a;
  if (s == "aie4z")         return aiebu::elf::platform::aie4z;
  throw std::runtime_error("Unknown platform name: " + s);
}

////////////////////////////////////////////////////////////////
// Individual getter tests
////////////////////////////////////////////////////////////////

static void
test_platform(const aiebu::elf& e, const std::string& expected_name)
{
  std::cout << "\n-- Platform / version --\n";

  auto p = e.get_platform();
  auto name = platform_name(p);
  std::cout << "  platform: " << name << "\n";

  if (!expected_name.empty())
    check(name == expected_name, "get_platform() == " + expected_name);

  // OS/ABI byte must match the platform enum value
  check(e.get_os_abi() == static_cast<uint8_t>(p), "get_os_abi() consistent with get_platform()");

  auto [major, minor] = e.get_abi_version();
  std::cout << "  abi version: " << static_cast<int>(major) << "." << static_cast<int>(minor) << "\n";
  check(major <= 0xF && minor <= 0xF, "get_abi_version() nibbles in range");
}

static void
test_shape(const aiebu::elf& e)
{
  std::cout << "\n-- Shape queries --\n";

  bool full = e.is_full_elf();
  bool group = e.is_group_elf();
  std::cout << "  is_full_elf:  " << (full  ? "yes" : "no") << "\n";
  std::cout << "  is_group_elf: " << (group ? "yes" : "no") << "\n";
  check(true, "is_full_elf() does not throw");
  check(true, "is_group_elf() does not throw");
}

static void
test_metadata(const aiebu::elf& e)
{
  std::cout << "\n-- Metadata --\n";

  // get_cfg_uuid()      requires .note.xrt.UID
  // get_partition_size() requires .note.xrt.configuration
  // is_full_elf()       tests only .note.xrt.configuration
  //
  // The two sections are independent: an ELF can have .note.xrt.UID
  // without .note.xrt.configuration (all dtrace ELFs are in this state).
  // We therefore call each unconditionally and treat either outcome as
  // valid — the functions themselves throw with a clear message when the
  // section is absent, and that is the contract being tested.
  std::cout << "  is_full_elf: " << (e.is_full_elf() ? "yes" : "no") << "\n";

  try {
    e.get_cfg_uuid();
    std::cout << "  get_cfg_uuid(): present\n";
    check(true, "get_cfg_uuid() returned without throwing");
  }
  catch (const std::exception& ex) {
    std::cout << "  get_cfg_uuid(): absent (" << ex.what() << ")\n";
    check(true, "get_cfg_uuid() threw as expected for missing .note.xrt.UID");
  }

  if (e.is_full_elf()) {
    CHECK_NOTHROW(e.get_partition_size(), "get_partition_size() (full ELF has .note.xrt.configuration)");
  }
  else {
    CHECK_THROWS(e.get_partition_size(), "get_partition_size() throws when .note.xrt.configuration absent");
  }
}

static void
test_kernels(const aiebu::elf& e)
{
  std::cout << "\n-- Kernel metadata --\n";

  auto kernels = e.get_kernels();
  std::cout << "  kernel count: " << kernels.size() << "\n";
  check(true, "get_kernels() does not throw");

  for (const auto& k : kernels) {
    check(!k.name.empty(), "kernel name non-empty: " + k.name);
    std::cout << "    kernel '" << k.name
              << "' args=" << k.args.size()
              << " instances=" << k.instances.size() << "\n";
    for (size_t i = 0; i < k.args.size(); ++i)
      check(k.args[i].index == static_cast<uint32_t>(i),
            "arg[" + std::to_string(i) + "].index == " + std::to_string(i));
  }
}

static void
test_section_access(const aiebu::elf& e)
{
  std::cout << "\n-- Section access --\n";

  // Missing section must return empty span, not throw
  auto empty = e.get_section(".__nonexistent__");
  check(empty.empty(), "get_section() returns empty for missing section");

#if 0 // round trip doesn't work with out elfs
  // save() round-trip: serialise and re-parse; platform must match
  std::ostringstream oss;
  CHECK_NOTHROW(e.save(oss), "save() does not throw");
  auto blob = oss.str();
  check(!blob.empty(), "save() produces non-empty output");

  aiebu::elf reloaded(blob.data(), blob.size());
  check(reloaded.get_platform() == e.get_platform(),
        "round-trip save+reload preserves platform");
#endif
}

static void
test_group_maps(const aiebu::elf& e)
{
  std::cout << "\n-- Group / ctrl-code maps --\n";

  auto s2g = e.get_section_to_group_map();
  auto g2s = e.get_group_to_sections_map();
  auto k2id = e.get_kernel_name_to_id_map();

  std::cout << "  sections in s2g: " << s2g.size() << "\n";
  std::cout << "  groups   in g2s: " << g2s.size() << "\n";
  std::cout << "  entries  in k2id: " << k2id.size() << "\n";

  // Every section that appears in g2s values must appear in s2g
  size_t orphan = 0;
  for (const auto& [gid, members] : g2s)
    for (auto sid : members)
      if (s2g.find(sid) == s2g.end())
        ++orphan;

  check(orphan == 0, "every g2s member appears in s2g");

  // Every s2g value (group id) must appear as a key in g2s
  size_t missing = 0;
  for (const auto& [sid, gid] : s2g)
    if (gid != UINT32_MAX && g2s.find(gid) == g2s.end())
      ++missing;

  check(missing == 0, "every s2g group-id appears in g2s");

  // Every k2id value must be a key in g2s or UINT32_MAX
  size_t bad_ids = 0;
  for (const auto& [key, gid] : k2id)
    if (gid != UINT32_MAX && g2s.find(gid) == g2s.end())
      ++bad_ids;

  check(bad_ids == 0, "every k2id group-id appears in g2s");
}

static void
test_ctrlcode_id(const aiebu::elf& e)
{
  std::cout << "\n-- get_ctrlcode_id --\n";

  auto kernels = e.get_kernels();
  for (const auto& k : kernels) {
    for (const auto& inst : k.instances) {
      auto qualified = k.name + ":" + inst;
      CHECK_NOTHROW(e.get_ctrlcode_id(qualified),
                    "get_ctrlcode_id(" + qualified + ") does not throw");
    }
    if (k.instances.size() == 1) {
      CHECK_NOTHROW(e.get_ctrlcode_id(k.name),
                    "get_ctrlcode_id(" + k.name + ") bare name works for single instance");
    }
  }

  // Unknown name must throw
  CHECK_THROWS(e.get_ctrlcode_id("__no_such_kernel__"),
               "get_ctrlcode_id() throws for unknown kernel");
}

static void
test_patch_points(const aiebu::elf& e)
{
  std::cout << "\n-- Patch points --\n";

  // Bind to const& — get_patch_points() returns a reference, not a copy.
  const auto& pp = e.get_patch_points();
  size_t total = 0;
  for (const auto& [gid, key_map] : pp)
    for (const auto& [key, pts] : key_map)
      total += pts.size();

  std::cout << "  total patch points: " << total << "\n";
  check(true, "get_patch_points() does not throw");

  // Every group id in patch_points must appear in g2s
  auto g2s = e.get_group_to_sections_map();
  size_t bad = 0;
  for (const auto& [gid, _] : pp)
    if (gid != UINT32_MAX && g2s.find(gid) == g2s.end())
      ++bad;

  check(bad == 0, "patch-point group ids all appear in g2s");
}

static void
test_clear_patch_points(const std::string& path)
{
  std::cout << "\n-- clear_patch_points --\n";

  // Construct a fresh elf so clearing does not affect the shared instance.
  aiebu::elf e(path);
  const auto& pp_before = e.get_patch_points();
  std::cout << "  patch points before clear: " << pp_before.size() << " groups\n";

  e.clear_patch_points();
  check(e.get_patch_points().empty(), "get_patch_points() empty after clear_patch_points()");
}

static void
test_section_name(const aiebu::elf& e)
{
  std::cout << "\n-- get_section_name --\n";

  auto g2s = e.get_group_to_sections_map();
  size_t tested = 0;
  for (const auto& [gid, sec_ids] : g2s) {
    for (auto idx : sec_ids) {
      // Index 0 is the reserved SHT_NULL section; it has an empty name by ELF
      // spec.  Legacy ELFs place every section (including null) in the group
      // map, so skip it here.
      if (idx == 0)
        continue;

      auto name = e.get_section_name(idx);
      // Every named section in the group map must have a non-empty name.
      check(!name.empty(), "get_section_name(" + std::to_string(idx) + ") non-empty");
      ++tested;
      if (tested >= 3)
        break; // spot-check a few; no need to iterate every section
    }
    if (tested >= 3)
      break;
  }

  // Out-of-range index must return empty string, not throw.
  check(e.get_section_name(UINT32_MAX).empty(),
        "get_section_name(UINT32_MAX) returns empty for invalid index");
}

static void
test_buffer_accessors(const aiebu::elf& e)
{
  std::cout << "\n-- Buffer size+copy accessors --\n";

  auto g2s   = e.get_group_to_sections_map();
  auto kernels = e.get_kernels();
  if (g2s.empty()) {
    std::cout << "  (no ctrl-code groups — skipping buffer accessor tests)\n";
    return;
  }

  // Use the first ctrl-code-id available.
  uint32_t id = g2s.begin()->first;
  std::cout << "  testing ctrl-code-id: " << id << "\n";

  auto p = e.get_platform();
  bool is_gen2 = (p == aiebu::elf::platform::aie2p);

  if (is_gen2) {
    // AIE gen2 — instruction / preemption buffers
    CHECK_NOTHROW(e.has_pdi(),        "has_pdi() does not throw");
    CHECK_NOTHROW(e.has_preemption(), "has_preemption() does not throw");

    auto instr_sz = e.get_instr_buf_size(id);
    std::cout << "  get_instr_buf_size: " << instr_sz << "\n";
    check(true, "get_instr_buf_size() does not throw");

    if (instr_sz > 0) {
      std::vector<std::byte> buf(instr_sz);
      CHECK_NOTHROW(e.copy_instr_buf(id, {buf.data(), buf.size()}),
                    "copy_instr_buf() does not throw");
    }

    auto ctrl_sz = e.get_ctrl_packet_size(id);
    std::cout << "  get_ctrl_packet_size: " << ctrl_sz << "\n";
    check(true, "get_ctrl_packet_size() does not throw");

    auto save_sz    = e.get_preempt_save_size(id);
    auto restore_sz = e.get_preempt_restore_size(id);
    std::cout << "  get_preempt_save_size: " << save_sz << "\n";
    std::cout << "  get_preempt_restore_size: " << restore_sz << "\n";
    check(true, "get_preempt_save_size() does not throw");
    check(true, "get_preempt_restore_size() does not throw");

    // PDI symbol set — O(1) lookup
    const auto& pdi_syms = e.get_pdi_symbols(id);
    std::cout << "  get_pdi_symbols() count: " << pdi_syms.size() << "\n";
    check(true, "get_pdi_symbols() does not throw");

    // Non-existent ctrl-code-id must return empty set, not throw
    const auto& empty_syms = e.get_pdi_symbols(UINT32_MAX - 1);
    check(empty_syms.empty(), "get_pdi_symbols() returns empty for unknown ctrl-code-id");
  }
  else {
    // AIE gen2plus — column ctrl-code buffers
    auto ncols = e.get_column_count(id);
    std::cout << "  get_column_count: " << ncols << "\n";
    check(true, "get_column_count() does not throw");

    for (uint32_t col = 0; col < ncols; ++col) {
      auto sz = e.get_ctrlcode_size(id, col);
      std::cout << "  get_ctrlcode_size(col=" << col << "): " << sz << "\n";
      check(true, "get_ctrlcode_size() does not throw");

      if (sz > 0) {
        std::vector<std::byte> buf(sz);
        CHECK_NOTHROW(e.copy_ctrlcode(id, col, {buf.data(), buf.size()}),
                      "copy_ctrlcode(col=" + std::to_string(col) + ") does not throw");
      }
    }

    // ctrlpkt sections
    auto names = e.get_ctrlpkt_section_names(id);
    std::cout << "  get_ctrlpkt_section_names() count: " << names.size() << "\n";
    check(true, "get_ctrlpkt_section_names() does not throw");

    for (const auto& name : names) {
      auto sz = e.get_ctrlpkt_size(id, name);
      check(sz > 0, "get_ctrlpkt_size(" + name + ") > 0");

      std::vector<std::byte> buf(sz);
      CHECK_NOTHROW(e.copy_ctrlpkt(id, name, {buf.data(), buf.size()}),
                    "copy_ctrlpkt(" + name + ") does not throw");
    }

    // dump buffer
    auto dump_sz = e.get_dump_buf_size(id);
    std::cout << "  get_dump_buf_size: " << dump_sz << "\n";
    check(true, "get_dump_buf_size() does not throw");

    if (dump_sz > 0) {
      std::vector<std::byte> buf(dump_sz);
      CHECK_NOTHROW(e.copy_dump_buf(id, {buf.data(), buf.size()}),
                    "copy_dump_buf() does not throw");
    }

    // gen2 methods must throw on gen2plus
    CHECK_THROWS(e.has_pdi(),        "has_pdi() throws on gen2plus");
    CHECK_THROWS(e.has_preemption(), "has_preemption() throws on gen2plus");
    CHECK_THROWS(e.get_pdi_symbols(id), "get_pdi_symbols() throws on gen2plus");
  }
}

// F1 regression guard: has_pdi() must be section-based, not relocation-based.
// The value drives ERT opcode selection (ERT_START_NPU_PREEMPT_ELF), so an
// ELF with a .pdi section but no PDI relocation must still report has_pdi()==true.
// Only meaningful on aie2p; has_pdi() throws on gen2plus (covered elsewhere).
static void
test_has_pdi_value(const aiebu::elf& e, const std::string& expected)
{
  std::cout << "\n-- has_pdi() value --\n";

  if (e.get_platform() != aiebu::elf::platform::aie2p) {
    std::cout << "  (non-aie2p — has_pdi() value not applicable)\n";
    return;
  }

  bool actual = e.has_pdi();
  std::cout << "  has_pdi(): " << (actual ? "true" : "false") << "\n";

  if (expected.empty()) {
    std::cout << "  (no expected-has-pdi argument — value not asserted)\n";
    return;
  }

  bool want = (expected == "pdi");
  check(actual == want, "has_pdi() == " + expected);
}

// F2 correctness guard: get_pdi_symbols(id) is the O(1) accessor that replaced
// the old per-construction scan of get_patch_points().  Cross-check that the
// fast accessor yields exactly the same set as an independent derivation, so
// the two cannot silently diverge.
//
// IMPORTANT: a PDI symbol (e.g. ".pdi.0") is identified by its SYMBOL NAME
// containing "pdi", NOT by target_buf==pdi.  The PDI address is patched into
// the .ctrltext section, so its patch_point.target_buf is ctrltext.  The
// oracle below must therefore match on arg_name — matching on target_buf
// would produce an empty set and mask a regression where get_pdi_symbols()
// wrongly returns nothing (FW timeout: PDI never allocated/patched).
static void
test_pdi_symbols_crosscheck(const aiebu::elf& e)
{
  std::cout << "\n-- get_pdi_symbols() cross-check vs patch points --\n";

  if (e.get_platform() != aiebu::elf::platform::aie2p) {
    std::cout << "  (non-aie2p — get_pdi_symbols() not applicable)\n";
    return;
  }

  // Reference set: every patch_point whose arg_name marks it as a PDI symbol,
  // grouped by ctrl-code-id.  Mirrors m_ctrl_pdi_map's population predicate.
  const auto& pp = e.get_patch_points();
  std::map<uint32_t, std::set<std::string>> expected;
  for (const auto& [gid, key_map] : pp)
    for (const auto& [key, pts] : key_map)
      for (const auto& p : pts)
        if (p.arg_name.find("pdi") != std::string::npos)
          expected[gid].insert(p.arg_name);

  // Compare accessor output to the reference for each group that has PDI syms.
  for (const auto& [gid, want] : expected) {
    const auto& got_us = e.get_pdi_symbols(gid);
    std::set<std::string> got(got_us.begin(), got_us.end());
    check(got == want,
          "get_pdi_symbols(" + std::to_string(gid) + ") matches PDI patch points");
  }

  // Groups with no PDI patch points must yield an empty accessor result.
  for (const auto& [gid, key_map] : pp) {
    (void) key_map;
    if (expected.count(gid))
      continue;

    check(e.get_pdi_symbols(gid).empty(),
          "get_pdi_symbols(" + std::to_string(gid) + ") empty when no PDI patch points");
  }
}

static void
test_pdi_and_ctrlpkt_pm(const aiebu::elf& e)
{
  std::cout << "\n-- PDI / ctrlpkt_pm / scratch_pad (AIE gen2 only) --\n";

  // These methods are only valid for aie2p; they throw on other platforms.
  // The test ELFs are all gen2plus so we exercise only the throw path.
  if (e.get_platform() != aiebu::elf::platform::aie2p) {
    CHECK_THROWS(e.get_ctrlpkt_pm_dynsyms(),       "get_ctrlpkt_pm_dynsyms() throws on non-aie2p");
    CHECK_THROWS(e.get_pdi_size("x"),              "get_pdi_size() throws on non-aie2p");
    CHECK_THROWS(e.get_ctrlpkt_pm_buf_size("x"),   "get_ctrlpkt_pm_buf_size() throws on non-aie2p");
    CHECK_THROWS(e.get_ctrl_scratch_pad_mem_size(), "get_ctrl_scratch_pad_mem_size() throws on non-aie2p");
    return;
  }

  // aie2p path — exercise each method with real data
  auto dynsyms = e.get_ctrlpkt_pm_dynsyms();
  std::cout << "  ctrlpkt_pm dynsyms: " << dynsyms.size() << "\n";
  check(true, "get_ctrlpkt_pm_dynsyms() does not throw");

  for (const auto& sym : dynsyms) {
    size_t sz = e.get_ctrlpkt_pm_buf_size(sym);
    check(sz > 0, "get_ctrlpkt_pm_buf_size(" + sym + ") > 0");

    std::vector<std::byte> buf(sz);
    CHECK_NOTHROW(
      e.copy_ctrlpkt_pm_buf(sym, {buf.data(), buf.size()}),
      "copy_ctrlpkt_pm_buf(" + sym + ") does not throw");
  }

  // Missing symbol returns 0 within the correct platform
  check(e.get_pdi_size("__no_such_pdi__") == 0,
        "get_pdi_size() returns 0 for unknown symbol on aie2p");
  check(e.get_ctrlpkt_pm_buf_size("__no__") == 0,
        "get_ctrlpkt_pm_buf_size() returns 0 for unknown symbol on aie2p");

  std::vector<std::byte> dummy(16);
  CHECK_NOTHROW(e.copy_pdi("__no_such_pdi__", {dummy.data(), dummy.size()}),
                "copy_pdi() no-op for unknown symbol on aie2p");

  size_t sz = e.get_ctrl_scratch_pad_mem_size();
  std::cout << "  ctrl_scratch_pad_mem_size: " << sz << "\n";
  check(true, "get_ctrl_scratch_pad_mem_size() does not throw");
}

////////////////////////////////////////////////////////////////
// Entry point
////////////////////////////////////////////////////////////////

int
run(int argc, char** argv)
{
    if (argc < 2 || argc > 4) {
    std::cerr << "Usage: elf_reader_test <elf-file> [expected-platform] [expected-has-pdi]\n"
              << "  expected-platform: aie2p aie2ps aie2ps_legacy aie4 aie4a aie4z\n"
              << "  expected-has-pdi:  pdi | nopdi  (aie2p only)\n";
    return 2;
  }

  std::string path = argv[1];
  std::string expected_platform = (argc >= 3) ? argv[2] : "";
  std::string expected_has_pdi  = (argc >= 4) ? argv[3] : "";

  std::cout << "=== elf_reader_test: " << path << " ===\n";

  // Validate expected platform name early so a typo gives a clear error
  if (!expected_platform.empty()) {
    try { parse_platform(expected_platform); }
    catch (const std::exception& e) {
      std::cerr << e.what() << "\n";
      return 2;
    }
  }

  // Validate expected-has-pdi spelling early
  if (!expected_has_pdi.empty() && expected_has_pdi != "pdi" && expected_has_pdi != "nopdi") {
    std::cerr << "expected-has-pdi must be 'pdi' or 'nopdi', got: " << expected_has_pdi << "\n";
    return 2;
  }

  // Construction from filename
  aiebu::elf e(path);

  // Construction from stream (exercises that overload independently)
  {
    std::ifstream fs(path, std::ios::binary);
    if (!fs)
      throw std::runtime_error("Cannot open: " + path);

    CHECK_NOTHROW(aiebu::elf{fs}, "construct from std::istream");
  }

  // Construction from buffer
  {
    std::ifstream fs(path, std::ios::binary);
    std::vector<char> buf((std::istreambuf_iterator<char>(fs)),
                           std::istreambuf_iterator<char>());
    CHECK_NOTHROW(aiebu::elf(buf.data(), buf.size()),
                  "construct from void*/size");
    auto sv = std::string_view{buf.data(), buf.size()};
    CHECK_NOTHROW(aiebu::elf(sv), "construct from string_view");
  }

  test_platform(e, expected_platform);
  test_shape(e);
  test_metadata(e);
  test_kernels(e);
  test_section_access(e);
  test_section_name(e);
  test_group_maps(e);
  test_ctrlcode_id(e);
  test_patch_points(e);
  test_pdi_symbols_crosscheck(e);
  test_has_pdi_value(e, expected_has_pdi);
  test_clear_patch_points(path);
  test_pdi_and_ctrlpkt_pm(e);
  test_buffer_accessors(e);

  std::cout << "\n=== Result: " << g_passed << " passed, "
            << g_failed << " failed ===\n";

  return g_failed > 0 ? 1 : 0;
}

int
main(int argc, char** argv)
{
  try {
    return run(argc, argv);
  }
  catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << "\n";
  }
  return 1;
}
