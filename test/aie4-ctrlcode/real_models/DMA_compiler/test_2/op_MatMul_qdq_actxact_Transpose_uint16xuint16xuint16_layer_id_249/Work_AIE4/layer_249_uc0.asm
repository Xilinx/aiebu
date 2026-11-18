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
APPLY_OFFSET_57	 @DMAWRITE_data_73, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_74, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_75, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
; SHIM_IFM_1
APPLY_OFFSET_57	 @DMAWRITE_data_76, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
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
	 UC_DMA_BD	 0, 0x110090, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100a0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100b0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100c0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100d0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100e0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100f0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110100, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110110, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110120, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110130, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110140, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110150, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110160, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110170, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110180, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110190, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101a0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101b0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101c0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101d0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101e0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101f0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x150000, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x150010, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x150020, @WRITE_data_0, 1, 0, 1
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
	 UC_DMA_BD	 0, 0x108900, @DMAWRITE_data_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108930, @DMAWRITE_data_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148000, @DMAWRITE_data_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148030, @DMAWRITE_data_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148300, @DMAWRITE_data_16, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148330, @DMAWRITE_data_17, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148600, @DMAWRITE_data_18, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148630, @DMAWRITE_data_19, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148660, @DMAWRITE_data_20, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148690, @DMAWRITE_data_21, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148900, @DMAWRITE_data_22, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c00, @DMAWRITE_data_23, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c30, @DMAWRITE_data_24, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c60, @DMAWRITE_data_25, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c90, @DMAWRITE_data_26, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108cc0, @DMAWRITE_data_27, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108cf0, @DMAWRITE_data_28, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108d20, @DMAWRITE_data_29, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f00, @DMAWRITE_data_30, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f30, @DMAWRITE_data_31, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f60, @DMAWRITE_data_32, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f90, @DMAWRITE_data_33, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108fc0, @DMAWRITE_data_34, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108ff0, @DMAWRITE_data_35, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109020, @DMAWRITE_data_36, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109200, @DMAWRITE_data_37, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109230, @DMAWRITE_data_38, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109260, @DMAWRITE_data_39, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109290, @DMAWRITE_data_40, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1092c0, @DMAWRITE_data_41, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1092f0, @DMAWRITE_data_42, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109320, @DMAWRITE_data_43, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109500, @DMAWRITE_data_44, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109530, @DMAWRITE_data_45, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109560, @DMAWRITE_data_46, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109590, @DMAWRITE_data_47, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1095c0, @DMAWRITE_data_48, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1095f0, @DMAWRITE_data_49, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109620, @DMAWRITE_data_50, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109800, @DMAWRITE_data_51, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109830, @DMAWRITE_data_52, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109860, @DMAWRITE_data_53, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109890, @DMAWRITE_data_54, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1098c0, @DMAWRITE_data_55, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1098f0, @DMAWRITE_data_56, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109920, @DMAWRITE_data_57, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109950, @DMAWRITE_data_58, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c00, @DMAWRITE_data_59, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c30, @DMAWRITE_data_60, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c60, @DMAWRITE_data_61, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c90, @DMAWRITE_data_62, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148cc0, @DMAWRITE_data_63, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148cf0, @DMAWRITE_data_64, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148d20, @DMAWRITE_data_65, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149200, @DMAWRITE_data_66, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149230, @DMAWRITE_data_67, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149260, @DMAWRITE_data_68, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149290, @DMAWRITE_data_69, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1492c0, @DMAWRITE_data_70, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1492f0, @DMAWRITE_data_71, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149320, @DMAWRITE_data_72, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e1c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e1c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110000, @WRITE_data_0, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_73, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x18030, @DMAWRITE_data_74, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x18060, @DMAWRITE_data_75, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x58030, @DMAWRITE_data_76, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1855c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10000, @WRITE_data_0, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x00000000
WRITE_data_1:
	.long 0x00000001
WRITE_data_2:
	.long 0x00000002
WRITE_data_3:
	.long 0x00000004
DMAWRITE_data_0:
	.long 0x0081ad60
	.long 0xfe000200
	.long 0x1d900bbb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_1:
	.long 0x0081b560
	.long 0xfe000200
	.long 0x1de00bc5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_2:
	.long 0x0081b160
	.long 0xfe000200
	.long 0x1da00bbb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_3:
	.long 0x0081b960
	.long 0xfe000200
	.long 0x1df00bc5
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
	.long 0xf8004470
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
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000b99
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
	.long 0x1c000b9b
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_8:
	.long 0x00804870
	.long 0xf8004470
	.long 0x1ce00b9f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
