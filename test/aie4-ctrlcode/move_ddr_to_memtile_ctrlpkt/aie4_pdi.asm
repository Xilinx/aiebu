;
; Code
;
START_JOB 4
  uC_DMA_WRITE_DES_SYNC @uc_dma_bd_pdi
END_JOB

EOF

;
; Data
;

.align             16
uc_dma_bd_pdi:
;
; tct 0_5
;
; slave tile ctl 32b packet routing for tct 0_5
  UC_DMA_BD    0, 0x09B4A00, @WRITE_label_36, 1, 0, 1
; slave tile ctl 32b slot0 0_0 for tct 0_5. packet routing for master 0 arbiter 0
  UC_DMA_BD    0, 0x09B4B00, @WRITE_label_37, 1, 0, 1
; master south0 32b from tile ctl 0_5 for tct 0_0
  UC_DMA_BD    0, 0x09B4900, @WRITE_label_38, 1, 0, 1
;
; tct 0_4
;
; slave tile ctl 32b packet routing for tct 0_4
  UC_DMA_BD    0, 0x08B4A00, @WRITE_label_33, 1, 0, 1
; slave tile ctl 32b slot0 0_0 for tct 0_4. packet routing for master 0 arbiter 0
  UC_DMA_BD    0, 0x08B4B00, @WRITE_label_34, 1, 0, 1
; slave north0 32b packet routing for tct 0_4
  UC_DMA_BD    0, 0x08B4A18, @WRITE_label_58, 1, 0, 1
; slave north0 32b slot0 0_0 for tct 0_4. packet routing for master 0 arbiter 0
  UC_DMA_BD    0, 0x08B4B60, @WRITE_label_59, 1, 0, 1
; master south0 32b from tile ctl 0_4 for tct 0_0
  UC_DMA_BD    0, 0x08B4900, @WRITE_label_35, 1, 0, 1
;
; tct 0_3
;
; slave tile ctl 32b packet routing for tct 0_3
  UC_DMA_BD    0, 0x07B4A00, @WRITE_label_30, 1, 0, 1
; slave tile ctl 32b slot0 0_0 for tct 0_3. packet routing for master 0 arbiter 0
  UC_DMA_BD    0, 0x07B4B00, @WRITE_label_31, 1, 0, 1
; slave north0 32b packet routing for tct 0_4
  UC_DMA_BD    0, 0x07B4A18, @WRITE_label_56, 1, 0, 1
; slave north0 32b slot0 0_0 for tct 0_4. packet routing for master 0 arbiter 0
  UC_DMA_BD    0, 0x07B4B60, @WRITE_label_57, 1, 0, 1
; master south0 32b from tile ctl 0_3 for tct 0_0
  UC_DMA_BD    0, 0x07B4900, @WRITE_label_32, 1, 0, 1
;
; tct 0_2
;
; slave tile ctl 32b packet routing for tct 0_2
  UC_DMA_BD    0, 0x06B4A00, @WRITE_label_27, 1, 0, 1
; slave tile ctl 32b slot0 0_0 for tct 0_2. packet routing for master 0 arbiter 0
  UC_DMA_BD    0, 0x06B4B00, @WRITE_label_28, 1, 0, 1
; slave north0 32b packet routing for tct 0_3
  UC_DMA_BD    0, 0x06B4A18, @WRITE_label_54, 1, 0, 1
; slave north0 32b slot0 0_0 for tct 0_3. packet routing for master 0 arbiter 0
  UC_DMA_BD    0, 0x06B4B60, @WRITE_label_55, 1, 0, 1
; master south0 32b from tile ctl 0_2 for tct 0_0
  UC_DMA_BD    0, 0x06B4900, @WRITE_label_29, 1, 0, 1
;
; tct 0_1
;
; slave tile ctl 32b packet routing for tct 0_1
  UC_DMA_BD    0, 0x01201C0, @WRITE_label_19, 1, 0, 1
; slave tile ctl 32b slot0 0_0 for tct 0_1. packet routing for master 0 arbiter 0
  UC_DMA_BD    0, 0x01201E0, @WRITE_label_18, 1, 0, 1
; slave north0 32b packet routing for tct 0_2
  UC_DMA_BD    0, 0x01201D0, @WRITE_label_52, 1, 0, 1
; slave north0 32b slot0 0_0 for tct 0_2. packet routing for master 0 arbiter 0
  UC_DMA_BD    0, 0x0120220, @WRITE_label_53, 1, 0, 1
; master south 32b from tile ctl 0_1 for tct 0_1
  UC_DMA_BD    0, 0x01201A0, @WRITE_label_20, 1, 0, 1
;
; tct 0_0
;
; slave tile ctl 32b for tct 0_0. packet routing for tct of 0_0
  UC_DMA_BD    0, 0x000081B0, @WRITE_label_9, 1, 0, 1
; slave tile ctl 32b slot0 0_0 for tct 0_0. packet routing for master 0 arbiter 0
  UC_DMA_BD    0, 0x000081D0, @WRITE_label_6, 1, 0, 1
; slave north 32b 0_0 packet routing for tct of 0_1 
  UC_DMA_BD    0, 0x000081C0, @WRITE_label_8, 1, 0, 1
; slave north 32b slot0 0_0 for tct 0_1. packet routing for master 0 arbiter 0
  UC_DMA_BD    0, 0x00008210, @WRITE_label_5, 1, 0, 1
;
; tcts to uC 
;
; master of uc 32b 0_0 packet routing for all master arbiter 0
  UC_DMA_BD    0, 0x00008190, @WRITE_label_10, 1, 0, 1
;
; input
;
; slave dma0 (mm2s) 0_0 for input
  UC_DMA_BD    0, 0x00008040, @WRITE_label_12, 1, 0, 1
