.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;
.attach_to_group	 4
; RECORD TIMER START
START_JOB 0
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_0
REMOTE_BARRIER	 $rb0, 0x15
; RECORD TIMER STOP
END_JOB

.eop

EOF

;
;data
;
.align    16
UCBD_label_0:
	 UC_DMA_BD	 0, 0x4109f70, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109f78, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109f80, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149f70, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149f78, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149f7c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109f74, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109f7c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110010, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110020, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110030, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110040, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110050, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4148900, @DMAWRITE_data_0, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148930, @DMAWRITE_data_1, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148960, @DMAWRITE_data_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148990, @DMAWRITE_data_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108c00, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109200, @DMAWRITE_data_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148c00, @DMAWRITE_data_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4149200, @DMAWRITE_data_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4149e1c, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_2, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110000, @WRITE_data_2, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x94af94af
WRITE_data_1:
	.long 0x00000004
WRITE_data_2:
	.long 0x00000000
DMAWRITE_data_0:
	.long 0x00800000
	.long 0xf8000400
	.long 0x1c100b85
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_1:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000b87
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x14000000
DMAWRITE_data_2:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000b89
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x1c000000
DMAWRITE_data_3:
	.long 0x00800000
	.long 0x00000000
	.long 0x1c000b8b
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4:
	.long 0x00800000
	.long 0xfe000100
	.long 0x1c200b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_5:
	.long 0x00800100
	.long 0xfe000100
	.long 0x1c300b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6:
	.long 0x00800200
	.long 0xfe000100
	.long 0x1c400b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7:
	.long 0x00800300
	.long 0xfe000100
	.long 0x1c500b83
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
hintmap_0:
	.long 0x00000000
