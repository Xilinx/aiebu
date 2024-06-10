# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

from ctrlcode.common.util import parse_num_arg
from ctrlcode.ops.serializer.op_serializer import OpSerializer
from ctrlcode.common.section import Section

class AlignOpSerializer(OpSerializer):
    def __init__(self, op, args, state):
        assert len(args) == 1
        super().__init__(op, args, state)
        self._align = parse_num_arg(args[0], self.state)

    def size(self):
        return (self._align - (self.state.getpos() % self._align)) if (self.state.getpos() % self._align) > 0 else 0

    def align(self):
        return 0

    def serialize(self, text_section, data_section, col, page, symbols):
        if self.state.section == Section.TEXT:
            for i in range(0, self.size()):
                text_section.write_byte(0xA5)
        else:
            for i in range(0, self.size()):
                data_section.write_byte(0xA5)
