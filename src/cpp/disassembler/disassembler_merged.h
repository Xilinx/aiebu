// SPDX-License-Identifier: MIT
// Copyright (C) 2025-2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_DISASSEMBLER_MERGED_H_
#define AIEBU_DISASSEMBLER_MERGED_H_

#include "elfio/elfio.hpp"
#include "ops/ops.h"

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace aiebu {

// Metadata built from a merged-page (ABI >= 0x21) ELF before disassembly.
class merged_disasm_context {
public:
  struct page_loc {
    int col = -1;
    uint16_t page_idx = 0;

    bool operator<(const page_loc& other) const
    {
      if (col != other.col)
        return col < other.col;
      return page_idx < other.page_idx;
    }
  };

  struct preempt_point {
    uint16_t id = 0;
    uint16_t save_page = 0;
    uint16_t restore_page = 0;
    std::string save_label;
    std::string restore_label;
    std::string hintmap_label;
    std::vector<uint32_t> hintmap_words;
    bool has_hintmap = false;
  };

  // Key: column, page index within column, byte offset of PREEMPT in page text.
  using preempt_key = std::tuple<int, uint16_t, uint32_t>;

  merged_disasm_context() = default;

  static merged_disasm_context build(ELFIO::elfio& elf,
                                     const std::map<uint8_t, isa_op_disasm>* isa_map,
                                     unsigned char abi_version);

  static int parse_column(const std::string& section_name);

  bool is_active() const { return m_active; }

  bool should_skip_page(int col, uint16_t page_idx) const;

  // Label for an out-of-order target page (without '@' prefix). Empty if none.
  std::string page_label(int col, uint16_t page_idx) const;

  // Pending OOO label for a page when encountered during linear walk.
  std::string pending_page_label(int col, uint16_t page_idx) const;

  const preempt_point* preempt_at(int col, uint16_t page_idx, uint32_t text_offset) const;

  int num_columns() const { return m_num_columns; }

private:
  bool m_active = false;
  int m_num_columns = 0;

  std::set<page_loc> m_skip_pages;
  std::map<page_loc, std::string> m_page_labels;
  std::map<preempt_key, preempt_point> m_preempt_points;

  static uint16_t read_page_index(const char* page_data);
  static uint16_t read_in_order_page_len(const char* page_data);
  static size_t find_code_end(const char* page_data, size_t page_data_size,
                              const std::map<uint8_t, isa_op_disasm>* isa_map);
};

} // namespace aiebu

#endif // AIEBU_DISASSEMBLER_MERGED_H_
