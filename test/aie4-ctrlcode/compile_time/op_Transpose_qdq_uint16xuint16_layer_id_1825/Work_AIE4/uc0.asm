.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;
.attach_to_group	 0
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
APPLY_OFFSET_57	 @DMAWRITE_data_21, 1, 3
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
APPLY_OFFSET_57	 @DMAWRITE_data_22, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
APPLY_OFFSET_57	 @DMAWRITE_data_23, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
APPLY_OFFSET_57	 @DMAWRITE_data_24, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
MASK_POLL_32	 0x109f10, 0x1f8003c, 0x0
MASK_POLL_32	 0x18570, 0x1f8003c, 0x0
REMOTE_BARRIER	 $rb0, 0x15
END_JOB

START_JOB 1
PREEMPT	0x62b5, @save, @restore , @hintmap_0
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x108000, @DMAWRITE_data_0, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108300, @DMAWRITE_data_1, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108600, @DMAWRITE_data_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108630, @DMAWRITE_data_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108900, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148000, @DMAWRITE_data_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148300, @DMAWRITE_data_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c00, @DMAWRITE_data_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c30, @DMAWRITE_data_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f00, @DMAWRITE_data_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109200, @DMAWRITE_data_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109230, @DMAWRITE_data_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109500, @DMAWRITE_data_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109800, @DMAWRITE_data_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109830, @DMAWRITE_data_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109860, @DMAWRITE_data_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109890, @DMAWRITE_data_16, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c00, @DMAWRITE_data_17, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c30, @DMAWRITE_data_18, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149200, @DMAWRITE_data_19, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149230, @DMAWRITE_data_20, 0xb, 0, 1
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
	 UC_DMA_BD	 0, 0x1100b0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100c0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100d0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100f0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110100, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110110, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110120, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e1c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x110000, @WRITE_data_1, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_21, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x18030, @DMAWRITE_data_22, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x18060, @DMAWRITE_data_23, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x18090, @DMAWRITE_data_24, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1855c, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x10000, @WRITE_data_1, 1, 0, 0
.align    4
DMAWRITE_data_0:
	.long 0x008084e0
	.long 0xfe002000
	.long 0x1ce00ba5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_1:
	.long 0x0080a4e0
	.long 0xfe002000
	.long 0x1cf00ba5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_2:
	.long 0x008000e0
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
	.long 0x008004e0
	.long 0xfe008000
	.long 0x1c600b8f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4:
	.long 0x00800000
	.long 0xfe0000e0
	.long 0x1cb00b99
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_5:
	.long 0x0080c4e0
	.long 0xfe002000
	.long 0x1d000ba5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6:
	.long 0x0080e4e0
	.long 0xfe002000
	.long 0x1d100ba5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7:
	.long 0x008000e0
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
DMAWRITE_data_8:
	.long 0x008004e0
	.long 0xfe002000
	.long 0x1c700b91
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_9:
	.long 0x00800000
	.long 0xfe0000e0
	.long 0x1cc00b9b
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_10:
	.long 0x008001e0
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
DMAWRITE_data_11:
	.long 0x00800500
	.long 0xfe002000
	.long 0x1c800b93
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_12:
	.long 0x00800000
	.long 0xfe0000e0
	.long 0x1cd00b97
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_13:
	.long 0x008084e0
	.long 0xf8008000
	.long 0x1d200b9d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_14:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000b9f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_15:
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
	.long 0x00000000
DMAWRITE_data_17:
	.long 0x008002e0
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
DMAWRITE_data_18:
	.long 0x00800520
	.long 0xfe002000
	.long 0x1c900b95
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_19:
	.long 0x008003e0
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
DMAWRITE_data_20:
	.long 0x00800540
	.long 0xfe002000
	.long 0x1ca00b8d
	.long 0x00000080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
WRITE_data_0:
	.long 0x00000001
WRITE_data_1:
	.long 0x00000000
WRITE_data_2:
	.long 0x00010001
WRITE_data_3:
	.long 0x00010000
DMAWRITE_data_21:
	.long 0x00000000
	.long 0x00e1f000
	.long 0x00000400
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_22:
	.long 0x00000000
	.long 0x02a6b000
	.long 0x00008000
	.long 0x03000000
	.long 0x04000000
	.long 0x00000280
	.long 0x00000001
	.long 0x20400080
	.long 0x00000400
DMAWRITE_data_23:
	.long 0x00000000
	.long 0x47c42c80
	.long 0x000000e0
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_24:
	.long 0x00000000
	.long 0x029cb000
	.long 0x00008000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20408000
	.long 0x00000400
WRITE_data_4:
	.long 0x00010003
WRITE_data_5:
	.long 0x00000002
hintmap_0:
	.long 0x00000000
