#! /usr/bin/env python3

# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2022-2023 Advanced Micro Devices, Inc.

import sys
import argparse
import os.path
import json
from typing import List

from enum import IntEnum

class Symbol:
    """
    Represents a user symbol (a variable reference) encountered in assembly code.
    """

    class XrtPatchBufferType(IntEnum):
        xrt_patch_buffer_type_instruct = 0
        xrt_patch_buffer_type_control_packet = 1
        xrt_patch_buffer_type_unkown = 2

    class XrtPatchSchema(IntEnum):
        xrt_patch_schema_uc_dma_remote_ptr_symbol = 1
        xrt_patch_schema_shim_dma_57 = 2
        xrt_patch_schema_scaler_32 = 3
        xrt_patch_schema_control_packet_48 = 4
        xrt_patch_schema_shim_dma_48 = 5
        xrt_patch_schema_unknown = 6

    def __init__(self, name, buf_type, pos, schema=XrtPatchSchema.xrt_patch_schema_unknown):
        self.name = name
        self.buf_type = buf_type
        self.offsets = [pos]
        self.schema = schema

    def addoffset(self, offset):
        self.offsets.append(offset)

class IpuCodeDecoder:
    DMA_BD0_2 = 0x0001D008
    DMA_BD_NUM = 16
    DMA_BD_SIZE = 0x20 # 8*4bytes

    IFM_TYPE = 0x0
    PARAM_TYPE = 0x1
    OFM_TYPE = 0x2
    INTER_TYPE = 0x3
    OUT2_TYPE  = 0x4
    MC_CODE_TYPE = 0x5

    ARGTYPE_TO_STRING = {
        IFM_TYPE: "ifm",
        PARAM_TYPE: "param",
        OFM_TYPE: "ofm",
        INTER_TYPE: "inter",
        OUT2_TYPE: "out2",
        MC_CODE_TYPE: "mc_code"
    }

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
    def __init__(self, symbols):
        # list all shim tile BD registers DDR address need to be processed
        self.DMABDx2RegAddr = [IpuCodeDecoder.DMA_BD0_2 + IpuCodeDecoder.DMA_BD_SIZE *i for i in range(IpuCodeDecoder.DMA_BD_NUM)]
        self.symbols = symbols
        self.symbolmap = {}

    def decode_control_packet(self, data, pad_control_packet):
        mc_code_ddr = [int.from_bytes([data[x], data[x+1], data[x+2], data[x+3]] , byteorder='little', signed = False)
                       for x in range(0, len(data), 4)]
        mc_code_ddr_size_bytes = len(mc_code_ddr)
        pc = 0
        dataSize = 0
        localByteAddress = 0
        while pc < mc_code_ddr_size_bytes:
            # read packet header and control packet, parse the data size and BD register addr
            pc += 2
            dataSize = ((mc_code_ddr[pc - 1] >> 20) & 0x3)
            localByteAddress = (mc_code_ddr[pc - 1] & 0xfffff)

            # patch shim tile register DMA_BDx DDR address
            self.patchddrAddress(mc_code_ddr, pc, dataSize, localByteAddress, pad_control_packet)
            pc += (dataSize + 1)

            # control packets aligned to 256 bits
            if pad_control_packet:
                pc += (8 - (pc % 8)) % 8

    def patchddrAddress(self, BDData, pc,length, addr, pad_control_packet):
        # check if shim tile BD register contains DDR address.
        # This is to support variable number of DMA_BDx register configurations, but this function needs to be checked.
        # Now we write register from DMA_BDx_0 to DMA_BDx_7 every time, for more efficiency, we may only write part of eight DMA_BDx later.
        # One thing to note is that we cannot only write the Base_Address_High of DMA_BDx_2, which also means that the address of DMA_BDx_2
        # cannot be in the Local Byte Address of control packet(CP). So we start traversing from addr plus 4.
        # Taking DMA_BD0 as an examle, now we fully configure from 0x1D000 to 0x1D01C, later we may only config five registers,
        # say from 0x1D00C to 0x1D01C. the position of Base_Address_High in BD data is variable, and may even not exist.
        # so We need to check if the shim tile DMA_BDx register contains the DDR address.
        for i in range(1, length+1):
            addr += 4
            if addr in self.DMABDx2RegAddr:
                regID = ((BDData[pc+i] >> 12) & 0xf)
                assert(regID in IpuCodeDecoder.ARGTYPE_TO_STRING.keys()), f"Invalid arg type {regID} found"
                # cpntrol packet pos(pc-1) multiply by 4 is because of 32bit to 8bit
                if regID in self.symbolmap.keys():
                    self.symbolmap[regID].addoffset((pc-1)*4)
                else:
                    self.symbolmap[regID] = Symbol(IpuCodeDecoder.ARGTYPE_TO_STRING[regID], pad_control_packet, (pc-1)*4,
                                               Symbol.XrtPatchSchema.xrt_patch_schema_control_packet_48)
                    self.symbols.append(self.symbolmap[regID])
                #self.symbols.append(Symbol(IpuCodeDecoder.ARGTYPE_TO_STRING[regID], pad_control_packet, (pc-1)*4,
                #                           Symbol.XrtPatchSchema.xrt_patch_schema_control_packet_48))

    def patch_shimbd(self, ins_buffer, ins_buffer_size_bytes, pc):
        regID = (ins_buffer[pc] & 0x000000F0) >> 4;
        if (regID != IpuCodeDecoder.IFM_TYPE and
            regID != IpuCodeDecoder.PARAM_TYPE and
            regID != IpuCodeDecoder.OFM_TYPE and
            regID != IpuCodeDecoder.INTER_TYPE and
            regID != IpuCodeDecoder.OUT2_TYPE and
            regID != IpuCodeDecoder.MC_CODE_TYPE):
            return
        #TODO: if both mc_code and instruction buffer have same key
        if regID in self.symbolmap.keys():
            self.symbolmap[regID].addoffset((pc+1)*4)
        else:
            self.symbolmap[regID] = Symbol(IpuCodeDecoder.ARGTYPE_TO_STRING[regID], Symbol.XrtPatchBufferType.xrt_patch_buffer_type_instruct, (pc+1)*4,
                                           Symbol.XrtPatchSchema.xrt_patch_schema_shim_dma_48)
            self.symbols.append(self.symbolmap[regID])

    def decode_instruction_buffer(self, data):
        ins_buffer = [int.from_bytes([data[x], data[x+1], data[x+2], data[x+3]] , byteorder='little', signed = False)
                       for x in range(0, len(data), 4)]
        ins_buffer_size_bytes = len(ins_buffer)
        pc = 0
        while pc < ins_buffer_size_bytes:
            op = (ins_buffer[pc] & 0xFF000000) >> 24
            if op == IpuCodeDecoder.OP_WRITESHIMBD:
                self.patch_shimbd(ins_buffer, ins_buffer_size_bytes, pc)
                pc = pc + 9
            elif op == IpuCodeDecoder.OP_SYNC:
                #TODO: not sure if we are using __USE_SYNC_POLL_API__
                #for now considering __USE_SYNC_POLL_API__ is not enabled
                pc = pc + 2
            elif op == IpuCodeDecoder.OP_WRITEBD:
                row = (ins_buffer[pc] & 0x0000FF00) >> 8
                if row == 0:
                    self.patch_shimbd(ins_buffer, ins_buffer_size_bytes, pc)
                    pc = pc + 9
                elif row == 1:
                    pc = pc + 9
                else:
                    pc = pc + 7
            elif op in IpuCodeDecoder.OPERATION_MAP.keys():
                 pc = pc + IpuCodeDecoder.OPERATION_MAP[op]
            else:
                raise RuntimeError(f"Unknown operation found: {op}")

