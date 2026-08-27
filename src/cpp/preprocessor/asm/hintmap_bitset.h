// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.
#ifndef AIEBU_PREPROCESSOR_ASM_HINTMAP_BITSET_H_
#define AIEBU_PREPROCESSOR_ASM_HINTMAP_BITSET_H_

#include "utils.h"

#include <bitset>
#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace aiebu {

constexpr uint64_t CHUNK_SIZE = 64ULL * BYTES_PER_KB; // 64KB
constexpr uint64_t COL_SCRATCHPAD_SIZE = 3ULL * BYTES_PER_MB; // 3MB
constexpr uint64_t CHUNKS_PER_COL = COL_SCRATCHPAD_SIZE / CHUNK_SIZE; // 48
constexpr std::size_t HINTMAP_CHUNK_BITS = 512;
constexpr std::size_t HINTMAP_WORD_BITS = 32;
constexpr std::size_t HINTMAP_WORD_COUNT = HINTMAP_CHUNK_BITS / HINTMAP_WORD_BITS; // 16 .long words

using hintmap_chunk_bits = std::bitset<HINTMAP_CHUNK_BITS>;

// chunk range in 64KB units: [first, last)
using chunk_rng = std::pair<uint64_t, uint64_t>;

hintmap_chunk_bits words_to_bitset(const std::vector<uint32_t>& words);

std::optional<uint64_t> bitset_find_first(const hintmap_chunk_bits& bs);
std::optional<uint64_t> bitset_find_last(const hintmap_chunk_bits& bs);

void verify_hintmap_chunk_limit(const hintmap_chunk_bits& bs,
                                uint64_t max_chunks,
                                const std::string& hintmap_label);

hintmap_chunk_bits chunk_range_bitset(uint64_t lo, uint64_t hi);

bool spans_overlap_inclusive(uint64_t lo_a, uint64_t hi_a, uint64_t lo_b, uint64_t hi_b);

std::pair<uint64_t, uint64_t> chunks_to_region(uint64_t lo, uint64_t hi);

std::vector<uint64_t> sorted_pool_chunks(const hintmap_chunk_bits& pool);

// Split pool chunks among k hintmap controllers by cutting at the largest gaps.
std::vector<chunk_rng> partition_flexible_minimize_holes(const std::vector<uint64_t>& sub,
                                                         std::size_t k_sub);

std::pair<uint64_t, uint64_t> chunk_rng_to_region(const chunk_rng& r);

std::string describe_chunk_span(uint64_t lo, uint64_t hi);

// Spec-aligned redistribution: OR pooled hintmap chunks, split at k-1 largest gaps
// in controller column order (isa-spec.md PREEMPT @hintmap example).
std::vector<chunk_rng> redistribute_hintmap_pool(const hintmap_chunk_bits& pool,
                                                 const std::vector<int>& ctrl_cols);

} // namespace aiebu

#endif // AIEBU_PREPROCESSOR_ASM_HINTMAP_BITSET_H_
