// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef _ISA_ASSEMBLER_STUBS_H_
#define _ISA_ASSEMBLER_STUBS_H_

#include "oparg.h"
#include "ops.h"

namespace aiebu {

// Named constants for opcodes and bit widths
constexpr int OPCODE_START_JOB = 0;
constexpr int OPCODE_START_JOB_DEFERRED = 23;
constexpr int OPCODE_LAUNCH_JOB = 24;
constexpr int OPCODE_UC_DMA_WRITE_DES = 1;
constexpr int OPCODE_WAIT_UC_DMA = 2;
constexpr int OPCODE_MASK_WRITE_32 = 3;
constexpr int OPCODE_WRITE_32 = 5;
constexpr int OPCODE_WAIT_TCTS = 6;
constexpr int OPCODE_END_JOB = 7;
constexpr int OPCODE_YIELD = 8;
constexpr int OPCODE_UC_DMA_WRITE_DES_SYNC = 9;
constexpr int OPCODE_WRITE_32_D = 11;
constexpr int OPCODE_READ_32 = 12;
constexpr int OPCODE_READ_32_D = 13;
constexpr int OPCODE_APPLY_OFFSET_57 = 14;
constexpr int OPCODE_ADD = 15;
constexpr int OPCODE_MOV = 16;
constexpr int OPCODE_LOCAL_BARRIER = 17;
constexpr int OPCODE_REMOTE_BARRIER = 18;
constexpr int OPCODE_EOF = 255;
constexpr int OPCODE_POLL_32 = 19;
constexpr int OPCODE_MASK_POLL_32 = 20;
constexpr int OPCODE_TRACE = 21;
constexpr int OPCODE_NOP = 22;
constexpr int OPCODE_PREEMPT = 25;
constexpr int OPCODE_LOAD_PDI = 26;
constexpr int OPCODE_LOAD_CORES = 4;
constexpr int OPCODE_LOAD_LAST_PDI = 27;
constexpr int OPCODE_SAVE_TIMESTAMPS = 28;
constexpr int OPCODE_SLEEP = 29;
constexpr int OPCODE_SAVE_REGISTER = 30;
constexpr int OPCODE_ALIGN = 0xA5;
constexpr int BIT_WIDTH_8 = 8;
constexpr int BIT_WIDTH_16 = 16;
constexpr int BIT_WIDTH_32 = 32;

// ...existing code...
class isa
{
private:
  std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> m_isa;

public:
  isa()
  {
    m_isa = std::make_shared<std::map<std::string, std::shared_ptr<isa_op>>>();

    (*m_isa)["start_job"] = std::make_shared<isa_op>("start_job", OPCODE_START_JOB, std::vector<opArg>{
     opArg("job_id", opArg::optype::CONST, BIT_WIDTH_16), opArg("size", opArg::optype::JOBSIZE, BIT_WIDTH_16), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    });

    (*m_isa)["start_job_deferred"] = std::make_shared<isa_op>("start_job_deferred", OPCODE_START_JOB_DEFERRED, std::vector<opArg>{
     opArg("job_id", opArg::optype::CONST, BIT_WIDTH_16), opArg("size", opArg::optype::JOBSIZE, BIT_WIDTH_16), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    });

    (*m_isa)["launch_job"] = std::make_shared<isa_op>("launch_job", OPCODE_LAUNCH_JOB, std::vector<opArg>{
     opArg("job_id", opArg::optype::CONST, BIT_WIDTH_16),
    });

    (*m_isa)["uc_dma_write_des"] = std::make_shared<isa_op>("uc_dma_write_des", OPCODE_UC_DMA_WRITE_DES, std::vector<opArg>{
     opArg("wait_handle", opArg::optype::REG, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8), opArg("descriptor_ptr", opArg::optype::CONST, BIT_WIDTH_16), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    });

    (*m_isa)["wait_uc_dma"] = std::make_shared<isa_op>("wait_uc_dma", OPCODE_WAIT_UC_DMA, std::vector<opArg>{
     opArg("wait_handle", opArg::optype::REG, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8),
    });

    (*m_isa)["mask_write_32"] = std::make_shared<isa_op>("mask_write_32", OPCODE_MASK_WRITE_32, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("address", opArg::optype::CONST, BIT_WIDTH_32), opArg("mask", opArg::optype::CONST, BIT_WIDTH_32), opArg("value", opArg::optype::CONST, BIT_WIDTH_32),
    });

