# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

from abc import ABC, abstractmethod

class OpDeSerializer(ABC):
    """ abstract class for Deserializer """
    def __init__(self, op, state, name):
        self.op = op
        self.state = state
        self.name = name

    @abstractmethod
    def size(self):
        pass

    @abstractmethod
    def deserialize(self, reader, writer):
        pass
