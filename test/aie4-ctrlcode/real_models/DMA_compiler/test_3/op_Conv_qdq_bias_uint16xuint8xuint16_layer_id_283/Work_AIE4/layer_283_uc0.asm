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
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
; SHIM_WGT
APPLY_OFFSET_57	 @DMAWRITE_data_50, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_51, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_52, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_53, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
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
	 UC_DMA_BD	 0, 0x110020, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110030, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110040, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110050, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110060, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x110070, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110080, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110090, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100a0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100b0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100c0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100d0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100e0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100f0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110100, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110110, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110120, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110130, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110140, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110150, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110160, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110170, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110180, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110190, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101a0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101b0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101c0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101d0, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101e0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101f0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x150000, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x150010, @WRITE_data_2, 1, 0, 0
UCBD_label_1:
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
	 UC_DMA_BD	 0, 0x108900, @DMAWRITE_data_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108930, @DMAWRITE_data_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148000, @DMAWRITE_data_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148030, @DMAWRITE_data_16, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148300, @DMAWRITE_data_17, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148330, @DMAWRITE_data_18, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148900, @DMAWRITE_data_19, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c00, @DMAWRITE_data_20, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c30, @DMAWRITE_data_21, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c60, @DMAWRITE_data_22, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c90, @DMAWRITE_data_23, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f00, @DMAWRITE_data_24, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f30, @DMAWRITE_data_25, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f60, @DMAWRITE_data_26, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109200, @DMAWRITE_data_27, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109230, @DMAWRITE_data_28, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109260, @DMAWRITE_data_29, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109290, @DMAWRITE_data_30, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109500, @DMAWRITE_data_31, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109530, @DMAWRITE_data_32, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109560, @DMAWRITE_data_33, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109800, @DMAWRITE_data_34, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109830, @DMAWRITE_data_35, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109860, @DMAWRITE_data_36, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109890, @DMAWRITE_data_37, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1098c0, @DMAWRITE_data_38, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1098f0, @DMAWRITE_data_39, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109920, @DMAWRITE_data_40, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109950, @DMAWRITE_data_41, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c00, @DMAWRITE_data_42, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c30, @DMAWRITE_data_43, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c60, @DMAWRITE_data_44, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c90, @DMAWRITE_data_45, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149200, @DMAWRITE_data_46, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149230, @DMAWRITE_data_47, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149260, @DMAWRITE_data_48, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149290, @DMAWRITE_data_49, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e1c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e1c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x110000, @WRITE_data_2, 1, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_50, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x18030, @DMAWRITE_data_51, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x18060, @DMAWRITE_data_52, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x18090, @DMAWRITE_data_53, 0x9, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x1855c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x10000, @WRITE_data_2, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x0d600d60
WRITE_data_1:
	.long 0x00000001
WRITE_data_2:
	.long 0x00000000
WRITE_data_3:
	.long 0x00000002
WRITE_data_4:
	.long 0x00000028
DMAWRITE_data_0:
	.long 0x0081e580
	.long 0xfe000400
	.long 0x1ce00ba5
	.long 0x00000400
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00002000
	.long 0x0c000000
DMAWRITE_data_1:
	.long 0x0081f580
	.long 0xfe000400
	.long 0x1d300baf
	.long 0x00000400
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00002000
	.long 0x00000000
DMAWRITE_data_2:
	.long 0x0081e780
	.long 0xfe000400
	.long 0x1cf00ba5
	.long 0x00000400
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00002000
	.long 0x0c000000
DMAWRITE_data_3:
	.long 0x0081f780
	.long 0xfe000400
	.long 0x1d400baf
	.long 0x00000400
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00002000
	.long 0x00000000
DMAWRITE_data_4:
	.long 0x00800400
	.long 0xb0006400
	.long 0x1c905395
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
	.long 0x1c005397
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
	.long 0x1c005399
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
	.long 0x1c00539b
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
	.long 0x1d800bb3
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
	.long 0xb0007800
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
	.long 0x00820580
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
DMAWRITE_data_14:
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
DMAWRITE_data_15:
	.long 0x0081e680
	.long 0xfe000400
	.long 0x1d000ba5
	.long 0x00000400
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00002000
	.long 0x0c000000
DMAWRITE_data_16:
	.long 0x0081f680
	.long 0xfe000400
	.long 0x1d500baf
	.long 0x00000400
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00002000
	.long 0x00000000
DMAWRITE_data_17:
	.long 0x0081e880
	.long 0xfe000400
	.long 0x1d100ba5
	.long 0x00000400
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00002000
	.long 0x0c000000
DMAWRITE_data_18:
	.long 0x0081f880
	.long 0xfe000400
	.long 0x1d600baf
	.long 0x00000400
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00002000
	.long 0x00000000
