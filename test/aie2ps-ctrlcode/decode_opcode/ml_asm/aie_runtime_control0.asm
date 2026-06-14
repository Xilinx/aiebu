.target	 aie2ps
.aie_row_topology	 1-2-4-0
.partition	 24column
;
;text
;
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
MASK_POLL_32	 0x1400fc04, 0x1, 0x1
MASK_POLL_32	 0x1400fe18, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
APPLY_OFFSET_57	 @DMAWRITE_data_8, 1, 1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
APPLY_OFFSET_57	 @DMAWRITE_data_9, 1, 1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
MASK_POLL_32	 0x1d0608, 0x1, 0x1
MASK_POLL_32	 0x1400f804, 0x1, 0x1
MASK_POLL_32	 0x1400fa18, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
MASK_POLL_32	 0x1400fdfc, 0x1, 0x1
MASK_POLL_32	 0x1400fe00, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_7
APPLY_OFFSET_57	 @DMAWRITE_data_26, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_8
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_9
APPLY_OFFSET_57	 @DMAWRITE_data_27, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_10
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_11
MASK_POLL_32	 0x9320, 0x78003c, 0x0
MASK_POLL_32	 0x9324, 0x78003c, 0x0
MASK_POLL_32	 0x1400f9fc, 0x1, 0x1
MASK_POLL_32	 0x1400fa00, 0x1, 0x1
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x30b500, @DMAWRITE_data_0, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x40b500, @DMAWRITE_data_1, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x50b500, @DMAWRITE_data_2, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x60b500, @DMAWRITE_data_3, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x230b500, @DMAWRITE_data_0, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x240b500, @DMAWRITE_data_1, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x250b500, @DMAWRITE_data_2, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x260b500, @DMAWRITE_data_3, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x430b500, @DMAWRITE_data_0, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x440b500, @DMAWRITE_data_1, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x450b500, @DMAWRITE_data_2, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x460b500, @DMAWRITE_data_3, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x630b500, @DMAWRITE_data_0, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x640b500, @DMAWRITE_data_1, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x650b500, @DMAWRITE_data_2, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x660b500, @DMAWRITE_data_3, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x31f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x41f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x51f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x61f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x231f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x241f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x251f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x261f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x431f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x441f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x451f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x461f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x631f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x641f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x651f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x661f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x61c0000, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x61c0010, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x61c0020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x61c0030, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c0000, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c0010, @WRITE_data_1, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x61a0000, @DMAWRITE_data_4, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x61a0604, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x61a0300, @DMAWRITE_data_5, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x61a060c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x61a0040, @DMAWRITE_data_6, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x61a0614, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x61a0340, @DMAWRITE_data_7, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x61a061c, @WRITE_data_4, 1, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x9000, @DMAWRITE_data_8, 0x9, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x9314, @WRITE_data_1, 1, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x9030, @DMAWRITE_data_9, 0x9, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x931c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a0000, @DMAWRITE_data_10, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a0604, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a0300, @DMAWRITE_data_11, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x1a060c, @WRITE_data_2, 1, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x1c0020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c0030, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c0040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c0050, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10, @WRITE_data_1, 1, 0, 0
UCBD_label_7:
	 UC_DMA_BD	 0, 0x21a06e0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21a06e4, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21a06e8, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21a06ec, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x61a06e0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x61a06e4, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x61a06e8, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x61a06ec, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21a0000, @DMAWRITE_data_12, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x21a0634, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x21a0300, @DMAWRITE_data_13, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x21a063c, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x21a0020, @DMAWRITE_data_14, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x21a0644, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x21a0320, @DMAWRITE_data_15, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x21a064c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x61a0080, @DMAWRITE_data_16, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x61a0634, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x61a0380, @DMAWRITE_data_17, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x61a063c, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x61a00c0, @DMAWRITE_data_18, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x61a0644, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x61a03c0, @DMAWRITE_data_19, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x61a064c, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x21a0040, @DMAWRITE_data_20, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x21a0604, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x21a0340, @DMAWRITE_data_21, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x21a060c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x21a0080, @DMAWRITE_data_22, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x21a0614, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x21a0380, @DMAWRITE_data_23, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x21a061c, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a06e0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a06e4, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a0000, @DMAWRITE_data_24, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x1a0634, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a0300, @DMAWRITE_data_25, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x1a063c, @WRITE_data_2, 1, 0, 0
