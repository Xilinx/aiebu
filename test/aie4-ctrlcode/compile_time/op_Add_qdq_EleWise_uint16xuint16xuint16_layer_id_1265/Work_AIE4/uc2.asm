.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;
.attach_to_group	 2
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
APPLY_OFFSET_57	 @DMAWRITE_data_63, 1, 3
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
APPLY_OFFSET_57	 @DMAWRITE_data_64, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
APPLY_OFFSET_57	 @DMAWRITE_data_65, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
APPLY_OFFSET_57	 @DMAWRITE_data_66, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
APPLY_OFFSET_57	 @DMAWRITE_data_67, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
APPLY_OFFSET_57	 @DMAWRITE_data_68, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
APPLY_OFFSET_57	 @DMAWRITE_data_69, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_7
APPLY_OFFSET_57	 @DMAWRITE_data_70, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_8
APPLY_OFFSET_57	 @DMAWRITE_data_71, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_9
APPLY_OFFSET_57	 @DMAWRITE_data_72, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_10
APPLY_OFFSET_57	 @DMAWRITE_data_73, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_11
APPLY_OFFSET_57	 @DMAWRITE_data_74, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_12
APPLY_OFFSET_57	 @DMAWRITE_data_75, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_13
APPLY_OFFSET_57	 @DMAWRITE_data_76, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_14
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_15
MASK_POLL_32	 0x2018570, 0x1f8003c, 0x0
APPLY_OFFSET_57	 @DMAWRITE_data_77, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_16
APPLY_OFFSET_57	 @DMAWRITE_data_78, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_17
APPLY_OFFSET_57	 @DMAWRITE_data_79, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_18
APPLY_OFFSET_57	 @DMAWRITE_data_80, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_19
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_20
MASK_POLL_32	 0x2018580, 0x1f8003c, 0x0
APPLY_OFFSET_57	 @DMAWRITE_data_81, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_21
APPLY_OFFSET_57	 @DMAWRITE_data_82, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_22
APPLY_OFFSET_57	 @DMAWRITE_data_83, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_23
APPLY_OFFSET_57	 @DMAWRITE_data_84, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_24
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_25
MASK_POLL_32	 0x2018584, 0x1f8003c, 0x0
APPLY_OFFSET_57	 @DMAWRITE_data_85, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_26
APPLY_OFFSET_57	 @DMAWRITE_data_86, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_27
APPLY_OFFSET_57	 @DMAWRITE_data_87, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_28
APPLY_OFFSET_57	 @DMAWRITE_data_88, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_29
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_30
MASK_POLL_32	 0x2109f10, 0x1f8003c, 0x0
MASK_POLL_32	 0x2018570, 0x1f8003c, 0x0
REMOTE_BARRIER	 $rb0, 0x15
END_JOB

START_JOB 1
PREEMPT	0x57ba, @save, @restore , @hintmap_0
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x2108000, @DMAWRITE_data_0, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108030, @DMAWRITE_data_1, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108060, @DMAWRITE_data_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108300, @DMAWRITE_data_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108330, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108600, @DMAWRITE_data_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108630, @DMAWRITE_data_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108660, @DMAWRITE_data_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108690, @DMAWRITE_data_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21086c0, @DMAWRITE_data_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21086f0, @DMAWRITE_data_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108720, @DMAWRITE_data_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108750, @DMAWRITE_data_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108780, @DMAWRITE_data_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21087b0, @DMAWRITE_data_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108900, @DMAWRITE_data_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108930, @DMAWRITE_data_16, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108960, @DMAWRITE_data_17, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108990, @DMAWRITE_data_18, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21089c0, @DMAWRITE_data_19, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21089f0, @DMAWRITE_data_20, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108a20, @DMAWRITE_data_21, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108a50, @DMAWRITE_data_22, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108a80, @DMAWRITE_data_23, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108ab0, @DMAWRITE_data_24, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148000, @DMAWRITE_data_25, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148030, @DMAWRITE_data_26, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148300, @DMAWRITE_data_27, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148330, @DMAWRITE_data_28, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c00, @DMAWRITE_data_29, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c30, @DMAWRITE_data_30, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c60, @DMAWRITE_data_31, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108c90, @DMAWRITE_data_32, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2108cc0, @DMAWRITE_data_33, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109200, @DMAWRITE_data_34, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109230, @DMAWRITE_data_35, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109260, @DMAWRITE_data_36, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109290, @DMAWRITE_data_37, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21092c0, @DMAWRITE_data_38, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109800, @DMAWRITE_data_39, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109830, @DMAWRITE_data_40, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109860, @DMAWRITE_data_41, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109890, @DMAWRITE_data_42, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21098c0, @DMAWRITE_data_43, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21098f0, @DMAWRITE_data_44, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109920, @DMAWRITE_data_45, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109950, @DMAWRITE_data_46, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109980, @DMAWRITE_data_47, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c00, @DMAWRITE_data_48, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c30, @DMAWRITE_data_49, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c60, @DMAWRITE_data_50, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148c90, @DMAWRITE_data_51, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148cc0, @DMAWRITE_data_52, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148f00, @DMAWRITE_data_53, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2148f30, @DMAWRITE_data_54, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149200, @DMAWRITE_data_55, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149230, @DMAWRITE_data_56, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149260, @DMAWRITE_data_57, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149290, @DMAWRITE_data_58, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21492c0, @DMAWRITE_data_59, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x21492f0, @DMAWRITE_data_60, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149500, @DMAWRITE_data_61, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2149530, @DMAWRITE_data_62, 0xb, 0, 1
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
	 UC_DMA_BD	 0, 0x2110170, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110180, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110190, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101a0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101b0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101c0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101d0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101e0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21101f0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150000, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150030, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2150050, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e1c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e1c, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e04, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e0c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e04, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e0c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e1c, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e04, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e04, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e0c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e04, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e0c, @WRITE_data_0, 1, 0, 1
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
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e84, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x2109e84, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110000, @WRITE_data_1, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x2018000, @DMAWRITE_data_63, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x2018030, @DMAWRITE_data_64, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x2018060, @DMAWRITE_data_65, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x2018090, @DMAWRITE_data_66, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x20180c0, @DMAWRITE_data_67, 0x9, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x20180f0, @DMAWRITE_data_68, 0x9, 0, 0
UCBD_label_7:
	 UC_DMA_BD	 0, 0x2018120, @DMAWRITE_data_69, 0x9, 0, 0
