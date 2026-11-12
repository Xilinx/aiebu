;
;text
;
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
APPLY_OFFSET_57	 @DMAWRITE_data_0, 1, 11
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
APPLY_OFFSET_57	 @DMAWRITE_data_2, 1, 9
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
MASK_POLL_32	 0x1e1d0604, 0x1, 0x1
MASK_POLL_32	 0x181d0604, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
MASK_POLL_32	 0x1a1a0680, 0x78003c, 0x0
MASK_POLL_32	 0x1a1a0684, 0x78003c, 0x0
MASK_POLL_32	 0x1a1a0688, 0x78003c, 0x0
MASK_POLL_32	 0x1a1a068c, 0x78003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
MASK_POLL_32	 0x1a1a0660, 0x78003c, 0x0
MASK_POLL_32	 0x1a1a0664, 0x78003c, 0x0
MASK_POLL_32	 0x1a1a0668, 0x78003c, 0x0
MASK_POLL_32	 0x1a1a066c, 0x78003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
APPLY_OFFSET_57	 @DMAWRITE_data_49, 1, 10
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
MASK_POLL_32	 0x1c00c604, 0x1, 0x1
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x1e000000, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e000010, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e1c0000, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e1c0010, @WRITE_data_3, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x1e009000, @DMAWRITE_data_0, 0x9, 0, 1
	 UC_DMA_BD	 0, 0x1e009314, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e1a0000, @DMAWRITE_data_1, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1e1a0604, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c000000, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c000010, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x181c0000, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x181c0010, @WRITE_data_9, 1, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x1c009000, @DMAWRITE_data_2, 0x9, 0, 1
	 UC_DMA_BD	 0, 0x1c009314, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x181a0000, @DMAWRITE_data_3, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x181a0604, @WRITE_data_11, 1, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x1a1c0000, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1c0010, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0000, @DMAWRITE_data_4, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0634, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0020, @DMAWRITE_data_5, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0634, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0040, @DMAWRITE_data_6, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0634, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0300, @DMAWRITE_data_7, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a063c, @WRITE_data_17, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0320, @DMAWRITE_data_8, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a063c, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0340, @DMAWRITE_data_9, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a063c, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0060, @DMAWRITE_data_10, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0644, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0080, @DMAWRITE_data_11, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0644, @WRITE_data_21, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a00a0, @DMAWRITE_data_12, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0644, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0360, @DMAWRITE_data_13, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a064c, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0380, @DMAWRITE_data_14, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a064c, @WRITE_data_24, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a03a0, @DMAWRITE_data_15, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a064c, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e1a0000, @DMAWRITE_data_16, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1e1a0634, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e1a0020, @DMAWRITE_data_17, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1e1a0634, @WRITE_data_27, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e1a0300, @DMAWRITE_data_18, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1e1a063c, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e1a0320, @DMAWRITE_data_19, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1e1a063c, @WRITE_data_29, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a02e0, @DMAWRITE_data_20, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a02c0, @DMAWRITE_data_21, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a02a0, @DMAWRITE_data_22, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0280, @DMAWRITE_data_23, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0604, @WRITE_data_30, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a05e0, @DMAWRITE_data_24, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a05c0, @DMAWRITE_data_25, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a05a0, @DMAWRITE_data_26, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0580, @DMAWRITE_data_27, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a060c, @WRITE_data_31, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0260, @DMAWRITE_data_28, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0240, @DMAWRITE_data_29, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0220, @DMAWRITE_data_30, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0200, @DMAWRITE_data_31, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0614, @WRITE_data_32, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0560, @DMAWRITE_data_32, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0540, @DMAWRITE_data_33, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0520, @DMAWRITE_data_34, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0500, @DMAWRITE_data_35, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a061c, @WRITE_data_33, 1, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x1a1a0000, @DMAWRITE_data_36, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0634, @WRITE_data_34, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0020, @DMAWRITE_data_37, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0634, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0040, @DMAWRITE_data_38, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0634, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0300, @DMAWRITE_data_39, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a063c, @WRITE_data_37, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0320, @DMAWRITE_data_40, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a063c, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0340, @DMAWRITE_data_41, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a063c, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0060, @DMAWRITE_data_42, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0644, @WRITE_data_40, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0080, @DMAWRITE_data_43, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0644, @WRITE_data_41, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a00a0, @DMAWRITE_data_44, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0644, @WRITE_data_42, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0360, @DMAWRITE_data_45, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a064c, @WRITE_data_43, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a0380, @DMAWRITE_data_46, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a064c, @WRITE_data_44, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1a03a0, @DMAWRITE_data_47, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a1a064c, @WRITE_data_45, 1, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x1c000000, @WRITE_data_46, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c000010, @WRITE_data_47, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c1a0000, @DMAWRITE_data_48, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1c1a0634, @WRITE_data_48, 1, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x1c009000, @DMAWRITE_data_49, 0x9, 0, 1
	 UC_DMA_BD	 0, 0x1c009304, @WRITE_data_49, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x00000000
