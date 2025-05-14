// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

#include <array>
#include <sstream>

#include "file_utils.h"
#include "packets.h"

namespace aiebu {

char control_packet_operations_map(uint32_t op)
{
  static const std::array<char, 4> optable = {'P', 'R', 'W', 'U'};
  if (op >= 4)
    return 'U';
  return optable[op];
}

size_t packets::serialize(std::ostream &stream, size_t offset) const
{
  const auto header = reinterpret_cast<const cp_pktheader_aie2p *>(m_buffer + offset);
  offset += sizeof(*header);
  const auto info = reinterpret_cast<const cp_ctrlinfo_aie2p *>(m_buffer + offset);
  offset += sizeof(*info);

  stream << "PH {#" << header->stream_packet_ID << ", R[" << header->source_row
         << "?], C[" << header->source_col << "?]}\n";

  stream << "CH {@0x" << std::hex << info->local_byte_addr << std::dec << ", ["
         << info->num_data_beat + 1 << "], " << control_packet_operations_map(info->operation) << "}\n";
  for (uint32_t i = 0; i <= info->num_data_beat; i++) {
    stream << "   [" << i << "] 0x" << std::hex
           << *(reinterpret_cast<const unsigned int *>(m_buffer + offset))
           << std::dec << "\n";
    offset += sizeof(unsigned int);
  }
  return offset;
}

std::string
packets::get_dump() const
{
  std::stringstream stream;
  size_t offset = 0;
  while (offset < m_size) {
    offset = serialize(stream, offset);
  }
  return stream.str();
}

}
