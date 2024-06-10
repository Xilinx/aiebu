# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

from ctrlcode.ops.serializer.ucDmaShimOp_serializer import UcDmaShimOpSerializer
from ctrlcode.ops.deserializer.ucDmaShimOp_deserializer import UcDmaShimOpDeSerializer
from ctrlcode.common.symbol import Symbol
from ctrlcode.common.util import parse_num_arg

class UcDmaShimOp:
    def serializer(self, args, state):
        return UcDmaShimOpSerializer(self, args, state)

    def handle_symbol(self, args, state, col_num, page_num):
        local_ptr_absolute = parse_num_arg(args[2], state)
        return Symbol(args[6], local_ptr_absolute, col_num, page_num, Symbol.XrtPatchSchema.xrt_patch_schema_shim_dma_57)

    def deserializer(self, state):
        return UcDmaShimOpDeSerializer(self, state)
