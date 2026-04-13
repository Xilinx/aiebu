.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;
.attach_to_group	 2
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
APPLY_OFFSET_57	 @DMAWRITE_data_35, 1, 3
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
APPLY_OFFSET_57	 @DMAWRITE_data_36, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
APPLY_OFFSET_57	 @DMAWRITE_data_37, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
APPLY_OFFSET_57	 @DMAWRITE_data_38, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
APPLY_OFFSET_57	 @DMAWRITE_data_39, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
APPLY_OFFSET_57	 @DMAWRITE_data_40, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
APPLY_OFFSET_57	 @DMAWRITE_data_41, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_7
APPLY_OFFSET_57	 @DMAWRITE_data_42, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_8
APPLY_OFFSET_57	 @DMAWRITE_data_43, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_9
APPLY_OFFSET_57	 @DMAWRITE_data_44, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_10
APPLY_OFFSET_57	 @DMAWRITE_data_45, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_11
APPLY_OFFSET_57	 @DMAWRITE_data_46, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_12
APPLY_OFFSET_57	 @DMAWRITE_data_47, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_13
APPLY_OFFSET_57	 @DMAWRITE_data_48, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_14
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_15
MASK_POLL_32	 0x2109f10, 0x1f8003c, 0x0
MASK_POLL_32	 0x2018570, 0x1f8003c, 0x0
REMOTE_BARRIER	 $rb0, 0x15
END_JOB

START_JOB 1
PREEMPT	0x68fa, @save, @restore , @hintmap_0
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x2108000, @DMAWRITE_data_0, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108300, @DMAWRITE_data_1, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108600, @DMAWRITE_data_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108630, @DMAWRITE_data_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108660, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108690, @DMAWRITE_data_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21086c0, @DMAWRITE_data_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108900, @DMAWRITE_data_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108930, @DMAWRITE_data_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108960, @DMAWRITE_data_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108990, @DMAWRITE_data_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21089c0, @DMAWRITE_data_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21089f0, @DMAWRITE_data_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148000, @DMAWRITE_data_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148300, @DMAWRITE_data_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c00, @DMAWRITE_data_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c30, @DMAWRITE_data_16, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c60, @DMAWRITE_data_17, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109200, @DMAWRITE_data_18, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109230, @DMAWRITE_data_19, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109260, @DMAWRITE_data_20, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109800, @DMAWRITE_data_21, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109830, @DMAWRITE_data_22, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109860, @DMAWRITE_data_23, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109890, @DMAWRITE_data_24, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c00, @DMAWRITE_data_25, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c30, @DMAWRITE_data_26, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c60, @DMAWRITE_data_27, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148f00, @DMAWRITE_data_28, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148f30, @DMAWRITE_data_29, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149200, @DMAWRITE_data_30, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149230, @DMAWRITE_data_31, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149260, @DMAWRITE_data_32, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149500, @DMAWRITE_data_33, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149530, @DMAWRITE_data_34, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2110010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110030, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110050, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110060, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110070, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110080, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110090, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100a0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100b0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100c0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100d0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100e0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21100f0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110100, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110110, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110120, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110130, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110140, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110150, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110160, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e1c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e1c, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e04, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e0c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e04, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e0c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e6c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e7c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e84, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110000, @WRITE_data_1, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x2018000, @DMAWRITE_data_35, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x2018030, @DMAWRITE_data_36, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x2018060, @DMAWRITE_data_37, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x2018090, @DMAWRITE_data_38, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x20180c0, @DMAWRITE_data_39, 0x9, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x20180f0, @DMAWRITE_data_40, 0x9, 0, 0
UCBD_label_7:
	 UC_DMA_BD	 0, 0x2018120, @DMAWRITE_data_41, 0x9, 0, 0
UCBD_label_8:
	 UC_DMA_BD	 0, 0x2018150, @DMAWRITE_data_42, 0x9, 0, 0
UCBD_label_9:
	 UC_DMA_BD	 0, 0x2018180, @DMAWRITE_data_43, 0x9, 0, 0
UCBD_label_10:
	 UC_DMA_BD	 0, 0x20181b0, @DMAWRITE_data_44, 0x9, 0, 0
UCBD_label_11:
	 UC_DMA_BD	 0, 0x20181e0, @DMAWRITE_data_45, 0x9, 0, 0
UCBD_label_12:
	 UC_DMA_BD	 0, 0x2018210, @DMAWRITE_data_46, 0x9, 0, 0
UCBD_label_13:
	 UC_DMA_BD	 0, 0x2018240, @DMAWRITE_data_47, 0x9, 0, 0
UCBD_label_14:
	 UC_DMA_BD	 0, 0x2018270, @DMAWRITE_data_48, 0x9, 0, 0
UCBD_label_15:
	 UC_DMA_BD	 0, 0x2018544, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x201855c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x201855c, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x2010000, @WRITE_data_1, 1, 0, 0
