.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;
START_JOB 0
SAVE_TIMESTAMPS	 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
APPLY_OFFSET_57	 @DMAWRITE_data_1, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
MASK_POLL_32	 0x118604, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
APPLY_OFFSET_57	 @DMAWRITE_data_6, 1, 1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
MASK_POLL_32	 0x119604, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
APPLY_OFFSET_57	 @DMAWRITE_data_9, 1, 2
APPLY_OFFSET_SRAM	 @DMAWRITE_data_9, 1, 0x220000
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_7
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_8
SAVE_TIMESTAMPS	 1
MASK_POLL_32	 0x119e04, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_9
MASK_POLL_32	 0x11a604, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_10
APPLY_OFFSET_57	 @DMAWRITE_data_14, 1, 1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_11
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_12
APPLY_OFFSET_57	 @DMAWRITE_data_16, 1, 3
APPLY_OFFSET_SRAM	 @DMAWRITE_data_16, 1, 0x220004
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_13
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_14
APPLY_OFFSET_57	 @DMAWRITE_data_22, 1, 6
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_15
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_16
MASK_POLL_32	 0x11ae04, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_17
APPLY_OFFSET_57	 @DMAWRITE_data_25, 1, 6
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_18
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_19
MASK_POLL_32	 0x18570, 0x1f8003c, 0x0
SAVE_TIMESTAMPS	 2
MASK_POLL_32	 0x11b604, 0x1, 0x1
MASK_POLL_32	 0x11be04, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_20
APPLY_OFFSET_57	 @DMAWRITE_data_26, 1, 1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_21
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_22
APPLY_OFFSET_57	 @DMAWRITE_data_28, 1, 4
APPLY_OFFSET_SRAM	 @DMAWRITE_data_28, 1, 0x220008
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_23
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_24
APPLY_OFFSET_57	 @DMAWRITE_data_34, 1, 6
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_25
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_26
MASK_POLL_32	 0x11c604, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_27
MASK_POLL_32	 0x18570, 0x1f8003c, 0x0
SAVE_TIMESTAMPS	 3
MASK_POLL_32	 0x11ce04, 0x1, 0x1
MASK_POLL_32	 0x11d604, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_28
APPLY_OFFSET_57	 @DMAWRITE_data_36, 1, 1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_29
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_30
APPLY_OFFSET_57	 @DMAWRITE_data_38, 1, 5
APPLY_OFFSET_SRAM	 @DMAWRITE_data_38, 1, 0x22000c
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_31
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_32
MASK_POLL_32	 0x11de04, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_33
MASK_POLL_32	 0x18570, 0x1f8003c, 0x0
SAVE_TIMESTAMPS	 4
MASK_POLL_32	 0x11e604, 0x1, 0x1
MASK_POLL_32	 0x11ee04, 0x1, 0x1
MASK_POLL_32	 0x11f604, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_34
APPLY_OFFSET_57	 @DMAWRITE_data_45, 1, 6
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_35
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_36
MASK_POLL_32	 0x18570, 0x1f8003c, 0x0
SAVE_TIMESTAMPS	 5
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x618000, @DMAWRITE_data_0, 0x36, 0, 1
	 UC_DMA_BD	 0, 0x6ae0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x10000, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10010, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110000, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110010, @WRITE_data_1, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_1, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x108600, @DMAWRITE_data_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_1, 1, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x110020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110030, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110050, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x108c00, @DMAWRITE_data_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x108f00, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x108000, @DMAWRITE_data_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10000, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10010, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110060, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110070, @WRITE_data_1, 1, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_6, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x1855c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x148900, @DMAWRITE_data_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e1c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x718000, @DMAWRITE_data_8, 0x36, 0, 1
	 UC_DMA_BD	 0, 0x7ae0e0, @WRITE_data_0, 1, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x10020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10030, @WRITE_data_1, 1, 0, 0
UCBD_label_7:
	 UC_DMA_BD	 0, 0x58000, @DMAWRITE_data_9, 0x9, 0, 0
UCBD_label_8:
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_1, 1, 0, 0
UCBD_label_9:
	 UC_DMA_BD	 0, 0x110080, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110090, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x148600, @DMAWRITE_data_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_1, 1, 0, 0
