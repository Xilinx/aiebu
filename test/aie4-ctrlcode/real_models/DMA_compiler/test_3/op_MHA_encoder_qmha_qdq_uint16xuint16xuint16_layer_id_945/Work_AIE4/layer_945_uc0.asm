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
; SHIM_PRM
APPLY_OFFSET_57	 @DMAWRITE_data_46, 1, 3
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
; SHIM_IFM_1
APPLY_OFFSET_57	 @DMAWRITE_data_47, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_48, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_49, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
; SHIM_WGT
APPLY_OFFSET_57	 @DMAWRITE_data_50, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
; SHIM_IFM_2
APPLY_OFFSET_57	 @DMAWRITE_data_51, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
; SHIM_IFM_2
APPLY_OFFSET_57	 @DMAWRITE_data_52, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_7
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_8
MASK_POLL_32	 0x149ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_9
MASK_POLL_32	 0x109f04, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_10
MASK_POLL_32	 0x109f0c, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_11
MASK_POLL_32	 0x109edc, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_12
MASK_POLL_32	 0x109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_13
MASK_POLL_32	 0x109f04, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_14
MASK_POLL_32	 0x109f0c, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_15
MASK_POLL_32	 0x109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_16
MASK_POLL_32	 0x109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_17
MASK_POLL_32	 0x149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_18
MASK_POLL_32	 0x149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_19
MASK_POLL_32	 0x149ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_20
MASK_POLL_32	 0x149edc, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_21
MASK_POLL_32	 0x109f04, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_22
MASK_POLL_32	 0x109f0c, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_23
MASK_POLL_32	 0x109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_24
MASK_POLL_32	 0x109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_25
MASK_POLL_32	 0x149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_26
MASK_POLL_32	 0x149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_27
MASK_POLL_32	 0x109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_28
MASK_POLL_32	 0x109edc, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_29
MASK_POLL_32	 0x109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_30
MASK_POLL_32	 0x109f04, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_31
MASK_POLL_32	 0x109f0c, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_32
MASK_POLL_32	 0x109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_33
MASK_POLL_32	 0x109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_34
MASK_POLL_32	 0x149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_35
MASK_POLL_32	 0x149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_36
MASK_POLL_32	 0x149ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_37
MASK_POLL_32	 0x149edc, 0x1f8003c, 0x0
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
MASK_POLL_32	 0x109edc, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_46
MASK_POLL_32	 0x109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_47
MASK_POLL_32	 0x109f04, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_48
MASK_POLL_32	 0x109f0c, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_49
MASK_POLL_32	 0x109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_50
MASK_POLL_32	 0x109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_51
MASK_POLL_32	 0x149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_52
MASK_POLL_32	 0x149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_53
MASK_POLL_32	 0x149ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_54
MASK_POLL_32	 0x149edc, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_55
MASK_POLL_32	 0x109f04, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_56
MASK_POLL_32	 0x109f0c, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_57
MASK_POLL_32	 0x109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_58
MASK_POLL_32	 0x109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_59
MASK_POLL_32	 0x149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_60
MASK_POLL_32	 0x149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_61
MASK_POLL_32	 0x109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_62
MASK_POLL_32	 0x109edc, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_63
MASK_POLL_32	 0x109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_64
MASK_POLL_32	 0x109f04, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_65
MASK_POLL_32	 0x109f0c, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_66
MASK_POLL_32	 0x109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_67
MASK_POLL_32	 0x109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_68
MASK_POLL_32	 0x149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_69
MASK_POLL_32	 0x149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_70
MASK_POLL_32	 0x149ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_71
MASK_POLL_32	 0x149edc, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_72
MASK_POLL_32	 0x109f04, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_73
MASK_POLL_32	 0x109f0c, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_74
MASK_POLL_32	 0x109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_75
MASK_POLL_32	 0x109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_76
MASK_POLL_32	 0x149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_77
MASK_POLL_32	 0x149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_78
MASK_POLL_32	 0x109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_79
MASK_POLL_32	 0x109edc, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_80
MASK_POLL_32	 0x109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_81
MASK_POLL_32	 0x109f04, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_82
MASK_POLL_32	 0x109f0c, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_83
MASK_POLL_32	 0x109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_84
MASK_POLL_32	 0x109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_85
MASK_POLL_32	 0x149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_86
MASK_POLL_32	 0x149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_87
MASK_POLL_32	 0x149ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_88
MASK_POLL_32	 0x149edc, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_89
MASK_POLL_32	 0x109f04, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_90
MASK_POLL_32	 0x109f0c, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_91
MASK_POLL_32	 0x109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_92
MASK_POLL_32	 0x109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_93
MASK_POLL_32	 0x149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_94
MASK_POLL_32	 0x149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_95
MASK_POLL_32	 0x109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_96
MASK_POLL_32	 0x109edc, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_97
MASK_POLL_32	 0x109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_98
MASK_POLL_32	 0x109f04, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_99
MASK_POLL_32	 0x109f0c, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_100
MASK_POLL_32	 0x109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_101
MASK_POLL_32	 0x109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_102
MASK_POLL_32	 0x149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_103
MASK_POLL_32	 0x149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_104
MASK_POLL_32	 0x149ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_105
MASK_POLL_32	 0x149edc, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_106
MASK_POLL_32	 0x109f04, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_107
MASK_POLL_32	 0x109f0c, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_108
MASK_POLL_32	 0x109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_109
MASK_POLL_32	 0x109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_110
MASK_POLL_32	 0x149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_111
MASK_POLL_32	 0x149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_112
MASK_POLL_32	 0x109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_113
MASK_POLL_32	 0x109f10, 0x1f8003c, 0x0
MASK_POLL_32	 0x18570, 0x1f8003c, 0x0
REMOTE_BARRIER	 $rb0, 0x15
; RECORD TIMER STOP
END_JOB

