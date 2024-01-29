from ctrlcode.ops.serializer.wordOp_serializer import WordOpSerializer
from ctrlcode.ops.deserializer.wordOp_deserializer import WordOpDeSerializer

class WordOp:
    def serializer(self, args, state):
        return WordOpSerializer(self, args, state)

    def deserializer(self, state):
        return WordOpDeSerializer(self, state)

