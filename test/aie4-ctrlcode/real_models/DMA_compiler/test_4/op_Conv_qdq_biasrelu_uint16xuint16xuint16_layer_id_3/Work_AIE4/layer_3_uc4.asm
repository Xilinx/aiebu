.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;
.attach_to_group	 4
; RECORD TIMER START
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
; SHIM_WGT
APPLY_OFFSET_57	 @DMAWRITE_data_45, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_46, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_47, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_48, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_49, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_50, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_7
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_51, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_8
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_52, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_9
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_53, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_10
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_54, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_11
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_55, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_12
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_56, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_13
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_57, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_14
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_58, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_15
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_59, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_16
; SHIM_OFM
APPLY_OFFSET_57	 @DMAWRITE_data_60, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_17
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_18
MASK_POLL_32	 0x4109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_19
MASK_POLL_32	 0x4109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_20
MASK_POLL_32	 0x4109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_21
MASK_POLL_32	 0x4149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_22
MASK_POLL_32	 0x4149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_23
MASK_POLL_32	 0x4109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_24
MASK_POLL_32	 0x4109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_25
MASK_POLL_32	 0x4149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_26
MASK_POLL_32	 0x4149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_27
MASK_POLL_32	 0x4109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_28
MASK_POLL_32	 0x4109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_29
MASK_POLL_32	 0x4109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_30
MASK_POLL_32	 0x4109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_31
MASK_POLL_32	 0x4149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_32
MASK_POLL_32	 0x4149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_33
MASK_POLL_32	 0x4109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_34
MASK_POLL_32	 0x4109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_35
MASK_POLL_32	 0x4149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_36
MASK_POLL_32	 0x4149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_37
MASK_POLL_32	 0x4109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_38
MASK_POLL_32	 0x4109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_39
MASK_POLL_32	 0x4109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_40
MASK_POLL_32	 0x4109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_41
MASK_POLL_32	 0x4149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_42
MASK_POLL_32	 0x4149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_43
MASK_POLL_32	 0x4109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_44
MASK_POLL_32	 0x4109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_45
MASK_POLL_32	 0x4149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_46
MASK_POLL_32	 0x4149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_47
MASK_POLL_32	 0x4109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_48
MASK_POLL_32	 0x4109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_49
MASK_POLL_32	 0x4109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_50
MASK_POLL_32	 0x4109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_51
MASK_POLL_32	 0x4149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_52
MASK_POLL_32	 0x4149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_53
MASK_POLL_32	 0x4109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_54
MASK_POLL_32	 0x4109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_55
MASK_POLL_32	 0x4149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_56
MASK_POLL_32	 0x4149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_57
MASK_POLL_32	 0x4109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_58
MASK_POLL_32	 0x4109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_59
MASK_POLL_32	 0x4109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_60
MASK_POLL_32	 0x4109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_61
MASK_POLL_32	 0x4149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_62
MASK_POLL_32	 0x4149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_63
MASK_POLL_32	 0x4109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_64
MASK_POLL_32	 0x4109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_65
MASK_POLL_32	 0x4149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_66
MASK_POLL_32	 0x4149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_67
MASK_POLL_32	 0x4109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_68
MASK_POLL_32	 0x4109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_69
MASK_POLL_32	 0x4109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_70
MASK_POLL_32	 0x4109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_71
MASK_POLL_32	 0x4149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_72
MASK_POLL_32	 0x4149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_73
MASK_POLL_32	 0x4109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_74
MASK_POLL_32	 0x4109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_75
MASK_POLL_32	 0x4149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_76
MASK_POLL_32	 0x4149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_77
MASK_POLL_32	 0x4109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_78
END_JOB

.eop

START_JOB 1
MASK_POLL_32	 0x4018580, 0x1f8003c, 0x0
; SHIM_IFM_0
APPLY_OFFSET_57	 @DMAWRITE_data_75, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_79
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_80
MASK_POLL_32	 0x4109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_81
MASK_POLL_32	 0x4109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_82
MASK_POLL_32	 0x4109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_83
MASK_POLL_32	 0x4149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_84
MASK_POLL_32	 0x4149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_85
MASK_POLL_32	 0x4109ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_86
MASK_POLL_32	 0x4109ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_87
MASK_POLL_32	 0x4149ed0, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_88
MASK_POLL_32	 0x4149ed4, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_89
MASK_POLL_32	 0x4109f10, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_90
MASK_POLL_32	 0x4109f10, 0x1f8003c, 0x0
MASK_POLL_32	 0x4018570, 0x1f8003c, 0x0
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
	 UC_DMA_BD	 0, 0x4109f70, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109f78, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109f80, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149f70, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149f78, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149f7c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109f74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109f7c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110010, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110020, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110030, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110050, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110060, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110070, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110080, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110090, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x41100a0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x41100b0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x41100c0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x41100d0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x41100e0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x41100f0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110100, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110110, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110120, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110130, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110140, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110150, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110160, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110170, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110180, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110190, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x41101a0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x41101b0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x41101c0, @WRITE_data_0, 1, 0, 0