UCBD_label_8:
	 UC_DMA_BD	 0, 0x9000, @DMAWRITE_data_26, 0x9, 0, 0
UCBD_label_9:
	 UC_DMA_BD	 0, 0x9304, @WRITE_data_1, 1, 0, 0
UCBD_label_10:
	 UC_DMA_BD	 0, 0x9030, @DMAWRITE_data_27, 0x9, 0, 0
UCBD_label_11:
	 UC_DMA_BD	 0, 0x930c, @WRITE_data_0, 1, 0, 0
.align    4
DMAWRITE_data_0:
	.long 0x00000001
	.long 0x00000002
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00076040
	.long 0x00000030
	.long 0x0007cb00
	.long 0x00000031
	.long 0x00070000
	.long 0x00000032
	.long 0x00074000
	.long 0x00000033
	.long 0x0007eb00
	.long 0x00000034
	.long 0x00078040
	.long 0x00000035
	.long 0x0000000d
	.long 0x00000020
	.long 0x00000080
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000008
	.long 0x00000000
	.long 0x00000020
	.long 0x00000080
	.long 0x00000000
	.long 0x00000000
	.long 0x00000012
	.long 0x0001f000
	.long 0x00000002
	.long 0x0001f010
	.long 0x00000000
	.long 0x0001d000
	.long 0x06040800
	.long 0x0001d004
	.long 0x00000000
	.long 0x0001d008
	.long 0x000fe000
	.long 0x0001d00c
	.long 0x02008003
	.long 0x0001d010
	.long 0x00003aaf
	.long 0x0001d014
	.long 0x02043fe0
	.long 0x0001de04
	.long 0x00070000
	.long 0x0001f020
	.long 0x00000002
	.long 0x0001f030
	.long 0x00000000
	.long 0x0001d020
	.long 0x00000810
	.long 0x0001d024
	.long 0x00000000
	.long 0x0001d028
	.long 0x00000000
	.long 0x0001d02c
	.long 0x00000000
	.long 0x0001d030
	.long 0x00002fff
	.long 0x0001d034
	.long 0x02047fe2
	.long 0x0001de0c
	.long 0x00070001
	.long 0x00000002
	.long 0x00000009
	.long 0x0001f040
	.long 0x00000002
	.long 0x0001f050
	.long 0x00000000
	.long 0x0001d040
	.long 0x08040200
	.long 0x0001d044
	.long 0x40000000
	.long 0x0001d048
	.long 0x000fe000
	.long 0x0001d04c
	.long 0x00808003
	.long 0x0001d050
	.long 0x00083aaf
	.long 0x0001d054
	.long 0x02049fe5
	.long 0x0001de14
	.long 0x00070002
	.long 0x00000008
	.long 0x00000000
	.long 0x00000001
DMAWRITE_data_1:
	.long 0x00000001
	.long 0x00000002
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00076040
	.long 0x00000030
	.long 0x0007cb00
	.long 0x00000031
	.long 0x00070000
	.long 0x00000032
	.long 0x00074000
	.long 0x00000033
	.long 0x0007eb00
	.long 0x00000034
	.long 0x00078040
	.long 0x00000035
	.long 0x0000000d
	.long 0x00000020
	.long 0x00000080
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000008
	.long 0x00000000
	.long 0x00000020
	.long 0x00000080
	.long 0x00000000
	.long 0x00000000
	.long 0x00000012
	.long 0x0001f000
	.long 0x00000002
	.long 0x0001f010
	.long 0x00000000
	.long 0x0001d000
	.long 0x06040800
	.long 0x0001d004
	.long 0x00000000
	.long 0x0001d008
	.long 0x000fe000
	.long 0x0001d00c
	.long 0x02008003
	.long 0x0001d010
	.long 0x00003aaf
	.long 0x0001d014
	.long 0x02043fe0
	.long 0x0001de04
	.long 0x00070000
	.long 0x0001f020
	.long 0x00000002
	.long 0x0001f030
	.long 0x00000000
	.long 0x0001d020
	.long 0x00000810
	.long 0x0001d024
	.long 0x00000000
	.long 0x0001d028
	.long 0x00000000
	.long 0x0001d02c
	.long 0x00000000
	.long 0x0001d030
	.long 0x00002fff
	.long 0x0001d034
	.long 0x02047fe2
	.long 0x0001de0c
	.long 0x00070001
	.long 0x00000002
	.long 0x00000009
	.long 0x0001f040
	.long 0x00000002
	.long 0x0001f050
	.long 0x00000000
	.long 0x0001d040
	.long 0x08040200
	.long 0x0001d044
	.long 0x40080000
	.long 0x0001d048
	.long 0x000fe000
	.long 0x0001d04c
	.long 0x00808003
	.long 0x0001d050
	.long 0x00083aaf
	.long 0x0001d054
	.long 0x02049fe5
	.long 0x0001de14
	.long 0x00070002
	.long 0x00000008
	.long 0x00000000
	.long 0x00000001
