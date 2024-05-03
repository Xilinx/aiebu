// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include "aie2ps_encoder.h"
#include "aiebu_error.h"

namespace aiebu {

std::shared_ptr<writer>
aie2ps_encoder::
process(std::shared_ptr<preprocessed_output> input)
{
  // encode asm data
  auto tinput = std::static_pointer_cast<aie2ps_preprocessed_output>(input);

  auto& totalcoldata = tinput->get_coldata();
  // for each colnum encode each page
  for (auto coldata: totalcoldata) {
    for (auto lpage : coldata.second)
    {
      std::vector<symbol> sym;
      page_writer(lpage, sym);
      twriter->add_symbols(sym);
/*
      for (auto s : sym)
        twriter->add_symbol(s);
*/
    }
  }

  return twriter;
}

void
aie2ps_encoder::
page_writer(page& lpage, std::vector<symbol>& sym)
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

  for (auto byte : page_header)
    twriter->write_byte(byte, code_section::text, colnum, pagenum);

  // encode text section
  code_section csection = code_section::text;
  offset_type offset = twriter->tell(csection, colnum, pagenum);
  for (auto text : lpage.m_text)
  {
    //TODO add debug info
    std::string name = text->get_operation()->get_name();
    if (text->isOpcode())
    {
      page_state.set_pos(twriter->tell(csection, colnum, pagenum) - offset);
      std::vector<uint8_t> ret = (*m_isa)[name]->serializer(text->get_operation()->get_args())
                                               ->serialize(page_state, sym, colnum, pagenum);
      for (uint8_t byte : ret) {
        twriter->write_byte(byte, code_section::text, colnum, pagenum);
      }
    } else 
      throw error(error::error_code::internal_error, "Invalid operation: " + name + " in TEXT section !!!");
  }

  // encode text section
  for (auto data : lpage.m_data)
  {
    page_state.set_pos(twriter->tell(csection, colnum, pagenum) - offset);
    std::string name = data->get_operation()->get_name();
    if (data->isLabel())
    {
      csection = code_section::data;
      // TODO assert
    } else if (data->isOpcode())
    {
      //TODO add debug info
      std::vector<uint8_t> ret = (*m_isa)[name]->serializer(data->get_operation()->get_args())
                                               ->serialize(page_state, sym, colnum, pagenum);
      for (auto byte : ret) {
        twriter->write_byte(byte, code_section::data, colnum, pagenum);
      }
    } else 
      throw error(error::error_code::internal_error, "Invalid operation: " + name + " in DATA section !!!");
  }

  twriter->padding(colnum, pagenum, PAGE_SIZE);

  // TODO add size and generate report
}

}
