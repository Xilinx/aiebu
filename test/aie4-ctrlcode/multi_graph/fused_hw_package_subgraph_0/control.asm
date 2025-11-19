.partition	 3column
;
;text
START_JOB 0
LOAD_PDI 0, @pdi
END_JOB
.eop

;
START_JOB 1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
APPLY_OFFSET_57	 @DMAWRITE_data_69, 1, 3
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
APPLY_OFFSET_57	 @DMAWRITE_data_70, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
APPLY_OFFSET_57	 @DMAWRITE_data_71, 1, 1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
APPLY_OFFSET_57	 @DMAWRITE_data_72, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
APPLY_OFFSET_57	 @DMAWRITE_data_73, 1, 3
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
APPLY_OFFSET_57	 @DMAWRITE_data_74, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_7
APPLY_OFFSET_57	 @DMAWRITE_data_75, 1, 1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_8
APPLY_OFFSET_57	 @DMAWRITE_data_76, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_9
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_10
APPLY_OFFSET_57	 @DMAWRITE_data_77, 1, 3
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_11
APPLY_OFFSET_57	 @DMAWRITE_data_78, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_12
APPLY_OFFSET_57	 @DMAWRITE_data_79, 1, 1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_13
APPLY_OFFSET_57	 @DMAWRITE_data_80, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_14
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_15
MASK_POLL_32	 0x109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_16
MASK_POLL_32	 0x2109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_17
MASK_POLL_32	 0x4109ed8, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_18
MASK_POLL_32	 0x109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_19
MASK_POLL_32	 0x109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_20
MASK_POLL_32	 0x149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_21
MASK_POLL_32	 0x149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_22
MASK_POLL_32	 0x2109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_23
MASK_POLL_32	 0x2109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_24
MASK_POLL_32	 0x2149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_25
MASK_POLL_32	 0x2149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_26
MASK_POLL_32	 0x4109f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_27
MASK_POLL_32	 0x4109f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_28
MASK_POLL_32	 0x4149f00, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_29
MASK_POLL_32	 0x4149f08, 0x1f8003c, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_30
MASK_POLL_32	 0x109f10, 0x1f8003c, 0x0
MASK_POLL_32	 0x18570, 0x1f8003c, 0x0
MASK_POLL_32	 0x2109f10, 0x1f8003c, 0x0
MASK_POLL_32	 0x2018570, 0x1f8003c, 0x0
MASK_POLL_32	 0x4109f10, 0x1f8003c, 0x0
MASK_POLL_32	 0x4018570, 0x1f8003c, 0x0
END_JOB