UCBD_label_8:
	 UC_DMA_BD	 0, 0x2018150, @DMAWRITE_data_70, 0x9, 0, 0
UCBD_label_9:
	 UC_DMA_BD	 0, 0x2018180, @DMAWRITE_data_71, 0x9, 0, 0
UCBD_label_10:
	 UC_DMA_BD	 0, 0x20181b0, @DMAWRITE_data_72, 0x9, 0, 0
UCBD_label_11:
	 UC_DMA_BD	 0, 0x20181e0, @DMAWRITE_data_73, 0x9, 0, 0
UCBD_label_12:
	 UC_DMA_BD	 0, 0x2018210, @DMAWRITE_data_74, 0x9, 0, 0
UCBD_label_13:
	 UC_DMA_BD	 0, 0x2018240, @DMAWRITE_data_75, 0x9, 0, 0
UCBD_label_14:
	 UC_DMA_BD	 0, 0x2018270, @DMAWRITE_data_76, 0x9, 0, 0
UCBD_label_15:
	 UC_DMA_BD	 0, 0x2018544, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x201855c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x201855c, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x2010000, @WRITE_data_1, 1, 0, 0
UCBD_label_16:
	 UC_DMA_BD	 0, 0x20181e0, @DMAWRITE_data_77, 0x9, 0, 0
UCBD_label_17:
	 UC_DMA_BD	 0, 0x2018210, @DMAWRITE_data_78, 0x9, 0, 0
UCBD_label_18:
	 UC_DMA_BD	 0, 0x2018240, @DMAWRITE_data_79, 0x9, 0, 0
UCBD_label_19:
	 UC_DMA_BD	 0, 0x2018270, @DMAWRITE_data_80, 0x9, 0, 0
UCBD_label_20:
	 UC_DMA_BD	 0, 0x2018544, @WRITE_data_16, 1, 0, 0
UCBD_label_21:
	 UC_DMA_BD	 0, 0x2018060, @DMAWRITE_data_81, 0x9, 0, 0
UCBD_label_22:
	 UC_DMA_BD	 0, 0x2018090, @DMAWRITE_data_82, 0x9, 0, 0
UCBD_label_23:
	 UC_DMA_BD	 0, 0x20180c0, @DMAWRITE_data_83, 0x9, 0, 0
UCBD_label_24:
	 UC_DMA_BD	 0, 0x20180f0, @DMAWRITE_data_84, 0x9, 0, 0
UCBD_label_25:
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_8, 1, 0, 0
UCBD_label_26:
	 UC_DMA_BD	 0, 0x2018120, @DMAWRITE_data_85, 0x9, 0, 0
UCBD_label_27:
	 UC_DMA_BD	 0, 0x2018150, @DMAWRITE_data_86, 0x9, 0, 0
UCBD_label_28:
	 UC_DMA_BD	 0, 0x2018180, @DMAWRITE_data_87, 0x9, 0, 0
UCBD_label_29:
	 UC_DMA_BD	 0, 0x20181b0, @DMAWRITE_data_88, 0x9, 0, 0
