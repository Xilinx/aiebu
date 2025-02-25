# Introduction
This document outlines the specification of the RyzenAI AIE2 _ctrlcode_ ISA which serves as the instruction set
interpreted by the *MPIPU* ERT thread.

{% dot
digraph cert_stack {
    fontname="Courier New"
	subgraph cluster_0 {
	node [shape="rectangle" fontname="Courier New"];
	SRAM
	rankdir=TB;
	node [shape="tab" style="dashed" fontname="Courier New"];
	CtrlCode;
	node [shape="record" style="solid" fontname="Courier New"];
	label="ERT";
	labelloc="t";
	structcert0 [ label = "{ERT Interpreter|Baremetal OS|uController}"; ];
	structcert0 -> CtrlCode;
	CtrlCode -> SRAM
	color=blue;
	}
    label="Application Context";
	labelloc="b";

}

%}

---
# Control-Code Sequence
The control-code is a sequence of opcodes along with operands, which are transparently paged into SRAM
by the RTOS and then executed by the interpreter running in ERT context.

The control-code opcodes have variable sized payloads.

e.g. *OP_WRITE32* is itself 4 bytes wide: 1 byte of opcode, 1 byte of column number, 1 byte of row number,
and 1 byte of padding, but is followed by 4 bytes of Register Address and 4 bytes of Register Value
which makes the *OP_WRITE32* opcode a total of 12 bytes.

|Start Bit | 31         | 23     | 15  | 7   |
|----------|------------|--------|-----|-----|
|W0        | Opcode (2) | Column | Row | Pad |
|W1        | RegAddr    |        |     |     |
|W2        | RegVal     |        |     |     |

---
# Alignment
All operands have to be naturally aligned, i.e. 16-bit numbers may only start on 16-bit boundaries and
32-bit numbers may only start on 32-bit boundaries.

The size of an operation, i.e. the opcode followed by all operands, should be a multiple of 32 bits.

To achieve the required alignment, padding operands can be used.

---
# Endianness
All numbers are encoded in little-endian order.

---
# Control-Code Pages
Control-code is organized as a series of pages which are transparently paged-in to the SRAM memory in
a ping-pong fashion.