.eop
pdi:
.include pdi.asm
.endl pdi

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x108000, @DMAWRITE_data_0, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x108300, @DMAWRITE_data_1, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x108600, @DMAWRITE_data_2, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x108630, @DMAWRITE_data_3, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x108660, @DMAWRITE_data_4, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x108690, @DMAWRITE_data_5, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x1086c0, @DMAWRITE_data_6, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x1086f0, @DMAWRITE_data_7, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x148000, @DMAWRITE_data_8, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x148300, @DMAWRITE_data_9, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x108c00, @DMAWRITE_data_10, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x108c30, @DMAWRITE_data_11, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x108c60, @DMAWRITE_data_12, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x109200, @DMAWRITE_data_13, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x109230, @DMAWRITE_data_14, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x109260, @DMAWRITE_data_15, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x109800, @DMAWRITE_data_16, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x148c00, @DMAWRITE_data_17, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x148c30, @DMAWRITE_data_18, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x148c60, @DMAWRITE_data_19, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x149200, @DMAWRITE_data_20, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x149230, @DMAWRITE_data_21, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x149260, @DMAWRITE_data_22, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x110010, @WRITE_data_0, 1, 0,  1
	 UC_DMA_BD	 0, 0x110020, @WRITE_data_1, 1, 0,  1
	 UC_DMA_BD	 0, 0x110030, @WRITE_data_2, 1, 0,  1
	 UC_DMA_BD	 0, 0x110040, @WRITE_data_3, 1, 0,  1
	 UC_DMA_BD	 0, 0x110050, @WRITE_data_4, 1, 0,  1
	 UC_DMA_BD	 0, 0x110060, @WRITE_data_5, 1, 0,  1
	 UC_DMA_BD	 0, 0x110070, @WRITE_data_6, 1, 0,  1
	 UC_DMA_BD	 0, 0x110080, @WRITE_data_7, 1, 0,  1
	 UC_DMA_BD	 0, 0x110090, @WRITE_data_8, 1, 0,  1
	 UC_DMA_BD	 0, 0x1100a0, @WRITE_data_9, 1, 0,  1
	 UC_DMA_BD	 0, 0x1100b0, @WRITE_data_10, 1, 0,  1
	 UC_DMA_BD	 0, 0x1100c0, @WRITE_data_11, 1, 0,  1
	 UC_DMA_BD	 0, 0x1100d0, @WRITE_data_12, 1, 0,  1
	 UC_DMA_BD	 0, 0x1100e0, @WRITE_data_13, 1, 0,  1
	 UC_DMA_BD	 0, 0x1100f0, @WRITE_data_14, 1, 0,  1
	 UC_DMA_BD	 0, 0x110100, @WRITE_data_15, 1, 0,  1
	 UC_DMA_BD	 0, 0x110110, @WRITE_data_16, 1, 0,  1
	 UC_DMA_BD	 0, 0x110120, @WRITE_data_17, 1, 0,  1
	 UC_DMA_BD	 0, 0x110130, @WRITE_data_18, 1, 0,  1
	 UC_DMA_BD	 0, 0x110140, @WRITE_data_19, 1, 0,  1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_20, 1, 0,  1
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_21, 1, 0,  1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_22, 1, 0,  1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_23, 1, 0,  1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_24, 1, 0,  1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_25, 1, 0,  1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_26, 1, 0,  1
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_27, 1, 0,  1
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_28, 1, 0,  1
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_29, 1, 0,  1
	 UC_DMA_BD	 0, 0x110000, @WRITE_data_30, 1, 0,  1
	 UC_DMA_BD	 0, 0x2108000, @DMAWRITE_data_23, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2108300, @DMAWRITE_data_24, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2108600, @DMAWRITE_data_25, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2108630, @DMAWRITE_data_26, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2108660, @DMAWRITE_data_27, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2108690, @DMAWRITE_data_28, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x21086c0, @DMAWRITE_data_29, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x21086f0, @DMAWRITE_data_30, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2148000, @DMAWRITE_data_31, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2148300, @DMAWRITE_data_32, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2108c00, @DMAWRITE_data_33, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2108c30, @DMAWRITE_data_34, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2108c60, @DMAWRITE_data_35, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2109200, @DMAWRITE_data_36, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2109230, @DMAWRITE_data_37, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2109260, @DMAWRITE_data_38, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2109800, @DMAWRITE_data_39, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2148c00, @DMAWRITE_data_40, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2148c30, @DMAWRITE_data_41, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2148c60, @DMAWRITE_data_42, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2149200, @DMAWRITE_data_43, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2149230, @DMAWRITE_data_44, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2149260, @DMAWRITE_data_45, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x2110010, @WRITE_data_31, 1, 0,  1
	 UC_DMA_BD	 0, 0x2110020, @WRITE_data_32, 1, 0,  1
	 UC_DMA_BD	 0, 0x2110030, @WRITE_data_33, 1, 0,  1
	 UC_DMA_BD	 0, 0x2110040, @WRITE_data_34, 1, 0,  1
	 UC_DMA_BD	 0, 0x2110050, @WRITE_data_35, 1, 0,  1
	 UC_DMA_BD	 0, 0x2110060, @WRITE_data_36, 1, 0,  1
	 UC_DMA_BD	 0, 0x2110070, @WRITE_data_37, 1, 0,  1
	 UC_DMA_BD	 0, 0x2110080, @WRITE_data_38, 1, 0,  1
	 UC_DMA_BD	 0, 0x2110090, @WRITE_data_39, 1, 0,  1
	 UC_DMA_BD	 0, 0x21100a0, @WRITE_data_40, 1, 0,  1
	 UC_DMA_BD	 0, 0x21100b0, @WRITE_data_41, 1, 0,  1
	 UC_DMA_BD	 0, 0x21100c0, @WRITE_data_42, 1, 0,  1
	 UC_DMA_BD	 0, 0x21100d0, @WRITE_data_43, 1, 0,  1
	 UC_DMA_BD	 0, 0x21100e0, @WRITE_data_44, 1, 0,  1
	 UC_DMA_BD	 0, 0x21100f0, @WRITE_data_45, 1, 0,  1
	 UC_DMA_BD	 0, 0x2110100, @WRITE_data_46, 1, 0,  1
	 UC_DMA_BD	 0, 0x2110110, @WRITE_data_47, 1, 0,  1
	 UC_DMA_BD	 0, 0x2110120, @WRITE_data_48, 1, 0,  1
	 UC_DMA_BD	 0, 0x2110130, @WRITE_data_49, 1, 0,  1
	 UC_DMA_BD	 0, 0x2110140, @WRITE_data_50, 1, 0,  1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_51, 1, 0,  1
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_52, 1, 0,  1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_53, 1, 0,  1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_54, 1, 0,  1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_55, 1, 0,  1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_56, 1, 0,  1
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_57, 1, 0,  1
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_58, 1, 0,  1
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_59, 1, 0,  1
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_60, 1, 0,  1
	 UC_DMA_BD	 0, 0x2110000, @WRITE_data_61, 1, 0,  1
	 UC_DMA_BD	 0, 0x4108000, @DMAWRITE_data_46, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4108300, @DMAWRITE_data_47, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4108600, @DMAWRITE_data_48, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4108630, @DMAWRITE_data_49, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4108660, @DMAWRITE_data_50, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4108690, @DMAWRITE_data_51, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x41086c0, @DMAWRITE_data_52, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x41086f0, @DMAWRITE_data_53, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4148000, @DMAWRITE_data_54, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4148300, @DMAWRITE_data_55, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4108c00, @DMAWRITE_data_56, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4108c30, @DMAWRITE_data_57, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4108c60, @DMAWRITE_data_58, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4109200, @DMAWRITE_data_59, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4109230, @DMAWRITE_data_60, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4109260, @DMAWRITE_data_61, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4109800, @DMAWRITE_data_62, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4148c00, @DMAWRITE_data_63, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4148c30, @DMAWRITE_data_64, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4148c60, @DMAWRITE_data_65, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4149200, @DMAWRITE_data_66, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4149230, @DMAWRITE_data_67, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4149260, @DMAWRITE_data_68, 0xb, 0,  1
	 UC_DMA_BD	 0, 0x4110010, @WRITE_data_62, 1, 0,  1
	 UC_DMA_BD	 0, 0x4110020, @WRITE_data_63, 1, 0,  1
	 UC_DMA_BD	 0, 0x4110030, @WRITE_data_64, 1, 0,  1
	 UC_DMA_BD	 0, 0x4110040, @WRITE_data_65, 1, 0,  1
	 UC_DMA_BD	 0, 0x4110050, @WRITE_data_66, 1, 0,  1
	 UC_DMA_BD	 0, 0x4110060, @WRITE_data_67, 1, 0,  1
	 UC_DMA_BD	 0, 0x4110070, @WRITE_data_68, 1, 0,  1
	 UC_DMA_BD	 0, 0x4110080, @WRITE_data_69, 1, 0,  1
	 UC_DMA_BD	 0, 0x4110090, @WRITE_data_70, 1, 0,  1
	 UC_DMA_BD	 0, 0x41100a0, @WRITE_data_71, 1, 0,  1
	 UC_DMA_BD	 0, 0x41100b0, @WRITE_data_72, 1, 0,  1
	 UC_DMA_BD	 0, 0x41100c0, @WRITE_data_73, 1, 0,  1
	 UC_DMA_BD	 0, 0x41100d0, @WRITE_data_74, 1, 0,  1
	 UC_DMA_BD	 0, 0x41100e0, @WRITE_data_75, 1, 0,  1
	 UC_DMA_BD	 0, 0x41100f0, @WRITE_data_76, 1, 0,  1
	 UC_DMA_BD	 0, 0x4110100, @WRITE_data_77, 1, 0,  1
	 UC_DMA_BD	 0, 0x4110110, @WRITE_data_78, 1, 0,  1
	 UC_DMA_BD	 0, 0x4110120, @WRITE_data_79, 1, 0,  1
	 UC_DMA_BD	 0, 0x4110130, @WRITE_data_80, 1, 0,  1
	 UC_DMA_BD	 0, 0x4110140, @WRITE_data_81, 1, 0,  1
	 UC_DMA_BD	 0, 0x4109e14, @WRITE_data_82, 1, 0,  1
	 UC_DMA_BD	 0, 0x4109e14, @WRITE_data_83, 1, 0,  1
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_84, 1, 0,  1
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_85, 1, 0,  1
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_86, 1, 0,  1
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_87, 1, 0,  1
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_88, 1, 0,  1
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_89, 1, 0,  1
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_90, 1, 0,  1
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_91, 1, 0,  1
	 UC_DMA_BD	 0, 0x4110000, @WRITE_data_92, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_69, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x18030, @DMAWRITE_data_70, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x18060, @DMAWRITE_data_71, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x18090, @DMAWRITE_data_72, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_93, 1, 0,  1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_94, 1, 0,  1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_95, 1, 0,  1
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_96, 1, 0,  1
	 UC_DMA_BD	 0, 0x10000, @WRITE_data_97, 1, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x2018000, @DMAWRITE_data_73, 0x9, 0, 0
