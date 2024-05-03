// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include "asm/asm_parser.h"
#include "aiebu_error.h"

namespace aiebu {

void
asm_parser::
insert_col_asmdata(std::shared_ptr<asm_data> data)
{
  // insert asm_data in col list
  if (m_current_col == -1)
    m_current_col = 0;
  auto it = m_col.find(m_current_col);
  if (it == m_col.end())
    m_col[m_current_col] = std::vector<std::shared_ptr<asm_data>>();
  m_col[m_current_col].emplace_back(data);
}

std::vector<uint32_t>
asm_parser::
get_col_list()
{
  // get col list
  std::vector<uint32_t> keys;

  std::transform(
    m_col.begin(),
    m_col.end(),
    std::back_inserter(keys),
    [](const std::unordered_map<int,std::vector<std::shared_ptr<asm_data>>>::value_type &pair){return pair.first;});
  return keys;
}

std::vector<std::shared_ptr<asm_data>>&
asm_parser::
get_col_asmdata(uint32_t colnum)
{
  // get list of asm data for perticular col
  auto it = m_col.find(colnum);
  if (it != m_col.end()) {
    return m_col[colnum];
  } else
    throw error(error::error_code::internal_error, "Key " + std::to_string(colnum)  + " not found!!!");
}

void
asm_parser::
parse_line()
{
  //parse asm code
  std::regex COMMENT_REGEX("^;(.*)$");
  std::regex LABEL_REGEX("^([a-zA-Z0-9_]+)\\:$");
  std::regex OP_REGEX("^([.a-zA-Z0-9_]+)(?:\\s+(.+)+)?$");

  std::string str(m_data.begin(), m_data.end());
  std::istringstream isstr(str);
  std::string line;
  uint32_t linenumber = 0;
  while (std::getline(isstr, line)) {
    line = trim(line);
    if(line.empty())
      continue;

    if (std::regex_match(line, COMMENT_REGEX))
      continue;

    std::smatch sm;

    // handle attach_to_group directive for col number
    if (line.rfind(".attach_to_group") == 0)
    {
      std::regex_match(line, sm, OP_REGEX);
      m_current_col = std::stoi(sm[2].str());
      continue;
    }

    // check for label
    std::regex_match(line, sm, LABEL_REGEX);
    if(sm.size())
      insert_col_asmdata(std::make_shared<asm_data>(std::make_shared<operation>(sm[1].str(), ""),
                                                    operation_type::label, code_section::unknown, 0,
                                                    (uint32_t)-1, linenumber, line));
    // check for operation
    std::regex_match(line, sm, OP_REGEX);
    if (sm.size())
      insert_col_asmdata(std::make_shared<asm_data>(std::make_shared<operation>(sm[1].str(), sm[2].str()),
                                                    operation_type::op, code_section::unknown, 0, (uint32_t)-1,
                                                    linenumber, line));

    ++linenumber;
  }
}

}
