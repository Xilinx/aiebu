; Negative test: multi-UC control code where load_cores_cp counts differ.
; col 0: 1 LOAD_CORES_CP   col 1: 0 LOAD_CORES_CP
; Expected error: "load_cores_cp opcode count mismatch"
.target aie4
.partition 2column
.attach_to_group 0
START_JOB 0
	LOAD_PDI 0, @pdi0
	LOAD_CORES_CP 0
END_JOB
pdi0:
START_JOB 0
	NOP
END_JOB
EOF
.endl pdi0


.attach_to_group 1
START_JOB 0
	LOAD_PDI 1, @pdi1
END_JOB

pdi1:
START_JOB 0
	NOP
END_JOB
EOF
.endl pdi1
EOF