UCBD_label_30:
	 UC_DMA_BD	 0, 0x201855c, @WRITE_data_6, 1, 0, 0
.align    4
DMAWRITE_data_0:
	.long 0x00805088
	.long 0xfe0007a0
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
	.long 0x00800000
	.long 0xfe000000
	.long 0x1d700bb1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_2:
	.long 0x00805088
	.long 0xfe0007a0
	.long 0x1e100bc5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_3:
	.long 0x00805828
	.long 0xfe0007a0
	.long 0x1d300ba9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4:
	.long 0x00805828
	.long 0xfe0007a0
	.long 0x1e200bc7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_5:
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
DMAWRITE_data_6:
	.long 0x00801388
	.long 0xfe0007a0
	.long 0x1cc00381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_7:
	.long 0x00801b28
	.long 0x000007a0
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_8:
	.long 0x008022c8
	.long 0x000007a0
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_9:
	.long 0x00802a68
	.long 0x000007a0
	.long 0x1c000b9b
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_10:
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
DMAWRITE_data_11:
	.long 0x00801388
	.long 0xfe0007a0
	.long 0x1db00381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x3c000000
DMAWRITE_data_12:
	.long 0x00801b28
	.long 0x000007a0
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x44000000
DMAWRITE_data_13:
	.long 0x008022c8
	.long 0x000007a0
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x4c000000
DMAWRITE_data_14:
	.long 0x00802a68
	.long 0x000007a0
	.long 0x1c000bb9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_15:
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
DMAWRITE_data_16:
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
DMAWRITE_data_17:
	.long 0x00803208
	.long 0xfe0007a0
	.long 0x1cd00381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_18:
	.long 0x008039a8
	.long 0x000007a0
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_19:
	.long 0x00804148
	.long 0x000007a0
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
DMAWRITE_data_20:
	.long 0x008048e8
	.long 0x000007a0
	.long 0x1c000b9d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_21:
	.long 0x00803208
	.long 0xfe0007a0
	.long 0x1dc00381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x3c000000
DMAWRITE_data_22:
	.long 0x008039a8
	.long 0x000007a0
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x44000000
DMAWRITE_data_23:
	.long 0x00804148
	.long 0x000007a0
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x4c000000
DMAWRITE_data_24:
	.long 0x008048e8
	.long 0x000007a0
	.long 0x1c000bbb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_25:
	.long 0x00805fc8
	.long 0xfe0007a0
	.long 0x1d400bab
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_26:
	.long 0x00805fc8
	.long 0xfe0007a0
	.long 0x1e300bc9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_27:
	.long 0x00806768
	.long 0xfe0007a0
	.long 0x1d500bad
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_28:
	.long 0x00806768
	.long 0xfe0007a0
	.long 0x1e400bcb
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_29:
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
DMAWRITE_data_30:
	.long 0x00801388
	.long 0xfe0007a0
	.long 0x1ce00381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_31:
	.long 0x00803208
	.long 0x000007a0
	.long 0x1c000b9f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_32:
	.long 0x00801388
	.long 0xfe0007a0
	.long 0x1dd00381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_33:
	.long 0x00803208
	.long 0x000007a0
	.long 0x1c000bbd
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_34:
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
DMAWRITE_data_35:
	.long 0x00801b28
	.long 0xfe0007a0
	.long 0x1cf00381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_36:
	.long 0x008039a8
	.long 0x000007a0
	.long 0x1c000ba1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_37:
	.long 0x00801b28
	.long 0xfe0007a0
	.long 0x1de00381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_38:
	.long 0x008039a8
	.long 0x000007a0
	.long 0x1c000bbf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_39:
	.long 0x00805088
	.long 0xfe0007a0
	.long 0x1d600381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_40:
	.long 0x00805828
	.long 0x000007a0
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_41:
	.long 0x00805fc8
	.long 0x000007a0
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_42:
	.long 0x00806768
	.long 0x000007a0
	.long 0x1c000ba5
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_43:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1d800baf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_44:
	.long 0x00805088
	.long 0xfe0007a0
	.long 0x1e500381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x34000000
DMAWRITE_data_45:
	.long 0x00805828
	.long 0x000007a0
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x3c000000
DMAWRITE_data_46:
	.long 0x00805fc8
	.long 0x000007a0
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x44000000
DMAWRITE_data_47:
	.long 0x00806768
	.long 0x000007a0
	.long 0x1c000bc3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_48:
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
DMAWRITE_data_49:
	.long 0x008022c8
	.long 0xfe0007a0
	.long 0x1d000381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_50:
	.long 0x00804148
	.long 0x000007a0
	.long 0x1c000ba3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_51:
	.long 0x008022c8
	.long 0xfe0007a0
	.long 0x1df00381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x24000000
