import git
import os

from ctrlcode.common.parser import Parser
from ctrlcode.common.parser import Data
from ctrlcode.assembler.assembler_state import AssemblerState
from ctrlcode.common.writer import AIE2PS_ELFWriter
from ctrlcode.common.symbol import Symbol
from ctrlcode.common.section import Section
from ctrlcode.common.operation import Operation
from ctrlcode.common.label import Label
from ctrlcode.pager.pager import Pager
from ctrlcode.common.report import Report
from ctrlcode.common.debug import Debug
from ctrlcode.common.elf_section import ELF_Section

PAGE_SIZE = 0x2000

def section_index_callback(buf_type, elf_sections):
  return elf_sections[buf_type].section_index

class Assembler:
    """ Assembler class """
    REPO = git.Repo(os.path.dirname(os.path.abspath(__file__)), search_parent_directories=True)
    def __init__(self, ifilename, efilename, isa, includedirlist, mapfilename):
        self.ifilename = ifilename
        self.ifile = open(self.ifilename, 'r')
        self.elffile = efilename
        includedirlist.append(os.path.dirname(os.path.abspath(ifilename)))
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
        self.header = [0xFF, 0xFF, 0x00, 0x00] + [0x01] + [0x00] * 11
        self.elf_sections = {}

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
            if isinstance(token, Label):
                last_label = index
            if token.name in self.isa_ops:
                align = self.isa_ops[token.name].serializer(token.args, None).align()
                self.isa_ops[token.name].serializer(token.args, None)
                if align != 0:
                    newData.insert(numalign + last_label, \
                                   Data(Operation('.align', str(align)), \
                                   Section.DATA, 0, page_num, "", f".align {align}", -1))
                    numalign += 1
                pos += self.isa_ops[token.name].serializer(token.args, None).size()
            newData.append(data)
        return newData

    def run(self):
        """ perform assembling """
        for line_number, line in enumerate(self.ifile, 1):
            self._parser.parse_line(line, self.ifilename, line_number)

        pages = []
        for col in self._parser.getcollist():
            data = self._parser.getcoldata(col)
            state = AssemblerState(self.isa_ops, data)

            # create pages for 8k (text + data)
            pages += self._pager.pagify(state, col, data)

        # create elfwriter
        ewriter = AIE2PS_ELFWriter(self.elffile, self.symbols, self.elf_sections, section_index_callback)

        # for each page do the serialize of text and data section
        for page in pages:
            self.elf_sections[".ctrltext." + str(page.col_num)+ "." + str(page.page_num)] = ELF_Section(".ctrltext." + str(page.col_num)+ "." + str(page.page_num), Section.TEXT)
            self.elf_sections[".ctrldata." + str(page.col_num)+ "." + str(page.page_num)] = ELF_Section(".ctrldata." + str(page.col_num)+ "." + str(page.page_num), Section.DATA)
            self.page_writer(self.elf_sections[".ctrltext." + str(page.col_num)+ "." + str(page.page_num)],
                             self.elf_sections[".ctrldata." + str(page.col_num)+ "." + str(page.page_num)],
                             page)

        ewriter.finalize()
        del ewriter
        self._report.generate()

    def page_writer(self, text_section, data_section, page):
        """ write page to ewriter """
        page_header = list(self.header)
        if page.islastpage:
            page_header[4] = 0x00    # not the last page

        # 4.1 Align each operation if needed, currently ony ucDmaOp need to be aligned to 128 bit
        page.data = self.alligner(page.page_num, page.text, page.data)

        # create state for each page
        pagestate = AssemblerState(self.isa_ops, page.text + page.data)

        for byte in page_header:
            text_section.write_byte(byte)

        # serialize text in elf format
        pagestate.section = Section.TEXT
        offset = text_section.tell()
        for text in page.text:
            token = text.gettoken()
            if token.name in ["start_job", "start_job_deferred"]:
                pc_low = page.page_num * PAGE_SIZE + text_section.tell()
                pc_high = pc_low + pagestate.getjob(int(token.args[0])).getsize() -1
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

        # Padding
        count = PAGE_SIZE - (text_section.tell() + data_section.tell())
        for c in range(count):
          data_section.write_byte(0x00)

        pagestate.settextsize(text_section.tell());
        pagestate.setdatasize(data_section.tell());

        self._report.addpage(page, pagestate)
