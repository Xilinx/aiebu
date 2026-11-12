;
;text
;
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
MASK_WRITE_32	 0x338000, 0x2, 0x2
MASK_WRITE_32	 0x338000, 0x2, 0x0
MASK_WRITE_32	 0x4002104, 0xc00, 0x400
MASK_WRITE_32	 0x4002108, 0xc, 0x4
MASK_WRITE_32	 0x6002104, 0xc00, 0x400
MASK_WRITE_32	 0x338038, 0x1, 0x1
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x33f004, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x33f154, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x33f008, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x33f158, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x233f030, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x233f158, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x433f030, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x433f158, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x21b0040, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x21b0100, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x41b0028, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x41b0100, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x61b0040, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x61b0100, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x21b0000, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x21b0140, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x22b0028, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x22b0140, @WRITE_data_17, 1, 0, 1
	 UC_DMA_BD	 0, 0x1b0000, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x1b0130, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x22b0040, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x22b0130, @WRITE_data_21, 1, 0, 1
	 UC_DMA_BD	 0, 0x61b0000, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x61b0130, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x62b0040, @WRITE_data_24, 1, 0, 1
	 UC_DMA_BD	 0, 0x62b0130, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x33f058, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x33f104, @WRITE_data_27, 1, 0, 1
	 UC_DMA_BD	 0, 0x3f044, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x3f154, @WRITE_data_29, 1, 0, 1
	 UC_DMA_BD	 0, 0x203f02c, @WRITE_data_30, 1, 0, 1
	 UC_DMA_BD	 0, 0x203f154, @WRITE_data_31, 1, 0, 1
	 UC_DMA_BD	 0, 0x403f00c, @WRITE_data_32, 1, 0, 1
	 UC_DMA_BD	 0, 0x403f144, @WRITE_data_33, 1, 0, 1
	 UC_DMA_BD	 0, 0x403f02c, @WRITE_data_34, 1, 0, 1
	 UC_DMA_BD	 0, 0x403f114, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x603f044, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x603f114, @WRITE_data_37, 1, 0, 1
	 UC_DMA_BD	 0, 0x233f02c, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x233f128, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x633f030, @WRITE_data_40, 1, 0, 1
	 UC_DMA_BD	 0, 0x633f128, @WRITE_data_41, 1, 0, 1
	 UC_DMA_BD	 0, 0x233f020, @WRITE_data_42, 1, 0, 1
	 UC_DMA_BD	 0, 0x233f138, @WRITE_data_43, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x80000015
WRITE_data_1:
	.long 0x80000000
WRITE_data_2:
	.long 0x80000016
WRITE_data_3:
	.long 0x80000000
WRITE_data_4:
	.long 0x80000016
WRITE_data_5:
	.long 0x80000000
WRITE_data_6:
	.long 0x80000016
WRITE_data_7:
	.long 0x80000000
WRITE_data_8:
	.long 0x80000000
WRITE_data_9:
	.long 0x80000000
WRITE_data_10:
	.long 0x80000000
WRITE_data_11:
	.long 0x80000000
WRITE_data_12:
	.long 0x80000000
WRITE_data_13:
	.long 0x80000000
WRITE_data_14:
	.long 0x80000010
WRITE_data_15:
	.long 0x80000000
WRITE_data_16:
	.long 0x80000010
WRITE_data_17:
	.long 0x80000000
WRITE_data_18:
	.long 0x8000000c
WRITE_data_19:
	.long 0x80000000
WRITE_data_20:
	.long 0x8000000c
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
WRITE_data_26:
	.long 0x80000001
WRITE_data_27:
	.long 0x80000000
WRITE_data_28:
	.long 0x80000015
WRITE_data_29:
	.long 0x80000000
WRITE_data_30:
	.long 0x80000015
WRITE_data_31:
	.long 0x80000000
WRITE_data_32:
	.long 0x80000011
WRITE_data_33:
	.long 0x80000000
WRITE_data_34:
	.long 0x80000005
WRITE_data_35:
	.long 0x80000000
WRITE_data_36:
	.long 0x80000005
WRITE_data_37:
	.long 0x80000000
WRITE_data_38:
	.long 0x8000000a
WRITE_data_39:
	.long 0x80000000
WRITE_data_40:
	.long 0x8000000a
WRITE_data_41:
	.long 0x80000000
WRITE_data_42:
	.long 0x8000000e
WRITE_data_43:
	.long 0x80000000
