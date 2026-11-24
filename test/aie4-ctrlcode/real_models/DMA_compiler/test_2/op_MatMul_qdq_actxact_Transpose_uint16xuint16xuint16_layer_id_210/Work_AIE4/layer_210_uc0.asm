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
; SHIM_WGT
APPLY_OFFSET_57	 @DMAWRITE_data_68, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_69, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_70, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_71, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_72, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_73, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
; SHIM_IFM_1
APPLY_OFFSET_57	 @DMAWRITE_data_74, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_7
; SHIM_IFM_1
APPLY_OFFSET_57	 @DMAWRITE_data_75, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_8
; SHIM_IFM_1
APPLY_OFFSET_57	 @DMAWRITE_data_76, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_9
; SHIM_IFM_1
APPLY_OFFSET_57	 @DMAWRITE_data_77, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_10
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_11
MASK_POLL_32	 0x109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_12
MASK_POLL_32	 0x109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_13
MASK_POLL_32	 0x109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_14
MASK_POLL_32	 0x149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_15
MASK_POLL_32	 0x149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_16
MASK_POLL_32	 0x109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_17
MASK_POLL_32	 0x109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_18
MASK_POLL_32	 0x149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_19
MASK_POLL_32	 0x149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_20
MASK_POLL_32	 0x109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_21
MASK_POLL_32	 0x109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_22
MASK_POLL_32	 0x109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_23
MASK_POLL_32	 0x109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_24
MASK_POLL_32	 0x149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_25
MASK_POLL_32	 0x149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_26
MASK_POLL_32	 0x109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_27
MASK_POLL_32	 0x109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_28
MASK_POLL_32	 0x149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_29
MASK_POLL_32	 0x149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_30
MASK_POLL_32	 0x109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_31
MASK_POLL_32	 0x109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_32
MASK_POLL_32	 0x149ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_33
MASK_POLL_32	 0x109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_34
MASK_POLL_32	 0x109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_35
END_JOB

.eop

START_JOB 1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_36
MASK_POLL_32	 0x149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_37
MASK_POLL_32	 0x149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_38
MASK_POLL_32	 0x109f04, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_39
MASK_POLL_32	 0x109f0c, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_40
MASK_POLL_32	 0x109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_41
MASK_POLL_32	 0x109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_42
MASK_POLL_32	 0x149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_43
MASK_POLL_32	 0x149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_44
MASK_POLL_32	 0x109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_45
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
	 UC_DMA_BD	 0, 0x1100c0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100d0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100e0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100f0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110100, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110110, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110120, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110130, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110140, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110150, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110160, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110170, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110180, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110190, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101a0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101b0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101c0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101d0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101e0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101f0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x150000, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x150010, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x150020, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x150030, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x150030, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x150040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x150040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x150050, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x150050, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x150060, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x150070, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x150080, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x150090, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1500a0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1500b0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1500c0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1500d0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1500e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1500f0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x150100, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x150110, @WRITE_data_0, 1, 0, 1
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
	 UC_DMA_BD	 0, 0x108900, @DMAWRITE_data_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108930, @DMAWRITE_data_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148000, @DMAWRITE_data_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148030, @DMAWRITE_data_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148300, @DMAWRITE_data_16, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148330, @DMAWRITE_data_17, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148600, @DMAWRITE_data_18, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148630, @DMAWRITE_data_19, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148660, @DMAWRITE_data_20, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148690, @DMAWRITE_data_21, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1486c0, @DMAWRITE_data_22, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1486f0, @DMAWRITE_data_23, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148720, @DMAWRITE_data_24, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148750, @DMAWRITE_data_25, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148780, @DMAWRITE_data_26, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1487b0, @DMAWRITE_data_27, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1487e0, @DMAWRITE_data_28, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148810, @DMAWRITE_data_29, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148840, @DMAWRITE_data_30, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148900, @DMAWRITE_data_31, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c00, @DMAWRITE_data_32, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c30, @DMAWRITE_data_33, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c60, @DMAWRITE_data_34, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f00, @DMAWRITE_data_35, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f30, @DMAWRITE_data_36, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f60, @DMAWRITE_data_37, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f90, @DMAWRITE_data_38, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108fc0, @DMAWRITE_data_39, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108ff0, @DMAWRITE_data_40, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109020, @DMAWRITE_data_41, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109050, @DMAWRITE_data_42, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109200, @DMAWRITE_data_43, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109230, @DMAWRITE_data_44, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109260, @DMAWRITE_data_45, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109500, @DMAWRITE_data_46, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109530, @DMAWRITE_data_47, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109560, @DMAWRITE_data_48, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109590, @DMAWRITE_data_49, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1095c0, @DMAWRITE_data_50, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1095f0, @DMAWRITE_data_51, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109620, @DMAWRITE_data_52, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109650, @DMAWRITE_data_53, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109800, @DMAWRITE_data_54, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109830, @DMAWRITE_data_55, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109860, @DMAWRITE_data_56, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109890, @DMAWRITE_data_57, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1098c0, @DMAWRITE_data_58, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1098f0, @DMAWRITE_data_59, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109920, @DMAWRITE_data_60, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109950, @DMAWRITE_data_61, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c00, @DMAWRITE_data_62, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c30, @DMAWRITE_data_63, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c60, @DMAWRITE_data_64, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149200, @DMAWRITE_data_65, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149230, @DMAWRITE_data_66, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149260, @DMAWRITE_data_67, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e1c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e1c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x110000, @WRITE_data_0, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_68, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x18030, @DMAWRITE_data_69, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x18060, @DMAWRITE_data_70, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x18090, @DMAWRITE_data_71, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x180c0, @DMAWRITE_data_72, 0x9, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x180f0, @DMAWRITE_data_73, 0x9, 0, 0
