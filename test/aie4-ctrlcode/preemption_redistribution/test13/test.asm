.target aie4
.aie_row_topology 1-1-4-0
.partition 3column
 
.attach_to_group 0
START_JOB 0
	LOAD_PDI 0, @pdi0
END_JOB
START_JOB 1
	PREEMPT 0x0000, @save, @restore, @hintmap_lo
END_JOB
START_JOB 2
	PREEMPT 0x0000, @save, @restore, @hintmap_hi
END_JOB
pdi0:
START_JOB 0
	NOP
END_JOB
EOF
.endl pdi0
EOF
.align 4
hintmap_lo:
	.long 0x0000000f
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
hintmap_hi:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0f000000
	.long 0x00000000
 
.attach_to_group 2
START_JOB 0
	LOAD_PDI 0, @pdi2
END_JOB
START_JOB 1
	PREEMPT 0x0000, @save, @restore
END_JOB
START_JOB 2
	PREEMPT 0x0000, @save, @restore
END_JOB
pdi2:
START_JOB 0
	NOP
END_JOB
EOF
.endl pdi2
EOF
 
.attach_to_group 4
START_JOB 0
	LOAD_PDI 0, @pdi4
END_JOB
START_JOB 1
	PREEMPT 0x0000, @save, @restore, @hintmap_z
END_JOB
START_JOB 2
	PREEMPT 0x0000, @save, @restore, @hintmap_z
END_JOB
pdi4:
START_JOB 0
	NOP
END_JOB
EOF
.endl pdi4
EOF
.align 4
hintmap_z:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000