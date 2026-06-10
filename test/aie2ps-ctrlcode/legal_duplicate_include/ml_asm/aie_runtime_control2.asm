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
APPLY_OFFSET_57	 @DMAWRITE_data_4, 1, 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
APPLY_OFFSET_57	 @DMAWRITE_data_9, 1, 1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_3
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_4
APPLY_OFFSET_57	 @DMAWRITE_data_10, 1, 1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_5
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_6
MASK_POLL_32	 0x101d0608, 0x1, 0x1
MASK_POLL_32	 0x1400f804, 0x1, 0x1
MASK_POLL_32	 0x1400fa18, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_7
MASK_POLL_32	 0x1400fdfc, 0x1, 0x1
MASK_POLL_32	 0x1400fe00, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_8
APPLY_OFFSET_57	 @DMAWRITE_data_27, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_9
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_10
APPLY_OFFSET_57	 @DMAWRITE_data_28, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_11
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_12
MASK_POLL_32	 0x10009320, 0x78003c, 0x0
MASK_POLL_32	 0x10009324, 0x78003c, 0x0
MASK_POLL_32	 0x1400f9fc, 0x1, 0x1
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x1030b500, @DMAWRITE_data_0, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x1040b500, @DMAWRITE_data_1, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x1050b500, @DMAWRITE_data_2, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x1060b500, @DMAWRITE_data_3, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x1230b500, @DMAWRITE_data_0, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x1240b500, @DMAWRITE_data_1, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x1250b500, @DMAWRITE_data_2, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x1260b500, @DMAWRITE_data_3, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x1430b500, @DMAWRITE_data_0, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x1440b500, @DMAWRITE_data_1, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x1450b500, @DMAWRITE_data_2, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x1460b500, @DMAWRITE_data_3, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x1630b500, @DMAWRITE_data_0, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x1640b500, @DMAWRITE_data_1, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x1650b500, @DMAWRITE_data_2, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x1660b500, @DMAWRITE_data_3, 0x5d, 0, 1
	 UC_DMA_BD	 0, 0x1031f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1041f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1051f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1061f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1231f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1241f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1251f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1261f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1431f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1441f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1451f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1461f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1631f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1641f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1651f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1661f0e0, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x16000000, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x16000010, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x161c0000, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x161c0010, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x161c0020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x161c0030, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10000000, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10000010, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x101c0000, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x101c0010, @WRITE_data_1, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x16009000, @DMAWRITE_data_4, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x1600931c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x161a0000, @DMAWRITE_data_5, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x161a0604, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x161a0300, @DMAWRITE_data_6, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x161a060c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x161a0040, @DMAWRITE_data_7, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x161a0614, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x161a0340, @DMAWRITE_data_8, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x161a061c, @WRITE_data_4, 1, 0, 0
UCBD_label_3:
	 UC_DMA_BD	 0, 0x10009000, @DMAWRITE_data_9, 0x9, 0, 0
UCBD_label_4:
	 UC_DMA_BD	 0, 0x10009314, @WRITE_data_1, 1, 0, 0
UCBD_label_5:
	 UC_DMA_BD	 0, 0x10009030, @DMAWRITE_data_10, 0x9, 0, 0
UCBD_label_6:
	 UC_DMA_BD	 0, 0x1000931c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x101a0000, @DMAWRITE_data_11, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x101a0604, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x101a0300, @DMAWRITE_data_12, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x101a060c, @WRITE_data_2, 1, 0, 0
UCBD_label_7:
	 UC_DMA_BD	 0, 0x101c0020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x101c0030, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x101c0040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x101c0050, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10000000, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10000010, @WRITE_data_1, 1, 0, 0
UCBD_label_8:
	 UC_DMA_BD	 0, 0x121a06e0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x121a06e4, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x121a06e8, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x121a06ec, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x161a06e0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x161a06e4, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x161a06e8, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x161a06ec, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x161a0080, @DMAWRITE_data_13, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x161a0634, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x161a0380, @DMAWRITE_data_14, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x161a063c, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x161a00c0, @DMAWRITE_data_15, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x161a0644, @WRITE_data_7, 1, 0, 1
	 UC_DMA_BD	 0, 0x161a03c0, @DMAWRITE_data_16, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x161a064c, @WRITE_data_8, 1, 0, 1
	 UC_DMA_BD	 0, 0x121a0000, @DMAWRITE_data_17, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x121a0634, @WRITE_data_9, 1, 0, 1
	 UC_DMA_BD	 0, 0x121a0300, @DMAWRITE_data_18, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x121a063c, @WRITE_data_10, 1, 0, 1
	 UC_DMA_BD	 0, 0x121a0020, @DMAWRITE_data_19, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x121a0644, @WRITE_data_11, 1, 0, 1
	 UC_DMA_BD	 0, 0x121a0320, @DMAWRITE_data_20, 0x8, 0, 1
	 UC_DMA_BD	 0, 0x121a064c, @WRITE_data_12, 1, 0, 1
	 UC_DMA_BD	 0, 0x121a0040, @DMAWRITE_data_21, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x121a0604, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x121a0340, @DMAWRITE_data_22, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x121a060c, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x121a0080, @DMAWRITE_data_23, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x121a0614, @WRITE_data_5, 1, 0, 1
	 UC_DMA_BD	 0, 0x121a0380, @DMAWRITE_data_24, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x121a061c, @WRITE_data_6, 1, 0, 1
	 UC_DMA_BD	 0, 0x101a06e0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x101a06e4, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x101a0000, @DMAWRITE_data_25, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x101a0634, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x101a0300, @DMAWRITE_data_26, 0x10, 0, 1
	 UC_DMA_BD	 0, 0x101a063c, @WRITE_data_2, 1, 0, 0
