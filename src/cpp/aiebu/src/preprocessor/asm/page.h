// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_PREPROCESSOR_ASM_PAGE_H_
#define _AIEBU_PREPROCESSOR_ASM_PAGE_H_

#include "asm/asm_parser.h"

namespace aiebu {

class page
{
  uint32_t m_colnum;
  pageid_type m_pagenum;
  bool m_islastpage;

public:
  std::vector<std::shared_ptr<asm_data>> m_text;
  std::vector<std::shared_ptr<asm_data>> m_data;

  page() {}
  page(uint32_t colnum, uint32_t pagenum, std::vector<std::shared_ptr<asm_data>> text,
       std::vector<std::shared_ptr<asm_data>> data, bool islastpage)
       : m_colnum(colnum), m_pagenum(pagenum), m_islastpage(islastpage),
         m_text(text), m_data(data) { }

  HEADER_ACCESS_GET_SET(uint32_t, colnum);
  HEADER_ACCESS_GET_SET(pageid_type, pagenum);
  HEADER_ACCESS_GET_SET(bool, islastpage);
};

}
#endif //_AIEBU_PREPROCESSOR_ASM_PAGE_H_

