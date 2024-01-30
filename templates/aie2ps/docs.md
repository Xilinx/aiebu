# Introduction
This document outlines the specification of the _ctrlcode_ ISA which serves as the instruction set
interpreted by the *CERT* _VM_ also referred to as _job-runner_ in this document. The _job-runner_
is hosted on top of a Baremetal OS which runs on top of Microblaze uController. The whole stack is called CERT.

{{ '{% dot
digraph cert_stack {
    fontname="Courier New"
	subgraph cluster_0 {
	rankdir=TB;
	node [shape="tab" fontname="Courier New"];
	CtrlCode;
	node [shape="record" fontname="Courier New"];
	label="CERT Array";
	labelloc="t";
	structcert0 [ label = "{Job-Runner|Baremetal OS|uController}"; ];
	structcert2 [ label = "{|...|}"; ];
	structcert1 [ label = "{Job-Runner|Baremetal OS|uController}"; ];
	structcertn [ label = "{Job-Runner|Baremetal OS|uController}"; ];
	structcert0 -> CtrlCode;
	structcert1 -> CtrlCode;
	structcert2 -> CtrlCode;
	structcertn -> CtrlCode;
	color=blue;
	}
    label="Application Context";
	labelloc="b";

}

%}' }}

# Control-Code Sequence
The control-code is a sequence of opcodes along with operands, organized as _jobs_ which run in a
pseudo-parallel fashion using cooperative multi-tasking.

Each job is stack-less and has a private set of 8 working registers `r0`..`r7` which can be used as input
and output operands.

The sequence of jobs can be followed by user-defined custom data, e.g. configuration data which is to be
transferred via the uC-DMA.

{{ '{% dot
digraph control_code_structure {
  node [shape="box" style="rounded"]
  rankdir="LR"

  begin [shape=point]
  end [shape=point]

  START_JOB [label="START_JOB(_DEFERRED)"]

  begin -> START_JOB
  START_JOB -> operation
  operation -> operation
  operation -> END_JOB
  END_JOB -> START_JOB
  END_JOB -> EOF
  EOF -> data
  data -> data
  data -> end

}

%}' }}

# Alignment
All operands have to be naturally aligned, i.e. 16-bit numbers may only start on 16-bit boundaries and
32-bit numbers may only start on 32-bit boundaries.

The size of an operation, i.e. the opcode followed by all operands, should be a multiple of 32 bits.

To achieve the required alignment, padding operands can be used.

# Endianness
All numbers are encoded in little-endian order.

# Control-Code Pages
Control-code is organized as a series of pages which are paged-in to the shared Data Memory (sDM) by the uC-DMA in
a ping-pong fashion. A page cannot have a barrier or lock dependency on a following page, although backward
dependency should work. Page boundary is hinted by the `.eop` [directive](#directives).

# Control-Code Structure
TODO: Describe START_JOB / END_JOB etc, state diagram?

# Tile and actor IDs
t.b.d.

# Binary Format
Control-code asm files are assembled into standard 32-bit ELF binary files. The file is organized into [sections](#directives).
A section's column number and page number tells CERT which column's sDM the section should be loaded on. The page number
is used for cooperative paging using a ping-pong scheme.

{{ '{% dot
digraph cert_stack {
    fontname="Courier New"
	rankdir=TB;
	node [shape="record" fontname="Courier New"];
	structcert0 [ label = "{ELF Header|.ctrltext.0.0|.ctrldata.0.0|.ctrltext.0.1|.ctrldata.0.1|.ctrltext.0.2|.ctrldata.0.2|.ctrltext.1.0|.ctrldata.1.0|.shstrtab|.dynsym}"; ];
    label="Column 0 with three pages\n and column 1 with one page";
	labelloc="b";

}

%}' }}


# Data Types
{% for type in types %}
## {{type.name}}
{{type.description}}
{% endfor %}

# Operations
{% for operation in operations %}
## {{operation.mnemonic}} ({{'0x{:02x}'.format(operation.opcode)}})

{{operation.brief}}

| {{'0x{:02x}'.format(operation.opcode)}} | - |{% for arg in operation.arguments %} {{get_arg_name(arg)}} |{% endfor %} instruction size |
| :-: | - |{% for arg in operation.arguments %} - |{% endfor %} -: |
| opcode (8b) | pad (8b) |{% for arg in operation.arguments %} {{arg.type}} ({{get_arg_width(arg)}}b) |{% endfor %} {{get_operation_size(operation)}}B |

{{operation.description}}
{% endfor %}


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