.align    4
DMAWRITE_data_0:
	.long 0x00805388
	.long 0xfe000800
	.long 0x1d200ba7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_1:
	.long 0x00805b88
	.long 0xfe000800
	.long 0x1d300ba9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_2:
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
DMAWRITE_data_3:
	.long 0x00801388
	.long 0xfe000800
	.long 0x1cc00381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_4:
	.long 0x00801b88
	.long 0x00000800
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_5:
	.long 0x00802388
	.long 0x00000800
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_6:
	.long 0x00802b88
	.long 0x00000800
	.long 0x1c000b9b
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7:
	.long 0x00800400
	.long 0xfe000120
	.long 0x1c600b8f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_8:
	.long 0x00800500
	.long 0xfe000120
	.long 0x1c900b95
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_9:
	.long 0x00803388
	.long 0xfe000800
	.long 0x1cd00381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_10:
	.long 0x00803b88
	.long 0x00000800
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_11:
	.long 0x00804388
	.long 0x00000800
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
DMAWRITE_data_12:
	.long 0x00804b88
	.long 0x00000800
	.long 0x1c000b9d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_13:
	.long 0x00806388
	.long 0xfe000800
	.long 0x1d400bab
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_14:
	.long 0x00806b88
	.long 0xfe000800
	.long 0x1d500bad
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
DMAWRITE_data_16:
	.long 0x00801388
	.long 0xfe000800
	.long 0x1ce00381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_17:
	.long 0x00803388
	.long 0x00000800
	.long 0x1c000b9f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_18:
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
DMAWRITE_data_19:
	.long 0x00801b88
	.long 0xfe000800
	.long 0x1cf00381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_20:
	.long 0x00803b88
	.long 0x00000800
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
	.long 0x00805388
	.long 0xfe000800
	.long 0x1d600381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_22:
	.long 0x00805b88
	.long 0x00000800
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_23:
	.long 0x00806388
	.long 0x00000800
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_24:
	.long 0x00806b88
	.long 0x00000800
	.long 0x1c000ba5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_25:
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
DMAWRITE_data_26:
	.long 0x00802388
	.long 0xfe000800
	.long 0x1d000381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_27:
	.long 0x00804388
	.long 0x00000800
	.long 0x1c000ba3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_28:
	.long 0x00800400
	.long 0xfe000120
	.long 0x1c700b91
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_29:
	.long 0x00800500
	.long 0xfe000120
	.long 0x1ca00b97
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_30:
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
DMAWRITE_data_31:
	.long 0x00802b88
	.long 0xfe000800
	.long 0x1d100381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_32:
	.long 0x00804b88
	.long 0x00000800
	.long 0x1c000b99
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_33:
	.long 0x00800400
	.long 0xfe000120
	.long 0x1c800b8d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_34:
	.long 0x00800500
	.long 0xfe000120
	.long 0x1cb00b93
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
WRITE_data_0:
	.long 0x00000001
WRITE_data_1:
	.long 0x00000000
WRITE_data_2:
	.long 0x00340001
WRITE_data_3:
	.long 0x00340002
WRITE_data_4:
	.long 0x00340000
DMAWRITE_data_35:
	.long 0x00000000
	.long 0x00661000
	.long 0x00000400
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_36:
	.long 0x00000000
	.long 0x1c4b1700
	.long 0x00000120
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_37:
	.long 0x00000000
	.long 0x046f6000
	.long 0x00000800
	.long 0x03000000
	.long 0x00000000
	.long 0x1c000001
	.long 0x00000001
	.long 0x2d006000
	.long 0x00000400
DMAWRITE_data_38:
	.long 0x00000000
	.long 0x046f8000
	.long 0x00000800
	.long 0x03000000
	.long 0x00000000
	.long 0x24000001
	.long 0x00000001
	.long 0x2d006000
	.long 0x00000400
DMAWRITE_data_39:
	.long 0x00000000
	.long 0x046fa000
	.long 0x00000800
	.long 0x03000000
	.long 0x00000000
	.long 0x2c000001
	.long 0x00000001
	.long 0x2d006000
	.long 0x00000400
DMAWRITE_data_40:
	.long 0x00000000
	.long 0x046fc000
	.long 0x00000800
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x2d006000
	.long 0x00000400
DMAWRITE_data_41:
	.long 0x00000000
	.long 0x04bf6000
	.long 0x00000800
	.long 0x03000000
	.long 0x00000000
	.long 0x3c000001
	.long 0x00000001
	.long 0x2d006000
	.long 0x00000400
DMAWRITE_data_42:
	.long 0x00000000
	.long 0x04bf8000
	.long 0x00000800
	.long 0x03000000
	.long 0x00000000
	.long 0x44000001
	.long 0x00000001
	.long 0x2d006000
	.long 0x00000400
DMAWRITE_data_43:
	.long 0x00000000
	.long 0x04bfa000
	.long 0x00000800
	.long 0x03000000
	.long 0x00000000
	.long 0x4c000001
	.long 0x00000001
	.long 0x2d006000
	.long 0x00000400
DMAWRITE_data_44:
	.long 0x00000000
	.long 0x04bfc000
	.long 0x00000800
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x2d006000
	.long 0x00000400
DMAWRITE_data_45:
	.long 0x00000000
	.long 0x041f6000
	.long 0x00000800
	.long 0x03000000
	.long 0x00000000
	.long 0x5c000001
	.long 0x00000001
	.long 0x2d006000
	.long 0x00000400
DMAWRITE_data_46:
	.long 0x00000000
	.long 0x041f8000
	.long 0x00000800
	.long 0x03000000
	.long 0x00000000
	.long 0x64000001
	.long 0x00000001
	.long 0x2d006000
	.long 0x00000400
DMAWRITE_data_47:
	.long 0x00000000
	.long 0x041fa000
	.long 0x00000800
	.long 0x03000000
	.long 0x00000000
	.long 0x6c000001
	.long 0x00000001
	.long 0x2d006000
	.long 0x00000400
DMAWRITE_data_48:
	.long 0x00000000
	.long 0x041fc000
	.long 0x00000800
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x2d006000
	.long 0x00000400
WRITE_data_5:
	.long 0x0034000a
WRITE_data_6:
	.long 0x00340006
hintmap_0:
	.long 0x00000000