WRITE_data_1:
	.long 0x00000000
WRITE_data_2:
	.long 0x00000000
WRITE_data_3:
	.long 0x00000000
WRITE_data_4:
	.long 0x00000000
WRITE_data_5:
	.long 0x00000000
WRITE_data_6:
	.long 0x00000000
WRITE_data_7:
	.long 0x00000000
WRITE_data_8:
	.long 0x00000000
WRITE_data_9:
	.long 0x00000000
WRITE_data_10:
	.long 0x00000000
WRITE_data_11:
	.long 0x00000000
WRITE_data_12:
	.long 0x00000000
WRITE_data_13:
	.long 0x00000000
WRITE_data_14:
	.long 0x00000000
WRITE_data_15:
	.long 0x00010001
WRITE_data_16:
	.long 0x00000002
WRITE_data_17:
	.long 0x00000018
WRITE_data_18:
	.long 0x00010019
WRITE_data_19:
	.long 0x0000001a
WRITE_data_20:
	.long 0x00000003
WRITE_data_21:
	.long 0x00010004
WRITE_data_22:
	.long 0x00000005
WRITE_data_23:
	.long 0x0000001b
WRITE_data_24:
	.long 0x0001001c
WRITE_data_25:
	.long 0x0000001d
WRITE_data_26:
	.long 0x00030000
WRITE_data_27:
	.long 0x00030001
WRITE_data_28:
	.long 0x00030018
WRITE_data_29:
	.long 0x00030019
WRITE_data_30:
	.long 0x000f0000
WRITE_data_31:
	.long 0x000f0018
WRITE_data_32:
	.long 0x000f0000
WRITE_data_33:
	.long 0x000f0018
WRITE_data_34:
	.long 0x00000000
WRITE_data_35:
	.long 0x00010001
WRITE_data_36:
	.long 0x00000002
WRITE_data_37:
	.long 0x00000018
WRITE_data_38:
	.long 0x00010019
WRITE_data_39:
	.long 0x0000001a
WRITE_data_40:
	.long 0x00000003
WRITE_data_41:
	.long 0x00010004
WRITE_data_42:
	.long 0x00000005
WRITE_data_43:
	.long 0x0000001b
WRITE_data_44:
	.long 0x0001001c
WRITE_data_45:
	.long 0x0000001d
WRITE_data_46:
	.long 0x00000000
WRITE_data_47:
	.long 0x00000000
WRITE_data_48:
	.long 0x00000000
WRITE_data_49:
	.long 0x00000000
DMAWRITE_data_0:
	.long 0x00004d00
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x80000000
	.long 0x00000000
	.long 0x00000000
	.long 0x02001001
	.long 0x00000000
DMAWRITE_data_1:
	.long 0x00004d00
	.long 0x00020000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81418040
DMAWRITE_data_2:
	.long 0x0000d000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x80000000
	.long 0x00000000
	.long 0x00000000
	.long 0x02001001
	.long 0x00000000
DMAWRITE_data_3:
	.long 0x0000d000
	.long 0x00020000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81418040
DMAWRITE_data_4:
	.long 0x00001400
	.long 0x08000000
	.long 0x004c0000
	.long 0x0004007f
	.long 0x081e03ff
	.long 0x000000ff
	.long 0x00000000
	.long 0x80008101
DMAWRITE_data_5:
	.long 0x00001400
	.long 0x08003400
	.long 0x004c0000
	.long 0x0004007f
	.long 0x002003ff
	.long 0x000000ff
	.long 0x000237ff
	.long 0x80008101
DMAWRITE_data_6:
	.long 0x00001400
	.long 0x0800a400
	.long 0x004c0000
	.long 0x0004007f
	.long 0x001603ff
	.long 0x500000ff
	.long 0x00000000
	.long 0x80008101
DMAWRITE_data_7:
	.long 0x00001400
	.long 0x0000001e
	.long 0x00500000
	.long 0x0004007f
	.long 0x081e03ff
	.long 0x000000ff
	.long 0x00000000
	.long 0x80008101
DMAWRITE_data_8:
	.long 0x00001400
	.long 0x0000341e
	.long 0x00500000
	.long 0x0004007f
	.long 0x002003ff
	.long 0x000000ff
	.long 0x000237ff
	.long 0x80008101
