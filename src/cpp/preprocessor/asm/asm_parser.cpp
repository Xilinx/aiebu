// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.
#include "asm_parser.h"
#include "preprocessor/aie4/aie4_save_restore_map_prebuilt.h"

#include "aiebu/aiebu_error.h"

#include <fstream>
#include <iomanip>
#include <optional>
#include <algorithm>
#include <climits>
#include "common/regex_wrapper.h"
#include <sstream>

// ---------------------------------------------------------------------------
// MSVC does not provide GCC __builtin_popcount / __builtin_ctz / __builtin_clz.
// Provide portable replacements when compiling with MSVC.
// ---------------------------------------------------------------------------
#if defined(_MSC_VER)
static inline int aiebu_popcount(unsigned int x) {
  int count = 0;
  while (x) {
      count += x & 1;
      x >>= 1;
  }
  return count;
}

static inline int aiebu_ctz(unsigned int x)
{
  // _BitScanForward: index of lowest set bit; x must be non-zero (caller guarantees w != 0)
  unsigned long idx = 0;
  _BitScanForward(&idx, static_cast<unsigned long>(x));
  return static_cast<int>(idx);
}
static inline int aiebu_clz(unsigned int x)
{
  // _BitScanReverse: index of highest set bit; x must be non-zero (caller guarantees w != 0)
  unsigned long idx = 0;
  _BitScanReverse(&idx, static_cast<unsigned long>(x));
  return 31 - static_cast<int>(idx);
}
#else
static inline int aiebu_popcount(unsigned int x) { return __builtin_popcount(x); }
static inline int aiebu_ctz(unsigned int x)      { return __builtin_ctz(x);      }
static inline int aiebu_clz(unsigned int x)      { return __builtin_clz(x);      }
#endif

