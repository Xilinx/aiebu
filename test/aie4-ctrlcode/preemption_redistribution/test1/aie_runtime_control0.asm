.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;

START_JOB 1
PREEMPT	0x0, @save, @restore , @hintmap_00
;PREEMPT	0x0, @save, @restore
END_JOB
.eop
START_JOB 2
PREEMPT	0x0001, @save, @restore , @hintmap_01
END_JOB

START_JOB 3
PREEMPT	0x0002, @save, @restore , @hintmap_01
END_JOB
EOF
.align    4

hintmap_00:
	.long 0x0000000f
	.long 0x00000000
	.long 0x00000000
	.long 0x0f000000
	.long 0x00000000
hintmap_01:
	.long 0x0000000f
	.long 0x00000000
	.long 0x00000000
	.long 0x0f000000
	.long 0x00000000
