# SPDX-License-Identifier: MIT
# Copyright (C) 2022-2023 Advanced Micro Devices, Inc.

import ctypes

import pylibelf.elf
import pylibelf.libelf
import abc

from ctrlcode.common.section import Section
from ctrlcode.common.util import ELFStringTable
from ctrlcode.common.util import ELFSymbolTable
from ctrlcode.common.util import ELFRelaTable
from ctrlcode.common.util import ELFDynamicTable
from ctrlcode.common.symbol import Symbol
from ctrlcode.common.util import words_to_bytes

class WriterBase(abc.ABC):
    PAGE_SIZE = 0x2000

    def write_words(self, words, section, page):
        data = words_to_bytes(words)
        for byte in data:
            self.write_byte(byte, section, page)

    @abc.abstractmethod
    def write_byte(self, byte, section, page):
        del byte, section, page
        return

class AsmWriter(WriterBase):
    def __init__(self, f, width=16):
        self.i = 0
        self.width = width
        self.f = f

    def write_byte(self, byte, section, page):
        # Mark unused function parameters
        del section, page
        self.f.write('{:02x}'.format(byte))
        self.i += 1
        if (self.i % self.width) == 0:
            self.f.write("\n")
        else:
            self.f.write(' ')

    def tell(self, section):
        # Mark unused function parameters
        del section
        return self.i

    def padding_page(self):
        index = self.tell(Section.DATA)
        index = index % AsmWriter.PAGE_SIZE
        while index > 0 and index < AsmWriter.PAGE_SIZE:
            self.write_byte(0, Section.DATA, 0)
            index += 1

    def __del__(self):
        self.f.flush()
        self.f.close()

class CtrlWriter:
    def __init__(self, f, width=16):
        self.i = 0
        self.width = width
        self.f = f
        self.current_label = None
        self.f.write(";Generated by ctrlcode disassembler\n")

    def write_directive(self, name):
        self.f.write(name + "\n")

    def write_label(self, name):
        if name.startswith('@'):
            name = name[1:]
        self.f.write(name + ":\n")
        self.current_label = name

    def write_endl(self, name):
        if name.startswith('@'):
            name = name[1:]
        self.f.write(f".endl {name}\n")

    def write_eop(self):
        self.f.write(".eop\n")

    def write_attach_to_group(self, col):
        self.f.write(f".attach_to_group {col}\n")

    def write_operation(self, name, args, label):
        if (self.current_label == label):
            self.f.write("    ")
        self.f.write(name + "\t")
        for index, arg in enumerate(args):
            if self.current_label != label:
                self.f.write(" ")
            self.f.write(str(arg))
            if index < len(args)-1:
                self.f.write(", ")
        self.f.write("\n")

    def tell(self, section):
        del section
        return self.i

