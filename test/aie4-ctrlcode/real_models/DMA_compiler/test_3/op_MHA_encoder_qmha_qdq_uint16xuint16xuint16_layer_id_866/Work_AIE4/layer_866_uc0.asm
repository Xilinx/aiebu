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
; SHIM_PRM
APPLY_OFFSET_57	 @DMAWRITE_data_70, 1, 3
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
; SHIM_IFM_1
APPLY_OFFSET_57	 @DMAWRITE_data_71, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_72, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_73, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
; SHIM_WGT
APPLY_OFFSET_57	 @DMAWRITE_data_74, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
; SHIM_IFM_2
APPLY_OFFSET_57	 @DMAWRITE_data_75, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
; SHIM_IFM_2
APPLY_OFFSET_57	 @DMAWRITE_data_76, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_7
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_8
MASK_POLL_32	 0x109f10, 0x1f8003c, 0x0
MASK_POLL_32	 0x18570, 0x1f8003c, 0x0
REMOTE_BARRIER	 $rb0, 0x15
; RECORD TIMER STOP
END_JOB

START_JOB 1
PREEMPT	0x004b, @save, @restore , @hintmap_0
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x110010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110030, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110050, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110060, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110070, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110080, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110090, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100a0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100b0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100c0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100d0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100e0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100f0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110100, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110110, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110120, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110130, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110140, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110150, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110160, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110170, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110180, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110190, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101a0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101b0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101c0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101d0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101e0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101f0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x150000, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x150010, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x150020, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x150030, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x150040, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x150050, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x150060, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x150070, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x150080, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x150090, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x1500a0, @WRITE_data_1, 1, 0, 1
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
	 UC_DMA_BD	 0, 0x108900, @DMAWRITE_data_16, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108930, @DMAWRITE_data_17, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108960, @DMAWRITE_data_18, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108990, @DMAWRITE_data_19, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1089c0, @DMAWRITE_data_20, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1089f0, @DMAWRITE_data_21, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148000, @DMAWRITE_data_22, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148030, @DMAWRITE_data_23, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148300, @DMAWRITE_data_24, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148330, @DMAWRITE_data_25, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148600, @DMAWRITE_data_26, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148630, @DMAWRITE_data_27, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148660, @DMAWRITE_data_28, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148690, @DMAWRITE_data_29, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1486c0, @DMAWRITE_data_30, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1486f0, @DMAWRITE_data_31, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148900, @DMAWRITE_data_32, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148930, @DMAWRITE_data_33, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c00, @DMAWRITE_data_34, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c30, @DMAWRITE_data_35, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c60, @DMAWRITE_data_36, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c90, @DMAWRITE_data_37, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f00, @DMAWRITE_data_38, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f30, @DMAWRITE_data_39, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f60, @DMAWRITE_data_40, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f90, @DMAWRITE_data_41, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108fc0, @DMAWRITE_data_42, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108ff0, @DMAWRITE_data_43, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109020, @DMAWRITE_data_44, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109050, @DMAWRITE_data_45, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109080, @DMAWRITE_data_46, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1090b0, @DMAWRITE_data_47, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109200, @DMAWRITE_data_48, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109230, @DMAWRITE_data_49, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109260, @DMAWRITE_data_50, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109290, @DMAWRITE_data_51, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109500, @DMAWRITE_data_52, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109530, @DMAWRITE_data_39, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109560, @DMAWRITE_data_40, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109590, @DMAWRITE_data_41, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1095c0, @DMAWRITE_data_42, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1095f0, @DMAWRITE_data_53, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109620, @DMAWRITE_data_44, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109650, @DMAWRITE_data_45, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109680, @DMAWRITE_data_46, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1096b0, @DMAWRITE_data_47, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109800, @DMAWRITE_data_54, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109830, @DMAWRITE_data_55, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109860, @DMAWRITE_data_56, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109890, @DMAWRITE_data_57, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1098c0, @DMAWRITE_data_58, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1098f0, @DMAWRITE_data_59, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109920, @DMAWRITE_data_60, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109950, @DMAWRITE_data_61, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c00, @DMAWRITE_data_62, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c30, @DMAWRITE_data_63, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c60, @DMAWRITE_data_64, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c90, @DMAWRITE_data_65, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149200, @DMAWRITE_data_66, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149230, @DMAWRITE_data_67, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149260, @DMAWRITE_data_68, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149290, @DMAWRITE_data_69, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e1c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e1c, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e1c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e1c, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110000, @WRITE_data_1, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_70, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x18030, @DMAWRITE_data_71, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x18060, @DMAWRITE_data_72, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x18090, @DMAWRITE_data_73, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x58030, @DMAWRITE_data_74, 0x9, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x58060, @DMAWRITE_data_75, 0x9, 0, 0
UCBD_label_7:
	 UC_DMA_BD	 0, 0x58090, @DMAWRITE_data_76, 0x9, 0, 0
