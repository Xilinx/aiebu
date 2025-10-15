// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

#include <iomanip>
#include <iostream>
#include <ostream>

#include "basicpass.h"
#include "passmanager.h"

#include "utils.h"
#include "code_section.h"
#include "aiebu/aiebu_error.h"

#include "xaiengine.h"

namespace aiebu {

// All the stringify code below has been copied from transaction.cpp and uses
// legacy C++ coding style

constexpr unsigned int field_width = 32;

inline std::ostream& op_format(std::ostream& strm) {
  strm << std::setw(field_width) << std::left;
  return strm;
}

inline std::ostream& dec_format(std::ostream& strm) {
  strm << std::setw(field_width/2) << std::right;
  return strm;
}

const std::array<std::string_view, 5> preempt_code_table{"#NOOP",
                                                         "#MEM_TILE",
                                                         "#AIE_TILE",
                                                         "#AIE_REGISTERS",
                                                         "#INVALID"};

size_t stringify_w32(const XAie_OpHdr *ptr, std::ostream &ss_ops_) {
  auto w_hdr = (const XAie_Write32Hdr *)(ptr);
  ss_ops_ << op_format << "XAIE_IO_WRITE " << "@0x" << std::hex << w_hdr->RegOff << ", 0x" << w_hdr->Value
          << std::endl;
  return w_hdr->Size;
}

size_t stringify_bw32(const XAie_OpHdr *ptr, std::ostream &ss_ops_) {
  auto bw_header = (const XAie_BlockWrite32Hdr *)(ptr);
  u32 bw_size = bw_header->Size;
  u32 Size = (bw_size - sizeof(*bw_header)) / 4;
  const char *curr = (const char *)ptr;
  curr += sizeof(*bw_header);
  u32 *Payload = (u32 *)curr;
  ss_ops_ << op_format << "XAIE_IO_BLOCKWRITE " << "@0x" << std::hex << bw_header->RegOff << ", [" <<
    std::dec << Size << "]" << std::endl;
  for (u32 i = 0; i < Size; i++) {
    ss_ops_ << op_format << ("XAIE_IO_BLOCKWRITE." + std::to_string(i)) << "0x" << std::hex << *Payload << std::endl;
    Payload++;
  }
  return bw_size;
}

size_t stringify_mw32(const XAie_OpHdr *ptr, std::ostream &ss_ops_) {
  auto mw_header = (const XAie_MaskWrite32Hdr *)(ptr);
  ss_ops_ << op_format << "XAIE_IO_MASKWRITE " << "@0x" << std::hex << mw_header->RegOff << ", 0x" << mw_header->Mask
          << "(), 0x" << mw_header->Value << std::endl;
  return mw_header->Size;
}

size_t stringify_mp32(const XAie_OpHdr *ptr, std::ostream &ss_ops_) {
  auto mp_header = (const XAie_MaskPoll32Hdr *)(ptr);
  ss_ops_ << op_format << "XAIE_IO_MASKPOLL " << "@0x" << std::hex << mp_header->RegOff << ", 0x" << mp_header->Mask
          << "()==0x" << mp_header->Value << std::endl;
  return mp_header->Size;
}

size_t stringify_mp32_busy(const XAie_OpHdr *ptr, std::ostream &ss_ops_) {
  auto mp_header = (const XAie_MaskPoll32Hdr *)(ptr);
  ss_ops_ << op_format << "XAIE_IO_MASKPOLL_BUSY " << "@0x" << std::hex << mp_header->RegOff << ", 0x" << mp_header->Mask
          << "()==0x" << mp_header->Value << std::endl;
  return mp_header->Size;
}

size_t stringify_noop(const XAie_OpHdr *ptr, std::ostream &ss_ops_) {
  (void)ptr;
  ss_ops_ << op_format << "XAIE_IO_NOOP " << std::endl;
  return sizeof(XAie_NoOpHdr);
}

size_t stringify_preempt(const XAie_OpHdr *ptr, std::ostream &ss_ops_) {
  auto mp_header = (const XAie_PreemptHdr *)(ptr);
  ss_ops_ << op_format << "XAIE_IO_PREEMPT " << preempt_code_table[mp_header->Preempt_level] << std::endl;
  return sizeof(XAie_PreemptHdr);
}

size_t stringify_loadpdi(const XAie_OpHdr *ptr, std::ostream &ss_ops_) {
  auto mp_header = (const XAie_LoadPdiHdr *)(ptr);
  ss_ops_ << op_format << "XAIE_IO_LOADPDI " << "#" <<  std::dec << mp_header->PdiId << ", 0x" <<
    mp_header->PdiSize << "0x" << mp_header->PdiAddress << std::endl;
  return sizeof(XAie_LoadPdiHdr);
}

size_t stringify_pmload(const XAie_OpHdr *ptr, std::ostream &ss_ops_) {
  auto mp_header = (const XAie_PmLoadHdr *)(ptr);
  uint32_t loadsequence = mp_header->LoadSequenceCount[2] << 16 | mp_header->LoadSequenceCount[1] << 8 | mp_header->LoadSequenceCount[0];
  ss_ops_ << op_format << "XAIE_IO_LOAD_PM_START " << "0x" <<  std::hex << loadsequence << std::dec << ", #" << mp_header->PmLoadId << std::endl;
  return sizeof(XAie_PmLoadHdr);
}

size_t stringify_tct(const XAie_OpHdr *ptr, std::ostream &ss_ops_) {
  auto hdr = (const XAie_CustomOpHdr *)(ptr);
  const char *curr = (const char *)ptr;
  curr += sizeof(*hdr);
  auto op = (const tct_op_t *)curr;
  unsigned int column = get_byte<2>(op->word);
  unsigned int row = get_byte<1>(op->word);
  const char *dir = get_byte<0>(op->word) ? "#DMA_MM2S" : "#DMA_S2MM";
  unsigned int channel = get_byte<3>(op->config);
  unsigned int column_count = get_byte<2>(op->config);
  unsigned int row_count = get_byte<1>(op->config);
  ss_ops_ << op_format << "XAIE_IO_CUSTOM_OP_TCT " << "R[" << std::dec
          << row << "]+" << row_count << ", C[" << std::dec << column
          << "]+" << column_count << ", " << dir << ", " << channel << std::endl;
  return hdr->Size;
}

size_t stringify_patchop(const XAie_OpHdr *ptr, std::ostream &ss_ops_) {
  auto hdr = (const XAie_CustomOpHdr *)(ptr);
  u32 size = hdr->Size;
  ss_ops_ << op_format << "XAIE_IO_CUSTOM_OP_DDR_PATCH ";
  const char *curr = (const char *)ptr;
  curr += sizeof(*hdr);
  auto op = (const patch_op_t *)curr;
  auto reg_off = op->regaddr;
  auto arg_idx = op->argidx;
  auto addr_offset = op->argplus;
  ss_ops_ << "@0x" << std::hex << reg_off << std::dec << ", #" << arg_idx
          << std::hex << ", +0x" << addr_offset << std::endl;
  return size;
}

size_t stringify_rdreg(const XAie_OpHdr *ptr, std::ostream &ss_ops_) {
  auto Hdr = (const XAie_CustomOpHdr *)(ptr);
  u32 size = Hdr->Size;
  ss_ops_ << "ReadOp: " << std::endl;
  return size;
}

size_t stringify_rectimer(const XAie_OpHdr *ptr, std::ostream &ss_ops_) {
  auto Hdr = (const XAie_CustomOpHdr *)(ptr);
  u32 size = Hdr->Size;
  const char *curr = (const char *)ptr;
  curr += sizeof(*Hdr);
  auto id = (unsigned int *)curr;
  ss_ops_ << op_format << "XAIE_IO_CUSTOM_OP_RECORD_TIMER " << '#' << std::dec << *id << std::endl;
  return size;
}

size_t stringify_merge_sync(const XAie_OpHdr *ptr, std::ostream &ss_ops_) {
  auto Hdr = (const XAie_CustomOpHdr *)(ptr);
  const char *curr = (const char *)ptr;
  curr += sizeof(*Hdr);
  auto op = (const tct_op_t *)curr;
  unsigned int tokens = get_byte<0>(op->word);
  unsigned int column = get_byte<1>(op->word);
  ss_ops_ << op_format << "XAIE_IO_CUSTOM_OP_MERGE_SYNC " << "C[" << std::dec << column << "]==" << std::dec << tokens << std::endl;
  return Hdr->Size;
}

class XAie_OpHdr_print : public aie2p_basicpass<XAie_OpHdr> {
private:
  std::ostream &m_stream;
  std::list<basic_node<XAie_OpHdr>> &m_nodes;

public:
  XAie_OpHdr_print(std::list<basic_node<XAie_OpHdr>> &nodes, std::ostream &stream)
    : m_stream(stream), m_nodes(nodes) {}

