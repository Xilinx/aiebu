.partition	 1column
.attach_to_group 0


.section .ctrltext

start_job	0
    uc_dma_write_des	$r0, @label0
    wait_uc_dma	$r0
    local_barrier	$lb0, 2
end_job	
start_job	1
    local_barrier	$lb0, 2
    write_32	0x1A0634, 0x80000000
    wait_tcts	TILE_0_1, MEM_MM2S_0, 1
end_job	
.eop
eof	

  .align             16

.section .ctrldata

label0:
UC_DMA_BD	 0x00000000,  0x001A0000,  @label1,  8,  0,  0

  .align             4
label1:
.long	 0x00000001
.long	 0x00020000
.long	 0x00000000
.long	 0x00000000
.long	 0x00000000
.long	 0x00000000
.long	 0x00000000
.long	 0x80000000
