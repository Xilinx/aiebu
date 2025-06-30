/* SPDX-License-Identifier: MIT */
/*
 * Copyright (C) 2023-2025 Advanced Micro Devices, Inc. All rights reserved.
 */

#ifndef _ISA_DEFINES_H_
#define _ISA_DEFINES_H_

#include <stdint.h>
#include "isa_stubs.h"

// Operation implementation forward declarations

static unsigned int control_op_start_job(const uint8_t *_pc, uint16_t job_id, uint16_t size);
static unsigned int control_op_start_job_deferred(const uint8_t *_pc, uint16_t job_id, uint16_t size);
static unsigned int control_op_launch_job(const uint8_t *_pc, uint16_t job_id);
static unsigned int control_op_uc_dma_write_des(const uint8_t *_pc, uint8_t wait_handle_reg, uint16_t descriptor_ptr);
static unsigned int control_op_wait_uc_dma(const uint8_t *_pc, uint8_t wait_handle_reg);
static unsigned int control_op_mask_write_32(const uint8_t *_pc, uint32_t address, uint32_t mask, uint32_t value);
static unsigned int control_op_write_32(const uint8_t *_pc, uint32_t address, uint32_t value);
static unsigned int control_op_wait_tcts(const uint8_t *_pc, uint16_t tile_id, uint8_t actor_id, uint8_t target_tcts);
static unsigned int control_op_end_job(const uint8_t *_pc);
static unsigned int control_op_yield(const uint8_t *_pc);
static unsigned int control_op_uc_dma_write_des_sync(const uint8_t *_pc, uint16_t descriptor_ptr);
static unsigned int control_op_write_32_d(const uint8_t *_pc, uint8_t flags, uint32_t address, uint32_t value);
static unsigned int control_op_read_32(const uint8_t *_pc, uint8_t value_reg, uint32_t address);
static unsigned int control_op_read_32_d(const uint8_t *_pc, uint8_t address_reg, uint8_t value_reg);
static unsigned int control_op_apply_offset_57(const uint8_t *_pc, uint16_t table_ptr, uint16_t num_entries, uint16_t offset);
static unsigned int control_op_add(const uint8_t *_pc, uint8_t dest_reg, uint32_t value);
static unsigned int control_op_mov(const uint8_t *_pc, uint8_t dest_reg, uint32_t value);
static unsigned int control_op_local_barrier(const uint8_t *_pc, uint8_t local_barrier_id, uint8_t num_participants);
static unsigned int control_op_remote_barrier(const uint8_t *_pc, uint8_t remote_barrier_id, uint32_t party_mask);
static unsigned int control_op_eof(const uint8_t *_pc);
static unsigned int control_op_poll_32(const uint8_t *_pc, uint32_t address, uint32_t value);
static unsigned int control_op_mask_poll_32(const uint8_t *_pc, uint32_t address, uint32_t mask, uint32_t value);
static unsigned int control_op_trace(const uint8_t *_pc, uint16_t info);
static unsigned int control_op_nop(const uint8_t *_pc);
static unsigned int control_op_preempt(const uint8_t *_pc, uint16_t id, uint16_t save_control_code_offset, uint16_t restore_control_code_offset);
static unsigned int control_op_load_pdi(const uint8_t *_pc, uint32_t pdi_id, uint16_t pdi_host_addr_offset);
static unsigned int control_op_load_cores(const uint8_t *_pc, uint32_t core_elf_id, uint16_t core_elf_host_addr_offset);
static unsigned int control_op_load_last_pdi(const uint8_t *_pc);
static unsigned int control_op_save_timestamps(const uint8_t *_pc, uint32_t unq_id);
static unsigned int control_op_sleep(const uint8_t *_pc, uint32_t target);
static unsigned int control_op_save_register(const uint8_t *_pc, uint32_t address, uint32_t unq_id);


// Dispatchers

static inline unsigned int control_dispatch_start_job(const uint8_t *pc)
{
  return control_op_start_job(
    pc,
    /* job_id (const) */ *(uint16_t *)(&pc[2]),
    /* size (jobsize) */ *(uint16_t *)(&pc[4])
  );
}

static inline unsigned int control_dispatch_start_job_deferred(const uint8_t *pc)
{
  return control_op_start_job_deferred(
    pc,
    /* job_id (const) */ *(uint16_t *)(&pc[2]),
    /* size (jobsize) */ *(uint16_t *)(&pc[4])
  );
}

static inline unsigned int control_dispatch_launch_job(const uint8_t *pc)
{
  return control_op_launch_job(
    pc,
    /* job_id (const) */ *(uint16_t *)(&pc[2])
  );
}

static inline unsigned int control_dispatch_uc_dma_write_des(const uint8_t *pc)
{
  return control_op_uc_dma_write_des(
    pc,
    /* wait_handle (register) */ *(uint8_t *)(&pc[2]),
    /* descriptor_ptr (const) */ *(uint16_t *)(&pc[4])
  );
}

