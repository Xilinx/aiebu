// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.
#include "asm_parser.h"
#include "preprocessor/aie4/aie4_save_restore_map_prebuilt.h"

#include "aiebu/aiebu_error.h"

#include <fstream>
#include <iomanip>
#include <numeric>
#include <optional>
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
//   Each bit represents one 64KB chunk.  Throws if the set bits are not
//   contiguous across all words.
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

  // Holes between set bits are absorbed: size covers the full first-to-last span
  // so that any transfer hole is included in the scratchpad region.
  uint64_t span = (first_bit != NO_BIT) ? (last_bit - first_bit + 1) : 0;
  if (span != set_bits) {
    log_info() << "Hintmap '" << hintmap_label << "' has non-contiguous bits "
               << "(first=bit " << first_bit << ", last=bit " << last_bit
               << ", span=" << span << ", set=" << set_bits
               << ") - hole absorbed into scratchpad" << std::endl;
  }

  // Guard against overflow when there are set bits: CHUNK_SIZE is 64KB; a
  // 160-chunk span (worst case for a 10MB memtile) gives 160*64KB = 10MB,
  // well within uint64_t range.  Reject obviously-corrupt bitmaps where span
  // or first_bit exceed 2^32 (no real hintmap would be that wide).
  if (first_bit != NO_BIT && (span > UINT32_MAX || first_bit > UINT32_MAX)) {
    throw error(error::error_code::invalid_asm,
                "Hintmap '" + hintmap_label + "': span or first bit out of range");
  }

  const uint64_t scratchbase = (first_bit != NO_BIT) ? (first_bit * CHUNK_SIZE) : DEFAULT_BASE;
  const uint64_t size        = (span > 0) ? (span * CHUNK_SIZE) : 0;

  log_info() << "Hintmap parsed for group " << group << " (col " << group << "): "
             << "scratchbase=0x" << std::hex << scratchbase
             << ", size=0x"      << size << " (" << std::dec
             << (size / BYTES_PER_MB) << "MB, span=" << span << " chunks, set=" << set_bits << " chunks)" << std::endl;

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
//   Each bit represents a 64KB chunk.  Returns the contiguous span that covers
//   all set bits: scratchbase = first_set_bit * 64KB,
//   size = (last_set_bit - first_set_bit + 1) * 64KB.
//   Holes between set bits are absorbed into the span (not thrown as errors).
//   Returns defaults when hintmap_label is empty.
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

  // Build the qualified key the same way the override map is keyed.
  const std::string ctx = find_hintmap_context(group, search_context, hintmap_label);
  const std::string qualified = ctx + ":" + hintmap_label;
  auto ov = m_hintmap_region_override.find({group, qualified});
  if (ov != m_hintmap_region_override.end())
    return ov->second;

  auto& col_data            = get_col_asmdata(static_cast<uint32_t>(group));
  const auto& all_entries   = col_data.get_label_asmdata_data(ctx);
  const auto words          = collect_hintmap_words(all_entries, hintmap_label);
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
asm_parser::build_hintmap_groups(int group,
                                 const std::vector<std::string>& hintmap_labels,
                                 int group_index)
{
  // Map (scratchbase, size) -> index in result vector
  std::map<std::pair<uint64_t,uint64_t>, std::size_t> key_to_idx;
  std::vector<hintmap_group_entry> result;
  int unique_idx = 0;

  for (const auto& qualified_key : hintmap_labels) {
    auto [ctx, label_name] = split_qualified_hintmap(qualified_key);

    auto [scratchbase, size] = parse_hintmap_and_calculate_scratchpad(group, ctx, label_name);
    auto key = std::make_pair(scratchbase, size);
    auto it  = key_to_idx.find(key);

    if (it == key_to_idx.end()) {
      hintmap_group_entry entry;
      entry.scratchbase = scratchbase;
      entry.size        = size;
      entry.hintmaps.push_back(qualified_key);
      entry.labels = {"save_"    + std::to_string(group_index) + "_" + std::to_string(unique_idx),
                      "restore_" + std::to_string(group_index) + "_" + std::to_string(unique_idx)};
      log_info() << "Column " << group << ": hintmap '" << qualified_key
                 << "' -> scratchbase=0x" << std::hex << scratchbase
                 << ", size=0x" << size << std::dec
                 << " -> new labels @" << entry.labels.first
                 << " / @" << entry.labels.second << std::endl;
      key_to_idx[key] = result.size();
      result.push_back(std::move(entry));
      ++unique_idx;
    } else {
      auto& entry = result[it->second];
      entry.hintmaps.push_back(qualified_key);
      log_info() << "Column " << group << ": hintmap '" << qualified_key
                 << "' -> sharing labels @" << entry.labels.first
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
  for (const auto& hm : grp.hintmaps)
    m_hintmap_labels[col_prefix + hm] = grp.labels;

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
             << "] for " << grp.hintmaps.size() << " hintmap(s) with shared labels @"
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

  log_info() << "Updating PREEMPT opcodes for column " << col
             << ", m_hintmap_labels has " << m_hintmap_labels.size() << " entries" << std::endl;

  try {
    for (auto& [lname, section] : get_col_asmdata(col).get_label_data()) {
      for (auto& entry : section.text) {
        if (!entry->isOpcode()) continue;
        const auto& op = entry->get_operation();
        if (op.get_name() != "preempt") continue;
        const auto& args = op.get_args();
        if (args.size() < 3) continue;

        // Extract hintmap label (arg 3: "@hintmap_N")
        std::string hm_label;
        if (args.size() >= 4)
          hm_label = clean_label_ref(args[3]);

        if (hm_label.empty()) continue;

        // Build the qualified key: the PREEMPT opcode lives under label scope 'lname',
        // so its hintmap_0 resolves to the hintmap_0 defined in that same scope.
        std::string qualified = lname + ":" + hm_label;

        // Only update opcodes whose qualified key was recorded for this column
        bool in_col = false;
        if (m_preempt_hintmaps.count(col)) {
          const auto& hl = m_preempt_hintmaps[col];
          in_col = std::find(hl.begin(), hl.end(), qualified) != hl.end();
        }
        if (!in_col) continue;

        auto it = m_hintmap_labels.find(std::to_string(col) + ":" + qualified);
        if (it == m_hintmap_labels.end()) continue;

        const auto& new_lbl  = it->second;
        std::string new_args = clean_arg(args[0])
                               + ", @" + new_lbl.first
                               + ", @" + new_lbl.second
                               + ", @" + hm_label;  // keep the short label in the opcode

        log_info() << "Updating PREEMPT opcode for hintmap '" << qualified
                   << "' in column " << col
                   << " from @" << args[1] << "/@" << args[2]
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
    auto groups = build_hintmap_groups(group, m_preempt_hintmaps[group], group_index);
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
// collect_set_chunks
//   Return the sorted list of chunk indices (bit positions) that are set in
//   the hintmap words of 'hintmap_label' within column 'col'.
// ---------------------------------------------------------------------------
static std::vector<uint64_t>
collect_set_chunks(asm_parser& parser,
                   int col,
                   const std::string& search_context,
                   const std::string& hintmap_label)
{
  // Re-use the public helpers to get the raw .long words.
  const std::string ctx   = parser.find_hintmap_context(col, search_context, hintmap_label);
  auto& col_data          = parser.get_col_asmdata(static_cast<uint32_t>(col));
  const auto& all_entries = col_data.get_label_asmdata_data(ctx);
  const auto words        = collect_hintmap_words(all_entries, hintmap_label);

  std::vector<uint64_t> chunks;
  constexpr uint64_t BITS_PER_WORD = 32;
  for (std::size_t widx = 0; widx < words.size(); ++widx) {
    auto w = words[widx];
    while (w) {
      int bit = aiebu_ctz(w);
      chunks.push_back(widx * BITS_PER_WORD + static_cast<uint64_t>(bit));
      w &= w - 1;  // clear lowest set bit
    }
  }
  return chunks;  // already in ascending order since we scan low-to-high
}

// ---------------------------------------------------------------------------
// build_point_infos
//   Snapshot the save-region for every active controller at preemption point
//   index 'idx'.
//
//   m_preempt_points maps each column to its ordered list of PREEMPT opcodes.
//   All columns must have the same count (checked earlier), so the n-th opcode
//   of every column belongs to the same logical preemption point.
//
//   For each column that has an opcode at position 'idx':
//     • Split the opcode's hintmap_key ("scope:label") into context and label.
//       An empty key means this controller has no hintmap.
//     • Call parse_hintmap_and_calculate_scratchpad to convert the hintmap bits
//       into a [base, size) region.  With a hintmap this is
//       [first_set_chunk * CHUNK_SIZE, (last_set_chunk+1) * CHUNK_SIZE);
//       without a hintmap it is the controller's full 3 MB column region.
//     • Append a col_point_info{col, id, hintmap_key, base, size} to the result.
//
//   The returned vector is consumed by validate_resolve_hintmap_overlap to
//   detect and fix span overlaps across controllers.
// ---------------------------------------------------------------------------
std::vector<asm_parser::col_point_info>
asm_parser::build_point_infos(std::size_t idx)
{
  std::vector<col_point_info> infos;
  for (const auto& [col, pts] : m_preempt_points) {
    if (idx >= pts.size())
      continue;
    const auto& point = pts[idx];
    auto [ctx, label] = split_qualified_hintmap(point.hintmap_key);
    auto [base, size] = parse_hintmap_and_calculate_scratchpad(col, ctx, label);
    infos.push_back({col, point.id, point.hintmap_key, base, size});
  }
  return infos;
}

// ---------------------------------------------------------------------------
// overlap_error  (file-local)
//   Build and throw the standard overlap error message.
// ---------------------------------------------------------------------------
[[noreturn]] static void
overlap_error(std::size_t pt_idx, std::string msg)
{
  throw error(error::error_code::invalid_asm,
              "hint bitmap overlap at preemption point " + std::to_string(pt_idx)
              + ": " + std::move(msg)
              + "; hint bitmaps of different controllers must not overlap at the same"
                " preemption point\n");
}

// ---------------------------------------------------------------------------
// chunks_to_bitset  (file-local)
//   Convert a sorted list of chunk indices to a std::bitset<512>.
//   Bits 0-143 correspond to the 144 usable 64KB chunks of a 9MB memtile
//   scratchpad; bits 144-511 are reserved and will never be set.
// ---------------------------------------------------------------------------
static std::bitset<512>
chunks_to_bitset(const std::vector<uint64_t>& chunks)
{
  std::bitset<512> bs;
  for (const auto ch : chunks) {
    if (ch < 512)
      bs.set(static_cast<std::size_t>(ch));
  }
  return bs;
}

// check_overlap_pair  (file-local)
//   Examines one (a, b) pair at preemption point pt_idx.
//   Returns false if spans don't overlap.
//   Throws on a real bit overlap.
//   Returns true and updates union-find on a span-only overlap (fixable).
// ---------------------------------------------------------------------------
static bool
check_overlap_pair(
    std::size_t a, std::size_t b, std::size_t pt_idx,
    const std::vector<asm_parser::col_point_info>& infos,
    std::vector<std::vector<uint64_t>>& chunks_cache,
    std::vector<int>& parent)
{
  const auto& ra = infos[a];
  const auto& rb = infos[b];

  // No span overlap — nothing to check.
  if (!(ra.base < rb.base + rb.size && rb.base < ra.base + ra.size))
    return false;

  // Union-find helpers (path-compressed find + union).
  auto uf_find = [&](int x) {
    while (parent[x] != x) { parent[x] = parent[parent[x]]; x = parent[x]; }
    return x;
  };
  auto uf_union = [&](int x, int y) {
    auto rx = uf_find(x), ry = uf_find(y);
    if (rx != ry) { parent[rx] = ry; }
  };

  // Case 1: both have no hintmap → fixed contiguous ranges, real overlap.
  if (ra.hintmap_key.empty() && rb.hintmap_key.empty()) {
    std::ostringstream oss;
    oss << "controller " << ra.col << " (id " << ra.id
        << ") saves [0x" << std::hex << ra.base << ", 0x" << (ra.base + ra.size) << ")"
        << " (no hintmap) and controller " << rb.col << " (id " << rb.id
        << ") saves [0x" << rb.base << ", 0x" << (rb.base + rb.size) << ") (no hintmap)"
        << std::dec;
    overlap_error(pt_idx, oss.str());
  }

  // Case 2: one fixed range, one hintmap.
  // Use a bitset<512> over the fixed region's chunk range and AND with the
  // hintmap bitset to detect any set chunk inside the fixed region.
  if (ra.hintmap_key.empty() || rb.hintmap_key.empty()) {
    const bool a_fixed         = ra.hintmap_key.empty();
    const auto& fixed          = a_fixed ? ra : rb;
    const std::size_t hm_idx   = a_fixed ? b  : a;
    const auto& hm             = infos[hm_idx];
    const auto first_chunk = fixed.base / CHUNK_SIZE;
    const auto last_chunk  = (fixed.base + fixed.size) / CHUNK_SIZE;

    // Build a bitset marking every chunk inside the fixed region.
    std::bitset<512> fixed_bs;
    for (auto ch = first_chunk; ch < last_chunk && ch < 512; ++ch)
      fixed_bs.set(static_cast<std::size_t>(ch));

    if ((chunks_to_bitset(chunks_cache[hm_idx]) & fixed_bs).any()) {
      std::ostringstream oss;
      oss << "controller " << hm.col << " (id " << hm.id
          << ", hintmap '" << hm.hintmap_key << "')"
          << " has chunks inside the fixed region of controller " << fixed.col
          << " (id " << fixed.id << ") [0x" << std::hex
          << fixed.base << ", 0x" << (fixed.base + fixed.size) << ")" << std::dec;
      overlap_error(pt_idx, oss.str());
    }
    // Span-only overlap: union every hintmap controller into one component.
    for (std::size_t i = 0; i < infos.size(); ++i) {
      if (!infos[i].hintmap_key.empty() && infos[i].size > 0)
        uf_union(static_cast<int>(i), static_cast<int>(hm_idx));
    }
    return true;
  }

  // Case 3: both have hintmaps — AND their bitsets to detect a shared chunk.
  if ((chunks_to_bitset(chunks_cache[a]) & chunks_to_bitset(chunks_cache[b])).any()) {
    std::ostringstream oss;
    oss << "controller " << ra.col << " (id " << ra.id
        << ", hintmap '" << ra.hintmap_key << "')"
        << " and controller " << rb.col << " (id " << rb.id
        << ", hintmap '" << rb.hintmap_key << "')"
        << " share one or more 64KB chunks";
    overlap_error(pt_idx, oss.str());
  }
  // Disjoint bits, span overlap: union just the two controllers.
  uf_union(static_cast<int>(a), static_cast<int>(b));
  return true;
}

// ---------------------------------------------------------------------------
// redistribute_component
//   Selects K-1 cut points across the combined sorted chunks of all component
//   members, builds one contiguous segment per controller, and stores the
//   new (base, size) in m_hintmap_region_override.
//
//   Cut priority (best = lexicographically largest tuple):
//     (mandatory, gap_size, seg_len, -dist_to_mid)
//   mandatory gaps (spanning a fixed-range controller) always win.
//   Among equal gaps, cut the largest segment closest to its midpoint.
// ---------------------------------------------------------------------------
void
asm_parser::redistribute_component(
    std::size_t pt_idx,
    const std::vector<col_point_info>& infos,
    const std::vector<std::size_t>& members,
    std::vector<std::vector<uint64_t>>& chunks_cache,
    const std::vector<std::pair<uint64_t,uint64_t>>& fixed_rngs)
{
  // Step 1 — collect the union of all set chunks across every member of this component.
  // Each member's chunk list is already sorted (produced by collect_set_chunks);
  // concatenating and re-sorting is safe because the bits are guaranteed disjoint
  // (check_overlap_pair verified that before unioning them).  Dedup is a safety net
  // in case the same chunk appears in two members despite the disjoint check.
  std::vector<uint64_t> chunks;
  for (std::size_t i : members)
    chunks.insert(chunks.end(), chunks_cache[i].begin(), chunks_cache[i].end());

  std::sort(chunks.begin(), chunks.end());
  chunks.erase(std::unique(chunks.begin(), chunks.end()), chunks.end());
  if (chunks.empty()) { return; }  // nothing to save — nothing to redistribute

  const std::size_t n = chunks.size();  // total set chunks across all members
  const std::size_t K = members.size(); // number of controllers = number of segments needed

  // Step 2 — pre-compute attributes for every inter-chunk gap.
  // gap[i] describes the space between chunks[i] and chunks[i+1]:
  //   size      = number of unset (hole) chunks in that gap.  Cutting here removes
  //               'size' hole chunks from the total data transferred, so larger is better.
  //   mandatory = true when a fixed-range controller's region [fs, fe) fits entirely
  //               inside the gap [chunks[i]+1, chunks[i+1]).  Such a gap MUST be a cut
  //               point so no redistributed segment ever spans across fixed territory.
  struct gap_t {
    bool        mandatory = false; // gap spans a fixed-range controller — must cut here
    uint64_t    size      = 0;    // hole chunks between chunks[i] and chunks[i+1]
  };
  std::vector<gap_t> gaps(n - 1);
  for (std::size_t i = 0; i + 1 < n; ++i) {
    gaps[i].size = chunks[i + 1] - chunks[i] - 1;
    const uint64_t lo = chunks[i] + 1, hi = chunks[i + 1];
    // A fixed region [fs, fe) is "inside" the gap when fs >= lo and fe <= hi,
    // meaning the entire fixed region lies in the hole between these two set chunks.
    gaps[i].mandatory = std::any_of(fixed_rngs.begin(), fixed_rngs.end(),
                                    [&](const auto& r) { return r.first >= lo && r.second <= hi; });
  }

  // Step 3 — greedy cut selection: choose K-1 cut positions one at a time.
  //
  // Seed the cut set with all mandatory positions — these are non-negotiable and
  // do not count against the 'remaining' optional cuts needed.
  // Then add optional cuts one per iteration until we have K-1 total.
  //
  // make_segs() rebuilds the current segment list [(start_idx, end_idx)] from
  // the ordered cut set.  A cut at index c ends one segment at chunks[c] and
  // starts the next at chunks[c+1].
  std::set<std::size_t> cuts;
  for (std::size_t i = 0; i + 1 < n; ++i) {
    if (gaps[i].mandatory)
      cuts.insert(i);
  }

  auto make_segs = [&]() {
    std::vector<std::pair<std::size_t,std::size_t>> segs;
    std::size_t s = 0;
    for (std::size_t c : cuts) { segs.emplace_back(s, c); s = c + 1; }
    segs.emplace_back(s, n - 1);
    return segs;
  };

  // 'remaining' = how many more optional cuts we still need after the mandatory ones.
  std::size_t remaining = (K > 1 + cuts.size()) ? K - 1 - cuts.size() : 0;
  for (std::size_t r = 0; r < remaining; ++r) {
    // Scan every gap position inside every current segment and pick the best one.
    // best_t tracks the winning candidate found so far.
    // beats() implements the 4-level lexicographic priority:
    //   (a) mandatory  — correctness: mandatory gaps always take precedence.
    //   (b) gap size   — larger gap removes more hole chunks (minimise total transfer).
    //   (c) seg_len    — prefer cutting the longest current segment (faster convergence).
    //   (d) dist       — among ties, cut closest to the segment midpoint (balance load).
    struct best_t {
      bool        valid    = false;
      bool        mand     = false;
      uint64_t    gap      = 0;
      std::size_t seg_len  = 0;
      std::size_t dist     = std::numeric_limits<std::size_t>::max(); // lower is better
      std::size_t gap_idx  = 0;

      bool beats(const best_t& o) const {
        if (mand    != o.mand)    return mand    > o.mand;    // (a)
        if (gap     != o.gap)     return gap     > o.gap;     // (b)
        if (seg_len != o.seg_len) return seg_len > o.seg_len; // (c)
        return dist < o.dist;                                  // (d)
      }
    } best;

    for (auto [s, e] : make_segs()) {
      if (e <= s) { continue; } // single-element segment: no internal gap to cut
      const std::size_t len = e - s + 1;
      // mid is the gap index that gives the most balanced split (equal set-chunk halves).
      const std::size_t mid = s + len / 2 - 1;
      for (std::size_t i = s; i < e; ++i) {
        const std::size_t dist = (i <= mid) ? mid - i : i - mid;
        best_t cand{true, gaps[i].mandatory, gaps[i].size, len, dist, i};
        if (!best.valid || cand.beats(best))
          best = cand;
      }
    }
    if (best.valid)
      cuts.insert(best.gap_idx); // commit the chosen cut
  }

  // Step 4 — materialise segments from the final cut set.
  // Each segment records its first and last chunk index (both inclusive) and the
  // number of set chunks it contains (used for logging holes = span - set_count).
  struct seg_t { uint64_t first = 0, last = 0; std::size_t set_count = 0; };
  std::vector<seg_t> segs;
  {
    std::size_t s = 0;
    for (std::size_t c : cuts) {
      segs.push_back({chunks[s], chunks[c], c - s + 1}); // [chunks[s], chunks[c]]
      s = c + 1;
    }
    segs.push_back({chunks[s], chunks.back(), n - s});   // final segment
  }

  // Step 5 — pair segments with controllers and write the redistribution overrides.
  // Sort controllers by their original base address so the lowest-address controller
  // receives the lowest-address segment.  This preserves the original ordering and
  // avoids unnecessary data movement between controllers.
  std::vector<std::size_t> sorted = members;
  std::sort(sorted.begin(), sorted.end(),
            [&](std::size_t a, std::size_t b) { return infos[a].base < infos[b].base; });

  const auto n_pairs = std::min(segs.size(), sorted.size());
  log_info() << "Preemption point " << pt_idx
             << ": redistributing " << n_pairs << " segment(s) (min-holes).\n";

  for (std::size_t k = 0; k < n_pairs; ++k) {
    const auto& seg  = segs[k];
    const auto& info = infos[sorted[k]];
    const auto new_base = seg.first * CHUNK_SIZE;
    const auto new_size = (seg.last - seg.first + 1) * CHUNK_SIZE;
    const auto holes    = (seg.last - seg.first + 1) - seg.set_count;

    log_info() << "  controller " << info.col
               << " (hintmap '" << info.hintmap_key << "')"
               << ": [0x" << std::hex << new_base << ", 0x" << (new_base + new_size) << ")"
               << std::dec << " (set=" << seg.set_count
               << " span=" << (seg.last - seg.first + 1) << " holes=" << holes << ")\n";

    m_hintmap_region_override[{info.col, info.hintmap_key}] = {new_base, new_size};
  }
}

// ---------------------------------------------------------------------------
// validate_resolve_hintmap_overlap
//
// BACKGROUND
//   In multi-uC mode all controllers save state into the same partition-wide
//   preemption scratchpad.  cert synchronises them at every preemption point,
//   so the memory regions written by two controllers at the same preemption
//   point must be disjoint.
//
//   All columns have the same number of PREEMPT opcodes (checked earlier), so
//   the n-th PREEMPT of every column belongs to the same preemption point.
//
//   A controller's save region is determined by its hint bitmap:
//     • With a hintmap  – region = [first_set_chunk, last_set_chunk] (span).
//                         The span may be wider than the actual bits because
//                         there can be holes (unset chunks) between set ones.
//     • Without hintmap – region = entire 3 MB of the controller's own column.
//
// TERMINOLOGY
//   chunk      – one 64 KB block (CHUNK_SIZE bytes).  Each hintmap bit = one chunk.
//   span       – contiguous range [first_set_chunk * CHUNK_SIZE,
//                                  (last_set_chunk + 1) * CHUNK_SIZE).
//   hole       – an unset chunk inside a span (transfers as zero bytes but
//                still counts against the span width).
//   fixed region – a no-hintmap controller's region; always contiguous,
//                  cannot be moved.
//
// OVERLAP CASES
//   When two controllers' spans intersect there are three sub-cases:
//
//   Case 1 – both have no hintmap (both fixed):
//     Both regions are solid contiguous ranges; any intersection is a real
//     conflict.  → Hard error, no recovery.
//
//   Case 2 – one fixed, one hintmap:
//     The hintmap controller's span may reach into the fixed region only
//     because of trailing/leading holes.  Check the actual bits:
//       a. If any set chunk of the hintmap lies inside [fixed_first_chunk,
//          fixed_last_chunk) → real overlap → hard error.
//       b. Otherwise the overlap is span-only.  The hintmap's bits are
//          entirely outside the fixed region but its span crosses it.
//          Redistribute all hintmap controllers as one component (see below).
//
//   Case 3 – both have hintmaps:
//     Merge-intersect the two sorted chunk lists in O(n).
//       a. If any chunk appears in both lists → real overlap → hard error.
//       b. Otherwise the overlap is span-only.
//          Union the two controllers for redistribution.
//
// REDISTRIBUTION (span-only overlap resolution)
//   Controllers involved in a span-only overlap are grouped into connected
//   components using union-find (controllers that transitively span-overlap
//   each other end up in the same component).  No-hintmap controllers are
//   never part of a component; their regions are unchanged.
//
//   For each component with K members:
//     1. Collect the union of all set chunks across members, sort and dedup.
//        Call this list C[0..n-1].
//     2. For each gap between consecutive chunks C[i] and C[i+1]:
//          mandatory = true if a fixed-region interval fits entirely inside
//                      the gap (i.e. the gap spans a no-hintmap controller's
//                      territory).  Mandatory gaps must always be cut points
//                      so that redistributed spans never cross fixed regions.
//     3. Select K-1 cut positions by greedy best-first search:
//          - Seed the cut set with all mandatory gap positions.
//          - Iteratively add one optional cut at a time, each time choosing
//            the gap with the lexicographically best score:
//              (a) mandatory flag  – mandatory gaps always win (correctness).
//              (b) gap size        – larger gap removes more hole chunks,
//                                    minimising total transferred data.
//              (c) segment length  – prefer cutting the longest segment first.
//              (d) distance to mid – among ties, cut closest to the segment's
//                                    midpoint for balanced load.
//     4. The K-1 cuts divide C into K contiguous segments.  Each segment's
//        region is [C[seg_start] * CHUNK_SIZE,
//                   (C[seg_end] + 1) * CHUNK_SIZE).
//     5. Sort component members by their original base address.  Pair the
//        lowest-address member with the lowest-address segment, and so on.
//     6. Write the new (base, size) into m_hintmap_region_override keyed by
//        (col, hintmap_key).  parse_hintmap_and_calculate_scratchpad checks
//        this map first, so all subsequent uses of these hintmaps see the
//        redistributed region automatically.
//
// ALGORITHM OUTLINE
//   for each preemption point index idx:
//     infos      = build_point_infos(idx)          // one entry per controller
//     chunks_cache[i] = collect_set_chunks(infos[i]) // bits → chunk indices
//     parent[i]  = i                               // union-find initialisation
//
//     for each pair (a, b) with non-zero size:
//       check_overlap_pair(a, b) → no overlap / hard error / union + redist flag
//
//     if no redistribution needed: continue
//
//     components = group hintmap controllers by union-find root
//     fixed_rngs = chunk ranges of no-hintmap controllers
//
//     for each component:
//       redistribute_component(component, chunks_cache, fixed_rngs)
//         → writes overrides into m_hintmap_region_override
// ---------------------------------------------------------------------------
void
asm_parser::
validate_resolve_hintmap_overlap()
{
  // Step 1 — find the total number of preemption points.
  // All columns must have the same count, so the maximum across all columns
  // equals the shared count.  Columns with fewer entries simply have no opcode
  // at the later indices and are skipped inside the per-point loop below.
  std::size_t num_points = 0;
  for (const auto& [col, pts] : m_preempt_points) {
    if (pts.size() > num_points)
      num_points = pts.size();
  }

  // Step 2 — process one preemption point at a time.
  // Each iteration is self-contained: it collects region info, checks overlaps,
  // and redistributes if needed — all scoped to a single synchronisation barrier.
  for (std::size_t idx = 0; idx < num_points; ++idx) {

    // Step 2a — collect the save region for every active controller.
    // infos[i] = {col, preempt_id, hintmap_key, base, size} for each column
    // that has a PREEMPT opcode at position 'idx'.
    const std::vector<col_point_info> infos = build_point_infos(idx);

    // Step 2b — pre-compute chunk lists for every hintmap controller.
    // Each entry chunks_cache[i] is the sorted list of 64KB chunk indices that
    // controller i's hintmap marks as needing save/restore.
    // Controllers without a hintmap leave their entry as an empty vector;
    // their save region is a solid contiguous range with no holes.
    std::vector<std::vector<uint64_t>> chunks_cache(infos.size());
    for (std::size_t i = 0; i < infos.size(); ++i) {
      if (infos[i].hintmap_key.empty())
        continue;
      auto [ctx, lbl] = split_qualified_hintmap(infos[i].hintmap_key);
      chunks_cache[i] = collect_set_chunks(*this, infos[i].col, ctx, lbl);
    }

    // Step 2c — initialise union-find.
    // parent[i] = i means each controller starts in its own singleton set.
    // check_overlap_pair will merge sets when it finds a span-only overlap
    // that can be fixed by redistribution.
    std::vector<int> parent(infos.size());
    std::iota(parent.begin(), parent.end(), 0);
    bool any_redist = false;

    // Step 2d — check every pair of controllers for overlap.
    // check_overlap_pair either throws (hard bit overlap → no recovery),
    // updates the union-find (span-only overlap → redistribution needed),
    // or returns false (no overlap at all → nothing to do).
    for (std::size_t a = 0; a < infos.size(); ++a) {
      if (infos[a].size == 0) { continue; }  // controller saves nothing, skip
      for (std::size_t b = a + 1; b < infos.size(); ++b) {
        if (infos[b].size == 0) { continue; }
        if (check_overlap_pair(a, b, idx, infos, chunks_cache, parent))
          any_redist = true;
      }
    }

    // Step 2e — if no pair required redistribution, nothing more to do here.
    if (!any_redist) { continue; }

    // Step 2f — path-compressed find to look up the root of each controller's set.
    // Defined here (after the pair loop) so the final component grouping sees the
    // fully-merged parent[] array from all check_overlap_pair calls above.
    auto uf_find = [&](int x) {
      while (parent[x] != x) { parent[x] = parent[parent[x]]; x = parent[x]; }
      return x;
    };

    // Step 2g — group hintmap controllers into connected components.
    // Controllers that transitively span-overlap each other share one root
    // and are collected into the same component vector.
    // No-hintmap (fixed-region) controllers are excluded: their regions are unchanged.
    // Zero-size hintmap controllers (all bits zero) are initially excluded but may
    // be pulled into a component below if mandatory cuts produce more segments than
    // the non-zero members — in that case the zero-size controllers absorb the extra
    // segments so redistribution can assign them a non-zero region.
    std::map<int, std::vector<std::size_t>> components;
    for (std::size_t i = 0; i < infos.size(); ++i) {
      if (infos[i].hintmap_key.empty() || infos[i].size == 0)
        continue;
      components[uf_find(static_cast<int>(i))].emplace_back(i);
    }

    // Count zero-size hintmap controllers (all-zero bitmaps) that could absorb
    // extra segments.  If any component would produce more segments than members,
    // pad the component with zero-size hintmap controllers ordered by column.
    std::vector<std::size_t> zero_hintmap_controllers;
    for (std::size_t i = 0; i < infos.size(); ++i) {
      if (!infos[i].hintmap_key.empty() && infos[i].size == 0)
        zero_hintmap_controllers.push_back(i);
    }

    // Step 2h — collect fixed-range intervals from no-hintmap controllers.
    // These become mandatory cut boundaries inside redistribute_component:
    // no redistributed segment is allowed to span across a fixed region.
    std::vector<std::pair<uint64_t,uint64_t>> fixed_rngs;
    for (const auto& info : infos) {
      if (info.hintmap_key.empty() && info.size > 0) {
        fixed_rngs.emplace_back(info.base / CHUNK_SIZE,
                                (info.base + info.size) / CHUNK_SIZE);
      }
    }

    // Step 2i — redistribute each component.
    // A singleton component with no fixed-range involvement means the controller
    // was pulled into the component by a fixed-vs-hintmap span overlap but its
    // own bits don't actually conflict with anything — skip it.
    //
    // When mandatory cuts would produce more segments than there are non-zero
    // members, pull zero-size hintmap controllers into the component to absorb
    // the extra segments.  This handles the case where a single controller's set
    // bits straddle a fixed region: the zero-bitmap controller on the other side
    // of the fixed region receives the extra segment and generates a save/restore
    // region for it.
    std::size_t zero_used = 0;
    for (auto& [root, members] : components) {
      if (fixed_rngs.empty() && members.size() < 2)
        continue;
      // Count mandatory gaps for this component to estimate extra segments needed.
      // Collect set chunks for this component to compute the gaps.
      {
        std::vector<uint64_t> all_chunks;
        for (std::size_t i : members)
          all_chunks.insert(all_chunks.end(), chunks_cache[i].begin(), chunks_cache[i].end());
        std::sort(all_chunks.begin(), all_chunks.end());
        all_chunks.erase(std::unique(all_chunks.begin(), all_chunks.end()), all_chunks.end());
        std::size_t mandatory_cuts = 0;
        for (std::size_t ci = 0; ci + 1 < all_chunks.size(); ++ci) {
          const uint64_t lo = all_chunks[ci] + 1, hi = all_chunks[ci + 1];
          if (std::any_of(fixed_rngs.begin(), fixed_rngs.end(),
                [&](const auto& r) { return r.first >= lo && r.second <= hi; }))
            ++mandatory_cuts;
        }
        // Number of segments = mandatory_cuts + 1 (plus optional cuts up to K-1).
        // If mandatory_cuts+1 > members.size(), pull in zero-size controllers.
        const std::size_t segments_needed = mandatory_cuts + 1;
        while (members.size() < segments_needed && zero_used < zero_hintmap_controllers.size())
          members.push_back(zero_hintmap_controllers[zero_used++]);
      }
      redistribute_component(idx, infos, members, chunks_cache, fixed_rngs);
    }
  }
}

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
    // Reject clashing save regions before any save/restore code is injected.
    validate_resolve_hintmap_overlap();

    for (const auto& [group, labels] : m_preempt_labels) {
      auto [save_data, restore_data] = get_preempt_save_restore(10 + group);
      auto [save_bd, restore_bd] = get_preempt_save_restore_shimbd(10 + group);
      auto [save_membd, restore_membd] = get_preempt_save_restore_membd(10 + group);
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
