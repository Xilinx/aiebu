;
;text
;
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
MASK_WRITE_32	 0x26a3000, 0x2, 0x2
MASK_WRITE_32	 0x26a3000, 0x2, 0x0
MASK_WRITE_32	 0x26a3060, 0x1, 0x1
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x26b400c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4160, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x216002c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120040, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x416002c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120040, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120018, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120044, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120000, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160070, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120000, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x412005c, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120004, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120060, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4028, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4104, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008024, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008040, @WRITE_data_17, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008020, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008040, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008000, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008068, @WRITE_data_21, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4008, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4130, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4034, @WRITE_data_24, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4130, @WRITE_data_25, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x80000018
WRITE_data_1:
	.long 0x80000000
WRITE_data_2:
	.long 0x80000000
WRITE_data_3:
	.long 0x80000000
WRITE_data_4:
	.long 0x80000000
WRITE_data_5:
	.long 0x80000000
WRITE_data_6:
	.long 0x80000001
WRITE_data_7:
	.long 0x80000000
WRITE_data_8:
	.long 0x8000001a
WRITE_data_9:
	.long 0x80000000
WRITE_data_10:
	.long 0x8000000e
WRITE_data_11:
	.long 0x80000000
WRITE_data_12:
	.long 0x8000000f
WRITE_data_13:
	.long 0x80000000
WRITE_data_14:
	.long 0x80000001
WRITE_data_15:
	.long 0x80000000
WRITE_data_16:
	.long 0x80000000
WRITE_data_17:
	.long 0x80000000
WRITE_data_18:
	.long 0x80000000
WRITE_data_19:
	.long 0x80000000
WRITE_data_20:
	.long 0x80000013
WRITE_data_21:
	.long 0x80000000
WRITE_data_22:
	.long 0x8000000c
WRITE_data_23:
	.long 0x80000000
WRITE_data_24:
	.long 0x8000000c
WRITE_data_25:
	.long 0x80000000
