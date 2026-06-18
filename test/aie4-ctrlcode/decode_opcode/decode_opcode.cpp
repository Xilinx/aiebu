// SPDX-License-Identifier: MIT
// Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.
//
// Test for aiebu::get_opcode_information() on an ELF without a .dump section
// (ISA binary walk / no-dump path).
//
// Usage:
//   decode_opcode <out_no_dump.elf> <input.json>
//
// input.json (Boost.JSON) supports multiple test vectors in one file:
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

#include <boost/json.hpp>

#include <elfio/elfio.hpp>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <string_view>
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

std::string read_text_file(const std::filesystem::path& path)
{
  if (!std::filesystem::exists(path))
    throw std::runtime_error("file not found: " + path.string());
  std::ifstream in(path);
  std::ostringstream ss;
  ss << in.rdbuf();
  return ss.str();
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

void require_key(const boost::json::object& o, std::string_view key)
{
  if (!o.contains(key))
    throw std::runtime_error(std::string("case object missing required key: ") + std::string(key));
}

std::string get_string(const boost::json::object& o, std::string_view key)
{
  require_key(o, key);
  const boost::json::value& v = o.at(key);
  if (!v.is_string())
    throw std::runtime_error(std::string("expected string for key: ") + std::string(key));
  return std::string(v.as_string());
}

uint32_t get_u32(const boost::json::object& o, std::string_view key)
{
  require_key(o, key);
  const boost::json::value& v = o.at(key);
  if (v.is_uint64()) {
    const uint64_t n = v.as_uint64();
    if (n > std::numeric_limits<uint32_t>::max())
      throw std::runtime_error(std::string("value too large for uint32: ") + std::string(key));
    return static_cast<uint32_t>(n);
  }
  if (v.is_int64()) {
    const int64_t n = v.as_int64();
    if (n < 0 || n > static_cast<int64_t>(std::numeric_limits<uint32_t>::max()))
      throw std::runtime_error(std::string("integer out of range for uint32: ") + std::string(key));
    return static_cast<uint32_t>(n);
  }
  throw std::runtime_error(std::string("expected integer for key: ") + std::string(key));
}

decode_opcode_case parse_case_object(const boost::json::object& o)
{
  decode_opcode_case c;
  if (o.contains("label") && o.at("label").is_string())
    c.label = std::string(o.at("label").as_string());
  c.kernel_name              = get_string(o, "kernel_name");
  c.uc_idx                   = get_u32(o, "uc_idx");
  c.page_idx                 = get_u32(o, "page_idx");
  c.offset                   = get_u32(o, "offset");
  c.expected_opcode_no_dump  = get_string(o, "expected_opcode_no_dump");
  c.expected_args_no_dump    = get_string(o, "expected_args_no_dump");
  return c;
}

std::vector<decode_opcode_case> parse_input_json(std::string_view text)
{
  boost::system::error_code ec;
  const boost::json::value root = boost::json::parse(text, ec);
  if (ec)
    throw std::runtime_error("JSON parse error: " + ec.message());

  std::vector<boost::json::object> raw_cases;

  if (root.is_array()) {
    const boost::json::array& arr = root.as_array();
    raw_cases.reserve(arr.size());
    for (const boost::json::value& el : arr) {
      if (!el.is_object())
        throw std::runtime_error("each element of root array must be a JSON object");
      raw_cases.push_back(el.as_object());
    }
  } else if (root.is_object()) {
    const boost::json::object& o = root.as_object();
    if (o.contains("cases")) {
      const boost::json::value& c = o.at("cases");
      if (!c.is_array())
        throw std::runtime_error("\"cases\" must be a JSON array");
      const boost::json::array& arr = c.as_array();
      raw_cases.reserve(arr.size());
      for (const boost::json::value& el : arr) {
        if (!el.is_object())
          throw std::runtime_error("each element of \"cases\" must be a JSON object");
        raw_cases.push_back(el.as_object());
      }
    } else {
      raw_cases.push_back(o);
    }
  } else {
    throw std::runtime_error("JSON root must be an object or an array");
  }

  if (raw_cases.empty())
    throw std::runtime_error("no test cases found (empty \"cases\" or array)");

  std::vector<decode_opcode_case> out;
  out.reserve(raw_cases.size());
  for (const boost::json::object& obj : raw_cases)
    out.push_back(parse_case_object(obj));
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
    const std::vector<decode_opcode_case> cases = parse_input_json(read_text_file(json_path));

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
