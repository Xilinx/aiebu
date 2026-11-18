.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
END_JOB

.eop

START_JOB 1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
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
MASK_WRITE_32	 0x6a7120, 0xffff, 0x0
MASK_WRITE_32	 0x7a7120, 0xffff, 0x1
MASK_WRITE_32	 0x8a7120, 0xffff, 0x2
MASK_WRITE_32	 0x9a7120, 0xffff, 0x3
MASK_WRITE_32	 0x26a7120, 0xffff, 0x100
MASK_WRITE_32	 0x27a7120, 0xffff, 0x101
MASK_WRITE_32	 0x28a7120, 0xffff, 0x102
MASK_WRITE_32	 0x29a7120, 0xffff, 0x103
MASK_WRITE_32	 0x46a7120, 0xffff, 0x200
MASK_WRITE_32	 0x47a7120, 0xffff, 0x201
MASK_WRITE_32	 0x48a7120, 0xffff, 0x202
MASK_WRITE_32	 0x49a7120, 0xffff, 0x203
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
	 UC_DMA_BD	 0, 0x8004, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x48078, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008004, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048078, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008004, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048078, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x160220, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1601a0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160220, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x21601a0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160220, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x41601a0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x1201f0, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x21201f0, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x41201f0, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4b70, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4904, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4b70, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4904, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4b70, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4904, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4b70, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4904, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4b70, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4904, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4b70, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4904, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4b70, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4904, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4b70, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4904, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4b70, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4904, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x48210, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x481a0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048210, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x20481a0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048210, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x40481a0, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x81e0, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x20081e0, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x40081e0, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4b10, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4b10, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4b10, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4b10, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4904, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4b10, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4b10, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4b10, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4b10, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4904, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4b10, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4b10, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4b10, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4b10, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4904, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4b20, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4b20, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4b20, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4b20, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4b20, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4b20, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4b20, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4b20, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4b20, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4b20, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4b20, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4b20, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x1601d0, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x21601d0, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x41601d0, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x1201c4, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x21201c4, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x41201c4, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4a1c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4a1c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4a1c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4a1c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4a1c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4a1c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4a1c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4a1c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4a1c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x481c0, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x20481c0, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x40481c0, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x81b4, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x20081b4, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x40081b4, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4a04, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4a04, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4a04, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4a04, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4a04, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4a04, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4a04, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4a04, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4a04, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4a04, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4a04, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4a04, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4a08, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4a08, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4a08, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4a08, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4a08, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4a08, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4a08, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4a08, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4a08, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4a08, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4a08, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4a08, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x40a0, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x40a0, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x416002c, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x412004c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b405c, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4130, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b405c, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4130, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b42c0, @WRITE_data_13, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x47b4004, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b405c, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b400c, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4130, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b42c0, @WRITE_data_17, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x49b4004, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b400c, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4130, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160028, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120044, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4058, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b412c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b42b0, @WRITE_data_20, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x46b4004, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4058, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b400c, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b412c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b42b0, @WRITE_data_21, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x48b4004, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b400c, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b412c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160024, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160048, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4054, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4128, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4054, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4128, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4058, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4128, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b42b0, @WRITE_data_24, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x49b4000, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4008, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b412c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160020, @WRITE_data_27, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4050, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4124, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4050, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4124, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4290, @WRITE_data_29, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x48b4000, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4008, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4124, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x412002c, @WRITE_data_30, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120048, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b404c, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4120, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4280, @WRITE_data_31, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x47b4000, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4008, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4120, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120028, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4270, @WRITE_data_32, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x46b4000, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4008, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b411c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x216002c, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x216004c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b405c, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4130, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b405c, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4130, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b42c0, @WRITE_data_33, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x27b4004, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b405c, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b400c, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4130, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b42c0, @WRITE_data_34, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x29b4004, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b400c, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4130, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160004, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160070, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4028, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4104, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4028, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4154, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4028, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4154, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4028, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4154, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x160004, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x160070, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4028, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4104, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4028, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4154, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4028, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4154, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4028, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4154, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x16002c, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x12004c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b405c, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4130, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b405c, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4130, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b42c0, @WRITE_data_37, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x7b4004, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b405c, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b400c, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4130, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b42c0, @WRITE_data_38, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x9b4004, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b400c, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4130, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160000, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x416006c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4024, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4104, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4024, @WRITE_data_40, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4150, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4024, @WRITE_data_40, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4150, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x160028, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x120044, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4058, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b412c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b42b0, @WRITE_data_41, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x6b4004, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4058, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b400c, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b412c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b42b0, @WRITE_data_42, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x8b4004, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b400c, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b412c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160018, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160050, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048000, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048068, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120004, @WRITE_data_43, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120070, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b401c, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4104, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b401c, @WRITE_data_44, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4148, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x120004, @WRITE_data_43, 1, 0, 1
	 UC_DMA_BD	 0, 0x120070, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b401c, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4104, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b401c, @WRITE_data_44, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4148, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120000, @WRITE_data_45, 1, 0, 1
	 UC_DMA_BD	 0, 0x412006c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4018, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4104, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x160020, @WRITE_data_27, 1, 0, 1
	 UC_DMA_BD	 0, 0x160040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4050, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4124, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4050, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4124, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4290, @WRITE_data_46, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x8b4000, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4008, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4124, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160004, @WRITE_data_35, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160070, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4028, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4104, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4028, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4154, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4028, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4154, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4028, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4154, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160024, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160048, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4054, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4128, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4054, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4128, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4058, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4128, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b42b0, @WRITE_data_47, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x29b4000, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4008, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b412c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160000, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x216006c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4024, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4104, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4024, @WRITE_data_40, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4150, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4024, @WRITE_data_40, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4150, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160028, @WRITE_data_48, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160044, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4058, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b412c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b42b0, @WRITE_data_49, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x26b4004, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4058, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b400c, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b412c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b42b0, @WRITE_data_50, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x28b4004, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b400c, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b412c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160020, @WRITE_data_27, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4050, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4124, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4050, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4124, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4290, @WRITE_data_51, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x28b4000, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4008, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4124, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x160024, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x160048, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4054, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4128, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4054, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4128, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4058, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4128, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b42b0, @WRITE_data_52, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x9b4000, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4008, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b412c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120004, @WRITE_data_43, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120070, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b401c, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4104, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b401c, @WRITE_data_44, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4148, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120028, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4270, @WRITE_data_53, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x26b4000, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4008, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b411c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x120008, @WRITE_data_54, 1, 0, 1
	 UC_DMA_BD	 0, 0x12005c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120008, @WRITE_data_54, 1, 0, 1
	 UC_DMA_BD	 0, 0x212005c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120000, @WRITE_data_45, 1, 0, 1
	 UC_DMA_BD	 0, 0x212006c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4018, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4104, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x160018, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x16004c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x48000, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x48068, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160018, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x416004c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048000, @WRITE_data_36, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048068, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x12000c, @WRITE_data_55, 1, 0, 1
	 UC_DMA_BD	 0, 0x120060, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8024, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x8044, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x216000c, @WRITE_data_56, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160060, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048024, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048044, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x160000, @WRITE_data_39, 1, 0, 1
	 UC_DMA_BD	 0, 0x16006c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4024, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4104, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4024, @WRITE_data_40, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4150, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4024, @WRITE_data_40, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4150, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x12002c, @WRITE_data_30, 1, 0, 1
	 UC_DMA_BD	 0, 0x120048, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b404c, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4120, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4280, @WRITE_data_57, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x7b4000, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4008, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4120, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x160008, @WRITE_data_44, 1, 0, 1
	 UC_DMA_BD	 0, 0x16005c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x48020, @WRITE_data_30, 1, 0, 1
	 UC_DMA_BD	 0, 0x48040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x212000c, @WRITE_data_55, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120060, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008024, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008044, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x416000c, @WRITE_data_56, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160060, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048024, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048044, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x412000c, @WRITE_data_55, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120060, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008024, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008044, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120008, @WRITE_data_54, 1, 0, 1
	 UC_DMA_BD	 0, 0x412005c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160008, @WRITE_data_44, 1, 0, 1
	 UC_DMA_BD	 0, 0x416005c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048020, @WRITE_data_30, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x212002c, @WRITE_data_30, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120048, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b404c, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4120, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4280, @WRITE_data_58, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x27b4000, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4008, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4120, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160008, @WRITE_data_44, 1, 0, 1
	 UC_DMA_BD	 0, 0x216005c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048020, @WRITE_data_30, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x120028, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x120040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4270, @WRITE_data_59, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x6b4000, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4008, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b411c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x4160014, @WRITE_data_60, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120050, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008000, @WRITE_data_40, 1, 0, 1
	 UC_DMA_BD	 0, 0x4048064, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x16000c, @WRITE_data_56, 1, 0, 1
	 UC_DMA_BD	 0, 0x160060, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x48024, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x48044, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2160014, @WRITE_data_60, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120050, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008000, @WRITE_data_40, 1, 0, 1
	 UC_DMA_BD	 0, 0x2048064, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x160014, @WRITE_data_60, 1, 0, 1
	 UC_DMA_BD	 0, 0x120050, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8000, @WRITE_data_40, 1, 0, 1
	 UC_DMA_BD	 0, 0x48064, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x120000, @WRITE_data_45, 1, 0, 1
	 UC_DMA_BD	 0, 0x12006c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4018, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4104, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b4078, @WRITE_data_61, 1, 0, 1
	 UC_DMA_BD	 0, 0x9b416c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8150, @WRITE_data_62, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x8038, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x8034, @WRITE_data_63, 1, 0, 1
	 UC_DMA_BD	 0, 0x8074, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x120160, @WRITE_data_64, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x120034, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x120030, @WRITE_data_63, 1, 0, 1
	 UC_DMA_BD	 0, 0x120078, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b43b0, @WRITE_data_65, 0x3, 0, 1
	 UC_DMA_BD	 0, 0x6b4078, @WRITE_data_66, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b4070, @WRITE_data_67, 1, 0, 1
	 UC_DMA_BD	 0, 0x6b416c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b43b0, @WRITE_data_68, 0x3, 0, 1
	 UC_DMA_BD	 0, 0x7b4078, @WRITE_data_66, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b4070, @WRITE_data_67, 1, 0, 1
	 UC_DMA_BD	 0, 0x7b416c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b43b0, @WRITE_data_69, 0x3, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x8b4078, @WRITE_data_70, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b4070, @WRITE_data_71, 1, 0, 1
	 UC_DMA_BD	 0, 0x8b416c, @WRITE_data_72, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b4078, @WRITE_data_73, 1, 0, 1
	 UC_DMA_BD	 0, 0x29b416c, @WRITE_data_74, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008150, @WRITE_data_75, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x2008038, @WRITE_data_76, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008034, @WRITE_data_77, 1, 0, 1
	 UC_DMA_BD	 0, 0x2008074, @WRITE_data_72, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120160, @WRITE_data_78, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x2120034, @WRITE_data_76, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120030, @WRITE_data_77, 1, 0, 1
	 UC_DMA_BD	 0, 0x2120078, @WRITE_data_72, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b43b0, @WRITE_data_79, 0x3, 0, 1
	 UC_DMA_BD	 0, 0x26b4078, @WRITE_data_70, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b4070, @WRITE_data_80, 1, 0, 1
	 UC_DMA_BD	 0, 0x26b416c, @WRITE_data_72, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b43b0, @WRITE_data_81, 0x3, 0, 1
	 UC_DMA_BD	 0, 0x27b4078, @WRITE_data_70, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b4070, @WRITE_data_80, 1, 0, 1
	 UC_DMA_BD	 0, 0x27b416c, @WRITE_data_72, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b43b0, @WRITE_data_82, 0x3, 0, 1
	 UC_DMA_BD	 0, 0x28b4078, @WRITE_data_70, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b4070, @WRITE_data_71, 1, 0, 1
	 UC_DMA_BD	 0, 0x28b416c, @WRITE_data_72, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b4078, @WRITE_data_73, 1, 0, 1
	 UC_DMA_BD	 0, 0x49b416c, @WRITE_data_74, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008150, @WRITE_data_83, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x4008038, @WRITE_data_76, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008034, @WRITE_data_77, 1, 0, 1
	 UC_DMA_BD	 0, 0x4008074, @WRITE_data_72, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120160, @WRITE_data_84, 0x2, 0, 1
	 UC_DMA_BD	 0, 0x4120034, @WRITE_data_76, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120030, @WRITE_data_77, 1, 0, 1
	 UC_DMA_BD	 0, 0x4120078, @WRITE_data_72, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b43b0, @WRITE_data_85, 0x3, 0, 1
	 UC_DMA_BD	 0, 0x46b4078, @WRITE_data_70, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b4070, @WRITE_data_80, 1, 0, 1
	 UC_DMA_BD	 0, 0x46b416c, @WRITE_data_72, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b43b0, @WRITE_data_86, 0x3, 0, 1
	 UC_DMA_BD	 0, 0x47b4078, @WRITE_data_70, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b4070, @WRITE_data_80, 1, 0, 1
	 UC_DMA_BD	 0, 0x47b416c, @WRITE_data_72, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b43b0, @WRITE_data_87, 0x3, 0, 1
	 UC_DMA_BD	 0, 0x48b4078, @WRITE_data_70, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b4070, @WRITE_data_71, 1, 0, 1
	 UC_DMA_BD	 0, 0x48b416c, @WRITE_data_72, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x8000001d
