// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef _ISA_ASSEMBLER_STUBS_H_
#define _ISA_ASSEMBLER_STUBS_H_

#include "oparg.h"
#include "ops.h"

namespace aiebu {

class isa
{
private:
  std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> m_isa;

public:
  isa()
  {
    m_isa = std::make_shared<std::map<std::string, std::shared_ptr<isa_op>>>();

    (*m_isa)["start_job"] = std::make_shared<isa_op>("start_job", 0, std::vector<opArg>{
     opArg("job_id", opArg::optype::CONST, 16), opArg("size", opArg::optype::JOBSIZE, 16), opArg("_pad", opArg::optype::PAD, 16),
    });

    (*m_isa)["start_job_deferred"] = std::make_shared<isa_op>("start_job_deferred", 23, std::vector<opArg>{
     opArg("job_id", opArg::optype::CONST, 16), opArg("size", opArg::optype::JOBSIZE, 16), opArg("_pad", opArg::optype::PAD, 16),
    });

    (*m_isa)["launch_job"] = std::make_shared<isa_op>("launch_job", 24, std::vector<opArg>{
     opArg("job_id", opArg::optype::CONST, 16),
    });

    (*m_isa)["uc_dma_write_des"] = std::make_shared<isa_op>("uc_dma_write_des", 1, std::vector<opArg>{
     opArg("wait_handle", opArg::optype::REG, 8), opArg("_pad", opArg::optype::PAD, 8), opArg("descriptor_ptr", opArg::optype::CONST, 16), opArg("_pad", opArg::optype::PAD, 16),
    });

    (*m_isa)["wait_uc_dma"] = std::make_shared<isa_op>("wait_uc_dma", 2, std::vector<opArg>{
     opArg("wait_handle", opArg::optype::REG, 8), opArg("_pad", opArg::optype::PAD, 8),
    });

    (*m_isa)["mask_write_32"] = std::make_shared<isa_op>("mask_write_32", 3, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, 16), opArg("address", opArg::optype::CONST, 32), opArg("mask", opArg::optype::CONST, 32), opArg("value", opArg::optype::CONST, 32),
    });

