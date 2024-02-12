// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include "writer.h"
#include "aiebu_error.h"

namespace aiebu {

void
writer::
write_byte(uint8_t byte, code_section sec, uint32_t colnum, uint32_t pagenum)
{
  auto it = m_buffermap.find(colnum);
  if (it == m_buffermap.end())
    m_buffermap[colnum] = std::vector<std::pair<std::vector<uint8_t>,std::vector<uint8_t>>>();
  if (pagenum >= m_buffermap[colnum].size())
    m_buffermap[colnum].emplace_back(std::make_pair<std::vector<uint8_t>, std::vector<uint8_t>>({},{}));
  auto &a = m_buffermap[colnum][pagenum];
  if (sec == code_section::text)
    a.first.push_back(byte);
  else
    a.second.push_back(byte);
}

void
writer::
write_word(uint32_t word, code_section sec, uint32_t colnum, uint32_t pagenum)
{
  write_byte(word & 0xFF, sec, colnum, pagenum);
  write_byte((word >> 8) & 0xFF, sec, colnum, pagenum);
  write_byte((word >> 16) & 0xFF, sec, colnum, pagenum);
  write_byte((word >> 24) & 0xFF, sec, colnum, pagenum);
}

offset_type
writer::
tell(code_section sec, uint32_t colnum, uint32_t pagenum)
{
  if (sec == code_section::text)
    return m_buffermap[colnum].at(pagenum).first.size();
  else
    return m_buffermap[colnum].at(pagenum).first.size() +
           m_buffermap[colnum].at(pagenum).second.size();
}


void
writer::
padding(uint32_t col, pageid_type pagenum, offset_type pagesize)
{
  auto datasize = tell(code_section::data, col, pagenum);
  if (datasize > pagesize)
    throw error(error::error_code::internal_error, "page content more the pagesize !!!");
  auto padsize = pagesize - datasize;
  for( int i=0; i<padsize; ++i)
    write_byte(0x00, code_section::data, col, pagenum);
}

}