class ELFWriter:
    AMD_AIE_CTRLCODE_ALIGN = 16
    AMD_AIE_CTRLCODE_TEXT_ALIGN = 16
    AMD_AIE_CTRLCODE_DATA_ALIGN = 16
    AMD_AIE_CTRLCODE_PHDR_ALIGN = 8

    PROGRAM_HEADER_STATIC_COUNT = 2
    PROGRAM_HEADER_DYNAMIC_COUNT = 3

    def __init__(self, OSABI, ABIVERSION, filename, symbols, elf_sections, section_index_callback):
        self.filename = filename
        self.symbols = symbols
        self.strtab = ELFStringTable()
        self.melf = pylibelf.libelf.ElfDescriptor.fromfile(self.filename,
                                                           pylibelf.libelf.Elf_Cmd.ELF_C_WRITE)
        self.ehdr = self.melf.elf32_newehdr()

        self.ehdr.contents.e_ident[pylibelf.elf.EI_DATA] = pylibelf.elf.ELFDATA2LSB
        self.ehdr.contents.e_ident[pylibelf.elf.EI_VERSION] = pylibelf.elf.EV_CURRENT
        # Our own ABI version
        self.ehdr.contents.e_ident[pylibelf.elf.EI_OSABI] = OSABI
        self.ehdr.contents.e_ident[pylibelf.elf.EI_ABIVERSION] = ABIVERSION
        # Repurpose obsolete EM_M32 for our machine type
        self.ehdr.contents.e_machine = pylibelf.elf.EM_M32
        self.ehdr.contents.e_type = pylibelf.elf.ET_EXEC
        self.ehdr.contents.e_flags = 0x0
        self.strtab.add("")
        self.elf_sections = elf_sections
        self.section_index_callback = section_index_callback
        self._numsection = 1

        self.symbol_schema2r_info = {
            Symbol.XrtPatchSchema.xrt_patch_schema_scaler_32: pylibelf.elf.R_M32R_32_RELA,
            Symbol.XrtPatchSchema.xrt_patch_schema_shim_dma_48: pylibelf.elf.R_M32R_24_RELA,
            Symbol.XrtPatchSchema.xrt_patch_schema_shim_dma_57: pylibelf.elf.R_M32R_NUM,
            Symbol.XrtPatchSchema.xrt_patch_schema_control_packet_48:pylibelf.elf.R_M32R_REL32,
            Symbol.XrtPatchSchema.xrt_patch_schema_control_packet_57:pylibelf.elf.R_M32R_GOT24,
            Symbol.XrtPatchSchema.xrt_patch_schema_shim_dma_57_aie4: pylibelf.elf.R_M32R_NUM,
            Symbol.XrtPatchSchema.xrt_patch_schema_unknown: pylibelf.elf.R_M32R_NONE
        }

    def _populate_elfsection(self, elf_section):
        # Populate text segment
        scn = self.melf.elf_newscn()
        scn_data = scn.elf_newdata()
        scn_data.contents.d_align = ELFWriter.AMD_AIE_CTRLCODE_ALIGN
        scn_data.contents.d_off = 0
        countt = int(len(elf_section.bytearray) / 4)
        twords = (ctypes.c_uint * countt).from_buffer(elf_section.bytearray)
        scn_data.contents.d_buf = ctypes.cast(twords, ctypes.c_void_p)
        scn_data.contents.d_type = pylibelf.libelf.Elf_Type.ELF_T_WORD
        scn_data.contents.d_size = len(elf_section.bytearray)
        scn_data.contents.d_version = pylibelf.elf.EV_CURRENT

        shdr = scn.elf32_getshdr()
        shdr.contents.sh_name = self.strtab.add(elf_section.name)
        shdr.contents.sh_type = pylibelf.elf.SHT_PROGBITS
        if Section.TEXT == elf_section.section:
          shdr.contents.sh_flags = pylibelf.elf.SHF_ALLOC | pylibelf.elf.SHF_EXECINSTR
        else:
          shdr.contents.sh_flags = pylibelf.elf.SHF_ALLOC | pylibelf.elf.SHF_WRITE
        shdr.contents.sh_entsize = 0
        elf_section.section_index = scn.elf_ndxscn()
        return elf_section.section_index

    def _populate_dynamic_section(self, dynstrindex, relaindex, relasize):
        dynamictab = ELFDynamicTable()
        scn = self.melf.elf_newscn()
        data = scn.elf_newdata()
        data.contents.d_align = 8
        data.contents.d_off = 0
        data.contents.d_type = pylibelf.libelf.Elf_Type.ELF_T_WORD
        data.contents.d_version = pylibelf.elf.EV_CURRENT

        shdr = scn.elf32_getshdr()
        shdr.contents.sh_name = self.strtab.add(".dynamic")
        shdr.contents.sh_type = pylibelf.elf.SHT_DYNAMIC
        shdr.contents.sh_flags = pylibelf.elf.SHF_ALLOC
        shdr.contents.sh_entsize = ctypes.sizeof(pylibelf.elf.Elf32_Dyn)
        shdr.contents.sh_link = dynstrindex
        shdr.contents.sh_info = 0

        d_un = pylibelf.elf._d_un()
        d_un.d_ptr = relaindex
        dynamictab.add(pylibelf.elf.Elf32_Dyn(pylibelf.elf.DT_RELA, d_un))
        d_un.d_val = relasize
        dynamictab.add(pylibelf.elf.Elf32_Dyn(pylibelf.elf.DT_RELASZ, d_un))

        dynamicdata = dynamictab.packsyms()
        data.contents.d_size = ctypes.sizeof(dynamicdata)
        data.contents.d_buf = ctypes.cast(dynamicdata, ctypes.c_void_p)

    def _populate_rela_section(self, refindex, dynsymindex, dynstrindex):
        rtab = ELFRelaTable()
        scn = self.melf.elf_newscn()
        data = scn.elf_newdata()
        data.contents.d_align = 8
        data.contents.d_off = 0
        data.contents.d_type = pylibelf.libelf.Elf_Type.ELF_T_BYTE
        data.contents.d_version = pylibelf.elf.EV_CURRENT
        relaindex = scn.elf_ndxscn()

        shdr = scn.elf32_getshdr()
        shdr.contents.sh_name = self.strtab.add(".rela.dyn")
        shdr.contents.sh_type = pylibelf.elf.SHT_RELA
        shdr.contents.sh_flags = pylibelf.elf.SHF_ALLOC
        shdr.contents.sh_entsize = ctypes.sizeof(pylibelf.elf.Elf32_Rela)
        shdr.contents.sh_link = dynsymindex
        shdr.contents.sh_info = refindex

        index = 0
        relasize = 0
        for symbol in self.symbols:
            if (len(symbol.name) == 0):
                index += 1
                continue
            info = self.symbol_schema2r_info[symbol.schema]
            rtab.add(pylibelf.elf.Elf32_Rela(symbol.offset,
                                             pylibelf.elf.ELF32_R_INFO(index, info),
                                             symbol.schema))
            relasize += shdr.contents.sh_entsize
            index += 1

        rdata = rtab.packsyms()
        data.contents.d_size = ctypes.sizeof(rdata)
        data.contents.d_buf = ctypes.cast(rdata, ctypes.c_void_p)

        self._populate_dynamic_section(dynstrindex, relaindex, relasize)

    def _populate_symbols_and_relocation_sections(self, refindex):
        dstrtab = ELFStringTable()
        symtab = ELFSymbolTable()
        scn = self.melf.elf_newscn()
        data = scn.elf_newdata()
        data.contents.d_align = 1
        data.contents.d_off = 0
        data.contents.d_type = pylibelf.libelf.Elf_Type.ELF_T_BYTE
        data.contents.d_version = pylibelf.elf.EV_CURRENT
        dynstrindex = scn.elf_ndxscn()

        shdr = scn.elf32_getshdr()
        shdr.contents.sh_name = self.strtab.add(".dynstr")
        shdr.contents.sh_type = pylibelf.elf.SHT_STRTAB
        shdr.contents.sh_flags = pylibelf.elf.SHF_STRINGS | pylibelf.elf.SHF_ALLOC
        shdr.contents.sh_entsize = 0

        defaultlocal = 0xffff
        for symbol in self.symbols:
            #print(symbol)
            loc = dstrtab.add(symbol.name)
            syminfo = pylibelf.elf.ELF32_ST_INFO(pylibelf.elf.STB_GLOBAL, pylibelf.elf.STT_OBJECT)
