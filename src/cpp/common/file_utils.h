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

inline std::vector<unsigned char> readfile(const std::string& filename)
{
  if (!std::filesystem::exists(filename))
    throw error(error::error_code::internal_error, "file:" + filename + " not found\n");

  std::ifstream input(filename, std::ios::in | std::ios::binary);
  auto file_size = std::filesystem::file_size(filename);
  std::vector<unsigned char> buffer(file_size);
  input.read(reinterpret_cast<char *>(buffer.data()), static_cast<std::streamsize>(file_size));
  return buffer;
}

inline std::string findFilePath(const std::string& filename, const std::vector<std::string>& libpaths)
{
  for (const auto &dir : libpaths ) {
    auto ret = std::filesystem::exists(dir + "/" + filename);
    if (ret) {
      return dir + "/" + filename;
    }
  }
  throw error(error::error_code::internal_error, filename + " file not found!!\n");
}

aiebu_assembler::buffer_type identify_buffer_type(const std::vector<unsigned char> &buffer);

}

#endif // AIEBU_COMMOM_FILE_UTILS_H_
