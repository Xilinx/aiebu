// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_ASM_ASM_PARSER_H_
#define _AIEBU_PREPROCESSOR_ASM_ASM_PARSER_H_

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <regex>
#include <map>
#include <unordered_map>
#include "utils.h"
#include "code_section.h"

namespace aiebu {

inline std::string trim(const std::string& line)
{
    // trim the last
    std::string WhiteSpace = " \t\f\v\n\r";
    std::size_t start = line.find_first_not_of(WhiteSpace);
    std::size_t end = line.find_last_not_of(WhiteSpace);
    return start == end ? std::string() : line.substr(start, end - start + 1);
}

enum class operation_type: uint8_t
{
  label = 1,
  op = 2,
};

// Start defining regexs required for parsing aie2asm

const std::string HEX_RE("[[:space:]]*(0[xX][[:xdigit:]]+)[[:space:]]*");
const std::string L_BRACK_RE("[[:space:]]*\\([[:space:]]*");
const std::string R_BRACK_RE("[[:space:]]*\\)[[:space:]]*");

class operation
{
  std::string m_name;
  std::vector<std::string> m_args;
public:

  operation(const std::string& name, std::string sargs): m_name(name)
  {
    std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);
    std::transform(sargs.begin(), sargs.end(), sargs.begin(), ::tolower);

    std::string s;
    std::stringstream ss(sargs);
    while (std::getline(ss, s, ' ')) {
        m_args.emplace_back(s.substr(0, s.find_last_not_of(",")+1));
    }
  }

  const std::string& get_name() const { return m_name; }
  const std::vector<std::string>& get_args() const { return m_args; }
};

class asm_parser;
class directive
{
public:
  std::shared_ptr<asm_parser> m_parserptr;
public:
  directive() {}
  virtual void operate(std::shared_ptr<asm_parser> parserptr, const std::smatch& sm) = 0;
};

class attach_to_group_directive: public directive
{
public:
  attach_to_group_directive() {}
  void operate(std::shared_ptr<asm_parser> parserptr, const std::smatch& sm);
};

class include_directive: public directive
{

  bool read_include_file(std::string filename);
  bool isAbsolutePath(const std::string& path) {
    // On Unix-like systems, an absolute path starts with '/'
    if (path.empty()) {
      return false;
    }
    if (path[0] == '/') {
      return true;
    }

    // On Windows, an absolute path can start with a drive letter followed by ':'
    // and a backslash or forward slash, e.g., "C:\\" or "C:/"
    if (path.size() > 1 && path[1] == ':' && (path[2] == '\\' || path[2] == '/')) {
      return true;
    }
    return false;
  }
public:
  include_directive() {}
  void operate(std::shared_ptr<asm_parser> parserptr, const std::smatch& sm);
};

class end_of_label_directive: public directive
{
public:
  end_of_label_directive() {}
  void operate(std::shared_ptr<asm_parser> parserptr, const std::smatch& sm);
};

class pad_directive: public directive
{
  bool read_pad_file(std::string& name, std::string& filename);

  bool isAbsolutePath(const std::string& path) {
    // On Unix-like systems, an absolute path starts with '/'
    if (path.empty()) {
      return false;
    }
    if (path[0] == '/') {
      return true;
    }

    // On Windows, an absolute path can start with a drive letter followed by ':'
    // and a backslash or forward slash, e.g., "C:\\" or "C:/"
    if (path.size() > 1 && path[1] == ':' && (path[2] == '\\' || path[2] == '/')) {
      return true;
    }
    return false;
  }
public:
  offset_type convert2int(std::string& str)
  {
    offset_type size = 0;
    try {
      size_t pos;
      size = std::stoul(str, &pos, 0);
      if (pos != str.length()) {
        throw error(error::error_code::internal_error,"Invalid characters found in the scratchpad size");
      }
    } catch (const std::invalid_argument& e) {
      throw error(error::error_code::internal_error, "Invalid argument in scratchpad size");
    } catch (const std::out_of_range& e) {
      throw error(error::error_code::internal_error, "Out of range scratchpad size");
    }
    return size;
  }
  void add_scratchpad(std::string& name, std::string& str);
  pad_directive() {}
  void operate(std::shared_ptr<asm_parser> parserptr, const std::smatch& sm);
};

class section_directive: public directive
{
  bool is_test_section(const std::string& str) {return !str.substr(0,9).compare(".ctrltext"); }
  bool is_data_section(const std::string& str) {return !str.substr(0,9).compare(".ctrldata"); }
public:
  section_directive() {}
  void operate(std::shared_ptr<asm_parser> parserptr, const std::smatch& sm);
};

class asm_data
{
  std::shared_ptr<operation> m_op;
  operation_type m_optype;
  code_section m_section;
  uint64_t m_size;
  pageid_type m_pagenum;
  uint32_t m_linenumber;
  std::string m_line;
  std::string m_file;

public:
  asm_data() = default;
  asm_data(std::shared_ptr<operation> op, operation_type optype,
           code_section sec, uint64_t size, uint32_t pgnum,
           uint32_t ln, std::string line, std::string file)
           :m_op(op), m_optype(optype), m_section(sec), m_size(size),
            m_pagenum(pgnum), m_linenumber(ln), m_line(line), m_file(file) {}

  asm_data( asm_data* a)
  {
    a->m_op = m_op;
    a->m_optype = m_optype;
    a->m_section = m_section;
    a->m_size = m_size;
    a->m_pagenum = m_pagenum;
    a->m_linenumber = m_linenumber;
    a->m_line = m_line;
    a->m_file = m_file;
  }

