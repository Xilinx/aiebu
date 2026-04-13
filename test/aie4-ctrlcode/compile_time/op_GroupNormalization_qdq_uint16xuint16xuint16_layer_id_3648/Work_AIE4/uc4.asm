.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;
.attach_to_group	 4
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
APPLY_OFFSET_57	 @DMAWRITE_data_38, 1, 3
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
APPLY_OFFSET_57	 @DMAWRITE_data_39, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
APPLY_OFFSET_57	 @DMAWRITE_data_40, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
APPLY_OFFSET_57	 @DMAWRITE_data_41, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
APPLY_OFFSET_57	 @DMAWRITE_data_42, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
MASK_POLL_32	 0x4109f10, 0x1f8003c, 0x0
MASK_POLL_32	 0x4018570, 0x1f8003c, 0x0
REMOTE_BARRIER	 $rb0, 0x15
END_JOB

START_JOB 1
PREEMPT	0x329f, @save, @restore , @hintmap_0
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x4108000, @DMAWRITE_data_0, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108030, @DMAWRITE_data_1, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108300, @DMAWRITE_data_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108330, @DMAWRITE_data_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108600, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108630, @DMAWRITE_data_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108660, @DMAWRITE_data_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108690, @DMAWRITE_data_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x41086c0, @DMAWRITE_data_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x41086f0, @DMAWRITE_data_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108720, @DMAWRITE_data_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108750, @DMAWRITE_data_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148000, @DMAWRITE_data_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148030, @DMAWRITE_data_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148300, @DMAWRITE_data_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148330, @DMAWRITE_data_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108c00, @DMAWRITE_data_16, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108c30, @DMAWRITE_data_17, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108c60, @DMAWRITE_data_18, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108c90, @DMAWRITE_data_19, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108cc0, @DMAWRITE_data_20, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109200, @DMAWRITE_data_21, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109230, @DMAWRITE_data_22, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109260, @DMAWRITE_data_23, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109290, @DMAWRITE_data_24, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x41092c0, @DMAWRITE_data_25, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109800, @DMAWRITE_data_26, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109830, @DMAWRITE_data_27, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148c00, @DMAWRITE_data_28, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148c30, @DMAWRITE_data_29, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148c60, @DMAWRITE_data_30, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148c90, @DMAWRITE_data_31, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148cc0, @DMAWRITE_data_32, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4149200, @DMAWRITE_data_33, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4149230, @DMAWRITE_data_34, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4149260, @DMAWRITE_data_35, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4149290, @DMAWRITE_data_36, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x41492c0, @DMAWRITE_data_37, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4110010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110030, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110050, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110060, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110070, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110080, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110090, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x41100a0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x41100b0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x41100c0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x41100d0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x41100e0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x41100f0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110100, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110110, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110120, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110130, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110140, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110150, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110160, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110170, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110180, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110190, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x41101a0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x41101b0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x41101c0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x41101d0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x41101e0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x41101f0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4150000, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4150010, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4150020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4150030, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e14, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e14, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e14, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e14, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e04, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e0c, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e04, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e0c, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e84, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110000, @WRITE_data_1, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x4018000, @DMAWRITE_data_38, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x4018030, @DMAWRITE_data_39, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x4018060, @DMAWRITE_data_40, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x4018090, @DMAWRITE_data_41, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x40180c0, @DMAWRITE_data_42, 0x9, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x4018544, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018554, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018554, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018554, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x4010000, @WRITE_data_1, 1, 0, 0
.align    4
DMAWRITE_data_0:
	.long 0x00803100
	.long 0xfe000370
	.long 0x1da00bb7
	.long 0x0000006e
	.long 0x0000000a
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x0c000000
DMAWRITE_data_1:
	.long 0x00803ec0
	.long 0xfe000370
	.long 0x1df00bc1
	.long 0x0000006e
	.long 0x0000000a
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x00000000
DMAWRITE_data_2:
	.long 0x00803470
	.long 0xfe000370
	.long 0x1db00bb9
	.long 0x0000006e
	.long 0x0000000a
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x0c000000
DMAWRITE_data_3:
	.long 0x00804230
	.long 0xfe000370
	.long 0x1e000bc3
	.long 0x0000006e
	.long 0x0000000a
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x00000000
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
	.long 0x00800dc0
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
DMAWRITE_data_9:
	.long 0x00801c60
	.long 0xfe001400
	.long 0x1cb00b99
	.long 0x00000a50
	.long 0x0000014a
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00102800
	.long 0x00000000
DMAWRITE_data_10:
	.long 0x00800000
	.long 0xfe000c80
	.long 0x1d000ba3
	.long 0x0000006e
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000c80
	.long 0x3c000000
DMAWRITE_data_11:
	.long 0x00800ea0
	.long 0xfe000c80
	.long 0x1d500bad
	.long 0x0000006e
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000c80
	.long 0x00000000
