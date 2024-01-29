# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2022-2023 Advanced Micro Devices, Inc.

import os

class InputReader:
    def __init__(self, filename):
        self.f = open(filename, 'rb')

    def __del__(self):
        self.f.close()

    @classmethod
    def isELF(cls, filename):
        fh = open(filename, 'rb')
        magic = fh.read(4)
        fh.close()
        if (magic == b'\x7fELF'):
            return True
        else:
            return False

    def read(self, count=1):
        return self.f.read(count)

    def read_integer(self, count=1, byteorder='little'):
        return int.from_bytes(self.f.read(count), byteorder)

    def seek(self, offset, from_what):
        return self.f.seek(offset, from_what)

class ByteReader:
    def __init__(self, data):
        self.data = data
        self.index = 0

    def read(self, count=1):
        if (self.index + count > len(self.data)):
            return b''
        else:
            temp = self.index
            self.index += count
            return self.data[temp:temp + count]

    def read_integer(self, count=1, byteorder='little'):
        return int.from_bytes(self.read(count), byteorder)

    def seek(self, offset, from_what):
        if (from_what == os.SEEK_SET):
            assert(offset < len(self.data)), f"Illegal seek offset {offset}"
            self.index = offset
        elif (from_what == os.SEEK_CUR):
            assert(self.index + offset < len(self.data)), f"Illegal seek offset {offset}"
            self.index += offset
        else:
            assert(-1 * offset < len(self.data)), f"Illegal seek offset {offset}"
            self.index = len(self.data) + offset
        return self.index