WRITE_data_1:
	.long 0x80000000
WRITE_data_2:
	.long 0x00000100
WRITE_data_3:
	.long 0xc0000008
WRITE_data_4:
	.long 0x001f0100
WRITE_data_5:
	.long 0x00000102
WRITE_data_6:
	.long 0xc000000a
WRITE_data_7:
	.long 0x001f0102
WRITE_data_8:
	.long 0xc0000000
WRITE_data_9:
	.long 0x000000f8
WRITE_data_10:
	.long 0x00000011
WRITE_data_11:
	.long 0x80000003
WRITE_data_12:
	.long 0x8000000c
WRITE_data_13:
	.long 0x011f0111
	.long 0x001f0101
WRITE_data_14:
	.long 0xc0000091
WRITE_data_15:
	.long 0xc0000019
WRITE_data_16:
	.long 0xc0000089
WRITE_data_17:
	.long 0x011f0111
	.long 0x001f0101
WRITE_data_18:
	.long 0x80000001
WRITE_data_19:
	.long 0x8000000b
WRITE_data_20:
	.long 0x011f0111
	.long 0x001f0101
WRITE_data_21:
	.long 0x011f0111
	.long 0x001f0101
WRITE_data_22:
	.long 0x80000008
WRITE_data_23:
	.long 0x8000000a
