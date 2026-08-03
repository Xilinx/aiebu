// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_ENCODER_AIE2PS_REPORT_H_
#define AIEBU_ENCODER_AIE2PS_REPORT_H_


#include <string>
#include <vector>
#include <memory>
#include <map>
#include <iostream>
#include "json/nlohmann/json.hpp"
#include "utils.h"
#include "assembler_state.h"
#include "asm/asm_parser.h"
#include "asm/page.h"
#include "version.h"

namespace aiebu {

using json = nlohmann::json;

// Class hold info for each line in jod/function
class Line {
public:
  Line(uint32_t linenumber, offset_type high_pc, offset_type low_pc, const std::string& opcode, int annotation_index)
      : m_linenumber(linenumber), m_highpc(high_pc), m_lowpc(low_pc), m_opcode(opcode), m_annotation_index(annotation_index) {}

  // Writes JSON directly to out, avoiding a per-line heap allocation.
  void write_json(std::ostream& out, int sno, uint32_t column, pageid_type page_num,
                  const std::string& filename, const std::vector<annotation_type>& annotations) const {
    out << "{\"sno\":" << sno
        << ",\"operation\":" << json(m_opcode).dump()
        << ",\"opcode_size\":" << (m_highpc - m_lowpc + 1)
        << ",\"column\":" << column
        << ",\"page_index\":" << page_num
        << ",\"page_offset\":" << m_lowpc
        << ",\"line\":" << m_linenumber
        << ",\"file\":" << json(filename).dump();
    if (m_annotation_index != -1 && m_annotation_index < static_cast<int>(annotations.size())) {
      out << ",\"annotation\":{"
          << "\"id\":"          << json(annotations[m_annotation_index].get_id()).dump()
          << ",\"name\":"       << json(annotations[m_annotation_index].get_name()).dump()
          << ",\"description\":" << json(annotations[m_annotation_index].get_description()).dump()
          << '}';
    }
    out << '}';
  }

private:
  uint32_t m_linenumber;
  offset_type m_highpc;
  offset_type m_lowpc;
  std::string m_opcode;
  int m_annotation_index;
};

// Class hold info for each job/function
class Function {
public:

  // Takes a file index instead of the filename
  // string, avoiding a heap allocation per Function object.
  Function(std::shared_ptr<const detail::filename_table> filename_table,
           uint32_t file_idx, const std::string& name, offset_type high_pc, offset_type low_pc,
           uint32_t col, pageid_type pagenum)
        : m_filename_table(std::move(filename_table)), m_file_idx(file_idx), m_name(name),
          m_colnum(col), m_pagenum(pagenum), m_highpc(high_pc), m_lowpc(low_pc) {}

  void add_textline(std::shared_ptr<Line> line) { m_textlines.push_back(std::move(line)); }
  void add_dataline(std::shared_ptr<Line> line) { m_datalines.push_back(std::move(line)); }

  const std::vector<std::shared_ptr<Line>>& get_textlines() const { return m_textlines; }
  const std::vector<std::shared_ptr<Line>>& get_datalines() const { return m_datalines; }

  const std::string& get_filename() const { return m_filename_table->lookup_filename(m_file_idx); }
  const std::string& get_name() const { return m_name; }
  uint32_t get_column() const { return m_colnum; }
  pageid_type get_pagenum() const { return m_pagenum; }
  offset_type get_highPc() const { return m_highpc; }
  offset_type get_lowPc() const { return m_lowpc; }

private:
  std::shared_ptr<const detail::filename_table> m_filename_table;
  uint32_t m_file_idx;
  std::string m_name;
  uint32_t m_colnum;
  pageid_type m_pagenum;
  offset_type m_highpc, m_lowpc;
  std::vector<std::shared_ptr<Line>> m_textlines, m_datalines;
};

// Class to generate debug section for tracing
class Debug {
public:
  void set_annotations(std::vector<annotation_type> annotations) {
    m_annotation_list = std::move(annotations);
  }

  void set_filename_table(std::shared_ptr<const detail::filename_table> table) {
    m_filename_table = std::move(table);
  }

  std::string add_function(uint32_t file_idx, const std::string& name, offset_type high_pc, offset_type low_pc, uint32_t col, pageid_type pagenum) {
    // source/file/column/page do not overwrite previously recorded functions.
    std::string key = std::to_string(file_idx) + "_" + std::to_string(col) + "_"
                      + std::to_string(pagenum) + "_" + name;
    functions[key] = std::make_shared<Function>(m_filename_table, file_idx, name, high_pc, low_pc, col, pagenum);
    insertion_order.push_back(key);
    return key;
  }

  void add_textline(const std::string& func, uint32_t line, offset_type hi, offset_type lo, const std::string& opcode, int ann) {
    functions.at(func)->add_textline(std::make_shared<Line>(line, hi, lo, opcode, ann));
  }

  void add_dataline(const std::string& func, uint32_t line, offset_type hi, offset_type lo, const std::string& opcode, int ann) {
    functions.at(func)->add_dataline(std::make_shared<Line>(line, hi, lo, opcode, ann));
  }

  // Serialises the entire debug array directly into out,
  // never building a large in-memory JSON tree.
  void write_json(std::ostream& out) const {
    out << "{\"debug\":[";
    bool first = true;
    int sno = 1;
    for (const auto& key : insertion_order) {
      const auto& func = functions.at(key);
      for (const auto& line : func->get_textlines()) {
        if (!first) out << ',';
        first = false;
        line->write_json(out, sno++, func->get_column(), func->get_pagenum(), func->get_filename(), m_annotation_list);
      }
      for (const auto& line : func->get_datalines()) {
        if (!first) out << ',';
        first = false;
        line->write_json(out, sno++, func->get_column(), func->get_pagenum(), func->get_filename(), m_annotation_list);
      }
    }
    out << "]}";
  }

private:
  std::shared_ptr<const detail::filename_table> m_filename_table;
  std::map<std::string, std::shared_ptr<Function>> functions;
  std::vector<std::string> insertion_order;
  std::vector<annotation_type> m_annotation_list;
};


// Class to generate asm report summary
class asm_report {
  class report_page {
  public:
    uint32_t m_colnum;
    pageid_type m_pagenum;
    offset_type m_textsize;
    offset_type m_datasize;
    offset_type m_padsize;
    std::vector<jobid_type> m_jobids;
    std::map<barrierid_type, std::vector<jobid_type>> m_localbarriermap;
    std::map<jobid_type, std::vector<jobid_type>> m_joblaunchmap;

    report_page(uint32_t colnum, pageid_type pagenum, offset_type textsize, offset_type datasize, offset_type padsize,
                const std::vector<jobid_type>& jobids,
                const std::map<barrierid_type, std::vector<jobid_type>>& barriermap,
                const std::map<jobid_type, std::vector<jobid_type>>& launchmap)
        : m_colnum(colnum), m_pagenum(pagenum), m_textsize(textsize), m_datasize(datasize), m_padsize(padsize),
          m_jobids(jobids), m_localbarriermap(barriermap), m_joblaunchmap(launchmap) {}

  };

  std::string m_build_id = aiebu_build_version_hash;
  std::map<uint32_t, std::vector<report_page>> m_colpages;
public:
  void addpage(page& lpage, std::shared_ptr<assembler_state> page_state, offset_type textsize, offset_type datasize, offset_type padsize); // Implementation assumed

  void summary(std::ostream& output);
};

}
#endif //AIEBU_ENCODER_AIE2PS_REPORT_H_
