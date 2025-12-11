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
 * without ctrl_pkt: ./aie2_full_elf_cpp tinyyolo_preempt test/cpp_test/aie2/tinyyolo_preempt
 * with ctrl_pkt   : ./aie2_full_elf_cpp tinyyolo_preempt_cp test/cpp_test/aie2/tinyyolo_preempt_cp
 */

void usage_exit()
{
  std::cout << "Usage: aie2_full_elf_cpp.out <testcase> <testcase path>" << std::endl;
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
  if (argc != 3){
    std::cout<<"TESTING argc "<<argc<<std::endl;
    for( int i =0; i< argc; i++)
      std::cout<<"TESTING argv "<<argv[i]<<std::endl;
    usage_exit();
  }
  std::string testcase = argv[1];
  std::string testcase_path = argv[2];

  if ((!testcase.compare("tinyyolo_preempt")) || (!testcase.compare("tinyyolo_preempt_cp")))
  // in memory assembling
  {
    aiebu::file_artifact resolver;
    std::vector<char> control_code;
    aiebu_ReadFile(testcase_path+"/ml_txn_lp.bin", control_code);
    resolver.add_vfile("ml_txn_lp.bin", std::move(control_code));
    std::vector<char> config_json;
    aiebu_ReadFile(testcase_path+"/config.json", config_json);
    std::vector<char> pdi_file;
    aiebu_ReadFile(testcase_path+"/00000000-0000-0000-0000-000000001111.pdi", pdi_file);
    resolver.add_vfile("00000000-0000-0000-0000-000000001111.pdi", std::move(pdi_file));
    if (!testcase.compare("tinyyolo_preempt_cp")) {
      std::vector<char> externel_json;
      aiebu_ReadFile(testcase_path+"/external_buffer_id.json", externel_json);
      resolver.add_vfile("external_buffer_id.json", externel_json);
      std::vector<char> ctrlpkt;
      aiebu_ReadFile(testcase_path+"/ctrl_pkt0.bin", ctrlpkt);
      resolver.add_vfile("ctrl_pkt0.bin", ctrlpkt);
    }

    try
    {
      auto as = aiebu::aiebu_assembler(aiebu::aiebu_assembler::buffer_type::aie2_config, config_json, resolver, {});

      auto e = as.get_elf();
      std::ofstream output_file(testcase+"_in_mem.elf", std::ios_base::binary);
      output_file.write(e.data(), e.size());
      output_file.close();
    }
    catch (aiebu::error &ex)
    {
      std::cout << "ERROR: " <<  ex.what() << ex.get_code() << std::endl;
      return 1;
    }
  }
  return 0;
}
