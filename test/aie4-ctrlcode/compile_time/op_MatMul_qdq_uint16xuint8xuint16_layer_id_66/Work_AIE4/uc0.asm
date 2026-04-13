.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;
.attach_to_group	 0
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
APPLY_OFFSET_57	 @DMAWRITE_data_42, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
APPLY_OFFSET_57	 @DMAWRITE_data_43, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
APPLY_OFFSET_57	 @DMAWRITE_data_44, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
APPLY_OFFSET_57	 @DMAWRITE_data_45, 1, 3
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
APPLY_OFFSET_57	 @DMAWRITE_data_46, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
MASK_POLL_32	 0x109f10, 0x1f8003c, 0x0
MASK_POLL_32	 0x18570, 0x1f8003c, 0x0
REMOTE_BARRIER	 $rb0, 0x15
END_JOB

START_JOB 1
PREEMPT	0x7ee6, @save, @restore , @hintmap_0
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
	 UC_DMA_BD	 0, 0x108000, @DMAWRITE_data_0, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108030, @DMAWRITE_data_1, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108300, @DMAWRITE_data_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108330, @DMAWRITE_data_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108600, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108630, @DMAWRITE_data_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108900, @DMAWRITE_data_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108930, @DMAWRITE_data_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108960, @DMAWRITE_data_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108990, @DMAWRITE_data_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148000, @DMAWRITE_data_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148030, @DMAWRITE_data_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148300, @DMAWRITE_data_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148330, @DMAWRITE_data_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148600, @DMAWRITE_data_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148630, @DMAWRITE_data_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148660, @DMAWRITE_data_16, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148690, @DMAWRITE_data_17, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1486c0, @DMAWRITE_data_18, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c00, @DMAWRITE_data_19, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c30, @DMAWRITE_data_20, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c60, @DMAWRITE_data_21, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f00, @DMAWRITE_data_22, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f30, @DMAWRITE_data_23, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109200, @DMAWRITE_data_24, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109230, @DMAWRITE_data_25, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109260, @DMAWRITE_data_26, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109500, @DMAWRITE_data_27, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109530, @DMAWRITE_data_23, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109800, @DMAWRITE_data_28, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109830, @DMAWRITE_data_29, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109860, @DMAWRITE_data_30, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109890, @DMAWRITE_data_31, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1098c0, @DMAWRITE_data_32, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1098f0, @DMAWRITE_data_33, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109920, @DMAWRITE_data_34, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109950, @DMAWRITE_data_35, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c00, @DMAWRITE_data_36, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c30, @DMAWRITE_data_37, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c60, @DMAWRITE_data_38, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149200, @DMAWRITE_data_39, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149230, @DMAWRITE_data_40, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149260, @DMAWRITE_data_41, 0xb, 0, 1
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
	 UC_DMA_BD	 0, 0x1100c0, @WRITE_data_3, 1, 0, 1
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
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e1c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x110000, @WRITE_data_0, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_42, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x18030, @DMAWRITE_data_43, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x18060, @DMAWRITE_data_44, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x58000, @DMAWRITE_data_45, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x58030, @DMAWRITE_data_46, 0x9, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1855c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10000, @WRITE_data_0, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x00000000
DMAWRITE_data_0:
	.long 0x00814b00
	.long 0xfe000400
	.long 0x1d500bb3
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x0c000000
DMAWRITE_data_1:
	.long 0x00815b00
	.long 0xfe000400
	.long 0x1da00bbd
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_2:
	.long 0x00814b20
	.long 0xfe000400
	.long 0x1d600bb3
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x0c000000
DMAWRITE_data_3:
	.long 0x00815b20
	.long 0xfe000400
	.long 0x1db00bbd
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
	.long 0xec008000
	.long 0x1c602b8f
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
	.long 0x1c002b91
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
	.long 0xfc0031c0
	.long 0x1c900b95
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_7:
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
DMAWRITE_data_8:
	.long 0x0080e780
	.long 0xfc0031c0
	.long 0x1cc00b9b
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_9:
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
DMAWRITE_data_10:
	.long 0x00814b40
	.long 0xfe000400
	.long 0x1d700bb3
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x0c000000
DMAWRITE_data_11:
	.long 0x00815b40
	.long 0xfe000400
	.long 0x1dc00bbd
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_12:
	.long 0x00814b60
	.long 0xfe000400
	.long 0x1d800bb3
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x0c000000
DMAWRITE_data_13:
	.long 0x00815b60
	.long 0xfe000400
	.long 0x1dd00bbd
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_14:
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
DMAWRITE_data_15:
	.long 0x0080b5c0
	.long 0xfc0031c0
	.long 0x1cf00ba1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_16:
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
	.long 0x1c000000
