// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

#include "utils.h"
#include "writer.h"

#include "aiebu/aiebu_error.h"

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

offset_type
section_writer::
tell() const
{
  return static_cast<offset_type>(m_data.size());
}

uint32_t
section_writer::
read_word(offset_type offset) const
{
  if (offset + 3 >= m_data.size())
    throw error(error::error_code::internal_error, "reading beyond data size !!!");
  return (m_data[offset + 3] << FORTH_BYTE_SHIFT)
         + (m_data[offset + 2] << THIRD_BYTE_SHIFT)
         + (m_data[offset + 1] << SECOND_BYTE_SHIFT)
         + m_data[offset];
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
  for( auto i=0U; i<padsize; ++i)
    write_byte(0x00);
}

ctrl_writer::
ctrl_writer(const std::string& filename)
{
  ofs.open(filename);
  if (!ofs)
    throw std::runtime_error("Unable to open log file: " + filename);
}

ctrl_writer::
~ctrl_writer() {
  ofs.close();
}

void
ctrl_writer::
write_label(const std::string& name)
{
  std::string clean_name = name;
  if (!name.empty() && name.front() == '@')
    clean_name = name.substr(1); // Remove leading '@'

  ofs << clean_name << ":\n";
  current_label = clean_name;
}

void
ctrl_writer::
write_attach_to_group(int col)
{
  ofs << ".attach_to_group " << col << '\n';
}

void
ctrl_writer::
write_directive(const std::string& name)
{
  ofs << name << '\n';
}

void
ctrl_writer::
write_endl(const std::string& name)
{
  std::string clean_name = name;
  if (!clean_name.empty() && clean_name.front() == '@')
    clean_name = clean_name.substr(1);  // Strip leading '@'

  ofs << ".endl " << clean_name << '\n';
}

void
ctrl_writer::
write_eop()
{
  ofs << ".eop\n";
}

void
ctrl_writer::
write_operation(const std::string& name,
                const std::vector<std::string>& args,
                const std::string& label)
{
  if (current_label == label)
    ofs << "    ";

  ofs << name << "\t";

  for (size_t index = 0; index < args.size(); ++index) {
    if (current_label != label)
      ofs << " ";

    ofs << args[index];

   if (index < args.size() - 1)
     ofs << ", ";
  }
  ofs << '\n';
}

}
