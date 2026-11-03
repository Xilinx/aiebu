.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column

.attach_to_group 2

START_JOB 1
	UC_DMA_WRITE_DES_SYNC	 @UCBD_label_uc2
.section annotation
id: 0
name: dma_uc2
description: dma uc2
END_JOB
.section annotation
id: 1
name: end_uc2
description: end uc2
EOF

.align    16
UCBD_label_uc2:
	 UC_DMA_BD	 0, 0x120000, @DMAWRITE_data_uc2, 4, 0, 1
.align    4
DMAWRITE_data_uc2:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
