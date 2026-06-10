.attach_to_group 0
START_JOB 0
    LOAD_PDI 1351194102, @pdi
END_JOB
.eop
START_JOB 0
    LOAD_CORES 4004200540, @core_elfs
END_JOB
.eop
.include ../ml_asm/aie_runtime_control0.asm
.include ../asm/pdi.asm
.include ../asm/core_elfs.asm

.attach_to_group 4
START_JOB 0
    LOAD_PDI 0, @dummy_pdi
END_JOB
.eop
START_JOB 0
    LOAD_CORES 0, @dummy_core_elfs
END_JOB
.eop
.include ../ml_asm/aie_runtime_control1.asm
.include ../ml_asm/dummy_pdi.asm
.include ../ml_asm/dummy_core_elfs.asm

.attach_to_group 8
START_JOB 0
    LOAD_PDI 0, @dummy_pdi
END_JOB
.eop
START_JOB 0
    LOAD_CORES 0, @dummy_core_elfs
END_JOB
.eop
.include ../ml_asm/aie_runtime_control2.asm
.include ../ml_asm/dummy_pdi.asm
.include ../ml_asm/dummy_core_elfs.asm

.attach_to_group 12
START_JOB 0
    LOAD_PDI 0, @dummy_pdi
END_JOB
.eop
START_JOB 0
    LOAD_CORES 0, @dummy_core_elfs
END_JOB
.eop
.include ../ml_asm/aie_runtime_control3.asm
.include ../ml_asm/dummy_pdi.asm
.include ../ml_asm/dummy_core_elfs.asm

.attach_to_group 16
START_JOB 0
    LOAD_PDI 0, @dummy_pdi
END_JOB
.eop
START_JOB 0
    LOAD_CORES 0, @dummy_core_elfs
END_JOB
.eop
.include ../ml_asm/aie_runtime_control4.asm
.include ../ml_asm/dummy_pdi.asm
.include ../ml_asm/dummy_core_elfs.asm

.attach_to_group 20
START_JOB 0
    LOAD_PDI 0, @dummy_pdi
END_JOB
.eop
START_JOB 0
    LOAD_CORES 0, @dummy_core_elfs
END_JOB
.eop
.include ../ml_asm/aie_runtime_control5.asm
.include ../ml_asm/dummy_pdi.asm
.include ../ml_asm/dummy_core_elfs.asm

EOF
