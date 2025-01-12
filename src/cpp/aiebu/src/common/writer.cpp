// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include "writer.h"
#include "aiebu_error.h"
#include "utils.h"

namespace aiebu {

void
writer::
write_byte(uint8_t byte)
{
  m_data.push_back(byte);
}

void
writer::
write_word(uint32_t word)
{
  write_byte((word >> FIRST_BYTE_SHIFT) & BYTE_MASK);
  write_byte((word >> SECOND_BYTE_SHIFT) & BYTE_MASK);
  write_byte((word >> THIRD_BYTE_SHIFT) & BYTE_MASK);
  write_byte((word >> FORTH_BYTE_SHIFT) & BYTE_MASK);
}

offset_type
writer::
tell() const
{
  return static_cast<offset_type>(m_data.size());
}

uint32_t
writer::
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
writer::
write_word_at(offset_type offset, uint32_t word)
{
  m_data[offset] = ((word >> FIRST_BYTE_SHIFT) & BYTE_MASK);
  m_data[offset + 1] = ((word >> SECOND_BYTE_SHIFT) & BYTE_MASK);
  m_data[offset + 2] = ((word >> THIRD_BYTE_SHIFT) & BYTE_MASK);
  m_data[offset + 3] = ((word >> FORTH_BYTE_SHIFT) & BYTE_MASK);
}

void
writer::
padding(offset_type pagesize)
{
  auto datasize = tell();
  if (datasize > pagesize)
    throw error(error::error_code::internal_error, "page content more the pagesize !!!");
  auto padsize = pagesize - datasize;
  for( auto i=0U; i<padsize; ++i)
    write_byte(0x00);
}

}
