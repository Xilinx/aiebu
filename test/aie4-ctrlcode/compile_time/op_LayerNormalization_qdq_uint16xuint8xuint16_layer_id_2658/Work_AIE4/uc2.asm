.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;
.attach_to_group	 2
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
APPLY_OFFSET_57	 @DMAWRITE_data_62, 1, 3
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
APPLY_OFFSET_57	 @DMAWRITE_data_63, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
APPLY_OFFSET_57	 @DMAWRITE_data_64, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
APPLY_OFFSET_57	 @DMAWRITE_data_65, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
MASK_POLL_32	 0x2109f10, 0x1f8003c, 0x0
MASK_POLL_32	 0x2018570, 0x1f8003c, 0x0
REMOTE_BARRIER	 $rb0, 0x15
END_JOB

START_JOB 1
PREEMPT	0x9a21, @save, @restore , @hintmap_0
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
	 UC_DMA_BD	 0, 0x2108060, @DMAWRITE_data_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108090, @DMAWRITE_data_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21080c0, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108300, @DMAWRITE_data_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108330, @DMAWRITE_data_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108360, @DMAWRITE_data_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108390, @DMAWRITE_data_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108600, @DMAWRITE_data_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108630, @DMAWRITE_data_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108660, @DMAWRITE_data_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108690, @DMAWRITE_data_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21086c0, @DMAWRITE_data_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21086f0, @DMAWRITE_data_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108720, @DMAWRITE_data_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108750, @DMAWRITE_data_16, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108780, @DMAWRITE_data_17, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21087b0, @DMAWRITE_data_18, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21087e0, @DMAWRITE_data_19, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148000, @DMAWRITE_data_20, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148030, @DMAWRITE_data_21, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148060, @DMAWRITE_data_22, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148090, @DMAWRITE_data_23, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148300, @DMAWRITE_data_24, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148330, @DMAWRITE_data_25, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148360, @DMAWRITE_data_26, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148390, @DMAWRITE_data_27, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c00, @DMAWRITE_data_28, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c30, @DMAWRITE_data_29, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c60, @DMAWRITE_data_30, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c90, @DMAWRITE_data_31, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108cc0, @DMAWRITE_data_32, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108cf0, @DMAWRITE_data_33, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108d20, @DMAWRITE_data_34, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109200, @DMAWRITE_data_35, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109230, @DMAWRITE_data_36, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109260, @DMAWRITE_data_37, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109290, @DMAWRITE_data_38, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21092c0, @DMAWRITE_data_39, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21092f0, @DMAWRITE_data_40, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109320, @DMAWRITE_data_41, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109800, @DMAWRITE_data_42, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109830, @DMAWRITE_data_43, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109860, @DMAWRITE_data_44, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109890, @DMAWRITE_data_45, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21098c0, @DMAWRITE_data_46, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c00, @DMAWRITE_data_47, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c30, @DMAWRITE_data_48, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c60, @DMAWRITE_data_49, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c90, @DMAWRITE_data_50, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148cc0, @DMAWRITE_data_51, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148cf0, @DMAWRITE_data_52, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148d20, @DMAWRITE_data_53, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149200, @DMAWRITE_data_54, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149230, @DMAWRITE_data_55, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149260, @DMAWRITE_data_56, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149290, @DMAWRITE_data_57, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21492c0, @DMAWRITE_data_58, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21492f0, @DMAWRITE_data_59, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149320, @DMAWRITE_data_60, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149350, @DMAWRITE_data_61, 0xb, 0, 1
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
	 UC_DMA_BD	 0, 0x21100b0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100c0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100d0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100f0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110100, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110110, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110120, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110140, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110150, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110160, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110170, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110180, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110190, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101a0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101b0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101c0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101d0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101f0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150000, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150020, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150030, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150040, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150050, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150060, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150070, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150080, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150090, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21500a0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21500b0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21500c0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21500d0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x21500e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21500f0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150100, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150110, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150120, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150140, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150150, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150160, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150170, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150180, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150190, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21501a0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21501b0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e04, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e0c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e04, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e0c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e04, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e04, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e0c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e04, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e0c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e84, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e84, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e84, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110000, @WRITE_data_0, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x2018000, @DMAWRITE_data_62, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x2018030, @DMAWRITE_data_63, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x2018060, @DMAWRITE_data_64, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x2018090, @DMAWRITE_data_65, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x2018544, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x2010000, @WRITE_data_0, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x00000000
