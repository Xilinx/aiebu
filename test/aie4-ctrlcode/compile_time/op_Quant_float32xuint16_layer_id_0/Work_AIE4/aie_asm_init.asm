.target	 aie4
.aie_row_topology	 1-1-4-0
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
	 UC_DMA_BD	 0, 0x6b405c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4100, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b405c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4100, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b405c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4100, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x120028, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x120040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120028, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120028, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x160028, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x120044, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160028, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120044, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x12002c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x120048, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x212002c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120048, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x412002c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120048, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x16002c, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x12004c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x416002c, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x412004c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x160014, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x120050, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160014, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120050, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160014, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120050, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x160020, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x160040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160020, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160020, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160028, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160044, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x160024, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x160048, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160024, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160048, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160024, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160048, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x160018, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x16004c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x216002c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x216004c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160018, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x416004c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160018, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160050, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x120000, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x12006c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120000, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x212006c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120000, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x412006c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x120004, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x120070, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120004, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120070, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120004, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120070, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x160000, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x16006c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160000, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x216006c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160000, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x416006c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x160004, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x160070, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160004, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160070, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160004, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160070, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x120030, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x120078, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120030, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120078, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120030, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120078, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x120008, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x12005c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120008, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x212005c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120008, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x412005c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x12000c, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x120060, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x212000c, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120060, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x412000c, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120060, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x160008, @WRITE_data_17, 1, 0, 1
	 UC_DMA_BD	 0, 0x16005c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160008, @WRITE_data_17, 1, 0, 1
	 UC_DMA_BD	 0, 0x216005c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160008, @WRITE_data_17, 1, 0, 1
	 UC_DMA_BD	 0, 0x416005c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x16000c, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x160060, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x216000c, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160060, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x416000c, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160060, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4018, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4104, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b401c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4104, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4024, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4104, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4028, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4104, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4018, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4104, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b401c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4104, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4024, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4104, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4028, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4104, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4018, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4104, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b401c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4104, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4024, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4104, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4028, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4104, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b401c, @WRITE_data_17, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4148, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b401c, @WRITE_data_17, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4148, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b401c, @WRITE_data_17, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4148, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4024, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4150, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4024, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4150, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4024, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4150, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4024, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4150, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4024, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4150, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4024, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4150, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4028, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4154, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4028, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4154, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4028, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4154, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4028, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4154, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4028, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4154, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4028, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4154, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4028, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4154, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4028, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4154, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4028, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4154, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b43b0, @WRITE_data_21, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4078, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b43b4, @WRITE_data_23, 0x3, 0, 1
	 UC_DMA_BD	 0, 0x6b4070, @WRITE_data_24, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b43b0, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4078, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b43b4, @WRITE_data_26, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x7b4070, @WRITE_data_24, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b43b0, @WRITE_data_27, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4078, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b43b4, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4070, @WRITE_data_29, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4078, @WRITE_data_30, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b416c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b43b0, @WRITE_data_21, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4078, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b43b4, @WRITE_data_31, 0x3, 0, 1
	 UC_DMA_BD	 0, 0x26b4070, @WRITE_data_24, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b43b0, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4078, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b43b4, @WRITE_data_32, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x27b4070, @WRITE_data_24, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b43b0, @WRITE_data_27, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4078, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b43b4, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4070, @WRITE_data_29, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4078, @WRITE_data_30, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b416c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b43b0, @WRITE_data_21, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4078, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b43b4, @WRITE_data_33, 0x3, 0, 1
	 UC_DMA_BD	 0, 0x46b4070, @WRITE_data_24, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b43b0, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4078, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b43b4, @WRITE_data_34, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x47b4070, @WRITE_data_24, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b43b0, @WRITE_data_27, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4078, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b43b4, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4070, @WRITE_data_29, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4078, @WRITE_data_30, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b416c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x8034, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x8074, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008034, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008074, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008034, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008074, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x8020, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x8040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008020, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008020, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x8024, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8044, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008024, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008044, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008024, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008044, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x48020, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x48040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048020, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048020, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048040, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x48024, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x48044, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048024, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048044, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048024, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048044, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x8000, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x48064, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008000, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048064, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008000, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048064, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x48000, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x48068, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048000, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048068, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048000, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048068, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4008, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b411c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4008, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b411c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4008, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b411c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4008, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b411c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4008, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b411c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4008, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b411c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4048, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4120, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b404c, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4120, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4008, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4120, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4048, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4120, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b404c, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4120, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4008, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4120, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4048, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4120, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b404c, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4120, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4008, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4120, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b404c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4124, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4050, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4124, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4054, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4124, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b404c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4124, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4050, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4124, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4054, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4124, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b404c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4124, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4050, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4124, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4054, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4124, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4050, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4128, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4054, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4128, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b400c, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4128, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4008, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4128, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4050, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4128, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4054, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4128, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b400c, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4128, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4008, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4128, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4050, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4128, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4054, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4128, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b400c, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4128, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4008, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4128, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4054, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b400c, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4058, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b400c, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4058, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b400c, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4054, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b400c, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4058, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b400c, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4058, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b400c, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4054, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b400c, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4058, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b400c, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4058, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b400c, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b412c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4058, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4000, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b405c, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4000, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b405c, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4000, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4058, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4000, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b405c, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4000, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b405c, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4000, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4058, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4000, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b405c, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4000, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b405c, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4000, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4130, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b416c, @WRITE_data_37, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b416c, @WRITE_data_37, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b416c, @WRITE_data_37, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b416c, @WRITE_data_37, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b416c, @WRITE_data_37, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b416c, @WRITE_data_37, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b416c, @WRITE_data_37, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b416c, @WRITE_data_37, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b416c, @WRITE_data_37, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x6ae000, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x6ac000, @DMAWRITE_data_0, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x6ac020, @DMAWRITE_data_1, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x6ae020, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x6ac040, @DMAWRITE_data_2, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x6ac060, @DMAWRITE_data_3, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x27ae000, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x27ac000, @DMAWRITE_data_0, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x27ac020, @DMAWRITE_data_1, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x27ae020, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x27ac040, @DMAWRITE_data_2, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x27ac060, @DMAWRITE_data_3, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x28ae000, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x28ac000, @DMAWRITE_data_0, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x28ac020, @DMAWRITE_data_1, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x28ae020, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x28ac040, @DMAWRITE_data_2, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x28ac060, @DMAWRITE_data_3, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x29ae000, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x29ac000, @DMAWRITE_data_0, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x29ac020, @DMAWRITE_data_1, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x29ae020, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x29ac040, @DMAWRITE_data_2, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x29ac060, @DMAWRITE_data_3, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x46ae000, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x46ac000, @DMAWRITE_data_0, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x46ac020, @DMAWRITE_data_1, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x46ae020, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x46ac040, @DMAWRITE_data_2, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x46ac060, @DMAWRITE_data_3, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x47ae000, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x47ac000, @DMAWRITE_data_0, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x47ac020, @DMAWRITE_data_1, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x47ae020, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x47ac040, @DMAWRITE_data_2, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x47ac060, @DMAWRITE_data_3, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x48ae000, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x48ac000, @DMAWRITE_data_0, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x48ac020, @DMAWRITE_data_1, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x48ae020, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x48ac040, @DMAWRITE_data_2, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x48ac060, @DMAWRITE_data_3, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x49ae000, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x49ac000, @DMAWRITE_data_0, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x49ac020, @DMAWRITE_data_1, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x49ae020, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x49ac040, @DMAWRITE_data_2, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x49ac060, @DMAWRITE_data_3, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x7ae000, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x7ac000, @DMAWRITE_data_0, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x7ac020, @DMAWRITE_data_1, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x7ae020, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x7ac040, @DMAWRITE_data_2, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x7ac060, @DMAWRITE_data_3, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x8ae000, @WRITE_data_38, 1, 0, 1
	 UC_DMA_BD	 0, 0x8ac000, @DMAWRITE_data_4, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x8ac020, @DMAWRITE_data_5, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x8ae020, @WRITE_data_38, 1, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x8ac040, @DMAWRITE_data_6, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x8ac060, @DMAWRITE_data_7, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x9ae000, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x9ac000, @DMAWRITE_data_8, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x9ac020, @DMAWRITE_data_9, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x9ae020, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x9ac040, @DMAWRITE_data_10, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x9ac060, @DMAWRITE_data_11, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x26ae000, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x26ac000, @DMAWRITE_data_8, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x26ac020, @DMAWRITE_data_9, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x26ae020, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x26ac040, @DMAWRITE_data_10, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x26ac060, @DMAWRITE_data_11, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x6ae040, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x6ac080, @DMAWRITE_data_12, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x6ac0a0, @DMAWRITE_data_13, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x27ae040, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x27ac080, @DMAWRITE_data_12, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x27ac0a0, @DMAWRITE_data_13, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x28ae040, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x28ac080, @DMAWRITE_data_12, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x28ac0a0, @DMAWRITE_data_13, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x29ae040, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x29ac080, @DMAWRITE_data_12, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x29ac0a0, @DMAWRITE_data_13, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x46ae040, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x46ac080, @DMAWRITE_data_12, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x46ac0a0, @DMAWRITE_data_13, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x47ae040, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x47ac080, @DMAWRITE_data_12, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x47ac0a0, @DMAWRITE_data_13, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x48ae040, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x48ac080, @DMAWRITE_data_12, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x48ac0a0, @DMAWRITE_data_13, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x49ae040, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x49ac080, @DMAWRITE_data_12, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x49ac0a0, @DMAWRITE_data_13, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x7ae040, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x7ac080, @DMAWRITE_data_12, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x7ac0a0, @DMAWRITE_data_13, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x8ae040, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x8ac080, @DMAWRITE_data_14, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x8ac0a0, @DMAWRITE_data_15, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x9ae040, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x9ac080, @DMAWRITE_data_12, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x9ac0a0, @DMAWRITE_data_13, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x26ae040, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x26ac080, @DMAWRITE_data_12, 0x6, 0, 1
	 UC_DMA_BD	 0, 0x26ac0a0, @DMAWRITE_data_13, 0x6, 0, 0
