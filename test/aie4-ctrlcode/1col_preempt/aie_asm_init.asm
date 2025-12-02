;
;text
;
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
MASK_WRITE_32	 0x6a3000, 0x2, 0x2
MASK_WRITE_32	 0x6a3000, 0x2, 0x0
MASK_WRITE_32	 0x7a3000, 0x2, 0x2
MASK_WRITE_32	 0x7a3000, 0x2, 0x0
MASK_WRITE_32	 0x8a3000, 0x2, 0x2
MASK_WRITE_32	 0x8a3000, 0x2, 0x0
MASK_WRITE_32	 0x9a3000, 0x2, 0x2
MASK_WRITE_32	 0x9a3000, 0x2, 0x0
MASK_WRITE_32	 0x6a3060, 0x1, 0x1
MASK_WRITE_32	 0x7a3060, 0x1, 0x1
MASK_WRITE_32	 0x8a3060, 0x1, 0x1
MASK_WRITE_32	 0x9a3060, 0x1, 0x1
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x120020, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x120040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x120024, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x120044, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x120028, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x120048, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x12002c, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x12004c, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x160018, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x120050, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x160024, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x160044, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x160028, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x160048, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x16002c, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x16004c, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x160020, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x160050, @WRITE_data_17, 1, 0, 1
	 UC_DMA_BD	 0, 0x120000, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x12006c, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x120004, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x120070, @WRITE_data_21, 1, 0, 1
	 UC_DMA_BD	 0, 0x160000, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x16006c, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x160004, @WRITE_data_24, 1, 0, 1
	 UC_DMA_BD	 0, 0x160070, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x120008, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x12005c, @WRITE_data_27, 1, 0, 1
	 UC_DMA_BD	 0, 0x12000c, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x120060, @WRITE_data_29, 1, 0, 1
	 UC_DMA_BD	 0, 0x160008, @WRITE_data_30, 1, 0, 1
	 UC_DMA_BD	 0, 0x16005c, @WRITE_data_31, 1, 0, 1
	 UC_DMA_BD	 0, 0x16000c, @WRITE_data_32, 1, 0, 1
	 UC_DMA_BD	 0, 0x160060, @WRITE_data_33, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4018, @WRITE_data_34, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4104, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b401c, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4104, @WRITE_data_37, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4024, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4104, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4028, @WRITE_data_40, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4104, @WRITE_data_41, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b401c, @WRITE_data_42, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4148, @WRITE_data_43, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4024, @WRITE_data_44, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4150, @WRITE_data_45, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4024, @WRITE_data_46, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4150, @WRITE_data_47, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4028, @WRITE_data_48, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4154, @WRITE_data_49, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4028, @WRITE_data_50, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4154, @WRITE_data_51, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4028, @WRITE_data_52, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4154, @WRITE_data_53, 1, 0, 1
	 UC_DMA_BD	 0, 0x8020, @WRITE_data_54, 1, 0, 1
	 UC_DMA_BD	 0, 0x8040, @WRITE_data_55, 1, 0, 1
	 UC_DMA_BD	 0, 0x8024, @WRITE_data_56, 1, 0, 1
	 UC_DMA_BD	 0, 0x8044, @WRITE_data_57, 1, 0, 1
	 UC_DMA_BD	 0, 0x48020, @WRITE_data_58, 1, 0, 1
	 UC_DMA_BD	 0, 0x48040, @WRITE_data_59, 1, 0, 1
	 UC_DMA_BD	 0, 0x48024, @WRITE_data_60, 1, 0, 1
	 UC_DMA_BD	 0, 0x48044, @WRITE_data_61, 1, 0, 1
	 UC_DMA_BD	 0, 0x8000, @WRITE_data_62, 1, 0, 1
	 UC_DMA_BD	 0, 0x48068, @WRITE_data_63, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4008, @WRITE_data_64, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4114, @WRITE_data_65, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4048, @WRITE_data_66, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4118, @WRITE_data_67, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b404c, @WRITE_data_68, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b411c, @WRITE_data_69, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4008, @WRITE_data_70, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b411c, @WRITE_data_71, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4050, @WRITE_data_72, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4120, @WRITE_data_73, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4050, @WRITE_data_74, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4120, @WRITE_data_75, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b405c, @WRITE_data_76, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4124, @WRITE_data_77, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4054, @WRITE_data_78, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4124, @WRITE_data_79, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4008, @WRITE_data_80, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4124, @WRITE_data_81, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4000, @WRITE_data_82, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4128, @WRITE_data_83, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4000, @WRITE_data_84, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4128, @WRITE_data_85, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4058, @WRITE_data_86, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4128, @WRITE_data_87, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4054, @WRITE_data_88, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b412c, @WRITE_data_89, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4058, @WRITE_data_90, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b412c, @WRITE_data_91, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4000, @WRITE_data_92, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b412c, @WRITE_data_93, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4008, @WRITE_data_94, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b412c, @WRITE_data_95, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4058, @WRITE_data_96, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4130, @WRITE_data_97, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b405c, @WRITE_data_98, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4130, @WRITE_data_99, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b405c, @WRITE_data_100, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4130, @WRITE_data_101, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4000, @WRITE_data_102, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4130, @WRITE_data_103, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x80000000
