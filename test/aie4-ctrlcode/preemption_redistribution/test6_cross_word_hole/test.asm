; Cross-word hintmap holes: col0 bits 0-3 and 120-123, col2 no hintmap, col4 bits 12-15.
; col0 span [0,123] overlaps col2 fixed [48,95] -> redistribution
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
	PREEMPT 0x0002, @save, @restore
END_JOB
START_JOB 3
	PREEMPT 0x0003, @save, @restore, @hintmap_1
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
	.long 0x00000000
	.long 0x00000000
	.long 0x0f000000
	.long 0x00000000

hintmap_1:
	.long 0x0000000f
	.long 0x00000000
	.long 0x00000f00
	.long 0x00000000
	.long 0x00000000

.attach_to_group 2
START_JOB 0
	LOAD_PDI 0, @pdi2
END_JOB
START_JOB 1
	PREEMPT 0x0001, @save, @restore
END_JOB
START_JOB 2
	PREEMPT 0x0002, @save, @restore, @hintmap_0
END_JOB
START_JOB 3
	PREEMPT 0x0003, @save, @restore, @hintmap_1
END_JOB
pdi2:
START_JOB 0
	NOP
END_JOB
EOF
.endl pdi2
EOF
hintmap_0:
	.long 0x00000000
	.long 0x000f0000
	.long 0x00000000
	.long 0x0f000000
	.long 0x00000000
hintmap_1:
	.long 0x0000f000

.attach_to_group 4
START_JOB 0
	LOAD_PDI 0, @pdi4
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
pdi4:
START_JOB 0
	NOP
END_JOB
EOF
.endl pdi4
EOF
.align 4
hintmap_0:
	.long 0x0000f000
hintmap_1:
	.long 0x00000000
	.long 0xf0000000
	