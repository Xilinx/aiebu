# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2022-2023 Advanced Micro Devices, Inc.

import ctypes

import pylibelf.elf
import pylibelf.libelf

from ctrlcode.disassembler.disassembler_state import DisAssemblerState
from ctrlcode.common.writer import CtrlWriter
from ctrlcode.common.reader import InputReader
from ctrlcode.common.reader import ByteReader
from ctrlcode.common.util import ELFStringTable
from ctrlcode.common.util import get_pagenum
from ctrlcode.common.util import get_colnum

class Disassembler:
    def __init__(self, isa):
        self.isa_ops = isa

    def run(self, ifile, lfile):
        state = DisAssemblerState()
        writer = CtrlWriter(lfile, 8)
        reader = InputReader(ifile)

        dummy = list(reader.read(16))
        opcode = reader.read()
        while opcode != b"":
            opcode = int.from_bytes(opcode, byteorder='little')
            if state.pos in state.labels:
                self.isa_ops['uc_dma_bd'].deserializer(state).deserialize(reader, writer)
            elif state.pos in state.local_ptrs:
                self.isa_ops['.long'].deserializer(state).deserialize(reader, writer)
            elif opcode in self.isa_ops:
                self.isa_ops[opcode].deserializer(state).deserialize(reader, writer)
            else:
                raise RuntimeError('Invalid operation: {}'.format(opcode))
            opcode = reader.read()

class ELFDisassembler():
    """
    Read the ELF file headers and display details
    """
    def __init__(self, ifilename, lfile, isa):
        assert (InputReader.isELF(ifilename)), f"Not an ELF file {ifilename}"
        self.filename = ifilename
        self.isa_ops = isa
        self.melf = pylibelf.libelf.ElfDescriptor.fromfile(self.filename,
                                                           pylibelf.libelf.Elf_Cmd.ELF_C_READ)
        self.ehdr = self.melf.elf32_getehdr()
        self.writer = CtrlWriter(lfile, 8)
        self.state = DisAssemblerState()
        curr = self.melf.elf_getscn(self.ehdr.contents.e_shstrndx)
        curr_shdr = curr.elf32_getshdr()
        curr_data = curr.elf_getdata()
        assert(curr_data.contents.d_size == curr_shdr.contents.sh_size)
        self.strtab = ELFStringTable(curr_data.contents.d_buf, curr_data.contents.d_size)

    def __del__(self):
        del self.strtab
        del self.writer
        del self.melf

    def _getsectionmode(self, shdr):
        mode = ""
        if (shdr.contents.sh_flags & pylibelf.elf.SHF_ALLOC):
            mode += "a"
        if (shdr.contents.sh_flags & pylibelf.elf.SHF_WRITE):
            mode += "w"
        if (shdr.contents.sh_flags & pylibelf.elf.SHF_EXECINSTR):
            mode += "x"
        return mode

    def _dumpcommon(self, shdr):
        mode = self._getsectionmode(shdr)
        name = self.strtab.get(shdr.contents.sh_name)
        self.writer.write_directive(f".section {name},\"{mode}\"")
        self.writer.write_directive(f".align {shdr.contents.sh_addralign}")

    def _dumptext(self, data, shdr):
        reader = ByteReader(data)
        name = self.strtab.get(shdr.contents.sh_name)
        # Dispose off the initial 16 bytes
        dummy = reader.read(16)
        opcode = reader.read()
        while (opcode != b''):
            opcode = int.from_bytes(opcode, byteorder="little")
            assert(opcode in self.isa_ops), f"Illegal opcode {opcode} in segment {name}"
            if opcode == 0xff:   # should put eop before eof
                self.writer.write_eop()
            self.isa_ops[opcode].deserializer(self.state).deserialize(reader, self.writer)
            opcode = reader.read()

    def _dumpdata(self, data, shdr):
        reader = ByteReader(data)
        opcode = reader.read()
        while (opcode != b''):
            if self.state.pos in self.state.labels:
                self.isa_ops['uc_dma_bd'].deserializer(self.state).deserialize(reader, self.writer)
            elif self.state.pos in self.state.local_ptrs:
                self.writer.write_directive(f".align 4")
                self.isa_ops['.long'].deserializer(self.state).deserialize(reader, self.writer)
            elif int.from_bytes(opcode, byteorder="little") == 165:
                self.state.pos += 1
            elif int.from_bytes(opcode, byteorder="little") == 0:
                pass
            else:
                raise RuntimeError(f"Illegal state {self.state}")
            opcode = reader.read()

    def _dump(self):
        currentcol = None
        currentlabel = ""
        curr = self.melf.elf_nextscn(None)
        while (curr != None):
            curr_shdr = curr.elf32_getshdr()
            if (curr_shdr.contents.sh_type == pylibelf.elf.SHT_PROGBITS):
                curr_name = curr_shdr.contents.sh_name
                name = self.strtab.get(curr_name)
                if (name[0:9] == ".ctrltext"):
                    colnum = get_colnum(name)
                    if colnum != currentcol:
                        self.writer.write_attach_to_group(colnum)
                        currentcol = colnum
                        self.state.externallabels.clear();
                self._dumpcommon(curr_shdr)
                if (name[0:9] == ".ctrltext"):
                    pagenum = get_pagenum(name)
                    if pagenum in self.state.externallabels:
                        l = self.state.externallabels[pagenum]
                        if currentlabel:
                            self.writer.write_endl(currentlabel[1:])
                        self.writer.write_label(l)
                        currentlabel = l
                scn_data = curr.elf_getdata()
                data = ctypes.string_at(scn_data.contents.d_buf, scn_data.contents.d_size)
                if (name[0:9] == ".ctrltext"):
                    self._dumptext(data, curr_shdr)
                else:
                    if name[0:9]  == ".ctrlbss.":
                        pass
                    else:
                        assert(name[0:9] == ".ctrldata"), f"Invalid program section name {name}"
                        self._dumpdata(data, curr_shdr)
                        self.state.reset()
            curr = self.melf.elf_nextscn(curr)

    def run(self):
        self._dump()
