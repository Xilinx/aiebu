// SPDX-License-Identifier: MIT
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

  if (m_col.find(m_current_col) == m_col.end())
    m_col[m_current_col] = col_data();

  auto& label_data = m_col[m_current_col].get_label_data();
  if (label_data.find(m_current_label) == label_data.end())
    label_data[m_current_label] = section_asmdata();

  if (get_data_state())
    label_data[m_current_label].data.emplace_back(data);
  else
    label_data[m_current_label].text.emplace_back(data);

  auto pagelabel = get_pagelabel(m_current_label);
  m_col[m_current_col].set_labelpageindex(pagelabel, 0);
}

void
asm_parser::
insert_scratchpad(std::string& name, offset_type size, std::vector<char>& content)
{
  if (m_current_col == -1)
    m_current_col = 0;

  m_col[m_current_col].set_scratchpad(name, size, content);
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
    [](const std::unordered_map<uint32_t, col_data>::value_type &pair){return pair.first;});
  return keys;
}

col_data&
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
parse_lines()
{
  directive_list[".attach_to_group"] = std::make_shared<attach_to_group_directive>();
  directive_list[".include"] = std::make_shared<include_directive>();
  directive_list[".endl"] = std::make_shared<end_of_label_directive>();
  directive_list[".setpad"] = std::make_shared<pad_directive>();
  directive_list[".section"] = std::make_shared<section_directive>();
  std::string file = "default";
  parse_lines(m_data, file);
}

void
asm_parser::
parse_lines(const std::vector<char>& data, std::string& file)
{
  //parse asm code
  const std::regex COMMENT_REGEX("^;(.*)$");
  const std::regex LABEL_REGEX("^([a-zA-Z0-9_]+)\\:$");
  const std::regex OP_REGEX("^([.a-zA-Z0-9_]+)(?:\\s+(.+)+)?$");
  const std::regex DIRCETIVE_REGEX(".^([a-zA-Z0-9_]+)(?:\\s+(.+)+)?$");

  std::string str(data.begin(), data.end());
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

    // Check for Directive
    std::regex_match(line, sm, DIRCETIVE_REGEX);
    if (operate_directive(line))
    {
      ++linenumber;

//      std::regex_match(line, sm, OP_REGEX);
//      m_current_col = std::stoi(sm[2].str());
      // Initialize empty m_col, it helps when .attach_to_group is immediately followed
      // by another .attach_to_group. This use case is relevant for aie2asm flow where
      // this directive is used to determine column range for a ctrlcode
//      m_col[m_current_col] = std::vector<std::shared_ptr<asm_data>>();
      continue;
    }

    // check for label
    std::regex_match(line, sm, LABEL_REGEX);
    if (sm.size())
    {
      if (!get_data_state())
        m_current_label = m_current_label + ":" + sm[1].str();
      else
        insert_col_asmdata(std::make_shared<asm_data>(std::make_shared<operation>(sm[1].str(), ""),
                                                      operation_type::label, code_section::unknown, 0,
                                                      (uint32_t)-1, linenumber, line, file));
    }
    // check for operation
    std::regex_match(line, sm, OP_REGEX);
    if (sm.size())
    {
      insert_col_asmdata(std::make_shared<asm_data>(std::make_shared<operation>(sm[1].str(), sm[2].str()),
                                                    operation_type::op, code_section::unknown, 0, (uint32_t)-1,
                                                    linenumber, line, file));
      if (!sm[1].str().compare("EOF"))
        set_data_state(true);
    }
    ++linenumber;
  }

}

void
attach_to_group_directive::
operate(std::shared_ptr<asm_parser> parserptr, const std::smatch& sm)
{
  m_parserptr = parserptr;
  if (sm.size() < 3)
    throw error(error::error_code::invalid_asm, "Invalid attach_to_group directive argument\n");

  // dummy eof added if col change happens before eof
  m_parserptr->insert_col_asmdata(std::make_shared<asm_data>(std::make_shared<operation>("eof", ""),
                                                    operation_type::op, code_section::unknown, 0, (uint32_t)-1,
                                                    0, "eof", "default"));
  m_parserptr->set_current_col(std::stoi(sm[2].str()));
  m_parserptr->set_data_state(false);
}

