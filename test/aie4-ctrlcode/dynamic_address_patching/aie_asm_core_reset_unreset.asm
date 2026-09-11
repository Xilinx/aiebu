.target	 aie4
.aie_row_topology	 1-1-4-0
.partition	 3column
;
;text
;
START_JOB 0
MASK_WRITE_32	 0x6a3000, 0x2, 0x2
MASK_WRITE_32	 0x6a3000, 0x2, 0x0
MASK_WRITE_32	 0x7a3000, 0x2, 0x2
MASK_WRITE_32	 0x7a3000, 0x2, 0x0
END_JOB

.eop

EOF

;
;data
;
.align    16
.align    4
