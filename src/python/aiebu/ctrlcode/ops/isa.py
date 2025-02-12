# SPDX-License-Identifier: MIT
# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.

import yaml
from ctrlcode.common.op_arg import OpArg
from ctrlcode.ops.isaOp import IsaOp
from ctrlcode.ops.wordOp import WordOp
from ctrlcode.ops.alignOp import AlignOp
from ctrlcode.ops.ucDmaOp import UcDmaOp

class ISA:
    def __init__(self, yfile):
        self.yaml_file = yfile
        self.UC_ISA_OPS = {}
        self.UC_ISA_OPS_REVERSE = {}
        self.PLATFORM = {}
        self.populate()

    def get_machine_models(self):
        systems = []
        for pconfig in self.PLATFORM['system']:
            systems.append(f"{self.PLATFORM['architecture']}.{pconfig}")
        return systems

    def get_arg_width(self, arg):
        if arg['type'] == 'register' or arg['type'] == 'barrier':
            return 8
        else:
            return arg['width']

    def postprocess_spec(self, spec):
        for op in spec['operations']:
            offset = 16
            for arg in op['arguments']:
                arg['_offset'] = offset // 8
                offset += self.get_arg_width(arg)

    def populate(self):
        spec = None
        with open(self.yaml_file, 'r') as f:
            spec = yaml.load(f, Loader=yaml.Loader)
        self.postprocess_spec(spec)
        self.PLATFORM = spec['platform']

        for operation in spec['operations']:
            opargs = []
            for arg in operation['arguments']:
                atype = OpArg.fromString(arg['type'].lower())
                if arg['type'] != 'pad':
                    aname = arg['name']
                else:
                    aname = "_pad"
                opargs.append(OpArg(aname, atype, self.get_arg_width(arg)))
            self.UC_ISA_OPS[operation['mnemonic'].lower()] = IsaOp(operation['opcode'], operation['mnemonic'].lower(), opargs)
            self.UC_ISA_OPS_REVERSE[operation['opcode']] = IsaOp(operation['opcode'], operation['mnemonic'].lower(), opargs)

            self.UC_ISA_OPS['.long'] = WordOp()
            self.UC_ISA_OPS['.align'] = AlignOp()
            self.UC_ISA_OPS['uc_dma_bd'] = UcDmaOp()

            self.UC_ISA_OPS_REVERSE['.long'] = WordOp()
            self.UC_ISA_OPS_REVERSE[165] = AlignOp()
            self.UC_ISA_OPS_REVERSE['uc_dma_bd'] = UcDmaOp()

    def __str__(self):
        mkeys = self.UC_ISA_OPS.keys()
        return str(mkeys)

    def __len__(self):
        return len(self.UC_ISA_OPS)