UCBD_label_1:
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
	 UC_DMA_BD	 0, 0x4108900, @DMAWRITE_data_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108930, @DMAWRITE_data_13, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148000, @DMAWRITE_data_14, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148030, @DMAWRITE_data_15, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148300, @DMAWRITE_data_16, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148330, @DMAWRITE_data_17, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148900, @DMAWRITE_data_18, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108c00, @DMAWRITE_data_19, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108c30, @DMAWRITE_data_20, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108c60, @DMAWRITE_data_21, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108f00, @DMAWRITE_data_22, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108f30, @DMAWRITE_data_23, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108f60, @DMAWRITE_data_24, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109200, @DMAWRITE_data_25, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109230, @DMAWRITE_data_26, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109260, @DMAWRITE_data_27, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109500, @DMAWRITE_data_28, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109530, @DMAWRITE_data_29, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109560, @DMAWRITE_data_30, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109800, @DMAWRITE_data_31, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109830, @DMAWRITE_data_32, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109860, @DMAWRITE_data_33, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109890, @DMAWRITE_data_34, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x41098c0, @DMAWRITE_data_35, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x41098f0, @DMAWRITE_data_36, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109920, @DMAWRITE_data_37, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109950, @DMAWRITE_data_38, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148c00, @DMAWRITE_data_39, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148c30, @DMAWRITE_data_40, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148c60, @DMAWRITE_data_41, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4149200, @DMAWRITE_data_42, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4149230, @DMAWRITE_data_43, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4149260, @DMAWRITE_data_44, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4149e1c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e1c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e14, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e04, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e0c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e04, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e0c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e6c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e7c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e6c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e7c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e84, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110000, @WRITE_data_0, 1, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x4018000, @DMAWRITE_data_45, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x4018030, @DMAWRITE_data_46, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x4018060, @DMAWRITE_data_47, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x4018090, @DMAWRITE_data_48, 0x9, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x40180c0, @DMAWRITE_data_49, 0x9, 0, 0
UCBD_label_7:
	 UC_DMA_BD	 0, 0x40180f0, @DMAWRITE_data_50, 0x9, 0, 0
UCBD_label_8:
	 UC_DMA_BD	 0, 0x4018120, @DMAWRITE_data_51, 0x9, 0, 0
UCBD_label_9:
	 UC_DMA_BD	 0, 0x4018150, @DMAWRITE_data_52, 0x9, 0, 0
UCBD_label_10:
	 UC_DMA_BD	 0, 0x4018180, @DMAWRITE_data_53, 0x9, 0, 0
UCBD_label_11:
	 UC_DMA_BD	 0, 0x40181b0, @DMAWRITE_data_54, 0x9, 0, 0
UCBD_label_12:
	 UC_DMA_BD	 0, 0x40181e0, @DMAWRITE_data_55, 0x9, 0, 0
UCBD_label_13:
	 UC_DMA_BD	 0, 0x4018210, @DMAWRITE_data_56, 0x9, 0, 0
UCBD_label_14:
	 UC_DMA_BD	 0, 0x4018240, @DMAWRITE_data_57, 0x9, 0, 0
UCBD_label_15:
	 UC_DMA_BD	 0, 0x4018270, @DMAWRITE_data_58, 0x9, 0, 0
UCBD_label_16:
	 UC_DMA_BD	 0, 0x40182a0, @DMAWRITE_data_59, 0x9, 0, 0
UCBD_label_17:
	 UC_DMA_BD	 0, 0x40182d0, @DMAWRITE_data_60, 0x9, 0, 0
UCBD_label_18:
	 UC_DMA_BD	 0, 0x4018544, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018544, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018544, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018544, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018544, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018544, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018544, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018544, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x401855c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018554, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018554, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018554, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018554, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018554, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018554, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x4018554, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x4010000, @WRITE_data_0, 1, 0, 0
