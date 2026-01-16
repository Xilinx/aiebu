.partition	 3column
.attach_to_group 0
START_JOB 0
    LOAD_PDI 0, @pdi
END_JOB
.eop
.include aie_runtime_control.asm
.include pdi.asm
EOF
