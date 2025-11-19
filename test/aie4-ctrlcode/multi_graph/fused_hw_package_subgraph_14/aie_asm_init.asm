.partition	 3column
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
MASK_WRITE_32	 0x26a3000, 0x2, 0x2
MASK_WRITE_32	 0x26a3000, 0x2, 0x0
MASK_WRITE_32	 0x27a3000, 0x2, 0x2
MASK_WRITE_32	 0x27a3000, 0x2, 0x0
MASK_WRITE_32	 0x28a3000, 0x2, 0x2
MASK_WRITE_32	 0x28a3000, 0x2, 0x0
MASK_WRITE_32	 0x29a3000, 0x2, 0x2
MASK_WRITE_32	 0x29a3000, 0x2, 0x0
MASK_WRITE_32	 0x46a3000, 0x2, 0x2
MASK_WRITE_32	 0x46a3000, 0x2, 0x0
MASK_WRITE_32	 0x47a3000, 0x2, 0x2
MASK_WRITE_32	 0x47a3000, 0x2, 0x0
MASK_WRITE_32	 0x48a3000, 0x2, 0x2
MASK_WRITE_32	 0x48a3000, 0x2, 0x0
MASK_WRITE_32	 0x49a3000, 0x2, 0x2
MASK_WRITE_32	 0x49a3000, 0x2, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
END_JOB

.eop