UCBD_label_19:
	 UC_DMA_BD	 0, 0x41086c0, @DMAWRITE_data_61, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e14, @WRITE_data_3, 1, 0, 0
UCBD_label_20:
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_2, 1, 0, 0
UCBD_label_21:
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_2, 1, 0, 0
UCBD_label_22:
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_2, 1, 0, 0
UCBD_label_23:
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_2, 1, 0, 0
UCBD_label_24:
	 UC_DMA_BD	 0, 0x4109e04, @WRITE_data_1, 1, 0, 0
UCBD_label_25:
	 UC_DMA_BD	 0, 0x4109e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_26:
	 UC_DMA_BD	 0, 0x4149e04, @WRITE_data_1, 1, 0, 0
UCBD_label_27:
	 UC_DMA_BD	 0, 0x4149e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_28:
	 UC_DMA_BD	 0, 0x41098c0, @DMAWRITE_data_62, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e84, @WRITE_data_3, 1, 0, 0
UCBD_label_29:
	 UC_DMA_BD	 0, 0x4108600, @DMAWRITE_data_63, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x41086c0, @DMAWRITE_data_64, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e14, @WRITE_data_0, 1, 0, 0
UCBD_label_30:
	 UC_DMA_BD	 0, 0x4108c30, @DMAWRITE_data_65, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108c60, @DMAWRITE_data_66, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_1, 1, 0, 0
UCBD_label_31:
	 UC_DMA_BD	 0, 0x4109230, @DMAWRITE_data_67, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109260, @DMAWRITE_data_68, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_1, 1, 0, 0
UCBD_label_32:
	 UC_DMA_BD	 0, 0x4148c30, @DMAWRITE_data_69, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148c60, @DMAWRITE_data_70, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_1, 1, 0, 0
UCBD_label_33:
	 UC_DMA_BD	 0, 0x4149230, @DMAWRITE_data_71, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4149260, @DMAWRITE_data_72, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_1, 1, 0, 0
UCBD_label_34:
	 UC_DMA_BD	 0, 0x4109e04, @WRITE_data_0, 1, 0, 0
UCBD_label_35:
	 UC_DMA_BD	 0, 0x4109e0c, @WRITE_data_0, 1, 0, 0
UCBD_label_36:
	 UC_DMA_BD	 0, 0x4149e04, @WRITE_data_0, 1, 0, 0
UCBD_label_37:
	 UC_DMA_BD	 0, 0x4149e0c, @WRITE_data_0, 1, 0, 0
UCBD_label_38:
	 UC_DMA_BD	 0, 0x41098c0, @DMAWRITE_data_35, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e84, @WRITE_data_0, 1, 0, 0
UCBD_label_39:
	 UC_DMA_BD	 0, 0x41086c0, @DMAWRITE_data_61, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e14, @WRITE_data_3, 1, 0, 0
UCBD_label_40:
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_2, 1, 0, 0
UCBD_label_41:
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_2, 1, 0, 0
UCBD_label_42:
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_2, 1, 0, 0
UCBD_label_43:
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_2, 1, 0, 0
UCBD_label_44:
	 UC_DMA_BD	 0, 0x4109e04, @WRITE_data_1, 1, 0, 0
UCBD_label_45:
	 UC_DMA_BD	 0, 0x4109e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_46:
	 UC_DMA_BD	 0, 0x4149e04, @WRITE_data_1, 1, 0, 0
UCBD_label_47:
	 UC_DMA_BD	 0, 0x4149e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_48:
	 UC_DMA_BD	 0, 0x41098c0, @DMAWRITE_data_62, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e84, @WRITE_data_3, 1, 0, 0
UCBD_label_49:
	 UC_DMA_BD	 0, 0x41086c0, @DMAWRITE_data_64, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e14, @WRITE_data_0, 1, 0, 0
UCBD_label_50:
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_1, 1, 0, 0
UCBD_label_51:
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_1, 1, 0, 0
UCBD_label_52:
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_1, 1, 0, 0
UCBD_label_53:
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_1, 1, 0, 0
UCBD_label_54:
	 UC_DMA_BD	 0, 0x4109e04, @WRITE_data_0, 1, 0, 0
UCBD_label_55:
	 UC_DMA_BD	 0, 0x4109e0c, @WRITE_data_0, 1, 0, 0
UCBD_label_56:
	 UC_DMA_BD	 0, 0x4149e04, @WRITE_data_0, 1, 0, 0
