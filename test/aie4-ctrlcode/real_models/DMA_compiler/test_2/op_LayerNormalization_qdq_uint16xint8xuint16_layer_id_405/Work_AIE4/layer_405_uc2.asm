.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;
.attach_to_group	 2
; RECORD TIMER START
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
; SHIM_WGT
APPLY_OFFSET_57	 @DMAWRITE_data_50, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_51, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_52, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
MASK_POLL_32	 0x2109f10, 0x1f8003c, 0x0
MASK_POLL_32	 0x2018570, 0x1f8003c, 0x0
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
	 UC_DMA_BD	 0, 0x2109f70, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109f78, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109f80, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149f70, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149f78, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149f80, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149f74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149f7c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110010, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110020, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110030, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110040, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110050, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110060, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110070, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110080, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110090, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100a0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100b0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100c0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100d0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100e0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100f0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110100, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110110, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110120, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110130, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110140, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110150, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110160, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110170, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110180, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110190, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101a0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101b0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101c0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101d0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101e0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101f0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150000, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150010, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150020, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150030, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2108000, @DMAWRITE_data_0, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108030, @DMAWRITE_data_1, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108300, @DMAWRITE_data_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108330, @DMAWRITE_data_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108600, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108630, @DMAWRITE_data_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108660, @DMAWRITE_data_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108690, @DMAWRITE_data_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21086c0, @DMAWRITE_data_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21086f0, @DMAWRITE_data_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108720, @DMAWRITE_data_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108750, @DMAWRITE_data_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108780, @DMAWRITE_data_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21087b0, @DMAWRITE_data_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148000, @DMAWRITE_data_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148030, @DMAWRITE_data_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148300, @DMAWRITE_data_16, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148330, @DMAWRITE_data_17, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148900, @DMAWRITE_data_18, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148930, @DMAWRITE_data_19, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148960, @DMAWRITE_data_20, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148990, @DMAWRITE_data_21, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c00, @DMAWRITE_data_22, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c30, @DMAWRITE_data_23, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c60, @DMAWRITE_data_24, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c90, @DMAWRITE_data_25, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108cc0, @DMAWRITE_data_26, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109200, @DMAWRITE_data_27, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109230, @DMAWRITE_data_28, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109260, @DMAWRITE_data_29, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109290, @DMAWRITE_data_30, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21092c0, @DMAWRITE_data_31, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109800, @DMAWRITE_data_32, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109830, @DMAWRITE_data_33, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109860, @DMAWRITE_data_34, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109890, @DMAWRITE_data_35, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21098c0, @DMAWRITE_data_36, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21098f0, @DMAWRITE_data_37, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109920, @DMAWRITE_data_38, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109950, @DMAWRITE_data_39, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c00, @DMAWRITE_data_40, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c30, @DMAWRITE_data_41, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c60, @DMAWRITE_data_42, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c90, @DMAWRITE_data_43, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148cc0, @DMAWRITE_data_44, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149200, @DMAWRITE_data_45, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149230, @DMAWRITE_data_46, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149260, @DMAWRITE_data_47, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149290, @DMAWRITE_data_48, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21492c0, @DMAWRITE_data_49, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149e1c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e04, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e0c, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e04, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e0c, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e84, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110000, @WRITE_data_2, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x2018000, @DMAWRITE_data_50, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x2018030, @DMAWRITE_data_51, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x2018060, @DMAWRITE_data_52, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x2018544, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2010000, @WRITE_data_2, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0xf724f724
WRITE_data_1:
	.long 0x00000004
WRITE_data_2:
	.long 0x00000000
WRITE_data_3:
	.long 0x00000001
DMAWRITE_data_0:
	.long 0x00801fe0
	.long 0xfe000300
	.long 0x1da00bbd
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_1:
	.long 0x00802be0
	.long 0xfe000300
	.long 0x1df00bc7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_2:
	.long 0x008022e0
	.long 0xfe000300
	.long 0x1db00bbd
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_3:
	.long 0x00802ee0
	.long 0xfe000300
	.long 0x1e000bc7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4:
	.long 0x00801000
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
DMAWRITE_data_5:
	.long 0x00801ce0
	.long 0xfe000300
	.long 0x1cb00b99
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6:
	.long 0x00800400
	.long 0xf8000c00
	.long 0x1d000ba3
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
	.long 0x1c000ba5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_8:
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
	.long 0x2c000000
