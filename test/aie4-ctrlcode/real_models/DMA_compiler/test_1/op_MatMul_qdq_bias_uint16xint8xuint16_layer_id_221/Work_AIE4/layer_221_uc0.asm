.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;
.attach_to_group	 0
; RECORD TIMER START
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
; SHIM_WGT
APPLY_OFFSET_57	 @DMAWRITE_data_60, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_61, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
; SHIM_WGT
APPLY_OFFSET_57	 @DMAWRITE_data_62, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_63, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_64, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_65, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
; SHIM_WGT
APPLY_OFFSET_57	 @DMAWRITE_data_66, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_7
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_8
MASK_POLL_32	 0x109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_9
MASK_POLL_32	 0x109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_10
MASK_POLL_32	 0x149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_11
MASK_POLL_32	 0x149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_12
MASK_POLL_32	 0x109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_13
MASK_POLL_32	 0x109f10, 0x1f8003c, 0x0
MASK_POLL_32	 0x18570, 0x1f8003c, 0x0
REMOTE_BARRIER	 $rb0, 0x15
; RECORD TIMER STOP
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x109f70, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109f78, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109f80, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x149f70, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x149f78, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x149f7c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109f74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109f7c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110010, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110020, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110030, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110050, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110060, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110070, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110080, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110090, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100a0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100b0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100c0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100d0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100f0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x110100, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110110, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110120, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x110130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110140, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110150, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110160, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110170, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110180, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110190, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101a0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101b0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101c0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101d0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101f0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101f0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x150000, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x150000, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x150010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x150010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x150020, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x150030, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x150040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x150050, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x150060, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x150070, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x150080, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x150090, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1500a0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1500b0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1500c0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1500d0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x108000, @DMAWRITE_data_0, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108030, @DMAWRITE_data_1, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108300, @DMAWRITE_data_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108330, @DMAWRITE_data_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108600, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108630, @DMAWRITE_data_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108660, @DMAWRITE_data_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108690, @DMAWRITE_data_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1086c0, @DMAWRITE_data_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1086f0, @DMAWRITE_data_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108720, @DMAWRITE_data_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108750, @DMAWRITE_data_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108780, @DMAWRITE_data_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1087b0, @DMAWRITE_data_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1087e0, @DMAWRITE_data_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108810, @DMAWRITE_data_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108840, @DMAWRITE_data_16, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108900, @DMAWRITE_data_17, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108930, @DMAWRITE_data_18, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108960, @DMAWRITE_data_19, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108990, @DMAWRITE_data_20, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148000, @DMAWRITE_data_21, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148030, @DMAWRITE_data_22, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148300, @DMAWRITE_data_23, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148330, @DMAWRITE_data_24, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148600, @DMAWRITE_data_25, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148630, @DMAWRITE_data_26, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148900, @DMAWRITE_data_27, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c00, @DMAWRITE_data_28, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c30, @DMAWRITE_data_29, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f00, @DMAWRITE_data_30, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f30, @DMAWRITE_data_31, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f60, @DMAWRITE_data_32, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f90, @DMAWRITE_data_33, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108fc0, @DMAWRITE_data_34, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108ff0, @DMAWRITE_data_35, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109020, @DMAWRITE_data_36, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109050, @DMAWRITE_data_37, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109200, @DMAWRITE_data_38, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109230, @DMAWRITE_data_39, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109500, @DMAWRITE_data_40, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109530, @DMAWRITE_data_41, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109560, @DMAWRITE_data_42, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109590, @DMAWRITE_data_43, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1095c0, @DMAWRITE_data_44, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1095f0, @DMAWRITE_data_45, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109620, @DMAWRITE_data_46, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109650, @DMAWRITE_data_47, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109800, @DMAWRITE_data_48, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109830, @DMAWRITE_data_49, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109860, @DMAWRITE_data_50, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109890, @DMAWRITE_data_51, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1098c0, @DMAWRITE_data_52, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1098f0, @DMAWRITE_data_53, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109920, @DMAWRITE_data_54, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109950, @DMAWRITE_data_55, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c00, @DMAWRITE_data_56, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c30, @DMAWRITE_data_57, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149200, @DMAWRITE_data_58, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149230, @DMAWRITE_data_59, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e1c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e1c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e1c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x110000, @WRITE_data_0, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_60, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x18030, @DMAWRITE_data_61, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x18060, @DMAWRITE_data_62, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x18090, @DMAWRITE_data_63, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x180c0, @DMAWRITE_data_64, 0x9, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x180f0, @DMAWRITE_data_65, 0x9, 0, 0
UCBD_label_7:
	 UC_DMA_BD	 0, 0x58030, @DMAWRITE_data_66, 0x9, 0, 0
UCBD_label_8:
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x1855c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1855c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x10000, @WRITE_data_0, 1, 0, 0
UCBD_label_9:
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_0, 1, 0, 0
UCBD_label_10:
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_0, 1, 0, 0
UCBD_label_11:
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_0, 1, 0, 0
UCBD_label_12:
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_0, 1, 0, 0
UCBD_label_13:
	 UC_DMA_BD	 0, 0x109800, @DMAWRITE_data_67, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1098c0, @DMAWRITE_data_68, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_0, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x00000000
