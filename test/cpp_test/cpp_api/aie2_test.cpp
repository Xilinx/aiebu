// SPDX-License-Identifier: MIT
// Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>
#include "aiebu_assembler.h"
#include <algorithm>

using namespace std;

int main(int argc, char ** argv)
{

  vector<char> v1;
  vector<char> v2;
  std::vector<aiebu::patch_info> patch_data;

  std::ifstream input( argv[1], std::ios::binary );
  std::copy(std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>( ),
            std::back_inserter(v1));

  if (argc > 2) {
  std::ifstream input( argv[2], std::ios::binary );
  std::copy(std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>( ),
            std::back_inserter(v2));
  }

  aiebu::aiebu_assembler as(aiebu::aiebu_assembler::buffer_type::blob_instr_dpu, v1, v2, patch_data);
  auto e = as.get_elf();
  cout << "elf size:" << e.size() << "\n";

  std::ofstream output_file("out.elf");
  std::ostream_iterator<char> output_iterator(output_file);
  std::copy(e.begin(), e.end(), output_iterator);
  return 0;
}