WRITE_data_1:
	.long 0x80000000
WRITE_data_2:
	.long 0x80000001
WRITE_data_3:
	.long 0x80000000
WRITE_data_4:
	.long 0x80000002
WRITE_data_5:
	.long 0x80000000
WRITE_data_6:
	.long 0x80000003
WRITE_data_7:
	.long 0x80000000
WRITE_data_8:
	.long 0x80000004
WRITE_data_9:
	.long 0x80000000
WRITE_data_10:
	.long 0x80000007
WRITE_data_11:
	.long 0x80000000
WRITE_data_12:
	.long 0x80000008
WRITE_data_13:
	.long 0x80000000
WRITE_data_14:
	.long 0x80000009
WRITE_data_15:
	.long 0x80000000
WRITE_data_16:
	.long 0x8000000a
WRITE_data_17:
	.long 0x80000000
WRITE_data_18:
	.long 0x80000016
WRITE_data_19:
	.long 0x80000000
WRITE_data_20:
	.long 0x80000017
WRITE_data_21:
	.long 0x80000000
WRITE_data_22:
	.long 0x80000019
WRITE_data_23:
	.long 0x80000000
WRITE_data_24:
	.long 0x8000001a
WRITE_data_25:
	.long 0x80000000
WRITE_data_26:
	.long 0x8000000e
WRITE_data_27:
	.long 0x80000000
WRITE_data_28:
	.long 0x8000000f
WRITE_data_29:
	.long 0x80000000
WRITE_data_30:
	.long 0x80000012
WRITE_data_31:
	.long 0x80000000
WRITE_data_32:
	.long 0x80000013
WRITE_data_33:
	.long 0x80000000
WRITE_data_34:
	.long 0x80000001
WRITE_data_35:
	.long 0x80000000
WRITE_data_36:
	.long 0x80000001
WRITE_data_37:
	.long 0x80000000
WRITE_data_38:
	.long 0x80000001
WRITE_data_39:
	.long 0x80000000
WRITE_data_40:
	.long 0x80000001
WRITE_data_41:
	.long 0x80000000
WRITE_data_42:
	.long 0x80000012
WRITE_data_43:
	.long 0x80000000
WRITE_data_44:
	.long 0x80000014
WRITE_data_45:
	.long 0x80000000
WRITE_data_46:
	.long 0x80000014
WRITE_data_47:
	.long 0x80000000
WRITE_data_48:
	.long 0x80000015
WRITE_data_49:
	.long 0x80000000
WRITE_data_50:
	.long 0x80000015
WRITE_data_51:
	.long 0x80000000
WRITE_data_52:
	.long 0x80000015
WRITE_data_53:
	.long 0x80000000
WRITE_data_54:
	.long 0x80000000
WRITE_data_55:
	.long 0x80000000
WRITE_data_56:
	.long 0x80000001
WRITE_data_57:
	.long 0x80000000
WRITE_data_58:
	.long 0x80000002
WRITE_data_59:
	.long 0x80000000
WRITE_data_60:
	.long 0x80000003
WRITE_data_61:
	.long 0x80000000
WRITE_data_62:
	.long 0x80000015
WRITE_data_63:
	.long 0x80000000
WRITE_data_64:
	.long 0x80000005
WRITE_data_65:
	.long 0x80000000
WRITE_data_66:
	.long 0x80000006
WRITE_data_67:
	.long 0x80000000
WRITE_data_68:
	.long 0x80000007
WRITE_data_69:
	.long 0x80000000
WRITE_data_70:
	.long 0x80000007
WRITE_data_71:
	.long 0x80000000
WRITE_data_72:
	.long 0x80000008
WRITE_data_73:
	.long 0x80000000
WRITE_data_74:
	.long 0x80000008
WRITE_data_75:
	.long 0x80000000
WRITE_data_76:
	.long 0x80000009
WRITE_data_77:
	.long 0x80000000
WRITE_data_78:
	.long 0x80000009
WRITE_data_79:
	.long 0x80000000
WRITE_data_80:
	.long 0x80000009
WRITE_data_81:
	.long 0x80000000
WRITE_data_82:
	.long 0x8000000a
WRITE_data_83:
	.long 0x80000000
WRITE_data_84:
	.long 0x8000000a
WRITE_data_85:
	.long 0x80000000
WRITE_data_86:
	.long 0x8000000a
WRITE_data_87:
	.long 0x80000000
WRITE_data_88:
	.long 0x8000000b
WRITE_data_89:
	.long 0x80000000
WRITE_data_90:
	.long 0x8000000b
WRITE_data_91:
	.long 0x80000000
WRITE_data_92:
	.long 0x8000000b
WRITE_data_93:
	.long 0x80000000
WRITE_data_94:
	.long 0x8000000b
WRITE_data_95:
	.long 0x80000000
WRITE_data_96:
	.long 0x8000000c
WRITE_data_97:
	.long 0x80000000
WRITE_data_98:
	.long 0x8000000c
WRITE_data_99:
	.long 0x80000000
WRITE_data_100:
	.long 0x8000000c
WRITE_data_101:
	.long 0x80000000
WRITE_data_102:
	.long 0x8000000c
WRITE_data_103:
	.long 0x80000000
