.target aie4
.aie_row_topology 1-1-4-0
.partition 3column

.attach_to_group 2

START_JOB 0
	LOAD_PDI 0, @pdi
END_JOB
START_JOB 1
; bits 1,4,7,10,13,16 — every 3rd chunk; span [1,16] overlaps col0 and col4.
; After redistribution: col2 gets chunks 6-11.
	PREEMPT 0, @save, @restore, @hintmap_0
END_JOB

.include pdi2.asm
EOF
.align 4
hintmap_0:
	.long 0x00012492