    (*m_isa)["write_32"] = std::make_shared<isa_op>("write_32", OPCODE_WRITE_32, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("address", opArg::optype::CONST, BIT_WIDTH_32), opArg("value", opArg::optype::CONST, BIT_WIDTH_32),
    });

    (*m_isa)["wait_tcts"] = std::make_shared<isa_op>("wait_tcts", OPCODE_WAIT_TCTS, std::vector<opArg>{
     opArg("tile_id", opArg::optype::CONST, BIT_WIDTH_16), opArg("actor_id", opArg::optype::CONST, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8), opArg("target_tcts", opArg::optype::CONST, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8),
    });

    (*m_isa)["end_job"] = std::make_shared<isa_op>("end_job", OPCODE_END_JOB, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    });

    (*m_isa)["yield"] = std::make_shared<isa_op>("yield", OPCODE_YIELD, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    });

    (*m_isa)["uc_dma_write_des_sync"] = std::make_shared<isa_op>("uc_dma_write_des_sync", OPCODE_UC_DMA_WRITE_DES_SYNC, std::vector<opArg>{
     opArg("descriptor_ptr", opArg::optype::CONST, BIT_WIDTH_16),
    });

    (*m_isa)["write_32_d"] = std::make_shared<isa_op>("write_32_d", OPCODE_WRITE_32_D, std::vector<opArg>{
     opArg("flags", opArg::optype::CONST, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8), opArg("address", opArg::optype::CONST, BIT_WIDTH_32), opArg("value", opArg::optype::CONST, BIT_WIDTH_32),
    });

    (*m_isa)["read_32"] = std::make_shared<isa_op>("read_32", OPCODE_READ_32, std::vector<opArg>{
     opArg("value", opArg::optype::REG, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8), opArg("address", opArg::optype::CONST, BIT_WIDTH_32),
    });

    (*m_isa)["read_32_d"] = std::make_shared<isa_op>("read_32_d", OPCODE_READ_32_D, std::vector<opArg>{
     opArg("address", opArg::optype::REG, BIT_WIDTH_8), opArg("value", opArg::optype::REG, BIT_WIDTH_8),
    });

    (*m_isa)["apply_offset_57"] = std::make_shared<isa_op>("apply_offset_57", OPCODE_APPLY_OFFSET_57, std::vector<opArg>{
     opArg("table_ptr", opArg::optype::CONST, BIT_WIDTH_16), opArg("num_entries", opArg::optype::CONST, BIT_WIDTH_16), opArg("offset", opArg::optype::CONST, BIT_WIDTH_16),
    });

    (*m_isa)["add"] = std::make_shared<isa_op>("add", OPCODE_ADD, std::vector<opArg>{
     opArg("dest", opArg::optype::REG, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8), opArg("value", opArg::optype::CONST, BIT_WIDTH_32),
    });

    (*m_isa)["mov"] = std::make_shared<isa_op>("mov", OPCODE_MOV, std::vector<opArg>{
     opArg("dest", opArg::optype::REG, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8), opArg("value", opArg::optype::CONST, BIT_WIDTH_32),
    });

    (*m_isa)["local_barrier"] = std::make_shared<isa_op>("local_barrier", OPCODE_LOCAL_BARRIER, std::vector<opArg>{
     opArg("local_barrier_id", opArg::optype::BARRIER, BIT_WIDTH_8), opArg("num_participants", opArg::optype::CONST, BIT_WIDTH_8),
    });

    (*m_isa)["remote_barrier"] = std::make_shared<isa_op>("remote_barrier", OPCODE_REMOTE_BARRIER, std::vector<opArg>{
     opArg("remote_barrier_id", opArg::optype::BARRIER, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8), opArg("party_mask", opArg::optype::CONST, BIT_WIDTH_32),
    });

    (*m_isa)["eof"] = std::make_shared<isa_op>("eof", OPCODE_EOF, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    });

    (*m_isa)["poll_32"] = std::make_shared<isa_op>("poll_32", OPCODE_POLL_32, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("address", opArg::optype::CONST, BIT_WIDTH_32), opArg("value", opArg::optype::CONST, BIT_WIDTH_32),
    });

    (*m_isa)["mask_poll_32"] = std::make_shared<isa_op>("mask_poll_32", OPCODE_MASK_POLL_32, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("address", opArg::optype::CONST, BIT_WIDTH_32), opArg("mask", opArg::optype::CONST, BIT_WIDTH_32), opArg("value", opArg::optype::CONST, BIT_WIDTH_32),
    });

    (*m_isa)["trace"] = std::make_shared<isa_op>("trace", OPCODE_TRACE, std::vector<opArg>{
     opArg("info", opArg::optype::CONST, BIT_WIDTH_16),
    });

    (*m_isa)["nop"] = std::make_shared<isa_op>("nop", OPCODE_NOP, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    });

    (*m_isa)["preempt"] = std::make_shared<isa_op>("preempt", OPCODE_PREEMPT, std::vector<opArg>{
     opArg("id", opArg::optype::CONST, BIT_WIDTH_16), opArg("save_control_code_offset", opArg::optype::PAGE_ID, BIT_WIDTH_16), opArg("restore_control_code_offset", opArg::optype::PAGE_ID, BIT_WIDTH_16),
    });

    (*m_isa)["load_pdi"] = std::make_shared<isa_op>("load_pdi", OPCODE_LOAD_PDI, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("pdi_id", opArg::optype::CONST, BIT_WIDTH_32), opArg("pdi_host_addr_offset", opArg::optype::PAGE_ID, BIT_WIDTH_16), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    });

    (*m_isa)["load_cores"] = std::make_shared<isa_op>("load_cores", OPCODE_LOAD_CORES, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("core_elf_id", opArg::optype::CONST, BIT_WIDTH_32), opArg("core_elf_host_addr_offset", opArg::optype::PAGE_ID, BIT_WIDTH_16), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    });

    (*m_isa)["load_last_pdi"] = std::make_shared<isa_op>("load_last_pdi", OPCODE_LOAD_LAST_PDI, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    });

    (*m_isa)["save_timestamps"] = std::make_shared<isa_op>("save_timestamps", OPCODE_SAVE_TIMESTAMPS, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("unq_id", opArg::optype::CONST, BIT_WIDTH_32),
    });

    (*m_isa)["sleep"] = std::make_shared<isa_op>("sleep", OPCODE_SLEEP, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("target", opArg::optype::CONST, BIT_WIDTH_32),
    });

    (*m_isa)["save_register"] = std::make_shared<isa_op>("save_register", OPCODE_SAVE_REGISTER, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("address", opArg::optype::CONST, BIT_WIDTH_32), opArg("unq_id", opArg::optype::CONST, BIT_WIDTH_32),
    });

    (*m_isa)[".align"] = std::make_shared<isa_op>(".align", OPCODE_ALIGN, std::vector<opArg>{});
    (*m_isa)[".long"] = std::make_shared<isa_op>(".long", 0/* dummy*/, std::vector<opArg>{});
    (*m_isa)["uc_dma_bd"] = std::make_shared<isa_op>("uc_dma_bd", 0/* dummy*/, std::vector<opArg>{});
    (*m_isa)["uc_dma_bd_shim"] = std::make_shared<isa_op>("uc_dma_bd_shim", 0/* dummy*/, std::vector<opArg>{});
  }

  std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> get_isamap()
  {
    return m_isa;
  }
};