UCBD_label_10:
	 UC_DMA_BD	 0, 0x1100a0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100b0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109500, @DMAWRITE_data_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109200, @DMAWRITE_data_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x108300, @DMAWRITE_data_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100c0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100d0, @WRITE_data_1, 1, 0, 0
UCBD_label_11:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_14, 0x9, 0, 0
UCBD_label_12:
	 UC_DMA_BD	 0, 0x1855c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x148900, @DMAWRITE_data_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e1c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10030, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100e0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100f0, @WRITE_data_1, 1, 0, 0
UCBD_label_13:
	 UC_DMA_BD	 0, 0x58000, @DMAWRITE_data_16, 0x9, 0, 0
UCBD_label_14:
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x148600, @DMAWRITE_data_17, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110100, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110110, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109530, @DMAWRITE_data_18, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109230, @DMAWRITE_data_19, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x108330, @DMAWRITE_data_20, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109830, @DMAWRITE_data_21, 0xb, 0, 0
UCBD_label_15:
	 UC_DMA_BD	 0, 0x18060, @DMAWRITE_data_22, 0x9, 0, 0
UCBD_label_16:
	 UC_DMA_BD	 0, 0x718400, @DMAWRITE_data_23, 0x36, 0, 1
	 UC_DMA_BD	 0, 0x7ae0f0, @WRITE_data_0, 1, 0, 0
UCBD_label_17:
	 UC_DMA_BD	 0, 0x10040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10050, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109800, @DMAWRITE_data_24, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_1, 1, 0, 0
UCBD_label_18:
	 UC_DMA_BD	 0, 0x18030, @DMAWRITE_data_25, 0x9, 0, 0
UCBD_label_19:
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_0, 1, 0, 0
UCBD_label_20:
	 UC_DMA_BD	 0, 0x110120, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110130, @WRITE_data_1, 1, 0, 0
UCBD_label_21:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_26, 0x9, 0, 0
UCBD_label_22:
	 UC_DMA_BD	 0, 0x1855c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x148900, @DMAWRITE_data_27, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e1c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10030, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110140, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110150, @WRITE_data_1, 1, 0, 0
UCBD_label_23:
	 UC_DMA_BD	 0, 0x58000, @DMAWRITE_data_28, 0x9, 0, 0
UCBD_label_24:
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x148600, @DMAWRITE_data_29, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110160, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110170, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109560, @DMAWRITE_data_30, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109260, @DMAWRITE_data_31, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x108360, @DMAWRITE_data_32, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109860, @DMAWRITE_data_33, 0xb, 0, 0
UCBD_label_25:
	 UC_DMA_BD	 0, 0x18090, @DMAWRITE_data_34, 0x9, 0, 0
UCBD_label_26:
	 UC_DMA_BD	 0, 0x718000, @DMAWRITE_data_35, 0x2e, 0, 1
	 UC_DMA_BD	 0, 0x7ae0e0, @WRITE_data_0, 1, 0, 0
UCBD_label_27:
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_2, 1, 0, 0
UCBD_label_28:
	 UC_DMA_BD	 0, 0x110180, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110190, @WRITE_data_1, 1, 0, 0
UCBD_label_29:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_36, 0x9, 0, 0
UCBD_label_30:
	 UC_DMA_BD	 0, 0x1855c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x148900, @DMAWRITE_data_37, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e1c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10030, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101a0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101b0, @WRITE_data_1, 1, 0, 0
UCBD_label_31:
	 UC_DMA_BD	 0, 0x58000, @DMAWRITE_data_38, 0x9, 0, 0
UCBD_label_32:
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x148600, @DMAWRITE_data_39, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101c0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101d0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109590, @DMAWRITE_data_40, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x109290, @DMAWRITE_data_41, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x108390, @DMAWRITE_data_42, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x109890, @DMAWRITE_data_43, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x718400, @DMAWRITE_data_44, 0x2e, 0, 1
	 UC_DMA_BD	 0, 0x7ae0f0, @WRITE_data_0, 1, 0, 0
