// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.
#include "asm_parser.h"

#include "aiebu/aiebu_error.h"

#include <iostream>
#include <fstream>

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
insert_annotation(int annotation_index)
{
  // insert annotation_index in col list
  if (m_current_col == -1)
    throw error(error::error_code::internal_error, "Invalid while adding annotation "
                + std::to_string(m_current_col) + "!!!");

  if (m_col.find(m_current_col) == m_col.end())
    throw error(error::error_code::internal_error, "Col data not find while adding annotation for col"
                + std::to_string(m_current_col) + "!!!");

  auto& label_data = m_col[m_current_col].get_label_data();

  for (auto it = label_data[m_current_label].text.rbegin(); it != label_data[m_current_label].text.rend(); ++it)
  {
    if ((*it)->get_operation()->get_name().compare(".eop"))
    {
      (*it)->set_annotation_index(annotation_index);
      break;
    }
  }
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
  directive_list[".partition"] = std::make_shared<partition_directive>();
  std::string file = "default";
  parse_lines(m_data, file);
}

void
asm_parser::
parse_lines(const std::vector<char>& data, std::string& file)
{
  //parse asm code
  const static regex COMMENT_REGEX("^;(.*)$");
  const static regex LABEL_REGEX("^([a-zA-Z0-9_]+):$");
  const static regex OP_REGEX("^([.a-zA-Z0-9_]+)(?:[ \\t]+(.+))?$");
  const static regex DIRCETIVE_REGEX(".^([a-zA-Z0-9_]+)(?:[ \\t]+(.+))?$");

  std::string str(data.begin(), data.end());
  std::istringstream isstr(str);
  std::string line;
  uint32_t linenumber = 0;

  while (std::getline(isstr, line)) {
    ++linenumber;
    line = trim(line);
    if(line.empty())
      continue;

    if (line[0] == ';') //regex_match(line, COMMENT_REGEX))
      continue;

    smatch sm;

    // Check for Directive (starts with .) - only check regex if prefix matches
    if (line[0] == '.' && operate_directive(line))
    {
      if (!get_annotation_state())
        continue;
      std::string aline, id_line, name_line, description_line;
      // there are 3 lines for annotation data (id,name,description)
      for (int count = 0 ; count < 3 ; ++count)   // NOLINT
      {
        std::getline(isstr, aline);
        aline = trim(aline);
        if (!aline.substr(0,3).compare("id:"))                 // NOLINT
          id_line = trim(aline.substr(aline.find(":") + 1));   // NOLINT
        else if (!aline.substr(0,5).compare("name:"))          // NOLINT
          name_line = trim(aline.substr(aline.find(":") + 1)); // NOLINT
        else if (!aline.substr(0,12).compare("description:"))  // NOLINT
          description_line = trim(aline.substr(aline.find(":") + 1)); // NOLINT
        else
          throw error(error::error_code::internal_error, "Unknown annotation field " + aline + "!!!");
      }
      m_annotation_list.emplace_back(id_line, name_line, description_line);
      insert_annotation(m_annotation_list.size() - 1);
      reset_annotation_state();
      linenumber+=3;
      continue;
    }

    // check for label - fast check for ':' at end before regex
    if (line.back() == ':' && regex_match(line, sm, LABEL_REGEX))
    {
      if (!get_data_state())
        m_current_label = m_current_label + ":" + sm[1].str();
      else
        insert_col_asmdata(std::make_shared<asm_data>(std::make_shared<operation>(sm[1].str(), ""),
                                                      operation_type::label, code_section::unknown, 0,
                                                      (uint32_t)-1, linenumber, line, file));
      continue;
    }
    // check for operation
    else if (regex_match(line, sm, OP_REGEX))
    {
      std::string arg_str = (sm.size() > 2 && sm[2].matched) ? sm[2].str() : "";
      insert_col_asmdata(std::make_shared<asm_data>(std::make_shared<operation>(sm[1].str(), arg_str),
                                                    operation_type::op, code_section::unknown, 0, (uint32_t)-1,
                                                    linenumber, line, file));
      std::string op_name = sm[1].str();
      std::transform(op_name.begin(), op_name.end(), op_name.begin(), ::tolower);
      if (!op_name.compare("eof"))
        set_data_state(true);
    }
  }

}

