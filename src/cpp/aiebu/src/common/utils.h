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

using jobid_type = int32_t;
using barrierid_type = uint32_t;
using offset_type = uint32_t;
using pageid_type = uint32_t;
constexpr pageid_type NO_PAGE = -1;
constexpr jobid_type EOF_ID = -2;
constexpr  jobid_type EOP_ID = -3;
constexpr offset_type PAGE_SIZE = 8192;
constexpr int HEX_BASE = 16;


namespace aiebu {

#define HEADER_ACCESS_GET_SET( TYPE, FNAME )  \
    const TYPE get_##FNAME() const            \
    {                                         \
        return m_##FNAME;                     \
    }                                         \
    void set_##FNAME( TYPE val )              \
    {                                         \
        m_##FNAME = val;                      \
    }

#define HEADER_ACCESS_GET( TYPE, FNAME )      \
    const TYPE get_##FNAME() const            \
    {                                         \
        return m_##FNAME;                     \
    }

inline uint32_t parse_register(const std::string& str)
{
  if ((str.rfind("$r") != 0 && str.rfind("$g") != 0))
    throw error(error::error_code::internal_error, "REG val not a register:" + str);

  // get register number
  uint32_t val = std::stoi(str.substr(2));
  if (str.rfind("$g") == 0)
  {
    if (val >= 16)
      throw error(error::error_code::internal_error, "Global Register " + str + " number out of range: " + std::to_string(val));
    val = val + 8;
  }

  if (val >= 24)
    throw error(error::error_code::internal_error, "Register number " + str + " out of range: " + std::to_string(val));
  return val;
}


inline uint32_t parse_barrier(const std::string& str)
{
  // get barrier id
  // TODO: this is temporary for backward support. Should be removed once all migrate to new.
  if (str.rfind("$") != 0)
    return std::stoi(str);

  if ((str.rfind("$lb") != 0 && str.rfind("$rb") != 0))
    throw error(error::error_code::internal_error, "BARRIER val not a barrier: " + str);
  uint32_t val = std::stoi(str.substr(3));

  if (str.rfind("$rb") == 0)
  {
    val = val + 1;

    if ((val <= 0 || val > 65))
      throw error(error::error_code::internal_error, "REMOTE BARRIER  " + str + " number out of range: " + std::to_string(val));
  }
  else //$lb
    if (val >= 16)
      throw error(error::error_code::internal_error, "LOCAL BARRIER  " + str + " number out of range: " + std::to_string(val));
  return val;
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
