# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

import git
import os
import re

from ctrlcode.common.parser import Parser
from ctrlcode.common.parser import Data
from ctrlcode.assembler.assembler_state import AssemblerState
from ctrlcode.common.writer import AIE2PS_ELFWriter, AIE4_ELFWriter
from ctrlcode.common.symbol import Symbol
from ctrlcode.common.section import Section
from ctrlcode.common.operation import Operation
from ctrlcode.common.label import Label
from ctrlcode.pager.pager import Pager
from ctrlcode.common.report import Report
from ctrlcode.common.debug import Debug
from ctrlcode.common.elf_section import ELF_Section
from ctrlcode.common.util import parse_num_arg, low_8, high_8

PAGE_SIZE = 0x2000

def section_index_callback(buf_type, elf_sections):
  return elf_sections[buf_type].section_index

class Assembler:
    """ Assembler class """
    REPO = git.Repo(os.path.dirname(os.path.abspath(__file__)), search_parent_directories=True)
    def __init__(self, target, ifilename, efilename, isa, includedirlist, mapfilename):
        self.target = target
        self.ifilename = ifilename
        self.ifile = open(self.ifilename, 'r')
        self.elffile = efilename
        includedirlist.append(os.path.dirname(os.path.abspath(ifilename)))
        self.scratchpad = {}
        self._parser = Parser(includedirlist)
        self.isa_ops = isa
        self._pager = Pager(PAGE_SIZE)
        # Create the symbol list with a dummy symbol. Note that the dummy symbol is needed
        # for various ELF sections which always have a dummy first element.
        self.symbols = [Symbol("", 0, 0, 0)]
        self._debug = Debug()
        self._report = Report(self.symbols, mapfilename, self._debug)
        self._report.setbuildid(Assembler.REPO.head.object.hexsha)
        # header[4] == 0x01 means we have more pages
        self.header = [0xFF]*2 + [0x00]*14
        self.elf_sections = {}
        if self.target == "aie2ps":
            self.patch_57 = self.aie2ps_patch_57
        elif self.target == "aie4":
            self.patch_57 = self.aie4_patch_57
        else:
            raise RuntimeError(f"Target ({self.target}) not supported!!!")

    def __del__(self):
        self.ifile.close()

    def alligner(self, page_num, parser_text, parser_data):
        newData = []
        pos = 0
        last_label = 0
        numalign = 0
        for text in parser_text:
            token = text.gettoken()
            if token.name in self.isa_ops:
                pos += self.isa_ops[token.name].serializer(token.args, None).size()
        # we loop though data section, check if alignment is as per needed for that token.
        # if not, we calculate alignment and insert '.align' opcode to align the token.
        for index, data in enumerate(parser_data):
            token = data.gettoken()
            if token.name == ".align":
                continue;
            if isinstance(token, Label):
                last_label = index
            if token.name in self.isa_ops:
                align = self.isa_ops[token.name].serializer(token.args, None).align()
                self.isa_ops[token.name].serializer(token.args, None)
                if align != 0:
                    newData.insert(numalign + last_label, \
                                   Data(Operation('.align', str(align), ""), \
                                   Section.DATA, 0, page_num, "", f".align {align}", -1))
                    numalign += 1
                pos += self.isa_ops[token.name].serializer(token.args, None).size()
            newData.append(data)
        return newData

    def run(self):
        """ perform assembling """
        for line_number, line in enumerate(self.ifile, 1):
            self._parser.parse_line(line, self.ifilename, line_number)
        #print("PRINTING PARSER:", self._parser)
        #for c in self._parser.col:
        #    print(self._parser.col[c])

        pages = []
        for col in self._parser.getcollist():
            relative_page_index = 0
            padsize = 0
            labelpageindex = self._parser.getcollabelpageindex(col)
            scratchpad = self._parser.getcolscratchpad(col)
            for label in self._parser.gettextlabelsforcol(col):
                #print("COL:", col, " LABEL:", label)
                data = self._parser.getcoltextforlabel(col, label) + self._parser.getcoldata(col)
                state = AssemblerState(self.target, self.isa_ops, data, scratchpad, labelpageindex, True)
                #print(state)
                # create pages for 8k (text + data)
                relative_page_index, pgs = self._pager.pagify(state, col, data, relative_page_index)
                pages += pgs
                #print("COL:", col, " LABEL:", label, " relative_page_index:", relative_page_index, "numpages:", len(pgs))
                labelpageindex[label.rsplit('::', 1)[-1] or label] = relative_page_index - len(pgs)
            for pad in scratchpad:
                scratchpad[pad]["offset"] = padsize
                scratchpad[pad]["base"] = PAGE_SIZE * relative_page_index
                padsize += scratchpad[pad]["size"]

            if len(scratchpad):
                self.elf_sections[".ctrlbss." + str(col)] = ELF_Section(".ctrlbss." + str(col), Section.DATA)
            #print("PAD:", padsize)
            for c in range(padsize):
                self.elf_sections[".ctrlbss." + str(col)].write_byte(0x00)
            #print("scratchpad[",col,"]:", scratchpad)
            #print("labelpageindex[",col,"]:", labelpageindex)

        # create elfwriter
        if self.target == "aie2ps":
            ewriter = AIE2PS_ELFWriter(self.elffile, self.symbols, self.elf_sections, section_index_callback)
        elif self.target == "aie4":
            ewriter = AIE4_ELFWriter(self.elffile, self.symbols, self.elf_sections, section_index_callback)
        else:
            raise RuntimeError(f"Target ({self.target}) not supported!!!")

        # for each page do the serialize of text and data section
        for page in pages:
            labelpageindex = self._parser.getcollabelpageindex(page.col_num)
            ooo = page.getout_of_order_page()
            assert len(ooo) <= 2
            ooo_page_len_1 = pages[labelpageindex[ooo[0]]].cur_page_len if len(ooo) else 0
            ooo_page_len_2 = pages[labelpageindex[ooo[1]]].cur_page_len if (len(ooo) == 2) else 0

            self.elf_sections[page.get_text_section_name()] = ELF_Section(page.get_text_section_name(), Section.TEXT)
            self.elf_sections[page.get_data_section_name()] = ELF_Section(page.get_data_section_name(), Section.DATA)
            self.page_writer(self.elf_sections[page.get_text_section_name()],
                             self.elf_sections[page.get_data_section_name()],
                             page, ooo_page_len_1, ooo_page_len_2)

        ewriter.finalize()
        del ewriter
        self._report.generate()

    def page_writer(self, text_section, data_section, page, ooo_page_len_1, ooo_page_len_2):
        """ write page to text_section, data_section """
        page_header = list(self.header)
        page_header[2] =  low_8(page.page_num)  # Lower 8 bit of page_index
        page_header[3] =  high_8(page.page_num)  # Higher 8 bit of page_index
        page_header[4] =  low_8(ooo_page_len_1)  # Lower 8 bit of out_of_order_page_len pdi/save
        page_header[5] =  high_8(ooo_page_len_1)  # Higher 8 bit of out_of_order_page_len pdi/save
        page_header[6] =  low_8(ooo_page_len_2)  # Lower 8 bit of out_of_order_page_len restore
        page_header[7] =  high_8(ooo_page_len_2)  # Higher 8 bit of out_of_order_page_len restore
        page_header[8] =  low_8(page.cur_page_len)  # Lower 8 bit of cur_page_len
        page_header[9] =  high_8(page.cur_page_len)  # Higher 8 bit of cur_page_len
        page_header[10] =  low_8(page.in_order_page_len)  # Lower 8 bit of in_order_page_len
        page_header[11] =  high_8(page.in_order_page_len)  # Higher 8 bit of in_order_page_len

        if page.islastpage:
            page_header[4] = 0x00    # not the last page

        # 4.1 Align each operation if needed, currently ony ucDmaOp need to be aligned to 128 bit
        page.data = self.alligner(page.page_num, page.text, page.data)

        # create state for each page
        pagestate = AssemblerState(self.target, self.isa_ops, page.text + page.data, self._parser.getcolscratchpad(page.col_num),
                                   self._parser.getcollabelpageindex(page.col_num), False)

        for byte in page_header:
            text_section.write_byte(byte)

        # serialize text in elf format
        pagestate.section = Section.TEXT
        offset = text_section.tell()
        for text in page.text:
            token = text.gettoken()
            if token.name in ["start_job", "start_job_deferred"]:
                pc_low = page.page_num * PAGE_SIZE + text_section.tell()
                pc_high = pc_low + pagestate.getjob(int(token.args[0]) if not token.args[0].startswith("0x") else int(token.args[0][2:], 16)).getsize() -1
                fid = self._debug.addfunction(text.getfilename(), f"{token.name.upper()}_{token.args[0]}", pc_high, pc_low, page.col_num, page.page_num)

            pc_low = page.page_num * PAGE_SIZE + text_section.tell()
            pc_high = pc_low + self.isa_ops[token.name].serializer(token.args, self).size() -1
            self._debug.addtextline(fid, text.getlinenum(), pc_high, pc_low, text.getline())

            pagestate.setpos(text_section.tell() - offset)
            if isinstance(token, Operation):
                self.isa_ops[token.name].serializer(token.args, pagestate) \
                                        .serialize(text_section, data_section, page.col_num, page.page_num, self.symbols)
            else:
                raise RuntimeError('Invalid operation: {}'.format(token.name))

        # serialize data in elf format
        for data in page.data:
            token = data.gettoken()
            pagestate.setpos(text_section.tell() + data_section.tell() - offset)
            if isinstance(token, Label):
                pagestate.section = Section.DATA
                assert pagestate.getpos() == pagestate.getlabelpos(token.name), \
                    f"Label {token.name} pagestate.pos {pagestate.getpos()} != \
                      pagestate.getlabelpos(token.name) {pagestate.getlabelpos(token.name)}"
            elif isinstance(token, Operation):
                pc_low = page.page_num * PAGE_SIZE + text_section.tell() + data_section.tell()
                pc_high = pc_low + self.isa_ops[token.name].serializer(token.args, pagestate).size() -1
                self._debug.adddataline(fid, data.getlinenum(), pc_high, pc_low, data.getline())

                # operation can be uc_dma_bd_shim, uc_dma_bd, align, .long
                self.isa_ops[token.name].serializer(token.args, pagestate) \
                                        .serialize(text_section, data_section, page.col_num, page.page_num, self.symbols)
            else:
                raise RuntimeError('Invalid operation: {}'.format(token.name))

        for spad in pagestate.patch:
            offset = parse_num_arg(pagestate.patch[spad], pagestate)
            self.patch_57(text_section, data_section, offset + len(self.header), pagestate.getscratchpadpos(spad));

        # Padding
        count = PAGE_SIZE - (text_section.tell() + data_section.tell())
        for c in range(count):
          data_section.write_byte(0x00)

        pagestate.settextsize(text_section.tell());
        pagestate.setdatasize(data_section.tell());

        self._report.addpage(page, pagestate)

    # http://cervino-doc.xilinx.com/aie2ps/r0p28.1/tile_links/xregdb_aie2ps_shim_tile_doc.html#noc_module___DMA_BD0_0
    def aie2ps_patch_57(self, text_section, data_section, offset, value):
        assert text_section.tell() + data_section.tell() > offset, f"offset beyond range !!!"
        assert text_section.tell() < offset, f"patch 57 patches shimbd, which should be in data section !!!"
        offset = offset - text_section.tell()

        bd1 = data_section.read_word(offset + 1*4)
        bd2 = data_section.read_word(offset + 2*4)
        bd8 = data_section.read_word(offset + 8*4)

        arg = ((bd8 & 0x1FF) << 48) + ((bd2 & 0xFFFF) << 32) + (bd1 & 0xFFFFFFFF)
        patch = arg + value
        pbd1 = patch & 0xFFFFFFFF
        pbd2 = ((patch >> 32) & 0xFFFF) | (bd2 & 0xFFFF0000)
        pbd8 = ((patch >> 48) & 0x1FF) | (bd8 & 0xFFFFFE00)

        data_section.write_word_at(offset + 1*4, pbd1)
        data_section.write_word_at(offset + 2*4, pbd2)
        data_section.write_word_at(offset + 8*4, pbd8)

    # http://cervino-doc/aie4/r0p18.2/tile_links/xregdb_aie4_shim_tile_doc.html#noc_module___DMA_BD0_0
    def aie4_patch_57(self, text_section, data_section, offset, value):
        assert text_section.tell() + data_section.tell() > offset, f"offset beyond range !!!"
        assert text_section.tell() < offset, f"patch 57 patches shimbd, which should be in data section !!!"
        offset = offset - text_section.tell()

        bd0 = data_section.read_word(offset)
        bd1 = data_section.read_word(offset + 1*4)

        arg = ((bd0 & 0x1FFFFFF) << 32) + (bd1 & 0xFFFFFFFF)
        patch = arg + value
        pbd1 = patch & 0xFFFFFFFF
        pbd0 = ((patch >> 32) & 0x1FFFFFF) | (bd0 & 0xFFFF0000)

        data_section.write_word_at(offset, pbd0)
        data_section.write_word_at(offset + 1*4, pbd1)