namespace aiebu {

namespace {

static hintmap_chunk_bits
words_to_bitset(const std::vector<uint32_t>& words)
{
  hintmap_chunk_bits bs;
  for (std::size_t w = 0; w < words.size(); ++w) {
    const uint32_t val = words[w];
    const std::size_t base = w * WORD_BITS;
    for (std::size_t bit = 0; bit < WORD_BITS; ++bit) {
      if (val & (1U << bit)) {
        const std::size_t chunk = base + bit;
        if (chunk >= HINTMAP_CHUNK_BITS) {
          throw error(error::error_code::invalid_asm,
                     "hintmap sets chunk bit " + std::to_string(chunk)
                     + " which exceeds supported range [0, "
                     + std::to_string(HINTMAP_CHUNK_BITS - 1) + "]");
        }
        bs.set(chunk);
      }
    }
  }
  return bs;
}

static std::optional<uint64_t>
bitset_find_first(const hintmap_chunk_bits& bs)
{
  for (std::size_t i = 0; i < bs.size(); ++i)
    if (bs.test(i))
      return static_cast<uint64_t>(i);
  return std::nullopt;
}

static std::optional<uint64_t>
bitset_find_last(const hintmap_chunk_bits& bs)
{
  for (std::size_t i = bs.size(); i-- > 0;)
    if (bs.test(i))
      return static_cast<uint64_t>(i);
  return std::nullopt;
}

static void
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

static hintmap_chunk_bits
chunk_range_bitset(uint64_t lo, uint64_t hi)
{
  hintmap_chunk_bits bs;
  for (uint64_t chunk = lo; chunk < hi && chunk < HINTMAP_CHUNK_BITS; ++chunk)
    bs.set(static_cast<std::size_t>(chunk));
  return bs;
}

static bool
spans_overlap_inclusive(uint64_t lo_a, uint64_t hi_a, uint64_t lo_b, uint64_t hi_b)
{
  return lo_a <= hi_b && lo_b <= hi_a;
}

static std::pair<uint64_t, uint64_t>
chunks_to_region(uint64_t lo, uint64_t hi)
{
  return {lo * CHUNK_SIZE, (hi - lo + 1) * CHUNK_SIZE};
}

// chunk range in 64KB units: [first, last)
using chunk_rng = std::pair<uint64_t, uint64_t>;

// Hard cut: a no-hintmap controller's fixed 3MB home window.  Pool runs must not
// merge across a hard cut; each segment between hard cuts is handled on its own.

struct pool_segment {
  uint64_t lo; // inclusive
  uint64_t hi; // exclusive
};

// One controller slot in column order: hintmap_cols + fixed (no-hintmap).
struct controller_spec {
  int      col;
  bool     is_fixed; // no-hintmap fixed 3MB home window (hard cut)
  uint64_t span_lo;  // inclusive chunk index for fixed windows
  uint64_t span_hi;  // inclusive chunk index for fixed windows
};

static pool_segment
segment_for_hintmap_group(const std::vector<controller_spec>& ordered,
                          std::size_t group_start,
                          std::size_t pat_idx_after_group)
{
  uint64_t seg_lo = 0;
  uint64_t seg_hi = HINTMAP_CHUNK_BITS;

  if (group_start > 0) {
    for (std::size_t j = group_start; j-- > 0;) {
      if (ordered[j].is_fixed) {
        seg_lo = ordered[j].span_hi + 1;
        break;
      }
    }
  }
  if (pat_idx_after_group < ordered.size() && ordered[pat_idx_after_group].is_fixed)
    seg_hi = ordered[pat_idx_after_group].span_lo;

  return {seg_lo, seg_hi};
}

static std::vector<uint64_t>
sorted_pool_chunks(const hintmap_chunk_bits& pool)
{
  std::vector<uint64_t> chunks;
  for (std::size_t c = 0; c < pool.size(); ++c) {
    if (pool.test(c))
      chunks.push_back(static_cast<uint64_t>(c));
  }
  return chunks;
}

// Split pool chunks among k hintmap controllers by cutting at the largest gaps.
static std::vector<chunk_rng>
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

static std::optional<std::size_t>
find_ctrl_index(const std::vector<int>& ctrl_cols, int col)
{
  for (std::size_t i = 0; i < ctrl_cols.size(); ++i) {
    if (ctrl_cols[i] == col)
      return i;
  }
  return std::nullopt;
}

// Redistributes memory chunks among controllers with hard cuts at fixed regions.
// Algorithm:
// 1. Walk controllers in column order, grouping consecutive hintmap controllers
// 2. Fixed controllers create "hard cuts" - their 3MB windows cannot be shared
// 3. For each hintmap group between hard cuts:
//    - Collect available pool chunks that fall before the next hard cut
//    - Use partition_flexible_minimize_holes() to split chunks at largest gaps
//    - Assign resulting ranges to controllers in the group
// 4. Skip pool chunks that fall within fixed controller spans
static std::vector<chunk_rng>
redistribute_with_hard_cuts(const hintmap_chunk_bits& pool,
                            const std::vector<controller_spec>& ordered,
                            const std::vector<int>& ctrl_cols)
{
  if (ctrl_cols.empty())
    return {};

  std::vector<chunk_rng> out(ctrl_cols.size(), {0, 0});

  if (pool.none())
    return out;

  const auto hintmap_count = static_cast<std::size_t>(
      std::count_if(ordered.begin(), ordered.end(),
                    [](const controller_spec& s) { return !s.is_fixed; }));
  const std::size_t fixed_count = ordered.size() - hintmap_count;
  log_info() << "  controllers: " << ordered.size() << " (hintmap=" << hintmap_count
             << ", fixed/no-hintmap=" << fixed_count << ")" << std::endl;

  log_info() << "  hard cuts (no-hintmap fixed 3MB windows):";
  bool any_hard_cut = false;
  for (const auto& spec : ordered) {
    if (!spec.is_fixed)
      continue;
    any_hard_cut = true;
    log_info() << "    [" << spec.span_lo << ", " << (spec.span_hi + 1) << ")" << std::endl;
  }
  if (!any_hard_cut)
    log_info() << "    (none)" << std::endl;

  // Walk pattern in column order; consecutive hintmap slots share one segment;
  // fixed slots are hard cuts; split each segment's pool chunks at largest gaps.
  const std::vector<uint64_t> nums = sorted_pool_chunks(pool);
  std::size_t num_idx = 0;
  std::size_t pat_idx = 0;

  while (pat_idx < ordered.size()) {
    if (ordered[pat_idx].is_fixed) {
      const uint64_t hc_end = ordered[pat_idx].span_hi;
      while (num_idx < nums.size() && nums[num_idx] <= hc_end)
        ++num_idx;
      ++pat_idx;
      continue;
    }

    const std::size_t group_start = pat_idx;
    std::vector<std::size_t> group_ctrl;
    while (pat_idx < ordered.size() && !ordered[pat_idx].is_fixed) {
      if (auto ci = find_ctrl_index(ctrl_cols, ordered[pat_idx].col))
        group_ctrl.push_back(*ci);
      ++pat_idx;
    }

    if (group_ctrl.empty())
      continue;

    uint64_t upper_bound = HINTMAP_CHUNK_BITS;
    if (pat_idx < ordered.size() && ordered[pat_idx].is_fixed)
      upper_bound = ordered[pat_idx].span_lo;

    std::vector<uint64_t> flex_nums;
    while (num_idx < nums.size() && nums[num_idx] < upper_bound) {
      flex_nums.push_back(nums[num_idx]);
      ++num_idx;
    }

    const pool_segment seg = segment_for_hintmap_group(ordered, group_start, pat_idx);
    log_info() << "  segment [" << seg.lo << ", " << seg.hi << ") -> "
               << group_ctrl.size() << " hintmap controller(s), "
               << flex_nums.size() << " pool chunk(s)" << std::endl;

    if (flex_nums.empty())
      continue;

    const auto parts = partition_flexible_minimize_holes(flex_nums, group_ctrl.size());
    for (std::size_t i = 0; i < parts.size() && i < group_ctrl.size(); ++i)
      out[group_ctrl[i]] = parts[i];
  }

  return out;
}

static std::pair<uint64_t, uint64_t>
chunk_rng_to_region(const chunk_rng& r)
{
  const uint64_t n = r.second - r.first;
  return {n ? r.first * CHUNK_SIZE : 0, n * CHUNK_SIZE};
}

static std::string
describe_chunk_span(uint64_t lo, uint64_t hi)
{
  std::ostringstream oss;
  oss << "chunks [" << lo << ".." << hi << "] ("
      << (hi - lo + 1) << " chunks, scratchpad [0x" << std::hex
      << (lo * CHUNK_SIZE) << ", 0x" << ((hi + 1) * CHUNK_SIZE) << ")" << std::dec << ")";
  return oss.str();
}

static std::string
format_byte_size(uint64_t size)
{
  if (size >= BYTES_PER_MB && size % BYTES_PER_MB == 0)
    return std::to_string(size / BYTES_PER_MB) + "MB";
  if (size >= BYTES_PER_KB && size % BYTES_PER_KB == 0)
    return std::to_string(size / BYTES_PER_KB) + "KB";
  return std::to_string(size) + " bytes";
}

static std::string
describe_region(uint64_t base, uint64_t size)
{
  std::ostringstream oss;
  if (size == 0) {
    oss << "NOP (size 0)";
    return oss.str();
  }
  const uint64_t lo = base / CHUNK_SIZE;
  const uint64_t hi = lo + size / CHUNK_SIZE - 1;
  const uint64_t nchunks = hi - lo + 1;
  oss << describe_chunk_span(lo, hi)
      << ", size=0x" << std::hex << size << std::dec
      << " (" << format_byte_size(size) << ", " << nchunks << " chunks)";
  return oss.str();
}

// One capture per directive token; capture 13 is optional args. Handled directives are captures 1–8
// (same eight as asm_directive_id / directive_list). Captures 9–12 are recognized but not dispatched here.
// Order of groups 1–8 must match k_alt_capture_to_handler_idx below.
const regex DIRECTIVE_ALT_RE(
    R"(^(?:(\.attach_to_group)|(\.include)|(\.endl)|(\.setpad)|(\.section)|(\.partition)|(\.target)|(\.aie_row_topology)|(\.align)|(\.long)|(\.eop))(?:[ \t]+(.+))?$)");

// Map DIRECTIVE_ALT_RE capture index 1..8 -> asm_parser::directive_list index (asm_directive_id order).
constexpr std::array<std::size_t, 8> k_alt_capture_to_handler_idx = {
    static_cast<std::size_t>(asm_directive_id::attach_to_group),
    static_cast<std::size_t>(asm_directive_id::include),
    static_cast<std::size_t>(asm_directive_id::endl),
    static_cast<std::size_t>(asm_directive_id::setpad),
    static_cast<std::size_t>(asm_directive_id::section),
    static_cast<std::size_t>(asm_directive_id::partition),
    static_cast<std::size_t>(asm_directive_id::target),
    static_cast<std::size_t>(asm_directive_id::aie_row_topology),
};

} // namespace

bool
asm_parser::
operate_directive(const std::string& line)
{
  smatch m;
  if (!regex_match(line, m, DIRECTIVE_ALT_RE) || m.size() < 13U)  // NOLINT
    return false;

  // match .align, .long, .eop
  if (m[9].matched || m[10].matched || m[11].matched)  //NOLINT
    return false;

  std::size_t idx = asm_directive_count;
  if (m[1].matched)  // NOLINT .attach_to_group
    idx = k_alt_capture_to_handler_idx[0];  // NOLINT
  else if (m[2].matched)   // NOLINT .include
    idx = k_alt_capture_to_handler_idx[1]; // NOLINT
  else if (m[3].matched)   // NOLINT .endl
    idx = k_alt_capture_to_handler_idx[2]; // NOLINT
  else if (m[4].matched)   // NOLINT .setpad
    idx = k_alt_capture_to_handler_idx[3]; // NOLINT
  else if (m[5].matched)   // NOLINT .section
    idx = k_alt_capture_to_handler_idx[4]; // NOLINT
  else if (m[6].matched)   // NOLINT .partition
    idx = k_alt_capture_to_handler_idx[5]; // NOLINT
  else if (m[7].matched)   // NOLINT .target
    idx = k_alt_capture_to_handler_idx[6]; // NOLINT
  else if (m[8].matched)   // NOLINT .aie_row_topology
    idx = k_alt_capture_to_handler_idx[7]; // NOLINT

  if (idx >= asm_directive_count)
    return false;

  const auto& op = directive_list[idx];
  if (!op)
    return false;

  const std::string args_tail = m[12].matched ? m[12].str() : std::string(); // NOLINT
  op->operate(shared_from_this(), line, args_tail);
  return true;
}

void
asm_parser::
insert_col_asmdata(std::shared_ptr<asm_data> data)
{
  // insert asm_data in col list
  if (m_current_col == -1)
    m_current_col = 0;

  if (m_col.find(m_current_col) == m_col.end())
    m_col[m_current_col] = col_data();

  auto& cdata = m_col[m_current_col];
  cdata.ensure_label(m_current_label);
  auto& label_data = cdata.get_label_data();

  if (get_data_state())
    label_data[m_current_label].data.emplace_back(data);
  else
    label_data[m_current_label].text.emplace_back(data);

  auto pagelabel = get_pagelabel(m_current_label);
  m_col[m_current_col].set_labelpageindex(pagelabel, 0);
}

void
asm_parser::
insert_annotation(int annotation_index)
{
  // insert annotation_index in col list
  if (m_current_col == -1)
    throw error(error::error_code::internal_error, "Invalid while adding annotation "
                + std::to_string(m_current_col) + "!!!");

  if (m_col.find(m_current_col) == m_col.end())
    throw error(error::error_code::internal_error, "Col data not find while adding annotation for col"
                + std::to_string(m_current_col) + "!!!");

  auto& label_data = m_col[m_current_col].get_label_data();

  for (auto it = label_data[m_current_label].text.rbegin(); it != label_data[m_current_label].text.rend(); ++it)
  {
    if ((*it)->get_operation().get_name().compare(".eop"))
    {
      (*it)->set_annotation_index(annotation_index);
      break;
    }
  }
}

void
asm_parser::
insert_scratchpad(std::string& name, offset_type size, std::vector<char>& content)
{
  if (m_current_col == -1)
    m_current_col = 0;

  m_col[m_current_col].set_scratchpad(name, size, content);
}

std::vector<uint32_t>
asm_parser::
get_col_list()
{
  // get col list (sorted order since unordered_map doesn't preserve insertion order)
  std::vector<uint32_t> keys;

  std::transform(
    m_col.begin(),
    m_col.end(),
    std::back_inserter(keys),
    [](const std::unordered_map<uint32_t, col_data>::value_type &pair){return pair.first;});
  std::sort(keys.begin(), keys.end());
  return keys;
}

col_data&
asm_parser::
get_col_asmdata(uint32_t colnum)
{
  // get list of asm data for perticular col
  auto it = m_col.find(colnum);
  if (it != m_col.end()) {
    return m_col[colnum];
  } else
    throw error(error::error_code::internal_error, "Key " + std::to_string(colnum)  + " not found!!!");
}

void
asm_parser::
parse_lines(const std::string& source_filename)
{
  directive_list[static_cast<std::size_t>(asm_directive_id::attach_to_group)] =
      std::make_shared<attach_to_group_directive>();
  directive_list[static_cast<std::size_t>(asm_directive_id::aie_row_topology)] =
      std::make_shared<aie_row_topology_directive>();
  directive_list[static_cast<std::size_t>(asm_directive_id::include)] = std::make_shared<include_directive>();
  directive_list[static_cast<std::size_t>(asm_directive_id::endl)] = std::make_shared<end_of_label_directive>();
  directive_list[static_cast<std::size_t>(asm_directive_id::setpad)] = std::make_shared<pad_directive>();
  directive_list[static_cast<std::size_t>(asm_directive_id::section)] = std::make_shared<section_directive>();
  directive_list[static_cast<std::size_t>(asm_directive_id::partition)] = std::make_shared<partition_directive>();
  directive_list[static_cast<std::size_t>(asm_directive_id::target)] = std::make_shared<target_directive>();
  parse_lines(m_data, source_filename);

  // After all parsing is done, inject actual save/restore code
  finalize_preempt();
}

void
asm_parser::
handle_preempt_opcode(std::string& arg_str, std::string& line)
{
  if (get_target_type() == "aie2ps")
    throw error(error::error_code::internal_error, "PREEMPT opcode is not supported for aie2ps target");

  int current_group = current_col();
  record_preempt_label(current_group);

  std::vector<std::string> args;
  if (!arg_str.empty()) {
    static const regex token_re("[^,]+");
    for (aiebu::sregex_iterator it(arg_str.begin(), arg_str.end(), token_re), end;
         it != end; ++it)
      args.push_back(trim(it->str()));
  }

  if (args.empty())
    throw error(error::error_code::internal_error, "PREEMPT opcode has no arguments");

  std::string first_arg = args[0];
  if (first_arg.empty())
    throw error(error::error_code::internal_error, "PREEMPT opcode has empty first argument");

  std::string hintmap_label;
  if (args.size() >= 4) {
    hintmap_label = args[3];
    if (!hintmap_label.empty() && hintmap_label[0] == '@')
      hintmap_label = hintmap_label.substr(1);
    if (!hintmap_label.empty()) {
      std::transform(hintmap_label.begin(), hintmap_label.end(), hintmap_label.begin(),
                     [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    }
  }

  std::pair<std::string, std::string> labels;
  std::string qualified_key = "";
  if (!hintmap_label.empty()) {
    // Qualify the hintmap key with its label scope so that the same label
    // name in different scopes (e.g. "default" vs "default:pdi") is treated
    // as a distinct hintmap.
    qualified_key = m_current_label + ":" + hintmap_label;
    // Get unique save/restore labels for this hintmap BEFORE adding to vector
    // (so the index calculation is correct)
    labels = get_hintmap_save_restore_labels(hintmap_label, current_group);
    // Store qualified key for later processing (after getting labels)
    m_preempt_hintmaps[current_group].push_back(qualified_key);
  } else {
    // No hintmap, use group-level labels
    // Track that this group has PREEMPT opcodes without hintmaps
    m_preempt_without_hintmap.insert(current_group);
    labels = m_preempt_labels[current_group];
  }

  m_preempt_points[current_group].push_back({first_arg, qualified_key});

  if (!hintmap_label.empty())
    arg_str = first_arg + ", @" + labels.first + ", @" + labels.second + ", @" + hintmap_label;
  else
    arg_str = first_arg + ", @" + labels.first + ", @" + labels.second;

  log_info() << "PREEMPT opcode: updated arg_str to '" << arg_str
             << "' (hintmap: '" << hintmap_label << "', labels: @" << labels.first
             << " / @" << labels.second << ")" << std::endl;

  line = "preempt\t" + arg_str;
}

void
asm_parser::
handle_load_or_preempt_cond(const std::string& op_name, const std::string& arg_str, const smatch& sm)
{
  // col is common to all four opcodes; compute once.
  int col = current_col();

  if (sm[1].matched || sm[3].matched) {  // load_pdi or load_cores
    // operator[] default-constructs col_data if the key is absent (single lookup).
    auto& cdata = m_col[col];
    bool is_load_pdi = sm[1].matched;

    // Extract the @label at argument index 1 (<id>, @<label>).
    // Compiled once (static); regex_search finds the first ", @<label>" token.
    static const regex LOAD_LABEL_RE(",\\s*(@[a-zA-Z0-9_]+)");
    std::string label_arg;
    smatch lm;
    if (regex_search(arg_str, lm, LOAD_LABEL_RE))
      label_arg = lm[1].str();
    // Enforce uniqueness of the PDI / core-elf address within this column.
    // label_arg will never be empty
    bool inserted = is_load_pdi
                      ? cdata.try_add_load_pdi_label(label_arg)
                      : cdata.try_add_load_cores_label(label_arg);
    if (!inserted)
      throw error(error::error_code::invalid_asm,
        op_name + " location '" + label_arg + "' is not unique in column " +
        std::to_string(col) + "; each " + op_name +
        " in a control code elf must use a distinct address\n");

    if (is_load_pdi)
      cdata.increment_load_pdi_count();
    else
      cdata.increment_load_cores_count();
  } else if (sm[2].matched) {  // load_cores_cp
    m_col[col].increment_load_cores_cp_count();
  } else {
    // start_cond_job_preempt must only appear after at least one preempt in
    // the same column (cert uses the preceding preempt point for recovery).
    auto col_it = m_col.find(col);
    if (col_it == m_col.end() || col_it->second.get_preempt_count() == 0)
      throw error(error::error_code::invalid_asm,
        "start_cond_job_preempt found in column " + std::to_string(col) +
        " before any preempt opcode; it must follow a preempt opcode\n");
    col_it->second.increment_start_cond_job_preempt_count();
  }
}

void
asm_parser::
parse_lines(const std::vector<char>& data, const std::string& file)
{
  //parse asm code
  const static regex COMMENT_REGEX("^;(.*)$");
  const static regex LABEL_REGEX("^([a-zA-Z0-9_]+):$");
  const static regex OP_REGEX("^([.a-zA-Z0-9_]+)(?:[ \\t]+(.+))?$");
  // Groups: [1]=load_pdi  [2]=load_cores_cp  [3]=load_cores  [4]=start_cond_job_preempt
  const static regex LOAD_OR_PREEMPT_COND_RE("^(?:(load_pdi)|(load_cores_cp)|(load_cores)|(start_cond_job_preempt))$");

  // Sanitize input data: filter out non-printable characters except newline, tab, and carriage return
  // This prevents corrupted operation names during parsing
  std::string str;
  str.reserve(data.size());
  for (char c : data) {
    uint8_t byte = static_cast<uint8_t>(c);
    // Only allow printable ASCII (32-126), newline (10), tab (9), and carriage return (13)
    if ((byte >= 32 && byte <= 126) || byte == '\n' || byte == '\t' || byte == '\r') {
      str += c;
    }
  }
  const uint32_t parse_file_idx = intern_filename(file);
  m_current_parse_file_idx = parse_file_idx;
  // Scan str for newlines directly instead of copying it into an istringstream
  size_t pos = 0;
  const size_t str_len = str.size();
  auto read_next_line = [&](std::string& out) -> bool {
    if (pos > str_len) return false;
    const size_t nl = str.find('\n', pos);
    if (nl == std::string::npos) {
      if (pos >= str_len) return false;
      out.assign(str, pos, str_len - pos);
      pos = str_len + 1;
    } else {
      out.assign(str, pos, nl - pos);
      pos = nl + 1;
    }
    return true;
  };
  std::string line;
  uint32_t linenumber = 0;

  while (read_next_line(line)) {
    ++linenumber;
    line = trim(line);
    if(line.empty())
      continue;

    if (line[0] == ';') //regex_match(line, COMMENT_REGEX))
      continue;

    smatch sm;

    if (line[0] == '.' &&  line.substr(0,5).compare(".long") && operate_directive(line))
    {
      if (!get_annotation_state())
        continue;
      std::string aline, id_line, name_line, description_line;
      // there are 3 lines for annotation data (id,name,description)
      for (int count = 0 ; count < 3 ; ++count)   // NOLINT
      {
        read_next_line(aline);
        aline = trim(aline);
        if (!aline.substr(0,3).compare("id:"))                 // NOLINT
          id_line = trim(aline.substr(aline.find(":") + 1));   // NOLINT
        else if (!aline.substr(0,5).compare("name:"))          // NOLINT
          name_line = trim(aline.substr(aline.find(":") + 1)); // NOLINT
        else if (!aline.substr(0,12).compare("description:"))  // NOLINT
          description_line = trim(aline.substr(aline.find(":") + 1)); // NOLINT
        else
          throw error(error::error_code::internal_error, "Unknown annotation field " + aline + "!!!");
      }
      m_annotation_list.emplace_back(id_line, name_line, description_line);
      insert_annotation(static_cast<int>(m_annotation_list.size() - 1));
      reset_annotation_state();
      linenumber+=3;
      continue;
    }

    // check for label - fast check for ':' at end before regex
    if (line.back() == ':' && regex_match(line, sm, LABEL_REGEX))
    {
      if (!get_data_state()) {
        m_current_label = m_current_label + ":" + sm[1].str();
        set_data_state(false);
      } else
        insert_col_asmdata(std::make_shared<asm_data>(operation(sm[1].str(), ""), operation_type::label,
                                                      code_section::unknown, 0, (uint32_t)-1, linenumber,
                                                      parse_file_idx, is_save_restore_routine()));

      continue;
    }
    // check for operation
    else if (regex_match(line, sm, OP_REGEX))
    {
      std::string arg_str = (sm.size() > 2 && sm[2].matched) ? sm[2].str() : "";
      std::string op_name = sm[1].str();
      std::transform(op_name.begin(), op_name.end(), op_name.begin(), ::tolower);

      // Handle PREEMPT opcode - record label for current group
      if (!op_name.compare("preempt")) {
        handle_preempt_opcode(arg_str, line);
      }
      // Per-opcode parse-time checks and counter updates.
      // These run after the preempt block so that m_preempt_count is already
      // incremented when we reach start_cond_job_preempt.
      else if (regex_match(op_name, sm, LOAD_OR_PREEMPT_COND_RE)) {
        handle_load_or_preempt_cond(op_name, arg_str, sm);
      }

      insert_col_asmdata(std::make_shared<asm_data>(operation(op_name, arg_str), operation_type::op,
                                                    code_section::unknown, 0, (uint32_t)-1, linenumber,
                                                    parse_file_idx, is_save_restore_routine()));
      if (!op_name.compare("eof"))
        set_data_state(true);
    }
  }

}

std::pair<std::vector<std::string>, std::vector<std::string>>
asm_parser::
get_preempt_save_restore_membd(uint32_t num_cols) const
{
  // Key    Save Label   Restore Label
  //
  // 1 (1c)      save_1:     restore_1:
  // 2 (2c)      save_1:     restore_1:
  // 3 (3c)      save_1:     restore_1:
  // 10 (1c0)    save_1:     restore_1:
  // 12 (1c1)    save_2:     restore_2:
  // 14 (1c2)    save_3:     restore_3:
  auto& save_restore_map = get_aie4_save_restore_membd();
  auto it = save_restore_map.find(num_cols);
  if (it != save_restore_map.end())
    return it->second;
  return {{}, {}};
}

std::pair<std::vector<std::string>, std::vector<std::string>>
asm_parser::
get_preempt_save_restore_shimbd(uint32_t num_cols) const
{
  // Key    Save Label   Restore Label
  //
  // 1 (1c)      save_1:     restore_1:
  // 2 (2c)      save_1:     restore_1:
  // 3 (3c)      save_1:     restore_1:
  // 10 (1c0)    save_1:     restore_1:
  // 12 (1c1)    save_2:     restore_2:
  // 14 (1c2)    save_3:     restore_3:
  auto& save_restore_map = get_aie4_save_restore_shimbd();
  auto it = save_restore_map.find(num_cols);
  if (it != save_restore_map.end())
    return it->second;
  return {{}, {}};
}

std::pair<std::vector<uint8_t>, std::vector<uint8_t>>
asm_parser::
get_preempt_save_restore(uint32_t num_cols) const
{
  // Key    Save Label   Restore Label
  //
  // 1 (1c)      save_1:     restore_1:
  // 2 (2c)      save_1:     restore_1:
  // 3 (3c)      save_1:     restore_1:
  // 10 (1c0)    save_1:     restore_1:
  // 12 (1c1)    save_2:     restore_2:
  // 14 (1c2)    save_3:     restore_3:
  const auto* save_restore = get_aie4_save_restore(num_cols);
  if (!save_restore)
    return {{}, {}};

  return {
    {save_restore->save.data, save_restore->save.data + save_restore->save.size},
    {save_restore->restore.data, save_restore->restore.data + save_restore->restore.size}
  };
}


// ---------------------------------------------------------------------------
// collect_hintmap_words
//   Scan entries inside 'label_context' and return all .long values that
//   appear after the 'hintmap_label' definition until the next label.
// ---------------------------------------------------------------------------
static std::vector<uint32_t>
collect_hintmap_words(const std::vector<std::shared_ptr<asm_data>>& entries,
                      const std::string& hintmap_label)
{
  std::vector<uint32_t> words;
  words.reserve(16);
  bool in_target = false;

  for (const auto& entry : entries) {
    if (entry->isLabel()) {
      const auto& op = entry->get_operation();
      if (op.get_name() == hintmap_label) {
        in_target = true;
        continue;
      }
      if (in_target)
        break;  // next label ends the hintmap block
      continue;
    }

    if (!in_target) continue;

    const auto& op = entry->get_operation();
    if (op.get_name() != ".long") continue;

    for (const auto& arg : op.get_args()) {
      const auto t = trim(arg);
      if (t.empty()) continue;
      try {
        const uint32_t val = (t.find("0x") == 0 || t.find("0X") == 0)
                             ? static_cast<uint32_t>(std::stoul(t, nullptr, 16))
                             : static_cast<uint32_t>(std::stoul(t, nullptr, 10));
        words.push_back(val);
      } catch (const std::exception&) {
        log_warn() << "Failed to parse hintmap value: " << arg << std::endl;
      }
    }
  }

  if (!in_target)
    log_warn() << "Hintmap label '" << hintmap_label
               << "' not found in provided entries" << std::endl;

  return words;
}

// ---------------------------------------------------------------------------
// hintmap_words_to_scratchpad
//   Interpret the .long bitmask words and return {scratchbase, size}.
//   Each bit represents one 64KB chunk.  The set bits need not be contiguous:
//   the region spans the first up to the last set bit so that every requested
//   chunk is saved.  Chunks sitting in a gap are saved as well (a superset of
//   the hintmap), since one preemption point transfers a single contiguous range.
// ---------------------------------------------------------------------------
static std::pair<uint64_t, uint64_t>
hintmap_words_to_scratchpad(const std::vector<uint32_t>& words,
                            const std::string& hintmap_label,
                            int group)
{
  constexpr uint64_t DEFAULT_SIZE = 9ULL * 1024ULL * 1024ULL; // 9MB
  constexpr uint64_t DEFAULT_BASE = 0x0ULL;
  constexpr uint64_t NO_BIT       = UINT64_MAX;

  if (words.empty()) {
    log_warn() << "No hintmap data for label '" << hintmap_label
               << "', using defaults" << std::endl;
    return {DEFAULT_BASE, DEFAULT_SIZE};
  }

  uint64_t first_bit = NO_BIT, last_bit = NO_BIT, set_bits = 0;

  for (std::size_t idx = 0; idx < words.size(); ++idx) {
    const uint32_t w = words[idx];
    if (w == 0) continue;

    set_bits += static_cast<uint64_t>(aiebu_popcount(w));

    const uint64_t lo = idx * 32ULL + static_cast<uint64_t>(aiebu_ctz(w));
    if (first_bit == NO_BIT) first_bit = lo;

    const uint64_t hi = idx * 32ULL + static_cast<uint64_t>(31 - aiebu_clz(w));
    last_bit = hi;
  }

  // Full span from first to last set bit, inclusive.  Holes are absorbed.
  const uint64_t span_bits   = (first_bit != NO_BIT) ? (last_bit - first_bit + 1) : 0;
  const uint64_t scratchbase = (first_bit != NO_BIT) ? (first_bit * CHUNK_SIZE) : DEFAULT_BASE;
  const uint64_t size        = span_bits * CHUNK_SIZE;

  if (first_bit != NO_BIT && span_bits != set_bits) {
    log_info() << "hintmap '" << hintmap_label << "' has gaps between bit "
               << first_bit << " and bit " << last_bit << ": saving "
               << span_bits << " chunks to cover the " << set_bits
               << " requested ones" << std::endl;
  }

  log_info() << "Hintmap parsed for group " << group << " (col " << group << "): "
             << "scratchbase=0x" << std::hex << scratchbase
             << ", size=0x"      << size << " (" << std::dec
             << (size / BYTES_PER_MB) << "MB, " << span_bits << " chunks)" << std::endl;

  return {scratchbase, size};
}

// ---------------------------------------------------------------------------
// find_hintmap_context
//   Return the label-scope name that contains 'hintmap_label' inside column
//   'group'.  If search_context is non-empty only that scope is checked.
//   Throws if the label is not found.
// ---------------------------------------------------------------------------
std::string
asm_parser::
find_hintmap_context(int group,
                     const std::string& search_context,
                     const std::string& hintmap_label)
{
  const auto& label_data = get_col_asmdata(static_cast<uint32_t>(group)).get_label_data();

  for (const auto& [lname, section_data] : label_data) {
    if (!search_context.empty() && lname != search_context)
      continue;
    const auto it = std::find_if(section_data.data.cbegin(), section_data.data.cend(),
      [&hintmap_label](const auto& entry) {
        if (!entry->isLabel()) return false;
        const auto& op = entry->get_operation();
        return op.get_name() == hintmap_label;
      });
    if (it != section_data.data.cend())
      return lname;
  }

  throw error(error::error_code::internal_error,
              "hintmap label '" + hintmap_label + "' not found in column "
              + std::to_string(group)
              + (search_context.empty() ? "" : " (scope '" + search_context + "')"));
}

// ---------------------------------------------------------------------------
// parse_hintmap_and_calculate_scratchpad
//   Each bit represents a 64KB chunk; scratchbase = first set bit * 64KB,
//   size = (last set bit - first set bit + 1) * 64KB.  Holes between the
//   first and last set bits are absorbed into the scratchpad.  Returns
//   defaults when hintmap_label is empty.
// ---------------------------------------------------------------------------
std::pair<uint64_t, uint64_t>
asm_parser::
parse_hintmap_and_calculate_scratchpad(int group,
                                       const std::string& search_context,
                                       const std::string& hintmap_label)
{
  constexpr uint64_t DEFAULT_SIZE = 9ULL * 1024ULL * 1024ULL;
  constexpr uint64_t DEFAULT_BASE = 0x0ULL;
  constexpr uint64_t DEFAULT_SIZE_PER_COL = 3ULL * 1024ULL * 1024ULL;

  if (hintmap_label.empty()) {
    if (is_multi_column_mode())
      return {DEFAULT_SIZE_PER_COL*group/2, DEFAULT_SIZE_PER_COL};
    return {DEFAULT_BASE, DEFAULT_SIZE};
  }

  const std::string ctx     = find_hintmap_context(group, search_context, hintmap_label);
  auto& col_data            = get_col_asmdata(static_cast<uint32_t>(group));
  const auto& all_entries   = col_data.get_label_asmdata_data(ctx);
  const auto words          = collect_hintmap_words(all_entries, hintmap_label);
  const uint64_t max_chunks = static_cast<uint64_t>(get_partition_info()->get_numcolumn()) * CHUNKS_PER_COL;
  verify_hintmap_chunk_limit(words_to_bitset(words), max_chunks, hintmap_label);
  return hintmap_words_to_scratchpad(words, hintmap_label, group);
}

std::vector<char>
asm_parser::
replace_save_restore_labels(const std::vector<uint8_t>& template_data,
                            const std::string& old_save_label,
                            const std::string& old_restore_label,
                            const std::string& new_save_label,
                            const std::string& new_restore_label)
{
  // Convert template data to string for replacement
  // Filter out non-printable characters that might cause parsing issues, except for newlines, tabs, and carriage returns
  std::string template_str;
  template_str.reserve(template_data.size());
  for (uint8_t byte : template_data) {
    // Only allow printable ASCII (32-126), newline (10), tab (9), and carriage return (13)
    // This ensures we don't include null bytes or other control characters that corrupt parsing
    if ((byte >= 32 && byte <= 126) || byte == '\n' || byte == '\t' || byte == '\r') {
      template_str += static_cast<char>(byte);
    }
  }

  // Replace label definitions (e.g., "save_1:" -> "save_1_0:")
  std::string old_save_label_def = old_save_label + ":";
  std::string new_save_label_def = new_save_label + ":";
  std::string old_restore_label_def = old_restore_label + ":";
  std::string new_restore_label_def = new_restore_label + ":";

  // Replace label references (e.g., "@save_1" -> "@save_1_0")
  std::string old_save_label_ref = "@" + old_save_label;
  std::string new_save_label_ref = "@" + new_save_label;
  std::string old_restore_label_ref = "@" + old_restore_label;
  std::string new_restore_label_ref = "@" + new_restore_label;

  // Replace .endl directives (e.g., ".endl save_1" -> ".endl save_1_0")
  std::string old_save_endl = ".endl " + old_save_label;
  std::string new_save_endl = ".endl " + new_save_label;
  std::string old_restore_endl = ".endl " + old_restore_label;
  std::string new_restore_endl = ".endl " + new_restore_label;

  // Perform replacements - order matters: do .endl first to avoid partial matches
  size_t pos = 0;
  while ((pos = template_str.find(old_save_endl, pos)) != std::string::npos) {
    template_str.replace(pos, old_save_endl.length(), new_save_endl);
    pos += new_save_endl.length();
  }

  pos = 0;
  while ((pos = template_str.find(old_restore_endl, pos)) != std::string::npos) {
    template_str.replace(pos, old_restore_endl.length(), new_restore_endl);
    pos += new_restore_endl.length();
  }

  pos = 0;
  while ((pos = template_str.find(old_save_label_def, pos)) != std::string::npos) {
    template_str.replace(pos, old_save_label_def.length(), new_save_label_def);
    pos += new_save_label_def.length();
  }

  pos = 0;
  while ((pos = template_str.find(old_restore_label_def, pos)) != std::string::npos) {
    template_str.replace(pos, old_restore_label_def.length(), new_restore_label_def);
    pos += new_restore_label_def.length();
  }

  pos = 0;
  while ((pos = template_str.find(old_save_label_ref, pos)) != std::string::npos) {
    template_str.replace(pos, old_save_label_ref.length(), new_save_label_ref);
    pos += new_save_label_ref.length();
  }

  pos = 0;
  while ((pos = template_str.find(old_restore_label_ref, pos)) != std::string::npos) {
    template_str.replace(pos, old_restore_label_ref.length(), new_restore_label_ref);
    pos += new_restore_label_ref.length();
  }

  // Final sanitization pass: remove any remaining non-printable characters
  // (in case replacements introduced any issues)
  std::string sanitized_str;
  sanitized_str.reserve(template_str.size());
  for (char c : template_str) {
    uint8_t byte = static_cast<uint8_t>(c);
    if ((byte >= 32 && byte <= 126) || byte == '\n' || byte == '\t' || byte == '\r') {
      sanitized_str += c;
    }
  }

  // Convert back to vector<char>
  return std::vector<char>(sanitized_str.begin(), sanitized_str.end());
}

// ---------------------------------------------------------------------------
// File-local helpers
// ---------------------------------------------------------------------------

// Map multi-column group number to save/restore file suffix.
static std::string multicol_suffix(int group)
{
  if (group == 0) return "1c0";
  if (group == 2) return "1c1";
  if (group == 4) return "1c2";
  return "1c" + std::to_string(group);
}

// Trim whitespace then strip a trailing comma from an argument string.
static std::string clean_arg(const std::string& arg)
{
  std::string s = trim(arg);
  if (!s.empty() && s.back() == ',')
    s.pop_back();
  return s;
}

// Trim, strip trailing comma, then strip a leading '@' (for label refs).
static std::string clean_label_ref(const std::string& arg)
{
  std::string s = clean_arg(arg);
  if (!s.empty() && s.front() == '@')
    s = s.substr(1);
  return s;
}

// Divide [address, address+size) into equal number of slots.
// Returns one (base, size) pair per slot; the last slot absorbs any remainder from integer division.
static std::vector<std::pair<uint64_t, uint64_t>>
compute_bd_ranges(uint32_t num_bd_per_column, uint64_t address, uint64_t size, uint32_t parts_per_region)
{
  uint32_t total = num_bd_per_column * parts_per_region;
  uint64_t part_size = size / static_cast<uint64_t>(total);

  std::vector<std::pair<uint64_t, uint64_t>> result;
  result.reserve(total);

  for (uint32_t i = 0; i < total; ++i) {
    uint64_t base = address + static_cast<uint64_t>(i) * part_size;
    // last slot absorbs remainder
    uint64_t this_size = (i == total - 1) ? (address + size - base) : part_size;
    result.push_back({base, this_size});
  }

  return result;
}

// 6 BD pairs for save  (2 equal halves per 3MB region)
static std::vector<std::pair<uint64_t, uint64_t>>
compute_save_bd_ranges(uint32_t num_save_bd, uint64_t address, uint64_t size)
{
  uint32_t s2mm_channel_per_col = 2;
  return compute_bd_ranges(num_save_bd/s2mm_channel_per_col, address, size, s2mm_channel_per_col);
}

// 12 BD pairs for restore  (4 equal quarters per 3MB region)
static std::vector<std::pair<uint64_t, uint64_t>>
compute_restore_bd_ranges(uint32_t num_restore_bd, uint64_t address, uint64_t size)
{
  uint32_t mm2s_channel_per_col = 4;
  return compute_bd_ranges(num_restore_bd/mm2s_channel_per_col, address, size, mm2s_channel_per_col);
}

// ---------------------------------------------------------------------------
// patch_bd_in_asm
//   In 'text', find 'label:' and overwrite the first three .long values with
//   the DMA BD register encoding for (byte_addr, size_bytes):
//     Word 0 (DMA_BD_0): bits[24:0] = byte_addr[56:32]  (Base_Address_High)
//     Word 1 (DMA_BD_1): bits[31:2] = byte_addr[31:2]   (Base_Address_Low)
//     Word 2 (DMA_BD_2): bits[31:0] = size_bytes / 4    (Buffer_Length in words)
// ---------------------------------------------------------------------------
static void
patch_bd_in_asm(std::string& text, const std::string& label,
                uint64_t byte_addr, uint64_t size_bytes)
{
  const std::string label_def = label + ":";
  auto pos = text.find(label_def);
  if (pos == std::string::npos)
    return;

  const uint32_t bd_vals[3] = {
    static_cast<uint32_t>((byte_addr >> 32) & 0x1FFFFFFU), // BD0_0: Base_Address_High
    static_cast<uint32_t>(byte_addr & 0xFFFFFFFCU),         // BD0_1: Base_Address_Low
    static_cast<uint32_t>(size_bytes / 4U)                  // BD0_2: Buffer_Length (words)
  };
  // find the .long instruction after the label
  auto search_pos = pos + label_def.size();
  for (int i = 0; i < 3; ++i) {
    auto long_pos = text.find(".long", search_pos);
    if (long_pos == std::string::npos)
      return;

    // Skip ".long" keyword and leading whitespace
    auto val_start = long_pos + 5;
    while (val_start < text.size() &&
           (text[val_start] == ' ' || text[val_start] == '\t'))
      ++val_start;

    // Find end of the hex token (stop at whitespace, comment, or newline)
    auto val_end = val_start;
    while (val_end < text.size() &&
           text[val_end] != '\n' && text[val_end] != '\r' &&
           text[val_end] != ' '  && text[val_end] != '\t' &&
           text[val_end] != ';')
      ++val_end;
    // print old and new
    {
    std::string old_val = text.substr(val_start, val_end - val_start);
    std::ostringstream oss;
    oss << "0x" << std::hex << std::uppercase
        << std::setw(8) << std::setfill('0') << bd_vals[i];
    std::string new_val = oss.str();
    log_info() << "patch_bd_in_asm: label=" << label
               << " idx=" << i
               << " old=" << old_val
               << " new=" << new_val << std::endl;
    }
    std::ostringstream oss;
    oss << "0x" << std::hex << std::uppercase
        << std::setw(8) << std::setfill('0') << bd_vals[i];
    text.replace(val_start, val_end - val_start, oss.str());

    search_pos = long_pos + 5; // advance past this .long
  }
}

static void
patch_membd_in_asm(std::string& text, const std::string& label,
                uint64_t byte_addr, uint64_t size_bytes, int group_index)
{
  const std::string label_def = label + ":";
  auto pos = text.find(label_def);
  if (pos == std::string::npos)
    return;

  log_info() << "group_index: " << group_index << " byte_addr=0x" << std::hex << byte_addr  << std::dec << std::endl;
/*
  //6.3.4 Inter-MEM tile memory and lock access
  Offset -> Address ranges (bytes)
  -2     -> 0x1A0_0000 – 0x1CF_FFFF  ---> 0x68_0000 - 0x73_ffff
  -1     -> 0x1D0_0000 – 0x1FF_FFFF  ---> 0x74_0000 - 0x7f_ffff
  0      -> 0x200_0000 – 0x22F_FFFF  ---> 0x80_0000 - 0x8b_ffff
  +1     -> 0x230_0000 – 0x25F_FFFF  ---> 0x8c_0000 - 0x97_ffff
  +2     -> 0x260_0000 – 0x28F_FFFF  ---> 0x98_0000 - 0xA3_ffff
*/

  // Translate physical scratchpad byte_addr to the inter-MEM tile word address seen by group_index col.
  //
  // The inter-MEM tile byte address table (relative to the channel's home col = slot 0):
  //   slot -2 -> 0x1A0_0000,  slot -1 -> 0x1D0_0000,  slot 0 -> 0x200_0000
  //   slot +1 -> 0x230_0000,  slot +2 -> 0x260_0000   (step = 0x30_0000 bytes per slot)
  //
  // Converting to word address (/4):
  //   word_base = 0x200_0000/4 = 0x80_0000,  word_step = 0x30_0000/4 = 0xC_0000
  //
  // Physical layout: col N occupies [N*0x300000, (N+1)*0x300000) of scratchpad.
  // slot = (which col the address is in) - (home col of this channel)
  {
    int slot = static_cast<int>(byte_addr / 0x300000) - group_index;  // NOLINT
    byte_addr = static_cast<uint64_t>(0x800000 + slot * 0xC0000) + (byte_addr % 0x300000) / 4;  // NOLINT
  }
  log_info() << "patch_membd_in_asm: label=" << label
             << " byte_addr=0x" << std::hex << byte_addr
             << " size_bytes=0x" << size_bytes/4 << std::dec << std::endl;
  const uint32_t bd_vals[3] = {
    static_cast<uint32_t>(byte_addr & 0xFFFFFFFFU),         // Base_Address
    static_cast<uint32_t>(size_bytes / 4U)                  // BD0_2: Buffer_Length (words)
  };
  // find the .long instruction after the label
  auto search_pos = pos + label_def.size();
  for (int i = 0; i < 2; ++i) {
    auto long_pos = text.find(".long", search_pos);
    if (long_pos == std::string::npos)
      return;

    // Skip ".long" keyword and leading whitespace
    auto val_start = long_pos + 5;
    while (val_start < text.size() &&
           (text[val_start] == ' ' || text[val_start] == '\t'))
      ++val_start;

    // Find end of the hex token (stop at whitespace, comment, or newline)
    auto val_end = val_start;
    while (val_end < text.size() &&
           text[val_end] != '\n' && text[val_end] != '\r' &&
           text[val_end] != ' '  && text[val_end] != '\t' &&
           text[val_end] != ';')
      ++val_end;
    // print old and new
    {
    std::string old_val = text.substr(val_start, val_end - val_start);
    std::ostringstream oss;
    oss << "0x" << std::hex << std::uppercase
        << std::setw(8) << std::setfill('0') << bd_vals[i];
    std::string new_val = oss.str();
    log_info() << "patch_membd_in_asm: label=" << label
               << " idx=" << i
               << " old=" << old_val
               << " new=" << new_val << std::endl;
    }
    std::ostringstream oss;
    oss << "0x" << std::hex << std::uppercase
        << std::setw(8) << std::setfill('0') << bd_vals[i];
    text.replace(val_start, val_end - val_start, oss.str());

    search_pos = long_pos + 5; // advance past this .long
  }
}

// ---------------------------------------------------------------------------
// split_qualified_hintmap
//   Split "label_context:hintmap_name" into {context, name}.  The context
//   itself can contain ':' (e.g. "default:pdi"), so the last ':' separates.
// ---------------------------------------------------------------------------
static std::pair<std::string, std::string>
split_qualified_hintmap(const std::string& qualified_key)
{
  const auto sep = qualified_key.rfind(':');
  if (sep == std::string::npos)
    return {"", qualified_key};  // no context prefix (shouldn't happen)
  return {qualified_key.substr(0, sep), qualified_key.substr(sep + 1)};
}

// ---------------------------------------------------------------------------
// build_hintmap_groups
//   First pass: group hintmaps by (scratchbase, size) and assign unique labels.
// ---------------------------------------------------------------------------
std::vector<asm_parser::hintmap_group_entry>
asm_parser::build_hintmap_groups(int group, int group_index)
{
  // Map (scratchbase, size) -> index in result vector
  std::map<std::pair<uint64_t,uint64_t>, std::size_t> key_to_idx;
  std::vector<hintmap_group_entry> result;
  int unique_idx = 0;

  const auto points_it = m_preempt_points.find(group);
  if (points_it == m_preempt_points.end())
    return result;

  const auto& points = points_it->second;
  const auto& regions  = m_preempt_region[group];

  for (std::size_t pt = 0; pt < points.size(); ++pt) {
    const auto& qualified_key = points[pt].hintmap_key;
    if (qualified_key.empty())
      continue;

    uint64_t scratchbase = 0;
    uint64_t size        = 0;
    if (pt < regions.size()) {
      scratchbase = regions[pt].scratchbase;
      size        = regions[pt].size;
    } else {
      auto [ctx, label_name] = split_qualified_hintmap(qualified_key);
      auto region = parse_hintmap_and_calculate_scratchpad(group, ctx, label_name);
      scratchbase = region.first;
      size        = region.second;
    }

    auto key = std::make_pair(scratchbase, size);
    auto it  = key_to_idx.find(key);

    if (it == key_to_idx.end()) {
      hintmap_group_entry entry;
      entry.scratchbase = scratchbase;
      entry.size        = size;
      entry.hintmap_pts.emplace_back(pt, qualified_key);
      entry.labels = {"save_"    + std::to_string(group_index) + "_" + std::to_string(unique_idx),
                      "restore_" + std::to_string(group_index) + "_" + std::to_string(unique_idx)};
      log_info() << "Column " << group << ": preemption point " << pt << ", hintmap '"
                 << qualified_key << "' -> scratchbase=0x" << std::hex << scratchbase
                 << ", size=0x" << size << std::dec
                 << " -> new labels @" << entry.labels.first
                 << " / @" << entry.labels.second << std::endl;
      key_to_idx[key] = result.size();
      result.push_back(std::move(entry));
      ++unique_idx;
    } else {
      auto& entry = result[it->second];
      const auto pt_key = std::make_pair(pt, qualified_key);
      if (std::find(entry.hintmap_pts.begin(), entry.hintmap_pts.end(), pt_key)
          == entry.hintmap_pts.end())
        entry.hintmap_pts.push_back(pt_key);
      log_info() << "Column " << group << ": preemption point " << pt << ", hintmap '"
                 << qualified_key << "' -> sharing labels @" << entry.labels.first
                 << " / @" << entry.labels.second << std::endl;
    }
  }
  return result;
}

constexpr uint32_t save_channel    = 2;  // number of mm2s memtile channel per col used
constexpr uint32_t restore_channel = 4;  // number of s2mm memtile channel per col used
// ---------------------------------------------------------------------------
// inject_hintmap_save_restore
//   Register scratchpad, patch template labels, and parse save+restore asm.
// ---------------------------------------------------------------------------
void
asm_parser::inject_hintmap_save_restore(int col, int group_index,
                                        const std::string& save_file,
                                        const std::string& restore_file,
                                        const std::vector<uint8_t>& save_data,
                                        const std::vector<uint8_t>& restore_data,
                                        const std::pair<std::string,std::string>& template_labels,
                                        const hintmap_group_entry& grp,
                                        const std::vector<std::string>& save_bd,
                                        const std::vector<std::string>& restore_bd,
                                        const std::vector<std::string>& save_membd,
                                        const std::vector<std::string>& restore_membd)
{
  // Bind all hintmaps in this group to the shared labels.
  // Key includes the column so that identical qualified names in different
  // columns never collide in the map.
  const std::string col_prefix = std::to_string(col) + ":";
  for (const auto& [pt, hm] : grp.hintmap_pts)
    m_hintmap_labels[col_prefix + hm + ":" + std::to_string(pt)] = grp.labels;

  // When hint_bitmap is all-zero the scratchpad size is 0: no state needs to
  // be saved or restored.  Inject minimal dummy jobs so the PREEMPT opcode
  // still has valid save/restore label targets, but skip all BD configuration
  // and stream-switch routing that the full templates would generate.
  if (grp.size == 0) {
    const std::string& slabel = grp.labels.first;
    const std::string& rlabel = grp.labels.second;
    log_info() << "hint_bitmap is 0 for col " << col
               << ": injecting dummy save/restore jobs (labels @"
               << slabel << " / @" << rlabel << ")" << std::endl;

    const std::string save_asm =
        slabel + ":\n"
        "START_JOB 0\n"
        "NOP\n"
        "END_JOB\n"
        "EOF\n"
        "\n.endl " + slabel + "\n";

    const std::string restore_asm =
        rlabel + ":\n"
        "START_JOB 0\n"
        "LOAD_LAST_PDI\n"
        "END_JOB\n"
        "EOF\n"
        "\n.endl " + rlabel + "\n";

    m_current_col = col;
    std::string dummy_save_file    = "0_0_" + save_file;
    std::string dummy_restore_file = "0_0_" + restore_file;
    log_info() << "Adding dummy save_file: " << dummy_save_file << " [size: " << save_asm.size()
               << "], restore_file: " << dummy_restore_file << " [size: " << restore_asm.size() << "]" << std::endl;
    set_data_state(false);
    parse_lines(std::vector<char>(save_asm.begin(), save_asm.end()),
                dummy_save_file);
    pop_data_state();

    set_data_state(false);
    parse_lines(std::vector<char>(restore_asm.begin(), restore_asm.end()),
                dummy_restore_file);
    pop_data_state();
    return;
  }

  // Modify save_file like {scratchaddress}_{size}_aie4_save_3c.asm / {scratchaddress}_{size}_aie4_restore_3c.asm
  std::string save_file_mod = std::to_string(grp.scratchbase / CHUNK_SIZE) + "_" + std::to_string(grp.size / CHUNK_SIZE) + "_" + save_file;
  std::string restore_file_mod = std::to_string(grp.scratchbase / CHUNK_SIZE) + "_" + std::to_string(grp.size / CHUNK_SIZE) + "_" + restore_file;
  log_info() << "Adding save_file: " << save_file_mod << " [size: " << save_data.size()
             << "], restore_file: " << restore_file_mod << " [size: " << restore_data.size()
             << "] for " << grp.hintmap_pts.size() << " preemption point(s) with shared labels @"
             << grp.labels.first << " / @" << grp.labels.second << std::endl;

  // Patch template labels and inject
  auto save_chars    = replace_save_restore_labels(save_data,
                           template_labels.first, template_labels.second,
                           grp.labels.first,      grp.labels.second);
  auto restore_chars = replace_save_restore_labels(restore_data,
                           template_labels.first, template_labels.second,
                           grp.labels.first,      grp.labels.second);

  // Compute per-BD address/size pairs from the allocated scratchpad region.
  //   Save:    3 regions × 2 halves   =  6 pairs  (shim BD)
  //   Restore: 3 regions × 4 quarters = 12 pairs  (shim BD)
  auto save_bd_ranges    = compute_save_bd_ranges(static_cast<uint32_t>(save_bd.size()), grp.scratchbase, grp.size);
  auto restore_bd_ranges = compute_restore_bd_ranges(static_cast<uint32_t>(restore_bd.size()), grp.scratchbase, grp.size);

  log_info() << "================BD ranges for save (scratchbase=0x" << std::hex << grp.scratchbase
             << ", size=0x" << grp.size << std::dec << "):" << std::endl;
  for (std::size_t i = 0; i < save_bd_ranges.size(); ++i)
    log_info() << "  save_bd[" << i << "]: base=0x" << std::hex << save_bd_ranges[i].first
               << " size=0x" << save_bd_ranges[i].second << std::dec << std::endl;

  log_info() << "================BD ranges for restore:" << std::endl;
  for (std::size_t i = 0; i < restore_bd_ranges.size(); ++i)
    log_info() << "  restore_bd[" << i << "]: base=0x" << std::hex << restore_bd_ranges[i].first
               << " size=0x" << restore_bd_ranges[i].second << std::dec << std::endl;

  // Patch BD address/length fields directly in the ASM text.
  // bd_col = which column's memtile this BD accesses:
  //   group_index is the base col; each pair of save BDs (2 per col) or quad of restore BDs (4 per col)
  //   steps to the next column.
  std::string save_text(save_chars.begin(), save_chars.end());
  for (std::size_t i = 0; i < save_bd.size() && i < save_bd_ranges.size(); ++i) {
    int bd_col = group_index + static_cast<int>(i / save_channel);
    patch_bd_in_asm(save_text, save_bd[i], save_bd_ranges[i].first, save_bd_ranges[i].second);
    patch_membd_in_asm(save_text, save_membd[i], save_bd_ranges[i].first, save_bd_ranges[i].second, bd_col);
  }
  save_chars.assign(save_text.begin(), save_text.end());

  std::string restore_text(restore_chars.begin(), restore_chars.end());
  for (std::size_t i = 0; i < restore_bd.size() && i < restore_bd_ranges.size(); ++i) {
    int bd_col = group_index + static_cast<int>(i / restore_channel);
    patch_bd_in_asm(restore_text, restore_bd[i], restore_bd_ranges[i].first, restore_bd_ranges[i].second);
    patch_membd_in_asm(restore_text, restore_membd[i], restore_bd_ranges[i].first, restore_bd_ranges[i].second, bd_col);
  }
  restore_chars.assign(restore_text.begin(), restore_text.end());

  m_current_col = col;
  set_data_state(false);
  set_save_restore_routine(true);  // Mark as save/restore routine
  parse_lines(save_chars, save_file_mod);
  pop_data_state();

  set_data_state(false);
  parse_lines(restore_chars, restore_file_mod);
  pop_data_state();
  set_save_restore_routine(false);  // Clear save/restore routine flag
}

// ---------------------------------------------------------------------------
// update_preempt_opcodes
//   Walk column col and rewrite PREEMPT opcode args to use shared labels.
// ---------------------------------------------------------------------------
void
asm_parser::update_preempt_opcodes(int col)
{
  if (m_col.find(col) == m_col.end())
    return;

  const auto points_it = m_preempt_points.find(col);
  if (points_it == m_preempt_points.end())
    return;

  const auto& points = points_it->second;
  const std::string col_prefix = std::to_string(col) + ":";
  std::map<std::string, std::pair<std::string, std::string>> labels_by_preempt_id;
  for (std::size_t pt = 0; pt < points.size(); ++pt) {
    if (points[pt].hintmap_key.empty())
      continue;
    const auto it = m_hintmap_labels.find(col_prefix + points[pt].hintmap_key + ":"
                                           + std::to_string(pt));
    if (it != m_hintmap_labels.end())
      labels_by_preempt_id.emplace(points[pt].id, it->second);
  }

  log_info() << "Updating PREEMPT opcodes for column " << col
             << ", " << labels_by_preempt_id.size() << " hintmap point(s)" << std::endl;

  try {
    for (auto& [lname, section] : get_col_asmdata(col).get_label_data()) {
      for (auto& entry : section.text) {
        if (!entry->isOpcode()) continue;
        const auto& op = entry->get_operation();
        if (op.get_name() != "preempt") continue;
        const auto& args = op.get_args();
        if (args.size() < 3) continue;

        std::string hm_label;
        if (args.size() >= 4)
          hm_label = clean_label_ref(args[3]);  // arg 3: "@hintmap_N"
        if (hm_label.empty()) continue;

        // Build the qualified key: the PREEMPT opcode lives under label scope 'lname',
        // so its hintmap_0 resolves to the hintmap_0 defined in that same scope.
        const std::string qualified = lname + ":" + hm_label;
        // Only update opcodes whose qualified key was recorded for this column
        if (m_preempt_hintmaps.count(col)) {
          const auto& hl = m_preempt_hintmaps[col];
          if (std::find(hl.begin(), hl.end(), qualified) == hl.end())
            continue;
        } else {
          continue;
        }

        const std::string preempt_id = clean_arg(args[0]);
        const auto lbl_it = labels_by_preempt_id.find(preempt_id);
        if (lbl_it == labels_by_preempt_id.end()) continue;

        const auto& new_lbl  = lbl_it->second;
        const std::string new_args = preempt_id
                                     + ", @" + new_lbl.first
                                     + ", @" + new_lbl.second
                                     + ", @" + hm_label;  // keep the short label in the opcode

        log_info() << "Updating PREEMPT opcode id " << preempt_id << " for hintmap '"
                   << qualified << "' in column " << col
                   << " to @" << new_lbl.first << "/@" << new_lbl.second << std::endl;

        entry->update_operation(operation("preempt", new_args));
        // set_line() removed: get_line() now reconstructs from the operation on demand.
      }
    }
  } catch (...) {
    throw error(error::error_code::internal_error, "Error updating PREEMPT opcodes for column "
                                                   + std::to_string(col));
  }
}

// ---------------------------------------------------------------------------
// inject_default_save_restore
//   Create a default scratchpad and inject unpatched save/restore asm.
// ---------------------------------------------------------------------------
void
asm_parser::inject_default_save_restore(int col, int group_index,
                                        const std::string& save_file,
                                        const std::string& restore_file,
                                        const std::vector<uint8_t>& save_data,
                                        const std::vector<uint8_t>& restore_data,
                                        const std::vector<std::string>& save_bd,
                                        const std::vector<std::string>& restore_bd,
                                        const std::vector<std::string>& save_membd,
                                        const std::vector<std::string>& restore_membd)
{

  auto [scratchbase, size] = parse_hintmap_and_calculate_scratchpad(col, "", "");
  auto save_bd_ranges    = compute_save_bd_ranges(static_cast<uint32_t>(save_bd.size()), scratchbase, size);
  auto restore_bd_ranges = compute_restore_bd_ranges(static_cast<uint32_t>(restore_bd.size()), scratchbase, size);

  std::string save_file_mod = std::to_string(scratchbase / CHUNK_SIZE) + "_" + std::to_string(size / CHUNK_SIZE) + "_" + save_file;
  std::string restore_file_mod = std::to_string(scratchbase / CHUNK_SIZE) + "_" + std::to_string(size / CHUNK_SIZE) + "_" + restore_file;

  log_info() << "Adding default save_file: " << save_file_mod << " [size: " << save_data.size()
             << "], restore_file: " << restore_file_mod << " [size: " << restore_data.size() << "]" << std::endl;

  log_info() << "================BD ranges for save (scratchbase=0x" << std::hex << scratchbase
             << ", size=0x" << size << std::dec << "):" << std::endl;
  for (std::size_t i = 0; i < save_bd_ranges.size(); ++i)
    log_info() << "  save_bd[" << i << "]: base=0x" << std::hex << save_bd_ranges[i].first
               << " size=0x" << save_bd_ranges[i].second << std::dec << std::endl;

  log_info() << "================BD ranges for restore:" << std::endl;
  for (std::size_t i = 0; i < restore_bd_ranges.size(); ++i)
    log_info() << "  restore_bd[" << i << "]: base=0x" << std::hex << restore_bd_ranges[i].first
               << " size=0x" << restore_bd_ranges[i].second << std::dec << std::endl;

  // Patch BD address/length fields directly in the ASM text.
  std::vector<char> save_chars(save_data.begin(), save_data.end());
  std::string save_text(save_chars.begin(), save_chars.end());
  for (std::size_t i = 0; i < save_bd.size() && i < save_bd_ranges.size(); ++i) {
    int bd_col = group_index + static_cast<int>(i / save_channel);
    patch_bd_in_asm(save_text, save_bd[i], save_bd_ranges[i].first, save_bd_ranges[i].second);
    patch_membd_in_asm(save_text, save_membd[i], save_bd_ranges[i].first, save_bd_ranges[i].second, bd_col);
  }
  save_chars.assign(save_text.begin(), save_text.end());

  std::vector<char> restore_chars(restore_data.begin(), restore_data.end());
  std::string restore_text(restore_chars.begin(), restore_chars.end());
  for (std::size_t i = 0; i < restore_bd.size() && i < restore_bd_ranges.size(); ++i) {
    int bd_col = group_index + static_cast<int>(i / restore_channel);
    patch_bd_in_asm(restore_text, restore_bd[i], restore_bd_ranges[i].first, restore_bd_ranges[i].second);
    patch_membd_in_asm(restore_text, restore_membd[i], restore_bd_ranges[i].first, restore_bd_ranges[i].second, bd_col);
  }
  restore_chars.assign(restore_text.begin(), restore_text.end());

  m_current_col = col;
  set_data_state(false);
  set_save_restore_routine(true);  // Mark as save/restore routine
  parse_lines(save_chars, save_file_mod);
  pop_data_state();

  set_data_state(false);
  parse_lines(restore_chars, restore_file_mod);
  pop_data_state();
  set_save_restore_routine(false);  // Clear save/restore routine flag
}

// ---------------------------------------------------------------------------
// process_preempt_group
//   Orchestrate hintmap grouping, code injection, and opcode updates for one group.
// ---------------------------------------------------------------------------
void
asm_parser::process_preempt_group(int group,
                                  int group_index,
                                  const std::string& save_file,
                                  const std::string& restore_file,
                                  const std::vector<uint8_t>& save_data,
                                  const std::vector<uint8_t>& restore_data,
                                  const std::vector<std::string>& save_bd,
                                  const std::vector<std::string>& restore_bd,
                                  const std::vector<std::string>& save_membd,
                                  const std::vector<std::string>& restore_membd)
{
  // Handle PREEMPT opcodes that specify a hintmap
  if (m_preempt_hintmaps.count(group)) {
    const auto& template_labels = m_preempt_labels[group];
    // First pass: group hintmaps by (scratchbase, size) and assign unique labels.
    auto groups = build_hintmap_groups(group, group_index);
    for (const auto& grp : groups) {
      // Second pass: inject save/restore code for each hintmap group.
      inject_hintmap_save_restore(group, group_index, save_file, restore_file,
                                  save_data, restore_data, template_labels, grp,
                                  save_bd, restore_bd, save_membd, restore_membd);
    }
    // Third pass: update PREEMPT opcode arguments to use the correct shared labels.
    update_preempt_opcodes(group);
  }

  // Handle PREEMPT opcodes with no hintmap (use group-level default labels)
  if (m_preempt_without_hintmap.count(group))
    inject_default_save_restore(group, group_index, save_file, restore_file,
                                save_data, restore_data,
                                save_bd, restore_bd, save_membd, restore_membd);
}

// ---------------------------------------------------------------------------
// hintmap_chunks — set bits of one hint bitmap
// ---------------------------------------------------------------------------
hintmap_chunk_bits
asm_parser::
hintmap_chunks(int col,
               const std::string& search_context,
               const std::string& hintmap_label)
{
  const std::string ctx   = find_hintmap_context(col, search_context, hintmap_label);
  const auto& all_entries = get_col_asmdata(static_cast<uint32_t>(col)).get_label_asmdata_data(ctx);
  const hintmap_chunk_bits bs = words_to_bitset(collect_hintmap_words(all_entries, hintmap_label));
  const uint64_t max_chunks = static_cast<uint64_t>(get_partition_info()->get_numcolumn()) * CHUNKS_PER_COL;
  verify_hintmap_chunk_limit(bs, max_chunks, hintmap_label);
  return bs;
}

// ---------------------------------------------------------------------------
// collect_preempt_point — Step 1: fixed regions and hintmap state at one PT index
// ---------------------------------------------------------------------------
asm_parser::preempt_point_state
asm_parser::
collect_preempt_point(std::size_t pt)
{
  preempt_point_state state;
  // Collect fixed regions (columns without hintmaps) for this preemption point.
  // Fixed regions are columns that don't have hintmap specifications and will
  // use the default 3MB memory allocation. Each column maps to a specific chunk
  // range based on its column index (col/2 * CHUNKS_PER_COL).
  for (const auto& [col, points] : m_preempt_points) {
    if (pt >= points.size() || !points[pt].hintmap_key.empty())
      continue;
    const uint64_t lo = static_cast<uint64_t>(col / 2) * CHUNKS_PER_COL;
    state.fixed.emplace_back(lo, lo + CHUNKS_PER_COL);
    state.fixed_cols.push_back(col);
    state.fixed_bs |= chunk_range_bitset(lo, lo + CHUNKS_PER_COL);
  }

  // log the fixed regions for debugging
  if (!state.fixed.empty()) {
    log_info() << "preemption point " << pt << ": fixed regions (columns without hintmap):" << std::endl;
    for (std::size_t i = 0; i < state.fixed.size(); ++i) {
      log_info() << "  col " << state.fixed_cols[i] << ": "
                 << describe_chunk_span(state.fixed[i].first, state.fixed[i].second - 1)
                 << std::endl;
    }
  }

  // Collect hintmap regions (columns with hintmap specifications) for this preemption point.
  // For each column that has a hintmap at this preemption point, we need to:
  // 1. Parse the hintmap key to get context and label
  // 2. Load the actual bitmap data from the hintmap
  // 3. Calculate which chunks are available (outside fixed regions)
  // 4. Determine the span of chunks this hintmap covers
  for (const auto& [col, points] : m_preempt_points) {
    // Skip if this column doesn't have enough preemption points or if we're beyond its range
    if (pt >= points.size())
      continue;

    // Get the hintmap key for this column at this preemption point
    const auto& key = points[pt].hintmap_key;

    // Skip columns that don't have a hintmap (empty key means no hintmap specification)
    // Handle columns without hintmap (empty key)
    if (key.empty()) {
      // Create a hintmap column descriptor for no-hintmap columns
      hintmap_col hc;
      hc.col          = col;                    // Column index
      hc.key          = "";                     // Empty key indicates no hintmap
      // For no-hintmap columns, full_bm contains the fixed region bits for this column
      const uint64_t lo = static_cast<uint64_t>(col / 2) * CHUNKS_PER_COL;
      hc.full_bm      = chunk_range_bitset(lo, lo + CHUNKS_PER_COL);  // Fixed region bits for this column
      hc.outside_bm   = hintmap_chunk_bits{};   // No available chunks for redistribution
      hc.zero_hintmap = false;                  // Not a zero hintmap - has fixed bits set
      hc.has_span     = true;                   // Has span from fixed region
      hc.span_lo      = lo;                     // Start of fixed region
      hc.span_hi      = lo + CHUNKS_PER_COL - 1; // End of fixed region (inclusive)

      // Add this no-hintmap column to our collection for this preemption point
      state.hintmap_cols.push_back(hc);
      continue;
    }

    // Parse the qualified hintmap key into context and label components
    // Format is typically "context.label" where context identifies the scope
    auto [ctx, label] = split_qualified_hintmap(key);

    // Load the actual bitmap data for this hintmap from the assembly data
    // This gives us a bitset where each bit represents a memory chunk
    const hintmap_chunk_bits full_bm = hintmap_chunks(col, ctx, label);

    // Calculate which chunks from the hintmap are actually available for redistribution
    // We exclude chunks that are already reserved for fixed regions (columns without hintmaps)
    const hintmap_chunk_bits outside_bm = full_bm & ~state.fixed_bs;

    // Create a hintmap column descriptor to track all the information about this column
    hintmap_col hc;
    hc.col          = col;                    // Column index
    hc.key          = key;                    // Original hintmap key for debugging/logging
    hc.full_bm      = full_bm;                // Complete bitmap as specified in hintmap
    hc.outside_bm   = outside_bm;             // Available chunks (excluding fixed regions)
    hc.zero_hintmap = full_bm.none();         // True if hintmap has no bits set (empty hintmap)
    hc.has_span     = false;                  // Will be set to true if we find a valid span

    // Calculate the span (range) of chunks covered by this hintmap's available bits
    // This helps with redistribution algorithms that need to know the extent of requested chunks
    if (auto lo = bitset_find_first(outside_bm)) {
      hc.span_lo = *lo;  // First (lowest) chunk index in the available bitmap
      if (auto hi = bitset_find_last(outside_bm)) {
        hc.span_hi  = *hi;  // Last (highest) chunk index in the available bitmap
        hc.has_span = true;
      }
    }

    // Add this hintmap column to our collection for this preemption point
    state.hintmap_cols.push_back(hc);
  }

  return state;
}

// ---------------------------------------------------------------------------
// verify_overlap — pairwise full_bm overlap among all controllers at pt.
//   No-hintmap controllers contribute their fixed 3MB home window as full_bm.
// ---------------------------------------------------------------------------
void
asm_parser::
verify_overlap(std::size_t pt, const preempt_point_state& state)
{
  for (std::size_t a = 0; a < state.hintmap_cols.size(); ++a) {
    for (std::size_t b = a + 1; b < state.hintmap_cols.size(); ++b) {
      if ((state.hintmap_cols[a].full_bm & state.hintmap_cols[b].full_bm).none())
        continue;

      std::string error_msg = "hintmap overlap at preemption point " + std::to_string(pt)
                            + " between controller " + std::to_string(state.hintmap_cols[a].col);
      if (state.hintmap_cols[a].key.empty())
        error_msg += " (no hintmap, using default 3MB window)";
      else
        error_msg += " (hintmap '" + state.hintmap_cols[a].key + "')";

      error_msg += " and controller " + std::to_string(state.hintmap_cols[b].col);
      if (state.hintmap_cols[b].key.empty())
        error_msg += " (no hintmap, using default 3MB window)";
      else
        error_msg += " (hintmap '" + state.hintmap_cols[b].key + "')";

      error_msg += "; the same chunk must not be requested by two controllers\n";

      throw error(error::error_code::invalid_asm, error_msg);
    }

  }
}

// Determines whether redistribution is needed or direct assignment is possible.
// Algorithm:
// 1. Check all pairs of hintmap controllers that have valid spans
// 2. If any two spans overlap (inclusive), redistribution is required
// 3. If all spans are disjoint, perform direct region assignment
// Returns true when redistribution is needed, false when direct assignment was done
bool
asm_parser::
need_distribution_or_assign_direct(std::size_t pt, const preempt_point_state& state)
{
  for (std::size_t a = 0; a < state.hintmap_cols.size(); ++a) {
    if (!state.hintmap_cols[a].has_span)
      continue;
    for (std::size_t b = a + 1; b < state.hintmap_cols.size(); ++b) {
      if (!state.hintmap_cols[b].has_span)
        continue;
      if (spans_overlap_inclusive(state.hintmap_cols[a].span_lo, state.hintmap_cols[a].span_hi,
                                  state.hintmap_cols[b].span_lo, state.hintmap_cols[b].span_hi)) {
        log_warn() << "[need_distribution] preemption point " << pt << ": span overlap detected between controller "
                   << state.hintmap_cols[a].col << " (hintmap '" << state.hintmap_cols[a].key << "') "
                   << "chunks [" << state.hintmap_cols[a].span_lo << ".." << state.hintmap_cols[a].span_hi << "] "
                   << "and controller " << state.hintmap_cols[b].col << " (hintmap '" << state.hintmap_cols[b].key << "') "
                   << "chunks [" << state.hintmap_cols[b].span_lo << ".." << state.hintmap_cols[b].span_hi << "]"
                   << ", redistribution needed" << std::endl;
        return true;
      }
    }
  }

  log_warn() << "[assign_direct] preemption point " << pt << ": spans disjoint, using direct regions"
             << std::endl;
  for (const auto& hc : state.hintmap_cols) {
    preempt_scratchpad region = {0, 0};
    if (!hc.zero_hintmap && hc.has_span) {
      auto [base, size] = chunks_to_region(hc.span_lo, hc.span_hi);
      region = {base, size};
    }
    m_preempt_region[hc.col][pt] = region;
    log_warn() << "  controller " << hc.col << " (hintmap '" << hc.key << "'): "
               << describe_region(region.scratchbase, region.size) << std::endl;
  }
  return false;
}

// ---------------------------------------------------------------------------
// redistribute_preempt_regions — chunk redistribution when direct assign fails.
// Called for one preemption point after need_distribution_or_assign_direct() finds
// overlapping spans. Builds a pool from each hintmap controller's outside_bm (bits
// outside no-hintmap 3MB home windows), then assigns chunk ranges via
// redistribute_with_hard_cuts():
//
//   1. Pool: OR together outside_bm from every hintmap controller at this pt.
//      Quota (new bits each controller adds) is logged only.
//
//   2. Pattern: sort all controllers by column; each slot is hintmap or fixed
//      (no-hintmap). fixed slots are hard cuts — pool chunks inside their 3MB
//      window are skipped and never shared across a cut.
//
//   3. Segment walk: consecutive hintmap slots between hard cuts form one segment.
//      Collect sorted pool-chunk indices below the next hard cut.
//
//   4. Split: partition_flexible_minimize_holes() divides those indices among
//      the segment's hintmap controllers by cutting at the largest gaps.
//
//   5. Apply: write each [lo, hi) chunk range into m_preempt_region[col][pt].
// ---------------------------------------------------------------------------
void
asm_parser::
redistribute_preempt_regions(std::size_t pt, const preempt_point_state& state)
{
  // Log the start of redistribution process for this preemption point
  log_info() << "preemption point " << pt << ": starting chunk redistribution" << std::endl;

  // Define a controller structure to track redistribution parameters
  struct ctrl {
    int      col;      // Column index of the controller
    uint64_t quota;    // Number of chunks this controller should receive
  };
  std::vector<ctrl> ctrls;
  hintmap_chunk_bits pool;  // Accumulated pool of all available chunks

  // Phase 1: Collect available chunks from all hintmap controllers
  // Build the redistribution pool by gathering chunks from each controller's hintmap
  for (const auto& hc : state.hintmap_cols) {
    // skip if no hintmap
    if (hc.key.empty())
      continue;
    // Calculate how many new chunks this controller contributes to the pool
    // (chunks that aren't already in the pool from previous controllers)
    const uint64_t pooled = (hc.outside_bm & ~pool).count();

    // Add this controller's available chunks to the global pool
    pool |= hc.outside_bm;

    // Record this controller's redistribution parameters
    ctrls.push_back({hc.col, pooled});

    // Log the controller's contribution and special properties
    log_info() << "  controller " << hc.col << " (hintmap '" << hc.key << "'): "
               << pooled << " new chunk(s) added to pool"
               << (hc.zero_hintmap ? " (zero hintmap)" : "") << std::endl;
  }

  // Early exit if no chunks are available for redistribution
  if (pool.none()) {
    log_info() << "  pool empty after stripping no-hintmap windows, nothing to redistribute"
               << std::endl;
    return;
  }

  // Phase 2: Prepare controller column list for the assignment algorithm
  std::vector<int> ctrl_cols;
  ctrl_cols.reserve(ctrls.size());
  for (const auto& c : ctrls)
    ctrl_cols.push_back(c.col);

  // Phase 3–4: segment by hard cuts; split pool among hintmap controllers per segment
  // (pattern.size() == hintmap_cols + fixed, one entry per controller in column order).
  std::vector<controller_spec> ordered;
  ordered.reserve(state.hintmap_cols.size());
  {
    auto cols = state.hintmap_cols;
    std::sort(cols.begin(), cols.end(),
              [](const hintmap_col& a, const hintmap_col& b) { return a.col < b.col; });
    for (const auto& hc : cols) {
      controller_spec spec{};
      spec.col      = hc.col;
      spec.is_fixed = hc.key.empty();
      spec.span_lo  = hc.span_lo;
      spec.span_hi  = hc.span_hi;
      ordered.push_back(spec);
    }
  }

  const auto assigned = redistribute_with_hard_cuts(pool, ordered, ctrl_cols);

  // Phase 5: Apply the assignments and update preemption regions
  // Log the final redistribution results for this preemption point
  log_warn() << "  final redistribution at preemption point " << pt << ":" << std::endl;
  for (std::size_t i = 0; i < ctrls.size(); ++i) {
    auto [base, size] = chunk_rng_to_region(assigned[i]);
    m_preempt_region[ctrls[i].col][pt] = {base, size};
    const auto key_it = std::find_if(state.hintmap_cols.cbegin(), state.hintmap_cols.cend(),
                                     [&](const hintmap_col& hc) { return hc.col == ctrls[i].col; });
    const std::string& key = (key_it != state.hintmap_cols.cend()) ? key_it->key : "?";
    std::ostringstream oss;
    oss << "    controller " << ctrls[i].col << " (hintmap '" << key << "'): "
        << describe_region(base, size);
    if (assigned[i].second > assigned[i].first && ctrls[i].quota > 0
        && static_cast<uint64_t>(assigned[i].second - assigned[i].first) > ctrls[i].quota)
      oss << " [includes hole chunks within span]";
    log_warn() << oss.str() << std::endl;
  }
  for (std::size_t fi = 0; fi < state.fixed_cols.size(); ++fi) {
    log_warn() << "    controller " << state.fixed_cols[fi]
               << " (no hintmap): unchanged default 3MB "
               << describe_chunk_span(state.fixed[fi].first, state.fixed[fi].second - 1)
               << std::endl;
  }
}

// ---------------------------------------------------------------------------
// settle_preempt_regions — Steps 1–4 per preemption point
//   For each preemption point, this function:
//   1. Collects fixed regions (columns without hintmaps) and hintmap regions
//   2. Verifies no overlap between different hintmap sets
//   3. Attempts direct assignment if possible (no redistribution needed)
//   4. Falls back to redistribution algorithm if direct assignment fails
// ---------------------------------------------------------------------------
void
asm_parser::
settle_preempt_regions()
{
  m_preempt_region.clear();

  // Initialize preemption regions for each column and find the maximum number of preemption points
  std::size_t num_points = 0;
  for (const auto& [col, points] : m_preempt_points) {
    m_preempt_region[col].resize(points.size());
    num_points = std::max(num_points, points.size());
  }

  // Process each preemption point across all columns
  for (std::size_t pt = 0; pt < num_points; ++pt) {
    // Step 1: Collect all fixed and hintmap regions for this preemption point
    const preempt_point_state state = collect_preempt_point(pt);

    // Step 2: Verify no overlap between different hintmap sets
    verify_overlap(pt, state);

    // Step 3: Try direct assignment first, fall back to redistribution if needed
    if (need_distribution_or_assign_direct(pt, state))
      redistribute_preempt_regions(pt, state);
  }
}

// Prebuilt save/restore map key for multi-column group 0 (1c0 template).
constexpr uint32_t MULTICOL_PREEMPT_SAVE_RESTORE_BASE = 10;

// ---------------------------------------------------------------------------
// finalize_preempt
// ---------------------------------------------------------------------------
void
asm_parser::
finalize_preempt()
{
  if (m_preempt_labels.empty())
    return;

  if (is_multi_column_mode()) {
    // Settle who transfers which chunks before any save/restore code is injected.
    settle_preempt_regions();

    for (const auto& [group, labels] : m_preempt_labels) {
      auto [save_data, restore_data] = get_preempt_save_restore(MULTICOL_PREEMPT_SAVE_RESTORE_BASE + group);
      auto [save_bd, restore_bd] = get_preempt_save_restore_shimbd(MULTICOL_PREEMPT_SAVE_RESTORE_BASE + group);
      auto [save_membd, restore_membd] = get_preempt_save_restore_membd(MULTICOL_PREEMPT_SAVE_RESTORE_BASE + group);
      if (save_data.empty() || restore_data.empty() || save_bd.empty() || restore_bd.empty() || save_membd.empty() || restore_membd.empty())
        throw error(error::error_code::internal_error,
                    "Preempt save/restore data not found for group " + std::to_string(group));

      std::string suffix       = multicol_suffix(group);
      std::string save_file    = "aie4_save_"    + suffix + ".asm";
      std::string restore_file = "aie4_restore_" + suffix + ".asm";
      process_preempt_group(group, group / 2,
                             save_file, restore_file, save_data, restore_data, save_bd, restore_bd, save_membd, restore_membd);
    }
  } else {
    uint32_t num_cols = get_partition_info()->get_numcolumn();
    auto [save_data, restore_data] = get_preempt_save_restore(num_cols);
    auto [save_bd, restore_bd] = get_preempt_save_restore_shimbd(num_cols);
    auto [save_membd, restore_membd] = get_preempt_save_restore_membd(num_cols);
    if (save_data.empty() || restore_data.empty() || save_bd.empty() || restore_bd.empty() || save_membd.empty() || restore_membd.empty())
      throw error(error::error_code::internal_error,
                  "Preempt save/restore data not found for " + std::to_string(num_cols) + " columns");

    std::string col_str      = std::to_string(num_cols) + "c.asm";
    std::string save_file    = "aie4_save_"    + col_str;
    std::string restore_file = "aie4_restore_" + col_str;
    process_preempt_group(0, 0, save_file, restore_file, save_data, restore_data, save_bd, restore_bd, save_membd, restore_membd);
  }
}

void
attach_to_group_directive::
operate(std::shared_ptr<asm_parser> parserptr,
        const std::string& /*directive_line*/,
        const std::string& args_tail)
{
  m_parserptr = parserptr;
  verify_nonempty_args(args_tail, error::error_code::invalid_asm, "Invalid attach_to_group directive argument\n");

  // dummy eof added if col change happens before eof
  m_parserptr->insert_col_asmdata(std::make_shared<asm_data>(operation("eof", ""),
                                                              operation_type::op, code_section::unknown, 0,
                                                              (uint32_t)-1, 0, m_parserptr->current_parse_file_idx()));
  m_parserptr->set_current_col(std::stoi(args_tail));
  m_parserptr->set_data_state(false);
}

void
section_directive::
operate(std::shared_ptr<asm_parser> parserptr,
        const std::string& /*directive_line*/,
        const std::string& args_tail)
{
  m_parserptr = parserptr;
  verify_nonempty_args(args_tail, error::error_code::invalid_asm, ".section directive requires arguments\n");

  std::vector<std::string> args = splitoption(args_tail.c_str(), ',');
  if (is_test_section(args[0]))
    m_parserptr->set_data_state(false);
  else if (is_data_section(args[0]))
    m_parserptr->set_data_state(true);
  else if (is_annotation_section(args[0]))
    m_parserptr->set_annotation_state();
  else
    log_warn() << "section directive with unknown section found:" << args[0] << "\n";
}

void
partition_directive::
operate(std::shared_ptr<asm_parser> parserptr,
        const std::string& directive_line,
        const std::string& /*args_tail*/)
{
  m_parserptr = parserptr;
  static const regex pattern(R"(\.partition\s+(\d+)(column|core:(\d+)mem))");
  smatch match;
  log_info() << "PARTITION:" << directive_line << "\n";
  std::string line = directive_line;
  if (regex_match(line, match, pattern)) {
    if (match[2] == "column") {
      log_info() << "Column count: " << match[1] << "\n";
      m_parserptr->set_numcolumn(to_uinteger<uint32_t>(match[1]));
    } else {
      m_parserptr->set_numcore(to_uinteger<uint32_t>(match[1]));
      m_parserptr->set_nummem(to_uinteger<uint32_t>(match[3]));
      log_info() << "Core count: " << match[1] << "\n";
      log_info() << "Memory size: " << match[3] << "\n";
    }
  } else
    throw error(error::error_code::invalid_asm, "Invalid format!! " + line + "\n");
}

void
target_directive::
operate(std::shared_ptr<asm_parser> parserptr,
        const std::string& directive_line,
        const std::string& /*args_tail*/)
{
  m_parserptr = parserptr;
  // Pattern: .target <arch>-<sub-arch> or .target <arch>
  static const regex pattern(R"(\.target\s+([a-zA-Z0-9]+)(?:-([a-zA-Z0-9]+))?)");
  smatch match;
  log_info() << "TARGET:" << directive_line << std::endl;
  std::string line = directive_line;
  if (regex_match(line, match, pattern)) {
    std::string arch = match[1].str();
    log_info() << "Target architecture: " << arch << std::endl;
    m_parserptr->set_target_arch(arch);

    if (match[2].matched && match[2].length() > 0) {
      std::string sub_arch = match[2].str();
      log_info() << "Target sub-architecture: " << sub_arch << std::endl;
      m_parserptr->set_target_sub_arch(sub_arch);
    }
  } else
    throw error(error::error_code::invalid_asm, "Invalid .target format!! " + line + "\n");
}

void
aie_row_topology_directive::
operate(std::shared_ptr<asm_parser> parserptr,
        const std::string& directive_line,
        const std::string& /*args_tail*/)
{
  m_parserptr = parserptr;
  // Pattern: .aie_row_topology A-B-C-D
  // Where: A=num_south_shim, B=num_memtile_row, C=num_coretile_row, D=num_north_shim
  // Example: .aie_row_topology 1-1-4-0
  static const regex pattern(R"(\.aie_row_topology\s+(\d+)-(\d+)-(\d+)-(\d+))");
  smatch match;
  log_info() << "AIE_ROW_TOPOLOGY:" << directive_line << std::endl;
  std::string line = directive_line;
  if (regex_match(line, match, pattern)) {
    uint32_t num_south_shim = to_uinteger<uint32_t>(match[1]);
    uint32_t num_memtile_row = to_uinteger<uint32_t>(match[2]);
    uint32_t num_coretile_row = to_uinteger<uint32_t>(match[3]);
    uint32_t num_north_shim = to_uinteger<uint32_t>(match[4]);

    log_info() << "Number of south shim: " << num_south_shim << std::endl;
    log_info() << "Number of memtile rows: " << num_memtile_row << std::endl;
    log_info() << "Number of coretile rows: " << num_coretile_row << std::endl;
    log_info() << "Number of north shim: " << num_north_shim << std::endl;

    m_parserptr->set_num_south_shim(num_south_shim);
    m_parserptr->set_num_memtile_row(num_memtile_row);
    m_parserptr->set_num_coretile_row(num_coretile_row);
    m_parserptr->set_num_north_shim(num_north_shim);
    m_parserptr->set_aie_row_topology_is_set(true);
  } else
    throw error(error::error_code::invalid_asm, "Invalid .aie_row_topology format!! " + line + "\nExpected format: .aie_row_topology A-B-C-D\n");
}

bool
include_directive::
read_include_file(std::string filename)
{
  if (m_parserptr->is_filename_seen(filename))
    throw error(error::error_code::invalid_input,
                "duplicate asm file name \"" + filename + "\"");
  m_parserptr->set_data_state(false);
  std::vector<char> data;
  log_info() << "Reading contents from virtual or disk file:" << filename << "\n";
  try {
    if (!m_parserptr->get_artifacts()) return false;
    data = m_parserptr->get_artifacts()->get(filename, m_parserptr->get_include_list());
  } catch (const std::runtime_error& e) {
    log_error() << "Error reading buffer from artifacts: " << e.what() << "\n";
    return false;
  }
  m_parserptr->parse_lines(data, filename);
  m_parserptr->pop_data_state();
  return true;
}

void
include_directive::
operate(std::shared_ptr<asm_parser> parserptr,
        const std::string& /*directive_line*/,
        const std::string& args_tail)
{
  m_parserptr = parserptr;
  std::string file = args_tail;
  if (file.size() >= 2 && file.front() == '"' && file.back() == '"')
    file =  file.substr(1, file.size() - 2);

  if (read_include_file(file))
    return;
  throw error(error::error_code::internal_error, "File " + file + " not exist\n");
}

void
end_of_label_directive::
operate(std::shared_ptr<asm_parser> parserptr,
        const std::string& /*directive_line*/,
        const std::string& args_tail)
{
  m_parserptr = parserptr;

  std::string label = m_parserptr->top_label();
  m_parserptr->pop_label();

  verify_nonempty_args(args_tail, error::error_code::invalid_asm, ".endl directive requires a label argument\n");

  std::vector<std::string> args = splitoption(args_tail.c_str(), ',');
  if (label.compare(args[0]))
    throw error(error::error_code::internal_error, "endl label mismatch (" + label + " != " + args[0] + ")\n");
  m_parserptr->pop_data_state();
}

void
pad_directive::
operate(std::shared_ptr<asm_parser> parserptr,
        const std::string& directive_line,
        const std::string& args_tail)
{
  m_parserptr = parserptr;
  verify_nonempty_args(args_tail, error::error_code::invalid_asm, ".setpad directive requires arguments\n");

  std::vector<std::string> args = splitoption(args_tail.c_str(), ',');

  // .setpad should only be part of save/restore routines
  if (!m_parserptr->should_skip_setpad_in_save_restore()) {
    log_warn() << "Warning: Directive \"" << directive_line << "\" found outside save/restore routine for target: "
               << m_parserptr->get_target_type() << "\n";
  }

  add_scratchpad(args[0], args[1]);
}

void
pad_directive::
add_scratchpad(std::string& name, std::string& str) {
  // Check if the string is an integer
  str = trim(str);
  if (std::all_of(str.begin(), str.end(), ::isdigit)) {
    std::vector<char> empty_vector;
    m_parserptr->insert_scratchpad(name, convert2int(str) * WORD_SIZE, empty_vector);
    return;
  }
  // Check if the string is a hexadecimal number
  static const regex hex_regex("0[xX][0-9a-fA-F]+");
  if (regex_match(str, hex_regex)) {
    std::vector<char> empty_vector;
    m_parserptr->insert_scratchpad(name, convert2int(str) * WORD_SIZE, empty_vector);
    return;
  }

  std::string file = str;
  if (file.front() == '"' && file.back() == '"')
    file = str.substr(1, str.size() - 2);

  if (read_pad_file(name, file))
    return;
  throw error(error::error_code::internal_error, "File " + file + " not exist\n");
}

bool
pad_directive::
read_pad_file(std::string& name, std::string& filename)
{

  log_info() << "Reading contents from virtual or disk file:" << filename << "\n";
  m_parserptr->set_data_state(false);
  std::vector<char> data;
  try {
    if (!m_parserptr->get_artifacts()) return false;
    data = m_parserptr->get_artifacts()->get(filename, m_parserptr->get_include_list());
  } catch (const std::runtime_error& e) {
    log_error() << "Error reading buffer from artifacts: " << e.what() << "\n";
    return false;
  }
  m_parserptr->insert_scratchpad(name, static_cast<offset_type>(data.size()), data);
  return true;
}
}
