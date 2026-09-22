;
;text
;
START_JOB 0
.section annotation
id: 0
name: start_restore
description: start restore
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
APPLY_OFFSET_57	 @DMAWRITE_data_0, 1, 65535, @preempt_buffer
APPLY_OFFSET_57	 @DMAWRITE_data_1, 1, 65535, @preempt_buffer
APPLY_OFFSET_57	 @DMAWRITE_data_2, 1, 65535, @preempt_buffer
APPLY_OFFSET_57	 @DMAWRITE_data_3, 1, 65535, @preempt_buffer
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
MASK_POLL_32	 0x109ed0, 0x1f8003c, 0x0
MASK_POLL_32	 0x109ed4, 0x1f8003c, 0x0
MASK_POLL_32	 0x149ed0, 0x1f8003c, 0x0
MASK_POLL_32	 0x149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
END_JOB
.section annotation
id: 1
name: end_restore
description: end restore

START_JOB 1
  LOAD_LAST_PDI
END_JOB

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
	 UC_DMA_BD	 0, 0x48044, @WRITE_data_15, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_0, 0x9, 0, 1
	 UC_DMA_BD	 0, 0x18030, @DMAWRITE_data_1, 0x9, 0, 1
	 UC_DMA_BD	 0, 0x58000, @DMAWRITE_data_2, 0x9, 0, 1
	 UC_DMA_BD	 0, 0x58030, @DMAWRITE_data_3, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x108000, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108030, @DMAWRITE_data_4_1, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108060, @DMAWRITE_data_4_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108090, @DMAWRITE_data_4_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1080c0, @DMAWRITE_data_4_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1080f0, @DMAWRITE_data_4_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108120, @DMAWRITE_data_4_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108150, @DMAWRITE_data_4_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108180, @DMAWRITE_data_4_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1081b0, @DMAWRITE_data_4_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1081e0, @DMAWRITE_data_4_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108210, @DMAWRITE_data_4_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108240, @DMAWRITE_data_4_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108270, @DMAWRITE_data_4_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1082a0, @DMAWRITE_data_4_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1082d0, @DMAWRITE_data_4_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_17, 1, 0, 1
	 UC_DMA_BD	 0, 0x108300, @DMAWRITE_data_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108330, @DMAWRITE_data_5_1, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108360, @DMAWRITE_data_5_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108390, @DMAWRITE_data_5_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1083c0, @DMAWRITE_data_5_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1083f0, @DMAWRITE_data_5_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108420, @DMAWRITE_data_5_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108450, @DMAWRITE_data_5_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108480, @DMAWRITE_data_5_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1084b0, @DMAWRITE_data_5_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1084e0, @DMAWRITE_data_5_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108510, @DMAWRITE_data_5_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108540, @DMAWRITE_data_5_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108570, @DMAWRITE_data_5_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1085a0, @DMAWRITE_data_5_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1085d0, @DMAWRITE_data_5_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1855c, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x148000, @DMAWRITE_data_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148030, @DMAWRITE_data_6_1, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148060, @DMAWRITE_data_6_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148090, @DMAWRITE_data_6_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1480c0, @DMAWRITE_data_6_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1480f0, @DMAWRITE_data_6_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148120, @DMAWRITE_data_6_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148150, @DMAWRITE_data_6_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148180, @DMAWRITE_data_6_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1481b0, @DMAWRITE_data_6_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1481e0, @DMAWRITE_data_6_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148210, @DMAWRITE_data_6_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148240, @DMAWRITE_data_6_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148270, @DMAWRITE_data_6_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1482a0, @DMAWRITE_data_6_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1482d0, @DMAWRITE_data_6_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_21, 1, 0, 1
	 UC_DMA_BD	 0, 0x148300, @DMAWRITE_data_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148330, @DMAWRITE_data_7_1, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148360, @DMAWRITE_data_7_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148390, @DMAWRITE_data_7_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1483c0, @DMAWRITE_data_7_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1483f0, @DMAWRITE_data_7_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148420, @DMAWRITE_data_7_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148450, @DMAWRITE_data_7_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148480, @DMAWRITE_data_7_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1484b0, @DMAWRITE_data_7_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1484e0, @DMAWRITE_data_7_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148510, @DMAWRITE_data_7_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148540, @DMAWRITE_data_7_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148570, @DMAWRITE_data_7_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1485a0, @DMAWRITE_data_7_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1485d0, @DMAWRITE_data_7_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x5855c, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_23, 1, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x120000, @WRITE_data_24, 1, 0, 1
	 UC_DMA_BD	 0, 0x12005c, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x8020, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x8040, @WRITE_data_27, 1, 0, 1
	 UC_DMA_BD	 0, 0x120004, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x120060, @WRITE_data_29, 1, 0, 1
	 UC_DMA_BD	 0, 0x8024, @WRITE_data_30, 1, 0, 1
	 UC_DMA_BD	 0, 0x8044, @WRITE_data_31, 1, 0, 1
	 UC_DMA_BD	 0, 0x160000, @WRITE_data_32, 1, 0, 1
	 UC_DMA_BD	 0, 0x16005c, @WRITE_data_33, 1, 0, 1
	 UC_DMA_BD	 0, 0x48020, @WRITE_data_34, 1, 0, 1
	 UC_DMA_BD	 0, 0x48040, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x160004, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x160060, @WRITE_data_37, 1, 0, 1
	 UC_DMA_BD	 0, 0x48024, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x48044, @WRITE_data_39, 1, 0, 0
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
DMAWRITE_data_4_1:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4_2:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4_3:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4_4:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4_5:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4_6:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4_7:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4_8:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4_9:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4_10:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4_11:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4_12:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4_13:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4_14:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4_15:
	.long 0x00000000
	.long 0x00000000
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
DMAWRITE_data_5_1:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_5_2:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_5_3:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_5_4:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_5_5:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_5_6:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_5_7:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_5_8:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_5_9:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_5_10:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_5_11:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_5_12:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_5_13:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_5_14:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_5_15:
	.long 0x00000000
	.long 0x00000000
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
DMAWRITE_data_6_1:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6_2:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6_3:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6_4:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6_5:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6_6:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6_7:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6_8:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6_9:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6_10:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6_11:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6_12:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6_13:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6_14:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6_15:
	.long 0x00000000
	.long 0x00000000
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
DMAWRITE_data_7_1:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7_2:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7_3:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7_4:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7_5:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7_6:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7_7:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7_8:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7_9:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7_10:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7_11:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7_12:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7_13:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7_14:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7_15:
	.long 0x00000000
	.long 0x00000000
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