UCBD_label_57:
	 UC_DMA_BD	 0, 0x4149e0c, @WRITE_data_0, 1, 0, 0
UCBD_label_58:
	 UC_DMA_BD	 0, 0x41098c0, @DMAWRITE_data_35, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e84, @WRITE_data_0, 1, 0, 0
UCBD_label_59:
	 UC_DMA_BD	 0, 0x41086c0, @DMAWRITE_data_61, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e14, @WRITE_data_3, 1, 0, 0
UCBD_label_60:
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_2, 1, 0, 0
UCBD_label_61:
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_2, 1, 0, 0
UCBD_label_62:
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_2, 1, 0, 0
UCBD_label_63:
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_2, 1, 0, 0
UCBD_label_64:
	 UC_DMA_BD	 0, 0x4109e04, @WRITE_data_1, 1, 0, 0
UCBD_label_65:
	 UC_DMA_BD	 0, 0x4109e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_66:
	 UC_DMA_BD	 0, 0x4149e04, @WRITE_data_1, 1, 0, 0
UCBD_label_67:
	 UC_DMA_BD	 0, 0x4149e0c, @WRITE_data_1, 1, 0, 0
UCBD_label_68:
	 UC_DMA_BD	 0, 0x41098c0, @DMAWRITE_data_62, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e84, @WRITE_data_3, 1, 0, 0
UCBD_label_69:
	 UC_DMA_BD	 0, 0x4108600, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x41086c0, @DMAWRITE_data_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e14, @WRITE_data_0, 1, 0, 0
UCBD_label_70:
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_1, 1, 0, 0
UCBD_label_71:
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_1, 1, 0, 0
UCBD_label_72:
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_1, 1, 0, 0
UCBD_label_73:
	 UC_DMA_BD	 0, 0x4149230, @DMAWRITE_data_73, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4149260, @DMAWRITE_data_74, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_1, 1, 0, 0
UCBD_label_74:
	 UC_DMA_BD	 0, 0x4109e04, @WRITE_data_0, 1, 0, 0
UCBD_label_75:
	 UC_DMA_BD	 0, 0x4109e0c, @WRITE_data_0, 1, 0, 0
UCBD_label_76:
	 UC_DMA_BD	 0, 0x4149e04, @WRITE_data_0, 1, 0, 0
UCBD_label_77:
	 UC_DMA_BD	 0, 0x4149e0c, @WRITE_data_0, 1, 0, 0
UCBD_label_78:
	 UC_DMA_BD	 0, 0x41098c0, @DMAWRITE_data_35, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e84, @WRITE_data_0, 1, 0, 0
UCBD_label_79:
	 UC_DMA_BD	 0, 0x4018180, @DMAWRITE_data_75, 0x9, 0, 0
UCBD_label_80:
	 UC_DMA_BD	 0, 0x4018554, @WRITE_data_17, 1, 0, 0
UCBD_label_81:
	 UC_DMA_BD	 0, 0x41086c0, @DMAWRITE_data_76, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e14, @WRITE_data_18, 1, 0, 0
UCBD_label_82:
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_19, 1, 0, 0
UCBD_label_83:
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_19, 1, 0, 0
UCBD_label_84:
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_19, 1, 0, 0
UCBD_label_85:
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_19, 1, 0, 0
UCBD_label_86:
	 UC_DMA_BD	 0, 0x4109e04, @WRITE_data_20, 1, 0, 0
UCBD_label_87:
	 UC_DMA_BD	 0, 0x4109e0c, @WRITE_data_20, 1, 0, 0
UCBD_label_88:
	 UC_DMA_BD	 0, 0x4149e04, @WRITE_data_20, 1, 0, 0
UCBD_label_89:
	 UC_DMA_BD	 0, 0x4149e0c, @WRITE_data_20, 1, 0, 0
UCBD_label_90:
	 UC_DMA_BD	 0, 0x41098c0, @DMAWRITE_data_77, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e84, @WRITE_data_18, 1, 0, 0
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
	.long 0x00809bc0
	.long 0xfe000200
	.long 0x1d300baf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_1:
	.long 0x0080a3c0
	.long 0xfe000200
	.long 0x1d800bb9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_2:
	.long 0x00809fc0
	.long 0xfe000200
	.long 0x1d400baf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_3:
	.long 0x0080a7c0
	.long 0xfe000200
	.long 0x1d900bb9
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
	.long 0xf80002d0
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
	.long 0x00000000