; master north0 0_0 from slave dma0 for input
  UC_DMA_BD    0, 0x00008020, @WRITE_label_13, 1, 0, 1
; slave south0 0_1 for input
  UC_DMA_BD    0, 0x0012005C, @WRITE_label_16, 1, 0, 1
; master dma0 (s2mm) 0_1 from slave south0 for input
  UC_DMA_BD    0, 0x00120000, @WRITE_label_15, 1, 0, 1
; master north0 0_1 from slave south0 for input
  UC_DMA_BD    0, 0x00120020, @WRITE_label_39, 1, 0, 1
; slave south0 0_2 for input
  UC_DMA_BD    0, 0x006B4114, @WRITE_label_40, 1, 0, 1
; master dma0 (s2mm) 0_2 from slave south0 for input
  UC_DMA_BD    0, 0x006B4008, @WRITE_label_41, 1, 0, 1
; master north0 0_2 from slave south0 for input
  UC_DMA_BD    0, 0x006B4040, @WRITE_label_42, 1, 0, 1
; slave south0 0_3 for input
  UC_DMA_BD    0, 0x007B4114, @WRITE_label_43, 1, 0, 1
; master dma0 (s2mm) 0_3 from slave south0 for input
  UC_DMA_BD    0, 0x007B4008, @WRITE_label_44, 1, 0, 1
; master north0 0_3 from slave south0 for input
  UC_DMA_BD    0, 0x007B4040, @WRITE_label_45, 1, 0, 1
; slave south0 0_4 for input
  UC_DMA_BD    0, 0x008B4114, @WRITE_label_46, 1, 0, 1
; master dma0 (s2mm) 0_4 from slave south0 for input
  UC_DMA_BD    0, 0x008B4008, @WRITE_label_47, 1, 0, 1
; master north0 0_4 from slave south0 for input
  UC_DMA_BD    0, 0x008B4040, @WRITE_label_48, 1, 0, 1
; slave south0 0_5 for input
  UC_DMA_BD    0, 0x009B4114, @WRITE_label_49, 1, 0, 1
; master dma0 (s2mm) 0_5 from slave south0 for input
  UC_DMA_BD    0, 0x009B4008, @WRITE_label_50, 1, 0, 1
; master north0 0_5 from slave south0 for input
  UC_DMA_BD    0, 0x009B4040, @WRITE_label_51, 1, 0, 1
;
; output
;
; slave dma0 (mm2s) 0_1 for output
  UC_DMA_BD    0, 0x00120040, @WRITE_label_21, 1, 0, 1
; master south0 from mm2s0 0_1 for output
  UC_DMA_BD    0, 0x00120014, @WRITE_label_22, 1, 0, 1
; slave north0 1_0 for output
  UC_DMA_BD    0, 0x00008064, @WRITE_label_23, 1, 0, 1
; master dma0 (s2mm) 0_0 from slave north0 for output
  UC_DMA_BD    0, 0x00008000, @WRITE_label_26, 1, 0, 0


  .align             4
WRITE_label_0:
  .long 0x80000000
WRITE_label_1:
  .long 0x80000000
WRITE_label_2:
  .long 0x80000007
WRITE_label_3:
  .long 0x80000000
WRITE_label_4:
  .long 0x00010100
WRITE_label_5:
  .long 0x00010100
WRITE_label_6:
  .long 0x00010100
WRITE_label_7:
  .long 0xC0000000
WRITE_label_8:
  .long 0xC0000000
WRITE_label_9:
  .long 0xC0000000
WRITE_label_10:
  .long 0xC0000078
WRITE_label_12:
  .long 0x80000000
WRITE_label_13:
  .long 0x80000000
WRITE_label_15:
  .long 0x8000000E
WRITE_label_16:
  .long 0x80000000
WRITE_label_18:
  .long 0x00010100
WRITE_label_19:
  .long 0xC0000000
WRITE_label_20:
  .long 0xC0000078
WRITE_label_21:
  .long 0x80000000
WRITE_label_22:
  .long 0x80000000
WRITE_label_23:
  .long 0x80000000
WRITE_label_26:
  .long 0x80000012
WRITE_label_27:
  .long 0xC0000000
WRITE_label_28:
  .long 0x00010100
WRITE_label_29:
  .long 0xC0000078
WRITE_label_30:
  .long 0xC0000000
WRITE_label_31:
  .long 0x00010100
WRITE_label_32:
  .long 0xC0000078
WRITE_label_33:
  .long 0xC0000000
WRITE_label_34:
  .long 0x00010100
WRITE_label_35:
  .long 0xC0000078
WRITE_label_36:
  .long 0xC0000000
WRITE_label_37:
  .long 0x00010100
WRITE_label_38:
  .long 0xC0000078
WRITE_label_39:
  .long 0x8000000E
WRITE_label_40:
  .long 0x80000000
WRITE_label_41:
  .long 0x80000005
WRITE_label_42:
  .long 0x80000005
WRITE_label_43:
  .long 0x80000000
WRITE_label_44:
  .long 0x80000005
WRITE_label_45:
  .long 0x80000005
WRITE_label_46:
  .long 0x80000000
WRITE_label_47:
  .long 0x80000005
WRITE_label_48:
  .long 0x80000005
WRITE_label_49:
  .long 0x80000000
WRITE_label_50:
  .long 0x80000005
WRITE_label_51:
  .long 0x80000005
WRITE_label_52:
  .long 0xC0000000
WRITE_label_53:
  .long 0x00010100
WRITE_label_54:
  .long 0xC0000000
WRITE_label_55:
  .long 0x00010100
WRITE_label_56:
  .long 0xC0000000
WRITE_label_57:
  .long 0x00010100
WRITE_label_58:
  .long 0xC0000000
WRITE_label_59:
  .long 0x00010100
