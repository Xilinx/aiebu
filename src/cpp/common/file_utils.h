// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_COMMOM_FILE_UTILS_H_
#define AIEBU_COMMOM_FILE_UTILS_H_

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "aiebu/aiebu_error.h"
#include "aiebu/aiebu_assembler.h"

namespace aiebu {

// Defined in AIE2p Architecture Specification v1.4
struct cp_pktheader_aie2p
{
  uint32_t stream_packet_ID : 5;
  uint32_t out_of_order_bd_idx : 6;
  uint32_t reserved_a_0 : 1;
  uint32_t stream_id_rtn : 3;
  uint32_t reserved_b_0 : 1;
  uint32_t source_row : 5;
  uint32_t source_col : 7;
  uint32_t reserved_c_000 : 3;
  uint32_t odd_parity : 1;
};

// Defined in AIE2p Architecture Specification v1.4
struct cp_ctrlinfo_aie2p
{
  uint32_t local_byte_addr : 20;
  uint32_t num_data_beat : 2;
  uint32_t operation : 2;
  uint32_t stream_id_rtn : 5;
  uint32_t reserved_00 : 2;
  uint32_t parity : 1;
};

inline std::vector<char>
readfile(const std::string& filename)
{
  if (!std::filesystem::exists(filename))
    throw error(error::error_code::internal_error, "file:" + filename + " not found\n");

  std::ifstream input(filename, std::ios::in | std::ios::binary);
  auto file_size = std::filesystem::file_size(filename);

  if (!file_size)
    throw error(error::error_code::invalid_asm, "filename " + filename + " is empty!!");

  std::vector<char> buffer(file_size);
  input.read(buffer.data(), static_cast<std::streamsize>(file_size));
  return buffer;
}

inline std::string
findFilePath(const std::string& filename, const std::vector<std::string>& libpaths)
{
  for (const auto &dir : libpaths ) {
    auto ret = std::filesystem::exists(dir + "/" + filename);
    if (ret) {
      return dir + "/" + filename;
    }
  }
  throw error(error::error_code::internal_error, filename + " file not found!!\n");
}

aiebu_assembler::buffer_type
identify_buffer_type(const std::vector<char> &buffer);

}

#endif // AIEBU_COMMOM_FILE_UTILS_H_
