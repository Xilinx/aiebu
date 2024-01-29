from ctrlcode.common.util import parse_num_arg
from ctrlcode.ops.serializer.op_serializer import OpSerializer

class AlignOpSerializer(OpSerializer):
    def __init__(self, op, args, state):
        assert len(args) == 1
        super().__init__(op, args, state)
        self._align = parse_num_arg(args[0], self.state)

    def size(self):
        return (self._align - (self.state.getpos() % self._align)) if (self.state.getpos() % self._align) > 0 else 0

    def align(self):
        return 0

    def serialize(self, writer, col, page, symbols):
        for i in range(0, self.size()):
            writer.write_byte(0xA5, self.state.section, col, page)