def parse_command_line(args):
    """ Command line parser """
    msg = "Generate patch infor for instruction buffer and control buffer"
    parser = argparse.ArgumentParser(description = msg)

    parser.add_argument("-o", "--output", dest ='patch_info', nargs = 1, required=True,
                        help = "Patch info output json file name")
    parser.add_argument("-c", "--control_code_file", dest ='ccfile', nargs = 1, required=False,
                        help = "control code file")
    parser.add_argument("-i", "--instruction_file", dest ='ifile', nargs = 1, required=False,
                        help = "instruction buffer file")

    # strip out the argv[0]
    return parser.parse_args(args[1:])

if __name__ == '__main__':
    argtab = parse_command_line(sys.argv)

    symbols = [Symbol("", 0, 0, 0)]
    ipudecoder = IpuCodeDecoder(symbols)
    if argtab.ifile:
        with open(argtab.ifile[0], 'rb') as f:
            idata = f.read()
            ipudecoder.decode_instruction_buffer(idata)

    if argtab.ccfile:
        with open(argtab.ccfile[0], 'rb') as f:
            ccdata = f.read()
            ipudecoder.decode_control_packet(ccdata, Symbol.XrtPatchBufferType.xrt_patch_buffer_type_control_packet)

    with open(argtab.patch_info[0], "w") as outfile:
        json.dump([ob.__dict__ for ob in symbols], outfile)