DMAWRITE_data_2:
	.long 0x00000001
	.long 0x00000002
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00076040
	.long 0x00000030
	.long 0x0007cb00
	.long 0x00000031
	.long 0x00070000
	.long 0x00000032
	.long 0x00074000
	.long 0x00000033
	.long 0x0007eb00
	.long 0x00000034
	.long 0x00078040
	.long 0x00000035
	.long 0x0000000d
	.long 0x00000020
	.long 0x00000080
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000008
	.long 0x00000000
	.long 0x00000020
	.long 0x00000080
	.long 0x00000000
	.long 0x00000000
	.long 0x00000012
	.long 0x0001f000
	.long 0x00000002
	.long 0x0001f010
	.long 0x00000000
	.long 0x0001d000
	.long 0x06040800
	.long 0x0001d004
	.long 0x00000000
	.long 0x0001d008
	.long 0x000fe000
	.long 0x0001d00c
	.long 0x02008003
	.long 0x0001d010
	.long 0x00003aaf
	.long 0x0001d014
	.long 0x02043fe0
	.long 0x0001de04
	.long 0x00070000
	.long 0x0001f020
	.long 0x00000002
	.long 0x0001f030
	.long 0x00000000
	.long 0x0001d020
	.long 0x00000810
	.long 0x0001d024
	.long 0x00000000
	.long 0x0001d028
	.long 0x00000000
	.long 0x0001d02c
	.long 0x00000000
	.long 0x0001d030
	.long 0x00002fff
	.long 0x0001d034
	.long 0x02047fe2
	.long 0x0001de0c
	.long 0x00070001
	.long 0x00000002
	.long 0x00000009
	.long 0x0001f040
	.long 0x00000002
	.long 0x0001f050
	.long 0x00000000
	.long 0x0001d040
	.long 0x08040200
	.long 0x0001d044
	.long 0x40100000
	.long 0x0001d048
	.long 0x000fe000
	.long 0x0001d04c
	.long 0x00808003
	.long 0x0001d050
	.long 0x00083aaf
	.long 0x0001d054
	.long 0x02049fe5
	.long 0x0001de14
	.long 0x00070002
	.long 0x00000008
	.long 0x00000000
	.long 0x00000001
DMAWRITE_data_3:
	.long 0x00000001
	.long 0x00000002
	.long 0x00000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00076040
	.long 0x00000030
	.long 0x0007cb00
	.long 0x00000031
	.long 0x00070000
	.long 0x00000032
	.long 0x00074000
	.long 0x00000033
	.long 0x0007eb00
	.long 0x00000034
	.long 0x00078040
	.long 0x00000035
	.long 0x0000000d
	.long 0x00000020
	.long 0x00000080
	.long 0x00000020
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000008
	.long 0x00000000
	.long 0x00000020
	.long 0x00000080
	.long 0x00000000
	.long 0x00000000
	.long 0x00000012
	.long 0x0001f000
	.long 0x00000002
	.long 0x0001f010
	.long 0x00000000
	.long 0x0001d000
	.long 0x06040800
	.long 0x0001d004
	.long 0x00000000
	.long 0x0001d008
	.long 0x000fe000
	.long 0x0001d00c
	.long 0x02008003
	.long 0x0001d010
	.long 0x00003aaf
	.long 0x0001d014
	.long 0x02043fe0
	.long 0x0001de04
	.long 0x00070000
	.long 0x0001f020
	.long 0x00000002
	.long 0x0001f030
	.long 0x00000000
	.long 0x0001d020
	.long 0x00000810
	.long 0x0001d024
	.long 0x00000000
	.long 0x0001d028
	.long 0x00000000
	.long 0x0001d02c
	.long 0x00000000
	.long 0x0001d030
	.long 0x00002fff
	.long 0x0001d034
	.long 0x02047fe2
	.long 0x0001de0c
	.long 0x00070001
	.long 0x00000002
	.long 0x00000009
	.long 0x0001f040
	.long 0x00000002
	.long 0x0001f050
	.long 0x00000000
	.long 0x0001d040
	.long 0x08040200
	.long 0x0001d044
	.long 0x40180000
	.long 0x0001d048
	.long 0x000fe000
	.long 0x0001d04c
	.long 0x00808003
	.long 0x0001d050
	.long 0x00083aaf
	.long 0x0001d054
	.long 0x02049fe5
	.long 0x0001de14
	.long 0x00070002
	.long 0x00000008
	.long 0x00000000
	.long 0x00000001
