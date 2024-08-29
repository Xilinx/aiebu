# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc.

from ctrlcode.common.writer import AIE2_ELFWriter
from ctrlcode.common.section import Section
from ctrlcode.common.symbol import Symbol
from ctrlcode.common.elf_section import ELF_Section

def section_index_callback(buf_type, elf_sections):
  if buf_type == Symbol.XrtPatchBufferType.xrt_patch_buffer_type_instruct:
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
        self.symbols = [Symbol("",0,0,0,0,0)]
        self.extract_patching_info(patch_info)
        self.elf_sections = {}
        self.ewriter = AIE2_ELFWriter(elffile, self.symbols, self.elf_sections, section_index_callback)

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

    # extract patching info from meta.json
    def extract_patching_info(self, patch_info):
        if patch_info:
          for sym in patch_info["symbols"]:
            for data in sym["patching"]:
              self.symbols.append(Symbol(data["name"], data["offsets"], 0, 0, data["schema"], data["buf_type"]))

class Assembler_blob_transaction(Assembler_blob):

    XAIE_IO_WRITE = 0
    XAIE_IO_BLOCKWRITE = 1
    XAIE_IO_BLOCKSET = 2
    XAIE_IO_MASKWRITE = 3
    XAIE_IO_MASKPOLL = 4
    XAIE_IO_CUSTOM_OP_BEGIN = 0x80
    XAIE_IO_CUSTOM_OP_BEGIN_1 = 0x81
    XAIE_IO_CUSTOM_OP_BEGIN_2 = 0x82
    XAIE_IO_CUSTOM_OP_BEGIN_3 = 0x83

    OPERATION_size_index_MAP = {
        XAIE_IO_WRITE: 20,
        XAIE_IO_BLOCKWRITE: 12,
        XAIE_IO_BLOCKSET: 12,
        XAIE_IO_MASKWRITE: 24,
        XAIE_IO_MASKPOLL: 24,
        XAIE_IO_CUSTOM_OP_BEGIN: 4,
        XAIE_IO_CUSTOM_OP_BEGIN_1: 4,
        XAIE_IO_CUSTOM_OP_BEGIN_2: 4,
        XAIE_IO_CUSTOM_OP_BEGIN_3: 4,
    }

    def __init__(self, ifile, ccfile, patch_info, elffile):
        super().__init__(ifile, ccfile, patch_info, elffile);

    def extract_tansaction_buffer_symbol(self):
        ins_buffer_size_bytes = len(self.idata)
        pc = 24  # transaction_op header and XAie_TxnHeader
        blockWriteRegOffsetMap = {}
        while pc < ins_buffer_size_bytes:
            op = self.idata[pc]
            sb = Assembler_blob_transaction.OPERATION_size_index_MAP[op]
            size = int.from_bytes([self.idata[pc+sb], self.idata[pc+sb+1], self.idata[pc+sb+2],
                                  self.idata[pc+sb+3]] , byteorder='little', signed = False)
            print("op:" , op)
            if op == Assembler_blob_transaction.XAIE_IO_BLOCKWRITE:
                reg = int.from_bytes([self.idata[pc+8], self.idata[pc+8+1], self.idata[pc+8+2],
                                     self.idata[pc+8+3]] , byteorder='little', signed = False) # bw_header->RegOff is at index 8
                blockWriteRegOffsetMap[reg] = pc + 16; # size of XAie_BlockWrite32Hdr (16)
                print("XAIE_IO_BLOCKWRITE:", hex(reg))

            if (op == Assembler_blob_transaction.XAIE_IO_CUSTOM_OP_BEGIN_1):
                argidx = int.from_bytes([self.idata[pc+32], self.idata[pc+32+1], self.idata[pc+32+2],
                                        self.idata[pc+32+3], self.idata[pc+32+4], self.idata[pc+32+5],
                                        self.idata[pc+32+6], self.idata[pc+32+7]] , byteorder='little', signed = False)
                reg = int.from_bytes([self.idata[pc+24], self.idata[pc+24+1], self.idata[pc+24+2],
                                     self.idata[pc+24+3], self.idata[pc+24+4], self.idata[pc+24+5],
                                     self.idata[pc+24+6], self.idata[pc+24+7]] , byteorder='little', signed = False)
                reg = reg - 4 # regaddr point to 2nd word of BD
                print("XAIE_IO_CUSTOM_OP_BEGIN_1:", hex(reg))
                if reg in blockWriteRegOffsetMap:
                   self.symbols.append(Symbol(str(argidx), blockWriteRegOffsetMap[reg], 0, 0,
                                              Symbol.XrtPatchSchema.xrt_patch_schema_shim_dma_48,
                                              Symbol.XrtPatchBufferType.xrt_patch_buffer_type_instruct))
                else:
                    print(f"address {hex(reg)} have no block write opcode !!! removing all patching info")
                    self.symbols.clear()
                    return
            pc = pc + size

    def run(self):
        self.extract_tansaction_buffer_symbol();
        super().run()