START_JOB 1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
MASK_WRITE_32	 0x6a3060, 0x1, 0x1
MASK_WRITE_32	 0x7a3060, 0x1, 0x1
MASK_WRITE_32	 0x8a3060, 0x1, 0x1
MASK_WRITE_32	 0x9a3060, 0x1, 0x1
MASK_WRITE_32	 0x26a3060, 0x1, 0x1
MASK_WRITE_32	 0x27a3060, 0x1, 0x1
MASK_WRITE_32	 0x28a3060, 0x1, 0x1
MASK_WRITE_32	 0x29a3060, 0x1, 0x1
MASK_WRITE_32	 0x46a3060, 0x1, 0x1
MASK_WRITE_32	 0x47a3060, 0x1, 0x1
MASK_WRITE_32	 0x48a3060, 0x1, 0x1
MASK_WRITE_32	 0x49a3060, 0x1, 0x1
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x120028, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x120040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120028, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120040, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120028, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120040, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x160028, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x120044, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160028, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120044, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x12002c, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x120048, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x212002c, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120048, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x412002c, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120048, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x16002c, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x12004c, @WRITE_data_17, 1, 0, 1
	 UC_DMA_BD	 0, 0x416002c, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x412004c, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x160014, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x120050, @WRITE_data_21, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160014, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120050, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160014, @WRITE_data_24, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120050, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x160020, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x160040, @WRITE_data_27, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160020, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160040, @WRITE_data_29, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160020, @WRITE_data_30, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160040, @WRITE_data_31, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160028, @WRITE_data_32, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160044, @WRITE_data_33, 1, 0, 1
	 UC_DMA_BD	 0, 0x160024, @WRITE_data_34, 1, 0, 1
	 UC_DMA_BD	 0, 0x160048, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160024, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160048, @WRITE_data_37, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160024, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160048, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x160018, @WRITE_data_40, 1, 0, 1
	 UC_DMA_BD	 0, 0x16004c, @WRITE_data_41, 1, 0, 1
	 UC_DMA_BD	 0, 0x216002c, @WRITE_data_42, 1, 0, 1
	 UC_DMA_BD	 0, 0x216004c, @WRITE_data_43, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160018, @WRITE_data_44, 1, 0, 1
	 UC_DMA_BD	 0, 0x416004c, @WRITE_data_45, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160018, @WRITE_data_46, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160050, @WRITE_data_47, 1, 0, 1
	 UC_DMA_BD	 0, 0x120000, @WRITE_data_48, 1, 0, 1
	 UC_DMA_BD	 0, 0x12006c, @WRITE_data_49, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120000, @WRITE_data_50, 1, 0, 1
	 UC_DMA_BD	 0, 0x212006c, @WRITE_data_51, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120000, @WRITE_data_52, 1, 0, 1
	 UC_DMA_BD	 0, 0x412006c, @WRITE_data_53, 1, 0, 1
	 UC_DMA_BD	 0, 0x120004, @WRITE_data_54, 1, 0, 1
	 UC_DMA_BD	 0, 0x120070, @WRITE_data_55, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120004, @WRITE_data_56, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120070, @WRITE_data_57, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120004, @WRITE_data_58, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120070, @WRITE_data_59, 1, 0, 1
	 UC_DMA_BD	 0, 0x160000, @WRITE_data_60, 1, 0, 1
	 UC_DMA_BD	 0, 0x16006c, @WRITE_data_61, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160000, @WRITE_data_62, 1, 0, 1
	 UC_DMA_BD	 0, 0x216006c, @WRITE_data_63, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160000, @WRITE_data_64, 1, 0, 1
	 UC_DMA_BD	 0, 0x416006c, @WRITE_data_65, 1, 0, 1
	 UC_DMA_BD	 0, 0x160004, @WRITE_data_66, 1, 0, 1
	 UC_DMA_BD	 0, 0x160070, @WRITE_data_67, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160004, @WRITE_data_68, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160070, @WRITE_data_69, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160004, @WRITE_data_70, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160070, @WRITE_data_71, 1, 0, 1
	 UC_DMA_BD	 0, 0x120008, @WRITE_data_72, 1, 0, 1
	 UC_DMA_BD	 0, 0x12005c, @WRITE_data_73, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120008, @WRITE_data_74, 1, 0, 1
	 UC_DMA_BD	 0, 0x212005c, @WRITE_data_75, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120008, @WRITE_data_76, 1, 0, 1
	 UC_DMA_BD	 0, 0x412005c, @WRITE_data_77, 1, 0, 1
	 UC_DMA_BD	 0, 0x12000c, @WRITE_data_78, 1, 0, 1
	 UC_DMA_BD	 0, 0x120060, @WRITE_data_79, 1, 0, 1
	 UC_DMA_BD	 0, 0x212000c, @WRITE_data_80, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120060, @WRITE_data_81, 1, 0, 1
	 UC_DMA_BD	 0, 0x412000c, @WRITE_data_82, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120060, @WRITE_data_83, 1, 0, 1
	 UC_DMA_BD	 0, 0x160008, @WRITE_data_84, 1, 0, 1
	 UC_DMA_BD	 0, 0x16005c, @WRITE_data_85, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160008, @WRITE_data_86, 1, 0, 1
	 UC_DMA_BD	 0, 0x216005c, @WRITE_data_87, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160008, @WRITE_data_88, 1, 0, 1
	 UC_DMA_BD	 0, 0x416005c, @WRITE_data_89, 1, 0, 1
	 UC_DMA_BD	 0, 0x16000c, @WRITE_data_90, 1, 0, 1
	 UC_DMA_BD	 0, 0x160060, @WRITE_data_91, 1, 0, 1
	 UC_DMA_BD	 0, 0x216000c, @WRITE_data_92, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160060, @WRITE_data_93, 1, 0, 1
	 UC_DMA_BD	 0, 0x416000c, @WRITE_data_94, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160060, @WRITE_data_95, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4018, @WRITE_data_96, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4104, @WRITE_data_97, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b401c, @WRITE_data_98, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4104, @WRITE_data_99, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4024, @WRITE_data_100, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4104, @WRITE_data_101, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4028, @WRITE_data_102, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4104, @WRITE_data_103, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4018, @WRITE_data_104, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4104, @WRITE_data_105, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b401c, @WRITE_data_106, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4104, @WRITE_data_107, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4024, @WRITE_data_108, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4104, @WRITE_data_109, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4028, @WRITE_data_110, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4104, @WRITE_data_111, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4018, @WRITE_data_112, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4104, @WRITE_data_113, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b401c, @WRITE_data_114, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4104, @WRITE_data_115, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4024, @WRITE_data_116, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4104, @WRITE_data_117, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4028, @WRITE_data_118, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4104, @WRITE_data_119, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b401c, @WRITE_data_120, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4148, @WRITE_data_121, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b401c, @WRITE_data_122, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4148, @WRITE_data_123, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b401c, @WRITE_data_124, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4148, @WRITE_data_125, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4024, @WRITE_data_126, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4150, @WRITE_data_127, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4024, @WRITE_data_128, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4150, @WRITE_data_129, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4024, @WRITE_data_130, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4150, @WRITE_data_131, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4024, @WRITE_data_132, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4150, @WRITE_data_133, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4024, @WRITE_data_134, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4150, @WRITE_data_135, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4024, @WRITE_data_136, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4150, @WRITE_data_137, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4028, @WRITE_data_138, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4154, @WRITE_data_139, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4028, @WRITE_data_140, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4154, @WRITE_data_141, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4028, @WRITE_data_142, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4154, @WRITE_data_143, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4028, @WRITE_data_144, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4154, @WRITE_data_145, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4028, @WRITE_data_146, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4154, @WRITE_data_147, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4028, @WRITE_data_148, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4154, @WRITE_data_149, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4028, @WRITE_data_150, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4154, @WRITE_data_151, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4028, @WRITE_data_152, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4154, @WRITE_data_153, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4028, @WRITE_data_154, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4154, @WRITE_data_155, 1, 0, 1
	 UC_DMA_BD	 0, 0x8020, @WRITE_data_156, 1, 0, 1
	 UC_DMA_BD	 0, 0x8040, @WRITE_data_157, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008020, @WRITE_data_158, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008040, @WRITE_data_159, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008020, @WRITE_data_160, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008040, @WRITE_data_161, 1, 0, 1
	 UC_DMA_BD	 0, 0x8024, @WRITE_data_162, 1, 0, 1
	 UC_DMA_BD	 0, 0x8044, @WRITE_data_163, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008024, @WRITE_data_164, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008044, @WRITE_data_165, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008024, @WRITE_data_166, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008044, @WRITE_data_167, 1, 0, 1
	 UC_DMA_BD	 0, 0x48020, @WRITE_data_168, 1, 0, 1
	 UC_DMA_BD	 0, 0x48040, @WRITE_data_169, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048020, @WRITE_data_170, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048040, @WRITE_data_171, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048020, @WRITE_data_172, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048040, @WRITE_data_173, 1, 0, 1
	 UC_DMA_BD	 0, 0x48024, @WRITE_data_174, 1, 0, 1
	 UC_DMA_BD	 0, 0x48044, @WRITE_data_175, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048024, @WRITE_data_176, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048044, @WRITE_data_177, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048024, @WRITE_data_178, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048044, @WRITE_data_179, 1, 0, 1
	 UC_DMA_BD	 0, 0x8000, @WRITE_data_180, 1, 0, 1
	 UC_DMA_BD	 0, 0x48064, @WRITE_data_181, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008000, @WRITE_data_182, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048064, @WRITE_data_183, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008000, @WRITE_data_184, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048064, @WRITE_data_185, 1, 0, 1
	 UC_DMA_BD	 0, 0x48000, @WRITE_data_186, 1, 0, 1
	 UC_DMA_BD	 0, 0x48068, @WRITE_data_187, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048000, @WRITE_data_188, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048068, @WRITE_data_189, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048000, @WRITE_data_190, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048068, @WRITE_data_191, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4008, @WRITE_data_192, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b411c, @WRITE_data_193, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4008, @WRITE_data_194, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b411c, @WRITE_data_195, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4008, @WRITE_data_196, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b411c, @WRITE_data_197, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b404c, @WRITE_data_198, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4120, @WRITE_data_199, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4008, @WRITE_data_200, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4120, @WRITE_data_201, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b404c, @WRITE_data_202, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4120, @WRITE_data_203, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4008, @WRITE_data_204, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4120, @WRITE_data_205, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b404c, @WRITE_data_206, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4120, @WRITE_data_207, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4008, @WRITE_data_208, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4120, @WRITE_data_209, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4050, @WRITE_data_210, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4124, @WRITE_data_211, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4050, @WRITE_data_212, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4124, @WRITE_data_213, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4008, @WRITE_data_214, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4124, @WRITE_data_215, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4050, @WRITE_data_216, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4124, @WRITE_data_217, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4050, @WRITE_data_218, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4124, @WRITE_data_219, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4008, @WRITE_data_220, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4124, @WRITE_data_221, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4050, @WRITE_data_222, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4124, @WRITE_data_223, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4050, @WRITE_data_224, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4124, @WRITE_data_225, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4008, @WRITE_data_226, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4124, @WRITE_data_227, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4054, @WRITE_data_228, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4128, @WRITE_data_229, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4054, @WRITE_data_230, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4128, @WRITE_data_231, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4058, @WRITE_data_232, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4128, @WRITE_data_233, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4054, @WRITE_data_234, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4128, @WRITE_data_235, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4054, @WRITE_data_236, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4128, @WRITE_data_237, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4058, @WRITE_data_238, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4128, @WRITE_data_239, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4054, @WRITE_data_240, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4128, @WRITE_data_241, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4054, @WRITE_data_242, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4128, @WRITE_data_243, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4058, @WRITE_data_244, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4128, @WRITE_data_245, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4058, @WRITE_data_246, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b412c, @WRITE_data_247, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b400c, @WRITE_data_248, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b412c, @WRITE_data_249, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4058, @WRITE_data_250, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b412c, @WRITE_data_251, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b400c, @WRITE_data_252, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b412c, @WRITE_data_253, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4008, @WRITE_data_254, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b412c, @WRITE_data_255, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4058, @WRITE_data_256, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b412c, @WRITE_data_257, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b400c, @WRITE_data_258, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b412c, @WRITE_data_259, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4058, @WRITE_data_260, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b412c, @WRITE_data_261, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b400c, @WRITE_data_262, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b412c, @WRITE_data_263, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4008, @WRITE_data_264, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b412c, @WRITE_data_265, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4058, @WRITE_data_266, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b412c, @WRITE_data_267, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b400c, @WRITE_data_268, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b412c, @WRITE_data_269, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4058, @WRITE_data_270, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b412c, @WRITE_data_271, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b400c, @WRITE_data_272, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b412c, @WRITE_data_273, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4008, @WRITE_data_274, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b412c, @WRITE_data_275, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b405c, @WRITE_data_276, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4130, @WRITE_data_277, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b405c, @WRITE_data_278, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4130, @WRITE_data_279, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b400c, @WRITE_data_280, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4130, @WRITE_data_281, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b405c, @WRITE_data_282, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4130, @WRITE_data_283, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b400c, @WRITE_data_284, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4130, @WRITE_data_285, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b405c, @WRITE_data_286, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4130, @WRITE_data_287, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b405c, @WRITE_data_288, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4130, @WRITE_data_289, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b400c, @WRITE_data_290, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4130, @WRITE_data_291, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b405c, @WRITE_data_292, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4130, @WRITE_data_293, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b400c, @WRITE_data_294, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4130, @WRITE_data_295, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b405c, @WRITE_data_296, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4130, @WRITE_data_297, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b405c, @WRITE_data_298, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4130, @WRITE_data_299, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b400c, @WRITE_data_300, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4130, @WRITE_data_301, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b405c, @WRITE_data_302, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4130, @WRITE_data_303, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b400c, @WRITE_data_304, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4130, @WRITE_data_305, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x6ae000, @WRITE_data_306, 1, 0, 1
	 UC_DMA_BD	 0, 0x6ac000, @DMAWRITE_data_0, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x6ac020, @DMAWRITE_data_1, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x6ae020, @WRITE_data_307, 1, 0, 1
	 UC_DMA_BD	 0, 0x6ac040, @DMAWRITE_data_2, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x6ac060, @DMAWRITE_data_3, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x48ae000, @WRITE_data_308, 1, 0, 1
	 UC_DMA_BD	 0, 0x48ac000, @DMAWRITE_data_4, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x48ac020, @DMAWRITE_data_5, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x48ae020, @WRITE_data_309, 1, 0, 1
	 UC_DMA_BD	 0, 0x48ac040, @DMAWRITE_data_6, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x48ac060, @DMAWRITE_data_7, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x49ae000, @WRITE_data_310, 1, 0, 1
	 UC_DMA_BD	 0, 0x49ac000, @DMAWRITE_data_8, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x49ac020, @DMAWRITE_data_9, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x49ae020, @WRITE_data_311, 1, 0, 1
	 UC_DMA_BD	 0, 0x49ac040, @DMAWRITE_data_10, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x49ac060, @DMAWRITE_data_11, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x7ae000, @WRITE_data_312, 1, 0, 1
	 UC_DMA_BD	 0, 0x7ac000, @DMAWRITE_data_12, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x7ac020, @DMAWRITE_data_13, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x7ae020, @WRITE_data_313, 1, 0, 1
	 UC_DMA_BD	 0, 0x7ac040, @DMAWRITE_data_14, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x7ac060, @DMAWRITE_data_15, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x8ae000, @WRITE_data_314, 1, 0, 1
	 UC_DMA_BD	 0, 0x8ac000, @DMAWRITE_data_16, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x8ac020, @DMAWRITE_data_17, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x8ae020, @WRITE_data_315, 1, 0, 1
	 UC_DMA_BD	 0, 0x8ac040, @DMAWRITE_data_18, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x8ac060, @DMAWRITE_data_19, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x9ae000, @WRITE_data_316, 1, 0, 1
	 UC_DMA_BD	 0, 0x9ac000, @DMAWRITE_data_20, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x9ac020, @DMAWRITE_data_21, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x9ae020, @WRITE_data_317, 1, 0, 1
	 UC_DMA_BD	 0, 0x9ac040, @DMAWRITE_data_22, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x9ac060, @DMAWRITE_data_23, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x26ae000, @WRITE_data_318, 1, 0, 1
	 UC_DMA_BD	 0, 0x26ac000, @DMAWRITE_data_24, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x26ac020, @DMAWRITE_data_25, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x26ae020, @WRITE_data_319, 1, 0, 1
	 UC_DMA_BD	 0, 0x26ac040, @DMAWRITE_data_26, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x26ac060, @DMAWRITE_data_27, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x27ae000, @WRITE_data_320, 1, 0, 1
	 UC_DMA_BD	 0, 0x27ac000, @DMAWRITE_data_28, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x27ac020, @DMAWRITE_data_29, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x27ae020, @WRITE_data_321, 1, 0, 1
	 UC_DMA_BD	 0, 0x27ac040, @DMAWRITE_data_30, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x27ac060, @DMAWRITE_data_31, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x28ae000, @WRITE_data_322, 1, 0, 1
	 UC_DMA_BD	 0, 0x28ac000, @DMAWRITE_data_32, 0x1, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x28ac004, @DMAWRITE_data_33, 0x5, 0, 1
	 UC_DMA_BD	 0, 0x28ac020, @DMAWRITE_data_34, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x28ae020, @WRITE_data_323, 1, 0, 1
	 UC_DMA_BD	 0, 0x28ac040, @DMAWRITE_data_35, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x28ac060, @DMAWRITE_data_36, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x29ae000, @WRITE_data_324, 1, 0, 1
	 UC_DMA_BD	 0, 0x29ac000, @DMAWRITE_data_37, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x29ac020, @DMAWRITE_data_38, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x29ae020, @WRITE_data_325, 1, 0, 1
	 UC_DMA_BD	 0, 0x29ac040, @DMAWRITE_data_39, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x29ac060, @DMAWRITE_data_40, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x46ae000, @WRITE_data_326, 1, 0, 1
	 UC_DMA_BD	 0, 0x46ac000, @DMAWRITE_data_41, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x46ac020, @DMAWRITE_data_42, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x46ae020, @WRITE_data_327, 1, 0, 1
	 UC_DMA_BD	 0, 0x46ac040, @DMAWRITE_data_43, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x46ac060, @DMAWRITE_data_44, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x47ae000, @WRITE_data_328, 1, 0, 1
	 UC_DMA_BD	 0, 0x47ac000, @DMAWRITE_data_45, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x47ac020, @DMAWRITE_data_46, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x47ae020, @WRITE_data_329, 1, 0, 1
	 UC_DMA_BD	 0, 0x47ac040, @DMAWRITE_data_47, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x47ac060, @DMAWRITE_data_48, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x6ae040, @WRITE_data_330, 1, 0, 1
	 UC_DMA_BD	 0, 0x6ac080, @DMAWRITE_data_49, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x6ac0a0, @DMAWRITE_data_50, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x48ae040, @WRITE_data_331, 1, 0, 1
	 UC_DMA_BD	 0, 0x48ac080, @DMAWRITE_data_51, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x48ac0a0, @DMAWRITE_data_52, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x49ae040, @WRITE_data_332, 1, 0, 1
	 UC_DMA_BD	 0, 0x49ac080, @DMAWRITE_data_53, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x49ac0a0, @DMAWRITE_data_54, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x7ae040, @WRITE_data_333, 1, 0, 1
	 UC_DMA_BD	 0, 0x7ac080, @DMAWRITE_data_55, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x7ac0a0, @DMAWRITE_data_56, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x8ae040, @WRITE_data_334, 1, 0, 1
	 UC_DMA_BD	 0, 0x8ac080, @DMAWRITE_data_57, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x8ac0a0, @DMAWRITE_data_58, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x9ae040, @WRITE_data_335, 1, 0, 1
	 UC_DMA_BD	 0, 0x9ac080, @DMAWRITE_data_59, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x9ac0a0, @DMAWRITE_data_60, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x26ae040, @WRITE_data_336, 1, 0, 1
	 UC_DMA_BD	 0, 0x26ac080, @DMAWRITE_data_61, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x26ac0a0, @DMAWRITE_data_62, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x27ae040, @WRITE_data_337, 1, 0, 1
	 UC_DMA_BD	 0, 0x27ac080, @DMAWRITE_data_63, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x27ac0a0, @DMAWRITE_data_64, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x28ae040, @WRITE_data_338, 1, 0, 1
	 UC_DMA_BD	 0, 0x28ac080, @DMAWRITE_data_65, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x28ac0a0, @DMAWRITE_data_66, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x29ae040, @WRITE_data_339, 1, 0, 1
	 UC_DMA_BD	 0, 0x29ac080, @DMAWRITE_data_67, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x29ac0a0, @DMAWRITE_data_68, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x46ae040, @WRITE_data_340, 1, 0, 1
	 UC_DMA_BD	 0, 0x46ac080, @DMAWRITE_data_69, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x46ac0a0, @DMAWRITE_data_70, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x47ae040, @WRITE_data_341, 1, 0, 1
	 UC_DMA_BD	 0, 0x47ac080, @DMAWRITE_data_71, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x47ac0a0, @DMAWRITE_data_72, 0x6, 0, 0
