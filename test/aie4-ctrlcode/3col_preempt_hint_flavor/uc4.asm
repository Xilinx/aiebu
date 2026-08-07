.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column

.attach_to_group 4

START_JOB 0
	LOAD_PDI 0, @pdi
END_JOB
START_JOB 1
; PREEMPT 0: own column — col2 bits 96-143 (full 48 chunks memtile 2_1)
	PREEMPT 0, @save, @restore , @hintmap_0
END_JOB
START_JOB 2
	NOP
END_JOB
START_JOB 3
; PREEMPT 1: own column — col2 bits 112-127 (middle 16 chunks of memtile 2_1)
	PREEMPT 1, @save, @restore , @hintmap_1
END_JOB
START_JOB 4
	NOP
END_JOB
START_JOB 5
; PREEMPT 2: own column — col2 bits 96-119 (first 24 chunks of memtile 2_1)
	PREEMPT 2, @save, @restore , @hintmap_2
END_JOB
START_JOB 6
	NOP
END_JOB
START_JOB 7
; PREEMPT 3: cross-column — col2 bits 80-95 (last 16 of memtile 1_1; 1-based col1)
	PREEMPT 3, @save, @restore , @hintmap_3
END_JOB
START_JOB 8
	NOP
END_JOB
START_JOB 9
; PREEMPT 4: own column — col2 bits 128-143 (last 16 of memtile 2_1; 1-based col2)
	PREEMPT 4, @save, @restore , @hintmap_4
END_JOB
START_JOB 10
	NOP
END_JOB
START_JOB 11
; PREEMPT 5: cross-column — col1 bits 32-47 (last 16 of memtile 0_1; 1-based col0)
	PREEMPT 5, @save, @restore , @hintmap_5
END_JOB
START_JOB 12
	NOP
END_JOB

.include pdi4.asm
EOF
.align 4
hintmap_0:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0xffffffff
	.long 0x0000ffff
hintmap_1:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0xffff0000
	.long 0x00000000
hintmap_2:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00ffffff
	.long 0x00000000
hintmap_3:
	.long 0x00000000
	.long 0x00000000
	.long 0xffff0000
	.long 0x00000000
	.long 0x00000000
hintmap_4:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0000ffff
hintmap_5:
	.long 0x00000000
	.long 0x0000ffff
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