UCBD_label_33:
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_3, 1, 0, 0
UCBD_label_34:
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_3, 1, 0, 0
UCBD_label_35:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_45, 0x9, 0, 0
UCBD_label_36:
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_1, 1, 0, 0
.align    4
DMAWRITE_data_0:
	.long 0x00000820
	.long 0x00000001
	.long 0x000f801c
	.long 0x000f8068
	.long 0x000f806c
	.long 0x000f8080
	.long 0x000f8098
	.long 0x00000009
	.long 0x000ae000
	.long 0x00000001
	.long 0x000ae010
	.long 0x00000000
	.long 0x000ac000
	.long 0x20000004
	.long 0x000ac004
	.long 0x00000000
	.long 0x000ac008
	.long 0x00010001
	.long 0x000ac00c
	.long 0x00000001
	.long 0x000ac010
	.long 0x00000001
	.long 0x000ac014
	.long 0x02041fe1
	.long 0x000ace14
	.long 0xffff0000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x000e8000
	.long 0x000e8000
	.long 0x00000000
	.long 0x00000030
	.long 0x00000031
	.long 0x00000000
	.long 0x00000040
	.long 0x00000004
	.long 0x00000080
	.long 0x00000020
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
WRITE_data_0:
	.long 0x00000001
WRITE_data_1:
	.long 0x00000000
DMAWRITE_data_1:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000020
	.long 0x0f000000
	.long 0x01000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_2:
	.long 0x00800000
	.long 0x00000020
	.long 0x1c000b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_3:
	.long 0x00800600
	.long 0x00000010
	.long 0x1c300385
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000200
	.long 0x00000000
DMAWRITE_data_4:
	.long 0x00800000
	.long 0x02000020
	.long 0x1c100381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_5:
	.long 0x00808000
	.long 0x00000004
	.long 0x1c400b8b
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000080
	.long 0x00000000
DMAWRITE_data_6:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000020
	.long 0x0f000000
	.long 0x01000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_7:
	.long 0x00801800
	.long 0x00000020
	.long 0x1c600b8f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_8:
	.long 0x00000820
	.long 0x00000000
	.long 0x000f801c
	.long 0x000f8068
	.long 0x000f806c
	.long 0x000f8080
	.long 0x000f8098
	.long 0x00000009
	.long 0x000ae000
	.long 0x00000001
	.long 0x000ae010
	.long 0x00000000
	.long 0x000ac000
	.long 0x20000020
	.long 0x000ac004
	.long 0x00000000
	.long 0x000ac008
	.long 0x00010001
	.long 0x000ac00c
	.long 0x00000001
	.long 0x000ac010
	.long 0x00000001
	.long 0x000ac014
	.long 0x02041fe1
	.long 0x000ace14
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x000e8000
	.long 0x000e8000
	.long 0x00000000
	.long 0x00000030
	.long 0x00000031
	.long 0x00000001
	.long 0x00000040
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_9:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000020
	.long 0x1f000000
	.long 0x01000100
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_10:
	.long 0x00800800
	.long 0x00000020
	.long 0x1c800b93
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_11:
	.long 0x00801800
	.long 0x02000020
	.long 0x1c70038d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_12:
	.long 0x00800800
	.long 0x02000020
	.long 0x1c900391
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_13:
	.long 0x00801000
	.long 0x00000020
	.long 0x1ca00b97
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_14:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000020
	.long 0x0f000000
	.long 0x01000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_15:
	.long 0x00801900
	.long 0x00000020
	.long 0x1cc00b9b
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_16:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000020
	.long 0x1f000000
	.long 0x01000100
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_17:
	.long 0x00800900
	.long 0x00000020
	.long 0x1ce00b9f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_18:
	.long 0x00801900
	.long 0x02000020
	.long 0x1cd00399
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_19:
	.long 0x00800900
	.long 0x02000020
	.long 0x1cf0039d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_20:
	.long 0x00801100
	.long 0x00000020
	.long 0x1d000ba3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_21:
	.long 0x00801100
	.long 0x02000020
	.long 0x1d1003a1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_22:
	.long 0x00000000
	.long 0x00000080
	.long 0x00000020
	.long 0x27000000
	.long 0x01000280
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_23:
	.long 0x00000820
	.long 0x00000000
	.long 0x000f841c
	.long 0x000f8468
	.long 0x000f846c
	.long 0x000f8480
	.long 0x000f8498
	.long 0x00000009
	.long 0x000ae020
	.long 0x00000001
	.long 0x000ae030
	.long 0x00000000
	.long 0x000ac020
	.long 0x20000020
	.long 0x000ac024
	.long 0x00000000
	.long 0x000ac028
	.long 0x00010001
	.long 0x000ac02c
	.long 0x00000001
	.long 0x000ac030
	.long 0x00000001
	.long 0x000ac034
	.long 0x02045fe3
	.long 0x000ace14
	.long 0x00000001
	.long 0x00000000
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x000e8000
	.long 0x000e8000
	.long 0x00000000
	.long 0x00000032
	.long 0x00000033
	.long 0x00000001
	.long 0x00000040
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_24:
	.long 0x00801000
	.long 0x02000020
	.long 0x1cb00395
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_25:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000020
	.long 0x27000000
	.long 0x01000280
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_26:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000020
	.long 0x0f000000
	.long 0x01000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_27:
	.long 0x00801a00
	.long 0x00000020
	.long 0x1d200ba7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_28:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000020
	.long 0x1f000000
	.long 0x01000100
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_29:
	.long 0x00800a00
	.long 0x00000020
	.long 0x1d400bab
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_30:
	.long 0x00801a00
	.long 0x02000020
	.long 0x1d3003a5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
