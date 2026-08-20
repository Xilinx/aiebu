// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
#ifndef AIEBU_COMMOM_UTILS_H_
#define AIEBU_COMMOM_UTILS_H_
#include "aiebu/aiebu_error.h"
#include "common/regex_wrapper.h"

#include <cassert>
#include <cstdint>
#include <limits>
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
using opcode_type = uint8_t;

constexpr pageid_type NO_PAGE = std::numeric_limits<pageid_type>::max();
const static jobid_type EOF_ID("EOF");
const static jobid_type EOP_ID("EOP");
constexpr offset_type PAGE_SIZE = 8192;
constexpr int HEX_BASE = 16;
constexpr int WORD_SIZE = 4;
constexpr uint32_t byte_to_bits = 8;
constexpr uint32_t shim_bd_len = 9;
constexpr opcode_type align_opcode = 0xA5;
const std::string default_source_filename = "default";

namespace aiebu {

enum class asm_dump_flag {
  text,     //Only text section dump information is generated and dumped in .dump section & debug_map.json
  disable,  //Only text section dump information is generated but dumped in debug_map.json only
  full      //Both text & data section dump information is generated and dumped in .dump section & debug_map.json
};

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

constexpr uint32_t DEFAULT_COLUMN = 4;

class partition_info {
  union partition_union {
    struct core_mem_info {
      uint32_t core;
      uint32_t mem;
    } core_mem;
    uint32_t column;
  } partition_union;
  public:
  partition_info() : partition_info(0, 0) {}
  partition_info(uint32_t core, uint32_t mem): partition_union{{core, mem}} { }

  uint32_t get_numcore() const { return partition_union.core_mem.core; }

  uint32_t get_numcolumn() const { return partition_union.column; }

  uint32_t get_nummem() const { return partition_union.core_mem.mem; }

  void set_numcolumn(uint32_t val) { partition_union.column = val; }

  void set_numcore(uint32_t val) { partition_union.core_mem.core = val; }

  void set_nummem(uint32_t val) { partition_union.core_mem.mem = val; }
};

// target_info: stores target architecture information from .target directive
// Format: .target <arch>-<sub-arch>
class target_info {
  std::string m_arch;
  std::string m_sub_arch;
public:
  target_info() : m_arch(""), m_sub_arch("") {}
  target_info(const std::string& arch, const std::string& sub_arch = "")
    : m_arch(arch), m_sub_arch(sub_arch) {}

  std::string get_arch() const { return m_arch; }
  std::string get_sub_arch() const { return m_sub_arch; }
  bool has_sub_arch() const { return !m_sub_arch.empty(); }
  bool is_set() const { return !m_arch.empty(); }

  void set_arch(const std::string& arch) { m_arch = arch; }
  void set_sub_arch(const std::string& sub_arch) { m_sub_arch = sub_arch; }

  // Returns full target string: "arch" or "arch-sub_arch"
  std::string get_full_target() const {
    if (m_sub_arch.empty())
      return m_arch;
    return m_arch + "-" + m_sub_arch;
  }
};

// aie_row_topology_info: stores row topology information from .aie_row_topology directive
// Format: .aie_row_topology A-B-C-D
// Where: A=num_south_shim, B=num_memtile_row, C=num_coretile_row, D=num_north_shim
// Example: .aie_row_topology 1-1-4-0
class aie_row_topology_info {
  uint32_t m_num_south_shim;    // Number of south shim rows in a column
  uint32_t m_num_memtile_row;   // Number of memtile rows in a column
  uint32_t m_num_coretile_row;  // Number of coretile rows in a column
  uint32_t m_num_north_shim;    // Number of north shim rows in a column
  bool m_is_set;
public:
  aie_row_topology_info()
    : m_num_south_shim(0), m_num_memtile_row(0), m_num_coretile_row(0), m_num_north_shim(0), m_is_set(false) {}
  aie_row_topology_info(uint32_t south_shim, uint32_t memtile, uint32_t coretile, uint32_t north_shim)
    : m_num_south_shim(south_shim), m_num_memtile_row(memtile), m_num_coretile_row(coretile), m_num_north_shim(north_shim), m_is_set(true) {}

  uint32_t get_num_south_shim() const { return m_num_south_shim; }
  uint32_t get_num_memtile_row() const { return m_num_memtile_row; }
  uint32_t get_num_coretile_row() const { return m_num_coretile_row; }
  uint32_t get_num_north_shim() const { return m_num_north_shim; }
  bool is_set() const { return m_is_set; }

  void set_num_south_shim(uint32_t val) { m_num_south_shim = val; }
  void set_num_memtile_row(uint32_t val) { m_num_memtile_row = val; }
  void set_num_coretile_row(uint32_t val) { m_num_coretile_row = val; }
  void set_num_north_shim(uint32_t val) { m_num_north_shim = val; }
  void set_is_set(bool val) { m_is_set = val; }

  // Returns topology string: "A-B-C-D"
  std::string get_topology_string() const {
    return std::to_string(m_num_south_shim) + "-" +
           std::to_string(m_num_memtile_row) + "-" +
           std::to_string(m_num_coretile_row) + "-" +
           std::to_string(m_num_north_shim);
  }
};

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

aiebu::regex get_regex(const std::vector<fragment>& pattern);

constexpr unsigned hexbase = 0x10;
constexpr uint64_t BYTES_PER_MB = 1024ULL * 1024ULL;

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

std::string metrics_report();

// Extracts the plain identifier from a C++ mangled symbol name.
// Format: _Z<length><name>...  e.g. "_Z3DPUPcPc" -> "DPU"
// Returns "" if the name is not a valid mangled symbol.
std::string extract_kernel_name_from_mangled(const std::string& symbol_name);

}
#endif // AIEBU_COMMOM_UTILS_H_
