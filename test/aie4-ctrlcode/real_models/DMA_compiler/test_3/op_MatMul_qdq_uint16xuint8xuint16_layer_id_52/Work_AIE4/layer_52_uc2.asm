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
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
; SHIM_WGT
APPLY_OFFSET_57	 @DMAWRITE_data_65, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_66, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
; SHIM_WGT
APPLY_OFFSET_57	 @DMAWRITE_data_67, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_68, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
; SHIM_WGT
APPLY_OFFSET_57	 @DMAWRITE_data_69, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_7
MASK_POLL_32	 0x2109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_8
MASK_POLL_32	 0x2109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_9
MASK_POLL_32	 0x2149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_10
MASK_POLL_32	 0x2149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_11
MASK_POLL_32	 0x2109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_12
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
	 UC_DMA_BD	 0, 0x2110020, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110030, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110050, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110060, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110070, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110080, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110090, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100a0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100b0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100c0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100d0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100f0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110100, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110110, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110120, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110140, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110150, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110160, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110170, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110180, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110190, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101a0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101b0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101c0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101d0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101f0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150000, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150020, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150030, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150040, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150050, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150060, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150070, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150080, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150090, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21500a0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21500b0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21500c0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21500d0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21500e0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21500f0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150100, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150110, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150120, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150130, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150140, @WRITE_data_0, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x2108000, @DMAWRITE_data_0, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108030, @DMAWRITE_data_1, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108300, @DMAWRITE_data_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108330, @DMAWRITE_data_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108600, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108630, @DMAWRITE_data_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108660, @DMAWRITE_data_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108690, @DMAWRITE_data_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108900, @DMAWRITE_data_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108930, @DMAWRITE_data_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108960, @DMAWRITE_data_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108990, @DMAWRITE_data_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21089c0, @DMAWRITE_data_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21089f0, @DMAWRITE_data_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108a20, @DMAWRITE_data_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108a50, @DMAWRITE_data_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108a80, @DMAWRITE_data_16, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108ab0, @DMAWRITE_data_17, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108ae0, @DMAWRITE_data_18, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108b10, @DMAWRITE_data_19, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108b40, @DMAWRITE_data_20, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108b70, @DMAWRITE_data_21, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148000, @DMAWRITE_data_22, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148030, @DMAWRITE_data_23, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148300, @DMAWRITE_data_24, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148330, @DMAWRITE_data_25, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148600, @DMAWRITE_data_26, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148630, @DMAWRITE_data_27, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148660, @DMAWRITE_data_28, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148690, @DMAWRITE_data_29, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21486c0, @DMAWRITE_data_30, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21486f0, @DMAWRITE_data_31, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148720, @DMAWRITE_data_32, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148750, @DMAWRITE_data_33, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148780, @DMAWRITE_data_34, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21487b0, @DMAWRITE_data_35, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21487e0, @DMAWRITE_data_36, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148810, @DMAWRITE_data_37, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148900, @DMAWRITE_data_38, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c00, @DMAWRITE_data_39, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c30, @DMAWRITE_data_40, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c60, @DMAWRITE_data_41, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109200, @DMAWRITE_data_42, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109230, @DMAWRITE_data_43, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109260, @DMAWRITE_data_44, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109800, @DMAWRITE_data_45, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109830, @DMAWRITE_data_46, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109860, @DMAWRITE_data_47, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109890, @DMAWRITE_data_48, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21098c0, @DMAWRITE_data_49, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21098f0, @DMAWRITE_data_50, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109920, @DMAWRITE_data_51, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109950, @DMAWRITE_data_52, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c00, @DMAWRITE_data_53, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c30, @DMAWRITE_data_54, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c60, @DMAWRITE_data_55, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148f00, @DMAWRITE_data_56, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148f30, @DMAWRITE_data_57, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148f60, @DMAWRITE_data_58, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149200, @DMAWRITE_data_59, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149230, @DMAWRITE_data_60, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149260, @DMAWRITE_data_61, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149500, @DMAWRITE_data_62, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149530, @DMAWRITE_data_63, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149560, @DMAWRITE_data_64, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149e1c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e1c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e1c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e14, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e04, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e0c, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e04, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e0c, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e6c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e7c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e6c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e7c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e84, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110000, @WRITE_data_0, 1, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x2018000, @DMAWRITE_data_65, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x2018030, @DMAWRITE_data_66, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x2018060, @DMAWRITE_data_67, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x2018090, @DMAWRITE_data_68, 0x9, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x2058030, @DMAWRITE_data_69, 0x9, 0, 0
