# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2023 Advanced Micro Devices, Inc.


from enum import IntEnum

class Symbol:
    """
    Represents a user symbol (a variable reference) encountered in assembly code.
    """
    class SymbolKind(IntEnum):
        UC_DMA_REMOTE_PTR_SYMBOL_KIND = 1
        SHIM_DMA_BASE_ADDR_SYMBOL_KIND = 2
        SCALAR_32BIT_KIND = 3
        UNKNOWN_SYMBOL_KIND = 4

    def __init__(self, name, pos, col, page_num, kind=SymbolKind.UNKNOWN_SYMBOL_KIND):
        self.name = name
        self.pos = pos
        self.kind = kind
        self.page_num = page_num
        self.col_num = col

    def setpos(self, pos):
        self.pos = pos

    def setcol(self, col):
        self.col = col

    def setkind(self, kind):
        self.kind = kind

    def setpagenum(self, page_num):
        self.page_num = page_num