UCBD_label_7:
	 UC_DMA_BD	 0, 0x58030, @DMAWRITE_data_74, 0x9, 0, 0
UCBD_label_8:
	 UC_DMA_BD	 0, 0x58060, @DMAWRITE_data_75, 0x9, 0, 0
UCBD_label_9:
	 UC_DMA_BD	 0, 0x58090, @DMAWRITE_data_76, 0x9, 0, 0
UCBD_label_10:
	 UC_DMA_BD	 0, 0x580c0, @DMAWRITE_data_77, 0x9, 0, 0
UCBD_label_11:
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x1855c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x10000, @WRITE_data_0, 1, 0, 0
UCBD_label_12:
	 UC_DMA_BD	 0, 0x1086c0, @DMAWRITE_data_78, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_3, 1, 0, 0
UCBD_label_13:
	 UC_DMA_BD	 0, 0x108c60, @DMAWRITE_data_79, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_2, 1, 0, 0
UCBD_label_14:
	 UC_DMA_BD	 0, 0x109260, @DMAWRITE_data_80, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_2, 1, 0, 0
UCBD_label_15:
	 UC_DMA_BD	 0, 0x148c60, @DMAWRITE_data_81, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_2, 1, 0, 0
UCBD_label_16:
	 UC_DMA_BD	 0, 0x149260, @DMAWRITE_data_82, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_2, 1, 0, 0
UCBD_label_17:
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_1, 1, 0, 0
UCBD_label_18:
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_19:
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_1, 1, 0, 0
UCBD_label_20:
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_21:
	 UC_DMA_BD	 0, 0x1098c0, @DMAWRITE_data_83, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_3, 1, 0, 0
UCBD_label_22:
	 UC_DMA_BD	 0, 0x1086c0, @DMAWRITE_data_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_3, 1, 0, 0
UCBD_label_23:
	 UC_DMA_BD	 0, 0x108c60, @DMAWRITE_data_34, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_2, 1, 0, 0
UCBD_label_24:
	 UC_DMA_BD	 0, 0x109260, @DMAWRITE_data_45, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_2, 1, 0, 0
UCBD_label_25:
	 UC_DMA_BD	 0, 0x148c60, @DMAWRITE_data_64, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_2, 1, 0, 0
UCBD_label_26:
	 UC_DMA_BD	 0, 0x149260, @DMAWRITE_data_67, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_2, 1, 0, 0