WRITE_data_24:
	.long 0x011f0110
	.long 0x001f0100
WRITE_data_25:
	.long 0xc0000090
WRITE_data_26:
	.long 0xc0000088
WRITE_data_27:
	.long 0x80000006
WRITE_data_28:
	.long 0x80000009
WRITE_data_29:
	.long 0x011f0110
	.long 0x001f0100
WRITE_data_30:
	.long 0x80000002
WRITE_data_31:
	.long 0x011f0110
	.long 0x001f0100
WRITE_data_32:
	.long 0x011f0110
	.long 0x001f0100
WRITE_data_33:
	.long 0x011f0111
	.long 0x001f0101
WRITE_data_34:
	.long 0x011f0111
	.long 0x001f0101
WRITE_data_35:
	.long 0x8000001a
WRITE_data_36:
	.long 0x80000015
WRITE_data_37:
	.long 0x011f0111
	.long 0x001f0101
WRITE_data_38:
	.long 0x011f0111
	.long 0x001f0101
WRITE_data_39:
	.long 0x80000019
WRITE_data_40:
	.long 0x80000014
WRITE_data_41:
	.long 0x011f0111
	.long 0x001f0101
WRITE_data_42:
	.long 0x011f0111
	.long 0x001f0101
WRITE_data_43:
	.long 0x80000017
