#! /usr/bin/env python3

# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2022-2023 Advanced Micro Devices, Inc.

import sys
import argparse
import os.path
import json
import importlib.resources

from ctrlcode.assembler.assembler import Assembler
from ctrlcode.disassembler.disassembler import ELFDisassembler
from ctrlcode.assembler.assembler_blob import Assembler_blob_transaction
from ctrlcode.assembler.assembler_blob import Assembler_blob_dpu
from ctrlcode.ops.isa import ISA

def parse_command_line(args):
  """ Command line parser for assembler """
  msg = "Assemble ctrlcode ASM file and write hex and or ELF"
  parser = argparse.ArgumentParser(description = msg)

  parser.add_argument('-t','--target', default='aie2ps', dest='target', help='supported targets aie2ps/aie2asm/aie2txn/aie2dpu')

  parser.add_argument('-d','--disassembler', default=False, dest='disassembler', action='store_true',  help='DisAssembler')

  parser.add_argument("-m", "--map", dest ='mapfilename', nargs = '?', default = 'debug_map.json',
                      help = "json output map file name")
  parser.add_argument("-o", "--output", dest ='efilename', nargs = 1, required=True,
                      help = "ELF/ASM output file name")
  parser.add_argument("-I", "--include", dest ='includedir', nargs = '*',
                      help = "Include directories")
  parser.add_argument("-c", "--control_packet", dest ='ccfile', nargs = 1,
                      help = "control packet file")
  parser.add_argument("-p", "--patch_info_file", dest ='patch_info', nargs = 1,
                      help = "patch info json file")
  parser.add_argument("ifilename", metavar="infile", nargs = 1)

  # strip out the argv[0]
  return parser.parse_args(args[1:])

def describe_platform(isa):
  print("aiebu supported platforms")
  print(f"{isa.get_machine_models()}")

if __name__ == '__main__':
  argtab = parse_command_line(sys.argv)
  if not os.path.isfile(argtab.ifilename[0]):
    sys.stderr.write(f"File {argtab.ifilename[0]} not exist\n")
    sys.exit(0)

  includedir = []
  if argtab.includedir:
    includedir = includedir + argtab.includedir

  specdir = "specification.aie2ps"
  if argtab.target == "aie2asm":
    specdir = "specification.aie2"
  elif argtab.target == "aie2txn" or argtab.target == "aie2dpu" :
    specdir = ""

  if argtab.target == "aie2txn" or argtab.target == "aie2dpu" :
    if argtab.disassembler:
      raise RuntimeError(f"Disassembler not supported with {argtab.target}")

    patch_info = {}
    if argtab.patch_info:
      with open(argtab.patch_info[0]) as f:
        patch_info = json.load(f)

    ccfile = None
    if argtab.ccfile:
      ccfile = argtab.ccfile[0]

    if argtab.target == "aie2dpu":
        operation = Assembler_blob_dpu(argtab.ifilename[0], ccfile, patch_info, argtab.efilename[0])
    else:
        operation = Assembler_blob_transaction(argtab.ifilename[0], ccfile, patch_info, argtab.efilename[0])
    operation.run()
    sys.exit(0)
  else:
    if argtab.ccfile:
      raise RuntimeError(f"Invalid option -c with target {argtab.target}")

    if argtab.patch_info:
      raise RuntimeError(f"Invalid option -p with target {argtab.target}")

  yamlres = importlib.resources.files(specdir).joinpath("isa-spec.yaml")
  yamlfile = str(yamlres)

  isa = ISA(yamlfile)
  describe_platform(isa)

  if not argtab.disassembler:
    operation = Assembler(argtab.target, argtab.ifilename[0], argtab.efilename[0], isa.UC_ISA_OPS, includedir,
                          argtab.mapfilename)
    operation.run()
  else:
    with open(argtab.efilename[0], 'w') as efile:
      operation = ELFDisassembler(argtab.ifilename[0], efile, isa.UC_ISA_OPS_REVERSE)
      operation.run()

  sys.exit(0)