UCBD_label_27:
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_1, 1, 0, 0
UCBD_label_28:
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_29:
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_1, 1, 0, 0
UCBD_label_30:
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_31:
	 UC_DMA_BD	 0, 0x1098c0, @DMAWRITE_data_58, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_3, 1, 0, 0
UCBD_label_32:
	 UC_DMA_BD	 0, 0x1086c0, @DMAWRITE_data_78, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_3, 1, 0, 0
UCBD_label_33:
	 UC_DMA_BD	 0, 0x148810, @DMAWRITE_data_84, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_8, 1, 0, 0
UCBD_label_34:
	 UC_DMA_BD	 0, 0x108c60, @DMAWRITE_data_79, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_2, 1, 0, 0
UCBD_label_35:
	 UC_DMA_BD	 0, 0x109260, @DMAWRITE_data_80, 0xb, 0, 0
UCBD_label_36:
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_14, 1, 0, 0
UCBD_label_37:
	 UC_DMA_BD	 0, 0x148c60, @DMAWRITE_data_85, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_14, 1, 0, 0
UCBD_label_38:
	 UC_DMA_BD	 0, 0x149260, @DMAWRITE_data_86, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_14, 1, 0, 0
UCBD_label_39:
	 UC_DMA_BD	 0, 0x109050, @DMAWRITE_data_87, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_15, 1, 0, 0
UCBD_label_40:
	 UC_DMA_BD	 0, 0x109650, @DMAWRITE_data_88, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_15, 1, 0, 0
UCBD_label_41:
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_16, 1, 0, 0
UCBD_label_42:
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_16, 1, 0, 0
UCBD_label_43:
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_16, 1, 0, 0
UCBD_label_44:
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_16, 1, 0, 0
UCBD_label_45:
	 UC_DMA_BD	 0, 0x1098c0, @DMAWRITE_data_89, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_17, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x00000000
WRITE_data_1:
	.long 0x00000001
WRITE_data_2:
	.long 0x00000002
WRITE_data_3:
	.long 0x00000004
DMAWRITE_data_0:
	.long 0x00803300
	.long 0xfe000200
	.long 0x1d900bbb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_1:
	.long 0x00803b00
	.long 0xfe000200
	.long 0x1de00bc5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_2:
	.long 0x00803700
	.long 0xfe000200
	.long 0x1da00bbb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_3:
	.long 0x00803f00
	.long 0xfe000200
	.long 0x1df00bc5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4:
	.long 0x00800400
	.long 0xf8000780
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
	.long 0x14000000
DMAWRITE_data_6:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000b99
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
	.long 0x1c000b9b
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_8:
	.long 0x00800b80
	.long 0xf8000780
	.long 0x1ce00b9f
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
	.long 0x1c000ba1
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
	.long 0x1c000ba3
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
	.long 0x1c000ba5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_12:
	.long 0x00804300
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
DMAWRITE_data_13:
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
DMAWRITE_data_14:
	.long 0x00803500
	.long 0xfe000200
	.long 0x1db00bbb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_15:
	.long 0x00803d00
	.long 0xfe000200
	.long 0x1e000bc5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_16:
	.long 0x00803900
	.long 0xfe000200
	.long 0x1dc00bbb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_17:
	.long 0x00804100
	.long 0xfe000200
	.long 0x1e100bc5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_18:
	.long 0x00801300
	.long 0xfc001000
	.long 0x1d300ba9
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
	.long 0x1c000bab
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_20:
	.long 0x00802300
	.long 0xfc001000
	.long 0x1d600baf
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
	.long 0x1c000bb1
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
	.long 0xfe000000
	.long 0x1e300bc9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_23:
	.long 0x00801300
	.long 0xfc000040
	.long 0x1e600bcf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x34000000
DMAWRITE_data_24:
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
	.long 0x3c000000
DMAWRITE_data_25:
	.long 0x00802300
	.long 0xfc000040
	.long 0x1e900bd5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x44000000
DMAWRITE_data_26:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000bd7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_27:
	.long 0x00801300
	.long 0xfc001000
	.long 0x1ec00bdb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x54000000
DMAWRITE_data_28:
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
	.long 0x5c000000
