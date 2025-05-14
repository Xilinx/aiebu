// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
#ifndef AIEBU_COMMOM_UTILS_H_
#define AIEBU_COMMOM_UTILS_H_
#include "aiebu/aiebu_error.h"

#include <cassert>
#include <cstdint>
#include <limits>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <bitset>

#define BYTE_MASK 0xFF
#define FIRST_BYTE_SHIFT 0
#define SECOND_BYTE_SHIFT 8
#define THIRD_BYTE_SHIFT 16
#define FORTH_BYTE_SHIFT 24

using jobid_type = std::string;
using barrierid_type = uint32_t;
using offset_type = uint32_t;
using pageid_type = uint32_t;
constexpr pageid_type NO_PAGE = -1;
const static jobid_type EOF_ID("EOF");
const static jobid_type EOP_ID("EOP");
constexpr offset_type PAGE_SIZE = 8192;
constexpr int HEX_BASE = 16;
constexpr int WORD_SIZE = 4;

namespace aiebu {

#define HEADER_ACCESS_GET_SET( TYPE, FNAME )  \
    TYPE get_##FNAME() const                  \
    {                                         \
        return m_##FNAME;                     \
    }                                         \
    void set_##FNAME( TYPE val )              \
    {                                         \
        m_##FNAME = val;                      \
    }

#define HEADER_ACCESS_GET( TYPE, FNAME )      \
    TYPE get_##FNAME() const                  \
    {                                         \
        return m_##FNAME;                     \
    }


inline uint8_t low_8(uint32_t num) { return (num >> FIRST_BYTE_SHIFT ) & BYTE_MASK; }
inline uint8_t high_8(uint32_t num) { return (num >> SECOND_BYTE_SHIFT) & BYTE_MASK; }

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

inline std::string get_pagelabel(const std::string& label)
{
  if (label.empty())
    return "";
  auto val = splitoption(label.c_str(), ':');
  return val[val.size() -1];
}

// Custom stream buffer that reads from a vector<char>
class vector_streambuf : public std::streambuf {
public:
    explicit vector_streambuf(const std::vector<char>& vec) {
        char* begin = const_cast<char*>(vec.data());
        this->setg(begin, begin, begin + vec.size());
    }
};

namespace utilities {
inline std::vector<const char*>
vector_of_string_to_vector_of_char(const std::vector<std::string>& args)
{
  std::vector<const char*> char_vec;
  for (auto& arg : args)
    char_vec.push_back(arg.c_str());

  return char_vec;
}
}

enum class fragment {
  begin_anchor_re,
  end_anchor_re,
  hex_re,
  dec_re,
  add_dec_re,
  l_brack_re,
  r_brack_re,
  equal_re,
  index_re,
  l_curly_re,
  r_curly_re,
  column,
  row,
  dma,
};

std::regex get_regex(const std::vector<fragment>& pattern);

constexpr unsigned hexbase = 0x10;

// Convert a string representation of a sized unsigned integer to the target integral value
template <typename UIntType>
UIntType
to_uinteger(const std::string& token) {
  const unsigned long long result = (std::is_same<UIntType, uint64_t>::value) ? std::stoull(token, nullptr, 0) :
    std::stoul(token, nullptr, 0);
  auto max = std::numeric_limits<UIntType>::max();
  if (result > max)
    throw error(error::error_code::invalid_asm, "Value " + token + " is out of range");

  return static_cast<UIntType>(result);
}

// Extract a specific byte from a 32-bit unsigned integer
template <unsigned int N>
uint8_t
get_byte(uint32_t data) {
  uint32_t mask = BYTE_MASK;
  static_assert((N >= 0) && (N <= 3));
  const unsigned int shift = N * 8;
  mask <<= shift;
  data = data & mask;
  data >>= shift;
  return static_cast<uint8_t>(data);
}

// Perform odd parity check for a 32-bit unsigned integer
inline bool
odd_parity_check(uint32_t data) {
  const std::bitset<32> parity(data);
  return (parity.count() & 0x1) ? true : false;
}

inline bool
even_parity_check(uint32_t data) {
  return !odd_parity_check(data);
}

std::string version_string();

}
#endif // AIEBU_COMMOM_UTILS_H_
