# SPDX-License-Identifier: MIT
# Copyright (C) 2022-2023 Advanced Micro Devices, Inc.

import ctypes
import pylibelf
from ctrlcode.common.symbol import Symbol

action_id = {'TILE_MM2S': {'aie2ps': {'count':2 , 'base_index':6, 'split_len':3, 'index_num':2},
                           'aie4'  : {'count':2 , 'base_index':6, 'split_len':3, 'index_num':2}},
             'TILE_S2MM': {'aie2ps': {'count':2 , 'base_index':0, 'split_len':3, 'index_num':2},
                          'aie4'  : {'count':2 , 'base_index':0, 'split_len':3, 'index_num':2}},
             'SHIM_MM2S': {'aie2ps': {'count':2 , 'base_index':6, 'split_len':3, 'index_num':2},
                           'aie4': {'count':4 , 'base_index':6, 'split_len':3, 'index_num':2}},
             'SHIM_S2MM': {'aie2ps': {'count':2 , 'base_index':0, 'split_len':3, 'index_num':2},
                           'aie4': {'count':4 , 'base_index':0, 'split_len':3, 'index_num':2}},
             'MEM_MM2S': {'aie2ps': {'count':6 , 'base_index':6, 'split_len':3, 'index_num':2},
                          'aie4': {'count':12 , 'base_index':16, 'split_len':3, 'index_num':2}},
             'MEM_S2MM': {'aie2ps': {'count':6 , 'base_index':0, 'split_len':3, 'index_num':2},
                          'aie4': {'count':8 , 'base_index':0, 'split_len':3, 'index_num':2}},
             'SHIM_CTRL_MM2S': {'aie4': {'count':2 , 'base_index':15, 'split_len':4, 'index_num':3}}}

def get_colnum(name):
    num = [int(i) for i in name.split('.') if i.isnumeric()]
    return num[0]

def get_pagenum(name):
    num = [int(i) for i in name.split('.') if i.isnumeric()]
    return num[1]

def high_8(num):
    return (num >> 8) & 0xFF

def low_8(num):
    return num & 0xFF

def get_text_section_name(col_num, page_num):
    return ".ctrltext." + str(col_num)+ "." + str(page_num)

def get_data_section_name(col_num, page_num):
    return ".ctrldata." + str(col_num)+ "." + str(page_num)

def get_pad_section_name(col_num, page_num):
    return ".pad." + str(col_num)

def words_to_bytes(words):
    result = []
    for word in words:
        word_data = []
        for i in range(0, 4):
            byte = (word >> ((3-i) * 8)) & 0xFF
            word_data.append(byte)
        result += reversed(word_data)
    return result

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

def parse_action_id(arg, state):
    args = arg.split('_')
    for key in action_id:
        if arg.startswith(key):
            assert state.target in action_id[key], f"Invalid {arg} in {state.target} target."
            actionId = action_id[key][state.target]
            assert len(args) == actionId['split_len'], f"Invalid key literal: {arg}"
            index = int(args[actionId['index_num']])
            assert index < actionId['count'], f"Invalid {key} index: {index}"
            return actionId['base_index'] + index

def parse_num_arg(arg, state):
    if isinstance(arg, int):
        return arg
    if arg.startswith('@'):
        label = arg[1:]
        assert state.containlabel(label) or state.containscratchpads(label), f"Label not found: {label}"
        if state.containscratchpads(label):
            #print("got scratchpad:", state.getscratchpadpos(label))
            return state.getscratchpadpos(label)
        assert state.containlabel(label), f"Label not found: {label}"
        return state.getlabelpos(label)

    elif arg.startswith(tuple(action_id.keys())):
        return parse_action_id(arg, state)

    elif arg.startswith('TILE_'):
        args = arg.split('_')
        assert len(args) == 3, f"Invalid TILE literal: {arg}"
        col = int(args[1])
        row = int(args[2])
        return ((col & 0x7F) << 5 | (row & 0x1F))

    elif arg.startswith('S2MM_'):
        args = arg.split('_')
        if state.target == 'aie4':
            raise RuntimeError(f"Error: {arg} invalid for aie4 target. Please used TILE_S2MM_/SHIM_S2MM_/MEN_S2MM_.")
        else:
            print(f"Warning: \"{args[0]}_\" is deprecated. Please used TILE_S2MM_/SHIM_S2MM_/MEN_S2MM_.")
        args = arg.split('_')
        assert len(args) == 2, f"Invalid S2MM literal: {arg}"
        index = int(args[1])
        assert index <= 5, f"Invalid S2MM index: {index}"
        return index

    elif arg.startswith('MM2S_'):
        args = arg.split('_')
        if state.target == 'aie4':
            raise RuntimeError(f"Error: {arg} invalid for aie4 target. Please used TILE_MM2S_/SHIM_MM2S_/MEN_MM2S_.")
        else:
            print(f"Warning: \"{args[0]}_\" is deprecated. Please used TILE_MM2S_/SHIM_MM2S_/MEN_MM2S_.")
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