DMAWRITE_data_29:
	.long 0x00802300
	.long 0xfc001000
	.long 0x1ef00be1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x64000000
DMAWRITE_data_30:
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
	.long 0x00000000
DMAWRITE_data_31:
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
DMAWRITE_data_32:
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
DMAWRITE_data_33:
	.long 0x00800400
	.long 0xfe001400
	.long 0x1ca00b93
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00004000
	.long 0x14000000
DMAWRITE_data_34:
	.long 0x00800b80
	.long 0xfe001400
	.long 0x1cf00b9d
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00004000
	.long 0x00000000
DMAWRITE_data_35:
	.long 0x00804300
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
DMAWRITE_data_36:
	.long 0x00801300
	.long 0xfe000800
	.long 0x1d400ba7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_37:
	.long 0x00802300
	.long 0xfe000800
	.long 0x1d700bad
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_38:
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
DMAWRITE_data_39:
	.long 0x00801300
	.long 0xfe000800
	.long 0x1e700bcd
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00003f00
	.long 0x00080400
	.long 0x2c000000
DMAWRITE_data_40:
	.long 0x00802300
	.long 0xfe000800
	.long 0x1ea00bd3
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00003f00
	.long 0x00080400
	.long 0x00000000
DMAWRITE_data_41:
	.long 0x00801300
	.long 0xfe000800
	.long 0x1ed00bd9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x3c000000
DMAWRITE_data_42:
	.long 0x00802300
	.long 0xfe000800
	.long 0x1f000bdf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_43:
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
DMAWRITE_data_44:
	.long 0x008007c0
	.long 0xfe001400
	.long 0x1cb00b93
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00004000
	.long 0x14000000
DMAWRITE_data_45:
	.long 0x00800f40
	.long 0xfe001400
	.long 0x1d000b9d
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00004000
	.long 0x00000000
DMAWRITE_data_46:
	.long 0x00804300
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
DMAWRITE_data_47:
	.long 0x00801b00
	.long 0xfe000800
	.long 0x1d500ba7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_48:
	.long 0x00802b00
	.long 0xfe000800
	.long 0x1d800bad
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_49:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1e500bc7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_50:
	.long 0x00801320
	.long 0xfe000800
	.long 0x1e800bcd
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00003f00
	.long 0x00080400
	.long 0x2c000000
DMAWRITE_data_51:
	.long 0x00802320
	.long 0xfe000800
	.long 0x1eb00bd3
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00003f00
	.long 0x00080400
	.long 0x00000000
DMAWRITE_data_52:
	.long 0x00801b00
	.long 0xfe000800
	.long 0x1ee00bd9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x3c000000
DMAWRITE_data_53:
	.long 0x00802b00
	.long 0xfe000800
	.long 0x1f100bdf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_54:
	.long 0x00803300
	.long 0xf8000780
	.long 0x1dd00bb3
	.long 0x00000400
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00007800
	.long 0x0c000000
DMAWRITE_data_55:
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
	.long 0x14000000
DMAWRITE_data_56:
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
	.long 0x1c000000
DMAWRITE_data_57:
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
	.long 0x24000000
DMAWRITE_data_58:
	.long 0x00803b00
	.long 0xf8000780
	.long 0x1e200bbd
	.long 0x00000400
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00007800
	.long 0x2c000000
DMAWRITE_data_59:
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
	.long 0x34000000
DMAWRITE_data_60:
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
	.long 0x3c000000
DMAWRITE_data_61:
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
	.long 0x00000000
DMAWRITE_data_62:
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
DMAWRITE_data_63:
	.long 0x00800600
	.long 0xfe001400
	.long 0x1cc00b93
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00204000
	.long 0x00000000
	.long 0x00003800
	.long 0x14000000
DMAWRITE_data_64:
	.long 0x00800d80
	.long 0xfe001400
	.long 0x1d100b9d
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00204000
	.long 0x00000000
	.long 0x00003800
	.long 0x00000000
DMAWRITE_data_65:
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
DMAWRITE_data_66:
	.long 0x008009c0
	.long 0xfe001400
	.long 0x1cd00b93
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00204000
	.long 0x00000000
	.long 0x00003800
	.long 0x14000000