#           syminfo = pylibelf.elf.ELF32_ST_INFO(symbol.sbind, symbol.stype)
            symtab.add(pylibelf.elf.Elf32_Sym(loc, 0, 0, syminfo, 0,
                       self.section_index_callback(symbol.getbuftype(), self.elf_sections)))

        symtab[0].st_info = 0
        symtab[0].st_shndx = pylibelf.elf.SHN_UNDEF

        defaultlocal = 0
        dsymsdata = dstrtab.packsyms()
        data.contents.d_size = ctypes.sizeof(dsymsdata)
        data.contents.d_buf = ctypes.cast(dsymsdata, ctypes.c_void_p)

        dynstrindex = scn.elf_ndxscn()
        scn = self.melf.elf_newscn()
        data = scn.elf_newdata()
        data.contents.d_align = 8
        data.contents.d_off = 0
        data.contents.d_type = pylibelf.libelf.Elf_Type.ELF_T_BYTE
        data.contents.d_version = pylibelf.elf.EV_CURRENT

        symsdata = symtab.packsyms()
        data.contents.d_size = ctypes.sizeof(symsdata)
        data.contents.d_buf = ctypes.cast(symsdata, ctypes.c_void_p)

        shdr = scn.elf32_getshdr()
        shdr.contents.sh_name = self.strtab.add(".dynsym")
        shdr.contents.sh_type = pylibelf.elf.SHT_DYNSYM
        shdr.contents.sh_flags = pylibelf.elf.SHF_ALLOC
        shdr.contents.sh_entsize = ctypes.sizeof(pylibelf.elf.Elf32_Sym)
        shdr.contents.sh_link = dynstrindex
        shdr.contents.sh_info = defaultlocal + 1

        self._populate_rela_section(refindex, scn.elf_ndxscn(), dynstrindex)

    def _populate_program_load_header(self, index, shdr_start, shdr_next):
        assert((shdr_start != None) and (shdr_next != None)), \
            "Need a start and next section to build program header"
        assert((index >= 2) and (index < ELFWriter.PROGRAM_HEADER_DYNAMIC_COUNT + self._numsection)), \
            f"Program LOAD segments should start from index 2 but should be less than \
             {ELFWriter.PROGRAM_HEADER_DYNAMIC_COUNT + self._numsection}"
        name = self.strtab.get(shdr_start.contents.sh_name)

        self.phdr[index].p_offset = shdr_start.contents.sh_offset
        self.phdr[index].p_vaddr = shdr_start.contents.sh_offset
        self.phdr[index].p_paddr = shdr_start.contents.sh_offset

        # All sections from start to next, but not including next
        self.phdr[index].p_filesz = shdr_next.contents.sh_offset - shdr_start.contents.sh_offset
        self.phdr[index].p_memsz = self.phdr[index].p_filesz
        self.phdr[index].p_type = pylibelf.elf.PT_LOAD
        if ("text" in name):
            self.phdr[index].p_flags = pylibelf.elf.PF_R | pylibelf.elf.PF_X
            self.phdr[index].p_align = ELFWriter.AMD_AIE_CTRLCODE_TEXT_ALIGN
        elif ("data" in name or "pad" in name or "dump" in name):
            self.phdr[index].p_flags = pylibelf.elf.PF_R | pylibelf.elf.PF_W
            self.phdr[index].p_align = ELFWriter.AMD_AIE_CTRLCODE_DATA_ALIGN
        else:
            assert(name == ".dynamic"), "Illegal section name {}".format(name)
            self.phdr[index].p_flags = pylibelf.elf.PF_R | pylibelf.elf.PF_W
            self.phdr[index].p_align = ELFWriter.AMD_AIE_CTRLCODE_PHDR_ALIGN
            self.phdr[index].p_type = pylibelf.elf.PT_DYNAMIC

    def _find_first_program_section(self):
        curr = self.melf.elf_nextscn(None)
        while (curr != None):
            curr_shdr = curr.elf32_getshdr()
            if (curr_shdr.contents.sh_type == pylibelf.elf.SHT_PROGBITS):
                break
            else:
                curr = self.melf.elf_nextscn(curr)
        return curr

    def _populate_program_header(self):
        self.phdr[0].p_type = pylibelf.elf.PT_PHDR
        self.phdr[0].p_offset = self.ehdr.contents.e_phoff
        self.phdr[0].p_vaddr = self.ehdr.contents.e_phoff
        self.phdr[0].p_paddr = self.ehdr.contents.e_phoff
        # Should include space taken by all the Program Headers
        if len(self.symbols) == 1:
            pgmh_cnt = ELFWriter.PROGRAM_HEADER_STATIC_COUNT + self._numsection
        else:
            pgmh_cnt = ELFWriter.PROGRAM_HEADER_DYNAMIC_COUNT + self._numsection
        self.phdr[0].p_filesz = pylibelf.libelf.elf32_fsize(pylibelf.libelf.Elf_Type.ELF_T_PHDR,
                                                            pgmh_cnt,
                                                            pylibelf.elf.EV_CURRENT)
        self.phdr[0].p_memsz = self.phdr[0].p_filesz
        self.phdr[0].p_flags = pylibelf.elf.PF_R
        self.phdr[0].p_align = ELFWriter.AMD_AIE_CTRLCODE_PHDR_ALIGN

    def finalize(self):
        self._numsection = len(self.elf_sections)
        if (len(self.symbols) == 1):
            self.phdr = self.melf.elf32_newphdr(ELFWriter.PROGRAM_HEADER_STATIC_COUNT + self._numsection)
        else:
            self.phdr = self.melf.elf32_newphdr(ELFWriter.PROGRAM_HEADER_DYNAMIC_COUNT + self._numsection)

        ndxscn = None
        for elf_section_key in self.elf_sections:
          ndxscn = self._populate_elfsection(self.elf_sections[elf_section_key])

        if (ndxscn != None) and (len(self.symbols) > 1):
          self._populate_symbols_and_relocation_sections(ndxscn)

        scn = self.melf.elf_newscn()
        data = scn.elf_newdata()
        data.contents.d_align = 1
        data.contents.d_off = 0
        data.contents.d_type = pylibelf.libelf.Elf_Type.ELF_T_BYTE
        data.contents.d_version = pylibelf.elf.EV_CURRENT

        shdr = scn.elf32_getshdr()
        shdr.contents.sh_name = self.strtab.add(".shstrtab")
        shdr.contents.sh_type = pylibelf.elf.SHT_STRTAB
        shdr.contents.sh_flags = pylibelf.elf.SHF_STRINGS | pylibelf.elf.SHF_ALLOC
        shdr.contents.sh_entsize = 0

        # Now that all sections have been named, create the native symbol table buffer
        symsdata = self.strtab.packsyms()
        data.contents.d_size = ctypes.sizeof(symsdata)
        data.contents.d_buf = ctypes.cast(symsdata, ctypes.c_void_p)
        self.ehdr.contents.e_shstrndx = scn.elf_ndxscn()

        self.melf.elf_update(pylibelf.libelf.Elf_Cmd.ELF_C_NULL)

        # Populate PHDR table
        self._populate_program_header()

        # This LOAD should include all the sections from the beginning of the file to the first
        # Program Section (exclusive)
        curr = self._find_first_program_section()
        assert(curr != None)
        curr_shdr = curr.elf32_getshdr()
        self.phdr[1].p_type = pylibelf.elf.PT_LOAD
        self.phdr[1].p_offset = 0
        self.phdr[1].p_vaddr = 0
        self.phdr[1].p_paddr = 0
        self.phdr[1].p_filesz = curr_shdr.contents.sh_offset
        self.phdr[1].p_memsz = self.phdr[1].p_filesz
        self.phdr[1].p_flags = pylibelf.elf.PF_R
        self.phdr[1].p_align = ELFWriter.AMD_AIE_CTRLCODE_PHDR_ALIGN

        # Now populate LOAD tables for ctrltext
        index = 2
        first = curr
        while (curr != None):
            curr_shdr = curr.elf32_getshdr()
            curr_name = curr_shdr.contents.sh_name
            if (curr_shdr.contents.sh_type == pylibelf.elf.SHT_PROGBITS):
                curr = self.melf.elf_nextscn(curr)
                self._populate_program_load_header(index, first.elf32_getshdr(),
                                                   curr.elf32_getshdr())
                first = curr
                index += 1
                continue

            if (curr_shdr.contents.sh_type == pylibelf.elf.SHT_DYNAMIC):
                assert (self.strtab.get(curr_name) == ".dynamic"), \
                    f"Incorrect ordering of section {self.strtab.get(curr_name)}"
                curr = self.melf.elf_nextscn(curr)
                self._populate_program_load_header(index, first.elf32_getshdr(), curr.elf32_getshdr())
                first = curr
                continue

            curr = self.melf.elf_nextscn(curr)
            first = curr

        self.melf.elf_flagphdr(pylibelf.libelf.Elf_Cmd.ELF_C_SET, pylibelf.libelf.ELF_F_DIRTY)
        self.melf.elf_update(pylibelf.libelf.Elf_Cmd.ELF_C_WRITE)
        del self.melf

