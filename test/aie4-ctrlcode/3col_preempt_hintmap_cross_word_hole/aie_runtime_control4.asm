.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;

START_JOB 1
PREEMPT	0x0, @save, @restore , @hintmap_40
;PREEMPT	0x0, @save, @restore
END_JOB
EOF
.align    4

hintmap_40:
	.long 0x0000f000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000

