.target	 aie4
.partition	 3column
.attach_to_group 0


.section .ctrltext

start_job	0
    load_pdi	0, @pdi0
end_job	
start_job	1
    preempt	0, @save, @restore, @hintmap_0
end_job	
.eop
eof	

.section .ctrldata

  .align             4
hintmap_0:
  .long              0x0000000f

.section .ctrltext

start_job	 2
preempt	 1,  @save,  @restore,  @hintmap_1
end_job	
start_job	 3
preempt	 2,  @save,  @restore,  @hintmap_2
end_job	
.eop
eof	

.section .ctrldata

  .align             4
hintmap_1:
  .long              0x0000000f
  .align             4
hintmap_2:
  .long              0xffffffff
  .long              0xffffffff
  .long              0xffffffff
  .long              0x0fffffff

.section .ctrltext

pdi0:
start_job	 9
nop	
end_job	
.eop
eof	

.section .ctrldata

.endl pdi0
.attach_to_group 2


.section .ctrltext

zpage_0007:
start_job	 0
load_pdi	 0,  @pdi0
end_job	
start_job	 1
preempt	 0,  @save,  @restore,  @hintmap_0
end_job	
start_job	 2
preempt	 1,  @save,  @restore,  @hintmap_1
end_job	
start_job	 3
preempt	 2,  @save,  @restore,  @hintmap_2
end_job	
.eop
eof	

.section .ctrldata

  .align             4
hintmap_0:
  .long              0x00000000
  .long              0xffff0000
  .long              0xffffffff
  .align             4
hintmap_1:
  .long              0x00000000
  .long              0xffff0000
  .long              0xffffffff
  .align             4
hintmap_2:
  .long              0x00000000
.endl zpage_0007

.section .ctrltext

pdi0:
start_job	 19
nop	
end_job	
.eop
eof	

.section .ctrldata

.endl pdi0
.attach_to_group 4


.section .ctrltext

zpage_0013:
start_job	 0
load_pdi	 0,  @pdi0
end_job	
start_job	 1
preempt	 0,  @save,  @restore,  @hintmap_0
end_job	
start_job	 2
preempt	 1,  @save,  @restore,  @hintmap_1
end_job	
start_job	 3
preempt	 2,  @save,  @restore,  @hintmap_2
end_job	
.eop
eof	

.section .ctrldata

  .align             4
hintmap_0:
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x0f000000
  .align             4
hintmap_1:
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0xff000000
  .long              0x00000007
  .align             4
hintmap_2:
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000000
  .long              0x00000004
.endl zpage_0013

.section .ctrltext

pdi0:
start_job	 29
nop	
end_job	
.eop
eof	

.section .ctrldata

.endl pdi0
