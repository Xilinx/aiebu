#! /usr/bin/env python3

# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2024 Advanced Micro Devices, Inc.

import os
import sys
import argparse
import json
from symbol import Symbol
from decoder import Decoder

class IpuTransactionBoDecoder(Decoder):

    def __init__(self, symbols):
        self.ext_buffer_map = {}
        super().__init__(symbols)

    def read_ext_buffers_json(self, extjson):
        keys = ["inputs", "outputs", "weights"];
        ext_buff_port_id = 0
        for key in keys:
            assert (key in extjson), f"{key} not in flexjson"
            if key == "inputs" or key == "outputs":
                 for io in extjson[key]:
                     name = io["name"]
                     ddr_offset = 0
                     if "offset" in io:
                         ddr_offset = io["offset"]
                     self.ext_buffer_map[name] = {"port":ext_buff_port_id, "offset":ddr_offset }
                     ext_buff_port_id = ext_buff_port_id + 1
            elif key == "weights":
                weights = extjson[key]
                layers = weights["layers"]
                for layer in layers:
                    name = layer["name"]
                    ddr_offset = layer["offset"] * 4
                    self.ext_buffer_map[name] = {"port":ext_buff_port_id, "offset":ddr_offset }
                ext_buff_port_id = ext_buff_port_id + 1

    def decode_control_packet_json(self, ccjson, extjson, ctrldata, pad_control_packet_json):
        self.read_ext_buffers_json(extjson)
        for patchid in ccjson["control_packet_patch"]:
            patch = ccjson["control_packet_patch"][patchid]
            split = patch["name"].split(".")
            name = split[len(split)-1]
            self.symbols.append(Symbol(name,
                                          Symbol.XrtPatchBufferType.xrt_patch_buffer_type_control_packet,
                                          patch["offset"], self.ext_buffer_map[patch["name"]]["offset"],
                                          Symbol.XrtPatchSchema.xrt_patch_schema_control_packet_48))

            # taking only 6 bytes as we need only 48bit
            val = int.from_bytes([ctrldata[patch["offset"]], ctrldata[patch["offset"]+1], ctrldata[patch["offset"]+2], ctrldata[patch["offset"]+3], ctrldata[patch["offset"]+4], ctrldata[patch["offset"]+5]], byteorder='little', signed = False)
            val = val + self.ext_buffer_map[patch["name"]]["offset"];
            ctrldata[patch["offset"]] = val & 0xFF;
            ctrldata[patch["offset"]+1] = (val >> 8) & 0xFF;
            ctrldata[patch["offset"]+2] = (val >> 16) & 0xFF;
            ctrldata[patch["offset"]+3] = (val >> 24) & 0xFF;
            ctrldata[patch["offset"]+4] = (val >> 32) & 0xFF;
            ctrldata[patch["offset"]+5] = (val >> 40) & 0xFF;

    def decode_tansaction_buffer(self, ifile, ins_buffer, isheader):
        ins_buffer_size_bytes = len(ins_buffer)
        if isheader:
            offset = 24
            patch_ml_txn = bytearray()
            # add e_TRANSACTION_OP (0)
            patch_ml_txn = patch_ml_txn + bytes([0x00]*4)
            # get XAie_TxnHeader->TxnSize
            size = int.from_bytes([ins_buffer[12], ins_buffer[13], ins_buffer[14], ins_buffer[15]] , byteorder='little', signed = False)
            size = size + 8  # 8 byte header (sizeof(transaction_op_t))
            patch_ml_txn = patch_ml_txn + size.to_bytes(4,'little')
            with open("patched_"+os.path.basename(ifile), "wb") as ofile:
                ofile.write(patch_ml_txn)
                ofile.write(ins_buffer)


def parse_command_line(args):
    """ Command line parser """
    msg = "Generate patch info for transaction buffer and control packet"
    parser = argparse.ArgumentParser(description = msg)

    parser.add_argument("-o", "--output", dest ='patch_info', nargs = 1, required=True,
                        help = "Patch info output json file name")
    parser.add_argument("-c", "--control_packet_json_file", dest ='ccfile', nargs = 1, required=False,
                        help = "control packet json file")
    parser.add_argument('-t',dest='isheader', action='store_true',  help="add header to ml_tnx.bin. \
                                                                          WARNING: ml_txn.bin is not owned by this script,\
                                                                          so should not be modified here.")
    parser.add_argument("-f", "--flexmlrt_hsi_json_file", dest ='extfile', nargs = 1, required=False,
                        help = "flexmlrt hsi json file")
    parser.add_argument("-p", "--ctrl_pkt_file", dest ='ctrlfile', nargs = 1, required=False,
                        help = "ctrl pkt bin file")
    parser.add_argument("-i", "--tansaction_file", dest ='ifile', nargs = 1, required=False,
                        help = "tansaction buffer bin file")

    # strip out the argv[0]
    return parser.parse_args(args[1:])

if __name__ == '__main__':
    argtab = parse_command_line(sys.argv)

    symbols = []
    dist = {}
    ipudecoder = IpuTransactionBoDecoder(symbols)
    if argtab.ifile:
        with open(argtab.ifile[0], 'rb') as f:
            idata = f.read()
            ipudecoder.decode_tansaction_buffer(argtab.ifile[0], idata, argtab.isheader)

    if argtab.ccfile:
        if argtab.extfile and argtab.ctrlfile:
            with open(argtab.extfile[0], 'rb') as f:
                extjson = json.load(f)

            with open(argtab.ctrlfile[0], 'rb') as f:
                ctrldata = bytearray(f.read())

            with open(argtab.ccfile[0], 'rb') as f:
                ccjson = json.load(f)
            ipudecoder.decode_control_packet_json(ccjson, extjson, ctrldata, Symbol.XrtPatchBufferType.xrt_patch_buffer_type_control_packet)
            dist = {"control-packet": { "name": "control-packet-0", "path": argtab.ctrlfile[0] } }
            with open("patched_"+os.path.basename(argtab.ctrlfile[0]), "wb") as ofile:
                ofile.write(ctrldata)
            dist = {"control-packet": { "name": "control-packet-0", "path": argtab.ctrlfile[0] } }

        else:
            sys.stderr.write(f"File {argtab.extfile[0]} or {argtab.ctrlfile[0]} not exist\n")
            sys.exit(0)

    dist["symbols"] = [ {"name":"sym", "patching":[ob.__dict__ for ob in symbols ] } ]
    #print(dist)
    with open(argtab.patch_info[0], "w") as outfile:
        json.dump(dist, outfile, indent=4)