class isa_disassembler
{
private:
  std::map<uint8_t, isa_op_disasm> m_isa_disasm;

public:
  isa_disassembler()
  {
    // All instruction entries by opcode - stack allocated
    m_isa_disasm.emplace(OPCODE_START_JOB, isa_op_disasm("start_job", OPCODE_START_JOB, std::vector<opArg>{
      opArg("job_id", opArg::optype::CONST, BIT_WIDTH_16), opArg("size", opArg::optype::JOBSIZE, BIT_WIDTH_16), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    }));

    m_isa_disasm.emplace(OPCODE_UC_DMA_WRITE_DES, isa_op_disasm("uc_dma_write_des", OPCODE_UC_DMA_WRITE_DES, std::vector<opArg>{
      opArg("wait_handle", opArg::optype::REG, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8), opArg("descriptor_ptr", opArg::optype::CONST, BIT_WIDTH_16), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    }));

    m_isa_disasm.emplace(OPCODE_WAIT_UC_DMA, isa_op_disasm("wait_uc_dma", OPCODE_WAIT_UC_DMA, std::vector<opArg>{
      opArg("wait_handle", opArg::optype::REG, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8),
    }));

    m_isa_disasm.emplace(OPCODE_MASK_WRITE_32, isa_op_disasm("mask_write_32", OPCODE_MASK_WRITE_32, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("address", opArg::optype::CONST, BIT_WIDTH_32), opArg("mask", opArg::optype::CONST, BIT_WIDTH_32), opArg("value", opArg::optype::CONST, BIT_WIDTH_32),
    }));