.align    4
WRITE_data_0:
	.long 0x80000000
WRITE_data_1:
	.long 0x80000000
WRITE_data_2:
	.long 0x80000000
WRITE_data_3:
	.long 0x80000000
WRITE_data_4:
	.long 0x80000000
WRITE_data_5:
	.long 0x80000000
WRITE_data_6:
	.long 0x80000001
WRITE_data_7:
	.long 0x80000000
WRITE_data_8:
	.long 0x80000001
WRITE_data_9:
	.long 0x80000000
WRITE_data_10:
	.long 0x80000002
WRITE_data_11:
	.long 0x80000000
WRITE_data_12:
	.long 0x80000002
WRITE_data_13:
	.long 0x80000000
WRITE_data_14:
	.long 0x80000002
WRITE_data_15:
	.long 0x80000000
WRITE_data_16:
	.long 0x80000003
WRITE_data_17:
	.long 0x80000000
WRITE_data_18:
	.long 0x80000003
WRITE_data_19:
	.long 0x80000000
WRITE_data_20:
	.long 0x80000004
WRITE_data_21:
	.long 0x80000000
WRITE_data_22:
	.long 0x80000004
WRITE_data_23:
	.long 0x80000000
WRITE_data_24:
	.long 0x80000004
WRITE_data_25:
	.long 0x80000000
