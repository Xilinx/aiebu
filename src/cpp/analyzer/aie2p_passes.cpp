// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include <iomanip>

#include "aie2p_passes.h"
#include "utils.h"

#include <xaiengine/xaiegbl.h>
#include <xaiengine/xaie_txn.h>

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
    mp_header->PdiSize << ", 0x" << mp_header->PdiAddress << std::endl;
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



void XAie_OpHdr_null::transform() {
}

void XAie_OpHdr_print::transform() {
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

void XAie_OpHdr_drop_preempt::transform() {
  for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
    switch (it->m_op->Op) {
    case XAIE_IO_PREEMPT:
    {
      it->m_state = basic_node_state::dropped;
      size_t original_offset = it->m_original_offset;
      it = m_nodes.erase(it);
      auto noop = allocXaie<XAie_NoOpHdr>();
      noop->Op = XAIE_IO_NOOP;
      m_nodes.emplace(it, reinterpret_cast<const XAie_OpHdr *>(noop),
                      sizeof(XAie_NoOpHdr), original_offset, basic_node_state::added);
      break;
    }
    default:
      break;
    }
  }
}

void XAie_OpHdr_add_loadpdi::transform() {
  auto load = allocXaie<XAie_LoadPdiHdr>();
  load->Op = XAIE_IO_LOADPDI;
  load->PdiId = m_pdiid;
  load->PdiSize = m_pdisize;
  auto it = m_nodes.begin();
  size_t original_offset = it->m_original_offset;
  m_nodes.emplace(it, reinterpret_cast<const XAie_OpHdr *>(load),
                  sizeof(XAie_LoadPdiHdr), original_offset, basic_node_state::added);
}

}
