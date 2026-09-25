; Positive test: hintmap bitmaps may contain holes.  The scratchpad is the
; full span from first set bit to last set bit (inclusive); holes are absorbed
; so the transferred region is a superset of the set bits.
; col 0: bits 0 and 3 -> span [0,3] = [0x0,      0x40000)
; col 1: bits 32 and 35 -> span [32,35] = [0x200000, 0x240000)
; col 2: bits 96 and 99 -> span [96,99] = [0x600000, 0x640000)
; Regions are disjoint, so assembly must succeed.
.target aie4
.aie_row_topology 1-1-4-0
.partition 3column

.attach_to_group 0
START_JOB 0
	LOAD_PDI 0, @pdi0
END_JOB
START_JOB 1
	PREEMPT 0x0000, @save, @restore, @hintmap_0
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
	.long 0x00000009

.attach_to_group 2
START_JOB 0
	LOAD_PDI 0, @pdi2
END_JOB
START_JOB 1
	PREEMPT 0x0000, @save, @restore, @hintmap_0
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
	.long 0x00000009

.attach_to_group 4
START_JOB 0
	LOAD_PDI 0, @pdi4
END_JOB
START_JOB 1
	PREEMPT 0x0000, @save, @restore, @hintmap_0
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
	.long 0x00000009
