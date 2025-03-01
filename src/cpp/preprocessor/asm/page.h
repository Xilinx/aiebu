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
  uint32_t m_cur_page_len;
  uint32_t m_in_order_page_len;
  uint32_t m_ooo_order_page_len_1;
  uint32_t m_ooo_order_page_len_2;
  std::vector<std::string> m_externallabels;

public:
  std::vector<std::shared_ptr<asm_data>> m_text;
  std::vector<std::shared_ptr<asm_data>> m_data;

  page() {}
  page(uint32_t colnum, uint32_t pagenum, std::vector<std::shared_ptr<asm_data>> text,
       std::vector<std::shared_ptr<asm_data>> data, bool islastpage, uint32_t cur_len, uint32_t in_order_len, std::vector<std::string> externallabels)
       : m_colnum(colnum), m_pagenum(pagenum), m_islastpage(islastpage), m_cur_page_len(cur_len), m_in_order_page_len(in_order_len),
         m_ooo_order_page_len_1(0), m_ooo_order_page_len_2(0), m_externallabels(externallabels), m_text(text), m_data(data) { }

  HEADER_ACCESS_GET_SET(uint32_t, colnum);
  HEADER_ACCESS_GET_SET(pageid_type, pagenum);
  HEADER_ACCESS_GET_SET(bool, islastpage);
  HEADER_ACCESS_GET_SET(uint32_t, cur_page_len);
  HEADER_ACCESS_GET_SET(uint32_t, in_order_page_len);
  void set_ooo_page_len(uint32_t len1, uint32_t len2)
  {
    m_ooo_order_page_len_1 = len1;
    m_ooo_order_page_len_2 = len2;
  }

  std::vector<std::string>& getout_of_order_page() { return m_externallabels;}
  uint32_t get_ooo_page_len_1() { return m_ooo_order_page_len_1;}
  uint32_t get_ooo_page_len_2() { return m_ooo_order_page_len_2;}
  HEADER_ACCESS_GET_SET(std::vector<std::string>, externallabels);
};

}
#endif //_AIEBU_PREPROCESSOR_ASM_PAGE_H_

