; Negative test: two LOAD_CORES instructions in the same column reference the
; same @label (same core-elf host address).  Each load_cores within a control
; code elf must point to a distinct address.
; Expected error: "load_cores location '@cores0' is not unique in column 0"
.target aie4
.partition 1column
.attach_to_group 0
START_JOB 0
	LOAD_PDI 0, @pdi0
	LOAD_CORES 0, @cores0
	LOAD_CORES 1, @cores0
END_JOB

pdi0:
START_JOB 0
	NOP
END_JOB
EOF
.endl pdi0


cores0:
START_JOB 0
	NOP
END_JOB
EOF
.endl cores0
EOF
