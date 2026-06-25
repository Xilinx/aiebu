; Negative test: PREEMPT opcode present but no LOAD_PDI in the same control
; code elf.  CERT relies on load_pdi to recover the last loaded PDI at each
; preemption point, so at least one load_pdi is mandatory when preempt is used.
; Expected error: "preempt opcode requires at least one load_pdi"
.target aie4
.partition 1column
.attach_to_group 0
START_JOB 0
	PREEMPT 0x0001, @save, @restore
END_JOB
.eop
EOF
