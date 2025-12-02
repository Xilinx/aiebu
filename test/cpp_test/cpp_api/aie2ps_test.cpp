// SPDX-License-Identifier: MIT
// Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

#include "aiebu/aiebu_assembler.h"
#include "aiebu/aiebu_error.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

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
    auto as = aiebu::aiebu_assembler(aiebu::aiebu_assembler::buffer_type::asm_aie2ps, control_code_buf, {"disabledump"}, paths, external_buffer_id);
    auto e = as.get_elf();
    std::ofstream output_file(testcase+".elf", std::ios_base::binary);
    output_file.write(e.data(), e.size());
    output_file.close();
    auto tbl = as.get_argtbl();

    auto& table = tbl.dump();
    table[1].xrt_idx = 45;
    table[1].bd_offset = 100;

    table[5].xrt_idx = 45;
    table[9].xrt_idx = 45;
    table[12].xrt_idx = 45;
    table[16].xrt_idx = 45;
    table[19].xrt_idx = 45;
    table[23].xrt_idx = 45;
    table[26].xrt_idx = 45;
    table[30].xrt_idx = 45;
    table[33].xrt_idx = 45;
    table[37].xrt_idx = 45;
    table[41].xrt_idx = 45;
    table[44].xrt_idx = 45;
    table[48].xrt_idx = 45;
    table[51].xrt_idx = 45;
    table[54].xrt_idx = 45;
    table[57].xrt_idx = 45;
    table[60].xrt_idx = 45;
    table[64].xrt_idx = 45;
    table[67].xrt_idx = 45;
    table[70].xrt_idx = 45;
    table[74].xrt_idx = 45;
    table[77].xrt_idx = 45;
    table[80].xrt_idx = 45;
    table[84].xrt_idx = 45;
    table[87].xrt_idx = 45;
    table[90].xrt_idx = 45;
    table[94].xrt_idx = 45;

    as.flush_argtbl(tbl);
    auto e_modified = as.get_elf();
    std::ofstream output_file_modified(testcase+"_modified.elf", std::ios_base::binary);
    output_file_modified.write(e_modified.data(), e_modified.size());
    output_file_modified.close();
  }
  catch (aiebu::error &ex)
  {
    std::cout << "ERROR: " <<  ex.what() << ex.get_code() << std::endl;
    return 1;
  }


  return 0;
}
