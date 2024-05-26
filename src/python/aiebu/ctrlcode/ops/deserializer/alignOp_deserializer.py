from ctrlcode.ops.deserializer.op_deserializer import OpDeSerializer

class AlignOpDeSerializer(OpDeSerializer):
    def __init__(self, op, state):
        super().__init__(op, state, ".align 16")
        #self.count = 1
        #self.opcode = 165 # A5

    def size(self):
        return 1

    def deserialize(self, reader, writer):
        self.state.pos += 1
        writer.write_operation(self.name.upper(), [], None)