DMAWRITE_data_52:
	.long 0x00804148
	.long 0x000007a0
	.long 0x1c000bc1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_53:
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
DMAWRITE_data_54:
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
DMAWRITE_data_55:
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
DMAWRITE_data_56:
	.long 0x00802a68
	.long 0xfe0007a0
	.long 0x1d100381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_57:
	.long 0x008048e8
	.long 0x000007a0
	.long 0x1c000b99
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_58:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1da00bb3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_59:
	.long 0x00802a68
	.long 0xfe0007a0
	.long 0x1e000381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
DMAWRITE_data_60:
	.long 0x008048e8
	.long 0x000007a0
	.long 0x1c000bb7
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_61:
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
DMAWRITE_data_62:
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
	.long 0x00050001
WRITE_data_3:
	.long 0x00050002
WRITE_data_4:
	.long 0x00050000
WRITE_data_5:
	.long 0x00010005
WRITE_data_6:
	.long 0x00000006
WRITE_data_7:
	.long 0x00010001
WRITE_data_8:
	.long 0x00000002
WRITE_data_9:
	.long 0x00010003
WRITE_data_10:
	.long 0x00000003
WRITE_data_11:
	.long 0x00000004
WRITE_data_12:
	.long 0x00010004
WRITE_data_13:
	.long 0x00000005
DMAWRITE_data_63:
	.long 0x00000000
	.long 0x00967000
	.long 0x00000400
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_64:
	.long 0x00000000
	.long 0x2cfcd580
	.long 0x00000120
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_65:
	.long 0x00000000
	.long 0x0139ba00
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x1c000001
	.long 0x00000001
	.long 0x21405b80
	.long 0x00000400
DMAWRITE_data_66:
	.long 0x00000000
	.long 0x0139d880
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x24000001
	.long 0x00000001
	.long 0x21405b80
	.long 0x00000400
DMAWRITE_data_67:
	.long 0x00000000
	.long 0x0139f700
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x2c000001
	.long 0x00000001
	.long 0x21405b80
	.long 0x00000400
DMAWRITE_data_68:
	.long 0x00000000
	.long 0x013a1580
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x21405b80
	.long 0x00000400
DMAWRITE_data_69:
	.long 0x00000000
	.long 0x02e95a00
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x3c000001
	.long 0x00000001
	.long 0x21405b80
	.long 0x00000400
DMAWRITE_data_70:
	.long 0x00000000
	.long 0x02e97880
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x44000001
	.long 0x00000001
	.long 0x21405b80
	.long 0x00000400
DMAWRITE_data_71:
	.long 0x00000000
	.long 0x02e99700
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x4c000001
	.long 0x00000001
	.long 0x21405b80
	.long 0x00000400
DMAWRITE_data_72:
	.long 0x00000000
	.long 0x02e9b580
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x21405b80
	.long 0x00000400
DMAWRITE_data_73:
	.long 0x00000000
	.long 0x005e5a00
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x5c000001
	.long 0x00000001
	.long 0x21405b80
	.long 0x00000400
DMAWRITE_data_74:
	.long 0x00000000
	.long 0x005e7880
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x64000001
	.long 0x00000001
	.long 0x21405b80
	.long 0x00000400
DMAWRITE_data_75:
	.long 0x00000000
	.long 0x005e9700
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x6c000001
	.long 0x00000001
	.long 0x21405b80
	.long 0x00000400
DMAWRITE_data_76:
	.long 0x00000000
	.long 0x005eb580
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x21405b80
	.long 0x00000400
WRITE_data_14:
	.long 0x0005000a
WRITE_data_15:
	.long 0x00050006
DMAWRITE_data_77:
	.long 0x00000000
	.long 0x0066ee00
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x5c000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_78:
	.long 0x00000000
	.long 0x00670c80
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x64000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_79:
	.long 0x00000000
	.long 0x00672b00
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x6c000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_80:
	.long 0x00000000
	.long 0x00674980
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
WRITE_data_16:
	.long 0x0000000a
DMAWRITE_data_81:
	.long 0x00000000
	.long 0x01424e00
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x1c000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_82:
	.long 0x00000000
	.long 0x01426c80
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x24000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_83:
	.long 0x00000000
	.long 0x01428b00
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x2c000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_84:
	.long 0x00000000
	.long 0x0142a980
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_85:
	.long 0x00000000
	.long 0x02f1ee00
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x3c000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_86:
	.long 0x00000000
	.long 0x02f20c80
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x44000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_87:
	.long 0x00000000
	.long 0x02f22b00
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x4c000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_88:
	.long 0x00000000
	.long 0x02f24980
	.long 0x000007a0
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
hintmap_0:
	.long 0x00000000