DMAWRITE_data_0:
	.long 0x008195e0
	.long 0xfe003200
	.long 0x1da00bb7
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x0c000000
DMAWRITE_data_1:
	.long 0x00825de0
	.long 0xfe003200
	.long 0x1df00bc1
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x00000000
DMAWRITE_data_2:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1e400bcb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_3:
	.long 0x008195e0
	.long 0xfe003200
	.long 0x1f200be7
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x24000000
DMAWRITE_data_4:
	.long 0x00825de0
	.long 0xfe003200
	.long 0x1f700bf1
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x00000000
DMAWRITE_data_5:
	.long 0x0081c7e0
	.long 0xfe003200
	.long 0x1db00bb9
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x0c000000
DMAWRITE_data_6:
	.long 0x00828fe0
	.long 0xfe003200
	.long 0x1e000bc3
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x00000000
DMAWRITE_data_7:
	.long 0x0081c7e0
	.long 0xfe003200
	.long 0x1f300be9
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x1c000000
DMAWRITE_data_8:
	.long 0x00828fe0
	.long 0xfe003200
	.long 0x1f800bf3
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x00000000
DMAWRITE_data_9:
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
DMAWRITE_data_10:
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
DMAWRITE_data_11:
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
DMAWRITE_data_12:
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
DMAWRITE_data_13:
	.long 0x0080c800
	.long 0xfe0000e0
	.long 0x1c600b8f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_14:
	.long 0x008190e0
	.long 0xfe000500
	.long 0x1cb00b99
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_15:
	.long 0x00800000
	.long 0xfe00c800
	.long 0x1d000ba3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x3c000000
DMAWRITE_data_16:
	.long 0x0080c8e0
	.long 0xfe00c800
	.long 0x1d500bad
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_17:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1e600bcf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_18:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1e800bd3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x54000000
DMAWRITE_data_19:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1ed00bdd
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_20:
	.long 0x0081f9e0
	.long 0xfe003200
	.long 0x1dc00bbb
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x0c000000
DMAWRITE_data_21:
	.long 0x0082c1e0
	.long 0xfe003200
	.long 0x1e100bc5
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x00000000
DMAWRITE_data_22:
	.long 0x0081f9e0
	.long 0xfe003200
	.long 0x1f400beb
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x1c000000
DMAWRITE_data_23:
	.long 0x0082c1e0
	.long 0xfe003200
	.long 0x1f900bf5
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x00000000
DMAWRITE_data_24:
	.long 0x00822be0
	.long 0xfe003200
	.long 0x1dd00bbd
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x0c000000
DMAWRITE_data_25:
	.long 0x0082f3e0
	.long 0xfe003200
	.long 0x1e200bc7
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x00000000
DMAWRITE_data_26:
	.long 0x00822be0
	.long 0xfe003200
	.long 0x1f500bed
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x1c000000
DMAWRITE_data_27:
	.long 0x0082f3e0
	.long 0xfe003200
	.long 0x1fa00bf7
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x00000000
DMAWRITE_data_28:
	.long 0x00800000
	.long 0xfe000100
	.long 0x1c200b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_29:
	.long 0x0080c800
	.long 0xfe0000e0
	.long 0x1c700b91
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_30:
	.long 0x008190e0
	.long 0xfe000500
	.long 0x1cc00b9b
	.long 0x00000280
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00100200
	.long 0x00000000
DMAWRITE_data_31:
	.long 0x00800000
	.long 0xfe003200
	.long 0x1d100ba5
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x24000000
DMAWRITE_data_32:
	.long 0x0080c8e0
	.long 0xfe003200
	.long 0x1d600baf
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x00000000
DMAWRITE_data_33:
	.long 0x00800000
	.long 0xfe003200
	.long 0x1e900bd5
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x34000000
DMAWRITE_data_34:
	.long 0x0080c8e0
	.long 0xfe003200
	.long 0x1ee00bdf
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x00000000
DMAWRITE_data_35:
	.long 0x00800100
	.long 0xfe000100
	.long 0x1c300b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_36:
	.long 0x0080c800
	.long 0xfe0000e0
	.long 0x1c800b93
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_37:
	.long 0x008190e0
	.long 0xfe000500
	.long 0x1cd00b9d
	.long 0x00000280
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00100200
	.long 0x00000000
