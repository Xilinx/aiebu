.attach_to_group 0
START_JOB 10
    LOAD_PDI 0, @pdi
END_JOB

.include aie_runtime_control.asm
pdi:
.include pdi.asm
.endl pdi

EOF