void
attach_to_group_directive::
operate(std::shared_ptr<asm_parser> parserptr, const smatch& sm)
{
  m_parserptr = parserptr;
  verify_match(sm, error::error_code::invalid_asm, "Invalid attach_to_group directive argument\n");

  // dummy eof added if col change happens before eof
  m_parserptr->insert_col_asmdata(std::make_shared<asm_data>(std::make_shared<operation>("eof", ""),
                                                    operation_type::op, code_section::unknown, 0, (uint32_t)-1,
                                                    0, "eof", "default"));
  m_parserptr->set_current_col(std::stoi(sm[2].str()));
  m_parserptr->set_data_state(false);
}

void
section_directive::
operate(std::shared_ptr<asm_parser> parserptr, const smatch& sm)
{
  m_parserptr = parserptr;
  verify_match(sm, error::error_code::invalid_asm, ".section directive requires arguments\n");

  std::vector<std::string> args = splitoption(sm[2].str().c_str(), ',');
  if (is_test_section(args[0]))
    m_parserptr->set_data_state(false);
  else if (is_data_section(args[0]))
    m_parserptr->set_data_state(true);
  else if (is_annotation_section(args[0]))
    m_parserptr->set_annotation_state();
  else
    std::cout << "section directive with unknown section found:" << args[0] << std::endl;
}

void
partition_directive::
operate(std::shared_ptr<asm_parser> parserptr, const smatch& sm)
{
  m_parserptr = parserptr;
  static const regex pattern(R"(\.partition\s+(\d+)(column|core:(\d+)mem))");
  smatch match;
//  LOG_INFO("PARTITION:" << sm[0].str());
  std::cout << "PARTITION:" << sm[0].str() << std::endl;
  std::string line = sm[0].str();
  if (regex_match(line, match, pattern)) {
    if (match[2] == "column") {
      std::cout << "Column count: " << match[1] << std::endl;
      m_parserptr->set_numcolumn(to_uinteger<uint32_t>(match[1]));
    } else {
      m_parserptr->set_numcore(to_uinteger<uint32_t>(match[1]));
      m_parserptr->set_nummem(to_uinteger<uint32_t>(match[3]));
      std::cout << "Core count: " << match[1] << std::endl;
      std::cout << "Memory size: " << match[3] << std::endl;
    }
  } else
    throw error(error::error_code::invalid_asm, "Invalid format!! " + line + "\n");
}

bool
include_directive::
read_include_file(std::string filename)
{
  std::ifstream file(filename);
  if (!file.is_open()) {
    return false;
  }
//  std::cout << "Reading file:" << filename << std::endl;
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
operate(std::shared_ptr<asm_parser> parserptr, const smatch& sm)
{
  m_parserptr = parserptr;
  //std::vector<std::string> args = splitoption(sm[2].str().c_str(), ',');
  std::string file = sm[2].str();//args[0];
  if (file.size() >= 2 && file.front() == '"' && file.back() == '"')
    file =  file.substr(1, file.size() - 2);

  if (is_absolute_path(file))
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
operate(std::shared_ptr<asm_parser> parserptr, const smatch& sm)
{
  m_parserptr = parserptr;

  std::string label = m_parserptr->top_label();
  m_parserptr->pop_label();

  verify_match(sm, error::error_code::invalid_asm, ".endl directive requires a label argument\n");

  std::vector<std::string> args = splitoption(sm[2].str().c_str(), ',');
  if (label.compare(args[0]))
    throw error(error::error_code::internal_error, "endl label missmatch (" + label + " != " + args[0] + ")\n");
}

void
pad_directive::
operate(std::shared_ptr<asm_parser> parserptr, const smatch& sm)
{
  m_parserptr = parserptr;
  verify_match(sm, error::error_code::invalid_asm, ".setpad directive requires arguments\n");

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
  static const regex hex_regex("0[xX][0-9a-fA-F]+");
  if (regex_match(str, hex_regex)) {
    std::vector<char> empty_vector;
    m_parserptr->insert_scratchpad(name, convert2int(str) * WORD_SIZE, empty_vector);
    return;
  }

  std::string file = str;
  if (file.front() == '"' && file.back() == '"')
    file = str.substr(1, str.size() - 2);

  if (is_absolute_path(file))
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

//  std::cout << "Reading file:" << filename << std::endl;
  std::string line;
  m_parserptr->set_data_state(false);

  auto data = std::vector<char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  m_parserptr->insert_scratchpad(name, data.size(), data);
  file.close();
  return true;
}
}