START_JOB 1
PREEMPT	0x0055, @save, @restore , @hintmap_0
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
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
	 UC_DMA_BD	 0, 0x1100b0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100c0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100d0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100e0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1100f0, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110100, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110110, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110120, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x110140, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110150, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110160, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110170, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x110180, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x110190, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101a0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101b0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x1101c0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x108000, @DMAWRITE_data_0, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108300, @DMAWRITE_data_1, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108600, @DMAWRITE_data_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108630, @DMAWRITE_data_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108660, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108690, @DMAWRITE_data_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1086c0, @DMAWRITE_data_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1086f0, @DMAWRITE_data_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108720, @DMAWRITE_data_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108750, @DMAWRITE_data_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108900, @DMAWRITE_data_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108930, @DMAWRITE_data_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148000, @DMAWRITE_data_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148300, @DMAWRITE_data_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148600, @DMAWRITE_data_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148630, @DMAWRITE_data_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148660, @DMAWRITE_data_16, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148690, @DMAWRITE_data_17, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1486c0, @DMAWRITE_data_18, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1486f0, @DMAWRITE_data_19, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148720, @DMAWRITE_data_20, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148900, @DMAWRITE_data_21, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c00, @DMAWRITE_data_22, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c30, @DMAWRITE_data_23, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108c60, @DMAWRITE_data_24, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f00, @DMAWRITE_data_25, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f30, @DMAWRITE_data_26, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f60, @DMAWRITE_data_27, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108f90, @DMAWRITE_data_28, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108fc0, @DMAWRITE_data_29, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x108ff0, @DMAWRITE_data_30, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109200, @DMAWRITE_data_31, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109230, @DMAWRITE_data_32, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109260, @DMAWRITE_data_33, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109500, @DMAWRITE_data_34, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109530, @DMAWRITE_data_26, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109560, @DMAWRITE_data_27, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109590, @DMAWRITE_data_28, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1095c0, @DMAWRITE_data_35, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x1095f0, @DMAWRITE_data_30, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109800, @DMAWRITE_data_36, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109830, @DMAWRITE_data_37, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109860, @DMAWRITE_data_38, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109890, @DMAWRITE_data_39, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c00, @DMAWRITE_data_40, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c30, @DMAWRITE_data_41, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x148c60, @DMAWRITE_data_42, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149200, @DMAWRITE_data_43, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149230, @DMAWRITE_data_44, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x149260, @DMAWRITE_data_45, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e1c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110000, @WRITE_data_1, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_46, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x18030, @DMAWRITE_data_47, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x18060, @DMAWRITE_data_48, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x18090, @DMAWRITE_data_49, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x58030, @DMAWRITE_data_50, 0x9, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x58060, @DMAWRITE_data_51, 0x9, 0, 0
UCBD_label_7:
	 UC_DMA_BD	 0, 0x58090, @DMAWRITE_data_52, 0x9, 0, 0
