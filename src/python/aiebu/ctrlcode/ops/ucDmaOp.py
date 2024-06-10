# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

from ctrlcode.ops.serializer.ucDmaOp_serializer import UcDmaOpSerializer
from ctrlcode.ops.deserializer.ucDmaOp_deserializer import UcDmaOpDeSerializer

class UcDmaOp:
    def serializer(self, args, state):
        return UcDmaOpSerializer(self, args, state)

    def deserializer(self, state):
        return UcDmaOpDeSerializer(self, state)