DMAWRITE_data_17:
	.long 0x00811940
	.long 0xfc0031c0
	.long 0x1d200ba7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_18:
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
	.long 0x00000000
DMAWRITE_data_19:
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
DMAWRITE_data_20:
	.long 0x00808400
	.long 0xfe0018e0
	.long 0x1ca00b93
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_21:
	.long 0x0080e780
	.long 0xfe0018e0
	.long 0x1cd00b99
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_22:
	.long 0x00800400
	.long 0xfe007800
	.long 0x1c700381
	.long 0x00000400
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x01000400
	.long 0x0c000000
DMAWRITE_data_23:
	.long 0x008007c0
	.long 0x00001800
	.long 0x1c000b8d
	.long 0x00000400
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x04000000
	.long 0x01000400
	.long 0x00008000
DMAWRITE_data_24:
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
DMAWRITE_data_25:
	.long 0x00809ce0
	.long 0xfe0018e0
	.long 0x1cb00b93
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_26:
	.long 0x00810060
	.long 0xfe0018e0
	.long 0x1ce00b99
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
	.long 0xfe007800
	.long 0x1c800381
	.long 0x00000400
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x01000400
	.long 0x0c000000
DMAWRITE_data_28:
	.long 0x00814b00
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
DMAWRITE_data_29:
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
DMAWRITE_data_30:
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
DMAWRITE_data_31:
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
DMAWRITE_data_32:
	.long 0x00815b00
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
	.long 0x34000000
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
	.long 0x3c000000
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
	.long 0x00000000
DMAWRITE_data_36:
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
DMAWRITE_data_37:
	.long 0x0080b5c0
	.long 0xfe0018e0
	.long 0x1d000b9f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_38:
	.long 0x00811940
	.long 0xfe0018e0
	.long 0x1d300ba5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_39:
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
DMAWRITE_data_40:
	.long 0x0080cea0
	.long 0xfe0018e0
	.long 0x1d100b9f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_41:
	.long 0x00813220
	.long 0xfe0018e0
	.long 0x1d400ba5
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
	.long 0x0000000a
WRITE_data_3:
	.long 0x00000002
WRITE_data_4:
	.long 0x000e0000
WRITE_data_5:
	.long 0x000e0001
WRITE_data_6:
	.long 0x00010000
WRITE_data_7:
	.long 0x00040000
WRITE_data_8:
	.long 0x00000004
DMAWRITE_data_42:
	.long 0x00000000
	.long 0x00181000
	.long 0x00008000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_43:
	.long 0x00000000
	.long 0x08b89e80
	.long 0x0005d480
	.long 0x03004006
	.long 0x63800000
	.long 0x00000c70
	.long 0x0001f180
	.long 0x20000001
	.long 0x018e0000
DMAWRITE_data_44:
	.long 0x00000000
	.long 0x019a5000
	.long 0x00005000
	.long 0x03020000
	.long 0x04000000
	.long 0x00000280
	.long 0x00000080
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_45:
	.long 0x00000000
	.long 0x000c6000
	.long 0x00000400
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_46:
	.long 0x00000000
	.long 0x08b96580
	.long 0x0005d480
	.long 0x03004006
	.long 0x63800000
	.long 0x00000c70
	.long 0x0001f180
	.long 0x20000001
	.long 0x018e0000
hintmap_0:
	.long 0x00000000
