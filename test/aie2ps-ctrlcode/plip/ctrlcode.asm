;
; Test: APPLY_OFFSET_PL + UC_DMA_MASK_POLL_EXT
; Verifies assembly, disassembly round-trip for the wts_pl_ctrl PL IP opcodes.
;

START_JOB 0
  APPLY_OFFSET_PL @wts_params, 1, 2
  UC_DMA_WRITE_DES_SYNC @wts_pl_bd_chain
  UC_DMA_MASK_POLL_EXT  0x00000800, 0x00030000, 0x00000002, 0x00000002
END_JOB

EOF

;
; Data
;

.align 16
wts_pl_bd_chain:
  UC_DMA_BD 0x800, 0x00030010, @wts_params, 10, 1, 1
  UC_DMA_BD 0x800, 0x00030000, @val_ap_start, 1, 1, 0

.align 4
wts_params:
  .long 0x00000001
  .long 0x00000002
  .long 0x00000003
  .long 0x00000004
  .long 0x00000005
  .long 0x00000006
  .long 0x00000007
  .long 0x00000008
  .long 0x00000009
  .long 0x00000010
val_ap_start:
  .long 0x00000001