WRITE_data_1:
	.long 0x00000001
WRITE_data_2:
	.long 0x00000002
WRITE_data_3:
	.long 0x00000014
DMAWRITE_data_0:
	.long 0x00860a00
	.long 0xfe000400
	.long 0x1d500bb3
	.long 0x00000040
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x0c000000
DMAWRITE_data_1:
	.long 0x00861a00
	.long 0xfe000400
	.long 0x1da00bbd
	.long 0x00000040
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_2:
	.long 0x00861200
	.long 0xfe000400
	.long 0x1d600bb3
	.long 0x00000040
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x0c000000
DMAWRITE_data_3:
	.long 0x00862200
	.long 0xfe000400
	.long 0x1db00bbd
	.long 0x00000040
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_4:
	.long 0x00800400
	.long 0xfc018000
	.long 0x1c900b95
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
	.long 0x1c000b97
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_6:
	.long 0x00818400
	.long 0xfc018000
	.long 0x1cc00b9b
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
	.long 0x1c000b9d
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
	.long 0x1df00bc1
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
	.long 0xfc018000
	.long 0x1e200bc7
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
	.long 0x1c000bc9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_11:
	.long 0x00818400
	.long 0xfc018000
	.long 0x1e500bcd
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
	.long 0x1c000bcf
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
	.long 0xfc000600
	.long 0x1e800bd3
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
	.long 0x1c000bd5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_15:
	.long 0x00818400
	.long 0xfc000600
	.long 0x1eb00bd9
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
	.long 0x1c000bdb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_17:
	.long 0x00862a00
	.long 0xfc000020
	.long 0x1c600b8f
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
	.long 0x1c000b91
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_19:
	.long 0x00830400
	.long 0xd8018300
	.long 0x1cf053a1
	.long 0x00000c18
	.long 0x00018300
	.long 0x0000c180
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00418300
	.long 0x1c008000
DMAWRITE_data_20:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c0053a3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_21:
	.long 0x00860a20
	.long 0xfe000400
	.long 0x1d700bb3
	.long 0x00000040
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x0c000000
DMAWRITE_data_22:
	.long 0x00861a20
	.long 0xfe000400
	.long 0x1dc00bbd
	.long 0x00000040
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_23:
	.long 0x00861220
	.long 0xfe000400
	.long 0x1d800bb3
	.long 0x00000040
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x0c000000
DMAWRITE_data_24:
	.long 0x00862220
	.long 0xfe000400
	.long 0x1dd00bbd
	.long 0x00000040
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_25:
	.long 0x008364c0
	.long 0xd8018300
	.long 0x1d2053a7
	.long 0x00000c18
	.long 0x00018300
	.long 0x0000c180
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00418300
	.long 0x0c008000
DMAWRITE_data_26:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c0053a9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_27:
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
DMAWRITE_data_28:
	.long 0x00800000
	.long 0xfe000100
	.long 0x1c200b87
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_29:
	.long 0x00830400
	.long 0xfe018300
	.long 0x1d000b9f
	.long 0x00000c18
	.long 0x00018300
	.long 0x0000c180
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00418300
	.long 0x00008000
DMAWRITE_data_30:
	.long 0x00862a00
	.long 0xfe000020
	.long 0x1c700b8d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_31:
	.long 0x00800400
	.long 0xfe018000
	.long 0x1ca00b93
	.long 0x00000600
	.long 0x00000020
	.long 0x00000000
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x01000400
	.long 0x140c0000
DMAWRITE_data_32:
	.long 0x00818400
	.long 0xfe018000
	.long 0x1cd00b99
	.long 0x00000600
	.long 0x00000020
	.long 0x00000000
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x01000400
	.long 0x000c0000
DMAWRITE_data_33:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1e000bc3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_34:
	.long 0x00800400
	.long 0xfe018000
	.long 0x1e300bc5
	.long 0x00000600
	.long 0x00000020
	.long 0x00000000
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x01000400
	.long 0x000c0000
DMAWRITE_data_35:
	.long 0x00818400
	.long 0xfe018000
	.long 0x1e600bcb
	.long 0x00000600
	.long 0x00000020
	.long 0x00000000
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x01000400
	.long 0x000c0000
DMAWRITE_data_36:
	.long 0x00800400
	.long 0xfe018000
	.long 0x1e900bd1
	.long 0x00000600
	.long 0x00000020
	.long 0x00000000
	.long 0x00000001
	.long 0x00210000
	.long 0x00001f00
	.long 0x00080400
	.long 0x000c0000
DMAWRITE_data_37:
	.long 0x00818400
	.long 0xfe018000
	.long 0x1ec00bd7
	.long 0x00000600
	.long 0x00000020
	.long 0x00000000
	.long 0x00000001
	.long 0x00210000
	.long 0x00001f00
	.long 0x00080400
	.long 0x000c0000
DMAWRITE_data_38:
	.long 0x00800100
	.long 0xfe000100
	.long 0x1c300b89
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_39:
	.long 0x00830400
	.long 0xfe018300
	.long 0x1d100b9f
	.long 0x00000c18
	.long 0x00018300
	.long 0x0000c180
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00418300
	.long 0x00008000
