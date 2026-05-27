; Negative test: START_COND_JOB_PREEMPT appears before any PREEMPT opcode in
; the same column.  The conditional job only makes sense after a preemption
; point has been established.
; Expected error: "start_cond_job_preempt found in column 0 before any preempt opcode"
.target aie4
.partition 1column
.attach_to_group 0
START_JOB 0
	LOAD_PDI 0, @pdi0
END_JOB
START_COND_JOB_PREEMPT 1
END_JOB
EOF
