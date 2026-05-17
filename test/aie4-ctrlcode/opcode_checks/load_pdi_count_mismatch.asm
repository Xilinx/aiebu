; Negative test: multi-UC control code where the number of LOAD_PDI opcodes
; differs between columns.  All UCs must issue the same number of load_pdi
; so that CERT can synchronize their barrier after each PDI load.
; col 0: 2 LOAD_PDI   col 1: 1 LOAD_PDI
; Expected error: "load_pdi opcode count mismatch"
.target aie4
.partition 2column
.attach_to_group 0
START_JOB 0
	LOAD_PDI 0, @pdi0
	LOAD_PDI 1, @pdi1
END_JOB

pdi0:
START_JOB 0
	NOP
END_JOB
EOF
.endl pdi0
pdi1:
START_JOB 0
	NOP
END_JOB
EOF
.endl pdi1

.attach_to_group 1
START_JOB 0
	LOAD_PDI 0, @pdi2
END_JOB

pdi2:
START_JOB 0
	NOP
END_JOB
EOF
.endl pdi2
EOF