UCBD_label_8:
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x1855c, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x58554, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x5855c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x10000, @WRITE_data_1, 1, 0, 0
UCBD_label_9:
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e1c, @WRITE_data_1, 1, 0, 0
UCBD_label_10:
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_0, 1, 0, 0
UCBD_label_11:
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_1, 1, 0, 0
UCBD_label_12:
	 UC_DMA_BD	 0, 0x109e1c, @WRITE_data_1, 1, 0, 0
UCBD_label_13:
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_0, 1, 0, 0
UCBD_label_14:
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_1, 1, 0, 0
UCBD_label_15:
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_1, 1, 0, 0
UCBD_label_16:
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_2, 1, 0, 0
UCBD_label_17:
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_2, 1, 0, 0
UCBD_label_18:
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_2, 1, 0, 0
UCBD_label_19:
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_2, 1, 0, 0
UCBD_label_20:
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_0, 1, 0, 0
UCBD_label_21:
	 UC_DMA_BD	 0, 0x149e1c, @WRITE_data_1, 1, 0, 0
UCBD_label_22:
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_0, 1, 0, 0
UCBD_label_23:
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_0, 1, 0, 0
UCBD_label_24:
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_1, 1, 0, 0
UCBD_label_25:
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_26:
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_1, 1, 0, 0
UCBD_label_27:
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_28:
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_1, 1, 0, 0
UCBD_label_29:
	 UC_DMA_BD	 0, 0x109e1c, @WRITE_data_1, 1, 0, 0
UCBD_label_30:
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_0, 1, 0, 0
UCBD_label_31:
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_1, 1, 0, 0
UCBD_label_32:
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_1, 1, 0, 0
UCBD_label_33:
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_2, 1, 0, 0
UCBD_label_34:
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_2, 1, 0, 0
UCBD_label_35:
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_2, 1, 0, 0
UCBD_label_36:
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_2, 1, 0, 0
UCBD_label_37:
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_0, 1, 0, 0
UCBD_label_38:
	 UC_DMA_BD	 0, 0x149e1c, @WRITE_data_1, 1, 0, 0
UCBD_label_39:
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_0, 1, 0, 0
UCBD_label_40:
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_0, 1, 0, 0
UCBD_label_41:
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_1, 1, 0, 0
UCBD_label_42:
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_43:
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_1, 1, 0, 0
UCBD_label_44:
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_45:
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_1, 1, 0, 0
UCBD_label_46:
	 UC_DMA_BD	 0, 0x109e1c, @WRITE_data_1, 1, 0, 0
UCBD_label_47:
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_0, 1, 0, 0
UCBD_label_48:
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_1, 1, 0, 0
UCBD_label_49:
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_1, 1, 0, 0
UCBD_label_50:
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_2, 1, 0, 0
UCBD_label_51:
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_2, 1, 0, 0
UCBD_label_52:
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_2, 1, 0, 0
UCBD_label_53:
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_2, 1, 0, 0
UCBD_label_54:
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_0, 1, 0, 0
UCBD_label_55:
	 UC_DMA_BD	 0, 0x149e1c, @WRITE_data_1, 1, 0, 0
UCBD_label_56:
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_0, 1, 0, 0
UCBD_label_57:
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_0, 1, 0, 0
UCBD_label_58:
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_1, 1, 0, 0
UCBD_label_59:
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_60:
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_1, 1, 0, 0
UCBD_label_61:
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_62:
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_1, 1, 0, 0
UCBD_label_63:
	 UC_DMA_BD	 0, 0x109e1c, @WRITE_data_1, 1, 0, 0
UCBD_label_64:
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_0, 1, 0, 0
UCBD_label_65:
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_1, 1, 0, 0
UCBD_label_66:
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_1, 1, 0, 0
UCBD_label_67:
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_2, 1, 0, 0
UCBD_label_68:
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_2, 1, 0, 0
UCBD_label_69:
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_2, 1, 0, 0
UCBD_label_70:
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_2, 1, 0, 0
UCBD_label_71:
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_0, 1, 0, 0
UCBD_label_72:
	 UC_DMA_BD	 0, 0x149e1c, @WRITE_data_1, 1, 0, 0
UCBD_label_73:
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_0, 1, 0, 0
UCBD_label_74:
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_0, 1, 0, 0
UCBD_label_75:
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_1, 1, 0, 0
UCBD_label_76:
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_77:
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_1, 1, 0, 0
UCBD_label_78:
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_79:
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_1, 1, 0, 0
UCBD_label_80:
	 UC_DMA_BD	 0, 0x109e1c, @WRITE_data_1, 1, 0, 0
