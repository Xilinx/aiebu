# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2022-2023 Advanced Micro Devices, Inc.

import ctypes
import pylibelf

def parse_register(arg):
    assert arg.startswith('$r') or arg.startswith('$g'), f"REG val not a register: {arg}"
    val = int(arg[2:])
    if arg.startswith('$g'):
        assert val < 16, f"Global Register {arg} number out of range: {val}"
        val = val + 8
    assert val < 24, f"Register number {arg} out of range: {val}"
    return val

def parse_barrier(arg):
    # TODO: this is temporary for backward support. Should be removed once all migrate to new.
    if arg.isnumeric():
        return int(arg)

    assert (arg.startswith('$lb') or arg.startswith('$rb')), f"BARRIER val not a barrier: {arg}"
    val = int(arg[3:])
    if arg.startswith('$lb'):
        assert val < 16, f"LOCAL BARRIER {arg} number out of range: {val}"
    else: #$rb
        val = val + 1
        assert (val > 0 and val < 65) , f"REMOTE BARRIER {arg} number out of range: {val}"
    return val

def parse_num_arg(arg, state):
    if isinstance(arg, int):
        return arg

    if arg.startswith('@'):
        label = arg[1:]
        assert state.containlabel(label), f"Label not found: {label}"
        return state.getlabelpos(label)

    elif arg.startswith('TILE_'):
        args = arg.split('_')
        assert len(args) == 3, f"Invalid TILE literal: {arg}"
        col = int(args[1])
        row = int(args[2])
        return ((col & 0x7F) << 5 | (row & 0x1F))

    elif arg.startswith('S2MM_'):
        args = arg.split('_')
        assert len(args) == 2, f"Invalid S2MM literal: {arg}"
        index = int(args[1])
        assert index <= 5, f"Invalid S2MM index: {index}"
        return index

    elif arg.startswith('MM2S_'):
        args = arg.split('_')
        assert len(args) == 2, f"Invalid MM2S literal: {arg}"
        index = int(args[1])
        assert index <= 5, f"Invalid MM2S index: {index}"
        return (6 + index)

    elif arg.startswith('0x'):
        return int(arg[2:], 16)

    elif arg.isnumeric():
        return int(arg)
    else:
        return Symbol(arg,0,0,0,0)

def to_Tile(arg):
    row = arg & 0x1F
    col = (arg >> 5) & 0x7F
    return 'TILE_' + str(col) + '_' + str(row)

def to_S2MM_MM2S(arg):
    if arg > 5:
        return 'MM2S_' + str(arg - 6)
    else:
        return 'S2MM_' + str(arg)

def union_of_lists_inorder(list1, list2):
  newlist = list(list1)
  for element in list2:
    if element not in newlist:
      newlist.append(element)
  return newlist

class ELFStringTable:
    """
    Helper data structure to store and pack strings for ELF which is later
    used to build ELF .shstrtab section
    """
    def __init__(self, data=None, size=0):
        self._size = size
        self._syms = []
        self._data = None
        if (data == None):
            return
        self._data = ctypes.string_at(data, size)
        # Virtual dispatch to class specific deserialize implementation
        self._populate()

    # Derived classes implement their own custom deserialize operation
    def _populate(self):
        # Populate our syms list
        pos = 0
        while (pos < self._size):
            item = self.get(pos)
            self._syms.append(item)
            pos += len(item)
            # Go past the null char
            pos += 1

    # Support len() operator on the table e.g. len(mystab)
    # Inherited by the derived classes
    def __len__(self):
        return len(self._syms)

    # Support indexing into the table e.g. mystab[i]
    # Inherited by the derived classes
    def __getitem__(self, index):
        return self._syms[index]

    def add(self, item):
        pos = self._size
        self._syms.append(item)
        self._size += (len(item) + 1)
        return pos

    # Used by all derived classes as well
    def _pack(self, arr, index):
        subdata = ctypes.cast(ctypes.addressof(self._data) + index, ctypes.c_void_p)
        ctypes.memmove(subdata, arr, len(arr))
        index += len(arr)
        return index

    def packsyms(self):
        self._data = ctypes.create_string_buffer(self._size)
        index = 0
        for item in self._syms:
            arr = bytes(item, "utf-8")
            index = self._pack(arr, index)
            self._data[index] = b'\0'
            index += 1
        return self._data

    def get(self, pos):
        assert(pos < self._size), f"Illegal offset into table storage"
        item = ctypes.string_at(self._data[pos:])
        return item.decode("utf-8")

    def space(self):
        return self._size

    def __str__(self):
        return f"{self._syms}\n{self._data}"

class ELFSymbolTable(ELFStringTable):
    def __init__(self, data=None, size=0):
        super().__init__(data, size)

    def _populate(self):
        # Populate our syms list
        pos = 0
        while (pos < self._size):
            self._syms.append(self.get(pos))
            pos += ctypes.sizeof(pylibelf.elf.Elf32_Sym)

    def add(self, item):
        pos = self._size
        self._syms.append(item)
        self._size += ctypes.sizeof(item)
        return pos

    def packsyms(self):
        self._data = ctypes.create_string_buffer(self._size)
        index = 0
        for item in self._syms:
            arr = bytes(item)
            index = self._pack(arr, index)
        return self._data

    def get(self, pos):
        assert(pos < self._size), f"Illegal offset into table storage"
        item = pylibelf.elf.Elf32_Sym.from_buffer_copy(self._data, pos)
        return item


class ELFRelaTable(ELFSymbolTable):
    """
    Helper data structure to store and pack Elf32_Rela which is later
    used to build ELF .rela.dyn section
    """
    def __init__(self, data=None, size=0):
        super().__init__(data, size)

    def _populate(self):
        # Populate our syms list
        pos = 0
        while (pos < self._size):
            self._syms.append(self.get(pos))
            pos += ctypes.sizeof(pylibelf.elf.Elf32_Rela)

    def get(self, pos):
        assert(pos < self._size), f"Illegal offset into table storage"
        item = pylibelf.elf.Elf32_Rela.from_buffer_copy(self._data, pos)
        return item


class ELFDynamicTable(ELFSymbolTable):
    """
    Helper data structure to store and pack Elf32_Dyn which is later
    used to build ELF .dynamic section
    """
    def __init__(self, data=None, size=0):
        super().__init__(data, size)

    def _populate(self):
        # Populate our syms list
        pos = 0
        while (pos < self._size):
            self._syms.append(self.get(pos))
            pos += ctypes.sizeof(pylibelf.elf.Elf32_Dyn)

    def get(self, pos):
        assert(pos < self._size), f"Illegal offset into table storage"
        item = pylibelf.elf.Elf32_Dyn.from_buffer_copy(self._data, pos)
        return item