WRITE_data_26:
	.long 0x80000006
WRITE_data_27:
	.long 0x80000000
WRITE_data_28:
	.long 0x80000006
WRITE_data_29:
	.long 0x80000000
WRITE_data_30:
	.long 0x80000006
WRITE_data_31:
	.long 0x80000000
WRITE_data_32:
	.long 0x80000007
WRITE_data_33:
	.long 0x80000000
WRITE_data_34:
	.long 0x80000008
WRITE_data_35:
	.long 0x80000000
WRITE_data_36:
	.long 0x80000008
WRITE_data_37:
	.long 0x80000000
WRITE_data_38:
	.long 0x80000008
WRITE_data_39:
	.long 0x80000000
WRITE_data_40:
	.long 0x80000009
WRITE_data_41:
	.long 0x80000000
WRITE_data_42:
	.long 0x80000009
WRITE_data_43:
	.long 0x80000000
WRITE_data_44:
	.long 0x80000009
WRITE_data_45:
	.long 0x80000000
WRITE_data_46:
	.long 0x8000000a
WRITE_data_47:
	.long 0x80000000
WRITE_data_48:
	.long 0x80000016
WRITE_data_49:
	.long 0x80000000
WRITE_data_50:
	.long 0x80000016
WRITE_data_51:
	.long 0x80000000