  void transform() override {
    for (const auto &node : m_nodes) {
      switch (node.m_op->Op) {
      case XAIE_IO_WRITE:
        stringify_w32(node.m_op, m_stream);
        break;
      case XAIE_IO_BLOCKWRITE:
        stringify_bw32(node.m_op, m_stream);
        break;
      case XAIE_IO_MASKWRITE:
        stringify_mw32(node.m_op, m_stream);
        break;
      case XAIE_IO_MASKPOLL:
        stringify_mp32(node.m_op, m_stream);
        break;
      case XAIE_IO_MASKPOLL_BUSY:
        stringify_mp32_busy(node.m_op, m_stream);
        break;
      case XAIE_IO_NOOP:
        stringify_noop(node.m_op, m_stream);
        break;
      case XAIE_IO_PREEMPT:
        stringify_preempt(node.m_op, m_stream);
        break;
      case XAIE_IO_LOADPDI:
        stringify_loadpdi(node.m_op, m_stream);
        break;
      case XAIE_IO_LOAD_PM_START:
        stringify_pmload(node.m_op, m_stream);
        break;
      case XAIE_IO_CUSTOM_OP_TCT:
        stringify_tct(node.m_op, m_stream);
        break;
      case XAIE_IO_CUSTOM_OP_DDR_PATCH:
        stringify_patchop(node.m_op, m_stream);
        break;
      case XAIE_IO_CUSTOM_OP_READ_REGS:
        stringify_rdreg(node.m_op, m_stream);
        break;
      case XAIE_IO_CUSTOM_OP_RECORD_TIMER:
        stringify_rectimer(node.m_op, m_stream);
        break;
      case XAIE_IO_CUSTOM_OP_MERGE_SYNC:
        stringify_merge_sync(node.m_op, m_stream);
        break;
      default:
        throw error(error::error_code::invalid_opcode,
                    std::to_string(node.m_op->Op) + "\n");
        break;
      }
    }
  }
};

class XAie_OpHdr_drop_preempt : public aie2p_basicpass<XAie_OpHdr> {
private:
  std::list<basic_node<XAie_OpHdr>> &m_nodes;

public:
  explicit XAie_OpHdr_drop_preempt(std::list<basic_node<XAie_OpHdr>> &nodes)
    : m_nodes(nodes)  {}

