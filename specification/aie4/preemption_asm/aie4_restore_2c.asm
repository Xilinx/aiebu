;.partition	 3column
;
;text
;
START_JOB 0
.section annotation
id: 2
name: start_restore
description: start restore
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
APPLY_OFFSET_57	 @DMAWRITE_data_0, 1, 65535, @preempt_buffer
APPLY_OFFSET_57	 @DMAWRITE_data_1, 1, 65535, @preempt_buffer
APPLY_OFFSET_57	 @DMAWRITE_data_2, 1, 65535, @preempt_buffer
APPLY_OFFSET_57	 @DMAWRITE_data_3, 1, 65535, @preempt_buffer
APPLY_OFFSET_57	 @DMAWRITE_data_4, 1, 65535, @preempt_buffer
APPLY_OFFSET_57	 @DMAWRITE_data_5, 1, 65535, @preempt_buffer
APPLY_OFFSET_57	 @DMAWRITE_data_6, 1, 65535, @preempt_buffer
APPLY_OFFSET_57	 @DMAWRITE_data_7, 1, 65535, @preempt_buffer
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
MASK_POLL_32	 0x109ed0, 0x1f8003c, 0x0
MASK_POLL_32	 0x109ed4, 0x1f8003c, 0x0
MASK_POLL_32	 0x149ed0, 0x1f8003c, 0x0
MASK_POLL_32	 0x149ed4, 0x1f8003c, 0x0
MASK_POLL_32	 0x2109ed0, 0x1f8003c, 0x0
MASK_POLL_32	 0x2109ed4, 0x1f8003c, 0x0
MASK_POLL_32	 0x2149ed0, 0x1f8003c, 0x0
MASK_POLL_32	 0x2149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
END_JOB

START_JOB 1
  LOAD_LAST_PDI
END_JOB
.section annotation
id: 3
name: end_restore
description: end restore


EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x120000, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x12005c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8020, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x8040, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x120004, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x120060, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x8024, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x8044, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x160000, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x16005c, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x48020, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x48040, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x160004, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x160060, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x48024, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x48044, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120000, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x212005c, @WRITE_data_17, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008020, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008040, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120004, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120060, @WRITE_data_21, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008024, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008044, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160000, @WRITE_data_24, 1, 0, 1
	 UC_DMA_BD	 0, 0x216005c, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048020, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048040, @WRITE_data_27, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160004, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160060, @WRITE_data_29, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048024, @WRITE_data_30, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048044, @WRITE_data_31, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_0, 0x9, 0, 1
	 UC_DMA_BD	 0, 0x18030, @DMAWRITE_data_1, 0x9, 0, 1
	 UC_DMA_BD	 0, 0x58000, @DMAWRITE_data_2, 0x9, 0, 1
	 UC_DMA_BD	 0, 0x58030, @DMAWRITE_data_3, 0x9, 0, 1
	 UC_DMA_BD	 0, 0x2018000, @DMAWRITE_data_4, 0x9, 0, 1
	 UC_DMA_BD	 0, 0x2018030, @DMAWRITE_data_5, 0x9, 0, 1
	 UC_DMA_BD	 0, 0x2058000, @DMAWRITE_data_6, 0x9, 0, 1
	 UC_DMA_BD	 0, 0x2058030, @DMAWRITE_data_7, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x108000, @DMAWRITE_data_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_32, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_33, 1, 0, 1
	 UC_DMA_BD	 0, 0x108300, @DMAWRITE_data_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1855c, @WRITE_data_34, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x148000, @DMAWRITE_data_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_37, 1, 0, 1
	 UC_DMA_BD	 0, 0x148300, @DMAWRITE_data_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x5855c, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x2108000, @DMAWRITE_data_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_40, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e04, @WRITE_data_41, 1, 0, 1
	 UC_DMA_BD	 0, 0x2108300, @DMAWRITE_data_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x201855c, @WRITE_data_42, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e0c, @WRITE_data_43, 1, 0, 1
	 UC_DMA_BD	 0, 0x2148000, @DMAWRITE_data_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2058554, @WRITE_data_44, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e04, @WRITE_data_45, 1, 0, 1
	 UC_DMA_BD	 0, 0x2148300, @DMAWRITE_data_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x205855c, @WRITE_data_46, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e0c, @WRITE_data_47, 1, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x120000, @WRITE_data_48, 1, 0, 1
	 UC_DMA_BD	 0, 0x12005c, @WRITE_data_49, 1, 0, 1
	 UC_DMA_BD	 0, 0x8020, @WRITE_data_50, 1, 0, 1
	 UC_DMA_BD	 0, 0x8040, @WRITE_data_51, 1, 0, 1
	 UC_DMA_BD	 0, 0x120004, @WRITE_data_52, 1, 0, 1
	 UC_DMA_BD	 0, 0x120060, @WRITE_data_53, 1, 0, 1
	 UC_DMA_BD	 0, 0x8024, @WRITE_data_54, 1, 0, 1
	 UC_DMA_BD	 0, 0x8044, @WRITE_data_55, 1, 0, 1
	 UC_DMA_BD	 0, 0x160000, @WRITE_data_56, 1, 0, 1
	 UC_DMA_BD	 0, 0x16005c, @WRITE_data_57, 1, 0, 1
	 UC_DMA_BD	 0, 0x48020, @WRITE_data_58, 1, 0, 1
	 UC_DMA_BD	 0, 0x48040, @WRITE_data_59, 1, 0, 1
	 UC_DMA_BD	 0, 0x160004, @WRITE_data_60, 1, 0, 1
	 UC_DMA_BD	 0, 0x160060, @WRITE_data_61, 1, 0, 1
	 UC_DMA_BD	 0, 0x48024, @WRITE_data_62, 1, 0, 1
	 UC_DMA_BD	 0, 0x48044, @WRITE_data_63, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120000, @WRITE_data_64, 1, 0, 1
	 UC_DMA_BD	 0, 0x212005c, @WRITE_data_65, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008020, @WRITE_data_66, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008040, @WRITE_data_67, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120004, @WRITE_data_68, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120060, @WRITE_data_69, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008024, @WRITE_data_70, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008044, @WRITE_data_71, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160000, @WRITE_data_72, 1, 0, 1
	 UC_DMA_BD	 0, 0x216005c, @WRITE_data_73, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048020, @WRITE_data_74, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048040, @WRITE_data_75, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160004, @WRITE_data_76, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160060, @WRITE_data_77, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048024, @WRITE_data_78, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048044, @WRITE_data_79, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x8000000e
WRITE_data_1:
	.long 0x80000000
WRITE_data_2:
	.long 0x80000000
WRITE_data_3:
	.long 0x80000000
WRITE_data_4:
	.long 0x8000000f
WRITE_data_5:
	.long 0x80000000
WRITE_data_6:
	.long 0x80000001
WRITE_data_7:
	.long 0x80000000
WRITE_data_8:
	.long 0x80000012
WRITE_data_9:
	.long 0x80000000
WRITE_data_10:
	.long 0x80000002
WRITE_data_11:
	.long 0x80000000
WRITE_data_12:
	.long 0x80000013
WRITE_data_13:
	.long 0x80000000
WRITE_data_14:
	.long 0x80000003
WRITE_data_15:
	.long 0x80000000
WRITE_data_16:
	.long 0x8000000e
WRITE_data_17:
	.long 0x80000000
WRITE_data_18:
	.long 0x80000000
WRITE_data_19:
	.long 0x80000000
WRITE_data_20:
	.long 0x8000000f
WRITE_data_21:
	.long 0x80000000
WRITE_data_22:
	.long 0x80000001
WRITE_data_23:
	.long 0x80000000
WRITE_data_24:
	.long 0x80000012
WRITE_data_25:
	.long 0x80000000
WRITE_data_26:
	.long 0x80000002
WRITE_data_27:
	.long 0x80000000