.align    4
WRITE_data_0:
	.long 0x80000000
WRITE_data_1:
	.long 0x80000001
WRITE_data_2:
	.long 0x80000002
WRITE_data_3:
	.long 0x80000003
WRITE_data_4:
	.long 0x80000004
WRITE_data_5:
	.long 0x80000006
WRITE_data_6:
	.long 0x80000007
WRITE_data_7:
	.long 0x80000008
WRITE_data_8:
	.long 0x80000009
WRITE_data_9:
	.long 0x8000000a
WRITE_data_10:
	.long 0x80000016
WRITE_data_11:
	.long 0x80000017
WRITE_data_12:
	.long 0x80000019
WRITE_data_13:
	.long 0x8000001a
WRITE_data_14:
	.long 0x8000001c
WRITE_data_15:
	.long 0x8000000e
WRITE_data_16:
	.long 0x8000000f
WRITE_data_17:
	.long 0x80000012
WRITE_data_18:
	.long 0x80000013
WRITE_data_19:
	.long 0x80000014
WRITE_data_20:
	.long 0x80000015
WRITE_data_21:
	.long 0x001f0100
WRITE_data_22:
	.long 0xc0000088
WRITE_data_23:
	.long 0x011f0110
	.long 0x021f0110
	.long 0x031f0110
