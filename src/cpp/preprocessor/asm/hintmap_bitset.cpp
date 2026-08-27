// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.
#include "hintmap_bitset.h"

#include "aiebu/aiebu_error.h"

#include <algorithm>
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

std::vector<uint64_t>
sorted_pool_chunks(const hintmap_chunk_bits& pool)
{
  std::vector<uint64_t> chunks;
  for (std::size_t c = 0; c < pool.size(); ++c) {
    if (pool.test(c))
      chunks.push_back(static_cast<uint64_t>(c));
  }
  return chunks;
}

std::vector<chunk_rng>
partition_flexible_minimize_holes(const std::vector<uint64_t>& sub, std::size_t k_sub)
{
  std::vector<chunk_rng> result;
  if (sub.empty() || k_sub == 0)
    return result;

  if (k_sub == 1) {
    result.emplace_back(sub.front(), sub.back() + 1);
    return result;
  }

  const int n = static_cast<int>(sub.size());
  if (n <= static_cast<int>(k_sub)) {
    for (std::size_t i = 0; i < k_sub; ++i) {
      if (i < sub.size())
        result.emplace_back(sub[i], sub[i] + 1);
      else
        result.emplace_back(0, 0);
    }
    return result;
  }

  std::vector<std::pair<int, int>> gaps;
  for (std::size_t i = 0; i + 1 < sub.size(); ++i) {
    const int gap = static_cast<int>(sub[i + 1] - sub[i] - 1);
    if (gap > 0)
      gaps.emplace_back(gap, static_cast<int>(i + 1));
  }
  std::sort(gaps.rbegin(), gaps.rend());

  std::vector<int> cut_indices{0};
  const int cuts_to_make = std::min(static_cast<int>(gaps.size()), static_cast<int>(k_sub) - 1);
  std::vector<int> chosen_split_points;
  for (int i = 0; i < cuts_to_make; ++i)
    chosen_split_points.push_back(gaps[static_cast<std::size_t>(i)].second);
  std::sort(chosen_split_points.begin(), chosen_split_points.end());

  for (int idx : chosen_split_points)
    cut_indices.push_back(idx);
  cut_indices.push_back(n);

  if (static_cast<int>(cut_indices.size()) - 1 < static_cast<int>(k_sub)) {
    cut_indices.clear();
    for (std::size_t i = 0; i <= k_sub; ++i)
      cut_indices.push_back((static_cast<int>(i) * n) / static_cast<int>(k_sub));
  }

  for (std::size_t i = 0; i + 1 < cut_indices.size(); ++i) {
    const int start_idx = cut_indices[i];
    const int end_idx   = cut_indices[i + 1] - 1;
    if (start_idx <= end_idx && end_idx < n)
      result.emplace_back(sub[static_cast<std::size_t>(start_idx)],
                          sub[static_cast<std::size_t>(end_idx)] + 1);
    else
      result.emplace_back(0, 0);
  }
  return result;
}

std::pair<uint64_t, uint64_t>
chunk_rng_to_region(const chunk_rng& r)
{
  const uint64_t n = r.second - r.first;
  return {n ? r.first * CHUNK_SIZE : 0, n * CHUNK_SIZE};
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

std::vector<chunk_rng>
redistribute_hintmap_pool(const hintmap_chunk_bits& pool,
                          const std::vector<int>& ctrl_cols)
{
  std::vector<chunk_rng> out(ctrl_cols.size(), {0, 0});
  if (pool.none() || ctrl_cols.empty())
    return out;

  const std::vector<uint64_t> chunks = sorted_pool_chunks(pool);
  const auto parts = partition_flexible_minimize_holes(chunks, ctrl_cols.size());
  for (std::size_t i = 0; i < parts.size() && i < ctrl_cols.size(); ++i)
    out[i] = parts[i];
  return out;
}

} // namespace aiebu