WRITE_data_52:
	.long 0x80000016
WRITE_data_53:
	.long 0x80000000
WRITE_data_54:
	.long 0x80000017
WRITE_data_55:
	.long 0x80000000
WRITE_data_56:
	.long 0x80000017
WRITE_data_57:
	.long 0x80000000
WRITE_data_58:
	.long 0x80000017
WRITE_data_59:
	.long 0x80000000
WRITE_data_60:
	.long 0x80000019
WRITE_data_61:
	.long 0x80000000
WRITE_data_62:
	.long 0x80000019
WRITE_data_63:
	.long 0x80000000
WRITE_data_64:
	.long 0x80000019
WRITE_data_65:
	.long 0x80000000
WRITE_data_66:
	.long 0x8000001a
WRITE_data_67:
	.long 0x80000000
WRITE_data_68:
	.long 0x8000001a
WRITE_data_69:
	.long 0x80000000
WRITE_data_70:
	.long 0x8000001a
WRITE_data_71:
	.long 0x80000000
WRITE_data_72:
	.long 0x8000000e
WRITE_data_73:
	.long 0x80000000
WRITE_data_74:
	.long 0x8000000e
WRITE_data_75:
	.long 0x80000000
WRITE_data_76:
	.long 0x8000000e
WRITE_data_77:
	.long 0x80000000
WRITE_data_78:
	.long 0x8000000f
WRITE_data_79:
	.long 0x80000000
WRITE_data_80:
	.long 0x8000000f
WRITE_data_81:
	.long 0x80000000
WRITE_data_82:
	.long 0x8000000f
WRITE_data_83:
	.long 0x80000000
WRITE_data_84:
	.long 0x80000012
WRITE_data_85:
	.long 0x80000000
WRITE_data_86:
	.long 0x80000012
WRITE_data_87:
	.long 0x80000000
WRITE_data_88:
	.long 0x80000012
WRITE_data_89:
	.long 0x80000000
WRITE_data_90:
	.long 0x80000013
WRITE_data_91:
	.long 0x80000000
WRITE_data_92:
	.long 0x80000013
WRITE_data_93:
	.long 0x80000000