DMAWRITE_data_38:
	.long 0x00803200
	.long 0xfe003200
	.long 0x1d200ba7
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x24000000
DMAWRITE_data_39:
	.long 0x0080fae0
	.long 0xfe003200
	.long 0x1d700bb1
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x00000000
DMAWRITE_data_40:
	.long 0x00803200
	.long 0xfe003200
	.long 0x1ea00bd7
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x34000000
DMAWRITE_data_41:
	.long 0x0080fae0
	.long 0xfe003200
	.long 0x1ef00be1
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x00000000
DMAWRITE_data_42:
	.long 0x008195e0
	.long 0xfe00c800
	.long 0x1de00bb5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_43:
	.long 0x00825de0
	.long 0xfe00c800
	.long 0x1e300bbf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_44:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1e500bc9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_45:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1f600be5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_46:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1fb00bef
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_47:
	.long 0x00800200
	.long 0xfe000100
	.long 0x1c400b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_48:
	.long 0x0080c800
	.long 0xfe0000e0
	.long 0x1c900b95
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_49:
	.long 0x008190e0
	.long 0xfe000500
	.long 0x1ce00b9f
	.long 0x00000280
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00100200
	.long 0x00000000
DMAWRITE_data_50:
	.long 0x00806400
	.long 0xfe003200
	.long 0x1d300ba9
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x24000000
DMAWRITE_data_51:
	.long 0x00812ce0
	.long 0xfe003200
	.long 0x1d800bb3
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x00000000
DMAWRITE_data_52:
	.long 0x00806400
	.long 0xfe003200
	.long 0x1eb00bd9
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x34000000
DMAWRITE_data_53:
	.long 0x00812ce0
	.long 0xfe003200
	.long 0x1f000be3
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x00000000
DMAWRITE_data_54:
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
DMAWRITE_data_55:
	.long 0x0080c800
	.long 0xfe0000e0
	.long 0x1ca00b8d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_56:
	.long 0x008190e0
	.long 0xfe000500
	.long 0x1cf00b97
	.long 0x00000280
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00100200
	.long 0x00000000
DMAWRITE_data_57:
	.long 0x00809600
	.long 0xfe003200
	.long 0x1d400ba1
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x24000000
DMAWRITE_data_58:
	.long 0x00815ee0
	.long 0xfe003200
	.long 0x1d900bab
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x00000000
DMAWRITE_data_59:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1e700bcd
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_60:
	.long 0x00809600
	.long 0xfe003200
	.long 0x1ec00bd1
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x3c000000
DMAWRITE_data_61:
	.long 0x00815ee0
	.long 0xfe003200
	.long 0x1f100bdb
	.long 0x00000280
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00a00400
	.long 0x00000000
WRITE_data_1:
	.long 0x00000004
WRITE_data_2:
	.long 0x00000001
WRITE_data_3:
	.long 0x00000005
WRITE_data_4:
	.long 0x00000007
WRITE_data_5:
	.long 0x00010008
WRITE_data_6:
	.long 0x0000000a
WRITE_data_7:
	.long 0x00010002
WRITE_data_8:
	.long 0x00000003
WRITE_data_9:
	.long 0x00000002
WRITE_data_10:
	.long 0x00010005
WRITE_data_11:
	.long 0x00000006
DMAWRITE_data_62:
	.long 0x00000000
	.long 0x01528000
	.long 0x00000400
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_63:
	.long 0x00000000
	.long 0x7006da80
	.long 0x000005e0
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_64:
	.long 0x00000000
	.long 0x00a7a000
	.long 0x0000c800
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_65:
	.long 0x00000000
	.long 0x02a07000
	.long 0x0000c800
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
hintmap_0:
	.long 0x00000000
