# Introduction
This document outlines the specification of the RyzenAI AIE2 _ctrlcode_ ISA which serves as the instruction set
interpreted by the *MPIPU* ERT thread.

{{ '{% dot
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

%}' }}

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


{{ '{% dot
digraph cert_stack {
    fontname="Courier New"
	rankdir=TB;
	node [shape="record" fontname="Courier New"];
	structcert0 [ label = "{ELF Header|.text_instr|.data_ctrl|.symtab}"; ];
    label="Sections in RyzenAI ctrlcode";
	labelloc="b";

}

%}' }}

---
# Data Types
{% for type in types %}
## {{type.name}}
{{type.description}}
{% endfor %}

---
# Operations
{% for operation in operations %}
## {{operation.mnemonic}} ({{'0x{:02x}'.format(operation.opcode)}})

{{operation.brief}}

| {{'0x{:02x}'.format(operation.opcode)}} | {% for arg in operation.arguments %} {{get_arg_name(arg)}} |{% endfor %} instruction size |
| :-: | {% for arg in operation.arguments %} - |{% endfor %} -: |
| opcode (8b) |{% for arg in operation.arguments %} {{arg.type}} ({{get_arg_width(arg)}}b) |{% endfor %} {{get_operation_size(operation)}}B |

{{operation.description}}
{% endfor %}

---
# Directives
The directive syntax is same as that used by GNU assembler <https://sourceware.org/binutils/docs/as/Pseudo-Ops.html>,
although only the following subset is supported.

{% for directive in directives %}
## {{directive.mnemonic}}

{{directive.brief}}

|`{{directive.mnemonic}}` |{% for arg in directive.arguments %} {{arg.name}} |{% endfor %}
|-|{% for arg in directive.arguments %} - |{% endfor %}
|-|{% for arg in directive.arguments %} {{arg.type}} |{% endfor %}

{{directive.description}}
{% endfor %}

---
