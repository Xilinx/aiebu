from abc import ABC, abstractmethod

class OpSerializer(ABC):
    """ abstract serializer class """
    def __init__(self, op, args, state):
        self.op = op
        self.args = args
        self.state = state

    @abstractmethod
    def size(self):
        pass

    @abstractmethod
    def serialize(self, writer, col, page, symbols):
        pass

    @abstractmethod
    def align(self):
        pass
