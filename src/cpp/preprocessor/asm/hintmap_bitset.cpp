// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.
#include "hintmap_bitset.h"

#include "aiebu/aiebu_error.h"

#include <sstream>

namespace aiebu {

namespace {

// First set bit in val (val must be non-zero); used only for error reporting.
std::size_t
first_set_bit(uint32_t val)
{
  for (std::size_t bit = 0; bit < HINTMAP_WORD_BITS; ++bit) {
    if (val & (1U << bit))
      return bit;
  }
  return 0;
}

} // namespace

hintmap_chunk_bits
words_to_bitset(const std::vector<uint32_t>& words)
{
  hintmap_chunk_bits bs;
  for (std::size_t w = 0; w < words.size(); ++w) {
    const uint32_t val = words[w];
    if (val == 0)
      continue;

    const std::size_t base = w * HINTMAP_WORD_BITS;
    if (w >= HINTMAP_WORD_COUNT) {
      throw error(error::error_code::invalid_asm,
                  "hintmap sets chunk bit " + std::to_string(base + first_set_bit(val))
                  + " which exceeds supported range [0, "
                  + std::to_string(HINTMAP_CHUNK_BITS - 1) + "]");
    }

    hintmap_chunk_bits chunk(val);
    bs |= (chunk << base);
  }
  return bs;
}

std::optional<uint64_t>
bitset_find_first(const hintmap_chunk_bits& bs)
{
  for (std::size_t i = 0; i < bs.size(); ++i)
    if (bs.test(i))
      return static_cast<uint64_t>(i);
  return std::nullopt;
}

std::optional<uint64_t>
bitset_find_last(const hintmap_chunk_bits& bs)
{
  for (std::size_t i = bs.size(); i-- > 0;)
    if (bs.test(i))
      return static_cast<uint64_t>(i);
  return std::nullopt;
}

void
verify_hintmap_chunk_limit(const hintmap_chunk_bits& bs,
                           uint64_t max_chunks,
                           const std::string& hintmap_label)
{
  if (max_chunks == 0)
    return;
  if (auto last = bitset_find_last(bs)) {
    if (*last >= max_chunks)
      throw error(error::error_code::invalid_asm,
                  "hintmap '" + hintmap_label + "': chunk bit " + std::to_string(*last)
                  + " exceeds valid partition range [0, " + std::to_string(max_chunks - 1) + "]");
  }
}

hintmap_chunk_bits
chunk_range_bitset(uint64_t lo, uint64_t hi)
{
  hintmap_chunk_bits bs;
  for (uint64_t chunk = lo; chunk < hi && chunk < HINTMAP_CHUNK_BITS; ++chunk)
    bs.set(static_cast<std::size_t>(chunk));
  return bs;
}

bool
spans_overlap_inclusive(uint64_t lo_a, uint64_t hi_a, uint64_t lo_b, uint64_t hi_b)
{
  return lo_a <= hi_b && lo_b <= hi_a;
}

std::pair<uint64_t, uint64_t>
chunks_to_region(uint64_t lo, uint64_t hi)
{
  return {lo * CHUNK_SIZE, (hi - lo + 1) * CHUNK_SIZE};
}

std::pair<uint64_t, uint64_t>
column_slice_bounds(int col)
{
  const uint64_t lo = static_cast<uint64_t>(col / 2) * CHUNKS_PER_COL;
  return {lo, lo + CHUNKS_PER_COL};
}

hintmap_chunk_bits
hintmap_in_column_slice(const hintmap_chunk_bits& bm, int col)
{
  const auto [lo, hi] = column_slice_bounds(col);
  return bm & chunk_range_bitset(lo, hi);
}

std::pair<uint64_t, uint64_t>
region_from_hintmap_bits(const hintmap_chunk_bits& bm)
{
  if (bm.none())
    return {0, 0};
  const auto lo = bitset_find_first(bm);
  const auto hi = bitset_find_last(bm);
  return chunks_to_region(*lo, *hi);
}

std::string
describe_chunk_span(uint64_t lo, uint64_t hi)
{
  std::ostringstream oss;
  oss << "chunks [" << lo << ".." << hi << "] ("
      << (hi - lo + 1) << " chunks, scratchpad [0x" << std::hex
      << (lo * CHUNK_SIZE) << ", 0x" << ((hi + 1) * CHUNK_SIZE) << ")" << std::dec << ")";
  return oss.str();
}

} // namespace aiebu
