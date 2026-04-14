// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.
#include "asm_parser.h"
#include "preprocessor/aie4/aie4_save_restore_map_prebuilt.h"

#include "aiebu/aiebu_error.h"

#include <fstream>
#include <iomanip>
#include <optional>
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
  // get col list
  std::vector<uint32_t> keys;

  std::transform(
    m_col.begin(),
    m_col.end(),
    std::back_inserter(keys),
    [](const std::unordered_map<uint32_t, col_data>::value_type &pair){return pair.first;});
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
parse_lines()
{
  directive_list[".attach_to_group"] = std::make_shared<attach_to_group_directive>();
  directive_list[".include"] = std::make_shared<include_directive>();
  directive_list[".endl"] = std::make_shared<end_of_label_directive>();
  directive_list[".setpad"] = std::make_shared<pad_directive>();
  directive_list[".section"] = std::make_shared<section_directive>();
  directive_list[".partition"] = std::make_shared<partition_directive>();
  directive_list[".target"] = std::make_shared<target_directive>();
  directive_list[".aie_row_topology"] = std::make_shared<aie_row_topology_directive>();
  std::string file = "default";
  parse_lines(m_data, file);

  // After all parsing is done, inject actual save/restore code
  finalize_preempt();
}

void
asm_parser::
parse_lines(const std::vector<char>& data, std::string& file)
{
  //parse asm code
  const static regex COMMENT_REGEX("^;(.*)$");
  const static regex LABEL_REGEX("^([a-zA-Z0-9_]+):$");
  const static regex OP_REGEX("^([.a-zA-Z0-9_]+)(?:[ \\t]+(.+))?$");
  const static regex DIRCETIVE_REGEX(".^([a-zA-Z0-9_]+)(?:[ \\t]+(.+))?$");

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

    // Check for Directive (starts with .) - only check regex if prefix matches
    if (line[0] == '.' && operate_directive(line))
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
        insert_col_asmdata(std::make_shared<asm_data>(operation(sm[1].str(), ""),
                                                      operation_type::label, code_section::unknown, 0,
                                                      (uint32_t)-1, linenumber, file));
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
        if (get_target_type() == "aie2ps")
          throw error(error::error_code::internal_error, "PREEMPT opcode is not supported for aie2ps target");

        // Get current group (default to 0 if no attach_to_group yet)
        int current_group = (m_current_col >= 0) ? m_current_col : 0;

        // Record preempt label for this group (save_N/restore_N) - for backward compatibility
        record_preempt_label(current_group);

        // Parse arguments: id, @save, @restore, [@hintmap]
        std::vector<std::string> args;
        if (!arg_str.empty()) {
          std::stringstream ss(arg_str);
          std::string arg;
          while (std::getline(ss, arg, ',')) {
            args.push_back(trim(arg));
          }
        }

        // Extract first argument (id)
        std::string first_arg;
        if (args.empty())
          throw error(error::error_code::internal_error, "PREEMPT opcode has no arguments");

        first_arg = args[0];
        if (first_arg.empty())
          throw error(error::error_code::internal_error, "PREEMPT opcode has empty first argument");

        // Extract hintmap label if present (4th argument, optional)
        std::string hintmap_label;
        if (args.size() >= 4) {
          hintmap_label = args[3];
          // Remove @ prefix if present
          if (!hintmap_label.empty() && hintmap_label[0] == '@') {
            hintmap_label = hintmap_label.substr(1);
          }
          if (!hintmap_label.empty()) {
            std::transform(hintmap_label.begin(), hintmap_label.end(), hintmap_label.begin(),
                           [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
          }
        }

        // Get save/restore labels - specific to hintmap if present, otherwise use group labels
        std::pair<std::string, std::string> labels;
        if (!hintmap_label.empty()) {
          // Qualify the hintmap key with its label scope so that the same label
          // name in different scopes (e.g. "default" vs "default:pdi") is treated
          // as a distinct hintmap.
          std::string qualified_key = m_current_label + ":" + hintmap_label;
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

        // Build new arg_str: <first_arg>, @<save_label>, @<restore_label>[, @<hintmap_label>]
        if (!hintmap_label.empty())
          arg_str = first_arg + ", @" + labels.first + ", @" + labels.second + ", @" + hintmap_label;
        else
          arg_str = first_arg + ", @" + labels.first + ", @" + labels.second;

        log_info() << "PREEMPT opcode: updated arg_str to '" << arg_str
                   << "' (hintmap: '" << hintmap_label << "', labels: @" << labels.first
                   << " / @" << labels.second << ")" << std::endl;

        line = op_name + "\t" + arg_str;
      }

      insert_col_asmdata(std::make_shared<asm_data>(operation(op_name, arg_str),
                                                    operation_type::op, code_section::unknown, 0, (uint32_t)-1,
                                                    linenumber, file));
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
  auto& save_restore_map = get_aie4_save_restore();
  auto it = save_restore_map.find(num_cols);
  if (it != save_restore_map.end())
    return it->second;
  return {{}, {}};
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

  // Contiguity check: no gaps allowed between first and last set bit
  if (first_bit != NO_BIT) {
    const uint64_t span = last_bit - first_bit + 1;
    if (span != set_bits)
      throw error(error::error_code::invalid_asm,
                  "hintmap '" + hintmap_label + "' has non-contiguous bits "
                  "(first=bit " + std::to_string(first_bit)
                  + ", last=bit "  + std::to_string(last_bit)
                  + ", span="      + std::to_string(span)
                  + ", set="       + std::to_string(set_bits) + ")");
  }

  const uint64_t scratchbase = (first_bit != NO_BIT) ? (first_bit * CHUNK_SIZE) : DEFAULT_BASE;
  const uint64_t size        = set_bits * CHUNK_SIZE;

  log_info() << "Hintmap parsed for group " << group << " (col " << group << "): "
             << "scratchbase=0x" << std::hex << scratchbase
             << ", size=0x"      << size << " (" << std::dec
             << (size / (1024ULL * 1024ULL)) << "MB, " << set_bits << " chunks)" << std::endl;

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
//   size = set_bits * 64KB.  Returns defaults when hintmap_label is empty.
// ---------------------------------------------------------------------------
std::pair<uint64_t, uint64_t>
asm_parser::
parse_hintmap_and_calculate_scratchpad(int group,
                                       const std::string& search_context,
                                       const std::string& hintmap_label)
{
  constexpr uint64_t DEFAULT_SIZE = 9ULL * 1024ULL * 1024ULL;
  constexpr uint64_t DEFAULT_BASE = 0x0ULL;

  if (hintmap_label.empty())
    return {DEFAULT_BASE, DEFAULT_SIZE};

  const std::string ctx     = find_hintmap_context(group, search_context, hintmap_label);
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

// ---------------------------------------------------------------------------
// BD range helpers
//   Split [address, address+size) across three 3MB scratchpad regions and
//   divide each region's intersection into equal sub-ranges (BD slots).
//
//   The 9MB scratchpad is partitioned into:
//     Region 0 : [0 MB, 3 MB)
//     Region 1 : [3 MB, 6 MB)
//     Region 2 : [6 MB, 9 MB)
//
//   For save:    parts_per_region = 2  →  6 (base, size) pairs total
//   For restore: parts_per_region = 4  → 12 (base, size) pairs total
//
//   Sub-ranges with no overlap get size = 0 (base = region start).
//   The last sub-range in an overlapping region absorbs any remainder from
//   integer division so that all sizes sum exactly to the intersection size.
// ---------------------------------------------------------------------------
static std::vector<std::pair<uint64_t, uint64_t>>
compute_bd_ranges(uint32_t num_bd_per_column, uint64_t address, uint64_t size, uint32_t parts_per_region)
{
  constexpr uint64_t MB3       = 3ULL * 1024ULL * 1024ULL;  // 3 MB
  uint32_t N_REGIONS = num_bd_per_column;

  std::vector<std::pair<uint64_t, uint64_t>> result;
  result.reserve(static_cast<std::size_t>(N_REGIONS * parts_per_region));

  const uint64_t alloc_end = address + size;

  for (uint32_t r = 0; r < N_REGIONS; ++r) {
    const uint64_t reg_base = static_cast<uint64_t>(r) * MB3;
    const uint64_t reg_end  = reg_base + MB3;

    // Intersection of [address, alloc_end) with [reg_base, reg_end)
    const uint64_t isect_base = std::max(address, reg_base);
    const uint64_t isect_end  = std::min(alloc_end, reg_end);

    if (isect_end <= isect_base) {
      // No overlap — every slot for this region has size 0
      for (uint32_t p = 0; p < parts_per_region; ++p)
        result.push_back({reg_base, 0ULL});
      continue;
    }

    const uint64_t isect_size = isect_end - isect_base;
    const uint64_t part_size  = isect_size / static_cast<uint64_t>(parts_per_region);

    for (uint32_t p = 0; p < parts_per_region; ++p) {
      const uint64_t part_base = isect_base + static_cast<uint64_t>(p) * part_size;
      // Last slot absorbs any remainder from integer division
      const uint64_t this_size = (p == parts_per_region - 1)
                                 ? (isect_end - part_base)
                                 : part_size;
      result.push_back({part_base, this_size});
    }
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
                uint64_t byte_addr, uint64_t size_bytes)
{
  const std::string label_def = label + ":";
  auto pos = text.find(label_def);
  if (pos == std::string::npos)
    return;

  byte_addr = byte_addr % 0x300000;
  byte_addr = (byte_addr/4) + 0x800000;


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
    // Split "label_context:hintmap_name" on the last ':'.
    // The context itself can contain ':' (e.g. "default:pdi"), so rfind is correct.
    std::string ctx, label_name;
    const auto sep = qualified_key.rfind(':');
    if (sep != std::string::npos) {
      ctx        = qualified_key.substr(0, sep);
      label_name = qualified_key.substr(sep + 1);
    } else {
      label_name = qualified_key;  // no context prefix (shouldn't happen)
    }

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

// ---------------------------------------------------------------------------
// inject_hintmap_save_restore
//   Register scratchpad, patch template labels, and parse save+restore asm.
// ---------------------------------------------------------------------------
void
asm_parser::inject_hintmap_save_restore(int col,
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

  // Patch BD address/length fields directly in the ASM text
  std::string save_text(save_chars.begin(), save_chars.end());
  for (std::size_t i = 0; i < save_bd.size() && i < save_bd_ranges.size(); ++i) {
    patch_bd_in_asm(save_text, save_bd[i], save_bd_ranges[i].first, save_bd_ranges[i].second);
    patch_membd_in_asm(save_text, save_membd[i], save_bd_ranges[i].first, save_bd_ranges[i].second);
  }
  save_chars.assign(save_text.begin(), save_text.end());

  std::string restore_text(restore_chars.begin(), restore_chars.end());
  for (std::size_t i = 0; i < restore_bd.size() && i < restore_bd_ranges.size(); ++i) {
    patch_bd_in_asm(restore_text, restore_bd[i], restore_bd_ranges[i].first, restore_bd_ranges[i].second);
    patch_membd_in_asm(restore_text, restore_membd[i], restore_bd_ranges[i].first, restore_bd_ranges[i].second);
  }
  restore_chars.assign(restore_text.begin(), restore_text.end());

  m_current_col = col;
  set_data_state(false);
  parse_lines(save_chars, save_file_mod);
  pop_data_state();

  set_data_state(false);
  parse_lines(restore_chars, restore_file_mod);
  pop_data_state();
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
asm_parser::inject_default_save_restore(int col,
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

  // Patch BD address/length fields directly in the ASM text
  std::vector<char> save_chars(save_data.begin(), save_data.end());
  std::string save_text(save_chars.begin(), save_chars.end());
  for (std::size_t i = 0; i < save_bd.size() && i < save_bd_ranges.size(); ++i) {
    patch_bd_in_asm(save_text, save_bd[i], save_bd_ranges[i].first, save_bd_ranges[i].second);
    patch_membd_in_asm(save_text, save_membd[i], save_bd_ranges[i].first, save_bd_ranges[i].second);
  }
  save_chars.assign(save_text.begin(), save_text.end());

  std::vector<char> restore_chars(restore_data.begin(), restore_data.end());
  std::string restore_text(restore_chars.begin(), restore_chars.end());
  for (std::size_t i = 0; i < restore_bd.size() && i < restore_bd_ranges.size(); ++i) {
    patch_bd_in_asm(restore_text, restore_bd[i], restore_bd_ranges[i].first, restore_bd_ranges[i].second);
    patch_membd_in_asm(restore_text, restore_membd[i], restore_bd_ranges[i].first, restore_bd_ranges[i].second);
  }
  restore_chars.assign(restore_text.begin(), restore_text.end());

  m_current_col = col;
  set_data_state(false);
  parse_lines(save_chars, save_file_mod);
  pop_data_state();

  set_data_state(false);
  parse_lines(restore_chars, restore_file_mod);
  pop_data_state();
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
      inject_hintmap_save_restore(group, save_file, restore_file,
                                  save_data, restore_data, template_labels, grp,
                                  save_bd, restore_bd, save_membd, restore_membd);
    }
    // Third pass: update PREEMPT opcode arguments to use the correct shared labels.
    update_preempt_opcodes(group);
  }

  // Handle PREEMPT opcodes with no hintmap (use group-level default labels)
  if (m_preempt_without_hintmap.count(group))
    inject_default_save_restore(group, save_file, restore_file,
                                save_data, restore_data,
                                save_bd, restore_bd, save_membd, restore_membd);
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
      process_preempt_group(group, group / 2 + 1,
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
    process_preempt_group(0, 1, save_file, restore_file, save_data, restore_data, save_bd, restore_bd, save_membd, restore_membd);
  }
}

void
attach_to_group_directive::
operate(std::shared_ptr<asm_parser> parserptr, const smatch& sm)
{
  m_parserptr = parserptr;
  verify_match(sm, error::error_code::invalid_asm, "Invalid attach_to_group directive argument\n");

  // dummy eof added if col change happens before eof
  m_parserptr->insert_col_asmdata(std::make_shared<asm_data>(operation("eof", ""),
                                                    operation_type::op, code_section::unknown, 0, (uint32_t)-1,
                                                    0, "default"));
  m_parserptr->set_current_col(std::stoi(sm[2].str()));
  m_parserptr->set_data_state(false);
}

void
section_directive::
operate(std::shared_ptr<asm_parser> parserptr, const smatch& sm)
{
  m_parserptr = parserptr;
  verify_match(sm, error::error_code::invalid_asm, ".section directive requires arguments\n");

  std::vector<std::string> args = splitoption(sm[2].str().c_str(), ',');
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
operate(std::shared_ptr<asm_parser> parserptr, const smatch& sm)
{
  m_parserptr = parserptr;
  static const regex pattern(R"(\.partition\s+(\d+)(column|core:(\d+)mem))");
  smatch match;
  log_info() << "PARTITION:" << sm[0].str() << "\n";
  std::string line = sm[0].str();
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
operate(std::shared_ptr<asm_parser> parserptr, const smatch& sm)
{
  m_parserptr = parserptr;
  // Pattern: .target <arch>-<sub-arch> or .target <arch>
  static const regex pattern(R"(\.target\s+([a-zA-Z0-9]+)(?:-([a-zA-Z0-9]+))?)");
  smatch match;
  log_info() << "TARGET:" << sm[0].str() << std::endl;
  std::string line = sm[0].str();
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
operate(std::shared_ptr<asm_parser> parserptr, const smatch& sm)
{
  m_parserptr = parserptr;
  // Pattern: .aie_row_topology A-B-C-D
  // Where: A=num_south_shim, B=num_memtile_row, C=num_coretile_row, D=num_north_shim
  // Example: .aie_row_topology 1-1-4-0
  static const regex pattern(R"(\.aie_row_topology\s+(\d+)-(\d+)-(\d+)-(\d+))");
  smatch match;
  log_info() << "AIE_ROW_TOPOLOGY:" << sm[0].str() << std::endl;
  std::string line = sm[0].str();
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
operate(std::shared_ptr<asm_parser> parserptr, const smatch& sm)
{
  m_parserptr = parserptr;
  //std::vector<std::string> args = splitoption(sm[2].str().c_str(), ',');
  std::string file = sm[2].str();//args[0];
  if (file.size() >= 2 && file.front() == '"' && file.back() == '"')
    file =  file.substr(1, file.size() - 2);

  if (read_include_file(file))
    return;
  throw error(error::error_code::internal_error, "File " + file + " not exist\n");
}

void
end_of_label_directive::
operate(std::shared_ptr<asm_parser> parserptr, const smatch& sm)
{
  m_parserptr = parserptr;

  std::string label = m_parserptr->top_label();
  m_parserptr->pop_label();

  verify_match(sm, error::error_code::invalid_asm, ".endl directive requires a label argument\n");

  std::vector<std::string> args = splitoption(sm[2].str().c_str(), ',');
  if (label.compare(args[0]))
    throw error(error::error_code::internal_error, "endl label mismatch (" + label + " != " + args[0] + ")\n");
  m_parserptr->pop_data_state();
}

void
pad_directive::
operate(std::shared_ptr<asm_parser> parserptr, const smatch& sm)
{
  m_parserptr = parserptr;
  verify_match(sm, error::error_code::invalid_asm, ".setpad directive requires arguments\n");

  std::vector<std::string> args = splitoption(sm[2].str().c_str(), ',');

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
