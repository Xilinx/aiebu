.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;
.attach_to_group	 2
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
APPLY_OFFSET_57	 @DMAWRITE_data_55, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
APPLY_OFFSET_57	 @DMAWRITE_data_56, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
APPLY_OFFSET_57	 @DMAWRITE_data_57, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
APPLY_OFFSET_57	 @DMAWRITE_data_58, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
APPLY_OFFSET_57	 @DMAWRITE_data_59, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
APPLY_OFFSET_57	 @DMAWRITE_data_60, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
APPLY_OFFSET_57	 @DMAWRITE_data_61, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_7
APPLY_OFFSET_57	 @DMAWRITE_data_62, 1, 3
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_8
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_9
MASK_POLL_32	 0x2109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_10
MASK_POLL_32	 0x2109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_11
MASK_POLL_32	 0x2149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_12
MASK_POLL_32	 0x2149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_13
MASK_POLL_32	 0x2109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_14
MASK_POLL_32	 0x2109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_15
MASK_POLL_32	 0x2109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_16
MASK_POLL_32	 0x2149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_17
MASK_POLL_32	 0x2149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_18
MASK_POLL_32	 0x2109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_19
MASK_POLL_32	 0x2109f10, 0x1f8003c, 0x0
MASK_POLL_32	 0x2018570, 0x1f8003c, 0x0
REMOTE_BARRIER	 $rb0, 0x15
END_JOB

START_JOB 1
PREEMPT	0x5ee7, @save, @restore , @hintmap_0
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x2109f70, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109f78, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109f80, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149f70, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149f78, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149f80, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149f74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149f7c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2108000, @DMAWRITE_data_0, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108030, @DMAWRITE_data_1, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108300, @DMAWRITE_data_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108330, @DMAWRITE_data_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108600, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108630, @DMAWRITE_data_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108660, @DMAWRITE_data_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108690, @DMAWRITE_data_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21086c0, @DMAWRITE_data_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21086c0, @DMAWRITE_data_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21086f0, @DMAWRITE_data_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108720, @DMAWRITE_data_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108750, @DMAWRITE_data_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108780, @DMAWRITE_data_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21087b0, @DMAWRITE_data_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21087e0, @DMAWRITE_data_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108810, @DMAWRITE_data_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108840, @DMAWRITE_data_16, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108900, @DMAWRITE_data_17, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108930, @DMAWRITE_data_18, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108960, @DMAWRITE_data_19, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108990, @DMAWRITE_data_20, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148000, @DMAWRITE_data_21, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148030, @DMAWRITE_data_22, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148300, @DMAWRITE_data_23, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148330, @DMAWRITE_data_24, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148600, @DMAWRITE_data_25, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c00, @DMAWRITE_data_26, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c30, @DMAWRITE_data_27, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c60, @DMAWRITE_data_28, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c90, @DMAWRITE_data_29, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109200, @DMAWRITE_data_30, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109230, @DMAWRITE_data_31, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109260, @DMAWRITE_data_32, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109290, @DMAWRITE_data_33, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109800, @DMAWRITE_data_34, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109830, @DMAWRITE_data_35, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109860, @DMAWRITE_data_36, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109890, @DMAWRITE_data_37, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21098c0, @DMAWRITE_data_38, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21098f0, @DMAWRITE_data_39, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109920, @DMAWRITE_data_40, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109950, @DMAWRITE_data_41, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c00, @DMAWRITE_data_42, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c30, @DMAWRITE_data_43, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c60, @DMAWRITE_data_44, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c90, @DMAWRITE_data_45, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148f00, @DMAWRITE_data_46, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148f30, @DMAWRITE_data_47, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149200, @DMAWRITE_data_48, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149230, @DMAWRITE_data_49, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149260, @DMAWRITE_data_50, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149260, @DMAWRITE_data_50, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149290, @DMAWRITE_data_51, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21492c0, @DMAWRITE_data_52, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149500, @DMAWRITE_data_53, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149530, @DMAWRITE_data_54, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2110010, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110020, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110030, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110050, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110060, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110070, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110080, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110090, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100a0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100b0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100c0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100d0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100e0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100f0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110100, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110110, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110120, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110130, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110140, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110150, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110160, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110170, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110180, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110190, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101a0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101b0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101b0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101c0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101c0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101d0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101f0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150000, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150020, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150030, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150050, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150060, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e14, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e1c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e04, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e0c, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e04, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e0c, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e6c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e7c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e84, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110000, @WRITE_data_0, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x2018000, @DMAWRITE_data_55, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x2018030, @DMAWRITE_data_56, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x2018060, @DMAWRITE_data_57, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x2018090, @DMAWRITE_data_58, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x20180c0, @DMAWRITE_data_59, 0x9, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x20180f0, @DMAWRITE_data_60, 0x9, 0, 0
UCBD_label_7:
	 UC_DMA_BD	 0, 0x2018120, @DMAWRITE_data_61, 0x9, 0, 0
