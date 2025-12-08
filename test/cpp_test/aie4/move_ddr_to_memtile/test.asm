;
; Code
;
.partition	 1column
START_JOB 0
  LOAD_PDI 0, @pdi
END_JOB
.eop

START_JOB 1
  APPLY_OFFSET_57     @shim00_bd0, 1, 0
  uC_DMA_WRITE_DES    $r0, @uc_dma_bd0
  WAIT_uC_DMA         $r0
  LOCAL_BARRIER       $lb0, 2
END_JOB

START_JOB 2
  LOCAL_BARRIER       $lb0, 2
; enqueue bd0 0_0
  WRITE_32            0x018554, 0x80000000
; enqueue bd0 0_1
  WRITE_32            0x0109E04, 0x80000000
; enqueue bd0 0_2
  WRITE_32            0x06ACE04, 0x80000000
; enqueue bd0 0_3
  WRITE_32            0x07ACE04, 0x80000000
; enqueue bd0 0_4
  WRITE_32            0x08ACE04, 0x80000000
; enqueue bd0 0_5
  WRITE_32            0x09ACE04, 0x80000000
; wait tct
  WAIT_TCTS           TILE_0_0, SHIM_MM2S_0, 1 
  WAIT_TCTS           TILE_0_1, MEM_S2MM_0, 1
  WAIT_TCTS           TILE_0_2, TILE_S2MM_0, 1
  WAIT_TCTS           TILE_0_3, TILE_S2MM_0, 1
  WAIT_TCTS           TILE_0_4, TILE_S2MM_0, 1
  WAIT_TCTS           TILE_0_5, TILE_S2MM_0, 1
END_JOB

pdi:
.include aie4_pdi.asm
.endl pdi
EOF

;
; Data
;

  .ALIGN             16
uc_dma_bd0:
  UC_DMA_BD         0, 0x00018000, @shim00_bd0, 9, 0, 1
  UC_DMA_BD         0, 0x00108000, @mem01_bd0, 11, 0, 1
  UC_DMA_BD         0, 0x006AC000, @core02_bd0, 6, 0, 1
  UC_DMA_BD         0, 0x007AC000, @core03_bd0, 6, 0, 1
  UC_DMA_BD         0, 0x008AC000, @core04_bd0, 6, 0, 1
  UC_DMA_BD         0, 0x009AC000, @core05_bd0, 6, 0, 0

  .align             4
shim00_bd0:
  .long              0x00000000
  .long              0x00000000
  .long              0x00000002
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000

  .align             4
mem01_bd0:
  .long              0x00800000
  .long              0x00000002
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
core02_bd0:
  .long              0x00000002
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
core03_bd0:
  .long              0x00000002
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
core04_bd0:
  .long              0x00000002
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
core05_bd0:
  .long              0x00000002
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