DMAWRITE_data_9:
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
	.long 0x34000000
DMAWRITE_data_10:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000ba3
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
	.long 0x1c000ba5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_12:
	.long 0x0081bd60
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
DMAWRITE_data_13:
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
DMAWRITE_data_14:
	.long 0x0081af60
	.long 0xfe000200
	.long 0x1db00bbb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_15:
	.long 0x0081b760
	.long 0xfe000200
	.long 0x1e000bc5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_16:
	.long 0x0081b360
	.long 0xfe000200
	.long 0x1dc00bbb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_17:
	.long 0x0081bb60
	.long 0xfe000200
	.long 0x1e100bc5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_18:
	.long 0x00808ce0
	.long 0xfc009040
	.long 0x1d300ba9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_19:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bab
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_20:
	.long 0x00811d20
	.long 0xfc009040
	.long 0x1d600baf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_21:
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
	.long 0x00000000
DMAWRITE_data_22:
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
DMAWRITE_data_23:
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
DMAWRITE_data_24:
	.long 0x00800400
	.long 0xfe000a00
	.long 0x1ca00381
	.long 0x00000124
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00800400
	.long 0x14000000
DMAWRITE_data_25:
	.long 0x008004a0
	.long 0x00000800
	.long 0x00000000
	.long 0x00000124
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00800400
	.long 0x1c000000
DMAWRITE_data_26:
	.long 0x00800520
	.long 0x00000200
	.long 0x1c000b93
	.long 0x00000124
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201c00
	.long 0x00000000
	.long 0x00000080
	.long 0x24000000
DMAWRITE_data_27:
	.long 0x00804870
	.long 0xfe000a00
	.long 0x1cf00381
	.long 0x00000124
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00800400
	.long 0x2c000000
DMAWRITE_data_28:
	.long 0x00804910
	.long 0x00000800
	.long 0x00000000
	.long 0x00000124
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00800400
	.long 0x34000000
DMAWRITE_data_29:
	.long 0x00804990
	.long 0x00000200
	.long 0x1c000b9d
	.long 0x00000124
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201c00
	.long 0x00000000
	.long 0x00000080
	.long 0x00000000
DMAWRITE_data_30:
	.long 0x0081bd60
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
	.long 0x00808ce0
	.long 0xfe002800
	.long 0x1d400381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_32:
	.long 0x0080b4e0
	.long 0x00002000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_33:
	.long 0x0080d4e0
	.long 0x00000800
	.long 0x1c000ba7
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00003f00
	.long 0x00080400
	.long 0x24000000
DMAWRITE_data_34:
	.long 0x00811d20
	.long 0xfe002800
	.long 0x1d700381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
DMAWRITE_data_35:
	.long 0x00814520
	.long 0x00002000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x34000000
DMAWRITE_data_36:
	.long 0x00816520
	.long 0x00000800
	.long 0x1c000bad
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00003f00
	.long 0x00080400
	.long 0x00000000
DMAWRITE_data_37:
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
DMAWRITE_data_38:
	.long 0x00802638
	.long 0xfe000a00
	.long 0x1cb00381
	.long 0x00000124
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00800400
	.long 0x14000000
DMAWRITE_data_39:
	.long 0x008026d8
	.long 0x00000800
	.long 0x00000000
	.long 0x00000124
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00800400
	.long 0x1c000000
DMAWRITE_data_40:
	.long 0x00802758
	.long 0x00000200
	.long 0x1c000b93
	.long 0x00000124
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201c00
	.long 0x00000000
	.long 0x00000080
	.long 0x24000000
DMAWRITE_data_41:
	.long 0x00806aa8
	.long 0xfe000a00
	.long 0x1d000381
	.long 0x00000124
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00800400
	.long 0x2c000000
DMAWRITE_data_42:
	.long 0x00806b48
	.long 0x00000800
	.long 0x00000000
	.long 0x00000124
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00800400
	.long 0x34000000
DMAWRITE_data_43:
	.long 0x00806bc8
	.long 0x00000200
	.long 0x1c000b9d
	.long 0x00000124
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201c00
	.long 0x00000000
	.long 0x00000080
	.long 0x00000000
DMAWRITE_data_44:
	.long 0x0081bd60
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
DMAWRITE_data_45:
	.long 0x0080d500
	.long 0xfe002800
	.long 0x1d500381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_46:
	.long 0x0080fd00
	.long 0x00002000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_47:
	.long 0x00811d00
	.long 0x00000800
	.long 0x1c000ba7
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00003f00
	.long 0x00080400
	.long 0x24000000
