// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

#include <iostream>
#include <ostream>

#include "basicpass.h"
#include "passmanager.h"
#include "aie2p_passes.h"

#include "code_section.h"
#include "aiebu/aiebu_error.h"

#include "xaiengine.h"

namespace aiebu {

std::list<basic_node<XAie_OpHdr>>
itemize(const ELFIO::section *buffer) {
  const char *ptr = buffer->get_data();
  auto Hdr = (const XAie_TxnHeader *)(ptr);
  const auto num_ops = Hdr->NumOps;
  std::list<basic_node<XAie_OpHdr>> nodes;

  if ((Hdr->Major == AIE2P_OPT_MAJOR_VER) &&
      (Hdr->Minor == AIE2P_OPT_MINOR_VER)) {
    // Only legacy transaction binary format supported today
    return nodes;
  }

  ptr += sizeof(*Hdr);
  for (auto i = 0U; i < num_ops; i++) {
    auto op_hdr = (const XAie_OpHdr *)(ptr);
    size_t size = 0;
    switch (op_hdr->Op) {
    case XAIE_IO_WRITE: {
      auto w_hdr = (const XAie_Write32Hdr *)(ptr);
      size = w_hdr->Size;
      break;
    }
    case XAIE_IO_BLOCKWRITE: {
      auto bw_header = (const XAie_BlockWrite32Hdr *)(ptr);
      size = bw_header->Size;
      break;
    }
    case XAIE_IO_MASKWRITE: {
      auto mw_header = (const XAie_MaskWrite32Hdr *)(ptr);
      size = mw_header->Size;
      break;
    }
    case XAIE_IO_MASKPOLL:
    case XAIE_IO_MASKPOLL_BUSY: {
      auto mp_header = (const XAie_MaskPoll32Hdr *)(ptr);
      size = mp_header->Size;
      break;
    }
    case XAIE_IO_NOOP: {
      size = sizeof(XAie_NoOpHdr);
      break;
    }
    case XAIE_IO_PREEMPT: {
      size = sizeof(XAie_PreemptHdr);
      break;
    }
    case XAIE_IO_LOADPDI: {
      size = sizeof(XAie_LoadPdiHdr);
      break;
    }
    case XAIE_IO_LOAD_PM_START: {
      size = sizeof(XAie_PmLoadHdr);
      break;
    }
    case (XAIE_IO_CUSTOM_OP_TCT):
    case (XAIE_IO_CUSTOM_OP_DDR_PATCH):
    case (XAIE_IO_CUSTOM_OP_READ_REGS):
    case (XAIE_IO_CUSTOM_OP_RECORD_TIMER):
    case (XAIE_IO_CUSTOM_OP_MERGE_SYNC): {
      auto Hdr = (const XAie_CustomOpHdr *)(ptr);
      size = Hdr->Size;
      break;
    }
    default:
      throw error(error::error_code::invalid_opcode, std::to_string(op_hdr->Op) + "\n");
      break;
    }
    nodes.emplace_back(op_hdr, size);
    ptr += size;
  }
  return nodes;
}

void serialize_nodes(ELFIO::section *psec,
                     const std::list<basic_node<XAie_OpHdr>> &nodes) {
  std::stringstream store;
  const char *ptr = psec->get_data();
  XAie_TxnHeader hdr;
  std::memcpy(&hdr, ptr, sizeof(hdr));
  hdr.NumOps = nodes.size();
  store.write(reinterpret_cast<const char *>(&hdr), sizeof(hdr));
  for (auto & node : nodes) {
    store.write(reinterpret_cast<const char *>(node.m_op), node.m_size);
  }
  store.seekp(0, std::ios_base::end);
  hdr.TxnSize = store.tellp();
  store.seekp(0);
  store.write(reinterpret_cast<const char *>(&hdr), sizeof(hdr));
  psec->free_data();
  psec->set_data(store.str());
}



void passmanager::adjust_relocations() {
  // TBD
  // We should iterate over patch op nodes and update the original relocation entries in the ELF
}

void passmanager::run_transforms(ELFIO::section *psec) {
  std::list<basic_node<XAie_OpHdr>> nodes = itemize(psec);
  if (!nodes.size())
    return;
  // Example transform sequence for what should really be driven by user request via a JSON file
  if (m_debug) {
    XAie_OpHdr_print printer1(nodes, std::cout);
    printer1.transform();
  }

  XAie_OpHdr_drop_preempt nopreempt(nodes);
  nopreempt.transform();

  if (m_debug) {
    XAie_OpHdr_print printer2(nodes, std::cout);
    printer2.transform();
  }

  // The loadpdi opcode details like id, size, etc. should come from the caller
  // The code here is merely representative of what plumbing should be done
  XAie_OpHdr_add_loadpdi loadpdi(nodes, 0x10, 0x800);
  loadpdi.transform();
  serialize_nodes(psec, nodes);
}

}