UCBD_label_8:
	 UC_DMA_BD	 0, 0x2058000, @DMAWRITE_data_62, 0x9, 0, 0
UCBD_label_9:
	 UC_DMA_BD	 0, 0x2018544, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2018544, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x2018544, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x2058554, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x201855c, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_17, 1, 0, 1
	 UC_DMA_BD	 0, 0x2010000, @WRITE_data_0, 1, 0, 0
UCBD_label_10:
	 UC_DMA_BD	 0, 0x2109e04, @WRITE_data_5, 1, 0, 0
UCBD_label_11:
	 UC_DMA_BD	 0, 0x2109e0c, @WRITE_data_5, 1, 0, 0
UCBD_label_12:
	 UC_DMA_BD	 0, 0x2149e04, @WRITE_data_5, 1, 0, 0
UCBD_label_13:
	 UC_DMA_BD	 0, 0x2149e0c, @WRITE_data_5, 1, 0, 0
UCBD_label_14:
	 UC_DMA_BD	 0, 0x2109e84, @WRITE_data_5, 1, 0, 0
UCBD_label_15:
	 UC_DMA_BD	 0, 0x2109e04, @WRITE_data_5, 1, 0, 0
UCBD_label_16:
	 UC_DMA_BD	 0, 0x2109e0c, @WRITE_data_5, 1, 0, 0
UCBD_label_17:
	 UC_DMA_BD	 0, 0x2149e04, @WRITE_data_5, 1, 0, 0
UCBD_label_18:
	 UC_DMA_BD	 0, 0x2149e0c, @WRITE_data_5, 1, 0, 0
UCBD_label_19:
	 UC_DMA_BD	 0, 0x2109e84, @WRITE_data_5, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x00000000
DMAWRITE_data_0:
	.long 0x00813dc0
	.long 0xfe000400
	.long 0x1d100bab
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_1:
	.long 0x00814dc0
	.long 0xfe000400
	.long 0x1d600bb5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_2:
	.long 0x008141c0
	.long 0xfe000400
	.long 0x1d200bab
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_3:
	.long 0x008151c0
	.long 0xfe000400
	.long 0x1d700bb5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4:
	.long 0x00800400
	.long 0xb000f000
	.long 0x1c60538f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_5:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c005391
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_6:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c005393
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_7:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c005395
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_8:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1db00bb9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_9:
	.long 0x00800400
	.long 0xb000f000
	.long 0x1dd053bd
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x34000000
DMAWRITE_data_10:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c0053bf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x3c000000
DMAWRITE_data_11:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c0053c1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x44000000
DMAWRITE_data_12:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c0053c3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_13:
	.long 0x00800400
	.long 0xb000c800
	.long 0x1e2053c7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x54000000
