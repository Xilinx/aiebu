# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2024 Advanced Micro Devices, Inc.

from ctrlcode.common.writer import AIE2_BLOB_ELFWriter
from ctrlcode.common.section import Section
from ctrlcode.common.symbol import AIE2_BLOB_Symbol
from ctrlcode.common.elf_section import ELF_Section

def section_index_callback(buf_type, elf_sections):
  if buf_type == AIE2_BLOB_Symbol.XrtPatchBufferType.xrt_patch_buffer_type_instruct:
    return elf_sections[".ctrltext"].section_index
  else:
    return elf_sections[".ctrldata"].section_index

class Assembler_blob:
    """ Assembler class for aie2 which takes blobs as input """
    INSTR_BUF = 0
    CONTROL_CODE = 1
    def __init__(self, ifile, ccfile, patch_info, elffile):
        with open(ifile, 'rb') as f:
            self.idata = f.read()
        self.ccdata = bytes()
        if ccfile:
            with open(ccfile, 'rb') as f:
                self.ccdata = f.read()
        self.symbols = patch_info
        self.elf_sections = {}
        self.ewriter = AIE2_BLOB_ELFWriter(elffile, self.symbols, self.elf_sections, section_index_callback)

    def run(self):
        """ perform assembling """
        text = ELF_Section(".ctrltext", Section.TEXT)
        text.write_bytes(self.idata)
        self.elf_sections[".ctrltext"]  = text
        if len(self.ccdata):
          data = ELF_Section(".ctrldata", Section.DATA)
          data.write_bytes(self.ccdata)
          self.elf_sections[".ctrldata"] = data
        self.ewriter.finalize()
        del self.ewriter
