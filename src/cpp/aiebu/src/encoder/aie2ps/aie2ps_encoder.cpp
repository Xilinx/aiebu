// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include "aie2ps_encoder.h"
#include "aiebu_error.h"
#include <cassert>

namespace aiebu {

void
aie2ps_encoder::
fill_scratchpad(writer& padwriter, const std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpads)
{
  for (auto& pad : scratchpads)
  {
    auto& content = pad.second->get_content();
    if (content.size())
    {
      assert((void("Pad content size and size doesnt match\n"), content.size() == pad.second->get_size()));
      for (auto& val : content)
        padwriter.write_byte(val);
    } else
      for (auto i = 0ul; i < pad.second->get_size(); ++i)
        padwriter.write_byte(0x00);
  }
}

void
aie2ps_encoder::
fill_control_packet_symbols(writer& padwriter,const uint32_t col,const std::string& controlpacket_padname,
                            std::vector<symbol>& syms,
                            const std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpads)
{
  if (scratchpads.find(controlpacket_padname) == scratchpads.end())
    throw error(error::error_code::invalid_asm, "controlpacket padname " +
                 controlpacket_padname + " not present in scratchpads\n");

  auto& pad = scratchpads.at(controlpacket_padname);
  for (auto& sym : syms)
  {
    if (sym.get_colnum() != col)
      continue;
    sym.set_section_name(get_PadSectionName(col));
    sym.set_pos(sym.get_pos() + pad->get_offset());
    padwriter.add_symbol(sym);
  }
}

std::vector<writer>
aie2ps_encoder::
process(std::shared_ptr<preprocessed_output> input)
{
  // encode asm data
  auto tinput = std::static_pointer_cast<aie2ps_preprocessed_output>(input);

  auto& totalcoldata = tinput->get_coldata();
  auto& totalsyms = tinput->get_symbols();

  // for each colnum encode each page
  for (auto coldata: totalcoldata) {
    auto colnum = coldata.first;
    std::string controlpacket_padname;
    for (auto& lpage : coldata.second->m_pages)
    {
      auto cp_name = page_writer(lpage, coldata.second->m_scratchpad, coldata.second->m_labelpageindex,
                                 coldata.second->m_control_packet_index);
      if (!cp_name.empty())
        controlpacket_padname = cp_name.substr(1);
    }

    if (!coldata.second->m_scratchpad.size())
      continue;

    writer padwriter(get_PadSectionName(colnum), code_section::data);
    fill_scratchpad(padwriter, coldata.second->m_scratchpad);
    fill_control_packet_symbols(padwriter, colnum, controlpacket_padname, totalsyms, coldata.second->m_scratchpad);

    twriter.push_back(padwriter);
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


std::string
aie2ps_encoder::
page_writer(page& lpage, std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpad,
            std::map<std::string, uint32_t>& labelpageindex, uint32_t control_packet_index)
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

  if (lpage.get_islastpage())
    page_header[4] = 0x0;

  auto pagenum = lpage.get_pagenum();
  auto colnum = lpage.get_colnum();
  // create state
  std::vector<std::shared_ptr<asm_data>> all;
  all.insert(all.end(), lpage.m_text.begin(), lpage.m_text.end());
  all.insert(all.end(), lpage.m_data.begin(), lpage.m_data.end());
  assembler_state page_state = assembler_state(m_isa, all, scratchpad, labelpageindex, control_packet_index, false);

  writer textwriter(get_TextSectionName(colnum, pagenum), code_section::text);
  writer datawriter(get_DataSectionName(colnum, pagenum), code_section::data);

  for (auto byte : page_header)
    textwriter.write_byte(byte);

  // encode text section
  offset_type offset = textwriter.tell();
  std::vector<symbol> tsym;
  for (auto text : lpage.m_text)
  {
    //TODO add debug info
    std::string name = text->get_operation()->get_name();
    if (text->isOpcode())
    {
      page_state.set_pos(textwriter.tell() - offset);
      std::vector<uint8_t> ret = (*m_isa)[name]->serializer(text->get_operation()->get_args())
                                               ->serialize(page_state, tsym, colnum, pagenum);
      for (uint8_t byte : ret) {
        textwriter.write_byte(byte);
      }
    } else 
      throw error(error::error_code::internal_error, "Invalid operation: " + name + " in TEXT section !!!");
  }

  std::vector<symbol> dsym;
  // encode data section
  for (auto data : lpage.m_data)
  {
    page_state.set_pos(datawriter.tell() + textwriter.tell() - offset);
    std::string name = data->get_operation()->get_name();
    if (!name.compare("eof"))
      continue;
    if (data->isLabel())
    {
      // TODO assert
    } else if (data->isOpcode())
    {
      //TODO add debug info
      std::vector<uint8_t> ret = (*m_isa)[name]->serializer(data->get_operation()->get_args())
                                               ->serialize(page_state, dsym, colnum, pagenum);
      for (auto byte : ret) {
        datawriter.write_byte(byte);
      }
    } else 
      throw error(error::error_code::internal_error, "Invalid operation: " + name + " in DATA section !!!");
  }

  for (auto &spad : page_state.m_patch)
  {
    for (auto& arg : spad.second)
    {
      offset = page_state.parse_num_arg(arg);
      patch57(textwriter, datawriter, offset + page_header.size(),
              page_state.m_scratchpad[spad.first.substr(1)]->get_base() + page_state.m_scratchpad[spad.first.substr(1)]->get_offset());
    }
  }

  datawriter.padding(PAGE_SIZE-textwriter.tell());

  textwriter.add_symbols(tsym);
  datawriter.add_symbols(dsym);
  twriter.push_back(textwriter);
  twriter.push_back(datawriter);

  return findKey(page_state.m_patch, page_state.m_controlpacket_padname);
  // TODO add size and generate report
}

void
aie2ps_encoder::
patch57(const writer& textwriter, writer& datawriter, offset_type offset, uint64_t patch)
{
  offset = offset - textwriter.tell();
  uint64_t bd1 = datawriter.read_word(offset + 1*4);
  uint64_t bd2 = datawriter.read_word(offset + 2*4);
  uint64_t bd8 = datawriter.read_word(offset + 8*4);

  uint64_t arg = ((bd8 & 0x1FF) << 48) + ((bd2 & 0xFFFF) << 32) + (bd1 & 0xFFFFFFFF); // NOLINT
  patch = arg + patch;
  datawriter.write_word_at(offset + 1*4, patch & 0xFFFFFFFF);
  datawriter.write_word_at(offset + 2*4, ((patch >> 32) & 0xFFFF) | (bd2 & 0xFFFF0000));
  datawriter.write_word_at(offset + 8*4, ((patch >> 48) & 0x1FF) | (bd8 & 0xFFFFFE00));
}

}