WRITE_data_44:
	.long 0x80000012
WRITE_data_45:
	.long 0x80000016
WRITE_data_46:
	.long 0x011f0110
	.long 0x001f0100
WRITE_data_47:
	.long 0x011f0110
	.long 0x001f0100
WRITE_data_48:
	.long 0x80000007
WRITE_data_49:
	.long 0x011f0111
	.long 0x001f0101
WRITE_data_50:
	.long 0x011f0111
	.long 0x001f0101
WRITE_data_51:
	.long 0x011f0110
	.long 0x001f0100
WRITE_data_52:
	.long 0x011f0110
	.long 0x001f0100
WRITE_data_53:
	.long 0x011f0110
	.long 0x001f0100
WRITE_data_54:
	.long 0x8000000e
WRITE_data_55:
	.long 0x8000000f
WRITE_data_56:
	.long 0x80000013
WRITE_data_57:
	.long 0x011f0110
	.long 0x001f0100
WRITE_data_58:
	.long 0x011f0110
	.long 0x001f0100
WRITE_data_59:
	.long 0x011f0110
	.long 0x001f0100
WRITE_data_60:
	.long 0x80000004
WRITE_data_61:
	.long 0x8000001b
WRITE_data_62:
	.long 0x10100100
	.long 0x00100110