DMAWRITE_data_40:
	.long 0x00862a00
	.long 0xfe000020
	.long 0x1c800b8d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_41:
	.long 0x0080c400
	.long 0xfe018000
	.long 0x1cb00b93
	.long 0x00000600
	.long 0x00000020
	.long 0x00000000
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x01000400
	.long 0x140c0000
DMAWRITE_data_42:
	.long 0x00824400
	.long 0xfe018000
	.long 0x1ce00b99
	.long 0x00000600
	.long 0x00000020
	.long 0x00000000
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x01000400
	.long 0x000c0000
DMAWRITE_data_43:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1e100bbf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_44:
	.long 0x0080c400
	.long 0xfe018000
	.long 0x1e400bc5
	.long 0x00000600
	.long 0x00000020
	.long 0x00000000
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x01000400
	.long 0x000c0000
DMAWRITE_data_45:
	.long 0x00824400
	.long 0xfe018000
	.long 0x1e700bcb
	.long 0x00000600
	.long 0x00000020
	.long 0x00000000
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x01000400
	.long 0x000c0000
DMAWRITE_data_46:
	.long 0x00800400
	.long 0xfe018000
	.long 0x1ea00bd1
	.long 0x00000600
	.long 0x00000020
	.long 0x00000000
	.long 0x00000001
	.long 0x00210000
	.long 0x00001f00
	.long 0x00080400
	.long 0x000c0000
DMAWRITE_data_47:
	.long 0x00818400
	.long 0xfe018000
	.long 0x1ed00bd7
	.long 0x00000600
	.long 0x00000020
	.long 0x00000000
	.long 0x00000001
	.long 0x00210000
	.long 0x00001f00
	.long 0x00080400
	.long 0x000c0000
DMAWRITE_data_48:
	.long 0x00860a00
	.long 0xf8001000
	.long 0x1d900bab
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_49:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bad
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_50:
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
	.long 0x1c000000
DMAWRITE_data_51:
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
	.long 0x24000000
DMAWRITE_data_52:
	.long 0x00861a00
	.long 0xf8001000
	.long 0x1de00bb5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
DMAWRITE_data_53:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bb7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x34000000
DMAWRITE_data_54:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bb9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x3c000000
DMAWRITE_data_55:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bbb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_56:
	.long 0x00800200
	.long 0xfe000100
	.long 0x1c400b8b
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_57:
	.long 0x008364c0
	.long 0xfe018300
	.long 0x1d300ba5
	.long 0x00000c18
	.long 0x00018300
	.long 0x0000c180
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00418300
	.long 0x00008000
DMAWRITE_data_58:
	.long 0x00800300
	.long 0xfe000100
	.long 0x1c500b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_59:
	.long 0x008364c0
	.long 0xfe018300
	.long 0x1d400ba5
	.long 0x00000c18
	.long 0x00018300
	.long 0x0000c180
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00418300
	.long 0x00008000
WRITE_data_4:
	.long 0x00030000
WRITE_data_5:
	.long 0x00080000
WRITE_data_6:
	.long 0x00010004
WRITE_data_7:
	.long 0x00000005
WRITE_data_8:
	.long 0x00000009
WRITE_data_9:
	.long 0x00030001
WRITE_data_10:
	.long 0x00090001
WRITE_data_11:
	.long 0x00010003
WRITE_data_12:
	.long 0x00000004
WRITE_data_13:
	.long 0x00000006
DMAWRITE_data_60:
	.long 0x00000000
    .long 0x05975680
	.long 0x00000020
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_61:
	.long 0x00000000
    .long 0x0043aa00
	.long 0x000d8000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_62:
	.long 0x00000000
    .long 0x05730e80
	.long 0x00018300
	.long 0x03008002
	.long 0x60c00000
	.long 0x00000c18
	.long 0x00048900
	.long 0x20000001
	.long 0x09120000
DMAWRITE_data_63:
	.long 0x00000000
    .long 0x001b1800
	.long 0x00012000
	.long 0x03040002
	.long 0x02000000
	.long 0x00000180
	.long 0x000000c0
	.long 0x20000001
	.long 0x01800000
DMAWRITE_data_64:
	.long 0x00000000
    .long 0x0079aa00
	.long 0x00000600
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_65:
	.long 0x00000000
    .long 0x00289800
	.long 0x00000080
	.long 0x03000000
	.long 0x02000000
	.long 0x000000c0
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_66:
	.long 0x00000000
    .long 0x05749180
	.long 0x00018300
	.long 0x03008002
	.long 0x60c00000
	.long 0x00000c18
	.long 0x00048900
	.long 0x20000001
	.long 0x09120000
WRITE_data_14:
	.long 0x00000003
DMAWRITE_data_67:
	.long 0x00860a00
	.long 0xf8000040
	.long 0x1d900bab
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_68:
	.long 0x00861a00
	.long 0xf8000040
	.long 0x1de00bb5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
hintmap_0:
	.long 0x00000000
