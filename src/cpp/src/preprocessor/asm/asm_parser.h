// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_ASM_ASM_PARSER_H_
#define _AIEBU_PREPROCESSOR_ASM_ASM_PARSER_H_

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <regex>
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

class operation
{
  std::string m_name;
  std::vector<std::string> m_args;
public:

  operation(std::string name, std::string sargs): m_name(name)
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
  const std::vector<std::string>& get_args() { return m_args; }
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

public:
  asm_data(std::shared_ptr<operation> op, operation_type optype,
           code_section sec, uint64_t size, uint32_t pgnum,
           uint32_t ln, std::string line)
           :m_op(op), m_optype(optype), m_section(sec), m_size(size),
            m_pagenum(pgnum), m_linenumber(), m_line(line) {}

  asm_data( asm_data* a)
  {
    a->m_op = m_op;
    a->m_optype = m_optype;
    a->m_section = m_section;
    a->m_size = m_size;
    a->m_pagenum = m_pagenum;
    a->m_linenumber = m_linenumber;
    a->m_line = m_line;
  }

  HEADER_ACCESS_GET_SET(code_section, section);
  HEADER_ACCESS_GET_SET(uint64_t, size);
  HEADER_ACCESS_GET_SET(pageid_type, pagenum);
  HEADER_ACCESS_GET_SET(uint32_t, linenumber);
  bool isLabel() { return m_optype == operation_type::label; }
  bool isOpcode() { return m_optype == operation_type::op; }
  std::shared_ptr<operation> get_operation() { return m_op; }
  
};

class asm_parser
{
  int m_current_col = -1;
  std::unordered_map<uint32_t, std::vector<std::shared_ptr<asm_data>>> m_col;
  const std::vector<char> &m_data;

public:
  asm_parser(const std::vector<char>& data):m_data(data) {  parse_line();}

  void insert_col_asmdata(std::shared_ptr<asm_data> data);

  std::vector<uint32_t> get_col_list();

  std::vector<std::shared_ptr<asm_data>>& get_col_asmdata(uint32_t colnum);

  void parse_line();

};

}
#endif //_AIEBU_PREPROCESSOR_ASM_ASM_PARSER_H_
