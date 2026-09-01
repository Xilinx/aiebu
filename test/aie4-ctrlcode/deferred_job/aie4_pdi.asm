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
WRITE_label_5:
  .long 0x00010100
WRITE_label_6:
  .long 0x00010100
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
WRITE_label_39:
  .long 0x8000000E
WRITE_label_52:
  .long 0xC0000000
WRITE_label_53:
  .long 0x00010100
