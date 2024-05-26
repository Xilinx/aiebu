from ctrlcode.common.op_arg import OpArg
from ctrlcode.common.util import to_Tile
from ctrlcode.common.util import to_S2MM_MM2S
from ctrlcode.ops.deserializer.op_deserializer import OpDeSerializer

class IsaOpDeSerializer(OpDeSerializer):
    numlabel = 1
    LABEL = "@label"
    def __init__(self, op, state, name=""):
        super().__init__(op, state, name)

    def size(self):
        result = int((16 + sum([arg.width for arg in self.op.args])) / 8)
        return result

    def deserialize(self, reader, writer):
        result = []
        pad = reader.read_integer()
        size = int(2)

        for arg_index, arg in enumerate(self.op.args):
            # read data
            l = int(arg.width/8)
            size += l
            val = reader.read_integer(l)
            if arg.argtype == OpArg.CONST:
                if arg.name == "tile_id":
                    result.append(to_Tile(val))
                elif arg.name == "actor_id":
                    result.append(to_S2MM_MM2S(val))
                elif arg.name == "descriptor_ptr":
                    if val not in self.state.labels.keys():
                        result.append(IsaOpDeSerializer.LABEL + str(IsaOpDeSerializer.numlabel))
                        self.state.labels[val] = IsaOpDeSerializer.LABEL + str(IsaOpDeSerializer.numlabel)
                        IsaOpDeSerializer.numlabel += 1
                    else:
                        result.append(self.state.labels[val])
                else:
                    result.append(f"0x{val:X}")
            elif arg.argtype == OpArg.JOBSIZE:
                pass
                # TODO: check for jobsize is correct or not
            elif arg.argtype == OpArg.REG:
                assert val < 24, "Register number out of range: {}".format(val)
                # [0:7] are local register [8:23] are global register
                if val < 8:
                    result.append('$r' + str(val))
                else:
                    result.append('$g' + str(val-8))
            elif arg.argtype == OpArg.BARRIER:
                if "local_barrier" == self.name:
                    result.append('$lb'+ str(val))
                elif "remote_barrier" == self.name:
                    result.append('$rb'+ str(val-1))
                else:
                    assert False, "Invalid barrier arg for {self.name}."
            elif arg.argtype == OpArg.PAD:
                pass
            elif arg.argtype == OpArg.PAGE_ID:
                result.append(IsaOpDeSerializer.LABEL + str(IsaOpDeSerializer.numlabel))
                self.state.externallabels[val] = IsaOpDeSerializer.LABEL + str(IsaOpDeSerializer.numlabel)
                IsaOpDeSerializer.numlabel += 1
            else:
                assert False, "Invalid arg type!"

        self.state.pos += size
        writer.write_operation(self.op.opcode.upper(), result, None)