WRITE_data_94:
	.long 0x80000013
WRITE_data_95:
	.long 0x80000000
WRITE_data_96:
	.long 0x80000001
WRITE_data_97:
	.long 0x80000000
WRITE_data_98:
	.long 0x80000001
WRITE_data_99:
	.long 0x80000000
WRITE_data_100:
	.long 0x80000001
WRITE_data_101:
	.long 0x80000000
WRITE_data_102:
	.long 0x80000001
WRITE_data_103:
	.long 0x80000000
WRITE_data_104:
	.long 0x80000001
WRITE_data_105:
	.long 0x80000000
WRITE_data_106:
	.long 0x80000001
WRITE_data_107:
	.long 0x80000000
WRITE_data_108:
	.long 0x80000001
WRITE_data_109:
	.long 0x80000000
WRITE_data_110:
	.long 0x80000001
WRITE_data_111:
	.long 0x80000000
WRITE_data_112:
	.long 0x80000001
WRITE_data_113:
	.long 0x80000000
WRITE_data_114:
	.long 0x80000001
WRITE_data_115:
	.long 0x80000000
WRITE_data_116:
	.long 0x80000001
WRITE_data_117:
	.long 0x80000000
WRITE_data_118:
	.long 0x80000001
WRITE_data_119:
	.long 0x80000000
WRITE_data_120:
	.long 0x80000012
WRITE_data_121:
	.long 0x80000000
WRITE_data_122:
	.long 0x80000012
WRITE_data_123:
	.long 0x80000000
WRITE_data_124:
	.long 0x80000012
WRITE_data_125:
	.long 0x80000000
WRITE_data_126:
	.long 0x80000014
WRITE_data_127:
	.long 0x80000000
WRITE_data_128:
	.long 0x80000014
WRITE_data_129:
	.long 0x80000000
WRITE_data_130:
	.long 0x80000014
WRITE_data_131:
	.long 0x80000000
WRITE_data_132:
	.long 0x80000014
WRITE_data_133:
	.long 0x80000000
WRITE_data_134:
	.long 0x80000014
WRITE_data_135:
	.long 0x80000000
WRITE_data_136:
	.long 0x80000014
WRITE_data_137:
	.long 0x80000000
WRITE_data_138:
	.long 0x80000015
WRITE_data_139:
	.long 0x80000000
WRITE_data_140:
	.long 0x80000015
WRITE_data_141:
	.long 0x80000000
WRITE_data_142:
	.long 0x80000015
WRITE_data_143:
	.long 0x80000000
WRITE_data_144:
	.long 0x80000015
WRITE_data_145:
	.long 0x80000000
WRITE_data_146:
	.long 0x80000015
WRITE_data_147:
	.long 0x80000000
WRITE_data_148:
	.long 0x80000015
WRITE_data_149:
	.long 0x80000000
WRITE_data_150:
	.long 0x80000015
WRITE_data_151:
	.long 0x80000000
WRITE_data_152:
	.long 0x80000015
WRITE_data_153:
	.long 0x80000000
WRITE_data_154:
	.long 0x80000015
WRITE_data_155:
	.long 0x80000000
WRITE_data_156:
	.long 0x80000000
WRITE_data_157:
	.long 0x80000000
WRITE_data_158:
	.long 0x80000000
WRITE_data_159:
	.long 0x80000000
WRITE_data_160:
	.long 0x80000000
WRITE_data_161:
	.long 0x80000000
WRITE_data_162:
	.long 0x80000001
WRITE_data_163:
	.long 0x80000000
WRITE_data_164:
	.long 0x80000001
WRITE_data_165:
	.long 0x80000000
WRITE_data_166:
	.long 0x80000001
WRITE_data_167:
	.long 0x80000000
WRITE_data_168:
	.long 0x80000002
WRITE_data_169:
	.long 0x80000000
WRITE_data_170:
	.long 0x80000002
WRITE_data_171:
	.long 0x80000000
WRITE_data_172:
	.long 0x80000002
WRITE_data_173:
	.long 0x80000000
WRITE_data_174:
	.long 0x80000003
WRITE_data_175:
	.long 0x80000000
WRITE_data_176:
	.long 0x80000003
WRITE_data_177:
	.long 0x80000000
WRITE_data_178:
	.long 0x80000003
WRITE_data_179:
	.long 0x80000000
WRITE_data_180:
	.long 0x80000014
WRITE_data_181:
	.long 0x80000000
WRITE_data_182:
	.long 0x80000014
WRITE_data_183:
	.long 0x80000000
WRITE_data_184:
	.long 0x80000014
WRITE_data_185:
	.long 0x80000000
WRITE_data_186:
	.long 0x80000015
WRITE_data_187:
	.long 0x80000000
WRITE_data_188:
	.long 0x80000015
WRITE_data_189:
	.long 0x80000000
WRITE_data_190:
	.long 0x80000015
WRITE_data_191:
	.long 0x80000000
WRITE_data_192:
	.long 0x80000007
WRITE_data_193:
	.long 0x80000000
WRITE_data_194:
	.long 0x80000007
WRITE_data_195:
	.long 0x80000000
WRITE_data_196:
	.long 0x80000007
WRITE_data_197:
	.long 0x80000000
WRITE_data_198:
	.long 0x80000008
WRITE_data_199:
	.long 0x80000000
WRITE_data_200:
	.long 0x80000008
WRITE_data_201:
	.long 0x80000000
WRITE_data_202:
	.long 0x80000008
WRITE_data_203:
	.long 0x80000000
WRITE_data_204:
	.long 0x80000008
WRITE_data_205:
	.long 0x80000000