  HEADER_ACCESS_GET_SET(code_section, section);
  HEADER_ACCESS_GET_SET(uint64_t, size);
  HEADER_ACCESS_GET_SET(pageid_type, pagenum);
  HEADER_ACCESS_GET_SET(uint32_t, linenumber);
  HEADER_ACCESS_GET_SET(std::string, file);
  bool isLabel() { return m_optype == operation_type::label; }
  bool isOpcode() { return m_optype == operation_type::op; }
  std::shared_ptr<operation> get_operation() { return m_op; }

};

class section_asmdata
{
public:
  std::vector<std::shared_ptr<asm_data>> text;
  std::vector<std::shared_ptr<asm_data>> data;
};

class scratchpad_info
{
  std::string m_name;
  offset_type m_size;
  offset_type m_offset;
  offset_type m_base;
  std::vector<char> m_content;
public:
  scratchpad_info(std::string name, offset_type size, offset_type offset, offset_type base, std::vector<char>& content):
    m_name(name), m_size(size), m_offset(offset), m_base(base), m_content(std::move(content)) {}

  scratchpad_info(const scratchpad_info& other) : m_name(other.m_name),
    m_size(other.m_size), m_offset(other.m_offset), m_base(other.m_base),
    m_content(other.m_content) { }

  HEADER_ACCESS_GET_SET(std::string, name);
  HEADER_ACCESS_GET_SET(offset_type, size);
  HEADER_ACCESS_GET_SET(offset_type, offset);
  HEADER_ACCESS_GET_SET(offset_type, base);

  const std::vector<char>& get_content() const { return m_content; }
};

class col_data
{
  std::map<std::string, section_asmdata> m_label_data;
  std::map<std::string, uint32_t> m_labelpageindex;
  std::map<std::string, std::shared_ptr<scratchpad_info>> m_scratchpads;
public:

  std::vector<std::shared_ptr<asm_data>> get_label_asmdata(const std::string& label)
  {
    std::vector<std::shared_ptr<asm_data>> result(m_label_data[label].text);
    result.insert(result.end(), m_label_data[label].data.begin(), m_label_data[label].data.end());
    return result;
  }

  std::map<std::string, section_asmdata>& get_label_data() { return m_label_data; }

  std::map<std::string, uint32_t>& get_labelpageindex() { return m_labelpageindex; }

  void set_labelpageindex(std::string& label, uint32_t val) { m_labelpageindex[label] = val; }

  std::map<std::string, std::shared_ptr<scratchpad_info>>& get_scratchpads() { return m_scratchpads; }
  void set_scratchpad(std::string& name, offset_type size, std::vector<char>& content)
  {
    m_scratchpads[name] = std::make_shared<scratchpad_info>(name, size, 0 , 0, content);
  }
};

class attach_to_group_directive;
class include_directive;
class end_of_label_directive;
class pad_directive;
class section_directive;

class asm_parser: public std::enable_shared_from_this<asm_parser>
{
  std::unordered_map<uint32_t, col_data> m_col;
  const std::vector<char> &m_data;
  std::map<std::string, std::shared_ptr<directive>> directive_list;
  std::stack<bool> isdatastack;
  std::string m_current_label = "default";
  int m_current_col = -1;
  const std::vector<std::string>& m_include_list;

public:
  asm_parser(const std::vector<char>& data, const std::vector<std::string>& include_list):m_data(data), m_include_list(include_list)
  {
    set_data_state(false);
    m_current_col = -1;
  }

  void set_data_state(bool state) { isdatastack.push(state); }

  void pop_data_state() { isdatastack.pop();}

  bool get_data_state() const { return isdatastack.top();}

  const std::vector<std::string>& get_include_list() const { return m_include_list; }

  std::string get_current_label() const { return m_current_label; }

  std::string top_label() const
  {
    std::vector<std::string> labels = splitoption(m_current_label.c_str(), ':');
    if (labels.size() == 0)
      throw error(error::error_code::internal_error, "invalid current label:" + m_current_label);
    return labels[labels.size() - 1];
  }

  void pop_label()
  {
    size_t pos = m_current_label.rfind(':');
    if (pos != std::string::npos) {
        m_current_label =  m_current_label.substr(0, pos);
    }
  }

  void set_current_label(std::string& label) { m_current_label = label; }

  void insert_col_asmdata(std::shared_ptr<asm_data> data);

  std::vector<uint32_t> get_col_list();

  col_data& get_col_asmdata(uint32_t colnum);

  void parse_lines();

  void parse_lines(const std::vector<char>& data, std::string& file);

  void set_current_col(int col) { m_current_col = col;
    m_col[m_current_col] = col_data();
  }

  std::map<std::string, uint32_t>& getcollabelpageindex(int col) { return m_col[col].get_labelpageindex(); }

  std::vector<std::string> getLabelsforcol(uint32_t col)
  {
    std::vector<std::string> keys;
    for (const auto& pair : m_col[col].get_label_data()) {
        keys.push_back(pair.first);
    }
    return keys;
  }

  void insert_scratchpad(std::string& name, offset_type size, std::vector<char>& content);

  std::map<std::string, std::shared_ptr<scratchpad_info>>& getcolscratchpad(int col) { return m_col[col].get_scratchpads(); }

  bool operate_directive(std::string& line)
  {
    std::smatch sm;
    std::regex DIRCETIVE_REGEX("^([.a-zA-Z0-9_]+)(?:\\s+(.+)+)?$");
    std::regex_match(line, sm, DIRCETIVE_REGEX);
    if (sm.size() == 0)
      return false;

    if (directive_list.count(sm[1].str()) == 0)
      return false;

    directive_list[sm[1].str()]->operate(shared_from_this(), sm);
    return true;
  }
};



}
#endif //_AIEBU_PREPROCESSOR_ASM_ASM_PARSER_H_