DMAWRITE_data_8:
	.long 0x00800720
	.long 0xf80002d0
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
	.long 0x0080abc0
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
	.long 0x00809dc0
	.long 0xfe000200
	.long 0x1d500baf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_15:
	.long 0x0080a5c0
	.long 0xfe000200
	.long 0x1da00bb9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_16:
	.long 0x0080a1c0
	.long 0xfe000200
	.long 0x1d600baf
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_17:
	.long 0x0080a9c0
	.long 0xfe000200
	.long 0x1db00bb9
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_18:
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
DMAWRITE_data_19:
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
DMAWRITE_data_20:
	.long 0x00800400
	.long 0xfe000500
	.long 0x1ca00b93
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x0000000a
	.long 0x00f00100
	.long 0x00000000
DMAWRITE_data_21:
	.long 0x00800720
	.long 0xfe000500
	.long 0x1cf00b9d
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x0000000a
	.long 0x00f00100
	.long 0x00000000
DMAWRITE_data_22:
	.long 0x0080abc0
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
DMAWRITE_data_23:
	.long 0x00740a40
	.long 0xfe0048c0
	.long 0x19800b27
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_24:
	.long 0x00745300
	.long 0xfe0048c0
	.long 0x19f00b35
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_25:
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
DMAWRITE_data_26:
	.long 0x008004f0
	.long 0xfe000500
	.long 0x1cb00b93
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x00000000
	.long 0x00000100
	.long 0x00000000
DMAWRITE_data_27:
	.long 0x00800810
	.long 0xfe000500
	.long 0x1d000b9d
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x00000000
	.long 0x00000100
	.long 0x00000000
DMAWRITE_data_28:
	.long 0x0080abc0
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
DMAWRITE_data_29:
	.long 0x00740a40
	.long 0xfe0048c0
	.long 0x19900b27
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_30:
	.long 0x00745300
	.long 0xfe0048c0
	.long 0x1a000b35
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00210000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_31:
	.long 0x00809bc0
	.long 0xf8000400
	.long 0x1d700ba7
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000200
	.long 0x0c000000
DMAWRITE_data_32:
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
	.long 0x14000000
DMAWRITE_data_33:
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
	.long 0x1c000000
DMAWRITE_data_34:
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
DMAWRITE_data_35:
	.long 0x0080a3c0
	.long 0xf8000400
	.long 0x1dc00bb1
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000200
	.long 0x2c000000
DMAWRITE_data_36:
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
	.long 0x34000000
DMAWRITE_data_37:
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
	.long 0x3c000000
DMAWRITE_data_38:
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
DMAWRITE_data_39:
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
DMAWRITE_data_40:
	.long 0x00800450
	.long 0xfe000500
	.long 0x1cc00b93
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x00000000
	.long 0x00000100
	.long 0x00000000
DMAWRITE_data_41:
	.long 0x00800770
	.long 0xfe000500
	.long 0x1d100b9d
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x00000000
	.long 0x00000100
	.long 0x00000000
DMAWRITE_data_42:
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
DMAWRITE_data_43:
	.long 0x00800590
	.long 0xfe000500
	.long 0x1cd00b93
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x00000000
	.long 0x00000100
	.long 0x00000000
DMAWRITE_data_44:
	.long 0x008008b0
	.long 0xfe000500
	.long 0x1d200b9d
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x00000000
	.long 0x00000100
	.long 0x00000000
WRITE_data_4:
	.long 0x00030001
DMAWRITE_data_45:
	.long 0x00000000
	.long 0x00024d80
	.long 0x00000020
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_46:
	.long 0x00000000
	.long 0x00000be0
	.long 0x000002d0
	.long 0x03000000
	.long 0x02800000
	.long 0x00000140
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_47:
	.long 0x00000000
	.long 0x00013a00
	.long 0x00000400
	.long 0x03000000
	.long 0x04000000
	.long 0x00000280
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_48:
	.long 0x00000000
	.long 0x00000a00
	.long 0x00000000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_49:
	.long 0x00000000
	.long 0x00002ee0
	.long 0x00000320
	.long 0x03000000
	.long 0x02800000
	.long 0x00000140
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_50:
	.long 0x00000000
	.long 0x00000a00
	.long 0x00000000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_51:
	.long 0x00000000
	.long 0x000056e0
	.long 0x00000320
	.long 0x03000000
	.long 0x02800000
	.long 0x00000140
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_52:
	.long 0x00000000
	.long 0x00000a00
	.long 0x00000000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_53:
	.long 0x00000000
	.long 0x00007ee0
	.long 0x000002d0
	.long 0x03000000
	.long 0x02800000
	.long 0x00000140
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_54:
	.long 0x00000000
	.long 0x00013600
	.long 0x00000000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_55:
	.long 0x00000000
	.long 0x00018a00
	.long 0x00000400
	.long 0x03000000
	.long 0x04000000
	.long 0x00000280
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_56:
	.long 0x00000000
	.long 0x00013600
	.long 0x00000000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_57:
	.long 0x00000000
	.long 0x0001da00
	.long 0x00000400
	.long 0x03000000
	.long 0x04000000
	.long 0x00000280
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_58:
	.long 0x00000000
	.long 0x00013600
	.long 0x00000000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_59:
	.long 0x00000000
	.long 0x00022a00
	.long 0x00000400
	.long 0x03000000
	.long 0x04000000
	.long 0x00000280
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_60:
	.long 0x00000000
	.long 0x00013600
	.long 0x00000000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
