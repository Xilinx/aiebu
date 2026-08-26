; Boundary: last valid chunk bit 143 in a 3-column partition (144 chunks total).
; col4 bit 143 only; col0/col2 no hintmap at same preemption point
.target aie4
.aie_row_topology 1-1-4-0
.partition 3column

.attach_to_group 0
START_JOB 0
	LOAD_PDI 0, @pdi0
END_JOB
START_JOB 1
	PREEMPT 0x0001, @save, @restore
END_JOB
pdi0:
START_JOB 0
	NOP
END_JOB
EOF
.endl pdi0
EOF

.attach_to_group 2
START_JOB 0
	LOAD_PDI 0, @pdi2
END_JOB
START_JOB 1
	PREEMPT 0x0001, @save, @restore
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
	PREEMPT 0x0001, @save, @restore, @hintmap_0
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
	.long 0x00000000
	.long 0x00008000
