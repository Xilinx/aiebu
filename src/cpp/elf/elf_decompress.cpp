// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

#include "aiebu/aiebu_decompress.h"
#include "elf_compression.h"

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>

#include <elfio/elfio.hpp>

namespace aiebu {

// ---------------------------------------------------------------------------
// Full-ELF APIs — delegate to elf_compression.h _impl functions
// ---------------------------------------------------------------------------

bool
is_elf_compressed(const std::vector<char>& elf_bytes)
{
  return is_elf_compressed_impl(elf_bytes.data(), elf_bytes.size());
}

std::vector<char>
decompress_elf(const std::vector<char>& elf_bytes)
{
  if (!is_elf_compressed(elf_bytes))
    return {elf_bytes.begin(), elf_bytes.end()};
  return make_elf_decompressor()->decompress(elf_bytes);
}

// ---------------------------------------------------------------------------
// High-level per-section APIs — ELFIO-aware wrappers
// ---------------------------------------------------------------------------

std::size_t
get_section_uncompressed_size(const ELFIO::section* sec, const ELFIO::elfio& elf)
{
  if (!sec)
    return 0;

  if (!(sec->get_flags() & ELFIO::SHF_COMPRESSED))
    return sec->get_size();

  auto sz = get_uncompressed_section_size_impl(
      sec->get_data(), sec->get_size(),
      static_cast<unsigned char>(elf.get_class()));
  if (sz == 0)
    throw std::runtime_error(
        "SHF_COMPRESSED section '" + sec->get_name()
        + "' has invalid Chdr header");
  return sz;
}

std::size_t
copy_section_uncompressed_data(const ELFIO::section* sec, const ELFIO::elfio& elf,
                               void* dest, std::size_t dest_size)
{
  if (!sec || sec->get_size() == 0)
    return 0;

  if (!(sec->get_flags() & ELFIO::SHF_COMPRESSED)) {
    auto sz = sec->get_size();
    if (dest_size < sz)
      throw std::runtime_error(
          "dest_size too small for section '" + sec->get_name() + "'");
    std::memcpy(dest, sec->get_data(), sz);
    return sz;
  }

  return decompress_section_into_impl(
      sec->get_data(), sec->get_size(),
      dest, dest_size,
      static_cast<unsigned char>(elf.get_class()));
}

} // namespace aiebu