WRITE_data_63:
	.long 0xc0000010
WRITE_data_64:
	.long 0x08080100
	.long 0x00080110
WRITE_data_65:
	.long 0x01070102
	.long 0x02020112
	.long 0x00070122
WRITE_data_66:
	.long 0xc00000aa
WRITE_data_67:
	.long 0xc0000032
WRITE_data_68:
	.long 0x02070102
	.long 0x04040112
	.long 0x00070122
WRITE_data_69:
	.long 0x06070102
	.long 0x07070112
	.long 0x00070122
WRITE_data_70:
	.long 0xc00000aa
WRITE_data_71:
	.long 0xc00000b2
WRITE_data_72:
	.long 0xc0000000
WRITE_data_73:
	.long 0x8000001b
WRITE_data_74:
	.long 0x80000000
WRITE_data_75:
	.long 0x10100100
	.long 0x00100110
WRITE_data_76:
	.long 0xc0000088
WRITE_data_77:
	.long 0xc0000010
WRITE_data_78:
	.long 0x08080100
	.long 0x00080110
WRITE_data_79:
	.long 0x01070102
	.long 0x02020112
	.long 0x00070122
WRITE_data_80:
	.long 0xc0000032
WRITE_data_81:
	.long 0x02070102
	.long 0x04040112
	.long 0x00070122
WRITE_data_82:
	.long 0x06070102
	.long 0x07070112
	.long 0x00070122
WRITE_data_83:
	.long 0x10100100
	.long 0x00100110
WRITE_data_84:
	.long 0x08080100
	.long 0x00080110
WRITE_data_85:
	.long 0x01070102
	.long 0x02020112
	.long 0x00070122
WRITE_data_86:
	.long 0x02070102
	.long 0x04040112
	.long 0x00070122
WRITE_data_87:
	.long 0x06070102
	.long 0x07070112
	.long 0x00070122