UCBD_label_8:
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x1855c, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x5855c, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x10000, @WRITE_data_1, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x00000004
WRITE_data_1:
	.long 0x00000000
WRITE_data_2:
	.long 0x00000002
WRITE_data_3:
	.long 0x00000001
DMAWRITE_data_0:
	.long 0x00804420
	.long 0xfe000800
	.long 0x1e100bcb
	.long 0x00000020
	.long 0x00000020
	.long 0x00000080
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00200400
	.long 0x0c004000
DMAWRITE_data_1:
	.long 0x00806420
	.long 0xfe000800
	.long 0x1e600bd5
	.long 0x00000020
	.long 0x00000020
	.long 0x00000080
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00200400
	.long 0x00004000
DMAWRITE_data_2:
	.long 0x00804c20
	.long 0xfe000800
	.long 0x1e200bcb
	.long 0x00000020
	.long 0x00000020
	.long 0x00000080
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00200400
	.long 0x0c004000
DMAWRITE_data_3:
	.long 0x00806c20
	.long 0xfe000800
	.long 0x1e700bd5
	.long 0x00000020
	.long 0x00000020
	.long 0x00000080
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00200400
	.long 0x00004000
DMAWRITE_data_4:
	.long 0x00800000
	.long 0xf8000400
	.long 0x1c100b85
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
	.long 0x1c000b87
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
	.long 0x1c000b89
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
	.long 0x1c000b8b
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_8:
	.long 0x00800420
	.long 0xf8002000
	.long 0x1d700bb1
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
	.long 0x1c000bb3
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
	.long 0x1c000bb5
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
	.long 0x1c000bb7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x44000000
DMAWRITE_data_12:
	.long 0x00802420
	.long 0xf8002000
	.long 0x1dc00bbb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x4c000000
DMAWRITE_data_13:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bbd
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
	.long 0x1c000bbf
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
	.long 0x1c000bc1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_16:
	.long 0x00808420
	.long 0xfc0009a0
	.long 0x1cb00b9d
	.long 0x00000020
	.long 0x00001000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x02680400
	.long 0x0c004000
DMAWRITE_data_17:
	.long 0x00800000
	.long 0xfa000000
	.long 0x1ce00b9f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_18:
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
	.long 0x1c000000
DMAWRITE_data_19:
	.long 0x0080a420
	.long 0xfc0009a0
	.long 0x1d100ba9
	.long 0x00000020
	.long 0x00001000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x02680400
	.long 0x24004000
DMAWRITE_data_20:
	.long 0x00800000
	.long 0xfa000000
	.long 0x1d400bab
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
DMAWRITE_data_21:
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
	.long 0x00000000
DMAWRITE_data_22:
	.long 0x00805420
	.long 0xfe000800
	.long 0x1e300bcb
	.long 0x00000020
	.long 0x00000020
	.long 0x00000080
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00200400
	.long 0x0c004000
DMAWRITE_data_23:
	.long 0x00807420
	.long 0xfe000800
	.long 0x1e800bd5
	.long 0x00000020
	.long 0x00000020
	.long 0x00000080
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00200400
	.long 0x00004000
DMAWRITE_data_24:
	.long 0x00805c20
	.long 0xfe000800
	.long 0x1e400bcb
	.long 0x00000020
	.long 0x00000020
	.long 0x00000080
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00200400
	.long 0x0c004000
DMAWRITE_data_25:
	.long 0x00807c20
	.long 0xfe000800
	.long 0x1e900bd5
	.long 0x00000020
	.long 0x00000020
	.long 0x00000080
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00200400
	.long 0x00004000
DMAWRITE_data_26:
	.long 0x00800400
	.long 0xf8000020
	.long 0x1c600b8f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_27:
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
	.long 0x14000000
DMAWRITE_data_28:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000b93
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_29:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000b95
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_30:
	.long 0x00809420
	.long 0xfc0004e0
	.long 0x1cc00b9d
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x01000020
	.long 0x2c09c000
DMAWRITE_data_31:
	.long 0x0080b420
	.long 0xfc0004e0
	.long 0x1d200ba9
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x01000020
	.long 0x0009c000
DMAWRITE_data_32:
	.long 0x00809447
	.long 0xfc0004c0
	.long 0x1cd00b9d
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x01000020
	.long 0x0c098000
DMAWRITE_data_33:
	.long 0x0080b447
	.long 0xfc0004c0
	.long 0x1d300ba9
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x01000020
	.long 0x00098000
DMAWRITE_data_34:
	.long 0x00800000
	.long 0xfe000100
	.long 0x1c200b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_35:
	.long 0x00800400
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
DMAWRITE_data_36:
	.long 0x00800420
	.long 0xfe000800
	.long 0x1d800baf
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x1c008000
DMAWRITE_data_37:
	.long 0x00802420
	.long 0xfe000800
	.long 0x1dd00bb9
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x00008000
DMAWRITE_data_38:
	.long 0x00808420
	.long 0xfe000800
	.long 0x1cf00381
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x0c008000
DMAWRITE_data_39:
	.long 0x00808c20
	.long 0x00000800
	.long 0x00000000
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x14008000
DMAWRITE_data_40:
	.long 0x00809420
	.long 0x00000c00
	.long 0x1c000b97
	.long 0x00000080
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x01000400
	.long 0x1c00c000
