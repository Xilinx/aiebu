; 3-column span-overlap-due-to-holes redistribution test.
;
; All three controllers have fully disjoint bits but interleaved every 3rd chunk,
; so every controller's span [first_bit, last_bit] overlaps every other controller's span.
;
; col0 hintmap: bits 0,3,6,9,12,15  -> word0 = 0x00009249  (6 set chunks, span [0,15])
; col2 hintmap: bits 1,4,7,10,13,16 -> word0 = 0x00012492  (6 set chunks, span [1,16])
; col4 hintmap: bits 2,5,8,11,14,17 -> word0 = 0x00024924  (6 set chunks, span [2,17])
;
; Combined sorted chunks: 0,1,2,...,17 (18 chunks, no gaps). K=3 controllers need 2 cuts.
; All inter-chunk gaps are size 0, so the greedy cutter falls back to midpoint distance.
; First cut: midpoint of [0,17] is index 8 → splits into [0,8] and [9,17].
; Second cut: midpoint of [0,8] is index 3 (mid = 0 + 9//2 - 1 = 3) → splits into [0,3] and [4,8].
; Resulting segments sorted by original base (col0<col2<col4):
;   col0 -> chunks  0- 3  (base=0x000000, size= 4*64KB=0x040000)
;   col2 -> chunks  4- 8  (base=0x040000, size= 5*64KB=0x050000)
;   col4 -> chunks  9-17  (base=0x090000, size= 9*64KB=0x090000)
.include uc0.asm
.eop
.include uc2.asm
.eop
.include uc4.asm