    m_isa_disasm.emplace(OPCODE_LOAD_CORES, isa_op_disasm("load_cores", OPCODE_LOAD_CORES, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("core_elf_id", opArg::optype::CONST, BIT_WIDTH_32), opArg("core_elf_host_addr_offset", opArg::optype::PAGE_ID, BIT_WIDTH_16), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    }));

    m_isa_disasm.emplace(OPCODE_WRITE_32, isa_op_disasm("write_32", OPCODE_WRITE_32, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("address", opArg::optype::CONST, BIT_WIDTH_32), opArg("value", opArg::optype::CONST, BIT_WIDTH_32),
    }));

    m_isa_disasm.emplace(OPCODE_WAIT_TCTS, isa_op_disasm("wait_tcts", OPCODE_WAIT_TCTS, std::vector<opArg>{
      opArg("tile_id", opArg::optype::CONST, BIT_WIDTH_16), opArg("actor_id", opArg::optype::CONST, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8), opArg("target_tcts", opArg::optype::CONST, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8),
    }));

    m_isa_disasm.emplace(OPCODE_END_JOB, isa_op_disasm("end_job", OPCODE_END_JOB, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    }));

    m_isa_disasm.emplace(OPCODE_YIELD, isa_op_disasm("yield", OPCODE_YIELD, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    }));

    m_isa_disasm.emplace(OPCODE_UC_DMA_WRITE_DES_SYNC, isa_op_disasm("uc_dma_write_des_sync", OPCODE_UC_DMA_WRITE_DES_SYNC, std::vector<opArg>{
      opArg("descriptor_ptr", opArg::optype::CONST, BIT_WIDTH_16),
    }));

    m_isa_disasm.emplace(OPCODE_WRITE_32_D, isa_op_disasm("write_32_d", OPCODE_WRITE_32_D, std::vector<opArg>{
      opArg("flags", opArg::optype::CONST, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8), opArg("address", opArg::optype::CONST, BIT_WIDTH_32), opArg("value", opArg::optype::CONST, BIT_WIDTH_32),
    }));

    m_isa_disasm.emplace(OPCODE_READ_32, isa_op_disasm("read_32", OPCODE_READ_32, std::vector<opArg>{
      opArg("value", opArg::optype::REG, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8), opArg("address", opArg::optype::CONST, BIT_WIDTH_32),
    }));

    m_isa_disasm.emplace(OPCODE_READ_32_D, isa_op_disasm("read_32_d", OPCODE_READ_32_D, std::vector<opArg>{
      opArg("address", opArg::optype::REG, BIT_WIDTH_8), opArg("value", opArg::optype::REG, BIT_WIDTH_8),
    }));

    m_isa_disasm.emplace(OPCODE_APPLY_OFFSET_57, isa_op_disasm("apply_offset_57", OPCODE_APPLY_OFFSET_57, std::vector<opArg>{
      opArg("table_ptr", opArg::optype::CONST, BIT_WIDTH_16), opArg("num_entries", opArg::optype::CONST, BIT_WIDTH_16), opArg("offset", opArg::optype::CONST, BIT_WIDTH_16),
    }));

    m_isa_disasm.emplace(OPCODE_ADD, isa_op_disasm("add", OPCODE_ADD, std::vector<opArg>{
      opArg("dest", opArg::optype::REG, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8), opArg("value", opArg::optype::CONST, BIT_WIDTH_32),
    }));

    m_isa_disasm.emplace(OPCODE_MOV, isa_op_disasm("mov", OPCODE_MOV, std::vector<opArg>{
      opArg("dest", opArg::optype::REG, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8), opArg("value", opArg::optype::CONST, BIT_WIDTH_32),
    }));

    m_isa_disasm.emplace(OPCODE_LOCAL_BARRIER, isa_op_disasm("local_barrier", OPCODE_LOCAL_BARRIER, std::vector<opArg>{
      opArg("local_barrier_id", opArg::optype::BARRIER, BIT_WIDTH_8), opArg("num_participants", opArg::optype::CONST, BIT_WIDTH_8),
    }));

    m_isa_disasm.emplace(OPCODE_REMOTE_BARRIER, isa_op_disasm("remote_barrier", OPCODE_REMOTE_BARRIER, std::vector<opArg>{
      opArg("remote_barrier_id", opArg::optype::BARRIER, BIT_WIDTH_8), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_8), opArg("party_mask", opArg::optype::CONST, BIT_WIDTH_32),
    }));

    m_isa_disasm.emplace(OPCODE_POLL_32, isa_op_disasm("poll_32", OPCODE_POLL_32, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("address", opArg::optype::CONST, BIT_WIDTH_32), opArg("value", opArg::optype::CONST, BIT_WIDTH_32),
    }));

    m_isa_disasm.emplace(OPCODE_MASK_POLL_32, isa_op_disasm("mask_poll_32", OPCODE_MASK_POLL_32, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("address", opArg::optype::CONST, BIT_WIDTH_32), opArg("mask", opArg::optype::CONST, BIT_WIDTH_32), opArg("value", opArg::optype::CONST, BIT_WIDTH_32),
    }));

    m_isa_disasm.emplace(OPCODE_TRACE, isa_op_disasm("trace", OPCODE_TRACE, std::vector<opArg>{
      opArg("info", opArg::optype::CONST, BIT_WIDTH_16),
    }));

    m_isa_disasm.emplace(OPCODE_NOP, isa_op_disasm("nop", OPCODE_NOP, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    }));

    m_isa_disasm.emplace(OPCODE_START_JOB_DEFERRED, isa_op_disasm("start_job_deferred", OPCODE_START_JOB_DEFERRED, std::vector<opArg>{
      opArg("job_id", opArg::optype::CONST, BIT_WIDTH_16), opArg("size", opArg::optype::JOBSIZE, BIT_WIDTH_16), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    }));

    m_isa_disasm.emplace(OPCODE_LAUNCH_JOB, isa_op_disasm("launch_job", OPCODE_LAUNCH_JOB, std::vector<opArg>{
      opArg("job_id", opArg::optype::CONST, BIT_WIDTH_16),
    }));

    m_isa_disasm.emplace(OPCODE_PREEMPT, isa_op_disasm("preempt", OPCODE_PREEMPT, std::vector<opArg>{
      opArg("id", opArg::optype::CONST, BIT_WIDTH_16), opArg("save_control_code_offset", opArg::optype::PAGE_ID, BIT_WIDTH_16), opArg("restore_control_code_offset", opArg::optype::PAGE_ID, BIT_WIDTH_16),
    }));

    m_isa_disasm.emplace(OPCODE_LOAD_PDI, isa_op_disasm("load_pdi", OPCODE_LOAD_PDI, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("pdi_id", opArg::optype::CONST, BIT_WIDTH_32), opArg("pdi_host_addr_offset", opArg::optype::PAGE_ID, BIT_WIDTH_16), opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    }));

    m_isa_disasm.emplace(OPCODE_LOAD_LAST_PDI, isa_op_disasm("load_last_pdi", OPCODE_LOAD_LAST_PDI, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    }));

    m_isa_disasm.emplace(OPCODE_SAVE_TIMESTAMPS, isa_op_disasm("save_timestamps", OPCODE_SAVE_TIMESTAMPS, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("unq_id", opArg::optype::CONST, BIT_WIDTH_32),
    }));

    m_isa_disasm.emplace(OPCODE_SLEEP, isa_op_disasm("sleep", OPCODE_SLEEP, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("target", opArg::optype::CONST, BIT_WIDTH_32),
    }));

    m_isa_disasm.emplace(OPCODE_SAVE_REGISTER, isa_op_disasm("save_register", OPCODE_SAVE_REGISTER, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16), opArg("address", opArg::optype::CONST, BIT_WIDTH_32), opArg("unq_id", opArg::optype::CONST, BIT_WIDTH_32),
    }));

    m_isa_disasm.emplace(OPCODE_EOF, isa_op_disasm("eof", OPCODE_EOF, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, BIT_WIDTH_16),
    }));

    // Pseudo-instructions
    m_isa_disasm.emplace(OPCODE_ALIGN, isa_op_disasm(".align", OPCODE_ALIGN, std::vector<opArg>{}));
  }

  const std::map<uint8_t, isa_op_disasm>* get_isa_map() const
  {
    return &m_isa_disasm;  // Return const pointer to owned data
  }
};

} // namespace aiebu
#endif // _ISA_ASSEMBLER_STUBS_H_
