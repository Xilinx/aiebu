# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

from ctrlcode.ops.serializer.alignOp_serializer import AlignOpSerializer
from ctrlcode.ops.deserializer.alignOp_deserializer import AlignOpDeSerializer

class AlignOp:
    def serializer(self, args, state):
        return AlignOpSerializer(self, args, state)

    def deserializer(self, state):
        return AlignOpDeSerializer(self, state)