void
section_directive::
operate(std::shared_ptr<asm_parser> parserptr, const std::smatch& sm)
{
  m_parserptr = parserptr;
  std::vector<std::string> args = splitoption(sm[2].str().c_str(), ',');
  if (is_test_section(args[0]))
    m_parserptr->set_data_state(false);
  else if (is_data_section(args[0]))
    m_parserptr->set_data_state(true);
  else
    std::cout << "section directive with unknown section found:" << args[0] << std::endl;
}

bool
include_directive::
read_include_file(std::string filename)
{
  std::ifstream file(filename);
  if (!file.is_open()) {
    return false;
  }
  std::cout << "Reading file:" << filename << std::endl;
  std::string line;
  m_parserptr->set_data_state(false);

  auto data = std::vector<char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  m_parserptr->parse_lines(data, filename);
  file.close();
  m_parserptr->pop_data_state();
  return true;
}

void
include_directive::
operate(std::shared_ptr<asm_parser> parserptr, const std::smatch& sm)
{
  m_parserptr = parserptr;
  //std::vector<std::string> args = splitoption(sm[2].str().c_str(), ',');
  std::string file = sm[2].str();//args[0];
  if (file.size() >= 2 && file.front() == '"' && file.back() == '"')
    file =  file.substr(1, file.size() - 2);

  if (isAbsolutePath(file))
  {
    if (!read_include_file(file))
      throw error(error::error_code::internal_error, "File " + file + " not exist\n");
    return;
  }

  for (auto& path : m_parserptr->get_include_list())
  {
    std::string fullpath = path + "/" + file;
    if (read_include_file(fullpath))
      return;
  }

  throw error(error::error_code::internal_error, "File " + file + " not exist\n");
}

void
end_of_label_directive::
operate(std::shared_ptr<asm_parser> parserptr, const std::smatch& sm)
{
  m_parserptr = parserptr;

  std::string label = m_parserptr->top_label();
  m_parserptr->pop_label();

  std::vector<std::string> args = splitoption(sm[2].str().c_str(), ',');
  if (label.compare(args[0]))
    throw error(error::error_code::internal_error, "endl label missmatch (" + label + " != " + args[0] + ")\n");
}

void
pad_directive::
operate(std::shared_ptr<asm_parser> parserptr, const std::smatch& sm)
{
  m_parserptr = parserptr;
  std::vector<std::string> args = splitoption(sm[2].str().c_str(), ',');

  add_scratchpad(args[0], args[1]);
}

void
pad_directive::
add_scratchpad(std::string& name, std::string& str) {
  // Check if the string is an integer
  str = trim(str);
  if (std::all_of(str.begin(), str.end(), ::isdigit)) {
    std::vector<char> empty_vector;
    m_parserptr->insert_scratchpad(name, convert2int(str) * WORD_SIZE, empty_vector);
    return;
  }
  // Check if the string is a hexadecimal number
  std::regex hex_regex("0[xX][0-9a-fA-F]+");
  if (std::regex_match(str, hex_regex)) {
    std::vector<char> empty_vector;
    m_parserptr->insert_scratchpad(name, convert2int(str) * WORD_SIZE, empty_vector);
    return;
  }

  std::string file = str;
  if (file.front() == '"' && file.back() == '"')
    file = str.substr(1, str.size() - 2);

  if (isAbsolutePath(file))
  {
    if (!read_pad_file(name, file))
      throw error(error::error_code::internal_error, "File " + file + " not exist\n");
    return;
  }

  for (auto& path : m_parserptr->get_include_list())
  {
    std::string fullpath = path + "/" + file;
    if (read_pad_file(name, fullpath))
      return;
  }

  throw error(error::error_code::internal_error, "File " + file + " not exist\n");

}

bool
pad_directive::
read_pad_file(std::string& name, std::string& filename)
{
  std::ifstream file(filename, std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    return false;
  }

  std::cout << "Reading file:" << filename << std::endl;
  std::string line;
  m_parserptr->set_data_state(false);

  auto data = std::vector<char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  m_parserptr->insert_scratchpad(name, data.size(), data);
  file.close();
  return true;
}
}