UCBD_label_7:
	 UC_DMA_BD	 0, 0x2018030, @DMAWRITE_data_74, 0x9, 0, 0
UCBD_label_8:
	 UC_DMA_BD	 0, 0x2018060, @DMAWRITE_data_75, 0x9, 0, 0
UCBD_label_9:
	 UC_DMA_BD	 0, 0x2018090, @DMAWRITE_data_76, 0x9, 0, 0
UCBD_label_10:
	 UC_DMA_BD	 0, 0x2018544, @WRITE_data_98, 1, 0,  1
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_99, 1, 0,  1
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_100, 1, 0,  1
	 UC_DMA_BD	 0, 0x2018554, @WRITE_data_101, 1, 0,  1
	 UC_DMA_BD	 0, 0x2010000, @WRITE_data_102, 1, 0, 0
UCBD_label_11:
	 UC_DMA_BD	 0, 0x4018000, @DMAWRITE_data_77, 0x9, 0, 0
UCBD_label_12:
	 UC_DMA_BD	 0, 0x4018030, @DMAWRITE_data_78, 0x9, 0, 0
UCBD_label_13:
	 UC_DMA_BD	 0, 0x4018060, @DMAWRITE_data_79, 0x9, 0, 0
UCBD_label_14:
	 UC_DMA_BD	 0, 0x4018090, @DMAWRITE_data_80, 0x9, 0, 0