UCBD_label_81:
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_0, 1, 0, 0
UCBD_label_82:
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_1, 1, 0, 0
UCBD_label_83:
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_1, 1, 0, 0
UCBD_label_84:
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_2, 1, 0, 0
UCBD_label_85:
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_2, 1, 0, 0
UCBD_label_86:
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_2, 1, 0, 0
UCBD_label_87:
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_2, 1, 0, 0
UCBD_label_88:
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_0, 1, 0, 0
UCBD_label_89:
	 UC_DMA_BD	 0, 0x149e1c, @WRITE_data_1, 1, 0, 0
UCBD_label_90:
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_0, 1, 0, 0
UCBD_label_91:
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_0, 1, 0, 0
UCBD_label_92:
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_1, 1, 0, 0
UCBD_label_93:
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_94:
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_1, 1, 0, 0
UCBD_label_95:
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_96:
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_1, 1, 0, 0
UCBD_label_97:
	 UC_DMA_BD	 0, 0x109e1c, @WRITE_data_1, 1, 0, 0
UCBD_label_98:
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_0, 1, 0, 0
UCBD_label_99:
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_1, 1, 0, 0
UCBD_label_100:
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_1, 1, 0, 0
UCBD_label_101:
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_2, 1, 0, 0
UCBD_label_102:
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_2, 1, 0, 0
UCBD_label_103:
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_2, 1, 0, 0
UCBD_label_104:
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_2, 1, 0, 0
UCBD_label_105:
	 UC_DMA_BD	 0, 0x149e14, @WRITE_data_0, 1, 0, 0
UCBD_label_106:
	 UC_DMA_BD	 0, 0x149e1c, @WRITE_data_1, 1, 0, 0
UCBD_label_107:
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_0, 1, 0, 0
UCBD_label_108:
	 UC_DMA_BD	 0, 0x109e7c, @WRITE_data_0, 1, 0, 0
UCBD_label_109:
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_1, 1, 0, 0
UCBD_label_110:
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_111:
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_1, 1, 0, 0
UCBD_label_112:
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_113:
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_1, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x00000004
WRITE_data_1:
	.long 0x00000000
WRITE_data_2:
	.long 0x00000002
WRITE_data_3:
	.long 0x00000001
DMAWRITE_data_0:
	.long 0x00802420
	.long 0xfe000800
	.long 0x1d800bb9
	.long 0x00000020
	.long 0x00000020
	.long 0x00000080
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00200400
	.long 0x00004000
DMAWRITE_data_1:
	.long 0x00802c20
	.long 0xfe000800
	.long 0x1d900bb9
	.long 0x00000020
	.long 0x00000020
	.long 0x00000080
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00200400
	.long 0x00004000
DMAWRITE_data_2:
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
DMAWRITE_data_3:
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
DMAWRITE_data_4:
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
DMAWRITE_data_5:
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
DMAWRITE_data_6:
	.long 0x00800420
	.long 0xf8002000
	.long 0x1d300ba9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
DMAWRITE_data_7:
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
	.long 0x34000000
DMAWRITE_data_8:
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
	.long 0x3c000000
DMAWRITE_data_9:
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
	.long 0x00000000
DMAWRITE_data_10:
	.long 0x00804420
	.long 0xfc002000
	.long 0x1cb00b99
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_11:
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
	.long 0x00000000
DMAWRITE_data_12:
	.long 0x00803420
	.long 0xfe000800
	.long 0x1da00bb9
	.long 0x00000020
	.long 0x00000020
	.long 0x00000080
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00200400
	.long 0x00004000
DMAWRITE_data_13:
	.long 0x00803c20
	.long 0xfe000800
	.long 0x1db00bb9
	.long 0x00000020
	.long 0x00000020
	.long 0x00000080
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00200400
	.long 0x00004000
DMAWRITE_data_14:
	.long 0x00800400
	.long 0xf8000020
	.long 0x1c600b8f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_15:
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
	.long 0x14000000
DMAWRITE_data_16:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000b93
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_17:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000b95
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_18:
	.long 0x00806420
	.long 0xfc001000
	.long 0x1ce00ba1
	.long 0x00000100
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x01000020
	.long 0x2c200000