DMAWRITE_data_9:
	.long 0x00001400
	.long 0x0000a41e
	.long 0x00500000
	.long 0x0004007f
	.long 0x001603ff
	.long 0x500000ff
	.long 0x00000000
	.long 0x80008101
DMAWRITE_data_10:
	.long 0x00001400
	.long 0x0000003e
	.long 0x00500000
	.long 0x0004007f
	.long 0x081e03ff
	.long 0x000000ff
	.long 0x00000000
	.long 0x80008101
DMAWRITE_data_11:
	.long 0x00001400
	.long 0x0000343e
	.long 0x00500000
	.long 0x0004007f
	.long 0x002003ff
	.long 0x000000ff
	.long 0x000237ff
	.long 0x80008101
DMAWRITE_data_12:
	.long 0x00001400
	.long 0x0000a43e
	.long 0x00500000
	.long 0x0004007f
	.long 0x001603ff
	.long 0x500000ff
	.long 0x00000000
	.long 0x80008101
DMAWRITE_data_13:
	.long 0x00001400
	.long 0x0000005e
	.long 0x00440000
	.long 0x0004007f
	.long 0x081e03ff
	.long 0x000c00ff
	.long 0x00000000
	.long 0x80008101
DMAWRITE_data_14:
	.long 0x00001400
	.long 0x0000345e
	.long 0x00440000
	.long 0x0004007f
	.long 0x002003ff
	.long 0x000c00ff
	.long 0x000237ff
	.long 0x80008101
DMAWRITE_data_15:
	.long 0x00001400
	.long 0x0000a45e
	.long 0x00440000
	.long 0x0004007f
	.long 0x001603ff
	.long 0x500c00ff
	.long 0x00000000
	.long 0x80008101
DMAWRITE_data_16:
	.long 0x00001340
	.long 0x00020000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x80408141
DMAWRITE_data_17:
	.long 0x00001340
	.long 0x00022680
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x80408141
DMAWRITE_data_18:
	.long 0x00001340
	.long 0x00021340
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x80408141
DMAWRITE_data_19:
	.long 0x00001340
	.long 0x000239c0
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x80408141
DMAWRITE_data_20:
	.long 0x000001c0
	.long 0x00030000
	.long 0x00200000
	.long 0x0008003f
	.long 0x000e03ff
	.long 0x00000000
	.long 0x00061bff
	.long 0x80418040
DMAWRITE_data_21:
	.long 0x000001c0
	.long 0x00030100
	.long 0x00200000
	.long 0x0008003f
	.long 0x000e03ff
	.long 0x00000000
	.long 0x00061bff
	.long 0x80418040
DMAWRITE_data_22:
	.long 0x000001c0
	.long 0x00030200
	.long 0x00200000
	.long 0x0008003f
	.long 0x000e03ff
	.long 0x00000000
	.long 0x00061bff
	.long 0x80418040
DMAWRITE_data_23:
	.long 0x000001c0
	.long 0x00030300
	.long 0x00200000
	.long 0x0008003f
	.long 0x000e03ff
	.long 0x00000000
	.long 0x00061bff
	.long 0x80418040
DMAWRITE_data_24:
	.long 0x000001c0
	.long 0x00030010
	.long 0x00200000
	.long 0x0008003f
	.long 0x000e03ff
	.long 0x00000000
	.long 0x00061bff
	.long 0x80418040
DMAWRITE_data_25:
	.long 0x000001c0
	.long 0x00030110
	.long 0x00200000
	.long 0x0008003f
	.long 0x000e03ff
	.long 0x00000000
	.long 0x00061bff
	.long 0x80418040
DMAWRITE_data_26:
	.long 0x000001c0
	.long 0x00030210
	.long 0x00200000
	.long 0x0008003f
	.long 0x000e03ff
	.long 0x00000000
	.long 0x00061bff
	.long 0x80418040
DMAWRITE_data_27:
	.long 0x000001c0
	.long 0x00030310
	.long 0x00200000
	.long 0x0008003f
	.long 0x000e03ff
	.long 0x00000000
	.long 0x00061bff
	.long 0x80418040
DMAWRITE_data_28:
	.long 0x000001c0
	.long 0x00030020
	.long 0x00200000
	.long 0x0008003f
	.long 0x000e03ff
	.long 0x00000000
	.long 0x00061bff
	.long 0x80418040
DMAWRITE_data_29:
	.long 0x000001c0
	.long 0x00030120
	.long 0x00200000
	.long 0x0008003f
	.long 0x000e03ff
	.long 0x00000000
	.long 0x00061bff
	.long 0x80418040
