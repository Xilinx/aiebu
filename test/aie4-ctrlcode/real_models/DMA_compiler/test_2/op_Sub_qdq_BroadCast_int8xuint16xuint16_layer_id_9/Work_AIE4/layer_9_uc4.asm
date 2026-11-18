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
; SHIM_WGT
APPLY_OFFSET_57	 @DMAWRITE_data_11, 1, 2
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_1
UC_DMA_WRITE_DES_SYNC	 @UCBD_label_2
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
	 UC_DMA_BD	 0, 0x4110010, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110020, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110030, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110040, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110050, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110060, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110070, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110080, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110090, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x41100a0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x41100b0, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4108900, @DMAWRITE_data_0, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108930, @DMAWRITE_data_1, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148900, @DMAWRITE_data_2, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108c00, @DMAWRITE_data_3, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108f00, @DMAWRITE_data_4, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4108f30, @DMAWRITE_data_5, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109200, @DMAWRITE_data_6, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109500, @DMAWRITE_data_7, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4109530, @DMAWRITE_data_8, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4148c00, @DMAWRITE_data_9, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4149200, @DMAWRITE_data_10, 0xb, 0, 1
	 UC_DMA_BD	 0, 0x4149e1c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e1c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e64, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4149e74, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e6c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4109e7c, @WRITE_data_0, 1, 0, 1
	 UC_DMA_BD	 0, 0x4110000, @WRITE_data_1, 1, 0, 0
UCBD_label_1:
	 UC_DMA_BD	 0, 0x4018000, @DMAWRITE_data_11, 0x9, 0, 0
UCBD_label_2:
	 UC_DMA_BD	 0, 0x401855c, @WRITE_data_1, 1, 0, 1
	 UC_DMA_BD	 0, 0x4010000, @WRITE_data_1, 1, 0, 0
.align    4
WRITE_data_0:
	.long 0x00000001
WRITE_data_1:
	.long 0x00000000
DMAWRITE_data_0:
	.long 0x00800400
	.long 0xfe000120
	.long 0x1c600b8f
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_1:
	.long 0x00800500
	.long 0xfe000120
	.long 0x1c900b95
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_2:
	.long 0x00800000
	.long 0xfe000400
	.long 0x1c100b85
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_3:
	.long 0x00800000
	.long 0xfe000100
	.long 0x1c200b87
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_4:
	.long 0x00800400
	.long 0xfe000120
	.long 0x1c700b91
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_5:
	.long 0x00800500
	.long 0xfe000120
	.long 0x1ca00b97
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_6:
	.long 0x00800100
	.long 0xfe000100
	.long 0x1c300b89
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_7:
	.long 0x00800400
	.long 0xfe000120
	.long 0x1c800b8d
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x0c000000
DMAWRITE_data_8:
	.long 0x00800500
	.long 0xfe000120
	.long 0x1cb00b93
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_9:
	.long 0x00800200
	.long 0xfe000100
	.long 0x1c400b8b
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00000001
	.long 0x00200000
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
DMAWRITE_data_10:
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
DMAWRITE_data_11:
	.long 0x00000000
	.long 0x00006908
	.long 0x00000120
	.long 0x03000000
	.long 0x00000000
	.long 0x00000001
	.long 0x00000001
	.long 0x20000001
	.long 0x00000400
hintmap_0:
	.long 0x00000000