WRITE_data_0:
	.long 0x00000001
WRITE_data_1:
	.long 0x00000000
DMAWRITE_data_4:
	.long 0x00002040
	.long 0x001afe00
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81418040
	.long 0x00002040
	.long 0x00037f00
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81438042
DMAWRITE_data_5:
	.long 0x00002040
	.long 0x019b1e40
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81418040
	.long 0x00002040
	.long 0x00039f40
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81438042
WRITE_data_2:
	.long 0x00000018
DMAWRITE_data_6:
	.long 0x00002040
	.long 0x003b3e80
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81418040
	.long 0x00002040
	.long 0x0003bf80
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81438042
WRITE_data_3:
	.long 0x00000002
DMAWRITE_data_7:
	.long 0x00002040
	.long 0x01bb5ec0
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81418040
	.long 0x00002040
	.long 0x0003dfc0
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81438042
WRITE_data_4:
	.long 0x0000001a
DMAWRITE_data_8:
	.long 0x00000f80
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0xc0000000
	.long 0x00000000
	.long 0x00000000
	.long 0x02001001
	.long 0x00000000
DMAWRITE_data_9:
	.long 0x00000f80
	.long 0x00003e00
	.long 0x00000000
	.long 0x00000000
	.long 0xc0000000
	.long 0x00000000
	.long 0x00000000
	.long 0x02001001
	.long 0x00000000
DMAWRITE_data_10:
	.long 0x00000f80
	.long 0x00030000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81418040
DMAWRITE_data_11:
	.long 0x00000f80
	.long 0x00030f80
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81418040
DMAWRITE_data_12:
	.long 0x00000800
	.long 0x00010000
	.long 0x007c0000
	.long 0x0040003d
	.long 0x00000000
	.long 0x00040000
	.long 0x00000000
	.long 0x80008201
WRITE_data_5:
	.long 0x00070000
DMAWRITE_data_13:
	.long 0x00000800
	.long 0x000107c0
	.long 0x007c0000
	.long 0x0040003d
	.long 0x00000000
	.long 0x00040000
	.long 0x00000000
	.long 0x80008201
WRITE_data_6:
	.long 0x00070018
DMAWRITE_data_14:
	.long 0x00000800
	.long 0x00010f80
	.long 0x007c0000
	.long 0x0040003d
	.long 0x00000000
	.long 0x00040000
	.long 0x00000000
	.long 0x80008201
WRITE_data_7:
	.long 0x00070001
DMAWRITE_data_15:
	.long 0x00000800
	.long 0x00011740
	.long 0x007c0000
	.long 0x0040003d
	.long 0x00000000
	.long 0x00040000
	.long 0x00000000
	.long 0x80008201
WRITE_data_8:
	.long 0x00070019
DMAWRITE_data_16:
	.long 0x00002040
	.long 0x005afe00
	.long 0x05600000
	.long 0x000602af
	.long 0x0008203f
	.long 0x00000000
	.long 0x00000000
	.long 0x81408441
	.long 0x00002040
	.long 0x00037f00
	.long 0x05600000
	.long 0x000602af
	.long 0x0008203f
	.long 0x00000000
	.long 0x00000000
	.long 0x81428443