WRITE_data_28:
	.long 0x80000013
WRITE_data_29:
	.long 0x80000000
WRITE_data_30:
	.long 0x80000003
WRITE_data_31:
	.long 0x80000000
DMAWRITE_data_0:
	.long 0x00000000
	.long 0x00000000
	.long 0x00030000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_1:
	.long 0x00000000
	.long 0x000c0000
	.long 0x00030000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_2:
	.long 0x00000000
	.long 0x00180000
	.long 0x00030000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_3:
	.long 0x00000000
	.long 0x00240000
	.long 0x00030000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_4:
	.long 0x00000000
	.long 0x00300000
	.long 0x00030000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_5:
	.long 0x00000000
	.long 0x003c0000
	.long 0x00030000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_6:
	.long 0x00000000
	.long 0x00480000
	.long 0x00030000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_7:
	.long 0x00000000
	.long 0x00540000
	.long 0x00030000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_8:
	.long 0x00800000
	.long 0x00030000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
WRITE_data_32:
	.long 0x00000000
WRITE_data_33:
	.long 0x00000000
DMAWRITE_data_9:
	.long 0x00830000
	.long 0x00030000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
WRITE_data_34:
	.long 0x00000001
WRITE_data_35:
	.long 0x00000000
DMAWRITE_data_10:
	.long 0x00860000
	.long 0x00030000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
WRITE_data_36:
	.long 0x00000010
WRITE_data_37:
	.long 0x00000000
DMAWRITE_data_11:
	.long 0x00890000
	.long 0x00030000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
WRITE_data_38:
	.long 0x00000011
WRITE_data_39:
	.long 0x00000000
DMAWRITE_data_12:
	.long 0x00800000
	.long 0x00030000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
WRITE_data_40:
	.long 0x00000000
WRITE_data_41:
	.long 0x00000000
DMAWRITE_data_13:
	.long 0x00830000
	.long 0x00030000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
WRITE_data_42:
	.long 0x00000001
WRITE_data_43:
	.long 0x00000000
DMAWRITE_data_14:
	.long 0x00860000
	.long 0x00030000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
WRITE_data_44:
	.long 0x00000010
WRITE_data_45:
	.long 0x00000000
DMAWRITE_data_15:
	.long 0x00890000
	.long 0x00030000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
WRITE_data_46:
	.long 0x00000011
WRITE_data_47:
	.long 0x00000000
WRITE_data_48:
	.long 0x00000000
WRITE_data_49:
	.long 0x00000000
WRITE_data_50:
	.long 0x00000000
WRITE_data_51:
	.long 0x00000000
WRITE_data_52:
	.long 0x00000000
WRITE_data_53:
	.long 0x00000000
WRITE_data_54:
	.long 0x00000000
WRITE_data_55:
	.long 0x00000000
WRITE_data_56:
	.long 0x00000000
WRITE_data_57:
	.long 0x00000000
WRITE_data_58:
	.long 0x00000000
WRITE_data_59:
	.long 0x00000000
WRITE_data_60:
	.long 0x00000000
WRITE_data_61:
	.long 0x00000000
WRITE_data_62:
	.long 0x00000000
WRITE_data_63:
	.long 0x00000000
WRITE_data_64:
	.long 0x00000000
WRITE_data_65:
	.long 0x00000000
WRITE_data_66:
	.long 0x00000000
WRITE_data_67:
	.long 0x00000000
WRITE_data_68:
	.long 0x00000000
WRITE_data_69:
	.long 0x00000000
WRITE_data_70:
	.long 0x00000000
WRITE_data_71:
	.long 0x00000000
WRITE_data_72:
	.long 0x00000000
WRITE_data_73:
	.long 0x00000000
WRITE_data_74:
	.long 0x00000000
WRITE_data_75:
	.long 0x00000000
WRITE_data_76:
	.long 0x00000000
WRITE_data_77:
	.long 0x00000000
WRITE_data_78:
	.long 0x00000000
WRITE_data_79:
	.long 0x00000000
