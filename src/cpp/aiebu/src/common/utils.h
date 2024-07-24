// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_COMMOM_UTILS_H_
#define _AIEBU_COMMOM_UTILS_H_

#include <fstream>
#include <filesystem>
#include <iostream>
#include <cassert>
#include <cstdint>
#include <string>
#include <sstream>
#include <vector>
#include "aiebu_error.h"

#define BYTE_MASK 0xFF
#define FIRST_BYTE_SHIFT 0
#define SECOND_BYTE_SHIFT 8
#define THIRD_BYTE_SHIFT 16
#define FORTH_BYTE_SHIFT 24

using offset_type = uint32_t;


namespace aiebu {

#define HEADER_ACCESS_GET_SET( TYPE, FNAME )  \
    TYPE get_##FNAME() const            \
    {                                         \
        return m_##FNAME;                     \
    }                                         \
    void set_##FNAME( TYPE val )              \
    {                                         \
        m_##FNAME = val;                      \
    }

#define HEADER_ACCESS_GET( TYPE, FNAME )      \
    TYPE get_##FNAME() const            \
    {                                         \
        return m_##FNAME;                     \
    }

inline std::vector<char> readfile(const std::string& filename)
{
  if (!std::filesystem::exists(filename))
    throw error(error::error_code::internal_error, "file:" + filename + " not found\n");

  std::ifstream input(filename, std::ios::in | std::ios::binary);
  auto file_size = std::filesystem::file_size(filename);
  std::vector<char> buffer(file_size);
  input.read(buffer.data(), file_size);
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

inline std::vector<std::string> splitoption(const char* data, char delimiter = ';')
{
  std::string str = data;
  std::stringstream ss(str);
  std::vector<std::string> tokens;
  std::string token;
  while (std::getline(ss, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

}
#endif // _AIEBU_COMMOM_UTILS_H_