DMAWRITE_data_30:
	.long 0x000001c0
	.long 0x00030220
	.long 0x00200000
	.long 0x0008003f
	.long 0x000e03ff
	.long 0x00000000
	.long 0x00061bff
	.long 0x80418040
DMAWRITE_data_31:
	.long 0x000001c0
	.long 0x00030320
	.long 0x00200000
	.long 0x0008003f
	.long 0x000e03ff
	.long 0x00000000
	.long 0x00061bff
	.long 0x80418040
DMAWRITE_data_32:
	.long 0x000001c0
	.long 0x00030030
	.long 0x00200000
	.long 0x0008003f
	.long 0x000e03ff
	.long 0x00000000
	.long 0x00061bff
	.long 0x80418040
DMAWRITE_data_33:
	.long 0x000001c0
	.long 0x00030130
	.long 0x00200000
	.long 0x0008003f
	.long 0x000e03ff
	.long 0x00000000
	.long 0x00061bff
	.long 0x80418040
DMAWRITE_data_34:
	.long 0x000001c0
	.long 0x00030230
	.long 0x00200000
	.long 0x0008003f
	.long 0x000e03ff
	.long 0x00000000
	.long 0x00061bff
	.long 0x80418040
DMAWRITE_data_35:
	.long 0x000001c0
	.long 0x00030330
	.long 0x00200000
	.long 0x0008003f
	.long 0x000e03ff
	.long 0x00000000
	.long 0x00061bff
	.long 0x80418040
DMAWRITE_data_36:
	.long 0x00001400
	.long 0x08000000
	.long 0x004c0000
	.long 0x0004007f
	.long 0x081e03ff
	.long 0x000000ff
	.long 0x00000000
	.long 0x80008101
DMAWRITE_data_37:
	.long 0x00001400
	.long 0x08003400
	.long 0x004c0000
	.long 0x0004007f
	.long 0x002003ff
	.long 0x000000ff
	.long 0x000237ff
	.long 0x80008101
DMAWRITE_data_38:
	.long 0x00001400
	.long 0x0800a400
	.long 0x004c0000
	.long 0x0004007f
	.long 0x001603ff
	.long 0x500000ff
	.long 0x00000000
	.long 0x80008101
DMAWRITE_data_39:
	.long 0x00001400
	.long 0x0000001e
	.long 0x00500000
	.long 0x0004007f
	.long 0x081e03ff
	.long 0x000000ff
	.long 0x00000000
	.long 0x80008101
DMAWRITE_data_40:
	.long 0x00001400
	.long 0x0000341e
	.long 0x00500000
	.long 0x0004007f
	.long 0x002003ff
	.long 0x000000ff
	.long 0x000237ff
	.long 0x80008101
DMAWRITE_data_41:
	.long 0x00001400
	.long 0x0000a41e
	.long 0x00500000
	.long 0x0004007f
	.long 0x001603ff
	.long 0x500000ff
	.long 0x00000000
	.long 0x80008101
DMAWRITE_data_42:
	.long 0x00001400
	.long 0x0000003e
	.long 0x00500000
	.long 0x0004007f
	.long 0x081e03ff
	.long 0x000000ff
	.long 0x00000000
	.long 0x80008101
DMAWRITE_data_43:
	.long 0x00001400
	.long 0x0000343e
	.long 0x00500000
	.long 0x0004007f
	.long 0x002003ff
	.long 0x000000ff
	.long 0x000237ff
	.long 0x80008101
DMAWRITE_data_44:
	.long 0x00001400
	.long 0x0000a43e
	.long 0x00500000
	.long 0x0004007f
	.long 0x001603ff
	.long 0x500000ff
	.long 0x00000000
	.long 0x80008101
DMAWRITE_data_45:
	.long 0x00001400
	.long 0x0000005e
	.long 0x00440000
	.long 0x0004007f
	.long 0x081e03ff
	.long 0x000c00ff
	.long 0x00000000
	.long 0x80008101
DMAWRITE_data_46:
	.long 0x00001400
	.long 0x0000345e
	.long 0x00440000
	.long 0x0004007f
	.long 0x002003ff
	.long 0x000c00ff
	.long 0x000237ff
	.long 0x80008101
DMAWRITE_data_47:
	.long 0x00001400
	.long 0x0000a45e
	.long 0x00440000
	.long 0x0004007f
	.long 0x001603ff
	.long 0x500c00ff
	.long 0x00000000
	.long 0x80008101
DMAWRITE_data_48:
	.long 0x00007000
	.long 0x00010000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x80008001
DMAWRITE_data_49:
	.long 0x00007000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x80000000
	.long 0x00000000
	.long 0x00000000
	.long 0x02043000
	.long 0x00000000
