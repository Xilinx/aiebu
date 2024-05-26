import struct

from ctrlcode.common.util import parse_num_arg
from ctrlcode.common.section import Section
from ctrlcode.ops.serializer.op_serializer import OpSerializer
from ctrlcode.common.symbol import Symbol

class UcDmaShimOpSerializer(OpSerializer):
    def __init__(self, op, args, state):
        assert len(args) == 7 or len(args) == 8, "Invalid args for UC_DMA_BD_SHIM!"
        super().__init__(op, args, state)

    def size(self):
        return 16

    def align(self):
        return 16

    def serialize(self, text_section, data_section, col, page, symbols):
        assert self.state.getpos() % 16 == 0, "uC DMA definition has to be 128-bit aligned!"
        #print(self.args)
        remote_ptr_high = parse_num_arg(self.args[0], self.state)
        remote_ptr_low = parse_num_arg(self.args[1], self.state)
        local_ptr_absolute = parse_num_arg(self.args[2], self.state)
        size = parse_num_arg(self.args[3], self.state)
        ctrl_external = parse_num_arg(self.args[4], self.state) != 0
        ctrl_next_BD = parse_num_arg(self.args[5], self.state) != 0
        ctrl_local_relative = True
        if self.args[6].startswith('@'):
            ursymbo = self.args[6][1:]
        else:
            ursymbo = self.args[6]
        patcher = 1 # default
        if len(self.args) == 8:
            patcher = parse_num_arg(self.args[7], self.state)

        assert local_ptr_absolute > self.state.getpos(), "uC DMA local ptr has to be located after the DMA definition!"
        local_ptr = local_ptr_absolute - self.state.getpos()

        assert (self.state.section == Section.DATA), "UC_DMA_BD can only be used in DATA section"

        # patcher:
        #    1--> runtime loader;
        #    2--> assembler;
        #    3--> runtime loader and assembler;
        if patcher == 1 or patcher == 3:
            symbols.append(Symbol(ursymbo, local_ptr_absolute, col, page,
                                  Symbol.XrtPatchSchema.xrt_patch_schema_shim_dma_57))

        if (patcher == 2 or patcher == 3) and self.state.containscratchpads(ursymbo):
           self.state.patch[ursymbo] = self.args[2]

        data_section.write_words([
            (size & 0x7FFF)
            | ((1 << 16) if ctrl_next_BD else 0)
            | ((1 << 17) if ctrl_external else 0)
            | ((1 << 18) if ctrl_local_relative else 0),
            local_ptr,
            remote_ptr_low,
            remote_ptr_high & 0x1FFFFFFF
        ])

    def _endian_swap_u32(self, word):
        return struct.unpack(">L", struct.pack("<L", word))[0]