DMAWRITE_data_67:
	.long 0x00801140
	.long 0xfe001400
	.long 0x1d200b9d
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00204000
	.long 0x00000000
	.long 0x00003800
	.long 0x00000000
WRITE_data_4:
	.long 0x00030000
WRITE_data_5:
	.long 0x00010004
WRITE_data_6:
	.long 0x00000007
WRITE_data_7:
	.long 0x00030009
WRITE_data_8:
	.long 0x0000000b
WRITE_data_9:
	.long 0x00030001
WRITE_data_10:
	.long 0x00010003
WRITE_data_11:
	.long 0x00000005
WRITE_data_12:
	.long 0x00030006
DMAWRITE_data_68:
	.long 0x00000000
    .long 0x028f2510
	.long 0x00000020
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_69:
	.long 0x00000000
    .long 0x0067f3a0
	.long 0x00000f00
	.long 0x0301e002
	.long 0x01000000
	.long 0x00000180
	.long 0x00000020
	.long 0x20000001
	.long 0x00030000
DMAWRITE_data_70:
	.long 0x00000000
    .long 0x005a6620
	.long 0x00004380
	.long 0x0303c000
	.long 0x01000000
	.long 0x00000124
	.long 0x00000020
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_71:
	.long 0x00000000
    .long 0x005a6aa0
	.long 0x000000f0
	.long 0x03000000
	.long 0x00200000
	.long 0x00000124
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_72:
	.long 0x00000000
    .long 0x005d9b60
	.long 0x00004380
	.long 0x0303c000
	.long 0x01000000
	.long 0x00000124
	.long 0x00000020
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_73:
	.long 0x00000000
    .long 0x005d9fe0
	.long 0x000000f0
	.long 0x03000000
	.long 0x00200000
	.long 0x00000124
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_74:
	.long 0x00000000
    .long 0x0134e9a0
	.long 0x00009000
	.long 0x03040002
	.long 0x01000000
	.long 0x00000180
	.long 0x00000020
	.long 0x20000001
	.long 0x01800000
DMAWRITE_data_75:
	.long 0x00000000
    .long 0x014269a0
	.long 0x00000040
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_76:
	.long 0x00000000
    .long 0x0134eca0
	.long 0x00009000
	.long 0x03040002
	.long 0x01000000
	.long 0x00000180
	.long 0x00000020
	.long 0x20000001
	.long 0x01800000
DMAWRITE_data_77:
	.long 0x00000000
    .long 0x01426ca0
	.long 0x00000040
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
WRITE_data_13:
	.long 0x00000003
DMAWRITE_data_78:
	.long 0x00800000
	.long 0xf8000000
	.long 0x1ce00b9f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
DMAWRITE_data_79:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1cf00b9d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_80:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1d000b9d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_81:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1d100b9d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_82:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1d200b9d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_83:
	.long 0x00803b00
	.long 0xf80000f0
	.long 0x1e200bbd
	.long 0x00000020
	.long 0x00000400
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00f00080
	.long 0x2c000000
DMAWRITE_data_84:
	.long 0x00802300
	.long 0xfc000040
	.long 0x1ef00be1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x64000000
WRITE_data_14:
	.long 0x00000002
DMAWRITE_data_85:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1d100b9d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_86:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1d200b9d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_87:
	.long 0x00802300
	.long 0xfe000800
	.long 0x1f000bdf
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00003f00
	.long 0x00080400
	.long 0x00000000
WRITE_data_15:
	.long 0x00000007
DMAWRITE_data_88:
	.long 0x00802320
	.long 0xfe000800
	.long 0x1f100bdf
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00003f00
	.long 0x00080400
	.long 0x00000000
WRITE_data_16:
	.long 0x00000001
DMAWRITE_data_89:
	.long 0x00803b00
	.long 0xf80000f0
	.long 0x1e200bbd
	.long 0x00000020
	.long 0x00000400
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00f00080
	.long 0x2c000000
WRITE_data_17:
	.long 0x00000004
hintmap_0:
	.long 0x00000000