UCBD_label_7:
	 UC_DMA_BD	 0, 0x2018544, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x201855c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x201855c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2058554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2010000, @WRITE_data_0, 1, 0, 0
UCBD_label_8:
	 UC_DMA_BD	 0, 0x2108000, @DMAWRITE_data_70, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109e04, @WRITE_data_0, 1, 0, 0
UCBD_label_9:
	 UC_DMA_BD	 0, 0x2108300, @DMAWRITE_data_71, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109e0c, @WRITE_data_0, 1, 0, 0
UCBD_label_10:
	 UC_DMA_BD	 0, 0x2148000, @DMAWRITE_data_72, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149e04, @WRITE_data_0, 1, 0, 0
UCBD_label_11:
	 UC_DMA_BD	 0, 0x2148300, @DMAWRITE_data_73, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149e0c, @WRITE_data_0, 1, 0, 0
UCBD_label_12:
	 UC_DMA_BD	 0, 0x2109890, @DMAWRITE_data_74, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109e84, @WRITE_data_0, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x00000000
WRITE_data_1:
	.long 0x00000001
WRITE_data_2:
	.long 0x00000002
WRITE_data_3:
	.long 0x00000006
DMAWRITE_data_0:
	.long 0x00830a00
	.long 0xfe000400
	.long 0x1eb00bdf
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x0c000000
DMAWRITE_data_1:
	.long 0x00831a00
	.long 0xfe000400
	.long 0x1f000be9
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_2:
	.long 0x00830a20
	.long 0xfe000400
	.long 0x1ec00bdf
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x0c000000
DMAWRITE_data_3:
	.long 0x00831a20
	.long 0xfe000400
	.long 0x1f100be9
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_4:
	.long 0x00800400
	.long 0xfc008000
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
	.long 0x00000000
DMAWRITE_data_6:
	.long 0x00808400
	.long 0xfc008000
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
	.long 0x00832a00
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
DMAWRITE_data_9:
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
DMAWRITE_data_10:
	.long 0x00810400
	.long 0xf4008180
	.long 0x1cf00ba1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_11:
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
	.long 0x24000000
DMAWRITE_data_12:
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
	.long 0x2c000000
DMAWRITE_data_13:
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
	.long 0x34000000
DMAWRITE_data_14:
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
	.long 0x3c000000
DMAWRITE_data_15:
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
	.long 0x44000000
DMAWRITE_data_16:
	.long 0x00820700
	.long 0xf4008180
	.long 0x1d600baf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x4c000000
DMAWRITE_data_17:
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
	.long 0x54000000
DMAWRITE_data_18:
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
	.long 0x5c000000
DMAWRITE_data_19:
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
	.long 0x64000000
DMAWRITE_data_20:
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
	.long 0x6c000000
DMAWRITE_data_21:
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
	.long 0x00000000
DMAWRITE_data_22:
	.long 0x00830a40
	.long 0xfe000400
	.long 0x1ed00bdf
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x0c000000
DMAWRITE_data_23:
	.long 0x00831a40
	.long 0xfe000400
	.long 0x1f200be9
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_24:
	.long 0x00830a60
	.long 0xfe000400
	.long 0x1ee00bdf
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x0c000000
DMAWRITE_data_25:
	.long 0x00831a60
	.long 0xfe000400
	.long 0x1f300be9
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_26:
	.long 0x00818580
	.long 0xf4008180
	.long 0x1dd00bbd
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
	.long 0x1c000bbf
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
	.long 0x1c000bc1
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
	.long 0x1c000bc3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_30:
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
	.long 0x2c000000
DMAWRITE_data_31:
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
	.long 0x34000000
DMAWRITE_data_32:
	.long 0x00828880
	.long 0xf4008180
	.long 0x1e400bcb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x3c000000
DMAWRITE_data_33:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bcd
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x44000000
DMAWRITE_data_34:
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
	.long 0x4c000000
DMAWRITE_data_35:
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
	.long 0x54000000
DMAWRITE_data_36:
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
	.long 0x5c000000
