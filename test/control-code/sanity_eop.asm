;
; Code
;

; ignored by pager
.eop
START_JOB 0
  uC_DMA_WRITE_DES    $r0, @uc_dma_bd0
  WAIT_uC_DMA         $r0
  LOCAL_BARRIER       $lb0, 2
END_JOB

START_JOB 1
  LOCAL_BARRIER       $lb0, 2
  WRITE_32            0x41A0634, 0x80000000
  WRITE_32            0x61A0604, 0x80000000
  WAIT_TCTS           TILE_2_1, MM2S_0, 1
  WAIT_TCTS           TILE_3_1, S2MM_0, 1
END_JOB

.eop
.eop

START_JOB 2
  WRITE_32            0x41A0634, 0x80000000
  WRITE_32            0x61A0604, 0x80000000
  WAIT_TCTS           TILE_2_1, MM2S_0, 1
  WAIT_TCTS           TILE_3_1, S2MM_0, 1
END_JOB

.eop
EOF
.eop

;
; Data
;

  .align             16
uc_dma_bd0:
  UC_DMA_BD         0, 0x041A0000, @mem21_bd0, 8, 0, 1
  UC_DMA_BD         0, 0x061A0000, @mem31_bd0, 8, 0, 0

  .align             4
mem21_bd0:
  .long              0x00000080
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x80000000

  .align             4
mem31_bd0:
  .long              0x00000080
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x80000000
