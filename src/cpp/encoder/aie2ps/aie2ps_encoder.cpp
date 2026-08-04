// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.
#include "aie2ps_encoder.h"

#include "aiebu/aiebu_error.h"
#include "logger.h"

#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>

namespace aiebu {

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
  m_debug.set_filename_table(tinput->get_filename_table());
  uint32_t optimizatiom_level = tinput->get_optimization_level();
  auto& ctrlpkt = tinput->get_ctrlpkt();
  auto& ctrlpkt_id_map = tinput->get_ctrlpkt_id_map();
  m_dump_flag = tinput->get_debug();

  for (const auto& coldata: totalcoldata) {
    auto colnum = coldata.first;

    if (use_merged_ctrltext_sections()) {
      // For each column, encode all pages into a single merged section (.ctrltext.<col>).
      // Each page occupies exactly PAGE_SIZE bytes: [header 16B][text][data][padding to PAGE_SIZE].
      auto merged_writer =
          std::make_shared<section_writer>(merged_ctrltext_section_name(colnum), code_section::text);

      struct per_page_sym_info {
        offset_type pre_tell;
        std::vector<symbol> syms;
      };
      std::vector<per_page_sym_info> all_page_info;

      for (auto& lpage : coldata.second->m_pages) {
        offset_type pre_tell = merged_writer->tell();
        std::vector<symbol> page_syms;
        page_writer(lpage, coldata.second->m_scratchpad, coldata.second->m_labelpageindex, ctrlpkt_id_map,
                    optimizatiom_level, merged_writer, &page_syms);
        all_page_info.push_back({pre_tell, std::move(page_syms)});
      }

      std::vector<symbol> merged_syms;
      for (auto& info : all_page_info) {
        offset_type correction = info.pre_tell;
        for (auto& sym : info.syms) {
          sym.set_pos(sym.get_pos() + correction);
          sym.set_section_name(merged_writer->get_name());
        }
        merged_syms.insert(merged_syms.end(), info.syms.begin(), info.syms.end());
      }
      if (!merged_syms.empty())
        merged_writer->add_symbols(merged_syms);

      twriter.push_back(merged_writer);
    } else {
      for (auto& lpage : coldata.second->m_pages)
        page_writer(lpage, coldata.second->m_scratchpad, coldata.second->m_labelpageindex, ctrlpkt_id_map,
                    optimizatiom_level, nullptr, nullptr);
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

  if (m_dump_flag == asm_dump_flag::full) {
    // Write to debug_map.json using streaming serialisation.
    std::ofstream file("debug_map.json");
    m_debug.write_json(file);
    file.close();
  }

  // Optional binary dump if debug flag is not disabled.
  // Stream JSON directly into the section vector via a thin streambuf shim.
  if (m_dump_flag != asm_dump_flag::disable) {
    auto dumpwriter = std::make_shared<section_writer>(".dump", code_section::data);
    std::vector<uint8_t> dump_data;
    {
      struct vec_streambuf : std::streambuf {
        std::vector<uint8_t>& buf;
        explicit vec_streambuf(std::vector<uint8_t>& b) : buf(b) {}
        std::streamsize xsputn(const char* s, std::streamsize n) override {
          buf.insert(buf.end(), reinterpret_cast<const uint8_t*>(s),
                     reinterpret_cast<const uint8_t*>(s) + n);
          return n;
        }
        int overflow(int c) override {
          if (c != EOF) buf.push_back(static_cast<uint8_t>(c));
          return c;
        }
      } vsb(dump_data);
      std::ostream os(&vsb);
      m_debug.write_json(os);
    }
    log_info() << ".dump JSON size: " << dump_data.size() << " bytes\n";
    dumpwriter->set_data(dump_data);
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
            std::map<std::string, uint32_t>& labelpageindex, std::map<uint32_t, std::string>& ctrlpkt_id_map,
            uint32_t optimization_level, std::shared_ptr<section_writer> merged_writer,
            std::vector<symbol>* merged_syms)
{
  const bool merged = use_merged_ctrltext_sections();
  if (merged && (merged_writer == nullptr || merged_syms == nullptr))
    throw error(error::error_code::internal_error,
                "page_writer: merged layout requires non-null merged_writer and merged_syms\n");
  if (!merged && (merged_writer != nullptr || merged_syms != nullptr))
    throw error(error::error_code::internal_error,
                "page_writer: non-merged layout must pass null merged_writer and merged_syms\n");

  std::vector<uint8_t> page_header = { 0xFF, 0xFF, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00};
  page_header[2] =  low_8(lpage.get_pagenum());             // Lower 8 bit of page_index
  page_header[3] =  high_8(lpage.get_pagenum());            // Higher 8 bit of page_index
  page_header[8] =  low_8(lpage.get_cur_page_len());        // Lower 8 bit of cur_page_len
  page_header[9] =  high_8(lpage.get_cur_page_len());       // Higher 8 bit of cur_page_len
  page_header[10] =  low_8(lpage.get_in_order_page_len());  // Lower 8 bit of in_order_page_len
  page_header[11] =  high_8(lpage.get_in_order_page_len()); // Higher 8 bit of in_order_page_len
  auto pagenum = lpage.get_pagenum();
  auto colnum = lpage.get_colnum();

  std::vector<std::shared_ptr<asm_data>> all;
  all.insert(all.end(), lpage.m_text.begin(), lpage.m_text.end());
  all.insert(all.end(), lpage.m_data.begin(), lpage.m_data.end());
  std::shared_ptr<assembler_state> page_state = create_assembler_state(m_isa, all, scratchpad, labelpageindex, ctrlpkt_id_map, optimization_level, false);

  std::shared_ptr<section_writer> textwriter;
  std::shared_ptr<section_writer> datawriter;
  if (merged) {
    textwriter = std::make_shared<section_writer>("", code_section::text);
    datawriter = std::make_shared<section_writer>("", code_section::data);
  } else {
    textwriter = std::make_shared<section_writer>(get_TextSectionName(colnum, pagenum), code_section::text);
    datawriter = std::make_shared<section_writer>(get_DataSectionName(colnum, pagenum), code_section::data);
  }

  textwriter->write_bytes(page_header);

  // text_base is the position of the first text instruction in textwriter (= 16)
  offset_type text_base = textwriter->tell();
  std::vector<symbol> tsym;
  std::string fid;
  for (const auto& text : lpage.m_text)
  {
    const std::string& name = text->get_operation().get_name();
    auto args = text->get_operation().get_args();

    if (m_dump_flag != asm_dump_flag::disable) {
      offset_type pc_low = pagenum * PAGE_SIZE + textwriter->tell();
      offset_type pc_high;
      if (name == "start_job" || name == "start_job_deferred" || name == "start_cond_job_preempt") {
        // Note: eopnum=0 passed since makeunique=false means eopnum is not used
        pc_high = pc_low + page_state->m_jobmap[page_state->gen_job_name(false, text, 0)]->get_size() - 1;
        fid = m_debug.add_function(text->get_file_idx(), name + "_" + page_state->gen_job_name(false, text, 0), pc_high, pc_low, colnum, pagenum);
      }
      pc_high = pc_low + (*m_isa)[name]->serializer(args)->size(*page_state) - 1;
      m_debug.add_textline(fid, text->get_linenumber(), pc_high, pc_low, text->get_line(), text->get_annotation_index());
    }

    if (text->isOpcode())
    {
      page_state->set_pos(textwriter->tell() - text_base);
      page_state->set_is_save_restore_op(text->get_is_save_restore());  // Track if this is save/restore op
      std::vector<uint8_t> ret = (*m_isa)[name]->serializer(args)
                                               ->serialize(page_state, tsym, colnum, pagenum);
      textwriter->write_bytes(ret);
    } else
      throw error(error::error_code::internal_error, "Invalid operation: " + name + " in TEXT section !!!");
  }

  std::vector<symbol> dsym;
  for (const auto& data : lpage.m_data)
  {
    page_state->set_pos(datawriter->tell() + textwriter->tell() - text_base);
    std::string name = data->get_operation().get_name();
    if (!name.compare("eof"))
      continue;
    if (data->isLabel())
    {
      // TODO assert
    } else if (data->isOpcode())
    {
      auto args = data->get_operation().get_args();  // split once, reuse below
      // data section dump is only generated in case of full dump.
      if (m_dump_flag == asm_dump_flag::full) {
        offset_type pc_low = pagenum * PAGE_SIZE + textwriter->tell() + datawriter->tell();
        offset_type pc_high = pc_low + (*m_isa)[name]->serializer(args)->size(*page_state) - 1;
        m_debug.add_dataline(fid, data->get_linenumber(), pc_high, pc_low, data->get_line(), data->get_annotation_index());
      }
      std::vector<uint8_t> ret = (*m_isa)[name]->serializer(args)
                                               ->serialize(page_state, dsym, colnum, pagenum);
      datawriter->write_bytes(ret);
    } else
      throw error(error::error_code::internal_error, "Invalid operation: " + name + " in DATA section !!!");
  }

  // patch57 uses temp writers: textwriter->tell() = 16 + T_N, so offset - textwriter->tell()
  // correctly yields the data-relative BD offset.
  for (auto &spad : page_state->m_patch)
  {
    for (auto& arg : spad.second)
    {
      offset_type offset = page_state->parse_num_arg(arg);
      patch57(textwriter, datawriter,
              offset + static_cast<offset_type>(page_header.size()),
              page_state->m_scratchpad[spad.first.substr(1)]->get_base() + page_state->m_scratchpad[spad.first.substr(1)]->get_offset());
    }
  }

  // Verify this page fits within PAGE_SIZE
  if (textwriter->tell() + datawriter->tell() > PAGE_SIZE)
    throw error(error::error_code::internal_error, "page content more the pagesize !!!");

  offset_type padsize = PAGE_SIZE - textwriter->tell() - datawriter->tell();
  if (merged) {
    merged_writer->write_bytes(textwriter->get_data());
    merged_writer->write_bytes(datawriter->get_data());
    if (padsize > 0) {
      std::vector<uint8_t> zeros(padsize, 0x00);
      merged_writer->write_bytes(zeros);
    }
    merged_syms->insert(merged_syms->end(), tsym.begin(), tsym.end());
    merged_syms->insert(merged_syms->end(), dsym.begin(), dsym.end());
    // in case of merged, data section of page dont have padsize added
    m_report.addpage(lpage, page_state, textwriter->tell(), datawriter->tell() + padsize, padsize);
  } else {
    datawriter->padding(PAGE_SIZE - textwriter->tell());
    textwriter->add_symbols(tsym);
    datawriter->add_symbols(dsym);
    twriter.push_back(textwriter);
    twriter.push_back(datawriter);
    // data section have pad added
    m_report.addpage(lpage, page_state, textwriter->tell(), datawriter->tell(), padsize);
  }
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
  // Add log for debugging patching
  log_info() << "aie2ps_encoder::patch57: offset=" << offset
             << ", patch=0x" << std::hex << patch
             << ", arg=0x" << std::hex << arg
             << ", after patch=0x" << std::hex << patch + arg
             << std::dec << std::endl;
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
