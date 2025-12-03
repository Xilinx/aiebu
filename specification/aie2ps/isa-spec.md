# Introduction
This document outlines the specification of the _ctrlcode_ ISA which serves as the instruction set
interpreted by the *CERT* _VM_ also referred to as _job-runner_ in this document. The _job-runner_
is hosted on top of a Baremetal OS which runs on top of Microblaze uController.
The whole stack is called CERT.

{% dot
digraph cert_stack {
    fontname="Courier New"
	subgraph cluster_0 {
	rankdir=TB;
	node [shape="box" fontname="Courier New"];
	CtrlCode;
	node [shape="record" fontname="Courier New"];
	label="CERT Array";
	labelloc="t";
	structcert0 [ label = "{Job-Runner|Baremetal OS|uController}"; ];
	structcert1 [ label = "{Job-Runner|Baremetal OS|uController}"; ];
	structcert2 [ label = "{|...|}"; ];
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

%}

# Control-Code Sequence
The control-code is a sequence of opcodes along with operands, organized as _jobs_ which run in a
pseudo-parallel fashion using cooperative multi-tasking.

Each job is stack-less and has a private set of 8 working registers `r0`..`r7` which can be used as input
and output operands. The sequence of jobs can be followed by user-defined custom data, e.g.
configuration data which is to be transferred via the uC-DMA.

{% dot
digraph control_code_structure {
  node [shape="box" style="rounded"]
  rankdir="LR"

  begin [shape=point]
  end [shape=point]

  START_JOB [label="START_JOB
(_DEFERRED)"]

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

%}

# Alignment
All operands have to be naturally aligned, i.e. 16-bit numbers may only start on 16-bit boundaries and
32-bit numbers may only start on 32-bit boundaries. The size of an operation, i.e. the opcode followed by all operands,
should be a multiple of 32 bits. To achieve the required alignment, padding operands can be used.

# Endianness
All numbers are encoded in little-endian order.

# Control-Code Pages
Control-code is organized as a series of pages which are paged-in to the shared Data Memory (sDM)
by the uC-DMA in a ping-pong fashion. A page cannot have a barrier or lock dependency on a following page,
although backward dependency should work. Page boundary is hinted by the `.eop` [directive](#directives).

# Control-Code Structure
TODO: Describe START_JOB / END_JOB etc, state diagram?
Control Code for uC 0 in Column 0

{% dot
digraph control_code {
    fontname="Courier New";
    rankdir=TB;
    subgraph cluster_0 {
        rankdir=TB;
        node [shape="record" fontname="Courier New"];
        label="Page 0";
        labelloc="t";
        fontname="Courier New";

        structcertn [label="{Job-n|Opcode 0|Opcode 1|...|Opcode n}"];
        structcert2 [label="{|||...|}"];
        structcert1 [label="{Job-1|Opcode 0|Opcode 1|...|Opcode n}"];
        structcert0 [label="{Job-0|Opcode 0|Opcode 1|...|Opcode n}"];
    }
	label="...
Page n";
	labelloc="b";
}
%}


# Tile and actor IDs
## Tile
- All DMA S2MM channels
- All DMA MM2S channels

## MEM
- All DMA S2MM channels
- All DMA MM2S channels

## Shim
- All DMA S2MM channels
- All DMA MM2S channels

# Binary Format
Control-code asm files are assembled into standard 32-bit ELF binary files. The file is organized into [sections](#directives).
A section's column number and page number tells CERT which column's sDM the section should be loaded on. The page number
is used for cooperative paging using a ping-pong scheme.

{% dot
digraph cert_stack {
    fontname="Courier New"
	rankdir=TB;
	node [shape="record" fontname="Courier New"];
	structcert0 [ label = "{ELF Header|.ctrltext.0.0|.ctrldata.0.0|.ctrltext.0.1|.ctrldata.0.1|.ctrltext.0.2|.ctrldata.0.2|.ctrltext.1.0|.ctrldata.1.0|.shstrtab|.dynsym}"; ];
    label="Column 0 with three pages
 and column 1 with one page";
	labelloc="b";

}

%}


# Data Types

## opcode
8-bit unsigned integer containing a unique identifier for the operation.


## register
8-bit unsigned integer 0..23 used to reference _job-runner_ registers `r0..r23`.
Register `r0..r7` are local registers which are private to each job.
Register `r8..r23` are global registers which are shared among all the jobs.
Global registers `r8..r23` also have alias name `g0..g15`


## barrier
8-bit unsigned integer used to reference barriers provided by _job-runner_.
There are 2 types of barrier provided, local_barrier and remote_barrier.
Barrier `lb0..lb15`, which essentially are integer 0..15, are local barriers
which are used for synchronization across jobs running in same _job-runner_.
Barrier `rb0..rb7`, which essentially are integer 1..8, are remote barriers
which are used for synchronization across jobs running on different _job-runner_.


## const
Constant number. The width is specified per-operand and should be 8, 16 or 32 bit.


## page_id
16 bit constant number to specify a label which is from the page `page_id`. This type is used
to tell the assembler the label is not resolved in current page


## pad
Used for padding / alignment purposes. The value is always set to zero.
In human-readable representations (asm file), padding operands are omitted from the operand list.


## jobsize
16-bit unsigned integer containing the size of the job (`START_JOB` until `END_JOB`), automatically
calculated and filled by the assembler.
In human-readable representations (asm file), this operand is omitted from the operand list.


## patch_buf
Used for host address patching. Arg with this type has name starting with @, and this type of arg
is consumed only by assembler.



# Operations

## START_JOB (0x00)

Indicates the start of a new job.

| 0x00 | - | job_id | size | - | instruction size |
| :-: | - | - | - | - | -: |
| opcode (8b) | pad (8b) | const (16b) | jobsize (16b) | pad (16b) | 8B |

Indicates the start of a new job and creates a new entry in the job table,
if not done yet.
The job size is auto-calculated and inserted by the assembler and not supplied
explicitly by the user.


## START_JOB_DEFERRED (0x17)

Indicates the start of a new deferred job.

| 0x17 | - | job_id | size | - | instruction size |
| :-: | - | - | - | - | -: |
| opcode (8b) | pad (8b) | const (16b) | jobsize (16b) | pad (16b) | 8B |

Same as `START_JOB`, but does not schedule the job yet. The new job can be
launched manually via `LAUNCH_JOB`.


## LAUNCH_JOB (0x18)

Launches a deferred job.

| 0x18 | - | job_id | instruction size |
| :-: | - | - | -: |
| opcode (8b) | pad (8b) | const (16b) | 4B |

Launch / schedule a job created with `START_JOB_DEFERRED`.
This operation might only be called once per job. Launching the same job multiple
times is undefined behavior.
The newly launched job will be first scheduled for the next scheduling cycle and
the current job will continue to run until encountering a blocking operation.


## START_COND_JOB_PREEMPT (0x1f)

Indicates a new job which can be conditionally started according to preemption.

| 0x1f | - | job_id | size | - | instruction size |
| :-: | - | - | - | - | -: |
| opcode (8b) | pad (8b) | const (16b) | jobsize (16b) | pad (16b) | 8B |

Indicates the start of a new job and creates a new entry in the job table,
if preemption flag has been set. The flag is cleared when preemption is
checked, and is set if preemption happens.
The job size is auto-calculated and inserted by the assembler and not supplied
explicitly by the user.


## UC_DMA_WRITE_DES (0x01)

Enqueues a DM2MM uC-DMA transfer and returns a wait handle for it.

| 0x01 | - | wait_handle | - | descriptor_ptr | - | instruction size |
| :-: | - | - | - | - | - | -: |
| opcode (8b) | pad (8b) | register (8b) | pad (8b) | const (16b) | pad (16b) | 8B |

Enqueues a DM2MM uC-DMA transfer with the given pointer used as the start
BD. The pointer is relative to the data section of the control page.
In case the DM2MM queue is full, the job will preempt and retried in the
next scheduling cycle, i.e. the job will be blocked until there is space available
in the queue.
In case enqueueing is successful, the operation returns immediately without
waiting for the DMA transfer to finish. A wait handle is returned in the
register specified in `wait_handle`. This handle can be used to wait for the
DMA transfer to finish via the `WAIT_UC_DMA` operation.


## WAIT_UC_DMA (0x02)

Waits for the specified uC-DMA wait handle to finish.

| 0x02 | - | wait_handle | - | instruction size |
| :-: | - | - | - | -: |
| opcode (8b) | pad (8b) | register (8b) | pad (8b) | 4B |

Waits for the wait handle stored in the register specified in `wait_handle` to
finish.
If the associated uC-DMA transfer is already finished when this operation is
called, the operation returns immediately. Otherwise, the operation will yield
control to a different job and the job will not be re-scheduled until the transfer
has finished.


## MASK_WRITE_32 (0x03)

Writes a constant value with masked bits to a constant AXI-MM address.

| 0x03 | - | - | address | mask | value | instruction size |
| :-: | - | - | - | - | - | -: |
| opcode (8b) | pad (8b) | pad (16b) | const (32b) | const (32b) | const (32b) | 16B |

This operation is RMW. Those unmasked bits at the address are unchanged before
and after the operation. The RMW itself is not atomic, so there would be potential
race condition if multiple uC run this opcode towards same address.


## WRITE_32 (0x05)

Writes a constant value to a constant AXI-MM address.

| 0x05 | - | - | address | value | instruction size |
| :-: | - | - | - | - | -: |
| opcode (8b) | pad (8b) | pad (16b) | const (32b) | const (32b) | 12B |

The constant value is written to a constant AXI-MM address. The write is performed
via the uC's AXI-MM store port and does not involve the uC-DMA.
This operation is synchronous, i.e. it is guaranteed that the write has been executed
completely when this operation finishes.


## WAIT_TCTS (0x06)

Wait for tcts for the shim/mem DMA tasks.

| 0x06 | - | tile_id | actor_id | - | target_tcts | - | instruction size |
| :-: | - | - | - | - | - | - | -: |
| opcode (8b) | pad (8b) | const (16b) | const (8b) | pad (8b) | const (8b) | pad (8b) | 8B |

Wait for `target_tcts` number of tcts for tasks enqueued on a shim or mem DMA channel.
Tasks can be enqueued through `WRITE_32` and/or `UC_DMA_WRITE_DES/UC_DMA_WRITE_DES_SYNC`
In case the number of TCTs haven't all been arrived yet when this operation is executed,
the current job will be blocked until all the TCT arrive. The runtime will proceed with
execution of other (non-blocked) jobs during that time.
Use this operation only for tasks which have TCTs enabled and where the TCTs are
routed to arrive at the uC executing this operation.
Note that tasks can be enqueued from any jobs, but only one job can call `WAIT_TCTS`. And
waiting for more tcts than enqueued will hang the job forever


## END_JOB (0x07)

Indicates the end of the current job.

| 0x07 | - | - | instruction size |
| :-: | - | - | -: |
| opcode (8b) | pad (8b) | pad (16b) | 4B |

Indicates the end of a job started with `START_JOB`. After `END_JOB`, the only
valid operations are `START_JOB` and `EOF`.


## YIELD (0x08)

Yields to another job.

| 0x08 | - | - | instruction size |
| :-: | - | - | -: |
| opcode (8b) | pad (8b) | pad (16b) | 4B |

Tells the scheduler to perform a context switch to the next available job. The
current job will be re-scheduled in the next scheduling cycle.


## UC_DMA_WRITE_DES_SYNC (0x09)

Enqueues a DM2MM uC-DMA transfer and waits for it to finish.

| 0x09 | - | descriptor_ptr | instruction size |
| :-: | - | - | -: |
| opcode (8b) | pad (8b) | const (16b) | 4B |

Convenience operation which combines `UC_DMA_WRITE_DES` and `WAIT_UC_DMA`, i.e. enqueues the uC-DMA BD
at `descriptor_ptr` and waits until the uC-DMA finishes processing that BD.


## WRITE_32_D (0x0b)

Writes a constant value or a value from a register to an AXI-MM address which is constant or comes from a register.

| 0x0b | - | flags | - | address | value | instruction size |
| :-: | - | - | - | - | - | -: |
| opcode (8b) | pad (8b) | const (8b) | pad (8b) | const (32b) | const (32b) | 12B |

Dynamic version of `WRITE_32`. The flags arg is used to tell whether address/value are immediate
or a register number.
```
| bit 0                                 | bit 1                               |
| ------------------------------------- | ----------------------------------- |
| 1 : address is immediate              | 1 : value is immediate              |
| 0 : address is register number.       | 0 : value is register number.       |
| The write address is in this register | The write value is in this register |
```


## READ_32 (0x0c)

Reads the value of a constant AXI-MM address and stores it in a register.

| 0x0c | - | value | - | address | instruction size |
| :-: | - | - | - | - | -: |
| opcode (8b) | pad (8b) | register (8b) | pad (8b) | const (32b) | 8B |

The value of the constant AXI-MM address in `address` is read and put into
destination register `value`. The read is performed via the uC's AXI-MM store port and
does not involve the uC-DMA.
This operation is synchronous, i.e. it is guaranteed that the read has been executed
completely when this operation finishes.


## READ_32_D (0x0d)

Reads the value of an AXI-MM address stored in a register and stores it in a register.

| 0x0d | - | address | value | instruction size |
| :-: | - | - | - | -: |
| opcode (8b) | pad (8b) | register (8b) | register (8b) | 4B |

The value of the AXI-MM address loaded from register `address` is read and put into
destination register `value`. The read is performed via the uC's AXI-MM store port and
does not involve the uC-DMA.
This operation is synchronous, i.e. it is guaranteed that the read has been executed
completely when this operation finishes.


## APPLY_OFFSET_57 (0x0e)

Applies an offset to one or more shim DMA BD base address fields. (Version for 57-bit base addresses.)

| 0x0e | - | table_ptr | num_entries | offset | pad_buf | instruction size |
| :-: | - | - | - | - | - | -: |
| opcode (8b) | pad (8b) | const (16b) | const (16b) | const (16b) | patch_buf (0b) | 8B |

Patches the base address of `num_entries` shim DMA buffer descriptors by adding the offset
loaded from 'offset' and 'offset+1' in argument list.
if the offset is 0xFFFF, the offset added is the host address of 1st page of control code
The location of the `num_entries` buffer descriptors should be given in a table stored
at `table_ptr`. One entry in the table is a set of shim DMA BDs. If there are multiple entries in the table,
those set of BDS have to be contiguous.
4th optional arg `pad_buf` with special type `patch_buf`, specifies the pad buffer to hold blob for control packet,
or save/restore L2 as scratchpad. It is a string starting with @, and only consumed by assembler to do host address
patch at compile time.
Example:
```
# non-contiguous BDs
  APPLY_OFFSET_57     @mem21_bd0, 1, 0xFFFF
  APPLY_OFFSET_57     @mem31_bd0, 1, 0xFFFF
# ...
.align              4
mem21_bd0:
  WORD              0x00000080
  WORD              0x00020000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x80000000
  WORD              0x00000000
.align              4
mem31_bd0:
  WORD              0x00000080
  WORD              0x00020000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x80000000
  WORD              0x00000000

# contiguous BDs
  APPLY_OFFSET_57     @mem41_bd0, 2, 0xFFFF
# ...
.align              4
mem41_bd0:
  # 1st set BDs
  WORD              0x00000080
  WORD              0x00020000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x80000000
  WORD              0x00000000
  # 2nd set BDs
  WORD              0x00000080
  WORD              0x00020000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x80000000
  WORD              0x00000000
# patch scratchpad to BDs
  .setpad ctrl_pkt, ctrlpkt.bin
  .setpad tile_pad, 0x100
# ...
  APPLY_OFFSET_57     @mem21_bd0, 1, 3, @ctrl_pkt
  APPLY_OFFSET_57     @mem21_bd1, 1, 0xFFFF, @tile_pad
# ...
  ALIGN             4
mem21_bd0:
  WORD              0x00000080
  WORD              0x00020000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x80000000
  WORD              0x00000000
mem21_bd1:
  WORD              0x00000080
  WORD              0x00020000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x00000000
  WORD              0x80000000
  WORD              0x00000000
```
The patching using this operation needs be done _before_ programming the shim DMA buffer
descriptors via the uC-DMA.


## ADD (0x0f)

Adds `value` to the content of register `dest`.

| 0x0f | - | dest | - | value | instruction size |
| :-: | - | - | - | - | -: |
| opcode (8b) | pad (8b) | register (8b) | pad (8b) | const (32b) | 8B |

The value stored in register `dest` is loaded, then `value` is added to it and the result is
stored in the `dest` register.


## MOV (0x10)

Moves `value` to register `dest`.

| 0x10 | - | dest | - | value | instruction size |
| :-: | - | - | - | - | -: |
| opcode (8b) | pad (8b) | register (8b) | pad (8b) | const (32b) | 8B |

Stores the constant `value` in the register `dest`.


## LOCAL_BARRIER (0x11)

Local barrier used to synchronize multiple jobs.

| 0x11 | - | local_barrier_id | num_participants | instruction size |
| :-: | - | - | - | -: |
| opcode (8b) | pad (8b) | barrier (8b) | const (8b) | 4B |

`local_barrier_id` indicates a barrier resource that is used to synchronize `num_participants`
number of jobs. _All_ participating jobs must reach this barrier before any participating job
can move forward. A job which executes this instruction is blocked till the barrier matures. Example:
```
LOCAL_BARRIER       $lb0, 2
```


## REMOTE_BARRIER (0x12)

Remote barrier used to synchronize multiple columns.

| 0x12 | - | remote_barrier_id | - | party_mask | instruction size |
| :-: | - | - | - | - | -: |
| opcode (8b) | pad (8b) | barrier (8b) | pad (8b) | const (32b) | 8B |

`remote_barrier_id` indicates a barrier resource that is used to synchronize jobs in different
columns. _All_ participating jobs must reach this barrier before any participating job can move forward.
A job which executes this instruction is blocked till the barrier matures. `party_mask` is the bit map of
participant columns, where each column takes the bit corresponding to its column index. For any specific
remote barrier, only one job in a column can participate in. Example:
```
REMOTE_BARRIER      $rb0, 0xC
```


## EOF (0xff)

Indicates the end of the operation sequence.

| 0xff | - | - | instruction size |
| :-: | - | - | -: |
| opcode (8b) | pad (8b) | pad (16b) | 4B |

`EOF` must follow the last `END_JOB` instruction in a page and tells the
runtime _job-runner_ to stop parsing the code any further.


## POLL_32 (0x13)

Poll read address until the response equals value.

| 0x13 | - | - | address | value | instruction size |
| :-: | - | - | - | - | -: |
| opcode (8b) | pad (8b) | pad (16b) | const (32b) | const (32b) | 12B |

This poll is a busy poll. When it is used to check the occur of an event,
e.g. DMA completion, it is way more expensive compared to the event trigger
implementation, aka TCT.
Note that the poll will block the current job, but other jobs will continue
to execute.


## MASK_POLL_32 (0x14)

Poll read address until the response equals value after mask applied.

| 0x14 | - | - | address | mask | value | instruction size |
| :-: | - | - | - | - | - | -: |
| opcode (8b) | pad (8b) | pad (16b) | const (32b) | const (32b) | const (32b) | 16B |

This poll is busy poll. When it is used to check the occur of an event,
e.g. DMA completion, it is way more expensive compared to the event trigger
implementation, aka, TCT


## TRACE (0x15)

Collect trace info at opcode boundary

| 0x15 | - | info | instruction size |
| :-: | - | - | -: |
| opcode (8b) | pad (8b) | const (16b) | 4B |

This opcode is used for dynamic tracing purpose. Dynamic means, without
control code change, trace probe can be placed at any place dynamically
in control code to collect the trace info when the job runs there.
This opcode should not be used in user control code.
In order to use this, for each trace probe that is inserted at a target opcode,
user needs to create a control body.
The first 4 bytes of the target opcode binary are replaced with the binary of this opcode
before the control code is loaded to CERT. Control body of all trace probes should be loaded to CERT first as well.
The trace control body locates in sDM. There are amount of sDM preserved for dynamic tracing purpose.
Once collected, those data will be moved out to host ddr.
Control body:
```
| Word | B0 | B1 | B2 | B3 | Description                             |
|------|----|----|----|----|---------------------------------------- |
| W0   | 01 | 00 | ff | ff | `probe_begin`                           |
| W1   | 01 | 01 | 04 | 00 | `probe_end`, control body at W4         |
| W2   | 00 | 02 | m1 | 00 | `probe_jprobe`, control body at Wm1     |
| W3   | 10 | 03 | n1 | 00 | `probe_tracepoint`, control body at Wn1 |
| W4   | .. | .. | .. | .. |                                         |
| ...  |    |    |    |    |                                         |
| Wm1  | 00 | 02 | m2 | 00 | `jprobe` for opcode in                  |
|      | p1 | 00 | o1 | 00 |  page p1 at offset o1                   |
|      | .. | .. | .. | .. |                                         |
| Wm2  | 00 | 02 | ff | ff | `jprobe` for opcode in                  |
|      | p2 | 00 | o2 | 00 |  page p2 at offset o2                   |
|      | .. | .. | .. | .. |                                         |
| Wn1  | 00 | 03 | ff | ff | `tracepoint` at Wn1 with                |
|      | 00 | 00 | i1 | 00 |  index i1                               |
|      | .. | .. | .. | .. |                                         |
```
The control body is organized as link list of each probe type in order of
`BEGIN/END/JPROBE/TRACEPOINT`. The second and third bytes of the probe's first word indicate the location of the next probe.
For the last probe, these bytes are set to 0xFFFF. Each probe will have one or multiple actions.
Each action specify what user wants to do once the probe fires. e.g collect timestamp, read/write aie register,
block read/write aie registers, break control code run.
For timestamp action, there are 3 word fields in control body,
```
| Word | B0 | B1 | B2 | B3 | Description           |
|------|----|----|----|----|-----------------------|
| W0   | 00 | 02 | 00 | 01 | `timestamp`           |
| W1   | 00 | 00 | 00 | 00 | high 32 bit timestamp |
| W2   | 00 | 00 | 00 | 00 | low 32 bit timestamp  |
```


## NOP (0x16)

No-op (no operation) instruction.

| 0x16 | - | - | instruction size |
| :-: | - | - | -: |
| opcode (8b) | pad (8b) | pad (16b) | 4B |

`NOP` instruction is ignored by the _job-runner_. `NOP` instructions may be used for benchmarking
the _job-runner_ performance and for inserting small delays. Calling this operation may yield
control to another job.


## PREEMPT (0x19)

save/restore context of an inference at preemption point in control code

| 0x19 | - | id | save_control_code_offset | restore_control_code_offset | instruction size |
| :-: | - | - | - | - | -: |
| opcode (8b) | pad (8b) | const (16b) | page_id (16b) | page_id (16b) | 8B |

If the control code supports preemption, there will be preemption points inserted in it. `id` specifies the preemption
point index. We assume all aie cores are stateless, so at each preemption point, we only need to save/restore contents
in memtile. `save_control_code_offset` specifies a relative address in unit of page to the 1st page of the control code,
and this address is where the `SAVE` control code resides. `restore_control_code_offset` specifies a relative address in
unit of page to the 1st page of control code, and this address is where the `RESTORE` control code resides. The opcode
can determine whether preemption is required, and if required, it can also distinguish whether it is to `SAVE` or to
`RESTORE`, and run the `SAVE` or `RESTORE` control code accordingly.
In multi-uc case, for each preemption point id, the control code of each uc should have this opcode with same `id`.
This opcode should take one whole job which in turn should take one whole page.


## LOAD_PDI (0x1a)

load pdi

| 0x1a | - | - | pdi_id | pdi_host_addr_offset | - | instruction size |
| :-: | - | - | - | - | - | -: |
| opcode (8b) | pad (8b) | pad (16b) | const (32b) | page_id (16b) | pad (16b) | 12B |

pdi itself is also a piece of control code. It can be loaded by other control code at anywhere anytime.
`pdi_id` is an elf wide unique id and specifies an unique pdi. consecutive loading of same pdi results in following
loading skipped by the uC. `pdi_host_addr_offset` specifies a relative address in unit of page to the 1st page of
the control code and is where the pdi control code resides
This opcode should take one whole job which in turn should take one whole page.


## LOAD_CORES (0x04)

load cores

| 0x04 | - | - | core_elf_id | core_elf_host_addr_offset | - | instruction size |
| :-: | - | - | - | - | - | -: |
| opcode (8b) | pad (8b) | pad (16b) | const (32b) | page_id (16b) | pad (16b) | 12B |

This essentially is same to LOAD_PDI except that CERT will save the elf info for aie cores to different location than the pdi so that cert can do recovery of both during preemption


## LOAD_CORES_CP (0x20)

load cores_cp

| 0x20 | - | - | core_elf_id | instruction size |
| :-: | - | - | - | -: |
| opcode (8b) | pad (8b) | pad (16b) | const (32b) | 8B |

This is to load control packet format core elfs. Compared to the LOAD_CORES, this opcode assumes the control code to load the control packet through shimdma will not take more than a page so that the current page that holds this opcode will not be overwritten. As a result, we can put this control packet loading part right after this opcode and hold everything in one job. The `core_elf_id` in this opcode and the one in LOAD_CORES are in same id space so that if there are LOAD_CORES and LOAD_CORES_CP, cert can only save the id for the last of these opcodes.


## LOAD_LAST_PDI (0x1b)

load last loaded pdi

| 0x1b | - | - | instruction size |
| :-: | - | - | -: |
| opcode (8b) | pad (8b) | pad (16b) | 4B |

Used in preemption restore case. The info (pdi id and location in host ddr) of the pdi last time loaded is saved
in firmware. During restore (after a context switch), the last loaded pdi will be loaded with this opcode


## SAVE_TIMESTAMPS (0x1c)

save the timestamps

| 0x1c | - | - | unq_id | instruction size |
| :-: | - | - | - | -: |
| opcode (8b) | pad (8b) | pad (16b) | const (32b) | 8B |

Used to save the time stamps whenever control code encountered this opcode. AIE compiler will also add the unique
identifier while generating control code. The timestamps will get saved in shared data memory. Whenever the
allocated shared memory gets fill, CERT should initiate the uc-DMA to transfer data from shared
data memory to host memory.


## SLEEP (0x1d)

sleep instruction.

| 0x1d | - | - | target | instruction size |
| :-: | - | - | - | -: |
| opcode (8b) | pad (8b) | pad (16b) | const (32b) | 8B |

Sleep instruction makes the current job busy waiting `target` microseconds. This opcode is for test purpose
only. Calling this operation doesn't yield control to another job.


## SAVE_REGISTER (0x1e)

Reads value from specified register address and saves it in Shared Data Memory.

| 0x1e | - | - | address | unq_id | instruction size |
| :-: | - | - | - | - | -: |
| opcode (8b) | pad (8b) | pad (16b) | const (32b) | const (32b) | 12B |

Used to save the register address values. AIE compiler will also add unique identifier while generating control code.
Values will be saved in the shared data memory. Whenever the allocated shared memory gets fille, CERT will start uC-DMA
to transfer data from sDM to host memory.



# Directives
The directive syntax is same as that used by GNU assembler <https://sourceware.org/binutils/docs/as/Pseudo-Ops.html>,
although only the following subset is supported.


## .long

Specifies a 4 byte integer value.

|`.long` | value |
|-| - |
|-| integer |

Specifies 4 byte integer value often used for BDs in ctrldata.
Example:
```
mem21_bd0:
  .long              0x00000080
  .long              0x00020000
  ...
```


## .align

Specifies a specific aligment for the next ctrlcode fragment.

|`.align` | value |
|-| - |
|-| integer |

Specifies a specific alignment defined by the `value` parameter for the following `ctrltext` or `ctrldata`.
Example:
```
  .align           16
  INPUT_row1_actor11_taskleadin_ucbds:
    UC_DMA_BD       0, 0x001A0000, @mem01_bd0, 8, 0, 1
    UC_DMA_BD       0, 0x001A0020, @mem01_bd1, 8, 0, 1
    UC_DMA_BD       0, 0x021A0000, @mem11_bd0, 8, 0, 0
    ...
```


## .section

Specifies a name for the section with optional flags.

|`.section` | name | flags |
|-| - | - |
|-| string | string |

Describes the name of a ctrlcode section and optional flags (A,W,X) for mapping at runtime.
`.ctrltext` is used for text sections and `.ctrldata` is used for data sections. Suffix in the
form of `.<ucnumber>` may be used with section names.
Example:
```
.section .ctrltext.2,"ax"
.align 16
START_JOB   0x15
  UC_DMA_WRITE_DES_SYNC   @label1
  LOCAL_BARRIER   $lb0, 0xB
END_JOB

START_JOB   0x16
...
```


## .include

Specifies the name of file that should be included at the point.

|`.include` | file |
|-| - |
|-| string |

Provides a method to organize miltiple asm files which can be combined together to produce a single logical asm file.
Example:
```
...
.include "resetcode.0.asm"
.include "corecode.1.asm"
START_JOB   0x15
  UC_DMA_WRITE_DES_SYNC   @label1
  LOCAL_BARRIER   $lb0, 0xB
END_JOB
START_JOB   0x16
...
```


## .eop

Specifies end of page.

|`.eop` |
|-|
|-|

Provides a method to specify end of page.
Example:
```
...
START_JOB   0x15
  UC_DMA_WRITE_DES_SYNC   @label1
  LOCAL_BARRIER   $lb0, 0xB
END_JOB
.eop

START_JOB   0x16
...
```


## .attach_to_group

Specifies the uC index for the following sections.

|`.attach_to_group` | uC index |
|-| - |
|-| integer |

Provides a method to bind the following sections to a specific uC. Multiple
such directives may be used in a file each with a different uC index. The
assembler uses this directive to generate sections names like `.ctrltext.<ucnumber>`
and `.ctrldata.<ucnumber>`.
Note that aie2ps uC index and column index are same, aie4 uC index is not similar to column index,
aie4 has two uC per column
```
aie4                          aie2ps
| column index | uC index |   | column index | uC index |
| -------------| -------- |   | -------------| -------- |
| column 0_A   | 0        |   | column 0     | 0        |
| column 0_B   | 1        |   | column 1     | 1        |
| column 1_A   | 2        |   | column 2     | 2        |
| column 1_B   | 3        |   | column 3     | 3        |
| column 2_A   | 4        |   | column 4     | 4        |
| column 2_B   | 5        |   | ...          | ...      |
```
Example:
```
...
.attach_to_group 0
START_JOB 0
  READ_32             $r0, 0x2100000
  WRITE_32_D          2, 0x4100000, 0
  REMOTE_BARRIER      $rb0, 0x6
END_JOB
EOF
.attach_to_group 1
START_JOB 0
  uC_DMA_WRITE_DES    $r0, @uc_dma_bd0
  WAIT_uC_DMA         $r0
  LOCAL_BARRIER       $lb0, 2
END_JOB
START_JOB 1
  LOCAL_BARRIER       $lb0, 2
  WRITE_32            0x01A0634, 0x80000000
  WAIT_TCTS           TILE_0_1, MM2S_0, 1
END_JOB
EOF
...
```


## .setpad

This directive is used to declare a buffer.

|`.setpad` | buffer name | buffer size/path to buffer blob |
|-| - | - |
|-| string | integer/string |

Provides a method to specify buffer/buffer blob. If an immediate number is passed
as 2nd arg, the number is size in words, there will be a buffer with name specified as 1st arg
declared with that size and initialized as all 0s. If a file is passed as 2nd arg, there will
be a buffer with name specified as 1st arg with the size of the file declared and initialized
with contents in the file.
`.setpad buffer_name, <buffer size>  or .setpad buffer_name, <buffer file path>`
All the buffers declared with this directive will be in .pad.<colnum> section in the elf.
Example:
```
...
.setpad save, 0x100
.setpad ctrl_pkt, ctrlpkt.bin
...
```


## .partition

This directive is used to specify the size of a partition.

|`.partition` | partition size information |
|-| - |
|-| string |

Specify size of a partition where the design can run. Depending on whether the design is in single-app mode or dual-app mode, the size of partition can be specified in number of columns as `Xcolumn` or number of cores plus number of 256kB memtile chunks as `Ycore:Zmem`
Example:
```
...
;For a 3 column partition in single-app mode
.partiton 3column
;For 2 core plus 1.5MB memory partition in dual-app mode
.partition 2core:6mem
...
```


## .target

This directive is used to specify the architecture platform where the control code runs

|`.target` | <arch>-<sub-arch> |
|-| - |
|-| string |

Specify the information of the target architecture. `arch` is aie architecture, i.e aie2ps or aie4. `sub-arch` is used to tell different version of the architecture, i.e a/b/z of aie4. `sub-arch` is optional
Example:
```
...
;For telluride
.target aie2ps
;For soundwave
.target aie4-a
;For medusa
.target aie4
;For pacifica
.target aie4-z
...
```


## UC_DMA_BD

In control code, the BD that needs to patch is defined by directive UC_DMA_BD

|`UC_DMA_BD` | address high | address low | local address | length | external | next_bd |
|-| - | - | - | - | - | - |
|-| integer | integer | integer | integer | integer | integer |

`address high`, always 0 in current generation of aie.
`address low`, address of aie array register that uC-DMA is going to access.
`local address` in sDM, it is a label in the data section of the page where the value of the BD registers are specified.
`length` is the number of registers staring from the `remote address`.
`external` flag, always 0.
`next_bd` flag, this is to tell whether this is the last uC-DMA BD in the chain.
Example:
```
  START_JOB 0
    uC_DMA_WRITE_DES_SYNC    $r0, @uc_dma_bd0
  END_JOB
  .align           16
  uc_dma_bd0:
    UC_DMA_BD       0, 0x001A0000, @mem01_bd0, 8, 0, 1
    UC_DMA_BD       0, 0x001A0020, @mem01_bd1, 8, 0, 1
    UC_DMA_BD       0, 0x021A0000, @mem11_bd0, 8, 0, 0
    ...
```


