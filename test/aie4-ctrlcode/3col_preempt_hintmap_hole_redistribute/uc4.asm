.target aie4
.aie_row_topology 1-1-4-0
.partition 3column

.attach_to_group 4

START_JOB 0
	LOAD_PDI 0, @pdi
END_JOB
START_JOB 1
; bits 2,5,8,11,14,17 — every 3rd chunk; span [2,17] overlaps col0 and col2.
; After redistribution: col4 gets chunks 12-17.
	PREEMPT 0, @save, @restore, @hintmap_0
END_JOB

.include pdi4.asm
EOF
.align 4
hintmap_0:
	.long 0x00024924
