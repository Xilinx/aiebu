; Negative test: two LOAD_PDI instructions in the same column reference the same
; @label (i.e. the same PDI host address).  Each load_pdi within a control code
; elf must point to a distinct address.
; Expected error: "load_pdi location '@pdi0' is not unique in column 0"
.target aie4
.partition 1column
.attach_to_group 0
START_JOB 0
	LOAD_PDI 0, @pdi0
	LOAD_PDI 1, @pdi0
END_JOB

pdi0:
START_JOB 0
	NOP
END_JOB
EOF
.endl pdi0


EOF
