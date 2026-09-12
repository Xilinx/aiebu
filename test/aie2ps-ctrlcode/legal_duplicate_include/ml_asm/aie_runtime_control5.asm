.target	 aie2ps
.aie_row_topology	 1-2-4-0
.partition	 24column
;
;text
;
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
MASK_POLL_32	 0x1400fc04, 0x1, 0x1
MASK_POLL_32	 0x1400fe18, 0x1, 0x1
APPLY_OFFSET_57	 @DMAWRITE_data_0, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
MASK_POLL_32	 0x1400f804, 0x1, 0x1
MASK_POLL_32	 0x1400fa18, 0x1, 0x1
MASK_POLL_32	 0x1400fdfc, 0x1, 0x1
MASK_POLL_32	 0x1400fe00, 0x1, 0x1
MASK_POLL_32	 0x1400f9fc, 0x1, 0x1
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x2e000000, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2e000010, @WRITE_data_0, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x2e009000, @DMAWRITE_data_0, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x2e00931c, @WRITE_data_0, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x00000000
DMAWRITE_data_0:
	.long 0x00004080
	.long 0x00018300
	.long 0x00000000
	.long 0x2b000000
	.long 0xc0c002af
	.long 0x000080ff
	.long 0x00000000
	.long 0x02001001
	.long 0x00000000
