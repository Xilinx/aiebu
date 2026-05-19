// SPDX-License-Identifier: MIT
// Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.
//
// Negative tests for duplicate ASM file name detection.
//
// Test 1 – duplicate_vfile:
//   Calling file_artifact::add_vfile() twice with the same name must throw
//   aiebu::error with error_code::invalid_input.
//
// Test 2 – duplicate_include:
//   An ASM file that contains two consecutive .include directives for the
//   same filename must cause aiebu_assembler to throw aiebu::error with
//   error_code::invalid_input.

#include "aiebu/aiebu_assembler.h"
#include "aiebu/aiebu_error.h"

#include <iostream>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// helpers
// ---------------------------------------------------------------------------

static std::vector<char> to_buf(const std::string& s)
{
  return std::vector<char>(s.begin(), s.end());
}

// Expected error_code value for invalid_input (aiebu_invalid_input == 7).
static constexpr int EXPECTED_CODE = aiebu_invalid_input;

struct result {
  bool passed = false;
  std::string message;
};

// ---------------------------------------------------------------------------
// Test 1: duplicate add_vfile
// ---------------------------------------------------------------------------
static result test_duplicate_vfile()
{
  aiebu::file_artifact fa;
  const auto buf = to_buf("START_JOB 0\n  NOP\nEND_JOB\nEOF\n");

  try {
    fa.add_vfile("kernel.asm", buf);  // first registration – must succeed
    fa.add_vfile("kernel.asm", buf);  // duplicate – must throw
    return {false, "no exception thrown for duplicate add_vfile"};
  } catch (const aiebu::error& ex) {
    if (ex.get_code() != EXPECTED_CODE)
      return {false, "wrong error code " + std::to_string(ex.get_code())
                     + " (expected " + std::to_string(EXPECTED_CODE) + ")"};
    const std::string msg = ex.what();
    if (msg.find("duplicate asm file name") == std::string::npos)
      return {false, "unexpected error message: " + msg};
    std::cout << "expected error message: " << msg << std::endl;
    std::cout << "expected error code: " << EXPECTED_CODE << std::endl;
    return {true, ""};
  } catch (const std::exception& ex) {
    return {false, std::string("unexpected exception type: ") + ex.what()};
  }
}

// ---------------------------------------------------------------------------
// Test 2: duplicate .include inside a single compile
//
// Layout (all in-memory via file_artifact):
//
//   config.json   – minimal aie4_config descriptor pointing to main.asm
//   main.asm      – includes helper.asm twice; second .include must throw
//   helper.asm    – minimal valid content (comment only)
// ---------------------------------------------------------------------------
static result test_duplicate_include()
{
  // Minimal config.json: one kernel "DPU", one instance whose ctrl_code
  // is main.asm.
  static const std::string CONFIG_JSON = R"({
  "xrt-kernels": [
    {
      "name": "DPU",
      "arguments": [],
      "instance": [
        {
          "id": "dpu",
          "ctrl_code_file": "main.asm"
        }
      ]
    }
  ]
})";

  // main.asm: deliberately includes the same file twice.
  static const std::string MAIN_ASM =
    "; main control code\n"
    ".include \"helper.asm\"\n"
    ".include \"helper.asm\"\n";

  // helper.asm: valid but trivially empty (comment only).
  static const std::string HELPER_ASM = "; helper\n";

  aiebu::file_artifact resolver;
  resolver.add_vfile("main.asm",   to_buf(MAIN_ASM));
  resolver.add_vfile("helper.asm", to_buf(HELPER_ASM));

  const auto config_buf = to_buf(CONFIG_JSON);

  try {
    aiebu::aiebu_assembler as(
        aiebu::aiebu_assembler::buffer_type::aie4_config,
        config_buf,
        resolver,
        {});   // flags: none
    // If we reach here, assembly succeeded without throwing – test fails.
    return {false, "no exception thrown for duplicate .include"};
  } catch (const aiebu::error& ex) {
    if (ex.get_code() != EXPECTED_CODE)
      return {false, "wrong error code " + std::to_string(ex.get_code())
                     + " (expected " + std::to_string(EXPECTED_CODE) + ")"};
    const std::string msg = ex.what();
    if (msg.find("duplicate asm file name") == std::string::npos)
      return {false, "unexpected error message: " + msg};
    std::cout << "expected error message: " << msg << std::endl;
    std::cout << "expected error code: " << EXPECTED_CODE << std::endl;
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
  const tc tests[] = {
    {"duplicate_vfile",   test_duplicate_vfile},
    {"duplicate_include", test_duplicate_include},
  };

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