DMAWRITE_data_19:
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
DMAWRITE_data_20:
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
DMAWRITE_data_21:
	.long 0x00800400
	.long 0xfe002580
	.long 0x1ca00b93
	.long 0x000000a0
	.long 0x00001400
	.long 0x00000020
	.long 0x00000001
	.long 0x00200000
	.long 0x00010001
	.long 0x00480400
	.long 0x00014000
DMAWRITE_data_22:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1d900bb5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_23:
	.long 0x00800400
	.long 0xfe002580
	.long 0x1de00bbb
	.long 0x000000a0
	.long 0x00001400
	.long 0x00000020
	.long 0x00000001
	.long 0x00200000
	.long 0x00000001
	.long 0x00480400
	.long 0x00018000
DMAWRITE_data_24:
	.long 0x00820580
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
DMAWRITE_data_25:
	.long 0x008c7c00
	.long 0xfe00b4c0
	.long 0x20f00c1d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_26:
	.long 0x008d30c0
	.long 0xfe00b4c0
	.long 0x21600c2b
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_27:
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
DMAWRITE_data_28:
	.long 0x00800d60
	.long 0xfe002580
	.long 0x1cb00b93
	.long 0x000000a0
	.long 0x00001400
	.long 0x00000020
	.long 0x00000001
	.long 0x00200000
	.long 0x00010000
	.long 0x00500400
	.long 0x00014000
DMAWRITE_data_29:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1da00bb7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_30:
	.long 0x00800d60
	.long 0xfe002580
	.long 0x1df00bbb
	.long 0x000000a0
	.long 0x00001400
	.long 0x00000020
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00500400
	.long 0x00018000
DMAWRITE_data_31:
	.long 0x00820580
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
DMAWRITE_data_32:
	.long 0x008c7c00
	.long 0xfe00b4c0
	.long 0x21000c1d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_33:
	.long 0x008d30c0
	.long 0xfe00b4c0
	.long 0x21700c2b
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_34:
	.long 0x0081e580
	.long 0xf8001000
	.long 0x1d200b9d
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
	.long 0x1c000b9f
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
	.long 0x1c000ba1
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
	.long 0x1c000ba3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_38:
	.long 0x0081f580
	.long 0xf8001000
	.long 0x1d700ba7
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
	.long 0x1c000ba9
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
	.long 0x1c000bab
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
	.long 0x1c000bad
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
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_43:
	.long 0x00800860
	.long 0xfe002580
	.long 0x1cc00b93
	.long 0x000000a0
	.long 0x00001400
	.long 0x00000020
	.long 0x00000001
	.long 0x00200000
	.long 0x00010000
	.long 0x00500400
	.long 0x00014000
DMAWRITE_data_44:
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
DMAWRITE_data_45:
	.long 0x00800860
	.long 0xfe002580
	.long 0x1e000bbb
	.long 0x000000a0
	.long 0x00001400
	.long 0x00000020
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00500400
	.long 0x00018000
DMAWRITE_data_46:
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
DMAWRITE_data_47:
	.long 0x00801260
	.long 0xfe002580
	.long 0x1cd00b93
	.long 0x000000a0
	.long 0x00001400
	.long 0x00000020
	.long 0x00000001
	.long 0x00200000
	.long 0x00010100
	.long 0x00480400
	.long 0x00014000
DMAWRITE_data_48:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1dc00bb1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_49:
	.long 0x00801260
	.long 0xfe002580
	.long 0x1e100bbb
	.long 0x000000a0
	.long 0x00001400
	.long 0x00000020
	.long 0x00000001
	.long 0x00200000
	.long 0x00000100
	.long 0x00480400
	.long 0x00018000
WRITE_data_5:
	.long 0x000e0000
WRITE_data_6:
	.long 0x00010004
WRITE_data_7:
	.long 0x00010005
WRITE_data_8:
	.long 0x00090001
WRITE_data_9:
	.long 0x000e0001
WRITE_data_10:
	.long 0x00010002
WRITE_data_11:
	.long 0x00130003
DMAWRITE_data_50:
	.long 0x00000000
	.long 0x0b33df00
	.long 0x00000020
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_51:
	.long 0x00000000
	.long 0x02be5a00
	.long 0x00006400
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_52:
	.long 0x00000000
	.long 0x0346ae00
	.long 0x0001e000
	.long 0x0308000a
	.long 0x01000000
	.long 0x00000140
	.long 0x00000020
	.long 0x20000001
	.long 0x07800000
DMAWRITE_data_53:
	.long 0x00000000
	.long 0x02c1ca00
	.long 0x00007800
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x2040f000
	.long 0x00000400
WRITE_data_12:
	.long 0x00010003
hintmap_0:
	.long 0x00000000
