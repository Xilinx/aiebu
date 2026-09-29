// SPDX-License-Identifier: MIT
// Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.

#include <algorithm>
#include "preprocessor/asm/asm_parser.h"

#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Keep only a weak reference outside each parser's scope. A directive must not
// retain its owning parser after dispatch, including when parsing throws.
static bool test_successful_parse()
{
  const std::string source = ".attach_to_group 0\nNOP\n";
  const std::vector<char> data(source.begin(), source.end());
  const std::vector<std::string> includes;
  std::weak_ptr<aiebu::asm_parser> observer;
  {
    auto parser = std::make_shared<aiebu::asm_parser>(data, includes, "aie4");
    observer = parser;
    parser->parse_lines("parser_lifetime.asm");

    bool found_nop = false;
    for (const auto& entry : parser->get_col_asmdata(0).get_label_asmdata("default")) {
      if (entry->get_operation().get_name() == "nop")
        found_nop = true;
    }
    if (!found_nop) {
      std::cerr << "FAIL: successful parse did not preserve the NOP operation\n";
      return false;
    }
  }
  if (!observer.expired()) {
    std::cerr << "FAIL: parser retained after successful parsing\n";
    return false;
  }
  return true;
}

static bool test_parse_error()
{
  // The handler stores its back-reference before rejecting the missing group.
  const std::string source = ".attach_to_group\n";
  const std::vector<char> data(source.begin(), source.end());
  const std::vector<std::string> includes;
  std::weak_ptr<aiebu::asm_parser> observer;
  bool caught_expected_error = false;
  try {
    auto parser = std::make_shared<aiebu::asm_parser>(data, includes, "aie4");
    observer = parser;
    parser->parse_lines("parser_lifetime_invalid.asm");
  } catch (const aiebu::error& ex) {
    if (ex.get_code() != aiebu_invalid_asm) {
      std::cerr << "FAIL: unexpected parser error: " << ex.what() << '\n';
      return false;
    }
    caught_expected_error = true;
  }
  if (!caught_expected_error) {
    std::cerr << "FAIL: missing group argument did not produce an error\n";
    return false;
  }
  if (!observer.expired()) {
    std::cerr << "FAIL: parser retained after exception unwinding\n";
    return false;
  }
  return true;
}

int main()
{
  try {
    const bool success_path = test_successful_parse();
    const bool error_path = test_parse_error();
    if (!success_path || !error_path)
      return 1;
    std::cout << "PASS: parser released after successful parsing and exceptions\n";
    return 0;
  } catch (const std::exception& ex) {
    std::cerr << "FAIL: unexpected exception: " << ex.what() << '\n';
    return 1;
  }
}
