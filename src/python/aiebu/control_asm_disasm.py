#! /usr/bin/env python3

# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2022-2023 Advanced Micro Devices, Inc.

import sys
import argparse
import os.path
import importlib.resources

from ctrlcode.assembler.assembler import Assembler
from ctrlcode.disassembler.disassembler import ELFDisassembler
from ctrlcode.ops.isa import ISA

def parse_command_line(args):
  """ Command line parser for assembler """
  msg = "Assemble ctrlcode ASM file and write hex and or ELF"
  parser = argparse.ArgumentParser(description = msg)

  parser.add_argument('-d','--disassembler', default=False, dest='disassembler', action='store_true',  help='DisAssembler')

  parser.add_argument("-m", "--map", dest ='mapfilename', nargs = '?', default = 'debug_map.json',
                      help = "json output map file name")
  parser.add_argument("-o", "--output", dest ='efilename', nargs = 1, required=True,
                      help = "ELF/ASM output file name")
  parser.add_argument("-I", "--include", dest ='includedir', nargs = '*',
                      help = "Include directories")
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

  yamlres = importlib.resources.files("specification.aie2ps").joinpath("isa-spec.yaml")
  yamlfile = str(yamlres)

  isa = ISA(yamlfile)
  describe_platform(isa)

  if not argtab.disassembler:
    operation = Assembler(argtab.ifilename[0], argtab.efilename[0], isa.UC_ISA_OPS, includedir,
                          argtab.mapfilename)
    operation.run()
  else:
    with open(argtab.efilename[0], 'w') as efile:
      operation = ELFDisassembler(argtab.ifilename[0], efile, isa.UC_ISA_OPS_REVERSE)
      operation.run()

  sys.exit(0)
