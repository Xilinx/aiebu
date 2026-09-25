;
; Code
;
.partition	 3column
.target aie4

START_JOB 0
  LOAD_PDI 0, @pdi
END_JOB

START_JOB 1
  LAUNCH_JOB 2
END_JOB

START_JOB_DEFERRED 2
  APPLY_OFFSET_57     @shim00_bd0, 1, 0
  uC_DMA_WRITE_DES    $r0, @uc_dma_bd0
  WAIT_uC_DMA         $r0
  LAUNCH_JOB          3
  LOCAL_BARRIER       $lb0, 2
END_JOB

START_JOB_DEFERRED 3
  LOCAL_BARRIER       $lb0, 2
  WRITE_32            0x018554, 0x80000000
  WAIT_TCTS           TILE_0_0, SHIM_MM2S_0, 1 
  WRITE_32            0x0109E04, 0x80000000
  WAIT_TCTS           TILE_0_1, MEM_S2MM_0, 1
  LOCAL_BARRIER       $lb1, 2
END_JOB

START_JOB 4
  LOCAL_BARRIER       $lb1, 2
  READ_32             $r0, 0x200000                       ;read mem-tile 0x200000 -> r0
  WRITE_32_D          2, 0x2200000, 0                     ;move $r0 → mem-tile 0x2200000
  MASK_POLL_32        0x2200000, 0xFFFFFFFF, 0xabcdabcd
  WRITE_32            0x200004, 0x2200000                 ;move pointer (0x2200000) 0x200004
  READ_32             $r1, 0x200004                       ;read mem-tile 0x200004 -> r1
  READ_32_D           $r1, $r2                            ;move $r1 [0x2200000] → $r2
  WRITE_32_D          2, 0x4200000, 2                     ;move $r2 → mem-tile 0x4200000
  MASK_POLL_32        0x4200000, 0xFFFFFFFF, 0xabcdabcd
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
  UC_DMA_BD         0, 0x00108000, @mem01_bd0, 11, 0, 0

  .align             4
shim00_bd0:
  .long              0x00000000
  .long              0x00000000
  .long              0x00000001
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000

  .align             4
mem01_bd0:
  .long              0x00800000
  .long              0x00000001
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
