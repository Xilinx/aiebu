from ctrlcode.ops.deserializer.op_deserializer import OpDeSerializer

class UcDmaShimOpDeSerializer(OpDeSerializer):
    numlabel = 1
    LABEL = "@label_shimbd"
    def __init__(self, op, state):
        super().__init__(op, state, "UC_DMA_BD_SHIM")

    def size(self):
        return 16

    def deserialize(self, reader, writer):
        assert self.state.pos % 16 == 0, "uC DMA definition has to be 128-bit aligned!"
        size = 0
        l = self.state.labels[self.state.pos]
        writer.write_label(l)
        reader.seek(-1, 1)

        ctrl_next_BD = 1
        while ctrl_next_BD == 1:
            ctrl_external = 0
            ctrl_local_relative = 1
            result = []
            arg = []
            arg.append(reader.read_integer(2))
            arg.append(reader.read_integer(2))
            arg.append(reader.read_integer(4))
            arg.append(reader.read_integer(4))
            arg.append(reader.read_integer(4))

            result.append(str('0x{0:08X}'.format(arg[4])))
            result.append(str('0x{0:08X}'.format(arg[3])))
            if arg[2]+self.state.pos not in self.state.local_ptrs.keys():
                result.append(UcDmaShimOpDeSerializer.LABEL + str(UcDmaShimOpDeSerializer.numlabel))
                self.state.local_ptrs[arg[2]+self.state.pos] = {'label':UcDmaShimOpDeSerializer.LABEL +\
                                                                 str(UcDmaShimOpDeSerializer.numlabel), \
                                                                'size':arg[0]}
                UcDmaShimOpDeSerializer.numlabel += 1
            else:
                result.append(self.state.local_ptrs[arg[2]+self.state.pos]['label'])
            result.append(arg[0])


            if not arg[1] & 0x1:
                ctrl_next_BD = 0
            if arg[1] & 0x2:
                ctrl_external = 1

            result.append(ctrl_external)
            result.append(ctrl_next_BD)

            self.state.pos += self.size()
            size += 1
            writer.write_operation(self.name.upper(), result, l)
