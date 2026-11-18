.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;
.attach_to_group	 4
; RECORD TIMER START
; SHIM_PRM
START_JOB 0
APPLY_OFFSET_57	 @DMAWRITE_data_0, 1, 3
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
REMOTE_BARRIER	 $rb0, 0x15
; RECORD TIMER STOP
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x4058000, @DMAWRITE_data_0, 0x9, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x405855c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4010000, @WRITE_data_0, 1, 0, 0
.align    4
DMAWRITE_data_0:
	.long 0x00000000
	.long 0x00341000
	.long 0x00002400
	.long 0x03000000
	.long 0x20000000
	.long 0x00000c00
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
WRITE_data_0:
	.long 0x00000000
