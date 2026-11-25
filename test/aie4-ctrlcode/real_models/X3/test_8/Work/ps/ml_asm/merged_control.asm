.attach_to_group 0
START_JOB 0
    LOAD_PDI 1072476209, @pdi
END_JOB
.eop
.include ../ml_asm/aie_runtime_control0.asm
.include ../asm/pdi.asm

.attach_to_group 2
START_JOB 0
    LOAD_PDI 0, @dummy_pdi
END_JOB
.eop
.include ../ml_asm/aie_runtime_control1.asm
.include ../ml_asm/dummy_pdi.asm

.attach_to_group 4
START_JOB 0
    LOAD_PDI 0, @dummy_pdi
END_JOB
.eop
.include ../ml_asm/aie_runtime_control2.asm
.include ../ml_asm/dummy_pdi.asm

EOF