  void transform() override {
    for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
      switch (it->m_op->Op) {
      case XAIE_IO_PREEMPT:
      {
        it->m_state = basic_node_state::dropped;
        it = m_nodes.erase(it);
        auto noop = new XAie_NoOpHdr();
        noop->Op = XAIE_IO_NOOP;
        m_nodes.emplace(it, reinterpret_cast<const XAie_OpHdr *>(noop),
                        sizeof(XAie_NoOpHdr), basic_node_state::added);
        break;
      }
      default:
        break;
      }
    }
  }
};

class XAie_OpHdr_add_loadpdi : public aie2p_basicpass<XAie_OpHdr> {
private:
  std::list<basic_node<XAie_OpHdr>> &m_nodes;
  uint16_t m_pdiid;
  uint64_t m_pdisize;

public:
  explicit XAie_OpHdr_add_loadpdi(std::list<basic_node<XAie_OpHdr>> &nodes, uint16_t pdiid, uint64_t pdisize)
    : m_nodes(nodes), m_pdiid(pdiid), m_pdisize(pdisize) {}

  void transform() override {
    auto load = new XAie_LoadPdiHdr();
    load->Op = XAIE_IO_LOADPDI;
    load->PdiId = m_pdiid;
    load->PdiSize = m_pdisize;
    m_nodes.emplace(m_nodes.begin(),
                    reinterpret_cast<const XAie_OpHdr *>(load),
                    sizeof(XAie_LoadPdiHdr), basic_node_state::added);
  }
};


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
