#! /usr/bin/env python3

# SPDX-License-Identifier: MIT
# Copyright (C) 2022-2023 Advanced Micro Devices, Inc.

import sys
import argparse
import os.path
import re
import pylibelf.elf
import pylibelf.libelf
import ctypes
from ctrlcode.common.reader import InputReader
from ctrlcode.common.reader import ByteReader
from ctrlcode.common.writer import AsmWriter
from ctrlcode.common.util import ELFStringTable
from ctrlcode.common.section import Section

class LegacyGenerator():
    """
    Read the ELF file headers and display details
    """
    def __init__(self, ifilename, ofile):
        assert (InputReader.isELF(ifilename)), f"Not an ELF file {ifilename}"
        self.filename = ifilename
        self.melf = pylibelf.libelf.ElfDescriptor.fromfile(self.filename, pylibelf.libelf.Elf_Cmd.ELF_C_READ)
        self.ehdr = self.melf.elf32_getehdr()
        self.writer = AsmWriter(ofile, 8)
        curr = self.melf.elf_getscn(self.ehdr.contents.e_shstrndx)
        curr_shdr = curr.elf32_getshdr()
        curr_data = curr.elf_getdata()
        assert(curr_data.contents.d_size == curr_shdr.contents.sh_size)
        self.strtab = ELFStringTable(curr_data.contents.d_buf, curr_data.contents.d_size)

    def __del__(self):
        del self.strtab
        del self.writer
        del self.melf

    def _dumptext(self, data, shdr, page):
        reader = ByteReader(data)
        name = self.strtab.get(shdr.contents.sh_name)
        opcode = reader.read()
        while (opcode != b''):
            opcode = int.from_bytes(opcode, byteorder = "little")
            self.writer.write_byte(opcode, Section.TEXT, page)
            opcode = reader.read()

    def _dumpdata(self, data, shdr, page):
        reader = ByteReader(data)
        opcode = reader.read()
        while (opcode != b''):
            opcode = int.from_bytes(opcode, byteorder = "little")
            self.writer.write_byte(opcode, Section.DATA, page)
            opcode = reader.read()
        offset = self.writer.tell(Section.DATA)

    def _dump(self):
      curr = self.melf.elf_nextscn(None)
      index = 0
      while (curr != None):
          curr_shdr = curr.elf32_getshdr()
          if (curr_shdr.contents.sh_type == pylibelf.elf.SHT_PROGBITS):
              curr_name = curr_shdr.contents.sh_name
              name = self.strtab.get(curr_name)
              scn_data = curr.elf_getdata()
              data = ctypes.string_at(scn_data.contents.d_buf, scn_data.contents.d_size)
              if (name[0:9] == ".ctrltext"):
                  idx = re.findall('\d+', name)
                  self._dumptext(data, curr_shdr, int(idx[0]))
              else:
                  if name[0:9]  == ".ctrlbss.":
                      pass
                  else:
                      assert(name[0:9]  == ".ctrldata"), f"Invalid program section name {name}"
                      idx = re.findall('\d+', name)
                      self._dumpdata(data, curr_shdr, int(idx[0]))
          curr = self.melf.elf_nextscn(curr)

    def run(self):
        self._dump()

def parse_command_line(args):
  """ Command line parser for assembler """
  msg = "Utility to convert EFL to legacy output"
  parser = argparse.ArgumentParser(description = msg)

  parser.add_argument("-i", "--input", dest ='ifilename', nargs = 1, required=True,
                      help = "input ElF file name")
  parser.add_argument("-o", "--output", dest ='ofilename', nargs = 1,
                      help = "output legacy file name")
  # strip out the argv[0]
  return parser.parse_args(args[1:])


if __name__ == '__main__':
  argtab = parse_command_line(sys.argv)

  ofile = sys.stdout
  if (argtab.ofilename != None):
    ofile = open(argtab.ofilename[0], 'w')

  operation = LegacyGenerator(argtab.ifilename[0], ofile)
  operation.run()

  del operation
  if (argtab.ofilename != None):
    ofile.close()

  sys.exit(0)