DMAWRITE_data_19:
	.long 0x00800000
	.long 0xfc000000
	.long 0x1d000ba3
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x34000000
DMAWRITE_data_20:
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
DMAWRITE_data_21:
	.long 0x008064a0
	.long 0xfc001000
	.long 0x1cf00ba1
	.long 0x00000100
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x01000020
	.long 0x00200000
DMAWRITE_data_22:
	.long 0x00800000
	.long 0xfe000100
	.long 0x1c200b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_23:
	.long 0x00800400
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
DMAWRITE_data_24:
	.long 0x00800420
	.long 0xfe000800
	.long 0x1d400ba7
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x00008000
DMAWRITE_data_25:
	.long 0x00804420
	.long 0xfe000800
	.long 0x1cc00381
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x0c008000
DMAWRITE_data_26:
	.long 0x00804c20
	.long 0x00000800
	.long 0x00000000
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x14008000
DMAWRITE_data_27:
	.long 0x00805420
	.long 0x00000800
	.long 0x00000000
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x1c008000
DMAWRITE_data_28:
	.long 0x00805c20
	.long 0x00000800
	.long 0x1c000b97
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x00008000
DMAWRITE_data_29:
	.long 0x00806420
	.long 0xfe002000
	.long 0x1d100b9d
	.long 0x00000100
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x01000400
	.long 0x2c020000
DMAWRITE_data_30:
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
	.long 0x00000000
DMAWRITE_data_31:
	.long 0x00800100
	.long 0xfe000100
	.long 0x1c300b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_32:
	.long 0x00800400
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
DMAWRITE_data_33:
	.long 0x00800c20
	.long 0xfe000800
	.long 0x1d500ba7
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x00008000
DMAWRITE_data_34:
	.long 0x00804420
	.long 0xfe000800
	.long 0x1cd00381
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x0c008000
DMAWRITE_data_35:
	.long 0x00806420
	.long 0xfe002000
	.long 0x1d200b9d
	.long 0x00000100
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x01000400
	.long 0x2c020000
DMAWRITE_data_36:
	.long 0x00802420
	.long 0xf8002000
	.long 0x1dc00bb1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_37:
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
	.long 0x14000000
DMAWRITE_data_38:
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
	.long 0x1c000000
DMAWRITE_data_39:
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
	.long 0x00000000
DMAWRITE_data_40:
	.long 0x00800200
	.long 0xfe000100
	.long 0x1c400b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_41:
	.long 0x00800400
	.long 0xfe000020
	.long 0x1c900b8d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_42:
	.long 0x00801420
	.long 0xfe000800
	.long 0x1d600ba7
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x00008000
DMAWRITE_data_43:
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
DMAWRITE_data_44:
	.long 0x00800400
	.long 0xfe000020
	.long 0x1ca00b8d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_45:
	.long 0x00801c20
	.long 0xfe000800
	.long 0x1d700ba7
	.long 0x00000020
	.long 0x00000010
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x02000200
	.long 0x00008000
DMAWRITE_data_46:
	.long 0x00000000
    .long 0x00861000
	.long 0x00000400
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_47:
	.long 0x00000000
    .long 0x03da6600
	.long 0x00002000
	.long 0x03004001
	.long 0x40000000
	.long 0x00000800
	.long 0x00000000
	.long 0x21802000
	.long 0x00000400
DMAWRITE_data_48:
	.long 0x00000000
    .long 0x02a05a00
	.long 0x00002000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x21802000
	.long 0x00000400
DMAWRITE_data_49:
	.long 0x00000000
    .long 0x01be5900
	.long 0x00002000
	.long 0x03100000
	.long 0x01000000
	.long 0x00000280
	.long 0x00000001
	.long 0x21800020
	.long 0x00000400
DMAWRITE_data_50:
	.long 0x00000000
    .long 0x2e556e80
	.long 0x00000020
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_51:
	.long 0x00000000
    .long 0x0600d400
	.long 0x00001000
	.long 0x03002000
	.long 0x40000000
	.long 0x00000800
	.long 0x00000001
	.long 0x21802000
	.long 0x00000400
DMAWRITE_data_52:
	.long 0x00000000
    .long 0x06011400
	.long 0x00001000
	.long 0x03002000
	.long 0x40000000
	.long 0x00000800
	.long 0x00000001
	.long 0x21802000
	.long 0x00000400
WRITE_data_4:
	.long 0x00060003
WRITE_data_5:
	.long 0x00060001
WRITE_data_6:
	.long 0x00060002
hintmap_0:
	.long 0x00000000