DMAWRITE_data_12:
	.long 0x008037e0
	.long 0xfe000370
	.long 0x1dc00bbb
	.long 0x0000006e
	.long 0x0000000a
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x0c000000
DMAWRITE_data_13:
	.long 0x008045a0
	.long 0xfe000370
	.long 0x1e100bc5
	.long 0x0000006e
	.long 0x0000000a
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x00000000
DMAWRITE_data_14:
	.long 0x00803b50
	.long 0xfe000370
	.long 0x1dd00bbd
	.long 0x0000006e
	.long 0x0000000a
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x0c000000
DMAWRITE_data_15:
	.long 0x00804910
	.long 0xfe000370
	.long 0x1e200bc7
	.long 0x0000006e
	.long 0x0000000a
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x00000000
DMAWRITE_data_16:
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
DMAWRITE_data_17:
	.long 0x00800dc0
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
DMAWRITE_data_18:
	.long 0x00801d3c
	.long 0xfe000640
	.long 0x1cc00b9b
	.long 0x0000014a
	.long 0x0000000a
	.long 0x00000a50
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x00028000
DMAWRITE_data_19:
	.long 0x00800000
	.long 0xfe000370
	.long 0x1d100ba5
	.long 0x0000006e
	.long 0x0000000a
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x24000000
DMAWRITE_data_20:
	.long 0x00800ea0
	.long 0xfe000370
	.long 0x1d600baf
	.long 0x0000006e
	.long 0x0000000a
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x00000000
DMAWRITE_data_21:
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
DMAWRITE_data_22:
	.long 0x00800dc0
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
DMAWRITE_data_23:
	.long 0x00801d3c
	.long 0xfe000640
	.long 0x1cd00b9d
	.long 0x0000014a
	.long 0x0000000a
	.long 0x00000a50
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x00028000
DMAWRITE_data_24:
	.long 0x00800370
	.long 0xfe000370
	.long 0x1d200ba7
	.long 0x0000006e
	.long 0x0000000a
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x24000000
DMAWRITE_data_25:
	.long 0x00801210
	.long 0xfe000370
	.long 0x1d700bb1
	.long 0x0000006e
	.long 0x0000000a
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x00000000
DMAWRITE_data_26:
	.long 0x00803100
	.long 0xfe000c80
	.long 0x1de00bb5
	.long 0x0000006e
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000c80
	.long 0x0c000000
DMAWRITE_data_27:
	.long 0x00803ec0
	.long 0xfe000c80
	.long 0x1e300bbf
	.long 0x0000006e
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000c80
	.long 0x00000000
DMAWRITE_data_28:
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
DMAWRITE_data_29:
	.long 0x00800dc0
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
DMAWRITE_data_30:
	.long 0x00801d3c
	.long 0xfe000640
	.long 0x1ce00b9f
	.long 0x0000014a
	.long 0x0000000a
	.long 0x00000a50
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x00028000
DMAWRITE_data_31:
	.long 0x008006e0
	.long 0xfe000370
	.long 0x1d300ba9
	.long 0x0000006e
	.long 0x0000000a
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x24000000
DMAWRITE_data_32:
	.long 0x00801580
	.long 0xfe000370
	.long 0x1d800bb3
	.long 0x0000006e
	.long 0x0000000a
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x00000000
DMAWRITE_data_33:
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
DMAWRITE_data_34:
	.long 0x00800dc0
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
DMAWRITE_data_35:
	.long 0x00801d3c
	.long 0xfe000640
	.long 0x1cf00b97
	.long 0x0000014a
	.long 0x0000000a
	.long 0x00000a50
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x00028000
DMAWRITE_data_36:
	.long 0x00800a50
	.long 0xfe000370
	.long 0x1d400ba1
	.long 0x0000006e
	.long 0x0000000a
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x24000000
DMAWRITE_data_37:
	.long 0x008018f0
	.long 0xfe000370
	.long 0x1d900bab
	.long 0x0000006e
	.long 0x0000000a
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00400140
	.long 0x00000000
WRITE_data_0:
	.long 0x00000004
WRITE_data_1:
	.long 0x00000000
WRITE_data_2:
	.long 0x00000001
WRITE_data_3:
	.long 0x00000005
WRITE_data_4:
	.long 0x001f0006
WRITE_data_5:
	.long 0x000f0000
WRITE_data_6:
	.long 0x00000002
WRITE_data_7:
	.long 0x001f0003
DMAWRITE_data_38:
	.long 0x00000000
	.long 0x01d96000
	.long 0x00000400
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_39:
	.long 0x00000000
	.long 0x9f8aeb84
	.long 0x000000e0
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_40:
	.long 0x00000000
	.long 0x9f8aef04
	.long 0x00000280
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_41:
	.long 0x00000000
	.long 0x0292b370
	.long 0x00019000
	.long 0x03000000
	.long 0x03200000
	.long 0x00000140
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_42:
	.long 0x00000000
	.long 0x02a6b370
	.long 0x00019000
	.long 0x03000000
	.long 0x03200000
	.long 0x00000140
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
WRITE_data_8:
	.long 0x00070002
WRITE_data_9:
	.long 0x00010003
hintmap_0:
	.long 0x00000000
