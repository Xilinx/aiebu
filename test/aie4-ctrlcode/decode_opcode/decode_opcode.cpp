// SPDX-License-Identifier: MIT
// Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.
//
// Test for aiebu::get_opcode_information() on an ELF without a .dump section
// (ISA binary walk / no-dump path).
//
// Usage:
//   decode_opcode <out_no_dump.elf> <input.json>
//
// input.json (Boost.PropertyTree JSON parser) supports multiple test vectors:
//
//   { "cases": [ { ... }, { ... } ] }
//
// Each case object may include an optional "label" for log output.
// Required per case: kernel_name, uc_idx, page_idx, offset,
// expected_opcode_no_dump, expected_args_no_dump.
//
// For a single vector you may instead use a bare object with those keys
// (no "cases" wrapper), or a JSON array of case objects at the root.
//
// Pass "none" for expected_args_no_dump to skip the args_str check.

#include "aiebu/aiebu_debug.h"
#include "aiebu/aiebu_error.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <elfio/elfio.hpp>

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <utility>
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

  if (!info.found) {
    if (expected_opcode == "")
      return true;

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
  std::cerr << "Usage: decode_opcode <out_no_dump.elf> <input.json>\n"
               "  JSON: { \"cases\": [ { kernel_name, uc_idx, page_idx, offset,\n"
               "      expected_opcode_no_dump, expected_args_no_dump [, label] }, ... ] }\n"
               "  or a root array of case objects, or one case object alone.\n"
               "  (use \"none\" for expected_args_no_dump to skip args check)\n";
}

struct decode_opcode_case
{
  std::string label;
  std::string kernel_name;
  uint32_t    uc_idx{};
  uint32_t    page_idx{};
  uint32_t    offset{};
  std::string expected_opcode_no_dump;
  std::string expected_args_no_dump;
};

std::string get_string(const boost::property_tree::ptree& o, const char* key)
{
  const auto v = o.get_optional<std::string>(key);
  if (!v)
    throw std::runtime_error(std::string("case object missing or invalid string key: ") + key);
  return *v;
}

uint32_t get_u32(const boost::property_tree::ptree& o, const char* key)
{
  const auto v = o.get_optional<unsigned long>(key);
  if (!v)
    throw std::runtime_error(std::string("case object missing or invalid integer key: ") + key);
  if (*v > static_cast<unsigned long>(std::numeric_limits<uint32_t>::max()))
    throw std::runtime_error(std::string("value too large for uint32: ") + key);
  return static_cast<uint32_t>(*v);
}

decode_opcode_case parse_case_object(const boost::property_tree::ptree& o)
{
  decode_opcode_case c;
  if (const auto label = o.get_optional<std::string>("label"))
    c.label = *label;
  c.kernel_name              = get_string(o, "kernel_name");
  c.uc_idx                   = get_u32(o, "uc_idx");
  c.page_idx                 = get_u32(o, "page_idx");
  c.offset                   = get_u32(o, "offset");
  c.expected_opcode_no_dump  = get_string(o, "expected_opcode_no_dump");
  c.expected_args_no_dump    = get_string(o, "expected_args_no_dump");
  return c;
}

// Boost.PropertyTree read_json maps JSON array elements to child nodes whose
// keys are empty strings (see boost/property_tree/json_parser.hpp). Some trees
// may instead use "0", "1", ... — support both: empty keys preserve iteration
// order; numeric keys are sorted by index.
void append_array_children(const boost::property_tree::ptree& arr,
                           std::vector<boost::property_tree::ptree>& out)
{
  bool have_empty_key = false;
  bool have_numeric_key = false;
  for (const auto& kv : arr) {
    if (kv.first.empty())
      have_empty_key = true;
    else {
      char* end_ptr = nullptr;
      std::strtol(kv.first.c_str(), &end_ptr, 10);  // NOLINT: return value intentionally ignored
      if (end_ptr != kv.first.c_str() && *end_ptr == '\0')
        have_numeric_key = true;
    }
  }
  if (have_empty_key && have_numeric_key) {
    throw std::runtime_error("JSON array mixes empty and numeric child keys (unsupported)");
  }

  if (have_empty_key) {
    for (const auto& kv : arr) {
      if (kv.first.empty())
        out.push_back(kv.second);
    }
    return;
  }

  std::vector<std::pair<int, boost::property_tree::ptree>> tmp;
  tmp.reserve(arr.size());
  for (const auto& kv : arr) {
    char* end_ptr = nullptr;
    const long n = std::strtol(kv.first.c_str(), &end_ptr, 10);
    if (end_ptr == kv.first.c_str() || *end_ptr != '\0')
      throw std::runtime_error("expected JSON array with numeric element keys, got key: " + kv.first);
    tmp.emplace_back(static_cast<int>(n), kv.second);
  }
  std::sort(tmp.begin(), tmp.end(),
            [](const std::pair<int, boost::property_tree::ptree>& lhs,
               const std::pair<int, boost::property_tree::ptree>& rhs) { return lhs.first < rhs.first; });
  for (auto& e : tmp)
    out.push_back(std::move(e.second));
}

std::vector<decode_opcode_case> parse_input_json(const std::filesystem::path& json_path)
{
  if (!std::filesystem::exists(json_path))
    throw std::runtime_error("file not found: " + json_path.string());

  boost::property_tree::ptree root;
  std::ifstream in(json_path);
  if (!in)
    throw std::runtime_error("failed to open JSON file: " + json_path.string());
  boost::property_tree::read_json(in, root);

  std::vector<boost::property_tree::ptree> raw_cases;

  if (root.count("cases") != 0) {
    append_array_children(root.get_child("cases"), raw_cases);
  } else if (root.count("kernel_name") != 0) {
    raw_cases.push_back(root);
  } else {
    append_array_children(root, raw_cases);
  }

  if (raw_cases.empty())
    throw std::runtime_error("no test cases found (empty \"cases\" or array)");

  std::vector<decode_opcode_case> out;
  out.reserve(raw_cases.size());
  for (const boost::property_tree::ptree& node : raw_cases)
    out.push_back(parse_case_object(node));
  return out;
}

} // namespace

int main(int argc, char** argv)
{
  if (argc != 3) {
    usage();
    return 2;
  }

  const std::filesystem::path in_no_dump = argv[1];
  const std::filesystem::path json_path = argv[2];

  try {
    const std::vector<decode_opcode_case> cases = parse_input_json(json_path);

    std::vector<char> buf;
    read_file(in_no_dump, buf);

    const ELFIO::elfio reader_no_dump = load_elfio(buf);

    bool all_pass = true;
    for (std::size_t i = 0; i < cases.size(); ++i) {
      const decode_opcode_case& in = cases[i];
      const std::string label =
          !in.label.empty()
              ? in.label
              : ("ISA walk path [case " + std::to_string(i) + "]");
      all_pass &= check_opcode(reader_no_dump,
                               in.kernel_name,
                               in.uc_idx,
                               in.page_idx,
                               in.offset,
                               in.expected_opcode_no_dump,
                               in.expected_args_no_dump,
                               "none",
                               0,
                               label);
    }

    return all_pass ? 0 : 1;

  } catch (const aiebu::error& ex) {
    std::cerr << "aiebu::error: " << ex.what() << " (" << ex.get_code() << ")\n";
    return 1;
  } catch (const std::exception& ex) {
    std::cerr << "error: " << ex.what() << '\n';
    return 1;
  }
}
