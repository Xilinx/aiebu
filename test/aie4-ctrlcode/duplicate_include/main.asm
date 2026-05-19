; Entry point assembled for the DPU instance.
; Includes test.asm (which itself includes dup.asm) and then includes
; dup.asm directly, triggering the duplicate-file-name error.
.attach_to_group 0
.partition 1column
START_JOB 0
  NOP
END_JOB
.include test.asm
.include dup.asm
