;.partition	 3column
;
;text
;
START_JOB 0
.section annotation
id: 10
name: start_restore_1c2
description: start restore 1c2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
APPLY_OFFSET_57	 @DMAWRITE_data_0, 1, 65535, @preempt_buffer2
APPLY_OFFSET_57	 @DMAWRITE_data_1, 1, 65535, @preempt_buffer2
APPLY_OFFSET_57	 @DMAWRITE_data_2, 1, 65535, @preempt_buffer2
APPLY_OFFSET_57	 @DMAWRITE_data_3, 1, 65535, @preempt_buffer2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
MASK_POLL_32	 0x4109ed0, 0x1f8003c, 0x0
MASK_POLL_32	 0x4109ed4, 0x1f8003c, 0x0
MASK_POLL_32	 0x4149ed0, 0x1f8003c, 0x0
MASK_POLL_32	 0x4149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
END_JOB

START_JOB 1
  LOAD_LAST_PDI
END_JOB
.section annotation
id: 11
name: end_restore_1c2
description: end restore 1c2

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x4120000, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x412005c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008020, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008040, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120004, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120060, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008024, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008044, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160000, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x416005c, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048020, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048040, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160004, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160060, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048024, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048044, @WRITE_data_15, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x4018000, @DMAWRITE_data_0, 0x9, 0, 1
	 UC_DMA_BD	 0, 0x4018030, @DMAWRITE_data_1, 0x9, 0, 1
	 UC_DMA_BD	 0, 0x4058000, @DMAWRITE_data_2, 0x9, 0, 1
	 UC_DMA_BD	 0, 0x4058030, @DMAWRITE_data_3, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x4108000, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4018554, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e04, @WRITE_data_17, 1, 0, 1
	 UC_DMA_BD	 0, 0x4108300, @DMAWRITE_data_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x401855c, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e0c, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x4148000, @DMAWRITE_data_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4058554, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e04, @WRITE_data_21, 1, 0, 1
	 UC_DMA_BD	 0, 0x4148300, @DMAWRITE_data_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x405855c, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e0c, @WRITE_data_23, 1, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x4120000, @WRITE_data_24, 1, 0, 1
	 UC_DMA_BD	 0, 0x412005c, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008020, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008040, @WRITE_data_27, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120004, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120060, @WRITE_data_29, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008024, @WRITE_data_30, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008044, @WRITE_data_31, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160000, @WRITE_data_32, 1, 0, 1
	 UC_DMA_BD	 0, 0x416005c, @WRITE_data_33, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048020, @WRITE_data_34, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048040, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160004, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160060, @WRITE_data_37, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048024, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048044, @WRITE_data_39, 1, 0, 0
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
WRITE_data_16:
	.long 0x00000000
WRITE_data_17:
	.long 0x00000000
DMAWRITE_data_5:
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
WRITE_data_18:
	.long 0x00000001
WRITE_data_19:
	.long 0x00000000
DMAWRITE_data_6:
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
WRITE_data_20:
	.long 0x00000010
WRITE_data_21:
	.long 0x00000000
DMAWRITE_data_7:
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
WRITE_data_22:
	.long 0x00000011
WRITE_data_23:
	.long 0x00000000
WRITE_data_24:
	.long 0x00000000
WRITE_data_25:
	.long 0x00000000
WRITE_data_26:
	.long 0x00000000
WRITE_data_27:
	.long 0x00000000
WRITE_data_28:
	.long 0x00000000
WRITE_data_29:
	.long 0x00000000
WRITE_data_30:
	.long 0x00000000
WRITE_data_31:
	.long 0x00000000
WRITE_data_32:
	.long 0x00000000
WRITE_data_33:
	.long 0x00000000
WRITE_data_34:
	.long 0x00000000
WRITE_data_35:
	.long 0x00000000
WRITE_data_36:
	.long 0x00000000
WRITE_data_37:
	.long 0x00000000
WRITE_data_38:
	.long 0x00000000
WRITE_data_39:
	.long 0x00000000