DMAWRITE_data_48:
	.long 0x00816540
	.long 0xfe002800
	.long 0x1d800381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
DMAWRITE_data_49:
	.long 0x00818d40
	.long 0x00002000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x34000000
DMAWRITE_data_50:
	.long 0x0081ad40
	.long 0x00000800
	.long 0x1c000bad
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00003f00
	.long 0x00080400
	.long 0x00000000
DMAWRITE_data_51:
	.long 0x0081ad60
	.long 0xf8000780
	.long 0x1dd00bb3
	.long 0x00000400
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00007800
	.long 0x0c000000
DMAWRITE_data_52:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bb5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
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
	.long 0x1c000000
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
	.long 0x24000000
DMAWRITE_data_55:
	.long 0x0081b560
	.long 0xf8000780
	.long 0x1e200bbd
	.long 0x00000400
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00007800
	.long 0x2c000000
DMAWRITE_data_56:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bbf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x34000000
DMAWRITE_data_57:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bc1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x3c000000
DMAWRITE_data_58:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bc3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_59:
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
DMAWRITE_data_60:
	.long 0x00801640
	.long 0xfe000a00
	.long 0x1cc00381
	.long 0x00000124
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000200
	.long 0x00700400
	.long 0x14000000
DMAWRITE_data_61:
	.long 0x008016e0
	.long 0x00000800
	.long 0x00000000
	.long 0x00000124
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000200
	.long 0x00700400
	.long 0x1c000000
DMAWRITE_data_62:
	.long 0x00801760
	.long 0x00000200
	.long 0x1c000b93
	.long 0x00000124
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201c00
	.long 0x00000200
	.long 0x00700080
	.long 0x24000000
DMAWRITE_data_63:
	.long 0x00805ab0
	.long 0xfe000a00
	.long 0x1d100381
	.long 0x00000124
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000200
	.long 0x00700400
	.long 0x2c000000
DMAWRITE_data_64:
	.long 0x00805b50
	.long 0x00000800
	.long 0x00000000
	.long 0x00000124
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000200
	.long 0x00700400
	.long 0x34000000
DMAWRITE_data_65:
	.long 0x00805bd0
	.long 0x00000200
	.long 0x1c000b9d
	.long 0x00000124
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201c00
	.long 0x00000200
	.long 0x00700080
	.long 0x00000000
DMAWRITE_data_66:
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
DMAWRITE_data_67:
	.long 0x00803878
	.long 0xfe000a00
	.long 0x1cd00381
	.long 0x00000124
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000200
	.long 0x00700400
	.long 0x14000000
DMAWRITE_data_68:
	.long 0x00803918
	.long 0x00000800
	.long 0x00000000
	.long 0x00000124
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000200
	.long 0x00700400
	.long 0x1c000000
DMAWRITE_data_69:
	.long 0x00803998
	.long 0x00000200
	.long 0x1c000b93
	.long 0x00000124
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201c00
	.long 0x00000200
	.long 0x00700080
	.long 0x24000000
DMAWRITE_data_70:
	.long 0x00807ce8
	.long 0xfe000a00
	.long 0x1d200381
	.long 0x00000124
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000200
	.long 0x00700400
	.long 0x2c000000
DMAWRITE_data_71:
	.long 0x00807d88
	.long 0x00000800
	.long 0x00000000
	.long 0x00000124
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000200
	.long 0x00700400
	.long 0x34000000
DMAWRITE_data_72:
	.long 0x00807e08
	.long 0x00000200
	.long 0x1c000b9d
	.long 0x00000124
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201c00
	.long 0x00000200
	.long 0x00700080
	.long 0x00000000
DMAWRITE_data_73:
	.long 0x00000000
    .long 0x0310c610
	.long 0x00000020
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_74:
	.long 0x00000000
    .long 0x005a6620
	.long 0x00004470
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x2040cd50
	.long 0x00000400
DMAWRITE_data_75:
	.long 0x00000000
    .long 0x0067f3a0
	.long 0x00000f00
	.long 0x0301e002
	.long 0x01000000
	.long 0x00000180
	.long 0x00000020
	.long 0x20000001
	.long 0x00030000
DMAWRITE_data_76:
	.long 0x00000000
    .long 0x016b01a0
	.long 0x00012080
	.long 0x03241002
	.long 0x01000000
	.long 0x00000180
	.long 0x00000020
	.long 0x20000001
	.long 0x00030000
WRITE_data_4:
	.long 0x00010001
hintmap_0:
	.long 0x00000000
