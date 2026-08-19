.target aie4
.aie_row_topology 1-1-4-0
.partition 3column

.attach_to_group 0

START_JOB 0
	LOAD_PDI 0, @pdi
END_JOB
START_JOB 1
; bits 0,3,6,9,12,15 — every 3rd chunk; span [0,15] overlaps col2 and col4.
; After redistribution: col0 gets chunks 0-5.
	PREEMPT 0, @save, @restore, @hintmap_0
END_JOB

.include pdi0.asm
EOF
.align 4
hintmap_0:
	.long 0x00009249
