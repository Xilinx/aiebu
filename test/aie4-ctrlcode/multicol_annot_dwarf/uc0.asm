.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column

.attach_to_group 0

START_JOB 0
	UC_DMA_WRITE_DES_SYNC	 @UCBD_label_uc0
.section annotation
id: 0
name: dma_uc0
description: dma uc0
END_JOB
.section annotation
id: 1
name: end_uc0
description: end uc0
EOF

.align    16
UCBD_label_uc0:
	 UC_DMA_BD	 0, 0x120000, @DMAWRITE_data_uc0, 4, 0, 1
.align    4
DMAWRITE_data_uc0:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
