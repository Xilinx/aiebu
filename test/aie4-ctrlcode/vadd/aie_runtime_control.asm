.partition	 3column
.target aie4
;
;text
;
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
APPLY_OFFSET_57	 @DMAWRITE_data_1, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
APPLY_OFFSET_57	 @DMAWRITE_data_3, 1, 1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
MASK_POLL_32	 0x118604, 0x1, 0x1
MASK_POLL_32	 0x4118604, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
MASK_POLL_32	 0x118e04, 0x1, 0x1
END_JOB

.eop

START_JOB 1
PREEMPT 0, @save, @restore, @hint_bitmap
END_JOB

.eop

START_JOB 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
MASK_POLL_32	 0x2118604, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_7
APPLY_OFFSET_57	 @DMAWRITE_data_13, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_8
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_9
MASK_POLL_32	 0x14604, 0x1, 0x1
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x618000, @DMAWRITE_data_0, 0x67, 0, 1
	 UC_DMA_BD	 0, 0x6ae0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x10000, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10010, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x110000, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x110010, @WRITE_data_4, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_1, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x18554, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x108000, @DMAWRITE_data_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e04, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x10020, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x10030, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110000, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110010, @WRITE_data_10, 1, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x18030, @DMAWRITE_data_3, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x1855c, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x4108000, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e04, @WRITE_data_12, 1, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x110020, @WRITE_data_13, 1, 0, 1
	 UC_DMA_BD	 0, 0x110030, @WRITE_data_14, 1, 0, 1
	 UC_DMA_BD	 0, 0x108f00, @DMAWRITE_data_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e6c, @WRITE_data_15, 1, 0, 1
	 UC_DMA_BD	 0, 0x4108c00, @DMAWRITE_data_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_16, 1, 0, 1
	 UC_DMA_BD	 0, 0x108300, @DMAWRITE_data_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e0c, @WRITE_data_17, 1, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x2618000, @DMAWRITE_data_8, 0x67, 0, 1
	 UC_DMA_BD	 0, 0x26ae0e0, @WRITE_data_18, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110000, @WRITE_data_19, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110010, @WRITE_data_20, 1, 0, 1
	 UC_DMA_BD	 0, 0x110000, @WRITE_data_21, 1, 0, 1
	 UC_DMA_BD	 0, 0x110010, @WRITE_data_22, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110000, @WRITE_data_23, 1, 0, 1
	 UC_DMA_BD	 0, 0x2110010, @WRITE_data_24, 1, 0, 1
	 UC_DMA_BD	 0, 0x4108f00, @DMAWRITE_data_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109e6c, @WRITE_data_25, 1, 0, 1
	 UC_DMA_BD	 0, 0x108c00, @DMAWRITE_data_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x109e64, @WRITE_data_26, 1, 0, 1
	 UC_DMA_BD	 0, 0x2108000, @DMAWRITE_data_11, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109e04, @WRITE_data_27, 1, 0, 0
UCBD_label_7:
	 UC_DMA_BD	 0, 0x10000, @WRITE_data_28, 1, 0, 1
	 UC_DMA_BD	 0, 0x10010, @WRITE_data_29, 1, 0, 1
	 UC_DMA_BD	 0, 0x2108f00, @DMAWRITE_data_12, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x2109e6c, @WRITE_data_30, 1, 0, 0
UCBD_label_8:
	 UC_DMA_BD	 0, 0x18000, @DMAWRITE_data_13, 0x9, 0, 0
UCBD_label_9:
	 UC_DMA_BD	 0, 0x18544, @WRITE_data_31, 1, 0, 0
.align    4
hint_bitmap:
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0x00000001
    .long 0x00000000
