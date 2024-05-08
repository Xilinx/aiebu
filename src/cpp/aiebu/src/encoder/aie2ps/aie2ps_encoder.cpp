// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include "aie2ps_encoder.h"
#include "aiebu_error.h"

namespace aiebu {

std::vector<writer>
aie2ps_encoder::
process(std::shared_ptr<preprocessed_output> input)
{
  // encode asm data
  auto tinput = std::static_pointer_cast<aie2ps_preprocessed_output>(input);

  auto& totalcoldata = tinput->get_coldata();
  // for each colnum encode each page
  for (auto coldata: totalcoldata) {
    for (auto lpage : coldata.second)
      page_writer(lpage);
  }

  return twriter;
}

void
aie2ps_encoder::
page_writer(page& lpage)
{
  // encode page
  std::vector<uint8_t> page_header = { 0xFF, 0xFF, 0x00, 0x00,
                                       0x01, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00};
  if (lpage.get_islastpage())
    page_header[4] = 0x0;

  auto pagenum = lpage.get_pagenum();
  auto colnum = lpage.get_colnum();
  // create state
  std::vector<std::shared_ptr<asm_data>> all;
  all.insert(all.end(), lpage.m_text.begin(), lpage.m_text.end());
  all.insert(all.end(), lpage.m_data.begin(), lpage.m_data.end());
  assembler_state page_state = assembler_state(m_isa, all);

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

  datawriter.padding(PAGE_SIZE-textwriter.tell());

  textwriter.add_symbols(tsym);
  datawriter.add_symbols(dsym);
  twriter.push_back(textwriter);
  twriter.push_back(datawriter);

  // TODO add size and generate report
}

}
