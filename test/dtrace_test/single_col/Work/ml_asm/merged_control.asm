.target aie2ps
.attach_to_group 0
START_JOB 0
        LOAD_PDI 0, @pdi
END_JOB
.eop
.include ml_asm/aie_runtime_control.asm
.include asm/pdi.asm