UCBD_label_15:
	 UC_DMA_BD	 0, 0x4018544, @WRITE_data_103, 1, 0,  1
	 UC_DMA_BD	 0, 0x4018554, @WRITE_data_104, 1, 0,  1
	 UC_DMA_BD	 0, 0x4018554, @WRITE_data_105, 1, 0,  1
	 UC_DMA_BD	 0, 0x4018554, @WRITE_data_106, 1, 0,  1
	 UC_DMA_BD	 0, 0x4010000, @WRITE_data_107, 1, 0, 0
UCBD_label_16:
	 UC_DMA_BD	 0, 0x109e14, @WRITE_data_108, 1, 0, 0
UCBD_label_17:
	 UC_DMA_BD	 0, 0x2109e14, @WRITE_data_109, 1, 0, 0
UCBD_label_18:
	 UC_DMA_BD	 0, 0x4109e14, @WRITE_data_110, 1, 0,  1
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_111, 1, 0,  1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_112, 1, 0,  1
	 UC_DMA_BD	 0, 0x149e04, @WRITE_data_113, 1, 0,  1
	 UC_DMA_BD	 0, 0x149e0c, @WRITE_data_114, 1, 0,  1
	 UC_DMA_BD	 0, 0x2109e04, @WRITE_data_115, 1, 0,  1
	 UC_DMA_BD	 0, 0x2109e0c, @WRITE_data_116, 1, 0,  1
	 UC_DMA_BD	 0, 0x2149e04, @WRITE_data_117, 1, 0,  1
	 UC_DMA_BD	 0, 0x2149e0c, @WRITE_data_118, 1, 0,  1
	 UC_DMA_BD	 0, 0x4109e04, @WRITE_data_119, 1, 0,  1
	 UC_DMA_BD	 0, 0x4109e0c, @WRITE_data_120, 1, 0,  1
	 UC_DMA_BD	 0, 0x4149e04, @WRITE_data_121, 1, 0,  1
	 UC_DMA_BD	 0, 0x4149e0c, @WRITE_data_122, 1, 0, 0
