;
;text
;
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
MASK_WRITE_32	 0x6a3000, 0x2, 0x2
MASK_WRITE_32	 0x6a3000, 0x2, 0x0
MASK_WRITE_32	 0x26a3000, 0x2, 0x2
MASK_WRITE_32	 0x26a3000, 0x2, 0x0
MASK_WRITE_32	 0x6a3060, 0x1, 0x1
MASK_WRITE_32	 0x26a3060, 0x1, 0x1
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x26b4034, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b415c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b400c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4160, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b400c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4160, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x16002c, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x120040, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160028, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120040, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x160028, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x120044, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120018, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120044, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x416002c, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120044, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x120004, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x160070, @WRITE_data_17, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120000, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160070, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x120000, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x12005c, @WRITE_data_21, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120000, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x412005c, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4028, @WRITE_data_24, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4104, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4028, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4104, @WRITE_data_27, 1, 0, 1
	 UC_DMA_BD	 0, 0x8000, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x8070, @WRITE_data_29, 1, 0, 1
	 UC_DMA_BD	 0, 0x8020, @WRITE_data_30, 1, 0, 1
	 UC_DMA_BD	 0, 0x8040, @WRITE_data_31, 1, 0, 1
	 UC_DMA_BD	 0, 0x8030, @WRITE_data_32, 1, 0, 1
	 UC_DMA_BD	 0, 0x8044, @WRITE_data_33, 1, 0, 1
	 UC_DMA_BD	 0, 0x200801c, @WRITE_data_34, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008068, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008030, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008060, @WRITE_data_37, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008020, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008060, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4008, @WRITE_data_40, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b412c, @WRITE_data_41, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4030, @WRITE_data_42, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b412c, @WRITE_data_43, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4064, @WRITE_data_44, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4130, @WRITE_data_45, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4034, @WRITE_data_46, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4130, @WRITE_data_47, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4008, @WRITE_data_48, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4138, @WRITE_data_49, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x80000017
WRITE_data_1:
	.long 0x80000000
WRITE_data_2:
	.long 0x80000018
WRITE_data_3:
	.long 0x80000000
WRITE_data_4:
	.long 0x80000018
WRITE_data_5:
	.long 0x80000000
WRITE_data_6:
	.long 0x80000000
WRITE_data_7:
	.long 0x80000000
WRITE_data_8:
	.long 0x80000000
WRITE_data_9:
	.long 0x80000000
WRITE_data_10:
	.long 0x80000001
WRITE_data_11:
	.long 0x80000000
WRITE_data_12:
	.long 0x80000001
WRITE_data_13:
	.long 0x80000000
WRITE_data_14:
	.long 0x80000001
WRITE_data_15:
	.long 0x80000000
WRITE_data_16:
	.long 0x8000001a
WRITE_data_17:
	.long 0x80000000
WRITE_data_18:
	.long 0x8000001a
WRITE_data_19:
	.long 0x80000000
WRITE_data_20:
	.long 0x8000000e
WRITE_data_21:
	.long 0x80000000
WRITE_data_22:
	.long 0x8000000e
WRITE_data_23:
	.long 0x80000000
WRITE_data_24:
	.long 0x80000001
WRITE_data_25:
	.long 0x80000000
WRITE_data_26:
	.long 0x80000001
WRITE_data_27:
	.long 0x80000000
WRITE_data_28:
	.long 0x80000017
WRITE_data_29:
	.long 0x80000000
WRITE_data_30:
	.long 0x80000000
WRITE_data_31:
	.long 0x80000000
WRITE_data_32:
	.long 0x80000001
WRITE_data_33:
	.long 0x80000000
WRITE_data_34:
	.long 0x80000013
WRITE_data_35:
	.long 0x80000000
WRITE_data_36:
	.long 0x8000000f
WRITE_data_37:
	.long 0x80000000
WRITE_data_38:
	.long 0x8000000f
WRITE_data_39:
	.long 0x80000000
WRITE_data_40:
	.long 0x8000000b
WRITE_data_41:
	.long 0x80000000
WRITE_data_42:
	.long 0x8000000b
WRITE_data_43:
	.long 0x80000000
WRITE_data_44:
	.long 0x8000000c
WRITE_data_45:
	.long 0x80000000
WRITE_data_46:
	.long 0x8000000c
WRITE_data_47:
	.long 0x80000000
WRITE_data_48:
	.long 0x8000000e
WRITE_data_49:
	.long 0x80000000
