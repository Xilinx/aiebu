.attach_to_group 0
.partition	 3column
START_JOB 0
    LOAD_PDI 0, @pdi
END_JOB
save:
.include aie4_save_1c.asm
.endl save
restore:
.include aie4_restore_1c.asm
.endl restore
.eop
.include aie_runtime_control.asm
.include pdi.asm
EOF
