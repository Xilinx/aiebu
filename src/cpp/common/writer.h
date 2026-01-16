// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_COMMON_WRITER_H_
#define _AIEBU_COMMON_WRITER_H_

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "symbol.h"
#include "code_section.h"

namespace aiebu {

// Class to hold sections name, data, symbols, type
class writer
{
public:
  virtual ~writer() = default;
};

class section_writer: public writer
{
  const std::string m_name;
  const code_section m_type;
  std::vector<uint8_t> m_data;
  std::vector<symbol> m_symbols;

public:
  section_writer(std::string name, code_section type, std::vector<uint8_t>&& data)
    : m_name(std::move(name)),
      m_type(type),
      m_data(std::move(data)) {}
  section_writer(std::string name, code_section type): m_name(std::move(name)), m_type(type) {}

  ~section_writer() override= default;
  section_writer(const section_writer& rhs) = default;
  section_writer& operator=(const section_writer& rhs) = delete;
  section_writer(section_writer &&s) = default;


  virtual void write_byte(uint8_t byte);

  virtual void write_word(uint32_t word);

  // Bulk write methods for better performance
  template <typename T> void
  write_bytes(const T &bytes)
  {
    m_data.insert(m_data.end(), bytes.begin(), bytes.end());
  }

  virtual void reserve(size_t capacity);

  virtual uint32_t read_word(offset_type offset) const;

  virtual void write_word_at(offset_type offset, uint32_t word);

  virtual offset_type tell() const;

  const std::vector<uint8_t>&
  get_data() const
  {
    return m_data;
  }

  const std::string&
  get_name() const
  {
    return m_name;
  }

  code_section
  get_type() const
  {
    return m_type;
  }

  void set_data(std::vector<uint8_t> &data)
  {
    m_data = std::move(data);
  }

  const std::vector<symbol>&
  get_symbols() const
  {
    return m_symbols;
  }

  void add_symbol(symbol sym)
  {
    m_symbols.emplace_back(sym);
  }

  void add_symbols(std::vector<symbol>& syms)
  {
    m_symbols = std::move(syms);
  }

  bool hassymbols() const
  {
    return m_symbols.size();
  }

  void padding(offset_type size);
};

class instance_writer
{
  // map<instance, vector<section_writer object having code and symbol info>>
  std::map<std::string, std::vector<std::shared_ptr<writer>>> m_instance;
  std::vector<std::shared_ptr<writer>> m_common;
public:
  const std::map<std::string, std::vector<std::shared_ptr<writer>>>& get_instance_map() const
  {
    return m_instance;
  }

  const std::vector<std::shared_ptr<writer>>& get_common() const
  {
    return m_common;
  }

  void add_instance(const std::string& instance, std::vector<std::shared_ptr<writer>> val)
  {
    m_instance[instance] = std::move(val);
  }

  void add_common_data(std::shared_ptr<writer> val)
  {
    m_common.emplace_back(val);
  }
};

class aie2_config_writer: public writer
{
  // map<kernel, instance_writer>
  std::map<std::string, instance_writer> m_output;
  std::shared_ptr<const partition_info> m_partition;

public:
  explicit aie2_config_writer(std::shared_ptr<const partition_info> partition): m_partition(std::move(partition)) {}

  const std::map<std::string, instance_writer>&
  get_kernel_map() const { return m_output; }

  void add_kernel_map(const std::string& kernel, const std::string& instance, std::vector<std::shared_ptr<writer>> val) {
    m_output[kernel].add_instance(instance, std::move(val));
  }

  void add_kernel_common_data(const std::string& kernel, std::shared_ptr<writer> val) {
    m_output[kernel].add_common_data(val);
  }

  std::shared_ptr<const partition_info> get_partition_info() const { return m_partition; }
};

class config_writer: public writer
{
  std::map<std::string, std::map<std::string, std::vector<std::shared_ptr<writer>>>> m_output;
  std::shared_ptr<const partition_info> m_partition;

public:
  explicit config_writer(std::shared_ptr<const partition_info> partition): m_partition(std::move(partition)) {}

  const std::map<std::string, std::map<std::string,  std::vector<std::shared_ptr<writer>>>>&
  get_kernel_map() const { return m_output; }

  void add_kernel_map(const std::string& kernel, const std::string& instance, std::vector<std::shared_ptr<writer>> val) {
    m_output[kernel][instance] = std::move(val);
  }

  std::shared_ptr<const partition_info> get_partition_info() const { return m_partition; }
};

class asm_writer
{
public:
  explicit asm_writer(std::ostream& stream); // For single stream
  explicit asm_writer(const std::string& filename); // For file only
  asm_writer(std::ostream& stream, const std::string& filename); // For both

  ~asm_writer();

  // Delete copy operations since we manage unique resources
  asm_writer(const asm_writer&) = delete;
  asm_writer& operator=(const asm_writer&) = delete;

  // Delete move operations to be explicit
  asm_writer(asm_writer&&) = delete;
  asm_writer& operator=(asm_writer&&) = delete;

  void write_directive(const std::string& directive);
  void write_label(const std::string& label);
  void write_attach_to_group(int colnum);
  void write_partition(const std::string& partition_str);
  void write_operation(const std::string& name, const std::vector<std::string>& args, const std::string& label);
  void write_endl(const std::string& label);
  void write_eop();

private:
  std::vector<std::ostream*> m_streams;
  std::unique_ptr<std::ofstream> m_ofstream;
  std::string current_label;
};

}
#endif //_AIEBU_COMMON_WRITER_H_
