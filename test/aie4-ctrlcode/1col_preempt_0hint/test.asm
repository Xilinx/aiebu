.target	 aie4
.partition	 1column
.attach_to_group 0
START_JOB 0
	LOAD_PDI 0, @pdi
END_JOB
START_JOB 1
	PREEMPT 0, @save, @restore , @hintmap_0
END_JOB
START_JOB 2
	NOP
END_JOB
START_JOB 3
	PREEMPT 1, @save, @restore , @hintmap_1
END_JOB
START_JOB 4
	NOP
END_JOB
START_JOB 5
	PREEMPT 2, @save, @restore , @hintmap_0
END_JOB
START_JOB 6
	NOP
END_JOB

.include pdi.asm
EOF
.align 4
hintmap_0:
	.long 0x00000000
hintmap_1:
	.long 0x0000000f
