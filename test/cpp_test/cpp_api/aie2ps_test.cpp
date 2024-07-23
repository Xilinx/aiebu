// SPDX-License-Identifier: MIT
// Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>
#include "aiebu_assembler.h"
#include "aiebu_error.h"
#include <algorithm>

void usage_exit()
{
  std::cout << "Usage: aie2ps_cpp.out <control_code.asm>" << std::endl;
  exit(1);
}

int main(int argc, char ** argv)
{
  if (argc != 2)
    usage_exit();

  std::ifstream input( argv[1], std::ios::binary );
  std::vector<char> control_code_buf;
  std::copy(std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>( ),
            std::back_inserter(control_code_buf));

  try
  {
    auto as = aiebu::aiebu_assembler(aiebu::aiebu_assembler::buffer_type::asm_aie2ps, control_code_buf);
    auto e = as.get_elf();
    std::cout << "elf size:" << e.size() << "\n";
    std::ofstream output_file("out.elf");
    std::ostream_iterator<char> output_iterator(output_file);
    std::copy(e.begin(), e.end(), output_iterator);
  }
  catch (aiebu::error &ex)
  {
    std::cout << "ERROR: " <<  ex.what() << ex.get_code() << std::endl;
  }


  return 0;
}