WRITE_data_206:
	.long 0x80000008
WRITE_data_207:
	.long 0x80000000
WRITE_data_208:
	.long 0x80000008
WRITE_data_209:
	.long 0x80000000
WRITE_data_210:
	.long 0x80000009
WRITE_data_211:
	.long 0x80000000
WRITE_data_212:
	.long 0x80000009
WRITE_data_213:
	.long 0x80000000
WRITE_data_214:
	.long 0x80000009
WRITE_data_215:
	.long 0x80000000
WRITE_data_216:
	.long 0x80000009
WRITE_data_217:
	.long 0x80000000
WRITE_data_218:
	.long 0x80000009
WRITE_data_219:
	.long 0x80000000
WRITE_data_220:
	.long 0x80000009
WRITE_data_221:
	.long 0x80000000
WRITE_data_222:
	.long 0x80000009
WRITE_data_223:
	.long 0x80000000
WRITE_data_224:
	.long 0x80000009
WRITE_data_225:
	.long 0x80000000
WRITE_data_226:
	.long 0x80000009
WRITE_data_227:
	.long 0x80000000
WRITE_data_228:
	.long 0x8000000a
WRITE_data_229:
	.long 0x80000000
WRITE_data_230:
	.long 0x8000000a
WRITE_data_231:
	.long 0x80000000
WRITE_data_232:
	.long 0x8000000a
WRITE_data_233:
	.long 0x80000000
WRITE_data_234:
	.long 0x8000000a
WRITE_data_235:
	.long 0x80000000
WRITE_data_236:
	.long 0x8000000a
WRITE_data_237:
	.long 0x80000000
WRITE_data_238:
	.long 0x8000000a
WRITE_data_239:
	.long 0x80000000
WRITE_data_240:
	.long 0x8000000a
WRITE_data_241:
	.long 0x80000000
WRITE_data_242:
	.long 0x8000000a
WRITE_data_243:
	.long 0x80000000
WRITE_data_244:
	.long 0x8000000a
WRITE_data_245:
	.long 0x80000000
WRITE_data_246:
	.long 0x8000000b
WRITE_data_247:
	.long 0x80000000
WRITE_data_248:
	.long 0x8000000b
WRITE_data_249:
	.long 0x80000000
WRITE_data_250:
	.long 0x8000000b
WRITE_data_251:
	.long 0x80000000
WRITE_data_252:
	.long 0x8000000b
WRITE_data_253:
	.long 0x80000000
WRITE_data_254:
	.long 0x8000000b
WRITE_data_255:
	.long 0x80000000
WRITE_data_256:
	.long 0x8000000b
WRITE_data_257:
	.long 0x80000000
WRITE_data_258:
	.long 0x8000000b
WRITE_data_259:
	.long 0x80000000
WRITE_data_260:
	.long 0x8000000b
WRITE_data_261:
	.long 0x80000000
WRITE_data_262:
	.long 0x8000000b
WRITE_data_263:
	.long 0x80000000
WRITE_data_264:
	.long 0x8000000b
WRITE_data_265:
	.long 0x80000000
WRITE_data_266:
	.long 0x8000000b
WRITE_data_267:
	.long 0x80000000
WRITE_data_268:
	.long 0x8000000b
WRITE_data_269:
	.long 0x80000000
WRITE_data_270:
	.long 0x8000000b
WRITE_data_271:
	.long 0x80000000
WRITE_data_272:
	.long 0x8000000b
WRITE_data_273:
	.long 0x80000000
WRITE_data_274:
	.long 0x8000000b
WRITE_data_275:
	.long 0x80000000
WRITE_data_276:
	.long 0x8000000c
WRITE_data_277:
	.long 0x80000000
WRITE_data_278:
	.long 0x8000000c
WRITE_data_279:
	.long 0x80000000
WRITE_data_280:
	.long 0x8000000c
WRITE_data_281:
	.long 0x80000000
WRITE_data_282:
	.long 0x8000000c
WRITE_data_283:
	.long 0x80000000
WRITE_data_284:
	.long 0x8000000c
WRITE_data_285:
	.long 0x80000000
WRITE_data_286:
	.long 0x8000000c
WRITE_data_287:
	.long 0x80000000
WRITE_data_288:
	.long 0x8000000c
WRITE_data_289:
	.long 0x80000000
WRITE_data_290:
	.long 0x8000000c
WRITE_data_291:
	.long 0x80000000
WRITE_data_292:
	.long 0x8000000c
WRITE_data_293:
	.long 0x80000000
WRITE_data_294:
	.long 0x8000000c
WRITE_data_295:
	.long 0x80000000
WRITE_data_296:
	.long 0x8000000c
WRITE_data_297:
	.long 0x80000000
WRITE_data_298:
	.long 0x8000000c
WRITE_data_299:
	.long 0x80000000
WRITE_data_300:
	.long 0x8000000c
WRITE_data_301:
	.long 0x80000000
WRITE_data_302:
	.long 0x8000000c
WRITE_data_303:
	.long 0x80000000
WRITE_data_304:
	.long 0x8000000c
WRITE_data_305:
	.long 0x80000000
WRITE_data_306:
	.long 0x00000002
DMAWRITE_data_0:
	.long 0x1fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
DMAWRITE_data_1:
	.long 0x3fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
WRITE_data_307:
	.long 0x00000002
DMAWRITE_data_2:
	.long 0x50000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
DMAWRITE_data_3:
	.long 0x60400004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
WRITE_data_308:
	.long 0x00000002
DMAWRITE_data_4:
	.long 0x1fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
DMAWRITE_data_5:
	.long 0x3fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
