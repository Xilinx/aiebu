.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
MASK_WRITE_32	 0x6a3000, 0x2, 0x2
MASK_WRITE_32	 0x6a3000, 0x2, 0x0
MASK_WRITE_32	 0x7a3000, 0x2, 0x2
MASK_WRITE_32	 0x7a3000, 0x2, 0x0
MASK_WRITE_32	 0x6a7120, 0xffff, 0x0
MASK_WRITE_32	 0x7a7120, 0xffff, 0x1
MASK_WRITE_32	 0x6a3060, 0x1, 0x1
MASK_WRITE_32	 0x7a3060, 0x1, 0x1
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x16002c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x120040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x160028, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x120044, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x160024, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x120048, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x160020, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x12004c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x160018, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x120050, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x120004, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x16006c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x120000, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x160070, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x120008, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x120060, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x16000c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x16005c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x160008, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x160060, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4028, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4104, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4028, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4104, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4024, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4154, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x8024, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x8040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x48020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8044, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x48024, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x48040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x8000, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x48068, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4058, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4124, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b405c, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4128, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4000, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4000, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4004, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4004, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4130, @WRITE_data_0, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x80000000
WRITE_data_1:
	.long 0x80000001
WRITE_data_2:
	.long 0x80000002
WRITE_data_3:
	.long 0x80000003
WRITE_data_4:
	.long 0x80000004
WRITE_data_5:
	.long 0x80000019
WRITE_data_6:
	.long 0x8000001a
WRITE_data_7:
	.long 0x8000000f
WRITE_data_8:
	.long 0x80000012
WRITE_data_9:
	.long 0x80000013
WRITE_data_10:
	.long 0x80000015
WRITE_data_11:
	.long 0x80000009
WRITE_data_12:
	.long 0x8000000a
WRITE_data_13:
	.long 0x8000000b
WRITE_data_14:
	.long 0x8000000c
