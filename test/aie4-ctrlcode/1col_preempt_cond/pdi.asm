;
;text
;
START_JOB 0
    UC_DMA_WRITE_DES_SYNC @UCBD_label_0
END_JOB


EOF

;
;data
;
.align    16
UCBD_label_0:
;
;input
;
;0_0 unicast from dma0 to east0
    UC_DMA_BD    0, 0x8040, @WRITE_data_0, 1, 0, 1
    UC_DMA_BD    0, 0x802c, @WRITE_data_1, 1, 0, 1
;1_0 unicast from west0 to north0
    UC_DMA_BD    0, 0x200805c, @WRITE_data_2, 1, 0, 1
    UC_DMA_BD    0, 0x2008020, @WRITE_data_3, 1, 0, 1
;1_1 unicast from south0 to dma0
    UC_DMA_BD    0, 0x212005c, @WRITE_data_4, 1, 0, 1
    UC_DMA_BD    0, 0x2120000, @WRITE_data_5, 1, 0, 1
;
;output
;
;0_0 unicast from east0 to dma0
    UC_DMA_BD    0, 0x806c, @WRITE_data_6, 1, 0, 1
    UC_DMA_BD    0, 0x8000, @WRITE_data_7, 1, 0, 1
;1_0 unicast from north0 to west0
    UC_DMA_BD    0, 0x2008064, @WRITE_data_8, 1, 0, 1
    UC_DMA_BD    0, 0x2008018, @WRITE_data_9, 1, 0, 1
;1_1 unicast from dma0 to south0
    UC_DMA_BD    0, 0x2120040, @WRITE_data_10, 1, 0, 1
    UC_DMA_BD    0, 0x2120014, @WRITE_data_11, 1, 0, 1
;
;data1
;
;1_1 unicast from dma1 to south1
    UC_DMA_BD    0, 0x2120044, @WRITE_data_12, 1, 0, 1
    UC_DMA_BD    0, 0x2120018, @WRITE_data_13, 1, 0, 1
;1_0 unicast from north1 to west1
    UC_DMA_BD    0, 0x2008068, @WRITE_data_14, 1, 0, 1
    UC_DMA_BD    0, 0x200801c, @WRITE_data_15, 1, 0, 1
;0_0 unicast from east1 to north1
    UC_DMA_BD    0, 0x8070, @WRITE_data_16, 1, 0, 1
    UC_DMA_BD    0, 0x8024, @WRITE_data_17, 1, 0, 1
;0_1 unicast from south1 to dma1
    UC_DMA_BD    0, 0x120060, @WRITE_data_18, 1, 0, 1
    UC_DMA_BD    0, 0x120004, @WRITE_data_19, 1, 0, 1
;
;data2
;
;0_1 unicast from dma1 to south1
    UC_DMA_BD    0, 0x120044, @WRITE_data_20, 1, 0, 1
    UC_DMA_BD    0, 0x120018, @WRITE_data_21, 1, 0, 1
;0_0 unicast from north1 to east1
    UC_DMA_BD    0, 0x8068, @WRITE_data_22, 1, 0, 1
    UC_DMA_BD    0, 0x8030, @WRITE_data_23, 1, 0, 1
;1_0 unicast from west1 to north1
    UC_DMA_BD    0, 0x2008060, @WRITE_data_24, 1, 0, 1
    UC_DMA_BD    0, 0x2008024, @WRITE_data_25, 1, 0, 1
;1_1 unicast from south1 to dma1
    UC_DMA_BD    0, 0x2120060, @WRITE_data_26, 1, 0, 1
    UC_DMA_BD    0, 0x2120004, @WRITE_data_27, 1, 0, 1
;
;tct
;
;0_0 packet routing from north0 and tile_ctrl 0 and east0
    UC_DMA_BD    0, 0x81c0, @WRITE_data_28, 1, 0, 1
    UC_DMA_BD    0, 0x8210, @WRITE_data_29, 1, 0, 1
    UC_DMA_BD    0, 0x81c4, @WRITE_data_30, 1, 0, 1
    UC_DMA_BD    0, 0x8220, @WRITE_data_31, 1, 0, 1
    UC_DMA_BD    0, 0x81b0, @WRITE_data_32, 1, 0, 1
    UC_DMA_BD    0, 0x81d0, @WRITE_data_33, 1, 0, 1
    UC_DMA_BD    0, 0x8190, @WRITE_data_34, 1, 0, 1
;0_1 circuit routing from tile_ctrl 0 to south0
    UC_DMA_BD    0, 0x1201c0, @WRITE_data_35, 1, 0, 1
    UC_DMA_BD    0, 0x1201a0, @WRITE_data_36, 1, 0, 1
;1_0 circuit routing from north0 to west0
    UC_DMA_BD    0, 0x20081c0, @WRITE_data_37, 1, 0, 1
    UC_DMA_BD    0, 0x2008194, @WRITE_data_38, 1, 0, 1
;1_1 circuit routing from tile_ctrl 0 to south0
    UC_DMA_BD    0, 0x21201c0, @WRITE_data_39, 1, 0, 1
    UC_DMA_BD    0, 0x21201a0, @WRITE_data_40, 1, 0, 0
.align    4
WRITE_data_0:
    .long 0x80000000
WRITE_data_1:
    .long 0x80000000
WRITE_data_2:
    .long 0x80000000
WRITE_data_3:
    .long 0x8000000e
WRITE_data_4:
    .long 0x80000000
WRITE_data_5:
    .long 0x8000000e
WRITE_data_6:
    .long 0x80000000
WRITE_data_7:
    .long 0x80000016
WRITE_data_8:
    .long 0x80000000
WRITE_data_9:
    .long 0x80000012
WRITE_data_10:
    .long 0x80000000
WRITE_data_11:
    .long 0x80000000
WRITE_data_12:
    .long 0x80000000
WRITE_data_13:
    .long 0x80000001
WRITE_data_14:
    .long 0x80000000
WRITE_data_15:
    .long 0x80000013
WRITE_data_16:
    .long 0x80000000
WRITE_data_17:
    .long 0x80000017
WRITE_data_18:
    .long 0x80000000
WRITE_data_19:
    .long 0x8000000f
WRITE_data_20:
    .long 0x80000000
WRITE_data_21:
    .long 0x80000001
WRITE_data_22:
    .long 0x80000000
WRITE_data_23:
    .long 0x80000013
WRITE_data_24:
    .long 0x80000000
WRITE_data_25:
    .long 0x8000000f
WRITE_data_26:
    .long 0x80000000
WRITE_data_27:
    .long 0x8000000f
WRITE_data_28:
    .long 0xc0000000
WRITE_data_29:
    .long 0x00010100
WRITE_data_30:
    .long 0xc0000000
WRITE_data_31:
    .long 0x00010100
WRITE_data_32:
    .long 0xc0000000
WRITE_data_33:
    .long 0x00010100
WRITE_data_34:
    .long 0xc0000078
WRITE_data_35:
    .long 0x80000000
WRITE_data_36:
    .long 0x80000000
WRITE_data_37:
    .long 0x80000000
WRITE_data_38:
    .long 0x80000007
WRITE_data_39:
    .long 0x80000000
WRITE_data_40:
    .long 0x80000000
