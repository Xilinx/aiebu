.attach_to_group 0
START_JOB 0
   LOAD_PDI 0, @pdi0
END_JOB
.include aie_runtime_control0.asm
pdi0:
START_JOB 9
NOP
END_JOB
EOF
.endl pdi0

.attach_to_group 2
START_JOB 0
   LOAD_PDI 0, @pdi1
END_JOB
.include aie_runtime_control2.asm

pdi1:
START_JOB 19
NOP
END_JOB
EOF
.endl pdi1

.attach_to_group 4
START_JOB 0
   LOAD_PDI 0, @pdi2
END_JOB
.include aie_runtime_control4.asm
pdi2:
START_JOB 29
NOP
END_JOB
EOF
.endl pdi2
EOF