WRITE_data_2:
	.long 0x00000002
DMAWRITE_data_31:
	.long 0x00800a00
	.long 0x02000020
	.long 0x1d5003a9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_32:
	.long 0x00801200
	.long 0x00000020
	.long 0x1d600baf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_33:
	.long 0x00801200
	.long 0x02000020
	.long 0x1d7003ad
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_34:
	.long 0x00000000
	.long 0x00000100
	.long 0x00000020
	.long 0x27000000
	.long 0x01000280
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_35:
	.long 0x00000820
	.long 0x00000000
	.long 0x000f801c
	.long 0x000f8040
	.long 0x000f8044
	.long 0x000f8060
	.long 0x000f8078
	.long 0x00000004
	.long 0x000ae000
	.long 0x00000001
	.long 0x000ae010
	.long 0x00000000
	.long 0x000ac00c
	.long 0x00000001
	.long 0x000ace14
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x000e8000
	.long 0x000e8000
	.long 0x00000000
	.long 0x00000030
	.long 0x00000031
	.long 0x00000001
	.long 0x00000040
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_36:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000020
	.long 0x0f000000
	.long 0x01000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_37:
	.long 0x00801b00
	.long 0x00000020
	.long 0x1d800bb3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_38:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000020
	.long 0x1f000000
	.long 0x01000100
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
DMAWRITE_data_39:
	.long 0x00800b00
	.long 0x00000020
	.long 0x1da00bb7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_40:
	.long 0x00801b00
	.long 0x02000020
	.long 0x1d9003b1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
WRITE_data_3:
	.long 0x00000003
DMAWRITE_data_41:
	.long 0x00800b00
	.long 0x02000020
	.long 0x1db003b5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_42:
	.long 0x00801300
	.long 0x00000020
	.long 0x1dc00bbb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_43:
	.long 0x00801300
	.long 0x02000020
	.long 0x1dd003b9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
DMAWRITE_data_44:
	.long 0x00000820
	.long 0x00000001
	.long 0x000f841c
	.long 0x000f8440
	.long 0x000f8444
	.long 0x000f8460
	.long 0x000f8478
	.long 0x00000004
	.long 0x000ae020
	.long 0x00000001
	.long 0x000ae030
	.long 0x00000000
	.long 0x000ac02c
	.long 0x00000001
	.long 0x000ace14
	.long 0x00000001
	.long 0x00000000
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x000e8000
	.long 0x000e8000
	.long 0x00000000
	.long 0x00000032
	.long 0x00000033
	.long 0x00000001
	.long 0x00000040
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_45:
	.long 0x00000000
	.long 0x00000180
	.long 0x00000020
	.long 0x27000000
	.long 0x01000280
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
