# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc.

from enum import IntEnum

class Symbol:
    """
    Represents a user symbol (a variable reference) encountered in assembly code.
    """

    class XrtPatchBufferType(IntEnum):
        xrt_patch_buffer_type_instruct = 0
        xrt_patch_buffer_type_control_packet = 1
        xrt_patch_buffer_type_transaction = 2
        xrt_patch_buffer_type_unkown = 3

    class XrtPatchSchema(IntEnum):
        xrt_patch_schema_uc_dma_remote_ptr_symbol = 1
        xrt_patch_schema_shim_dma_57 = 2
        xrt_patch_schema_scaler_32 = 3
        xrt_patch_schema_control_packet_48 = 4
        xrt_patch_schema_shim_dma_48 = 5
        xrt_patch_schema_shim_dma_57_aie4 = 6
        xrt_patch_schema_unknown = 8

    def __init__(self, name, buf_type, pos, addend, schema):
        self.name = name
        self.buf_type = buf_type
        self.offsets = pos
        self.schema = schema
        self.addend = addend
