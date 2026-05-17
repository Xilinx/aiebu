; Negative test: multi-UC control code where start_cond_job_preempt counts
; differ across columns.
; Uses groups 0 and 2 (even groups required for multi-column preempt prebuilt
; templates: keys 10 and 12 in the aie4 save/restore map).
; col 0: PREEMPT + START_COND_JOB_PREEMPT (count = 1)
; col 2: PREEMPT only                     (count = 0)
; Expected error: "start_cond_job_preempt opcode count mismatch"
.target aie4
.partition 3column
.attach_to_group 0
START_JOB 0
	LOAD_PDI 0, @pdi0
END_JOB
START_JOB 1
	PREEMPT 0x0001, @save, @restore
END_JOB
START_COND_JOB_PREEMPT 2
END_JOB
.eop
EOF
.attach_to_group 2
START_JOB 0
	LOAD_PDI 1, @pdi2
END_JOB
START_JOB 1
	PREEMPT 0x0001, @save, @restore
END_JOB
.eop
EOF
