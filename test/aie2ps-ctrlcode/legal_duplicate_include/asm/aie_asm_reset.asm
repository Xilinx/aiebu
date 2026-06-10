.target	 aie2ps
.aie_row_topology	 1-2-4-0
.partition	 24column
;
;text
;
START_JOB 0
MASK_WRITE_32	 0x7ff20, 0x1, 0x0
MASK_WRITE_32	 0x207ff20, 0x1, 0x0
MASK_WRITE_32	 0x407ff20, 0x1, 0x0
MASK_WRITE_32	 0x607ff20, 0x1, 0x0
MASK_WRITE_32	 0x807ff20, 0x1, 0x0
MASK_WRITE_32	 0xa07ff20, 0x1, 0x0
MASK_WRITE_32	 0xc07ff20, 0x1, 0x0
MASK_WRITE_32	 0xe07ff20, 0x1, 0x0
MASK_WRITE_32	 0x1007ff20, 0x1, 0x0
MASK_WRITE_32	 0x1207ff20, 0x1, 0x0
MASK_WRITE_32	 0x1407ff20, 0x1, 0x0
MASK_WRITE_32	 0x1607ff20, 0x1, 0x0
MASK_WRITE_32	 0x1807ff20, 0x1, 0x0
MASK_WRITE_32	 0x1a07ff20, 0x1, 0x0
MASK_WRITE_32	 0x1c07ff20, 0x1, 0x0
MASK_WRITE_32	 0x1e07ff20, 0x1, 0x0
MASK_WRITE_32	 0x2007ff20, 0x1, 0x0
MASK_WRITE_32	 0x2207ff20, 0x1, 0x0
MASK_WRITE_32	 0x2407ff20, 0x1, 0x0
MASK_WRITE_32	 0x2607ff20, 0x1, 0x0
MASK_WRITE_32	 0x2807ff20, 0x1, 0x0
MASK_WRITE_32	 0x2a07ff20, 0x1, 0x0
MASK_WRITE_32	 0x2c07ff20, 0x1, 0x0
MASK_WRITE_32	 0x2e07ff20, 0x1, 0x0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
MASK_WRITE_32	 0x7ff20, 0x1, 0x1
MASK_WRITE_32	 0x207ff20, 0x1, 0x1
MASK_WRITE_32	 0x407ff20, 0x1, 0x1
MASK_WRITE_32	 0x607ff20, 0x1, 0x1
MASK_WRITE_32	 0x807ff20, 0x1, 0x1
MASK_WRITE_32	 0xa07ff20, 0x1, 0x1
MASK_WRITE_32	 0xc07ff20, 0x1, 0x1
MASK_WRITE_32	 0xe07ff20, 0x1, 0x1
MASK_WRITE_32	 0x1007ff20, 0x1, 0x1
MASK_WRITE_32	 0x1207ff20, 0x1, 0x1
MASK_WRITE_32	 0x1407ff20, 0x1, 0x1
MASK_WRITE_32	 0x1607ff20, 0x1, 0x1
MASK_WRITE_32	 0x1807ff20, 0x1, 0x1
MASK_WRITE_32	 0x1a07ff20, 0x1, 0x1
MASK_WRITE_32	 0x1c07ff20, 0x1, 0x1
MASK_WRITE_32	 0x1e07ff20, 0x1, 0x1
MASK_WRITE_32	 0x2007ff20, 0x1, 0x1
MASK_WRITE_32	 0x2207ff20, 0x1, 0x1
MASK_WRITE_32	 0x2407ff20, 0x1, 0x1
MASK_WRITE_32	 0x2607ff20, 0x1, 0x1
MASK_WRITE_32	 0x2807ff20, 0x1, 0x1
MASK_WRITE_32	 0x2a07ff20, 0x1, 0x1
MASK_WRITE_32	 0x2c07ff20, 0x1, 0x1
MASK_WRITE_32	 0x2e07ff20, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
MASK_WRITE_32	 0x7ff20, 0x1, 0x1
MASK_WRITE_32	 0x207ff20, 0x1, 0x1
MASK_WRITE_32	 0x407ff20, 0x1, 0x1
MASK_WRITE_32	 0x607ff20, 0x1, 0x1
MASK_WRITE_32	 0x807ff20, 0x1, 0x1
MASK_WRITE_32	 0xa07ff20, 0x1, 0x1
MASK_WRITE_32	 0xc07ff20, 0x1, 0x1
MASK_WRITE_32	 0xe07ff20, 0x1, 0x1
MASK_WRITE_32	 0x1007ff20, 0x1, 0x1
MASK_WRITE_32	 0x1207ff20, 0x1, 0x1
MASK_WRITE_32	 0x1407ff20, 0x1, 0x1
MASK_WRITE_32	 0x1607ff20, 0x1, 0x1
MASK_WRITE_32	 0x1807ff20, 0x1, 0x1
MASK_WRITE_32	 0x1a07ff20, 0x1, 0x1
MASK_WRITE_32	 0x1c07ff20, 0x1, 0x1
MASK_WRITE_32	 0x1e07ff20, 0x1, 0x1
MASK_WRITE_32	 0x2007ff20, 0x1, 0x1
MASK_WRITE_32	 0x2207ff20, 0x1, 0x1
MASK_WRITE_32	 0x2407ff20, 0x1, 0x1
MASK_WRITE_32	 0x2607ff20, 0x1, 0x1
MASK_WRITE_32	 0x2807ff20, 0x1, 0x1
MASK_WRITE_32	 0x2a07ff20, 0x1, 0x1
MASK_WRITE_32	 0x2c07ff20, 0x1, 0x1
MASK_WRITE_32	 0x2e07ff20, 0x1, 0x1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x7ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x207ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x407ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x607ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x807ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0xa07ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0xc07ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0xe07ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1007ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1207ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1407ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1607ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1807ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a07ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c07ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e07ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2007ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2207ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2407ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2607ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2807ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2a07ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2c07ff28, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2e07ff28, @WRITE_data_0, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x7ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x207ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x407ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x607ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x807ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xa07ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xc07ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xe07ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1007ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1207ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1407ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1607ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1807ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a07ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c07ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e07ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2007ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2207ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2407ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2607ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2807ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2a07ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2c07ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2e07ff28, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x7ff48, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x207ff48, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x4002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x6002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x8002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0xa002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0xc002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0xe002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x10002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x12002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x14002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x16002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x18002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x20002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x22002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x24002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x26002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x28002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2a002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2c002100, @WRITE_data_3, 1, 0, 1
	 UC_DMA_BD	 0, 0x2e002100, @WRITE_data_3, 1, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x7ff40, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x1fff20, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x2fff20, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x360020, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x460020, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x560020, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x660020, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x207ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x21fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x22fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x407ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x41fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x42fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x607ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x61fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x62fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x6360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x6460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x6560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x6660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x807ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x81fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x82fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x8660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xa07ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xa1fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xa2fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xa360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xa460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xa560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xa660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xc07ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xc1fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xc2fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xc360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xc460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xc560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xc660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xe07ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xe1fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xe2fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xe360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xe460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xe560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0xe660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1007ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x101fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x102fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x10660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1207ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x121fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x122fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x12360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x12460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x12560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x12660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1407ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x141fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x142fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x14360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x14460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x14560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x14660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1607ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x161fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x162fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x16360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x16460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x16560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x16660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1807ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x181fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x182fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x18360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x18460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x18560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x18660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a07ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a1fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a2fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c07ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c1fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c2fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e07ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e1fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e2fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2007ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x201fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x202fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x20360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x20460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x20560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x20660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2207ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x221fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x222fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x22360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x22460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x22560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x22660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2407ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x241fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x242fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x24360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x24460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x24560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x24660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2607ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x261fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x262fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x26360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x26460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x26560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x26660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2807ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x281fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x282fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x28360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x28460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x28560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x28660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2a07ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2a1fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2a2fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2a360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2a460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2a560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2a660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2c07ff40, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2c1fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2c2fff20, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2c360020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2c460020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2c560020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2c660020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x2e07ff40, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2e1fff20, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2e2fff20, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2e360020, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2e460020, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2e560020, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x2e660020, @WRITE_data_4, 1, 0, 1
	 UC_DMA_BD	 0, 0x1010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x6001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x8001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0xa001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0xc001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0xe001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x10001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x12001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x14001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x16001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x18001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1a001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1c001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x1e001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x20001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x22001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x24001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x26001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x28001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2a001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2c001010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x2e001010, @WRITE_data_0, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x00000001
WRITE_data_1:
	.long 0x00000000
WRITE_data_2:
	.long 0x00000002
WRITE_data_3:
	.long 0x0000000c
WRITE_data_4:
	.long 0x00000008