UCBD_label_9:
	 UC_DMA_BD	 0, 0x10009000, @DMAWRITE_data_27, 0x9, 0, 0
UCBD_label_10:
	 UC_DMA_BD	 0, 0x10009304, @WRITE_data_1, 1, 0, 0
UCBD_label_11:
	 UC_DMA_BD	 0, 0x10009030, @DMAWRITE_data_28, 0x9, 0, 0
UCBD_label_12:
	 UC_DMA_BD	 0, 0x1000930c, @WRITE_data_0, 1, 0, 0
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
	.long 0x00004080
	.long 0x00008100
	.long 0x00000000
	.long 0x2b000000
	.long 0xc0c002af
	.long 0x000080ff
	.long 0x00000000
	.long 0x02001001
	.long 0x00000000
DMAWRITE_data_5:
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
DMAWRITE_data_6:
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
DMAWRITE_data_7:
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
DMAWRITE_data_8:
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
DMAWRITE_data_9:
	.long 0x00000f80
	.long 0x0000f800
	.long 0x00000000
	.long 0x00000000
	.long 0xc0000000
	.long 0x00000000
	.long 0x00000000
	.long 0x02001001
	.long 0x00000000
DMAWRITE_data_10:
	.long 0x00000f80
	.long 0x00013600
	.long 0x00000000
	.long 0x00000000
	.long 0xc0000000
	.long 0x00000000
	.long 0x00000000
	.long 0x02001001
	.long 0x00000000
DMAWRITE_data_11:
	.long 0x00000f80
	.long 0x00030000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81418040
DMAWRITE_data_12:
	.long 0x00000f80
	.long 0x00030f80
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x81418040
DMAWRITE_data_13:
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
WRITE_data_5:
	.long 0x00000004
DMAWRITE_data_14:
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
WRITE_data_6:
	.long 0x0000001c
DMAWRITE_data_15:
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
WRITE_data_7:
	.long 0x00000006
DMAWRITE_data_16:
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
WRITE_data_8:
	.long 0x0000001e
DMAWRITE_data_17:
	.long 0x00000800
	.long 0x00010000
	.long 0x007c0000
	.long 0x0040003d
	.long 0x00000000
	.long 0x00040000
	.long 0x00000000
	.long 0x80008201
WRITE_data_9:
	.long 0x00070000
DMAWRITE_data_18:
	.long 0x00000800
	.long 0x000107c0
	.long 0x007c0000
	.long 0x0040003d
	.long 0x00000000
	.long 0x00040000
	.long 0x00000000
	.long 0x80008201
WRITE_data_10:
	.long 0x00070018
DMAWRITE_data_19:
	.long 0x00000800
	.long 0x00010f80
	.long 0x007c0000
	.long 0x0040003d
	.long 0x00000000
	.long 0x00040000
	.long 0x00000000
	.long 0x80008201
WRITE_data_11:
	.long 0x00070001
DMAWRITE_data_20:
	.long 0x00000800
	.long 0x00011740
	.long 0x007c0000
	.long 0x0040003d
	.long 0x00000000
	.long 0x00040000
	.long 0x00000000
	.long 0x80008201
WRITE_data_12:
	.long 0x00070019
DMAWRITE_data_21:
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
DMAWRITE_data_22:
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
DMAWRITE_data_23:
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
DMAWRITE_data_24:
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
DMAWRITE_data_25:
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
DMAWRITE_data_26:
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
DMAWRITE_data_27:
	.long 0x00008000
	.long 0x00080000
	.long 0x00000000
	.long 0x10000000
	.long 0xc40001ff
	.long 0x000000ff
	.long 0x00000000
	.long 0x02003000
	.long 0x00000000
DMAWRITE_data_28:
	.long 0x00008000
	.long 0x000a0000
	.long 0x00000000
	.long 0x10000000
	.long 0xc40001ff
	.long 0x000000ff
	.long 0x00000000
	.long 0x02003000
	.long 0x00000000
