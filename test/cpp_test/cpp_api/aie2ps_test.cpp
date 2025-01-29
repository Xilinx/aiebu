// SPDX-License-Identifier: MIT
// Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>
#include "aiebu_assembler.h"
#include "aiebu_error.h"
#include <algorithm>

/* 2 testcases added
 * without ctrl_pkt: ./aie2ps_cpp eff_net_coal test/cpp_test/aie2ps/eff_net_coal
 * with ctrl_pkt   : ./aie2ps_cpp eff_net_ctrlpacket test/cpp_test/aie2ps/eff_net_ctrlpacket
 */

void usage_exit()
{
  std::cout << "Usage: aie2ps_cpp.out <testcase> <testcase path>" << std::endl;
  std::cout << "testcase: " << std::endl;
  exit(1);
}

inline bool file_exists(const std::string& name) {
  return std::filesystem::exists(name);
}

inline void aiebu_ReadFile(const std::string& filename, std::vector<char>& buffer)
{
  if (!file_exists(filename))
    throw std::runtime_error("file:" + filename + " not found\n");
  std::ifstream input(filename, std::ios::in | std::ios::binary);
  auto file_size = std::filesystem::file_size(filename);
  buffer.resize(file_size);
  input.read(buffer.data(), file_size);
}

int main(int argc, char ** argv)
{
  if (argc != 3)
    usage_exit();

  std::string testcase = argv[1];
  std::string testcase_path = argv[2];

  std::vector<char> control_code_buf;
  aiebu_ReadFile(testcase_path+"/ml_asm/merged_control.asm", control_code_buf);

  std::vector<std::string> paths;
  paths.emplace_back(testcase_path+"/ml_asm/");
  paths.emplace_back(testcase_path+"/asm/");

  std::vector<char> external_buffer_id;
  // external_buffer_id and ctrl_pkt path needed only in ctrlpkt testcase
  if (!testcase.compare("eff_net_ctrlpacket"))
  {
    aiebu_ReadFile(testcase_path+"/external_buffer_id.json", external_buffer_id);
    paths.emplace_back(testcase_path+"/ctrl_pkt/");
  }

  try
  {
    auto as = aiebu::aiebu_assembler(aiebu::aiebu_assembler::buffer_type::asm_aie2ps, control_code_buf, {}, paths, external_buffer_id);
    auto e = as.get_elf();
    std::ofstream output_file(testcase+".elf", std::ios_base::binary);
    output_file.write(e.data(), e.size());
    output_file.close();
  }
  catch (aiebu::error &ex)
  {
    std::cout << "ERROR: " <<  ex.what() << ex.get_code() << std::endl;
    return 1;
  }


  return 0;
}