DMAWRITE_data_14:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c0053c9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x5c000000
DMAWRITE_data_15:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c0053cb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x64000000
DMAWRITE_data_16:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c0053cd
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_17:
	.long 0x0080f400
	.long 0xfc0024e0
	.long 0x1cb00b99
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_18:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000b9b
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_19:
	.long 0x008118e0
	.long 0xfc0024e0
	.long 0x1ce00b9f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_20:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000ba1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_21:
	.long 0x008145c0
	.long 0xfe000400
	.long 0x1d300bab
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_22:
	.long 0x008155c0
	.long 0xfe000400
	.long 0x1d800bb5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_23:
	.long 0x008149c0
	.long 0xfe000400
	.long 0x1d400bab
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_24:
	.long 0x008159c0
	.long 0xfe000400
	.long 0x1d900bb5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_25:
	.long 0x00800000
	.long 0xfe000400
	.long 0x1c100b85
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_26:
	.long 0x00800000
	.long 0xfe000100
	.long 0x1c200b87
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_27:
	.long 0x00800400
	.long 0xfe007f80
	.long 0x1c700b8d
	.long 0x00000140
	.long 0x00002800
	.long 0x00000020
	.long 0x00000001
	.long 0x00000000
	.long 0x00000101
	.long 0x01000400
	.long 0x0000c000
DMAWRITE_data_28:
	.long 0x00800400
	.long 0xfe007f80
	.long 0x1de00bbb
	.long 0x00000140
	.long 0x00002800
	.long 0x00000020
	.long 0x00000001
	.long 0x00000000
	.long 0x00000101
	.long 0x01000400
	.long 0x0000c000
DMAWRITE_data_29:
	.long 0x00800400
	.long 0xfe007f80
	.long 0x1e300bc5
	.long 0x00000140
	.long 0x00002800
	.long 0x00000020
	.long 0x00000001
	.long 0x00000000
	.long 0x00000101
	.long 0x01000400
	.long 0x0000c000
DMAWRITE_data_30:
	.long 0x00800100
	.long 0xfe000100
	.long 0x1c300b89
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_31:
	.long 0x00802c00
	.long 0xfe007f80
	.long 0x1c800b8d
	.long 0x00000140
	.long 0x00002800
	.long 0x00000020
	.long 0x00000001
	.long 0x00000000
	.long 0x00000101
	.long 0x01000400
	.long 0x0000c000
DMAWRITE_data_32:
	.long 0x00802c00
	.long 0xfe007f80
	.long 0x1df00bbb
	.long 0x00000140
	.long 0x00002800
	.long 0x00000020
	.long 0x00000001
	.long 0x00000000
	.long 0x00000101
	.long 0x01000400
	.long 0x0000c000
DMAWRITE_data_33:
	.long 0x00802c00
	.long 0xfe007f80
	.long 0x1e400bc5
	.long 0x00000140
	.long 0x00002800
	.long 0x00000020
	.long 0x00000001
	.long 0x00000000
	.long 0x00000101
	.long 0x01000400
	.long 0x0000c000
DMAWRITE_data_34:
	.long 0x00813dc0
	.long 0xf8001000
	.long 0x1d500ba3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_35:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000ba5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_36:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000ba7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_37:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000ba9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_38:
	.long 0x00814dc0
	.long 0xf8001000
	.long 0x1da00bad
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
DMAWRITE_data_39:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000baf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x34000000
DMAWRITE_data_40:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bb1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x3c000000
DMAWRITE_data_41:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bb3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_42:
	.long 0x00800200
	.long 0xfe000100
	.long 0x1c400b8b
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_43:
	.long 0x00805400
	.long 0xfe007f80
	.long 0x1c900b8d
	.long 0x00000140
	.long 0x00002800
	.long 0x00000020
	.long 0x00000001
	.long 0x00000000
	.long 0x00000101
	.long 0x01000400
	.long 0x0000c000
DMAWRITE_data_44:
	.long 0x00805400
	.long 0xfe007f80
	.long 0x1e000bbb
	.long 0x00000140
	.long 0x00002800
	.long 0x00000020
	.long 0x00000001
	.long 0x00000000
	.long 0x00000101
	.long 0x01000400
	.long 0x0000c000