DMAWRITE_data_0:
	.long 0x66660001
	.long 0x00000000
	.long 0x00000001
	.long 0x000e0000
	.long 0x00000010
	.long 0x00000000
	.long 0x00000010
	.long 0x000e4000
	.long 0x00000032
	.long 0x000e6000
	.long 0x00000033
	.long 0x00000010
	.long 0x00000000
	.long 0x00000010
	.long 0x000e8000
	.long 0x00000010
	.long 0x00000000
	.long 0x00000010
	.long 0x00000012
	.long 0x000ae000
	.long 0x00000002
	.long 0x000ae010
	.long 0x00000000
	.long 0x000ac000
	.long 0x00000010
	.long 0x000ac004
	.long 0x00000000
	.long 0x000ac008
	.long 0x00010001
	.long 0x000ac00c
	.long 0x00000001
	.long 0x000ac010
	.long 0x00020800
	.long 0x000ac014
	.long 0x02043fe0
	.long 0x000ace04
	.long 0x00010000
	.long 0x000ae020
	.long 0x00000002
	.long 0x000ae030
	.long 0x00000000
	.long 0x000ac020
	.long 0x10000010
	.long 0x000ac024
	.long 0x00000000
	.long 0x000ac028
	.long 0x00010001
	.long 0x000ac02c
	.long 0x00000001
	.long 0x000ac030
	.long 0x00020800
	.long 0x000ac034
	.long 0x02047fe2
	.long 0x000ace0c
	.long 0x00000001
	.long 0x00000002
	.long 0x00000009
	.long 0x000ae040
	.long 0x00000002
	.long 0x000ae050
	.long 0x00000000
	.long 0x000ac040
	.long 0x20000010
	.long 0x000ac044
	.long 0x00000000
	.long 0x000ac048
	.long 0x00010001
	.long 0x000ac04c
	.long 0x00000001
	.long 0x000ac050
	.long 0x00020800
	.long 0x000ac054
	.long 0x02049fe5
	.long 0x000ace14
	.long 0x00010002
	.long 0x00000002
	.long 0x000e0000
	.long 0x00000030
	.long 0x000e2000
	.long 0x00000031
	.long 0x00000000
	.long 0x00000000
	.long 0x000e8000
	.long 0x00000034
	.long 0x000ea000
	.long 0x00000035
	.long 0x00000000
	.long 0x00000000
	.long 0x000e0000
	.long 0x00000030
	.long 0x000e2000
	.long 0x00000031
	.long 0x00000010
	.long 0x00000000
	.long 0x000e8000
	.long 0x00000034
	.long 0x000ea000
	.long 0x00000035
	.long 0x00000010
	.long 0x00000000
	.long 0x00000018
	.long 0x00000062
	.long 0x66660003
WRITE_data_0:
	.long 0x00000001
WRITE_data_1:
	.long 0x00000000
WRITE_data_2:
	.long 0x00000000
WRITE_data_3:
	.long 0x00000000
WRITE_data_4:
	.long 0x00000000
DMAWRITE_data_1:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000020
	.long 0x0f000000
	.long 0x01000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
WRITE_data_5:
	.long 0x00000000
DMAWRITE_data_2:
	.long 0x00800000
	.long 0x00000020
	.long 0x1c000b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
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
DMAWRITE_data_3:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000010
	.long 0x1f000000
	.long 0x00800100
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
WRITE_data_11:
	.long 0x00000001
DMAWRITE_data_4:
	.long 0x00800000
	.long 0x00000010
	.long 0x1c000b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000200
	.long 0x00000000
WRITE_data_12:
	.long 0x00000000
WRITE_data_13:
	.long 0x00000000
WRITE_data_14:
	.long 0x00000000
DMAWRITE_data_5:
	.long 0x00800000
	.long 0x02000020
	.long 0x1c100381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
WRITE_data_15:
	.long 0x00000000
DMAWRITE_data_6:
	.long 0x00800000
	.long 0x02000010
	.long 0x1c100381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000200
	.long 0x00000000
WRITE_data_16:
	.long 0x00000000
DMAWRITE_data_7:
	.long 0x00800400
	.long 0x00000020
	.long 0x1c200b87
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
WRITE_data_17:
	.long 0x00000000
