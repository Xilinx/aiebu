# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

from ctrlcode.ops.serializer.isaOp_serializer import IsaOpSerializer
from ctrlcode.ops.deserializer.isaOp_deserializer import IsaOpDeSerializer

class IsaOp:
    def __init__(self, opcode, args=[]):
        self.opcode = opcode
        self.name = opcode
        self.args = args

    def serializer(self, args, state):
        return IsaOpSerializer(self, args, state)

    def __str__(self):
        return f"{self.opcode} {self.args}"

    def deserializer(self, state):
        return IsaOpDeSerializer(self, state, self.name)
