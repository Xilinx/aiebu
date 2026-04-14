.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;
START_JOB 0
MASK_WRITE_32	 0x46a0408, 0x3, 0x2
MASK_WRITE_32	 0x46a0400, 0xffff, 0x605
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x46a0008, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x46a0008, @WRITE_data_1, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x00000005
WRITE_data_1:
	.long 0x00000006