.attach_to_group 0
START_JOB 0
	LOAD_PDI 0, @pdi
END_JOB
.eop
.include aie_runtime_control.asm
.include ../asm/pdi.asm
.setpad control-packet, ctrl_pkt0.bin