UCBD_label_19:
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_123, 1, 0, 0
UCBD_label_20:
	 UC_DMA_BD	 0, 0x109e74, @WRITE_data_124, 1, 0, 0
UCBD_label_21:
	 UC_DMA_BD	 0, 0x149e64, @WRITE_data_125, 1, 0, 0
UCBD_label_22:
	 UC_DMA_BD	 0, 0x149e74, @WRITE_data_126, 1, 0, 0
UCBD_label_23:
	 UC_DMA_BD	 0, 0x2109e64, @WRITE_data_127, 1, 0, 0
UCBD_label_24:
	 UC_DMA_BD	 0, 0x2109e74, @WRITE_data_128, 1, 0, 0
UCBD_label_25:
	 UC_DMA_BD	 0, 0x2149e64, @WRITE_data_129, 1, 0, 0
UCBD_label_26:
	 UC_DMA_BD	 0, 0x2149e74, @WRITE_data_130, 1, 0, 0
UCBD_label_27:
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_131, 1, 0, 0
UCBD_label_28:
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_132, 1, 0, 0
UCBD_label_29:
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_133, 1, 0, 0
UCBD_label_30:
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_134, 1, 0,  1
	 UC_DMA_BD	 0, 0x109e84, @WRITE_data_135, 1, 0,  1
	 UC_DMA_BD	 0, 0x2109e84, @WRITE_data_136, 1, 0,  1
	 UC_DMA_BD	 0, 0x4109e84, @WRITE_data_137, 1, 0, 0
