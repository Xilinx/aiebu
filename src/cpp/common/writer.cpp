// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.

#include "utils.h"
#include "writer.h"

#include "aiebu/aiebu_error.h"
#include <iostream>
#include <utility>

namespace aiebu {

void
section_writer::
write_byte(uint8_t byte)
{
  m_data.push_back(byte);
}

void
section_writer::
write_word(uint32_t word)
{
  write_byte((word >> FIRST_BYTE_SHIFT) & BYTE_MASK);
  write_byte((word >> SECOND_BYTE_SHIFT) & BYTE_MASK);
  write_byte((word >> THIRD_BYTE_SHIFT) & BYTE_MASK);
  write_byte((word >> FORTH_BYTE_SHIFT) & BYTE_MASK);
}


void
section_writer::
reserve(size_t capacity)
{
  m_data.reserve(capacity);
}

offset_type
section_writer::
tell() const
{
  auto size = static_cast<offset_type>(m_data.size());
  return size;
}

uint32_t
section_writer::
read_word(offset_type offset) const
{
  if (offset + 3 >= m_data.size())
    throw error(error::error_code::internal_error, "reading beyond data size !!!");
  uint32_t result = (m_data[offset + 3] << FORTH_BYTE_SHIFT)
         + (m_data[offset + 2] << THIRD_BYTE_SHIFT)
         + (m_data[offset + 1] << SECOND_BYTE_SHIFT)
         + m_data[offset];
  return result;
}

void
section_writer::
write_word_at(offset_type offset, uint32_t word)
{
  m_data[offset] = ((word >> FIRST_BYTE_SHIFT) & BYTE_MASK);
  m_data[offset + 1] = ((word >> SECOND_BYTE_SHIFT) & BYTE_MASK);
  m_data[offset + 2] = ((word >> THIRD_BYTE_SHIFT) & BYTE_MASK);
  m_data[offset + 3] = ((word >> FORTH_BYTE_SHIFT) & BYTE_MASK);
}

void
section_writer::
padding(offset_type pagesize)
{
  auto datasize = tell();
  if (datasize > pagesize)
    throw error(error::error_code::internal_error, "page content more the pagesize !!!");
  auto padsize = pagesize - datasize;
  if (padsize > 0) {
    m_data.insert(m_data.end(), padsize, 0x00);
  }
}

asm_writer::asm_writer(std::ostream& stream)
{
  m_streams.push_back(&stream);
}

asm_writer::asm_writer(const std::string& filename)
  : m_ofstream(std::make_unique<std::ofstream>(filename))
{
  if (!m_ofstream->is_open())
    throw std::runtime_error("Failed to open file: " + filename);
  m_streams.push_back(m_ofstream.get());
}

asm_writer::asm_writer(std::ostream& stream, const std::string& filename)
  : m_ofstream(std::make_unique<std::ofstream>(filename))
{
  if (!m_ofstream->is_open())
    throw std::runtime_error("Failed to open file: " + filename);
  m_streams.push_back(&stream);
  m_streams.push_back(m_ofstream.get());
}

asm_writer::~asm_writer() = default;
template <typename Func>
void for_all_streams(std::vector<std::ostream*>& streams, Func&& func) {
  for (auto s : streams) {
    std::forward<Func>(func)(s);
  }
}

void asm_writer::write_label(const std::string& name)
{
  std::string clean_name = name;
  if (!name.empty() && name.front() == '@')
    clean_name = name.substr(1);
  for_all_streams(m_streams, [&](std::ostream* s) { (*s) << clean_name << ":\n"; });
  current_label = clean_name;
}

void asm_writer::write_attach_to_group(int col)
{
  for_all_streams(m_streams, [&](std::ostream* s) { (*s) << ".attach_to_group " << col << '\n'; });
}

void asm_writer::write_partition(const std::string& partition_str)
{
  for_all_streams(m_streams, [&](std::ostream* s) { (*s) << ".partition\t " << partition_str << '\n'; });
}

void asm_writer::write_directive(const std::string& name)
{
  for_all_streams(m_streams, [&](std::ostream* s) { (*s) << name << '\n'; });
}

void asm_writer::write_endl(const std::string& name)
{
  std::string clean_name = name;
  if (!clean_name.empty() && clean_name.front() == '@')
    clean_name = clean_name.substr(1);
  for_all_streams(m_streams, [&](std::ostream* s) { (*s) << ".endl " << clean_name << '\n'; });
}

void asm_writer::write_eop()
{
  for_all_streams(m_streams, [&](std::ostream* s) { (*s) << ".eop\n"; });
}

void asm_writer::write_operation(const std::string& name,
                                  const std::vector<std::string>& args,
                                  const std::string& label)
{
  for_all_streams(m_streams, [&](std::ostream* s) {
    if (current_label == label) {
      if (!(name == "start_job" || name == "end_job" || name == "eof" || name == "start_cond_job_preempt")) {
        (*s) << "    ";
      }
    }
    (*s) << name << "\t";
    for (size_t index = 0; index < args.size(); ++index) {
      if (current_label != label)
        (*s) << " ";
      (*s) << args[index];
      if (index < args.size() - 1)
        (*s) << ", ";
    }
    (*s) << '\n';
  });
}

}