    (*m_isa)["write_32"] = std::make_shared<isa_op>("write_32", 5, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, 16), opArg("address", opArg::optype::CONST, 32), opArg("value", opArg::optype::CONST, 32),
    });

    (*m_isa)["wait_tcts"] = std::make_shared<isa_op>("wait_tcts", 6, std::vector<opArg>{
     opArg("tile_id", opArg::optype::CONST, 16), opArg("actor_id", opArg::optype::CONST, 8), opArg("_pad", opArg::optype::PAD, 8), opArg("target_tcts", opArg::optype::CONST, 8), opArg("_pad", opArg::optype::PAD, 8),
    });

    (*m_isa)["end_job"] = std::make_shared<isa_op>("end_job", 7, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, 16),
    });

    (*m_isa)["yield"] = std::make_shared<isa_op>("yield", 8, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, 16),
    });

    (*m_isa)["uc_dma_write_des_sync"] = std::make_shared<isa_op>("uc_dma_write_des_sync", 9, std::vector<opArg>{
     opArg("descriptor_ptr", opArg::optype::CONST, 16),
    });

    (*m_isa)["write_32_d"] = std::make_shared<isa_op>("write_32_d", 11, std::vector<opArg>{
     opArg("flags", opArg::optype::CONST, 8), opArg("_pad", opArg::optype::PAD, 8), opArg("address", opArg::optype::CONST, 32), opArg("value", opArg::optype::CONST, 32),
    });

    (*m_isa)["read_32"] = std::make_shared<isa_op>("read_32", 12, std::vector<opArg>{
     opArg("value", opArg::optype::REG, 8), opArg("_pad", opArg::optype::PAD, 8), opArg("address", opArg::optype::CONST, 32),
    });

    (*m_isa)["read_32_d"] = std::make_shared<isa_op>("read_32_d", 13, std::vector<opArg>{
     opArg("address", opArg::optype::REG, 8), opArg("value", opArg::optype::REG, 8),
    });

    (*m_isa)["apply_offset_57"] = std::make_shared<isa_op>("apply_offset_57", 14, std::vector<opArg>{
     opArg("table_ptr", opArg::optype::CONST, 16), opArg("num_entries", opArg::optype::CONST, 16), opArg("offset", opArg::optype::CONST, 16),
    });

    (*m_isa)["add"] = std::make_shared<isa_op>("add", 15, std::vector<opArg>{
     opArg("dest", opArg::optype::REG, 8), opArg("_pad", opArg::optype::PAD, 8), opArg("value", opArg::optype::CONST, 32),
    });

    (*m_isa)["mov"] = std::make_shared<isa_op>("mov", 16, std::vector<opArg>{
     opArg("dest", opArg::optype::REG, 8), opArg("_pad", opArg::optype::PAD, 8), opArg("value", opArg::optype::CONST, 32),
    });

    (*m_isa)["local_barrier"] = std::make_shared<isa_op>("local_barrier", 17, std::vector<opArg>{
     opArg("local_barrier_id", opArg::optype::BARRIER, 8), opArg("num_participants", opArg::optype::CONST, 8),
    });

    (*m_isa)["remote_barrier"] = std::make_shared<isa_op>("remote_barrier", 18, std::vector<opArg>{
     opArg("remote_barrier_id", opArg::optype::BARRIER, 8), opArg("_pad", opArg::optype::PAD, 8), opArg("party_mask", opArg::optype::CONST, 32),
    });

    (*m_isa)["eof"] = std::make_shared<isa_op>("eof", 255, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, 16),
    });

    (*m_isa)["poll_32"] = std::make_shared<isa_op>("poll_32", 19, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, 16), opArg("address", opArg::optype::CONST, 32), opArg("value", opArg::optype::CONST, 32),
    });

    (*m_isa)["mask_poll_32"] = std::make_shared<isa_op>("mask_poll_32", 20, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, 16), opArg("address", opArg::optype::CONST, 32), opArg("mask", opArg::optype::CONST, 32), opArg("value", opArg::optype::CONST, 32),
    });

    (*m_isa)["trace"] = std::make_shared<isa_op>("trace", 21, std::vector<opArg>{
     opArg("info", opArg::optype::CONST, 16),
    });

    (*m_isa)["nop"] = std::make_shared<isa_op>("nop", 22, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, 16),
    });

    (*m_isa)["preempt"] = std::make_shared<isa_op>("preempt", 25, std::vector<opArg>{
     opArg("id", opArg::optype::CONST, 16), opArg("save_control_code_offset", opArg::optype::PAGE_ID, 16), opArg("restore_control_code_offset", opArg::optype::PAGE_ID, 16),
    });

    (*m_isa)["load_pdi"] = std::make_shared<isa_op>("load_pdi", 26, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, 16), opArg("pdi_id", opArg::optype::CONST, 32), opArg("pdi_host_addr_offset", opArg::optype::PAGE_ID, 16), opArg("_pad", opArg::optype::PAD, 16),
    });

    (*m_isa)["load_cores"] = std::make_shared<isa_op>("load_cores", 4, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, 16), opArg("core_elf_id", opArg::optype::CONST, 32), opArg("core_elf_host_addr_offset", opArg::optype::PAGE_ID, 16), opArg("_pad", opArg::optype::PAD, 16),
    });

    (*m_isa)["load_last_pdi"] = std::make_shared<isa_op>("load_last_pdi", 27, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, 16),
    });

    (*m_isa)["save_timestamps"] = std::make_shared<isa_op>("save_timestamps", 28, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, 16), opArg("unq_id", opArg::optype::CONST, 32),
    });

    (*m_isa)["sleep"] = std::make_shared<isa_op>("sleep", 29, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, 16), opArg("target", opArg::optype::CONST, 32),
    });

    (*m_isa)["save_register"] = std::make_shared<isa_op>("save_register", 30, std::vector<opArg>{
     opArg("_pad", opArg::optype::PAD, 16), opArg("address", opArg::optype::CONST, 32), opArg("unq_id", opArg::optype::CONST, 32),
    });

    

    (*m_isa)[".align"] = std::make_shared<isa_op>(".align", 0XA5, std::vector<opArg>{});
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
    m_isa_disasm.emplace(0, isa_op_disasm("start_job", 0, std::vector<opArg>{
      opArg("job_id", opArg::optype::CONST, 16), opArg("size", opArg::optype::JOBSIZE, 16), opArg("_pad", opArg::optype::PAD, 16),
    }));

    m_isa_disasm.emplace(1, isa_op_disasm("uc_dma_write_des", 1, std::vector<opArg>{
      opArg("wait_handle", opArg::optype::REG, 8), opArg("_pad", opArg::optype::PAD, 8), opArg("descriptor_ptr", opArg::optype::CONST, 16), opArg("_pad", opArg::optype::PAD, 16),
    }));

    m_isa_disasm.emplace(2, isa_op_disasm("wait_uc_dma", 2, std::vector<opArg>{
      opArg("wait_handle", opArg::optype::REG, 8), opArg("_pad", opArg::optype::PAD, 8),
    }));

    m_isa_disasm.emplace(3, isa_op_disasm("mask_write_32", 3, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, 16), opArg("address", opArg::optype::CONST, 32), opArg("mask", opArg::optype::CONST, 32), opArg("value", opArg::optype::CONST, 32),
    }));

    m_isa_disasm.emplace(4, isa_op_disasm("load_cores", 4, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, 16), opArg("core_elf_id", opArg::optype::CONST, 32), opArg("core_elf_host_addr_offset", opArg::optype::PAGE_ID, 16), opArg("_pad", opArg::optype::PAD, 16),
    }));

    m_isa_disasm.emplace(5, isa_op_disasm("write_32", 5, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, 16), opArg("address", opArg::optype::CONST, 32), opArg("value", opArg::optype::CONST, 32),
    }));

    m_isa_disasm.emplace(6, isa_op_disasm("wait_tcts", 6, std::vector<opArg>{
      opArg("tile_id", opArg::optype::CONST, 16), opArg("actor_id", opArg::optype::CONST, 8), opArg("_pad", opArg::optype::PAD, 8), opArg("target_tcts", opArg::optype::CONST, 8), opArg("_pad", opArg::optype::PAD, 8),
    }));

    m_isa_disasm.emplace(7, isa_op_disasm("end_job", 7, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, 16),
    }));

    m_isa_disasm.emplace(8, isa_op_disasm("yield", 8, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, 16),
    }));

    m_isa_disasm.emplace(9, isa_op_disasm("uc_dma_write_des_sync", 9, std::vector<opArg>{
      opArg("descriptor_ptr", opArg::optype::CONST, 16),
    }));

    m_isa_disasm.emplace(11, isa_op_disasm("write_32_d", 11, std::vector<opArg>{
      opArg("flags", opArg::optype::CONST, 8), opArg("_pad", opArg::optype::PAD, 8), opArg("address", opArg::optype::CONST, 32), opArg("value", opArg::optype::CONST, 32),
    }));

    m_isa_disasm.emplace(12, isa_op_disasm("read_32", 12, std::vector<opArg>{
      opArg("value", opArg::optype::REG, 8), opArg("_pad", opArg::optype::PAD, 8), opArg("address", opArg::optype::CONST, 32),
    }));

    m_isa_disasm.emplace(13, isa_op_disasm("read_32_d", 13, std::vector<opArg>{
      opArg("address", opArg::optype::REG, 8), opArg("value", opArg::optype::REG, 8),
    }));

    m_isa_disasm.emplace(14, isa_op_disasm("apply_offset_57", 14, std::vector<opArg>{
      opArg("table_ptr", opArg::optype::CONST, 16), opArg("num_entries", opArg::optype::CONST, 16), opArg("offset", opArg::optype::CONST, 16),
    }));

    m_isa_disasm.emplace(15, isa_op_disasm("add", 15, std::vector<opArg>{
      opArg("dest", opArg::optype::REG, 8), opArg("_pad", opArg::optype::PAD, 8), opArg("value", opArg::optype::CONST, 32),
    }));

    m_isa_disasm.emplace(16, isa_op_disasm("mov", 16, std::vector<opArg>{
      opArg("dest", opArg::optype::REG, 8), opArg("_pad", opArg::optype::PAD, 8), opArg("value", opArg::optype::CONST, 32),
    }));

    m_isa_disasm.emplace(17, isa_op_disasm("local_barrier", 17, std::vector<opArg>{
      opArg("local_barrier_id", opArg::optype::BARRIER, 8), opArg("num_participants", opArg::optype::CONST, 8),
    }));

    m_isa_disasm.emplace(18, isa_op_disasm("remote_barrier", 18, std::vector<opArg>{
      opArg("remote_barrier_id", opArg::optype::BARRIER, 8), opArg("_pad", opArg::optype::PAD, 8), opArg("party_mask", opArg::optype::CONST, 32),
    }));

    m_isa_disasm.emplace(19, isa_op_disasm("poll_32", 19, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, 16), opArg("address", opArg::optype::CONST, 32), opArg("value", opArg::optype::CONST, 32),
    }));

    m_isa_disasm.emplace(20, isa_op_disasm("mask_poll_32", 20, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, 16), opArg("address", opArg::optype::CONST, 32), opArg("mask", opArg::optype::CONST, 32), opArg("value", opArg::optype::CONST, 32),
    }));

    m_isa_disasm.emplace(21, isa_op_disasm("trace", 21, std::vector<opArg>{
      opArg("info", opArg::optype::CONST, 16),
    }));

    m_isa_disasm.emplace(22, isa_op_disasm("nop", 22, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, 16),
    }));

    m_isa_disasm.emplace(23, isa_op_disasm("start_job_deferred", 23, std::vector<opArg>{
      opArg("job_id", opArg::optype::CONST, 16), opArg("size", opArg::optype::JOBSIZE, 16), opArg("_pad", opArg::optype::PAD, 16),
    }));

    m_isa_disasm.emplace(24, isa_op_disasm("launch_job", 24, std::vector<opArg>{
      opArg("job_id", opArg::optype::CONST, 16),
    }));

    m_isa_disasm.emplace(25, isa_op_disasm("preempt", 25, std::vector<opArg>{
      opArg("id", opArg::optype::CONST, 16), opArg("save_control_code_offset", opArg::optype::PAGE_ID, 16), opArg("restore_control_code_offset", opArg::optype::PAGE_ID, 16),
    }));

    m_isa_disasm.emplace(26, isa_op_disasm("load_pdi", 26, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, 16), opArg("pdi_id", opArg::optype::CONST, 32), opArg("pdi_host_addr_offset", opArg::optype::PAGE_ID, 16), opArg("_pad", opArg::optype::PAD, 16),
    }));

    m_isa_disasm.emplace(27, isa_op_disasm("load_last_pdi", 27, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, 16),
    }));

    m_isa_disasm.emplace(28, isa_op_disasm("save_timestamps", 28, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, 16), opArg("unq_id", opArg::optype::CONST, 32),
    }));

    m_isa_disasm.emplace(29, isa_op_disasm("sleep", 29, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, 16), opArg("target", opArg::optype::CONST, 32),
    }));

    m_isa_disasm.emplace(30, isa_op_disasm("save_register", 30, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, 16), opArg("address", opArg::optype::CONST, 32), opArg("unq_id", opArg::optype::CONST, 32),
    }));

    m_isa_disasm.emplace(255, isa_op_disasm("eof", 255, std::vector<opArg>{
      opArg("_pad", opArg::optype::PAD, 16),
    }));

    // Pseudo-instructions
    m_isa_disasm.emplace(0xA5, isa_op_disasm(".align", 0XA5, std::vector<opArg>{}));
  }

  const std::map<uint8_t, isa_op_disasm>* get_isa_map() const
  {
    return &m_isa_disasm;  // Return const pointer to owned data
  }
};

} // namespace aiebu
#endif // _ISA_ASSEMBLER_STUBS_H_