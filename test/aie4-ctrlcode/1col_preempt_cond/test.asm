;
; Code
;
.partition	 3column
.target aie4

START_JOB 0
  LOAD_PDI 0, @pdi 
END_JOB

START_JOB 1
  APPLY_OFFSET_57     @shim00_bd0, 1, 0
  uC_DMA_WRITE_DES    $r0, @uc_dma_bd0
  WAIT_uC_DMA         $r0
  LOCAL_BARRIER       $lb0, 2
END_JOB

START_JOB 2
  LOCAL_BARRIER       $lb0, 2
; enqueue bd0 0_0 to mm2s0
  WRITE_32            0x018554, 0x80000000
; enqueue bd0 1_1 to s2mm0
  WRITE_32            0x2109E04, 0x80000000
  WAIT_TCTS           TILE_0_0, SHIM_MM2S_0, 1 
  WAIT_TCTS           TILE_1_1, MEM_S2MM_0, 1
; enqueue bd0 1_1 mm2s1
  WRITE_32            0x2109E6C, 0x80000000
; enqueue bd0 0_1 s2mm1
  WRITE_32            0x0109E0C, 0x80000000
  WAIT_TCTS           TILE_1_1, MEM_MM2S_1, 1 
  WAIT_TCTS           TILE_0_1, MEM_S2MM_1, 1
  WRITE_32            0x1C000, 0xCAFECAFE
END_JOB

START_JOB 3
  PREEMPT 0, @save, @restore, @hint_bitmap0
END_JOB

START_COND_JOB_PREEMPT 4
  uc_DMA_WRITE_DES_SYNC @uc_dma_bd2
  uc_DMA_WRITE_DES_SYNC @uc_dma_bd2
  uc_DMA_WRITE_DES_SYNC @uc_dma_bd2
END_JOB

START_JOB 5
  uC_DMA_WRITE_DES    $r0, @uc_dma_bd1
  WAIT_uC_DMA         $r0
  LOCAL_BARRIER       $lb1, 2
END_JOB

START_JOB 6
  LOCAL_BARRIER       $lb1, 2
; enqueue bd0 0_1 mm2s1
  WRITE_32            0x0109E6C, 0x80000000
; enqueue bd0 1_1 s2mm1
  WRITE_32            0x2109E0C, 0x80000000
  WAIT_TCTS           TILE_0_1, MEM_MM2S_1, 1 
  WAIT_TCTS           TILE_1_1, MEM_S2MM_1, 1
  MASK_POLL_32        0x1C000, 0x0000FFFF, 0xCAFE
  WRITE_32            0x2210000, 0xabcdabcd
END_JOB

START_JOB 7
  PREEMPT 1, @save, @restore, @hint_bitmap1
END_JOB

START_JOB 8
  MASK_POLL_32        0x2210000, 0xFFFFFFFF, 0xabcdabcd
  APPLY_OFFSET_57     @shim00_bd1, 1, 1
  uC_DMA_WRITE_DES    $r0, @uc_dma_bd3
  WAIT_uC_DMA         $r0
  LOCAL_BARRIER       $lb2, 2
END_JOB

START_JOB 9
  LOCAL_BARRIER       $lb2, 2
; enqueue bd0 1_1 to mm2s0
  WRITE_32            0x2109E64, 0x80000000
; enqueue bd1 0_0 to s2mm0
  WRITE_32            0x018544, 0x80000001
  WAIT_TCTS           TILE_1_1, MEM_MM2S_0, 1
  WAIT_TCTS           TILE_0_0, SHIM_S2MM_0, 1
END_JOB

pdi:
.include pdi.asm
.endl pdi
EOF

;
; Data
;

  .align             16
uc_dma_bd0:
  UC_DMA_BD         0, 0x00018000, @shim00_bd0, 9, 0, 1
  UC_DMA_BD         0, 0x02108000, @mem11_s2mm0_bd0, 11, 0, 1
  UC_DMA_BD         0, 0x02108F00, @mem11_mm2s1_bd0, 11, 0, 1
  UC_DMA_BD         0, 0x00108300, @mem01_s2mm1_bd0, 11, 0, 0
uc_dma_bd1:
  UC_DMA_BD         0, 0x02108300, @mem11_s2mm1_bd0, 11, 0, 1
  UC_DMA_BD         0, 0x00108F00, @mem01_mm2s1_bd0, 11, 0, 0
uc_dma_bd3:
  UC_DMA_BD         0, 0x00018030, @shim00_bd1, 9, 0, 1
  UC_DMA_BD         0, 0x02108C00, @mem11_mm2s0_bd0, 11, 0, 0
uc_dma_bd2:
  UC_DMA_BD         0, 0x0001C000, @test_cond_job, 1, 0, 0

  .align             4
test_cond_job:
  .long              0xCAFECAFE
hint_bitmap0:
  .long              0x00000001
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
hint_bitmap1:
  .long              0x00000000
  .long              0x00030000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
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
shim00_bd1:
  .long              0x00000000
  .long              0x00000000
  .long              0x00000001
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000

mem11_s2mm0_bd0:
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
mem11_mm2s0_bd0:
  .long              0x00800001
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
mem11_mm2s1_bd0:
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
mem11_s2mm1_bd0:
  .long              0x00800001
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
mem01_s2mm1_bd0:
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
mem01_mm2s1_bd0:
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