WRITE_data_24:
	.long 0xc0000010
WRITE_data_25:
	.long 0x011f0100
WRITE_data_26:
	.long 0x021f0110
	.long 0x031f0110
WRITE_data_27:
	.long 0x021f0100
WRITE_data_28:
	.long 0x031f0110
WRITE_data_29:
	.long 0xc0000090
WRITE_data_30:
	.long 0x8000001b
WRITE_data_31:
	.long 0x011f0110
	.long 0x021f0110
	.long 0x031f0110
WRITE_data_32:
	.long 0x021f0110
	.long 0x031f0110
WRITE_data_33:
	.long 0x011f0110
	.long 0x021f0110
	.long 0x031f0110
WRITE_data_34:
	.long 0x021f0110
	.long 0x031f0110
WRITE_data_35:
	.long 0x8000000b
WRITE_data_36:
	.long 0x8000000c
WRITE_data_37:
	.long 0xc0000000
WRITE_data_38:
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
DMAWRITE_data_4:
	.long 0x7fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
DMAWRITE_data_5:
	.long 0x5fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00043fe0
DMAWRITE_data_6:
	.long 0x2fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
DMAWRITE_data_7:
	.long 0x0fc00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00047fe2
WRITE_data_39:
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
DMAWRITE_data_12:
	.long 0x60000004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
DMAWRITE_data_13:
	.long 0x50200004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
DMAWRITE_data_14:
	.long 0x1fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
DMAWRITE_data_15:
	.long 0x3fe00004
	.long 0x00000000
	.long 0x00010000
	.long 0x00000001
	.long 0x00000001
	.long 0x00049fe5