class Assembler_blob_dpu(Assembler_blob):

    IFM_TYPE = 0x0
    PARAM_TYPE = 0x1
    OFM_TYPE = 0x2
    INTER_TYPE = 0x3
    OUT2_TYPE  = 0x4
    MC_CODE_TYPE = 0x5

    OP_NOOP = 0
    OP_WRITEBD = 1
    OP_WRITE32 = 2
    OP_SYNC = 3
    OP_WRITEBD_EXTEND_AIETILE = 4
    OP_WRITE32_EXTEND_GENERAL = 5
    OP_WRITEBD_EXTEND_SHIMTILE = 6
    OP_WRITEBD_EXTEND_MEMTILE = 7
    OP_WRITE32_EXTEND_DIFFBD = 8
    OP_WRITEBD_EXTEND_SAMEBD_MEMTILE = 9
    OP_DUMPDDR = 10
    OP_WRITESHIMBD = 11
    OP_WRITEMEMBD = 12
    OP_WRITE32_RTP = 13
    OP_READ32 = 14
    OP_READ32_POLL = 15
    OP_RECORD_TIMESTAMP = 16
    OP_MERGESYNC = 17
    OP_DUMP_REGISTER = 18

    # Map of fixed length packet
    # OP_WRITEBD, OP_SYNC not included in this
    OPERATION_MAP = {
        OP_NOOP: 1,
        OP_WRITE32: 3,
        OP_WRITEBD_EXTEND_AIETILE: 8,
        OP_WRITE32_EXTEND_GENERAL: 3,
        OP_WRITEBD_EXTEND_SHIMTILE: 10,
        OP_WRITEBD_EXTEND_MEMTILE: 11,
        OP_WRITE32_EXTEND_DIFFBD: 4,
        OP_WRITEBD_EXTEND_SAMEBD_MEMTILE: 9,
        OP_DUMPDDR: 44,
        OP_WRITESHIMBD: 9,
        OP_WRITEMEMBD: 9,
        OP_WRITE32_RTP: 3,
        OP_READ32: 2,
        OP_READ32_POLL: 4
    }

    ARGTYPE_TO_STRING = {
        IFM_TYPE: "ifm",
        PARAM_TYPE: "param",
        OFM_TYPE: "ofm",
        INTER_TYPE: "inter",
        OUT2_TYPE: "out2",
        MC_CODE_TYPE: "control-packet"
    }

    def __init__(self, ifile, ccfile, patch_info, elffile):
        super().__init__(ifile, ccfile, patch_info, elffile);

    def patch_shimbd(self, ins_buffer, pc):
        regID = (ins_buffer[pc] & 0x000000F0) >> 4;
        if (regID != Assembler_blob_dpu.IFM_TYPE and
            regID != Assembler_blob_dpu.PARAM_TYPE and
            regID != Assembler_blob_dpu.OFM_TYPE and
            regID != Assembler_blob_dpu.INTER_TYPE and
            regID != Assembler_blob_dpu.OUT2_TYPE and
            regID != Assembler_blob_dpu.MC_CODE_TYPE):
            return
        #TODO: if both mc_code and instruction buffer have same key
        self.symbols.append(Symbol(Assembler_blob_dpu.ARGTYPE_TO_STRING[regID], (pc+1)*4, 0, 0, Symbol.XrtPatchSchema.xrt_patch_schema_shim_dma_48,
                                   Symbol.XrtPatchBufferType.xrt_patch_buffer_type_instruct))

    def extract_instruction_buffer_symbol(self):
        ins_buffer = [int.from_bytes([self.idata[x], self.idata[x+1], self.idata[x+2], self.idata[x+3]] , byteorder='little', signed = False)
                       for x in range(0, len(self.idata), 4)]
        ins_buffer_size_bytes = len(ins_buffer)
        pc = 0
        while pc < ins_buffer_size_bytes:
            op = (ins_buffer[pc] & 0xFF000000) >> 24
            if op == Assembler_blob_dpu.OP_WRITESHIMBD:
                self.patch_shimbd(ins_buffer, pc)
                pc = pc + 9
            elif op == Assembler_blob_dpu.OP_SYNC:
                #TODO: not sure if we are using __USE_SYNC_POLL_API__
                #for now considering __USE_SYNC_POLL_API__ is not enabled
                pc = pc + 2
            elif op == Assembler_blob_dpu.OP_WRITEBD:
                row = (ins_buffer[pc] & 0x0000FF00) >> 8
                if row == 0:
                    self.patch_shimbd(ins_buffer, pc)
                    pc = pc + 9
                elif row == 1:
                    pc = pc + 9
                else:
                    pc = pc + 7
            elif op in Assembler_blob_dpu.OPERATION_MAP.keys():
                 pc = pc + Assembler_blob_dpu.OPERATION_MAP[op]
            else:
                raise RuntimeError(f"Unknown operation found: {op}")

    def run(self):
        self.extract_instruction_buffer_symbol();
        super().run()
