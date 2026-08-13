; Negative test: multi-UC control code where two controllers select overlapping
; regions of the preemption scratchpad at the same preemption point.
; Hint bitmap bits are absolute addresses in the partition wide scratchpad, so
; the regions saved by different controllers must be disjoint.
; col 0: hintmap_0 = bits 0-3 -> [0x0,      0x40000)
; col 1: hintmap_0 = bits 2-3 -> [0x20000,  0x40000)  <- overlaps col 0
; col 2: hintmap_0 = bits 96-99 -> [0x600000, 0x640000)
; Expected error: "hint bitmap overlap at preemption point 0"
.target aie4
.aie_row_topology 1-1-4-0
.partition 3column

.attach_to_group 0
START_JOB 0
	LOAD_PDI 0, @pdi0
END_JOB
START_JOB 1
	PREEMPT 0x0001, @save, @restore, @hintmap_0
END_JOB
START_JOB 2
	PREEMPT 0x0002, @save, @restore, @hintmap_1
END_JOB
START_JOB 3
	PREEMPT 0x0001, @save, @restore, @hintmap_3
END_JOB
pdi0:
START_JOB 0
	NOP
END_JOB
EOF
.endl pdi0
EOF
.align 4
hintmap_0:
	.long 0x0000000f
hintmap_1:
	.long 0x0000000f
hintmap_3:
	.long 0x00000000
	.long 0x10000000

.attach_to_group 2
START_JOB 0
	LOAD_PDI 0, @pdi2
END_JOB
START_JOB 1
	PREEMPT 0x0001, @save, @restore, @hintmap_0
END_JOB
START_JOB 2
	PREEMPT 0x0002, @save, @restore, @hintmap_1
END_JOB
START_JOB 3
	PREEMPT 0x0003, @save, @restore
END_JOB
pdi2:
START_JOB 0
	NOP
END_JOB
EOF
.endl pdi2
EOF
.align 4
hintmap_0:
	.long 0x00000000
	.long 0x0000000f
hintmap_1:
	.long 0x00000000

.attach_to_group 4
START_JOB 0
	LOAD_PDI 0, @pdi4
END_JOB
START_JOB 1
	PREEMPT 0x0001, @save, @restore, @hintmap_0
END_JOB
START_JOB 2
	PREEMPT 0x0002, @save, @restore
END_JOB
START_JOB 3
	PREEMPT 0x0001, @save, @restore, @hintmap_3
END_JOB
pdi4:
START_JOB 0
	NOP
END_JOB
EOF
.endl pdi4
EOF
.align 4
hintmap_0:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0000000f
hintmap_3:
	.long 0x00000000
	.long 0x00000000
	.long 0x0000000f