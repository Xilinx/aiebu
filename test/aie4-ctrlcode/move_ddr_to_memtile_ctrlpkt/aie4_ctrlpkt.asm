;
;text
;
START_JOB 1
    APPLY_OFFSET_57         @ctl_mm2s0_a_bd0_col0, 1, 3
    UC_DMA_WRITE_DES_SYNC   @UCBD_label_0
    uC_DMA_WRITE_DES_SYNC   @uc_dma_bd_cp
; enqueue cp bd0 
    WRITE_32                0x00018564, 0x80000000
    MASK_POLL_32            0x1C000, 0xFFFFFFFF, 0xCAFECAFE
    LOCAL_BARRIER           $lb0, 2
END_JOB

EOF

;
;data
;
.align    16
uc_dma_bd_cp:
    UC_DMA_BD    0, 0x00018300, @ctl_mm2s0_a_bd0_col0, 4, 0, 1
    UC_DMA_BD    0, 0x00018314, @ctl_mm2s0_b_bd0_col0, 4, 0, 0

UCBD_label_0:
;
;control packet through circuit routing
;
;0_0 unicast from mm2s ctrl 0 to tile ctrl 0
    UC_DMA_BD    0, 0x8074, @WRITE_data_0, 1, 0, 1
    UC_DMA_BD    0, 0x8038, @WRITE_data_1, 1, 0, 0

.align    4
ctl_mm2s0_a_bd0_col0:
  .long 0x00000000
  .long 0x00000000
  .long 0x0000000c
  .long 0x00000000
ctl_mm2s0_b_bd0_col0:
  .long 0x00000000
  .long 0x00000000
  .long 0x00000000
  .long 0x00000000
WRITE_data_0:
    .long 0x80000000
WRITE_data_1:
    .long 0x8000001a