WRITE_data_9:
	.long 0x00000004
DMAWRITE_data_17:
	.long 0x00002040
	.long 0x01db0610
	.long 0x05600000
	.long 0x000602af
	.long 0x0008203f
	.long 0x00000000
	.long 0x00000000
	.long 0x81408441
	.long 0x00002040
	.long 0x00038710
	.long 0x05600000
	.long 0x000602af
	.long 0x0008203f
	.long 0x00000000
	.long 0x00000000
	.long 0x81428443
WRITE_data_10:
	.long 0x0000001c
DMAWRITE_data_18:
	.long 0x00002040
	.long 0x007b0e20
	.long 0x05600000
	.long 0x000602af
	.long 0x0008203f
	.long 0x00000000
	.long 0x00000000
	.long 0x81408441
	.long 0x00002040
	.long 0x00038f20
	.long 0x05600000
	.long 0x000602af
	.long 0x0008203f
	.long 0x00000000
	.long 0x00000000
	.long 0x81428443
WRITE_data_11:
	.long 0x00000006
DMAWRITE_data_19:
	.long 0x00002040
	.long 0x01fb1630
	.long 0x05600000
	.long 0x000602af
	.long 0x0008203f
	.long 0x00000000
	.long 0x00000000
	.long 0x81408441
	.long 0x00002040
	.long 0x00039730
	.long 0x05600000
	.long 0x000602af
	.long 0x0008203f
	.long 0x00000000
	.long 0x00000000
	.long 0x81428443
WRITE_data_12:
	.long 0x0000001e
DMAWRITE_data_20:
	.long 0x00002000
	.long 0x00380000
	.long 0x00200000
	.long 0x004000ff
	.long 0x0020000f
	.long 0x00000000
	.long 0x00000000
	.long 0x81038002
	.long 0x00002000
	.long 0x00008000
	.long 0x00200000
	.long 0x004000ff
	.long 0x0020000f
	.long 0x00000000
	.long 0x00000000
	.long 0x81058004
DMAWRITE_data_21:
	.long 0x00002000
	.long 0x01b82000
	.long 0x00200000
	.long 0x004000ff
	.long 0x0020000f
	.long 0x00000000
	.long 0x00000000
	.long 0x81038002
	.long 0x00002000
	.long 0x0000a000
	.long 0x00200000
	.long 0x004000ff
	.long 0x0020000f
	.long 0x00000000
	.long 0x00000000
	.long 0x81058004
DMAWRITE_data_22:
	.long 0x00002000
	.long 0x00584000
	.long 0x00200000
	.long 0x004000ff
	.long 0x0020000f
	.long 0x00000000
	.long 0x00000000
	.long 0x81038002
	.long 0x00002000
	.long 0x0000c000
	.long 0x00200000
	.long 0x004000ff
	.long 0x0020000f
	.long 0x00000000
	.long 0x00000000
	.long 0x81058004
DMAWRITE_data_23:
	.long 0x00002000
	.long 0x01d86000
	.long 0x00200000
	.long 0x004000ff
	.long 0x0020000f
	.long 0x00000000
	.long 0x00000000
	.long 0x81038002
	.long 0x00002000
	.long 0x0000e000
	.long 0x00200000
	.long 0x004000ff
	.long 0x0020000f
	.long 0x00000000
	.long 0x00000000
	.long 0x81058004
DMAWRITE_data_24:
	.long 0x00004000
	.long 0x001a0000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81428443
	.long 0x00004000
	.long 0x00028000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81448445
DMAWRITE_data_25:
	.long 0x00004000
	.long 0x019a4000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81428443
	.long 0x00004000
	.long 0x0002c000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81448445
DMAWRITE_data_26:
	.long 0x00008000
	.long 0x00000000
	.long 0x00000000
	.long 0x10000000
	.long 0xc40001ff
	.long 0x000000ff
	.long 0x00000000
	.long 0x02003000
	.long 0x00000000
DMAWRITE_data_27:
	.long 0x00008000
	.long 0x00020000
	.long 0x00000000
	.long 0x10000000
	.long 0xc40001ff
	.long 0x000000ff
	.long 0x00000000
	.long 0x02003000
	.long 0x00000000