DMAWRITE_data_45:
	.long 0x00805400
	.long 0xfe007f80
	.long 0x1e500bc5
	.long 0x00000140
	.long 0x00002800
	.long 0x00000020
	.long 0x00000001
	.long 0x00000000
	.long 0x00000101
	.long 0x01000400
	.long 0x0000c000
DMAWRITE_data_46:
	.long 0x0080f400
	.long 0xfe0024e0
	.long 0x1cc00b97
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_47:
	.long 0x008118e0
	.long 0xfe0024e0
	.long 0x1cf00b9d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_48:
	.long 0x00800300
	.long 0xfe000100
	.long 0x1c500b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_49:
	.long 0x00807c00
	.long 0xfe007f80
	.long 0x1ca00b8d
	.long 0x00000140
	.long 0x00002800
	.long 0x00000020
	.long 0x00000001
	.long 0x00000000
	.long 0x00000101
	.long 0x01000400
	.long 0x0000c000
DMAWRITE_data_50:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1dc00bb7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_51:
	.long 0x00807c00
	.long 0xfe007f80
	.long 0x1e100bbb
	.long 0x00000140
	.long 0x00002800
	.long 0x00000020
	.long 0x00000001
	.long 0x00000000
	.long 0x00000101
	.long 0x01000400
	.long 0x0000c000
DMAWRITE_data_52:
	.long 0x00807c00
	.long 0xfe007f80
	.long 0x1e600bc5
	.long 0x00000140
	.long 0x00002800
	.long 0x00000020
	.long 0x00000001
	.long 0x00000000
	.long 0x01000101
	.long 0x01000400
	.long 0x00008000
DMAWRITE_data_53:
	.long 0x0080f400
	.long 0xfe0024e0
	.long 0x1cd00b97
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_54:
	.long 0x008118e0
	.long 0xfe0024e0
	.long 0x1d000b9d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
WRITE_data_1:
	.long 0x00000001
WRITE_data_2:
	.long 0x00000028
WRITE_data_3:
	.long 0x00000002
WRITE_data_4:
	.long 0x00950000
WRITE_data_5:
	.long 0x00040000
WRITE_data_6:
	.long 0x00010004
WRITE_data_7:
	.long 0x00000005
WRITE_data_8:
	.long 0x00000009
WRITE_data_9:
	.long 0x00090001
WRITE_data_10:
	.long 0x00010002
WRITE_data_11:
	.long 0x00090002
WRITE_data_12:
	.long 0x00090003
WRITE_data_13:
	.long 0x00090004
DMAWRITE_data_55:
	.long 0x00000000
	.long 0x02949000
	.long 0x0000f000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_56:
	.long 0x00000000
	.long 0x9f8b1c84
	.long 0x000e6780
	.long 0x0300400a
	.long 0x49c00000
	.long 0x00000938
	.long 0x000170c0
	.long 0x20000001
	.long 0x00938000
DMAWRITE_data_57:
	.long 0x00000000
	.long 0x02a93000
	.long 0x0000a000
	.long 0x03080000
	.long 0x01000000
	.long 0x00000140
	.long 0x00000020
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_58:
	.long 0x00000000
	.long 0x029c1000
	.long 0x0000f000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_59:
	.long 0x00000000
	.long 0x02a39000
	.long 0x0000c800
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_60:
	.long 0x00000000
	.long 0x02b0b000
	.long 0x0000a000
	.long 0x03080000
	.long 0x01000000
	.long 0x00000140
	.long 0x00000020
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_61:
	.long 0x00000000
	.long 0x02b83000
	.long 0x0000a000
	.long 0x03080000
	.long 0x01000000
	.long 0x00000140
	.long 0x00000020
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_62:
	.long 0x00000000
	.long 0x01d9b000
	.long 0x00000400
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
WRITE_data_14:
	.long 0x00000006
WRITE_data_15:
	.long 0x00020001
WRITE_data_16:
	.long 0x00000003
WRITE_data_17:
	.long 0x00000004
hintmap_0:
	.long 0x00000000
