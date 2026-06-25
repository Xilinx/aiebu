; Leaf file included both via test.asm and directly from main.asm.
; The second inclusion must be caught as a duplicate-file-name error.
START_JOB 0
  NOP
END_JOB
EOF
