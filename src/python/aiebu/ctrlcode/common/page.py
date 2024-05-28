# SPDX-License-Identifier: MIT
# Copyright (C) 2022-2023 Advanced Micro Devices, Inc.

from ctrlcode.common.util import get_text_section_name, get_data_section_name

class Page:
    def __init__(self, col_num, page_num, text, data, islastpage, cur_page_len, in_order_page_len, external_labels):
        self.col_num = col_num
        self.page_num = page_num
        self.text = text
        self.data = data
        self.islastpage = islastpage
        self.in_order_page_len = in_order_page_len
        self.cur_page_len = cur_page_len
        self._out_of_order_page = external_labels

    def getout_of_order_page(self):
        return self._out_of_order_page

    def get_text_section_name(self):
        return get_text_section_name(self.col_num, self.page_num)

    def get_data_section_name(self):
        return get_data_section_name(self.col_num, self.page_num)

    def __str__(self):
        return f"Col_num:{self.col_num}\tpage_num:{self.page_num}\ttext:{self.text}\t\
                data:{self.data}\tislastpage:{self.islastpage}\_out_of_order_page:{self._out_of_order_page}"

