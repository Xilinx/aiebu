; Negative test: TRACE opcode used directly in user control code.
; TRACE is reserved for internal use by CERT (dynamic tracing) and must
; never appear in user-generated control code.
; Expected error: "TRACE opcode is reserved for internal use by CERT"
START_JOB 0
  NOP
  TRACE 0x1
END_JOB
EOF
