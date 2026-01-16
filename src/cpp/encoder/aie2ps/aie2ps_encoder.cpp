// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.
#include "aie2ps_encoder.h"

#include "aiebu/aiebu_error.h"
#include "logger.h"

#include <cassert>
#include <iostream>

namespace aiebu {

void
aie2ps_encoder::
fill_scratchpad(std::shared_ptr<section_writer> padwriter, const std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpads)
{
  for (const auto& pad : scratchpads)
  {
    const auto& content = pad.second->get_content();
    if (content.size())
    {
      assert((void("Pad content size and size doesnt match\n"), content.size() == pad.second->get_size()));
      padwriter->write_bytes(content);
    } else {
      auto size = pad.second->get_size();
      std::vector<uint8_t> zeros(size, 0x00);
      padwriter->write_bytes(zeros);
    }
  }
}

void
aie2ps_encoder::
fill_controlpkt(std::shared_ptr<section_writer> ctrlpktwriter, const std::vector<char>& ctrlpkt)
{
  ctrlpktwriter->write_bytes(ctrlpkt);
}

void
aie2ps_encoder::
fill_control_packet_symbols(std::shared_ptr<section_writer> ctrlpktwriter,
                            std::vector<symbol>& syms)
{
  for (auto& sym : syms) {
    patch_cp_57(ctrlpktwriter, sym.get_pos(), sym.get_addend());
    // reset addend
    sym.set_addend(0);
    ctrlpktwriter->add_symbol(sym);
  }
}

std::vector<std::shared_ptr<writer>>
aie2ps_encoder::
process(std::shared_ptr<preprocessed_output> input)
{
  // encode asm data
  auto tinput = std::static_pointer_cast<aie2ps_preprocessed_output>(input);

  auto& totalcoldata = tinput->get_coldata();
  auto& totalsyms = tinput->get_symbols();
  m_debug.set_annotations(tinput->get_annotations());
  uint32_t optimizatiom_level = tinput->get_optimization_level();
  auto& ctrlpkt = tinput->get_ctrlpkt();
  auto& ctrlpkt_id_map = tinput->get_ctrlpkt_id_map();
  m_dump_flag = tinput->get_debug();

  // for each colnum encode each page
  for (const auto& coldata: totalcoldata) {
    auto colnum = coldata.first;
    for (auto& lpage : coldata.second->m_pages)
      page_writer(lpage, coldata.second->m_scratchpad, coldata.second->m_labelpageindex,
                                 ctrlpkt_id_map, optimizatiom_level);

    if (coldata.second->m_scratchpad.size()) {
      auto padwriter = std::make_shared<section_writer>(get_PadSectionName(colnum), code_section::data);
      fill_scratchpad(padwriter, coldata.second->m_scratchpad);
      twriter.push_back(padwriter); 
    }

    for (const auto& pair : ctrlpkt_id_map) {
      auto ctrlpktwriter = std::make_shared<section_writer>(pair.second, code_section::data);
      fill_controlpkt(ctrlpktwriter, ctrlpkt[pair.second]);
      fill_control_packet_symbols(ctrlpktwriter, totalsyms);
      twriter.push_back(ctrlpktwriter); 
    }
  }

  // Report (only if log level is info or higher)
  if (get_log_level() >= log_level::info)
    m_report.summary(std::cout);

  // Debug JSON serialization
  json dbg_json = m_debug.to_json();

  if (m_dump_flag == asm_dump_flag::full) {
    // Report
    m_report.summary(std::cout);
    // Write to debug_map.json
    std::ofstream file("debug_map.json");
    file << dbg_json.dump(4);  // pretty print with 4-space indent
    file.close();
  }

  // Optional binary dump if debug flag is not disabled.
  if (m_dump_flag != asm_dump_flag::disable) {
    auto dumpwriter = std::make_shared<section_writer>(".dump", code_section::data);
    std::string dbg_str = dbg_json.dump(); // no indent for compact output
    dumpwriter->write_bytes(dbg_str);
    twriter.push_back(dumpwriter);
  }
  return twriter;
}

std::string
aie2ps_encoder::
findKey(const std::map<std::string, std::vector<std::string>>& myMap, const std::string& value) {
  if (value.empty())
    return "";

  for (const auto& pair : myMap) {
    const auto& vec = pair.second;
    if (std::find(vec.begin(), vec.end(), value) != vec.end()) {
      return pair.first;
    }
  }
  throw error(error::error_code::invalid_asm, "No key found corresponding to value:" + value + "\n");
}

void
aie2ps_encoder::
page_writer(page& lpage, std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpad,
  std::map<std::string, uint32_t>& labelpageindex, std::map<uint32_t, std::string>& ctrlpkt_id_map, uint32_t optimization_level)
{
  // encode page
  std::vector<uint8_t> page_header = { 0xFF, 0xFF, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00};
  page_header[2] =  low_8(lpage.get_pagenum());             // Lower 8 bit of page_index
  page_header[3] =  high_8(lpage.get_pagenum());            // Higher 8 bit of page_index
  page_header[4] =  low_8(lpage.get_ooo_page_len_1());      // Lower 8 bit of out_of_order_page_len pdi/save
  page_header[5] =  high_8(lpage.get_ooo_page_len_1());     // Higher 8 bit of out_of_order_page_len pdi/save
  page_header[6] =  low_8(lpage.get_ooo_page_len_2());      // Lower 8 bit of out_of_order_page_len restore
  page_header[7] =  high_8(lpage.get_ooo_page_len_2());     // Higher 8 bit of out_of_order_page_len restore
  page_header[8] =  low_8(lpage.get_cur_page_len());        // Lower 8 bit of cur_page_len
  page_header[9] =  high_8(lpage.get_cur_page_len());       // Higher 8 bit of cur_page_len
  page_header[10] =  low_8(lpage.get_in_order_page_len());  // Lower 8 bit of in_order_page_len
  page_header[11] =  high_8(lpage.get_in_order_page_len()); // Higher 8 bit of in_order_page_len

  auto pagenum = lpage.get_pagenum();
  auto colnum = lpage.get_colnum();
  // create state
  std::vector<std::shared_ptr<asm_data>> all;
  all.insert(all.end(), lpage.m_text.begin(), lpage.m_text.end());
  all.insert(all.end(), lpage.m_data.begin(), lpage.m_data.end());
  std::shared_ptr<assembler_state> page_state = create_assembler_state(m_isa, all, scratchpad, labelpageindex, ctrlpkt_id_map, optimization_level,false);

  auto textwriter = std::make_shared<section_writer>(get_TextSectionName(colnum, pagenum), code_section::text);
  auto datawriter = std::make_shared<section_writer>(get_DataSectionName(colnum, pagenum), code_section::data);

  textwriter->write_bytes(page_header);

  // encode text section
  offset_type offset = textwriter->tell();
  std::vector<symbol> tsym;
  std::string fid;
  for (const auto& text : lpage.m_text)
  {
    //TODO add debug info
    std::string name = text->get_operation()->get_name();
    offset_type pc_low, pc_high;

    // Text section dump is default generated
    if (name == "start_job" || name == "start_job_deferred" || name == "start_cond_job_preempt") {
      pc_low = pagenum * PAGE_SIZE + textwriter->tell();
      // Note: eopnum=0 passed since makeunique=false means eopnum is not used
      pc_high = pc_low + page_state->m_jobmap[page_state->gen_job_name(false, text, 0)]->get_size() - 1;
      fid = m_debug.add_function(text->get_file(), name + "_" + page_state->gen_job_name(false, text, 0), pc_high, pc_low, colnum, pagenum);
    }
    pc_low = pagenum * PAGE_SIZE + textwriter->tell();
    pc_high = pc_low + (*m_isa)[name]->serializer(text->get_operation()->get_args())->size(*page_state) - 1;
    m_debug.add_textline(fid, text->get_linenumber(), pc_high, pc_low, text->get_line(), text->get_annotation_index());

    if (text->isOpcode())
    {
      page_state->set_pos(textwriter->tell() - offset);
      std::vector<uint8_t> ret = (*m_isa)[name]->serializer(text->get_operation()->get_args())
                                               ->serialize(page_state, tsym, colnum, pagenum);
      textwriter->write_bytes(ret);
    } else 
      throw error(error::error_code::internal_error, "Invalid operation: " + name + " in TEXT section !!!");
  }

  std::vector<symbol> dsym;
  // encode data section
  for (const auto& data : lpage.m_data)
  {
    page_state->set_pos(datawriter->tell() + textwriter->tell() - offset);
    std::string name = data->get_operation()->get_name();
    if (!name.compare("eof"))
      continue;
    if (data->isLabel())
    {
      // TODO assert
    } else if (data->isOpcode())
    {
      // data section dump is only generated in case of full dump.
      if (m_dump_flag == asm_dump_flag::full) {
        offset_type pc_low = pagenum * PAGE_SIZE + textwriter->tell() + datawriter->tell();
        offset_type pc_high = pc_low + (*m_isa)[name]->serializer(data->get_operation()->get_args())->size(*page_state) - 1;
        m_debug.add_dataline(fid, data->get_linenumber(), pc_high, pc_low, data->get_line(), data->get_annotation_index());
      }
      std::vector<uint8_t> ret = (*m_isa)[name]->serializer(data->get_operation()->get_args())
                                               ->serialize(page_state, dsym, colnum, pagenum);
      datawriter->write_bytes(ret);
    } else 
      throw error(error::error_code::internal_error, "Invalid operation: " + name + " in DATA section !!!");
  }

  for (auto &spad : page_state->m_patch)
  {
    for (auto& arg : spad.second)
    {
      offset = page_state->parse_num_arg(arg);
      patch57(textwriter, datawriter, offset + page_header.size(),
              page_state->m_scratchpad[spad.first.substr(1)]->get_base() + page_state->m_scratchpad[spad.first.substr(1)]->get_offset());
    }
  }

  datawriter->padding(PAGE_SIZE-textwriter->tell());

  textwriter->add_symbols(tsym);
  datawriter->add_symbols(dsym);
  twriter.push_back(textwriter);
  twriter.push_back(datawriter);
  m_report.addpage(lpage, page_state, textwriter->tell(), datawriter->tell());

  // TODO add size and generate report
}

void
aie2ps_encoder::
patch57(const std::shared_ptr<section_writer> textwriter, std::shared_ptr<section_writer> datawriter, offset_type offset, uint64_t patch)
{
  offset = offset - textwriter->tell();
  uint64_t bd1 = datawriter->read_word(offset + 1*4); // NOLINT
  uint64_t bd2 = datawriter->read_word(offset + 2*4); // NOLINT
  uint64_t bd8 = datawriter->read_word(offset + 8*4); // NOLINT
  uint64_t arg = ((bd8 & 0x1FF) << 48) + ((bd2 & 0xFFFF) << 32) + (bd1 & 0xFFFFFFFF); // NOLINT
  patch = arg + patch;
  datawriter->write_word_at(offset + 1*4, patch & 0xFFFFFFFF); // NOLINT
  datawriter->write_word_at(offset + 2*4, ((patch >> 32) & 0xFFFF) | (bd2 & 0xFFFF0000)); // NOLINT
  datawriter->write_word_at(offset + 8*4, ((patch >> 48) & 0x1FF) | (bd8 & 0xFFFFFE00));  // NOLINT
}

void
aie2ps_encoder::
patch_cp_57(const std::shared_ptr<section_writer> ctrlpktwriter, offset_type offset, uint64_t patch)
{
  uint64_t bd1 = ctrlpktwriter->read_word(offset + 2*4); // NOLINT
  uint64_t bd2 = ctrlpktwriter->read_word(offset + 3*4); // NOLINT
  uint64_t arg = ((bd2 & 0xFFFF) << 32) + (bd1 & 0xFFFFFFFF); // NOLINT
  patch = arg + patch;
  ctrlpktwriter->write_word_at(offset + 2*4, patch & 0xFFFFFFFF); // NOLINT
  ctrlpktwriter->write_word_at(offset + 3*4, (((patch >> 32) & 0xFFFF) | (bd2 & 0xFFFF0000))); // NOLINT
}

}
