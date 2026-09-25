.attach_to_group 0
START_JOB 0
    LOAD_PDI 10511469, @pdi
END_JOB
.eop
START_JOB 0
    LOAD_CORES 2217025085, @core_elfs
END_JOB
.eop
.include aie_runtime_control.asm
.include pdi.asm
.include core_elfs.asm

EOF
