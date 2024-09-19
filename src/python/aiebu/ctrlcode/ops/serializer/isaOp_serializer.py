# SPDX-License-Identifier: MIT
# Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

from ctrlcode.common.util import parse_num_arg
from ctrlcode.common.util import parse_register
from ctrlcode.common.util import parse_barrier
from ctrlcode.common.section import Section
from ctrlcode.common.op_arg import OpArg
from ctrlcode.ops.serializer.op_serializer import OpSerializer
from ctrlcode.common.symbol import Symbol

class IsaOpSerializer(OpSerializer):
    def __init__(self, op, args, state):
        super().__init__(op, args, state)

    def size(self):
        result = int((16 + sum([arg.width for arg in self.op.args])) / 8)
        return result

    def align(self):
        return 0

    def serialize(self, text_section, data_section, col, page, symbols):
        result = [
            self.op.opcode,
            0
        ]
        arg_index = 0
        for arg in self.op.args:
            if arg.argtype == OpArg.PAD:
                val = 0
                argtype = OpArg.CONST
            elif arg.argtype == OpArg.JOBSIZE:
                job_id = parse_num_arg(self.args[0], self.state)
                val = self.state.getjobsize(job_id)
                argtype = OpArg.CONST
            elif arg.argtype == OpArg.PAGE_ID:
                val = self.state.getlabelpageindex(self.args[arg_index][1:])
                argtype = OpArg.CONST
                arg_index += 1
            else:
                val = self.args[arg_index]
                argtype = arg.argtype
                arg_index += 1

            if argtype == OpArg.REG:
                result.append(parse_register(val))
            elif argtype == OpArg.BARRIER:
                result.append(parse_barrier(val))
            elif argtype == OpArg.CONST:
                val = parse_num_arg(val, self.state)
                if isinstance(val, Symbol):
                    assert arg.width == 32, f"Symbol:{val} for width {arg.width} not supported"
                    val.setcol(col)
                    # We need to subtract 16 bytes control code header
                    val.setoffset(text_section.tell() + len(result) - 16)
                    val.setschema(Symbol.XrtPatchSchema.xrt_patch_schema_scaler_32)
                    val.setpagenum(page)
                    symbols.append(val)
                    val = 0

                if arg.width == 8:
                    if val == -1:
                        val = 0
                    result.append(val)
                elif arg.width == 16:
                    if val == -1:
                        val = 0
                    # For opcode is 'apply_offset_57' and arg is 'offset',
                    # if val is 0xFFFF means we need to patch the host address of 1st page of controlcode
                    # and that is always patched in cert,
                    # if val is not 0xFFFF, we can do patching in cert or host so add symbol info in elf
                    if self.op.name == "apply_offset_57" and arg.name == "offset" and val != 0xFFFF:
                        symbols.append(Symbol(str(val), parse_num_arg(self.args[0], self.state), col, page,
                                              Symbol.XrtPatchSchema.xrt_patch_schema_shim_dma_57 if self.state.target == "aie2ps" else
                                              Symbol.XrtPatchSchema.xrt_patch_schema_shim_dma_57_aie4))
                        # arg 0 to 6 and be patched in CERT.
                        # Beyond that its elfloader/host responsibility to patch mandatorily
                        if val > 6:
                            print(f"WARNING: Apply_offset_57 has arg index {val} > 6, Should be mandatorily patched in host!!!")
                        # val is arg index, to get offset x2
                        val = val * 2
                    result.append(val & 0xFF)
                    result.append((val >> 8) & 0xFF)
                elif arg.width == 32:
                    if val == -1:
                        val = 0
                    result.append(val & 0xFF)
                    result.append((val >> 8) & 0xFF)
                    result.append((val >> 16) & 0xFF)
                    result.append((val >> 24) & 0xFF)
                else:
                    assert False, "Unsupported arg width!"
            else:
                assert False, "Invalid arg type!"

        assert (self.state.section == Section.TEXT), "Instructions can only be used in TEXT section"
        for b in result:
            text_section.write_byte(b)
