.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;

START_JOB 1
PREEMPT	0x0, @save, @restore
END_JOB
START_JOB 2
PREEMPT	0x1, @save, @restore
END_JOB

START_JOB 3
	PREEMPT 0x0002, @save, @restore , @hintmap_22
END_JOB
EOF
.align    4

hintmap_22:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
hintmap_21:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