.align    4
DMAWRITE_data_0:
	.long 0x008181c0
	.long 0xfe003000
	.long 0x1d000ba3
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_1:
	.long 0x0081b1c0
	.long 0xfe003000
	.long 0x1d100ba5
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
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
	.long 0x0080c000
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
DMAWRITE_data_7:
	.long 0x00800000
	.long 0xfe000300
	.long 0x1cb00b99
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_8:
	.long 0x0081e1c0
	.long 0xfe003000
	.long 0x1d200ba7
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_9:
	.long 0x008211c0
	.long 0xfe003000
	.long 0x1d300ba9
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_10:
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
DMAWRITE_data_11:
	.long 0x0080c000
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
DMAWRITE_data_12:
	.long 0x00800000
	.long 0xfe003000
	.long 0x1cc00b9b
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_13:
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
DMAWRITE_data_14:
	.long 0x0080c000
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
DMAWRITE_data_15:
	.long 0x00803000
	.long 0xfe003000
	.long 0x1cd00b9d
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_16:
	.long 0x008181c0
	.long 0xfe000300
	.long 0x1d400ba1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_17:
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
DMAWRITE_data_18:
	.long 0x0080c000
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
DMAWRITE_data_19:
	.long 0x00806000
	.long 0xfe003000
	.long 0x1ce00b9f
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_20:
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
DMAWRITE_data_21:
	.long 0x0080c000
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
DMAWRITE_data_22:
	.long 0x00809000
	.long 0xfe003000
	.long 0x1cf00b97
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
WRITE_data_0:
	.long 0x00000004
WRITE_data_1:
	.long 0x00000000
WRITE_data_2:
	.long 0x00000000
WRITE_data_3:
	.long 0x00000000
WRITE_data_4:
	.long 0x00000000
WRITE_data_5:
	.long 0x00000001
WRITE_data_6:
	.long 0x00000000
WRITE_data_7:
	.long 0x00000000
WRITE_data_8:
	.long 0x00000000
WRITE_data_9:
	.long 0x00000000
WRITE_data_10:
	.long 0x00000001
WRITE_data_11:
	.long 0x00000000
WRITE_data_12:
	.long 0x00000000
WRITE_data_13:
	.long 0x00000000
WRITE_data_14:
	.long 0x00000000
WRITE_data_15:
	.long 0x00000001
WRITE_data_16:
	.long 0x00000000
WRITE_data_17:
	.long 0x00000000
WRITE_data_18:
	.long 0x00000000
WRITE_data_19:
	.long 0x00000000
WRITE_data_20:
	.long 0x00000000
WRITE_data_21:
	.long 0x00000004
WRITE_data_22:
	.long 0x00000000
WRITE_data_23:
	.long 0x00000000
WRITE_data_24:
	.long 0x00000000
WRITE_data_25:
	.long 0x00000000
WRITE_data_26:
	.long 0x00000001
WRITE_data_27:
	.long 0x00000001
WRITE_data_28:
	.long 0x00000001
WRITE_data_29:
	.long 0x00000001
WRITE_data_30:
	.long 0x00000000
DMAWRITE_data_23:
	.long 0x008181c0
	.long 0xfe003000
	.long 0x1d000ba3
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_24:
	.long 0x0081b1c0
	.long 0xfe003000
	.long 0x1d100ba5
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_25:
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
DMAWRITE_data_26:
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
DMAWRITE_data_27:
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
DMAWRITE_data_28:
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
DMAWRITE_data_29:
	.long 0x0080c000
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
DMAWRITE_data_30:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1cb00b99
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_31:
	.long 0x0081e1c0
	.long 0xfe003000
	.long 0x1d200ba7
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_32:
	.long 0x008211c0
	.long 0xfe003000
	.long 0x1d300ba9
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_33:
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
DMAWRITE_data_34:
	.long 0x0080c000
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
DMAWRITE_data_35:
	.long 0x00800000
	.long 0xfe003000
	.long 0x1cc00b9b
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_36:
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
DMAWRITE_data_37:
	.long 0x0080c000
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
DMAWRITE_data_38:
	.long 0x00803000
	.long 0xfe003000
	.long 0x1cd00b9d
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_39:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1d400ba1
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
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_41:
	.long 0x0080c000
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
DMAWRITE_data_42:
	.long 0x00806000
	.long 0xfe003000
	.long 0x1ce00b9f
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_43:
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
DMAWRITE_data_44:
	.long 0x0080c000
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
DMAWRITE_data_45:
	.long 0x00809000
	.long 0xfe003000
	.long 0x1cf00b97
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
WRITE_data_31:
	.long 0x00000004
