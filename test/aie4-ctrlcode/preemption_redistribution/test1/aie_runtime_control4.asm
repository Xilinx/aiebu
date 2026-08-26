.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;

START_JOB 1
PREEMPT	0x0, @save, @restore , @hintmap_40
END_JOB
START_JOB 2
PREEMPT	0x0, @save, @restore , @hintmap_41
END_JOB

START_JOB 3
PREEMPT	0x0, @save, @restore , @hintmap_42
END_JOB
EOF
.align    4

hintmap_40:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
hintmap_41:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
hintmap_42:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000004
