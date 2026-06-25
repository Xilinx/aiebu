; Negative test: LOAD_CORES_CP present but no LOAD_PDI in the same control code elf.
; cert needs load_pdi as the recovery anchor; load_cores_cp without load_pdi is invalid.
; Expected error: "load_cores / load_cores_cp opcode requires at least one load_pdi"
.target aie4
.partition 1column
.attach_to_group 0
START_JOB 0
	LOAD_CORES_CP 0
END_JOB
EOF
