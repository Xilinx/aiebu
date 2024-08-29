# SPDX-License-Identifier: MIT
# Copyright (C) 2023 Advanced Micro Devices, Inc.

from enum import IntEnum

class Symbol:
    """
    Represents a user symbol (a variable reference) encountered in assembly code.
    """
    class XrtPatchSchema(IntEnum):
        xrt_patch_schema_uc_dma_remote_ptr_symbol = 1
        xrt_patch_schema_shim_dma_57 = 2
        xrt_patch_schema_scaler_32 = 3
        xrt_patch_schema_control_packet_48 = 4
        xrt_patch_schema_shim_dma_48 = 5
        xrt_patch_schema_shim_dma_57_aie4 = 6
        xrt_patch_schema_unknown = 8

    class XrtPatchBufferType(IntEnum):
        xrt_patch_buffer_type_instruct = 0
        xrt_patch_buffer_type_control_packet = 1
        xrt_patch_buffer_type_unkown = 2

    def __init__(self, name, pos, col, page_num, schema=XrtPatchSchema.xrt_patch_schema_unknown, buf_type=None):
        self.name = name
        self.offset = pos
        self.schema = schema
        self.page_num = page_num
        self.col_num = col
        self._buf_type = buf_type

    def setoffset(self, pos):
        self.offset = pos

    def setcol(self, col):
        self.col = col

    def setschema(self, schema):
        self.schema = schema

    def setpagenum(self, page_num):
        self.page_num = page_num

    def getbuftype(self):
        if self._buf_type == None:
            from ctrlcode.common.util import get_data_section_name
            return get_data_section_name(self.col_num, self.page_num)
        return self._buf_type

    def __str__(self):
        return f"Name:{self.name}\toffset:{self.offset}\tschema:{self.schema}\tbuf_type:{self._buf_type}"