static inline unsigned int control_dispatch_wait_uc_dma(const uint8_t *pc)
{
  return control_op_wait_uc_dma(
    pc,
    /* wait_handle (register) */ *(uint8_t *)(&pc[2])
  );
}

static inline unsigned int control_dispatch_mask_write_32(const uint8_t *pc)
{
  return control_op_mask_write_32(
    pc,
    /* address (const) */ *(uint32_t *)(&pc[4]),
    /* mask (const) */ *(uint32_t *)(&pc[8]),
    /* value (const) */ *(uint32_t *)(&pc[12])
  );
}

static inline unsigned int control_dispatch_write_32(const uint8_t *pc)
{
  return control_op_write_32(
    pc,
    /* address (const) */ *(uint32_t *)(&pc[4]),
    /* value (const) */ *(uint32_t *)(&pc[8])
  );
}

static inline unsigned int control_dispatch_wait_tcts(const uint8_t *pc)
{
  return control_op_wait_tcts(
    pc,
    /* tile_id (const) */ *(uint16_t *)(&pc[2]),
    /* actor_id (const) */ *(uint8_t *)(&pc[4]),
    /* target_tcts (const) */ *(uint8_t *)(&pc[6])
  );
}

static inline unsigned int control_dispatch_end_job(const uint8_t *pc)
{
  return control_op_end_job(
    pc
  );
}

static inline unsigned int control_dispatch_yield(const uint8_t *pc)
{
  return control_op_yield(
    pc
  );
}

static inline unsigned int control_dispatch_uc_dma_write_des_sync(const uint8_t *pc)
{
  return control_op_uc_dma_write_des_sync(
    pc,
    /* descriptor_ptr (const) */ *(uint16_t *)(&pc[2])
  );
}

static inline unsigned int control_dispatch_write_32_d(const uint8_t *pc)
{
  return control_op_write_32_d(
    pc,
    /* flags (const) */ *(uint8_t *)(&pc[2]),
    /* address (const) */ *(uint32_t *)(&pc[4]),
    /* value (const) */ *(uint32_t *)(&pc[8])
  );
}

static inline unsigned int control_dispatch_read_32(const uint8_t *pc)
{
  return control_op_read_32(
    pc,
    /* value (register) */ *(uint8_t *)(&pc[2]),
    /* address (const) */ *(uint32_t *)(&pc[4])
  );
}

static inline unsigned int control_dispatch_read_32_d(const uint8_t *pc)
{
  return control_op_read_32_d(
    pc,
    /* address (register) */ *(uint8_t *)(&pc[2]),
    /* value (register) */ *(uint8_t *)(&pc[3])
  );
}

static inline unsigned int control_dispatch_apply_offset_57(const uint8_t *pc)
{
  return control_op_apply_offset_57(
    pc,
    /* table_ptr (const) */ *(uint16_t *)(&pc[2]),
    /* num_entries (const) */ *(uint16_t *)(&pc[4]),
    /* offset (const) */ *(uint16_t *)(&pc[6])
  );
}

static inline unsigned int control_dispatch_add(const uint8_t *pc)
{
  return control_op_add(
    pc,
    /* dest (register) */ *(uint8_t *)(&pc[2]),
    /* value (const) */ *(uint32_t *)(&pc[4])
  );
}

static inline unsigned int control_dispatch_mov(const uint8_t *pc)
{
  return control_op_mov(
    pc,
    /* dest (register) */ *(uint8_t *)(&pc[2]),
    /* value (const) */ *(uint32_t *)(&pc[4])
  );
}

static inline unsigned int control_dispatch_local_barrier(const uint8_t *pc)
{
  return control_op_local_barrier(
    pc,
    /* local_barrier_id (barrier) */ *(uint8_t *)(&pc[2]),
    /* num_participants (const) */ *(uint8_t *)(&pc[3])
  );
}

static inline unsigned int control_dispatch_remote_barrier(const uint8_t *pc)
{
  return control_op_remote_barrier(
    pc,
    /* remote_barrier_id (barrier) */ *(uint8_t *)(&pc[2]),
    /* party_mask (const) */ *(uint32_t *)(&pc[4])
  );
}

static inline unsigned int control_dispatch_eof(const uint8_t *pc)
{
  return control_op_eof(
    pc
  );
}

static inline unsigned int control_dispatch_poll_32(const uint8_t *pc)
{
  return control_op_poll_32(
    pc,
    /* address (const) */ *(uint32_t *)(&pc[4]),
    /* value (const) */ *(uint32_t *)(&pc[8])
  );
}

static inline unsigned int control_dispatch_mask_poll_32(const uint8_t *pc)
{
  return control_op_mask_poll_32(
    pc,
    /* address (const) */ *(uint32_t *)(&pc[4]),
    /* mask (const) */ *(uint32_t *)(&pc[8]),
    /* value (const) */ *(uint32_t *)(&pc[12])
  );
}

static inline unsigned int control_dispatch_trace(const uint8_t *pc)
{
  return control_op_trace(
    pc,
    /* info (const) */ *(uint16_t *)(&pc[2])
  );
}