WRITE_data_32:
	.long 0x00000000
WRITE_data_33:
	.long 0x00000000
WRITE_data_34:
	.long 0x00000000
WRITE_data_35:
	.long 0x00000000
WRITE_data_36:
	.long 0x00000001
WRITE_data_37:
	.long 0x00000000
WRITE_data_38:
	.long 0x00000000
WRITE_data_39:
	.long 0x00000000
WRITE_data_40:
	.long 0x00000000
WRITE_data_41:
	.long 0x00000001
WRITE_data_42:
	.long 0x00000000
WRITE_data_43:
	.long 0x00000000
WRITE_data_44:
	.long 0x00000000
WRITE_data_45:
	.long 0x00000000
WRITE_data_46:
	.long 0x00000001
WRITE_data_47:
	.long 0x00000000
WRITE_data_48:
	.long 0x00000000
WRITE_data_49:
	.long 0x00000000
WRITE_data_50:
	.long 0x00000000
WRITE_data_51:
	.long 0x00000000
WRITE_data_52:
	.long 0x00000004
WRITE_data_53:
	.long 0x00000000
WRITE_data_54:
	.long 0x00000000
WRITE_data_55:
	.long 0x00000000
WRITE_data_56:
	.long 0x00000000
WRITE_data_57:
	.long 0x00000001
WRITE_data_58:
	.long 0x00000001
WRITE_data_59:
	.long 0x00000001
WRITE_data_60:
	.long 0x00000001
WRITE_data_61:
	.long 0x00000000
DMAWRITE_data_46:
	.long 0x008181c0
	.long 0xfe003000
	.long 0x1d000ba3
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_47:
	.long 0x0081b1c0
	.long 0xfe003000
	.long 0x1d100ba5
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_48:
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
DMAWRITE_data_49:
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
DMAWRITE_data_50:
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
DMAWRITE_data_51:
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
DMAWRITE_data_52:
	.long 0x0080c000
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
DMAWRITE_data_53:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1cb00b99
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_54:
	.long 0x0081e1c0
	.long 0xfe003000
	.long 0x1d200ba7
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_55:
	.long 0x008211c0
	.long 0xfe003000
	.long 0x1d300ba9
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_56:
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
DMAWRITE_data_57:
	.long 0x0080c000
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
DMAWRITE_data_58:
	.long 0x00800000
	.long 0xfe003000
	.long 0x1cc00b9b
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_59:
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
DMAWRITE_data_60:
	.long 0x0080c000
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
DMAWRITE_data_61:
	.long 0x00803000
	.long 0xfe003000
	.long 0x1cd00b9d
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_62:
	.long 0x00800000
	.long 0xfe000000
	.long 0x1d400ba1
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_63:
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
DMAWRITE_data_64:
	.long 0x0080c000
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
DMAWRITE_data_65:
	.long 0x00806000
	.long 0xfe003000
	.long 0x1ce00b9f
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
DMAWRITE_data_66:
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
DMAWRITE_data_67:
	.long 0x0080c000
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
DMAWRITE_data_68:
	.long 0x00809000
	.long 0xfe003000
	.long 0x1cf00b97
	.long 0x00000300
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00800400
	.long 0x00000000
WRITE_data_62:
	.long 0x00000004
WRITE_data_63:
	.long 0x00000000
WRITE_data_64:
	.long 0x00000000
WRITE_data_65:
	.long 0x00000000
WRITE_data_66:
	.long 0x00000000
WRITE_data_67:
	.long 0x00000001
WRITE_data_68:
	.long 0x00000000
WRITE_data_69:
	.long 0x00000000
WRITE_data_70:
	.long 0x00000000
WRITE_data_71:
	.long 0x00000000
WRITE_data_72:
	.long 0x00000001