WRITE_data_5:
	.long 0x00000009
WRITE_data_6:
	.long 0x0000000a
WRITE_data_7:
	.long 0x0000000b
WRITE_data_8:
	.long 0x0000000c
WRITE_data_9:
	.long 0x0000000d
WRITE_data_10:
	.long 0x0000000e
WRITE_data_11:
	.long 0x0000000f
WRITE_data_12:
	.long 0x00000003
WRITE_data_13:
	.long 0x00000005
WRITE_data_14:
	.long 0x00000006
WRITE_data_15:
	.long 0x00000007
WRITE_data_16:
	.long 0x00000008
DMAWRITE_data_61:
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
DMAWRITE_data_62:
	.long 0x00800000
	.long 0xf8000000
	.long 0x1dc00bb1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
DMAWRITE_data_63:
	.long 0x00800400
	.long 0xf8000320
	.long 0x1c900b95
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_64:
	.long 0x00800720
	.long 0xf8000320
	.long 0x1ce00b9f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
DMAWRITE_data_65:
	.long 0x00800400
	.long 0xfe000500
	.long 0x1ca00b93
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x00000000
	.long 0x00000100
	.long 0x00000000
DMAWRITE_data_66:
	.long 0x00800720
	.long 0xfe000500
	.long 0x1cf00b9d
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x00000000
	.long 0x00000100
	.long 0x00000000
DMAWRITE_data_67:
	.long 0x00800540
	.long 0xfe000500
	.long 0x1cb00b93
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x00000000
	.long 0x00000100
	.long 0x00000000
DMAWRITE_data_68:
	.long 0x00800860
	.long 0xfe000500
	.long 0x1d000b9d
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x00000000
	.long 0x00000100
	.long 0x00000000
DMAWRITE_data_69:
	.long 0x008004a0
	.long 0xfe000500
	.long 0x1cc00b93
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x00000000
	.long 0x00000100
	.long 0x00000000
DMAWRITE_data_70:
	.long 0x008007c0
	.long 0xfe000500
	.long 0x1d100b9d
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x00000000
	.long 0x00000100
	.long 0x00000000
DMAWRITE_data_71:
	.long 0x008005e0
	.long 0xfe000500
	.long 0x1cd00b93
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x00000000
	.long 0x00000100
	.long 0x00000000
DMAWRITE_data_72:
	.long 0x00800900
	.long 0xfe000500
	.long 0x1d200b9d
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x00000000
	.long 0x00000100
	.long 0x00000000
DMAWRITE_data_73:
	.long 0x008005e0
	.long 0xfe000500
	.long 0x1cd00b93
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x00000a00
	.long 0x00f00100
	.long 0x00000000
DMAWRITE_data_74:
	.long 0x00800900
	.long 0xfe000500
	.long 0x1d200b9d
	.long 0x00000008
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00201800
	.long 0x00000a00
	.long 0x00f00100
	.long 0x00000000
DMAWRITE_data_75:
	.long 0x00000000
	.long 0x00000a00
	.long 0x00000000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
WRITE_data_17:
	.long 0x00000008
DMAWRITE_data_76:
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
WRITE_data_18:
	.long 0x00000004
WRITE_data_19:
	.long 0x00000002
WRITE_data_20:
	.long 0x00000001
DMAWRITE_data_77:
	.long 0x00800000
	.long 0xf8000000
	.long 0x1dc00bb1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x2c000000
hintmap_0:
	.long 0x00000000
