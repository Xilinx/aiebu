; Disjoint hintmap spans -> direct Part-1 assignment, no redistribution.
; col0 bits 0,3; col2 bits 32,35; col4 bits 96,99
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