WRITE_data_73:
	.long 0x00000000
WRITE_data_74:
	.long 0x00000000
WRITE_data_75:
	.long 0x00000000
WRITE_data_76:
	.long 0x00000000
WRITE_data_77:
	.long 0x00000001
WRITE_data_78:
	.long 0x00000000
WRITE_data_79:
	.long 0x00000000
WRITE_data_80:
	.long 0x00000000
WRITE_data_81:
	.long 0x00000000
WRITE_data_82:
	.long 0x00000000
WRITE_data_83:
	.long 0x00000004
WRITE_data_84:
	.long 0x00000000
WRITE_data_85:
	.long 0x00000000
WRITE_data_86:
	.long 0x00000000
WRITE_data_87:
	.long 0x00000000
WRITE_data_88:
	.long 0x00000001
WRITE_data_89:
	.long 0x00000001
WRITE_data_90:
	.long 0x00000001
WRITE_data_91:
	.long 0x00000001
WRITE_data_92:
	.long 0x00000000
DMAWRITE_data_69:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_70:
	.long 0x00000000
	.long 0x00000000
	.long 0x000000e0
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_71:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000300
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_72:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000300
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
WRITE_data_93:
	.long 0x00000003
WRITE_data_94:
	.long 0x00000000
WRITE_data_95:
	.long 0x00000001
WRITE_data_96:
	.long 0x00000002
WRITE_data_97:
	.long 0x00000000
DMAWRITE_data_73:
	.long 0x00000000
	.long 0x00001000
	.long 0x00000400
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_74:
	.long 0x00000000
	.long 0x00000000
	.long 0x000000e0
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_75:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_76:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
WRITE_data_98:
	.long 0x00000003
WRITE_data_99:
	.long 0x00000000
WRITE_data_100:
	.long 0x00000001
WRITE_data_101:
	.long 0x00000002
WRITE_data_102:
	.long 0x00000000
DMAWRITE_data_77:
	.long 0x00000000
	.long 0x00002000
	.long 0x00000400
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_78:
	.long 0x00000000
	.long 0x00000000
	.long 0x000000e0
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_79:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
DMAWRITE_data_80:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
WRITE_data_103:
	.long 0x00000003
WRITE_data_104:
	.long 0x00000000
WRITE_data_105:
	.long 0x00000001
WRITE_data_106:
	.long 0x00000002
WRITE_data_107:
	.long 0x00000000
WRITE_data_108:
	.long 0x00000005
WRITE_data_109:
	.long 0x00000005
WRITE_data_110:
	.long 0x00000005
WRITE_data_111:
	.long 0x00000000
WRITE_data_112:
	.long 0x00000000
WRITE_data_113:
	.long 0x00000000
WRITE_data_114:
	.long 0x00000000
WRITE_data_115:
	.long 0x00000000
WRITE_data_116:
	.long 0x00000000
WRITE_data_117:
	.long 0x00000000
WRITE_data_118:
	.long 0x00000000
WRITE_data_119:
	.long 0x00000000
WRITE_data_120:
	.long 0x00000000
WRITE_data_121:
	.long 0x00000000
WRITE_data_122:
	.long 0x00000000
WRITE_data_123:
	.long 0x00000002
WRITE_data_124:
	.long 0x00000002
WRITE_data_125:
	.long 0x00000002
WRITE_data_126:
	.long 0x00000002
WRITE_data_127:
	.long 0x00000002
WRITE_data_128:
	.long 0x00000002
WRITE_data_129:
	.long 0x00000002
WRITE_data_130:
	.long 0x00000002
WRITE_data_131:
	.long 0x00000002
WRITE_data_132:
	.long 0x00000002
WRITE_data_133:
	.long 0x00000002
WRITE_data_134:
	.long 0x00000002
WRITE_data_135:
	.long 0x00000000
WRITE_data_136:
	.long 0x00000000
WRITE_data_137:
	.long 0x00000000