---
# Binary Format
Control-code asm files are assembled into standard 32-bit ELF binary files. The file is organized into [sections](#directives).


{% dot
digraph cert_stack {
    fontname="Courier New"
	rankdir=TB;
	node [shape="record" fontname="Courier New"];
	structcert0 [ label = "{ELF Header|.text_instr|.data_ctrl|.symtab}"; ];
    label="Sections in RyzenAI ctrlcode";
	labelloc="b";

}

%}

---
# Data Types

## opcode
8-bit unsigned integer containing a unique identifier for the operation.


## const
Constant number. The width is specified per-operand and should be 8, 16 or 32 bit.


## pad
Used for padding / alignment purposes. The value is always set to zero.
In human-readable representations (asm file), padding operands are omitted from the operand list.



---
# Operations

## OP_NOOP (0x00)

None

| 0x00 |  - | instruction size |
| :-: |  - | -: |
| opcode (8b) | pad (24b) | 4B |

Do nothing but return imediately.


## OP_WRITEBD (0x01)

write BdContent to tile(Col, Row) BdId  BD registers

| 0x01 |  column | row | ddr_type | bd_id | instruction size |
| :-: |  - | - | - | - | -: |
| opcode (8b) | const (8b) | const (8b) | const (3b) | const (5b) | 4B |

Write 8 DWORDs to a single shim/memtile BD. Patch BD address if not yet.


## OP_WRITE32 (0x02)

write 32bits RegVal to tile(Col, Row):RegAddr

| 0x02 |  column | row | - | instruction size |
| :-: |  - | - | - | -: |
| opcode (8b) | const (8b) | const (8b) | pad (8b) | 4B |

Write 32-bits data to a single register.


## OP_SYNC (0x03)

None

| 0x03 |  column | row | dma_direction | instruction size |
| :-: |  - | - | - | -: |
| opcode (8b) | const (8b) | const (8b) | const (8b) | 4B |

Wait until the number of BDs in the same channel of all tiles equal to 0.


## OP_WRITEBD_EXTEND_AIETILE (0x04)

write BdContent to tile(Col:+ColNum, Row:+RowNum) BdId  BD registers

| 0x04 |  column | row | bd_id | instruction size |
| :-: |  - | - | - | -: |
| opcode (8b) | const (8b) | const (8b) | const (8b) | 4B |

Same as OP_WRITEBD,  write same content to multi-tiles.


## OP_WRITE32_EXTEND_GENERAL (0x05)

write 32bits RegVal to tile(Col:+ColNum, Row:+RowNum):RegAddr

| 0x05 |  column | row | col_num | row_num | instruction size |
| :-: |  - | - | - | - | -: |
| opcode (8b) | const (8b) | const (8b) | const (3b) | const (5b) | 4B |

Same ad OP_WRITE32, write same 32-bits to multi-tiles.


## OP_WRITEBD_EXTEND_SHIMTILE (0x06)

write BdContent to tile(Col:+ColNum, 0) BdId  BD registers

| 0x06 |  column | row | ddr_type | bd_id | instruction size |
| :-: |  - | - | - | - | -: |
| opcode (8b) | const (8b) | const (8b) | const (3b) | const (5b) | 4B |

Same as OP_WRITEBD,  write different content to same BdId of tiles at row-0,  each tile BD has different address field.


## OP_WRITEBD_EXTEND_MEMTILE (0x07)

write BdContent to tile(Col:+ColNum, Row) BdId  BD registers, Each col has different BdId and NextBdId

| 0x07 |  column | row | col_num | instruction size |
| :-: |  - | - | - | -: |
| opcode (8b) | const (8b) | const (8b) | const (8b) | 4B |

Same as OP_WRITEBD,  write different content to different BdId of tiles at one row,  each tile BD has different NextBd field.


## OP_WRITE32_EXTEND_DIFFBD (0x08)

write 32bits RegVal to tile(Col:+ColNum, Row:+RowNum):RegAddr

| 0x08 |  column | row | col_num | row_num | instruction size |
| :-: |  - | - | - | - | -: |
| opcode (8b) | const (8b) | const (8b) | const (3b) | const (5b) | 4B |

same as OP_WRITE32_EXTEND_GENERAL,  each tile has different register value..


## OP_WRITEBD_EXTEND_SAMEBD_MEMTILE (0x09)

write BdContent to tile(Col:+ColNum) BdId. Each col has same BD content

| 0x09 |  column | row | bd_id | instruction size |
| :-: |  - | - | - | -: |
| opcode (8b) | const (8b) | const (8b) | const (8b) | 4B |

same as OP_WRITEBD, write same content to same BDId to different columns.


## OP_DUMPDDR (0x0a)

Simulation only

| 0x0a |  - | instruction size |
| :-: |  - | -: |
| opcode (8b) | pad (24b) | 4B |

Not used on AIE IPU.


## OP_WRITESHIMBD (0x0b)

write BdContent to tile(Col, Row) BdId  to shim BD registers

| 0x0b |  column | row | ddr_type | bd_id | instruction size |
| :-: |  - | - | - | - | -: |
| opcode (8b) | const (8b) | const (8b) | const (3b) | const (5b) | 4B |

Write 8 DWORDs to a single shim BD. Patch BD address if not yet.


## OP_WRITEMEMBD (0x0c)

write BdContent to tile(Col, Row) BdId  to memtile BD registers

| 0x0c |  column | row | bd_id | instruction size |
| :-: |  - | - | - | -: |
| opcode (8b) | const (8b) | const (8b) | const (8b) | 4B |

Write 8 DWORDs to a single memtile BD..


## OP_WRITE32_RTP (0x0d)

write content in RTPOffset at instruction buffer to RTPAddr

| 0x0d |  column | row | - | instruction size |
| :-: |  - | - | - | -: |
| opcode (8b) | const (8b) | const (8b) | pad (8b) | 4B |

Write 32-bits data at given offset in instruction buffer to a single register.


## OP_READ32 (0x0e)

read RegAddr to MPIPU debug register

| 0x0e |  column | row | - | instruction size |
| :-: |  - | - | - | -: |
| opcode (8b) | const (8b) | const (8b) | pad (8b) | 4B |

Under debug mode, read a given register value and write to debug register.


## OP_READ32_POLL (0x0f)

read value at RegAddr and compare to RegVal. Exit on equal or LoopCount is reached.

| 0x0f |  column | row | - | instruction size |
| :-: |  - | - | - | -: |
| opcode (8b) | const (8b) | const (8b) | pad (8b) | 4B |

Poll a register unit it equals to a given value or timeouts.


## OP_RECORD_TIMESTAMP (0x10)

write Operation ID and the current value of AIE TILE TIMER Register into a dedicated write buffer

| 0x10 |  operation_id | instruction size |
| :-: |  - | -: |
| opcode (8b) | const (24b) | 4B |

Profile AIE cycle count between DPU opcodes.



---
# Directives
The directive syntax is same as that used by GNU assembler <https://sourceware.org/binutils/docs/as/Pseudo-Ops.html>,
although only the following subset is supported.



---
