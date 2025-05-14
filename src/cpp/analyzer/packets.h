// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include <string>
#include <cstdint>
#include <iostream>

namespace aiebu {

class packets {
private:
  const char *m_buffer;
  uint64_t m_size;

private:
  size_t serialize(std::ostream &stream, size_t offset) const;

public:
  packets(const char *buffer, uint64_t size) : m_buffer(buffer), m_size(size) {}
  std::string get_dump() const;
};

}