class AIE2_ELFWriter(ELFWriter):
    AMD_AIE2P_IPU = 0x45
    AMD_AIE2P_IPU_V1 = 1

    def __init__(self, filename, symbols, elf_sections, section_index_callback):
        super().__init__(AIE2_ELFWriter.AMD_AIE2P_IPU, AIE2_ELFWriter.AMD_AIE2P_IPU_V1, filename, symbols, elf_sections, section_index_callback)

class AIE2PS_ELFWriter(ELFWriter):
    AMD_AIE_CERT = 0x40
    AMD_AIE_CERT_V1 = 1

    def __init__(self, filename, symbols, elf_sections, section_index_callback):
        super().__init__(AIE2PS_ELFWriter.AMD_AIE_CERT, AIE2PS_ELFWriter.AMD_AIE_CERT_V1, filename, symbols, elf_sections, section_index_callback)

class AIE4_ELFWriter(ELFWriter):
    AMD_AIE_CERT = 0x40
    AMD_AIE_CERT_V1 = 1

    def __init__(self, filename, symbols, elf_sections, section_index_callback):
        super().__init__(AIE4_ELFWriter.AMD_AIE_CERT, AIE4_ELFWriter.AMD_AIE_CERT_V1, filename, symbols, elf_sections, section_index_callback)