DMAWRITE_data_41:
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
	.long 0x24000000
DMAWRITE_data_42:
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
	.long 0x2c000000
DMAWRITE_data_43:
	.long 0x0080a420
	.long 0xfe000800
	.long 0x1d500381
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x34008000
DMAWRITE_data_44:
	.long 0x0080ac20
	.long 0x00000800
	.long 0x00000000
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x3c008000
DMAWRITE_data_45:
	.long 0x0080b420
	.long 0x00000c00
	.long 0x1c000ba3
	.long 0x00000080
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x01000400
	.long 0x4400c000
DMAWRITE_data_46:
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
	.long 0x4c000000
DMAWRITE_data_47:
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
	.long 0x00000000
DMAWRITE_data_48:
	.long 0x00800100
	.long 0xfe000100
	.long 0x1c300b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_49:
	.long 0x00800400
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
DMAWRITE_data_50:
	.long 0x00800c20
	.long 0xfe000800
	.long 0x1d900baf
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x1c008000
DMAWRITE_data_51:
	.long 0x00802c20
	.long 0xfe000800
	.long 0x1de00bb9
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x00008000
DMAWRITE_data_52:
	.long 0x00808420
	.long 0xfe000800
	.long 0x1d000381
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x0c008000
DMAWRITE_data_53:
	.long 0x0080a420
	.long 0xfe000800
	.long 0x1d600381
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x34008000
DMAWRITE_data_54:
	.long 0x00804420
	.long 0xf8002000
	.long 0x1e500bc3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_55:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bc5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_56:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bc7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_57:
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
	.long 0x24000000
DMAWRITE_data_58:
	.long 0x00806420
	.long 0xf8002000
	.long 0x1ea00bcd
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
DMAWRITE_data_59:
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
	.long 0x34000000
DMAWRITE_data_60:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bd1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x3c000000
DMAWRITE_data_61:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bd3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_62:
	.long 0x00800200
	.long 0xfe000100
	.long 0x1c400b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_63:
	.long 0x00800400
	.long 0xfe000020
	.long 0x1c900b8d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_64:
	.long 0x00801420
	.long 0xfe000800
	.long 0x1da00baf
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x1c008000
DMAWRITE_data_65:
	.long 0x00803420
	.long 0xfe000800
	.long 0x1df00bb9
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x00008000
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
	.long 0x00800400
	.long 0xfe000020
	.long 0x1ca00b8d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_68:
	.long 0x00801c20
	.long 0xfe000800
	.long 0x1db00baf
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x1c008000
DMAWRITE_data_69:
	.long 0x00803c20
	.long 0xfe000800
	.long 0x1e000bb9
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x00008000
WRITE_data_4:
	.long 0x00020000
WRITE_data_5:
	.long 0x00000003
WRITE_data_6:
	.long 0x00020004
WRITE_data_7:
	.long 0x00000005
WRITE_data_8:
	.long 0x00000008
WRITE_data_9:
	.long 0x00020002
DMAWRITE_data_70:
	.long 0x00000000
    .long 0x007a4000
	.long 0x00000400
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_71:
	.long 0x00000000
    .long 0x05cab000
	.long 0x000009a0
	.long 0x03001001
	.long 0x4d000000
	.long 0x000009a0
	.long 0x00000000
	.long 0x218009a0
	.long 0x00000400
DMAWRITE_data_72:
	.long 0x00000000
    .long 0x03da6600
	.long 0x00002000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x21802000
	.long 0x00000400
DMAWRITE_data_73:
	.long 0x00000000
    .long 0x01be5900
	.long 0x00002000
	.long 0x03100000
	.long 0x01000000
	.long 0x00000280
	.long 0x00000001
	.long 0x21800020
	.long 0x00000400
DMAWRITE_data_74:
	.long 0x00000000
    .long 0x2933b100
	.long 0x00000020
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_75:
	.long 0x00000000
    .long 0x05dcbc00
	.long 0x000004e0
	.long 0x03001000
	.long 0x27000000
	.long 0x000004e0
	.long 0x00000001
	.long 0x218009a0
	.long 0x00000400
DMAWRITE_data_76:
	.long 0x00000000
    .long 0x05dccf80
	.long 0x000004c0
	.long 0x03001000
	.long 0x26000000
	.long 0x000004c0
	.long 0x00000001
	.long 0x218009a0
	.long 0x00000400
WRITE_data_10:
	.long 0x00060003
WRITE_data_11:
	.long 0x00060001
WRITE_data_12:
	.long 0x00060002
hintmap_0:
	.long 0x00000000
