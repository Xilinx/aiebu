// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_PACKETS_H_
#define AIEBU_PACKETS_H_

#include <string>
#include <cstdint>
#include <iostream>
#include "aiebu/aiebu_assembler.h"

namespace aiebu {

class packets {
private:
  const char *m_buffer;
  uint64_t m_size;
  const aiebu::aiebu_assembler::buffer_type m_buffer_type;

private:
  size_t serialize(std::ostream &stream, size_t offset) const;

public:
  packets(const char *buffer, uint64_t size, aiebu::aiebu_assembler::buffer_type buffer_type)
    : m_buffer(buffer), m_size(size), m_buffer_type(buffer_type) {}

  std::string get_dump() const;
};

}

#endif