DMAWRITE_data_8:
	.long 0x66660001
	.long 0x00000000
	.long 0x00000001
	.long 0x000e0000
	.long 0x00000010
	.long 0x00000000
	.long 0x00000010
	.long 0x000e4000
	.long 0x00000032
	.long 0x000e6000
	.long 0x00000033
	.long 0x00000010
	.long 0x00000000
	.long 0x00000010
	.long 0x000e8000
	.long 0x00000010
	.long 0x00000000
	.long 0x00000010
	.long 0x00000012
	.long 0x000ae000
	.long 0x00000002
	.long 0x000ae010
	.long 0x00000000
	.long 0x000ac000
	.long 0x00000010
	.long 0x000ac004
	.long 0x00000000
	.long 0x000ac008
	.long 0x00010001
	.long 0x000ac00c
	.long 0x00000001
	.long 0x000ac010
	.long 0x00020800
	.long 0x000ac014
	.long 0x02043fe0
	.long 0x000ace04
	.long 0x00010000
	.long 0x000ae020
	.long 0x00000002
	.long 0x000ae030
	.long 0x00000000
	.long 0x000ac020
	.long 0x10000010
	.long 0x000ac024
	.long 0x00000000
	.long 0x000ac028
	.long 0x00010001
	.long 0x000ac02c
	.long 0x00000001
	.long 0x000ac030
	.long 0x00020800
	.long 0x000ac034
	.long 0x02047fe2
	.long 0x000ace0c
	.long 0x00000001
	.long 0x00000002
	.long 0x00000009
	.long 0x000ae040
	.long 0x00000002
	.long 0x000ae050
	.long 0x00000000
	.long 0x000ac040
	.long 0x20000010
	.long 0x000ac044
	.long 0x00000000
	.long 0x000ac048
	.long 0x00010001
	.long 0x000ac04c
	.long 0x00000001
	.long 0x000ac050
	.long 0x00020800
	.long 0x000ac054
	.long 0x02049fe5
	.long 0x000ace14
	.long 0x00010002
	.long 0x00000002
	.long 0x000e0000
	.long 0x00000030
	.long 0x000e2000
	.long 0x00000031
	.long 0x00000000
	.long 0x00000000
	.long 0x000e8000
	.long 0x00000034
	.long 0x000ea000
	.long 0x00000035
	.long 0x00000000
	.long 0x00000000
	.long 0x000e0000
	.long 0x00000030
	.long 0x000e2000
	.long 0x00000031
	.long 0x00000010
	.long 0x00000000
	.long 0x000e8000
	.long 0x00000034
	.long 0x000ea000
	.long 0x00000035
	.long 0x00000010
	.long 0x00000000
	.long 0x00000018
	.long 0x00000062
	.long 0x66660003
WRITE_data_18:
	.long 0x00000001
WRITE_data_19:
	.long 0x00000000
WRITE_data_20:
	.long 0x00000000
WRITE_data_21:
	.long 0x00000000
WRITE_data_22:
	.long 0x00000000
WRITE_data_23:
	.long 0x00000000
WRITE_data_24:
	.long 0x00000000
DMAWRITE_data_9:
	.long 0x00800000
	.long 0x00000010
	.long 0x1c100381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000200
	.long 0x00000000
WRITE_data_25:
	.long 0x00000000
DMAWRITE_data_10:
	.long 0x00800400
	.long 0x00000020
	.long 0x1c100381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
WRITE_data_26:
	.long 0x00000000
DMAWRITE_data_11:
	.long 0x00800000
	.long 0x00000020
	.long 0x1c000b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
WRITE_data_27:
	.long 0x00000000
WRITE_data_28:
	.long 0x00000000
WRITE_data_29:
	.long 0x00000000
DMAWRITE_data_12:
	.long 0x00800000
	.long 0x02000020
	.long 0x1c100381
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000400
	.long 0x00000000
WRITE_data_30:
	.long 0x00000000
DMAWRITE_data_13:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000020
	.long 0x07000000
	.long 0x01001080
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000400
WRITE_data_31:
	.long 0x00000000
