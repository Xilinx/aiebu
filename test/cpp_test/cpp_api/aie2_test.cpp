// SPDX-License-Identifier: MIT
// Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>
#include "aiebu_assembler.h"
#include <algorithm>

void usage_exit()
{
  std::cout << "Usage: aie2_cpp.out <txn.bin> <control_packet.bin> <external_buffer_id.json>" << std::endl;
  exit(1);
}

int main(int argc, char ** argv)
{

  if (argc != 2 && argc != 4)
    usage_exit();

  std::vector<char> txn_buf;
  std::vector<char> control_packet_buf;
  std::vector<char> external_buffer_id_json_buf;

  // Reading txn buffer
  std::ifstream input(argv[1], std::ios::binary);
  std::copy(std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>( ),
            std::back_inserter(txn_buf));

  if (argc > 2) {
    // Reading control_pack and external_buffer_id_json
    std::ifstream input2(argv[2], std::ios::binary);
    std::copy(std::istreambuf_iterator<char>(input2),
              std::istreambuf_iterator<char>( ),
              std::back_inserter(control_packet_buf));

    std::ifstream input1(argv[3], std::ios::binary);
    std::copy(std::istreambuf_iterator<char>(input1),
              std::istreambuf_iterator<char>( ),
              std::back_inserter(external_buffer_id_json_buf));
  }

  aiebu::aiebu_assembler as(aiebu::aiebu_assembler::buffer_type::blob_instr_transaction,
                            txn_buf, control_packet_buf, external_buffer_id_json_buf);
  auto e = as.get_elf();
  std::cout << "elf size:" << e.size() << "\n";
  std::ofstream output_file("out.elf");
  std::ostream_iterator<char> output_iterator(output_file);
  std::copy(e.begin(), e.end(), output_iterator);
  return 0;
}