DMAWRITE_data_9:
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
	.long 0x34000000
DMAWRITE_data_10:
	.long 0x008010e0
	.long 0xf8000c00
	.long 0x1d500bad
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
	.long 0x1c000baf
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
	.long 0x1c000bb1
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
	.long 0x1c000bb3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_14:
	.long 0x008025e0
	.long 0xfe000300
	.long 0x1dc00bbd
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_15:
	.long 0x008031e0
	.long 0xfe000300
	.long 0x1e100bc7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_16:
	.long 0x008028e0
	.long 0xfe000300
	.long 0x1dd00bbd
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_17:
	.long 0x008034e0
	.long 0xfe000300
	.long 0x1e200bc7
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
DMAWRITE_data_19:
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
DMAWRITE_data_20:
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
DMAWRITE_data_21:
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
DMAWRITE_data_22:
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
DMAWRITE_data_23:
	.long 0x00801000
	.long 0xfe0000e0
	.long 0x1c700b91
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_24:
	.long 0x00801ce0
	.long 0xfe000300
	.long 0x1cc00b9b
	.long 0x00000180
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00100200
	.long 0x00000000
DMAWRITE_data_25:
	.long 0x00800400
	.long 0xfe000300
	.long 0x1d100ba1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_26:
	.long 0x008010e0
	.long 0xfe000300
	.long 0x1d600bab
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_27:
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
DMAWRITE_data_28:
	.long 0x00801000
	.long 0xfe0000e0
	.long 0x1c800b93
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_29:
	.long 0x00801ce0
	.long 0xfe000300
	.long 0x1cd00b9d
	.long 0x00000180
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00100200
	.long 0x00000000
DMAWRITE_data_30:
	.long 0x00800700
	.long 0xfe000300
	.long 0x1d200ba1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_31:
	.long 0x008013e0
	.long 0xfe000300
	.long 0x1d700bab
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_32:
	.long 0x00801fe0
	.long 0xf8000c00
	.long 0x1de00bb5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_33:
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
	.long 0x14000000
DMAWRITE_data_34:
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
	.long 0x1c000000
DMAWRITE_data_35:
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
	.long 0x24000000
DMAWRITE_data_36:
	.long 0x00802be0
	.long 0xf8000c00
	.long 0x1e300bbf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
DMAWRITE_data_37:
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
	.long 0x34000000
DMAWRITE_data_38:
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
	.long 0x3c000000
DMAWRITE_data_39:
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
	.long 0x00000000
DMAWRITE_data_40:
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
DMAWRITE_data_41:
	.long 0x00801000
	.long 0xfe0000e0
	.long 0x1c900b95
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_42:
	.long 0x00801ce0
	.long 0xfe000300
	.long 0x1ce00b9f
	.long 0x00000180
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00100200
	.long 0x00000000
DMAWRITE_data_43:
	.long 0x00800a00
	.long 0xfe000300
	.long 0x1d300ba1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_44:
	.long 0x008016e0
	.long 0xfe000300
	.long 0x1d800bab
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_45:
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
DMAWRITE_data_46:
	.long 0x00801000
	.long 0xfe0000e0
	.long 0x1ca00b8d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_47:
	.long 0x00801ce0
	.long 0xfe000300
	.long 0x1cf00b97
	.long 0x00000180
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00100200
	.long 0x00000000
DMAWRITE_data_48:
	.long 0x00800d00
	.long 0xfe000300
	.long 0x1d400ba1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_49:
	.long 0x008019e0
	.long 0xfe000300
	.long 0x1d900bab
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
WRITE_data_4:
	.long 0x00000006
WRITE_data_5:
	.long 0x00000002
DMAWRITE_data_50:
	.long 0x00000000
    .long 0x056a3a90
	.long 0x000003e0
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_51:
	.long 0x00000000
    .long 0x005aba20
	.long 0x00000c00
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_52:
	.long 0x00000000
    .long 0x005a0620
	.long 0x00000c00
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
hintmap_0:
	.long 0x00000000
