from ctrlcode.common.section import Section
from ctrlcode.ops.deserializer.op_deserializer import OpDeSerializer

class WordOpDeSerializer(OpDeSerializer):
    def __init__(self, op, state):
        super().__init__(op, state, ".long")

    def size(self):
        return 4

    def deserialize(self, reader, writer):
        lp = self.state.pos
        l = self.state.local_ptrs[lp]['label']
        writer.write_label(l)
        reader.seek(-1, 1)
        for i in range(self.state.local_ptrs[lp]['size']):
            result = []
            val = reader.read_integer(self.size())
            result.append(str('0x{0:08X}'.format(val)))
            self.state.pos += self.size()
            writer.write_operation(self.name, result, l)