static inline unsigned int control_dispatch_nop(const uint8_t *pc)
{
  return control_op_nop(
    pc
  );
}

static inline unsigned int control_dispatch_preempt(const uint8_t *pc)
{
  return control_op_preempt(
    pc,
    /* id (const) */ *(uint16_t *)(&pc[2]),
    /* save_control_code_offset (page_id) */ *(uint16_t *)(&pc[4]),
    /* restore_control_code_offset (page_id) */ *(uint16_t *)(&pc[6])
  );
}

static inline unsigned int control_dispatch_load_pdi(const uint8_t *pc)
{
  return control_op_load_pdi(
    pc,
    /* pdi_id (const) */ *(uint32_t *)(&pc[4]),
    /* pdi_host_addr_offset (page_id) */ *(uint16_t *)(&pc[8])
  );
}

static inline unsigned int control_dispatch_load_cores(const uint8_t *pc)
{
  return control_op_load_cores(
    pc,
    /* core_elf_id (const) */ *(uint32_t *)(&pc[4]),
    /* core_elf_host_addr_offset (page_id) */ *(uint16_t *)(&pc[8])
  );
}

static inline unsigned int control_dispatch_load_last_pdi(const uint8_t *pc)
{
  return control_op_load_last_pdi(
    pc
  );
}

static inline unsigned int control_dispatch_save_timestamps(const uint8_t *pc)
{
  return control_op_save_timestamps(
    pc,
    /* unq_id (const) */ *(uint32_t *)(&pc[4])
  );
}

static inline unsigned int control_dispatch_sleep(const uint8_t *pc)
{
  return control_op_sleep(
    pc,
    /* target (const) */ *(uint32_t *)(&pc[4])
  );
}

static inline unsigned int control_dispatch_save_register(const uint8_t *pc)
{
  return control_op_save_register(
    pc,
    /* address (const) */ *(uint32_t *)(&pc[4]),
    /* unq_id (const) */ *(uint32_t *)(&pc[8])
  );
}


// Case statements for regular operations

#define DISPATCH_REGULAR_OPS \
  case ISA_OPCODE_LAUNCH_JOB: pc += control_dispatch_launch_job(pc); break; \
  case ISA_OPCODE_UC_DMA_WRITE_DES: pc += control_dispatch_uc_dma_write_des(pc); break; \
  case ISA_OPCODE_WAIT_UC_DMA: pc += control_dispatch_wait_uc_dma(pc); break; \
  case ISA_OPCODE_MASK_WRITE_32: pc += control_dispatch_mask_write_32(pc); break; \
  case ISA_OPCODE_WRITE_32: pc += control_dispatch_write_32(pc); break; \
  case ISA_OPCODE_WAIT_TCTS: pc += control_dispatch_wait_tcts(pc); break; \
  case ISA_OPCODE_YIELD: pc += control_dispatch_yield(pc); break; \
  case ISA_OPCODE_UC_DMA_WRITE_DES_SYNC: pc += control_dispatch_uc_dma_write_des_sync(pc); break; \
  case ISA_OPCODE_WRITE_32_D: pc += control_dispatch_write_32_d(pc); break; \
  case ISA_OPCODE_READ_32: pc += control_dispatch_read_32(pc); break; \
  case ISA_OPCODE_READ_32_D: pc += control_dispatch_read_32_d(pc); break; \
  case ISA_OPCODE_APPLY_OFFSET_57: pc += control_dispatch_apply_offset_57(pc); break; \
  case ISA_OPCODE_ADD: pc += control_dispatch_add(pc); break; \
  case ISA_OPCODE_MOV: pc += control_dispatch_mov(pc); break; \
  case ISA_OPCODE_LOCAL_BARRIER: pc += control_dispatch_local_barrier(pc); break; \
  case ISA_OPCODE_REMOTE_BARRIER: pc += control_dispatch_remote_barrier(pc); break; \
  case ISA_OPCODE_POLL_32: pc += control_dispatch_poll_32(pc); break; \
  case ISA_OPCODE_MASK_POLL_32: pc += control_dispatch_mask_poll_32(pc); break; \
  case ISA_OPCODE_TRACE: pc += control_dispatch_trace(pc); break; \
  case ISA_OPCODE_NOP: pc += control_dispatch_nop(pc); break; \
  case ISA_OPCODE_PREEMPT: pc += control_dispatch_preempt(pc); break; \
  case ISA_OPCODE_LOAD_PDI: pc += control_dispatch_load_pdi(pc); break; \
  case ISA_OPCODE_LOAD_CORES: pc += control_dispatch_load_cores(pc); break; \
  case ISA_OPCODE_LOAD_LAST_PDI: pc += control_dispatch_load_last_pdi(pc); break; \
  case ISA_OPCODE_SAVE_TIMESTAMPS: pc += control_dispatch_save_timestamps(pc); break; \
  case ISA_OPCODE_SLEEP: pc += control_dispatch_sleep(pc); break; \
  case ISA_OPCODE_SAVE_REGISTER: pc += control_dispatch_save_register(pc); break;


#endif