DMAWRITE_data_37:
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
DMAWRITE_data_38:
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
DMAWRITE_data_39:
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
DMAWRITE_data_40:
	.long 0x00810400
	.long 0xfe0040c0
	.long 0x1d200b9f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_41:
	.long 0x00820700
	.long 0xfe0040c0
	.long 0x1d900bad
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_42:
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
DMAWRITE_data_43:
	.long 0x008144c0
	.long 0xfe0040c0
	.long 0x1d300b9f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_44:
	.long 0x008247c0
	.long 0xfe0040c0
	.long 0x1da00bad
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_45:
	.long 0x00830a00
	.long 0xf8001000
	.long 0x1ef00bd7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_46:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bd9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_47:
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
	.long 0x1c000000
DMAWRITE_data_48:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bdd
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_49:
	.long 0x00831a00
	.long 0xf8001000
	.long 0x1f400be1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
DMAWRITE_data_50:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000be3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x34000000
DMAWRITE_data_51:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000be5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x3c000000
DMAWRITE_data_52:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000be7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_53:
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
DMAWRITE_data_54:
	.long 0x00818580
	.long 0xfe0040c0
	.long 0x1e000bbb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_55:
	.long 0x00828880
	.long 0xfe0040c0
	.long 0x1e700bc9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_56:
	.long 0x00832a00
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
DMAWRITE_data_57:
	.long 0x00800400
	.long 0xfe028000
	.long 0x1ca00b93
	.long 0x00000400
	.long 0x00000020
	.long 0x00000000
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x01000400
	.long 0x00080000
DMAWRITE_data_58:
	.long 0x00808400
	.long 0xfe028000
	.long 0x1cd00b99
	.long 0x00000400
	.long 0x00000020
	.long 0x00000000
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x01000400
	.long 0x00080000
DMAWRITE_data_59:
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
DMAWRITE_data_60:
	.long 0x0081c640
	.long 0xfe0040c0
	.long 0x1e100bbb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_61:
	.long 0x0082c940
	.long 0xfe0040c0
	.long 0x1e800bc9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_62:
	.long 0x00832a00
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
DMAWRITE_data_63:
	.long 0x00800400
	.long 0xfe028000
	.long 0x1cb00b93
	.long 0x00000400
	.long 0x00000020
	.long 0x00000000
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x01000400
	.long 0x00080000
DMAWRITE_data_64:
	.long 0x00808400
	.long 0xfe028000
	.long 0x1ce00b99
	.long 0x00000400
	.long 0x00000020
	.long 0x00000000
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x01000400
	.long 0x00080000
WRITE_data_4:
	.long 0x00040002
WRITE_data_5:
	.long 0x00040000
WRITE_data_6:
	.long 0x00010000
WRITE_data_7:
	.long 0x00040001
DMAWRITE_data_65:
	.long 0x00000000
    .long 0x076e2f00
	.long 0x00000020
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_66:
	.long 0x00000000
    .long 0x019c0000
	.long 0x00008000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_67:
	.long 0x00000000
    .long 0x0745b700
	.long 0x00050f00
	.long 0x0300e002
	.long 0x4a000000
	.long 0x00000940
	.long 0x00050f00
	.long 0x20000001
	.long 0x040c0000
DMAWRITE_data_68:
	.long 0x00000000
    .long 0x026b7600
	.long 0x00005000
	.long 0x03020000
	.long 0x04000000
	.long 0x00000280
	.long 0x00000080
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_69:
	.long 0x00000000
    .long 0x0747bd00
	.long 0x00050f00
	.long 0x0300e002
	.long 0x4a000000
	.long 0x00000940
	.long 0x00050f00
	.long 0x20000001
	.long 0x040c0000
WRITE_data_8:
	.long 0x00000003
DMAWRITE_data_70:
	.long 0x00830a00
	.long 0xfe000400
	.long 0x1eb00bdf
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_71:
	.long 0x00830a20
	.long 0xfe000400
	.long 0x1ec00bdf
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_72:
	.long 0x00830a40
	.long 0xfe000400
	.long 0x1ed00bdf
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_73:
	.long 0x00830a60
	.long 0xfe000400
	.long 0x1ee00bdf
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_74:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bdd
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
hintmap_0:
	.long 0x00000000