WRITE_data_309:
	.long 0x00000002
DMAWRITE_data_6:
	.long 0x50000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
DMAWRITE_data_7:
	.long 0x60400004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
WRITE_data_310:
	.long 0x00000002
DMAWRITE_data_8:
	.long 0x1fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
DMAWRITE_data_9:
	.long 0x3fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
WRITE_data_311:
	.long 0x00000002
DMAWRITE_data_10:
	.long 0x50000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
DMAWRITE_data_11:
	.long 0x60400004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
WRITE_data_312:
	.long 0x00000002
DMAWRITE_data_12:
	.long 0x1fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
DMAWRITE_data_13:
	.long 0x3fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
WRITE_data_313:
	.long 0x00000002
DMAWRITE_data_14:
	.long 0x50000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
DMAWRITE_data_15:
	.long 0x60400004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
WRITE_data_314:
	.long 0x00000002
DMAWRITE_data_16:
	.long 0x7fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
DMAWRITE_data_17:
	.long 0x5fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
WRITE_data_315:
	.long 0x00000002
DMAWRITE_data_18:
	.long 0x2fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
DMAWRITE_data_19:
	.long 0x0fc00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
WRITE_data_316:
	.long 0x00000002
DMAWRITE_data_20:
	.long 0x1fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
DMAWRITE_data_21:
	.long 0x3fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
WRITE_data_317:
	.long 0x00000002
DMAWRITE_data_22:
	.long 0x50000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
DMAWRITE_data_23:
	.long 0x60400004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
WRITE_data_318:
	.long 0x00000002
DMAWRITE_data_24:
	.long 0x1fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
DMAWRITE_data_25:
	.long 0x3fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
WRITE_data_319:
	.long 0x00000002
DMAWRITE_data_26:
	.long 0x50000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
DMAWRITE_data_27:
	.long 0x60400004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
WRITE_data_320:
	.long 0x00000002
DMAWRITE_data_28:
	.long 0x1fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
DMAWRITE_data_29:
	.long 0x3fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
WRITE_data_321:
	.long 0x00000002
DMAWRITE_data_30:
	.long 0x50000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
DMAWRITE_data_31:
	.long 0x60400004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
WRITE_data_322:
	.long 0x00000002
DMAWRITE_data_32:
	.long 0x1fe00004
DMAWRITE_data_33:
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
DMAWRITE_data_34:
	.long 0x3fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
WRITE_data_323:
	.long 0x00000002
DMAWRITE_data_35:
	.long 0x50000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
DMAWRITE_data_36:
	.long 0x60400004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
WRITE_data_324:
	.long 0x00000002
DMAWRITE_data_37:
	.long 0x1fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
DMAWRITE_data_38:
	.long 0x3fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
WRITE_data_325:
	.long 0x00000002
DMAWRITE_data_39:
	.long 0x50000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
DMAWRITE_data_40:
	.long 0x60400004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
WRITE_data_326:
	.long 0x00000002
DMAWRITE_data_41:
	.long 0x1fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
DMAWRITE_data_42:
	.long 0x3fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
WRITE_data_327:
	.long 0x00000002
DMAWRITE_data_43:
	.long 0x50000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
DMAWRITE_data_44:
	.long 0x60400004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
WRITE_data_328:
	.long 0x00000002
DMAWRITE_data_45:
	.long 0x1fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
DMAWRITE_data_46:
	.long 0x3fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
WRITE_data_329:
	.long 0x00000002
DMAWRITE_data_47:
	.long 0x50000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
DMAWRITE_data_48:
	.long 0x60400004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
WRITE_data_330:
	.long 0x00000002
DMAWRITE_data_49:
	.long 0x60000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
DMAWRITE_data_50:
	.long 0x50200004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
WRITE_data_331:
	.long 0x00000002
DMAWRITE_data_51:
	.long 0x60000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
DMAWRITE_data_52:
	.long 0x50200004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
WRITE_data_332:
	.long 0x00000002
DMAWRITE_data_53:
	.long 0x60000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
DMAWRITE_data_54:
	.long 0x50200004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
WRITE_data_333:
	.long 0x00000002
DMAWRITE_data_55:
	.long 0x60000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
DMAWRITE_data_56:
	.long 0x50200004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
WRITE_data_334:
	.long 0x00000002
DMAWRITE_data_57:
	.long 0x1fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
DMAWRITE_data_58:
	.long 0x3fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
WRITE_data_335:
	.long 0x00000002
DMAWRITE_data_59:
	.long 0x60000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
DMAWRITE_data_60:
	.long 0x50200004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
WRITE_data_336:
	.long 0x00000002
DMAWRITE_data_61:
	.long 0x60000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
DMAWRITE_data_62:
	.long 0x50200004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
WRITE_data_337:
	.long 0x00000002
DMAWRITE_data_63:
	.long 0x60000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
DMAWRITE_data_64:
	.long 0x50200004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
WRITE_data_338:
	.long 0x00000002
DMAWRITE_data_65:
	.long 0x60000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
DMAWRITE_data_66:
	.long 0x50200004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
WRITE_data_339:
	.long 0x00000002
DMAWRITE_data_67:
	.long 0x60000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
DMAWRITE_data_68:
	.long 0x50200004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
WRITE_data_340:
	.long 0x00000002
DMAWRITE_data_69:
	.long 0x60000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
DMAWRITE_data_70:
	.long 0x50200004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
WRITE_data_341:
	.long 0x00000002
DMAWRITE_data_71:
	.long 0x60000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
DMAWRITE_data_72:
	.long 0x50200004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
