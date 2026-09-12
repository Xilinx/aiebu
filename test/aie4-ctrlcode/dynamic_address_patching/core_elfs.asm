core_elfs:
.include aie_asm_disable.asm
.eop
.include aie_asm_elfs.asm
.eop
.include aie_asm_core_reset_unreset.asm
.eop
.include aie_asm_enable.asm
.eop
.endl core_elfs
