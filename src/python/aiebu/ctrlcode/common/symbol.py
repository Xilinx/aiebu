# SPDX-License-Identifier: Apache-2.0
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
        xrt_patch_schema_tansaction_ctrlpkt_48 = 6
        xrt_patch_schema_tansaction_48 = 7
        xrt_patch_schema_unknown = 8

    def __init__(self, name, pos, col, page_num, kind=XrtPatchSchema.xrt_patch_schema_unknown):
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

class AIE2_BLOB_Symbol:
    """
    Represents a user symbol (a variable reference) encountered in aie2 blob.
    """

    class XrtPatchBufferType(IntEnum):
        xrt_patch_buffer_type_instruct = 0
        xrt_patch_buffer_type_control_packet = 1
        xrt_patch_buffer_type_unkown = 2

    class XrtPatchSchema(IntEnum):
        xrt_patch_schema_uc_dma_remote_ptr_symbol = 1
        xrt_patch_schema_shim_dma_57 = 2
        xrt_patch_schema_scaler_32 = 3
        xrt_patch_schema_control_packet_48 = 4
        xrt_patch_schema_shim_dma_48 = 5
        xrt_patch_schema_tansaction_ctrlpkt_48 = 6
        xrt_patch_schema_tansaction_48 = 7
        xrt_patch_schema_unknown = 8

    def __init__(self, name, buf_type, pos, schema=XrtPatchSchema.xrt_patch_schema_unknown):
        self.name = name
        self.buf_type = buf_type
        self.offsets = [pos]
        self.schema = schema

    def addoffset(self, offset):
        self.offsets.append(offset)
