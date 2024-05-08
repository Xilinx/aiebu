// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>

//https://gitenterprise.xilinx.com/tsiddaga/dynamic_op_dispatch/blob/main/include/transaction.hpp
// Subroutines to read the transaction binary
#include "ps/op_buf.hpp"
#include "ps/op_types.h"

#include "xaiengine.h"
#include "transaction.hpp"

std::string transaction::get_txn_summary() {
  uint8_t *ptr = txn_.data();
  txn_pass_through(ptr);
  std::stringstream ss;
  ss << "Summary of transaction binary" << std::endl;
  ss << "Number of write ops: " << std::to_string(num_w_ops) << std::endl;
  ss << "Number of block_write ops: " << std::to_string(num_bw_ops)
     << std::endl;
  ss << "Number of mask_write ops: " << std::to_string(num_mw_ops) << std::endl;
  ss << "Number of mask_poll ops: " << std::to_string(num_mp_ops) << std::endl;
  ss << "Number of tct ops: " << std::to_string(num_tct_ops) << std::endl;
  ss << "Number of patch ops: " << std::to_string(num_patch_ops) << std::endl;
  ss << "Number of read ops: " << std::to_string(num_read_ops) << std::endl;
  ss << "Number of timer ops: " << std::to_string(num_readtimer_ops)
     << std::endl;
  ss << "Number of merge sync ops: " << std::to_string(num_merge_sync_ops)
     << std::endl;
  return get_txn_summary(txn_.data()) + ss.str();
}

transaction::transaction(const char *txn, unsigned size) {
  // load transaction binary from file to buffer

  // The following code attempts to detect if the TXN includes transaction_op_t header
  const auto *hdr = reinterpret_cast<const XAie_TxnHeader *>(txn);
  if (hdr->TxnSize != size) {
      // Cast to XAie_TxnHeader failed so there must be a transaction_op_t header sitting there
      const auto *tptr = reinterpret_cast<const transaction_op_t *>(txn);
      if ((tptr->b.type == e_TRANSACTION_OP) && (tptr->b.size_in_bytes == size)) {
          // This confirms that there is a transaction_op_t header, so strip it out
          txn += sizeof(transaction_op_t);
          size -= sizeof(transaction_op_t);
          hdr = reinterpret_cast<const XAie_TxnHeader *>(txn);
      }
  }

  if (hdr->TxnSize != size) {
      throw std::runtime_error("Corrupted transaction binary");
  }

  txn_.resize(hdr->TxnSize);
  txn_size_ = hdr->TxnSize;
  txn_num_ops_ = hdr->NumOps;

  uint8_t *ptr = txn_.data();
  std::memcpy(ptr, hdr, sizeof(XAie_TxnHeader));

  uint8_t *txn_ptr = ptr + sizeof(*hdr);
  std::memcpy((char *)txn_ptr, txn + sizeof(*hdr), hdr->TxnSize - sizeof(XAie_TxnHeader));
}

void transaction::txn_pass_through(uint8_t *ptr) {
  XAie_TxnHeader *Hdr = (XAie_TxnHeader *)(ptr);
  auto num_ops = Hdr->NumOps;
  ptr = (ptr) + sizeof(*Hdr);

  for (auto i = 0; i < num_ops; i++) {
    auto op_hdr = (XAie_OpHdr *)(ptr);
    switch (op_hdr->Op) {
    case XAIE_IO_WRITE: {
      XAie_Write32Hdr *w_hdr = (XAie_Write32Hdr *)(ptr);
      ptr = ptr + w_hdr->Size;
      num_w_ops++;
      break;
    }
    case XAIE_IO_BLOCKWRITE: {
      XAie_BlockWrite32Hdr *bw_header = (XAie_BlockWrite32Hdr *)(ptr);
      ptr = ptr + bw_header->Size;
      num_bw_ops++;
      break;
    }
    case XAIE_IO_MASKWRITE: {
      XAie_MaskWrite32Hdr *mw_header = (XAie_MaskWrite32Hdr *)(ptr);
      ptr = ptr + mw_header->Size;
      num_mw_ops++;
      break;
    }
    case XAIE_IO_MASKPOLL: {
      XAie_MaskPoll32Hdr *mp_header = (XAie_MaskPoll32Hdr *)(ptr);
      ptr = ptr + mp_header->Size;
      num_mp_ops++;
      break;
    }
    case (XAIE_IO_CUSTOM_OP_TCT): {
      XAie_CustomOpHdr *Hdr = (XAie_CustomOpHdr *)(ptr);
      ptr = ptr + Hdr->Size;
      num_tct_ops++;
      break;
    }
    case (XAIE_IO_CUSTOM_OP_DDR_PATCH): {
      XAie_CustomOpHdr *Hdr = (XAie_CustomOpHdr *)(ptr);
      ptr = ptr + Hdr->Size;
      num_patch_ops++;
      break;
    }
    case (XAIE_IO_CUSTOM_OP_BEGIN + 2):
    case (XAIE_IO_CUSTOM_OP_BEGIN + 3): {
      XAie_CustomOpHdr *Hdr = (XAie_CustomOpHdr *)(ptr);
      ptr = ptr + Hdr->Size;
      break;
    }
    case (XAIE_IO_CUSTOM_OP_BEGIN + 4): {
      XAie_CustomOpHdr *Hdr = (XAie_CustomOpHdr *)(ptr);
      ptr = ptr + Hdr->Size;
      num_merge_sync_ops++;
      break;
    }
    default:
      throw std::runtime_error("Unknown op to pass through");
      break;
    }
  }
}

std::string transaction::get_txn_summary(uint8_t *txn_ptr) {

  std::stringstream ss;
  XAie_TxnHeader *Hdr = (XAie_TxnHeader *)txn_ptr;

  ss << "Header version: " << std::to_string(Hdr->Major) << "."
     << std::to_string(Hdr->Minor) << std::endl;
  ss << "Device Generation: " << std::to_string(Hdr->DevGen) << std::endl;
  ss << "Partition Info: " << std::endl;
  ss << "     Num Cols:" << std::to_string(Hdr->NumCols) << std::endl;
  ss << "     Num Rows:" << std::to_string(Hdr->NumRows) << std::endl;
  ss << "     Num MemTile Rows:" << std::to_string(Hdr->NumMemTileRows)
     << std::endl;
  ss << "Transaction Metadata:" << std::endl;
  ss << "     Size: " << std::to_string(Hdr->TxnSize) << std::endl;
  ss << "     NumOps: " << std::to_string(Hdr->NumOps) << std::endl;

  return ss.str();
}

void transaction::stringify_w32(uint8_t **ptr) {
  XAie_Write32Hdr *w_hdr = (XAie_Write32Hdr *)(*ptr);
  ss_ops_ << "W: 0x" << std::hex << w_hdr->RegOff << " 0x" << w_hdr->Value
          << std::endl;
  *ptr = *ptr + w_hdr->Size;
}

void transaction::stringify_bw32(uint8_t **ptr) {
  XAie_BlockWrite32Hdr *bw_header = (XAie_BlockWrite32Hdr *)(*ptr);
  u32 reg_addr = bw_header->RegOff;
  u32 bw_size = bw_header->Size;
  u32 Size = (bw_size - sizeof(*bw_header)) / 4;
  u32 *Payload = (u32 *)((*ptr) + sizeof(*bw_header));
  ss_ops_ << "BW: 0x" << std::hex << bw_header->RegOff << " ";
  // ss_ops_ << "Payload: ";
  for (u32 i = 0; i < Size; i++) {
    ss_ops_ << "0x" << std::hex << *Payload << " ";
    Payload++;
  }
  ss_ops_ << std::endl;
  *ptr = *ptr + bw_size;
}

void transaction::stringify_mw32(uint8_t **ptr) {
  XAie_MaskWrite32Hdr *mw_header = (XAie_MaskWrite32Hdr *)(*ptr);
  ss_ops_ << "MW: 0x" << std::hex << mw_header->RegOff << " " << mw_header->Mask
          << " " << mw_header->Value << std::endl;
  *ptr = *ptr + mw_header->Size;
}

void transaction::stringify_mp32(uint8_t **ptr) {
  XAie_MaskPoll32Hdr *mp_header = (XAie_MaskPoll32Hdr *)(*ptr);
  ss_ops_ << "MP: 0x" << std::hex << mp_header->RegOff << " " << mp_header->Mask
          << " " << mp_header->Value << std::endl;
  *ptr = *ptr + mp_header->Size;
}

void transaction::stringify_tct(uint8_t **ptr) {
  XAie_CustomOpHdr *co_header = (XAie_CustomOpHdr *)(*ptr);
  ss_ops_ << "TCT: " << std::endl;
  *ptr = *ptr + co_header->Size;
}

void transaction::stringify_patchop(uint8_t **ptr) {
  XAie_CustomOpHdr *hdr = (XAie_CustomOpHdr *)(*ptr);
  u32 size = hdr->Size;
  ss_ops_ << "PatchOp: ";
  patch_op_t *op = (patch_op_t *)((*ptr) + sizeof(*hdr));
  auto reg_off = op->regaddr;
  auto arg_idx = op->argidx;
  auto addr_offset = op->argplus;
  ss_ops_ << "(RegAddr: " << std::hex << reg_off << " Arg Idx: " << arg_idx
          << " Addr Offset: " << addr_offset << ")" << std::endl;
  *ptr = *ptr + size;
}

void transaction::stringify_rdreg(uint8_t **ptr) {
  XAie_CustomOpHdr *Hdr = (XAie_CustomOpHdr *)(*ptr);
  u32 size = Hdr->Size;
  ss_ops_ << "ReadOp: " << std::endl;
  *ptr = *ptr + size;
}

void transaction::stringify_rectimer(uint8_t **ptr) {
  XAie_CustomOpHdr *Hdr = (XAie_CustomOpHdr *)(*ptr);
  u32 size = Hdr->Size;
  ss_ops_ << "TimerOp: " << std::endl;
  *ptr = *ptr + size;
}

void transaction::stringify_merge_sync(uint8_t **ptr) {
  XAie_CustomOpHdr *Hdr = (XAie_CustomOpHdr *)(*ptr);
  u32 size = Hdr->Size;
  ss_ops_ << "MergeSync Op: " << std::endl;
  *ptr = *ptr + size;
}

void transaction::stringify_txn_ops() {
  auto txn_ptr_ = txn_.data();
  // XAie_TxnHeader *Hdr = (XAie_TxnHeader *)txn_ptr_;
  XAie_TxnHeader *Hdr = (XAie_TxnHeader *)txn_.data();
  auto num_ops = Hdr->NumOps;
  // auto ptr = txn_ptr_ + sizeof(*Hdr);
  auto ptr = txn_.data() + sizeof(*Hdr);

  printf("TCT OP: %d\n", XAIE_IO_CUSTOM_OP_TCT);
  printf("DDR PATCH OP: %d\n", XAIE_IO_CUSTOM_OP_DDR_PATCH);
  printf("Read Reg OP: %d\n", XAIE_IO_CUSTOM_OP_BEGIN + 2);
  printf("Record Timer OP: %d\n", XAIE_IO_CUSTOM_OP_BEGIN + 3);
  printf("Merge Sync OP: %d\n", XAIE_IO_CUSTOM_OP_BEGIN + 4);

  XAie_OpHdr *op_hdr;
  for (auto i = 0; i < num_ops; i++) {
    op_hdr = (XAie_OpHdr *)ptr;
    switch (op_hdr->Op) {
    case XAIE_IO_WRITE:
      stringify_w32(&ptr);
      break;
    case XAIE_IO_BLOCKWRITE:
      stringify_bw32(&ptr);
      break;
    case XAIE_IO_MASKWRITE:
      stringify_mw32(&ptr);
      break;
    case XAIE_IO_MASKPOLL:
      stringify_mp32(&ptr);
      break;
    case XAIE_IO_CUSTOM_OP_TCT:
      stringify_tct(&ptr);
      break;
    case XAIE_IO_CUSTOM_OP_DDR_PATCH:
      stringify_patchop(&ptr);
      break;
    case XAIE_IO_CUSTOM_OP_BEGIN + 2:
      stringify_rdreg(&ptr);
      break;
    case XAIE_IO_CUSTOM_OP_BEGIN + 3:
      stringify_rectimer(&ptr);
      break;
    case XAIE_IO_CUSTOM_OP_BEGIN + 4:
      stringify_merge_sync(&ptr);
      break;
    default:
      throw std::runtime_error("Error: Unknown op code at offset at " +
                               std::to_string(ptr - txn_.data()) +
                               ". OpCode: " + std::to_string(op_hdr->Op));
      break;
    }
  }
}

std::string transaction::get_all_ops() {
  std::cout << "Converting to strings" << std::endl;
  stringify_txn_ops();
  return get_txn_summary() + ss_ops_.str();
}

void transaction::update_txns(struct arg_map &amap) {
  uint8_t *txn_ptr = txn_.data();

  // modify the txn in place.
  XAie_TxnHeader *txn_hdr = (XAie_TxnHeader *)txn_ptr;
  auto ptr = txn_ptr + sizeof(*txn_hdr);
  for (int i = 0; i < txn_hdr->NumOps; i++) {
    auto op_hdr = (XAie_OpHdr *)ptr;
    switch (op_hdr->Op) {
    case XAIE_IO_CUSTOM_OP_DDR_PATCH: {
      XAie_CustomOpHdr *hdr = (XAie_CustomOpHdr *)(ptr);
      u32 size = hdr->Size;
      patch_op_t *op = (patch_op_t *)((ptr) + sizeof(*hdr));
      auto arg_idx = op->argidx;
      op->argidx = amap.amap[arg_idx].first;
      auto offset = op->argplus;
      op->argplus = op->argplus + amap.amap[arg_idx].second;

      // RYZENAI_LOG_TRACE("[fuser] updating arg_idx: " +
      // std::to_string(arg_idx) +
      //                   " offset" + std::to_string(op->argplus) + " -> " +
      //                   std::to_string(op->argplus + offset));
      std::string log = "[fuser] updating arg_idx: " + std::to_string(arg_idx) +
                        "->" + std::to_string(op->argidx) + " offset " +
                        std::to_string(offset) + " -> " +
                        std::to_string(op->argplus);
      std::cout << log << std::endl;
      ptr = ptr + size;
    } break;
    default:
      // no modification for other ops
      txn_pass_through(&ptr);
      break;
    }
  }
}

void transaction::txn_pass_through(uint8_t **ptr) {
  auto op_hdr = (XAie_OpHdr *)(*ptr);
  switch (op_hdr->Op) {
  case XAIE_IO_WRITE: {
    XAie_Write32Hdr *w_hdr = (XAie_Write32Hdr *)(*ptr);
    *ptr = *ptr + w_hdr->Size;
    break;
  }
  case XAIE_IO_BLOCKWRITE: {
    XAie_BlockWrite32Hdr *bw_header = (XAie_BlockWrite32Hdr *)(*ptr);
    *ptr = *ptr + bw_header->Size;
    break;
  }
  case XAIE_IO_MASKWRITE: {
    XAie_MaskWrite32Hdr *mw_header = (XAie_MaskWrite32Hdr *)(*ptr);
    *ptr = *ptr + mw_header->Size;
    break;
  }
  case XAIE_IO_MASKPOLL: {
    XAie_MaskPoll32Hdr *mp_header = (XAie_MaskPoll32Hdr *)(*ptr);
    *ptr = *ptr + mp_header->Size;
    break;
  }
  case (XAIE_IO_CUSTOM_OP_BEGIN):
  case (XAIE_IO_CUSTOM_OP_BEGIN + 2):
  case (XAIE_IO_CUSTOM_OP_BEGIN + 3): {
    XAie_CustomOpHdr *Hdr = (XAie_CustomOpHdr *)(*ptr);
    *ptr = *ptr + Hdr->Size;
    break;
  }
  default:
    throw std::runtime_error("Unknown op to pass through");
  }
}
/*
std::vector<uint8_t> serialize(std::vector<transaction> &txns,
                               std::string txn_fname) {
  XAie_TxnHeader *hdr;
  std::vector<uint8_t> fused_txn(sizeof(*hdr));

  // get first_transaction
  // auto txn = txns.at(0);

  // memcpy(fused_txn.data(), hdr, sizeof(*hdr));

  uint32_t num_ops = 0;
  for (int i = 0; i < txns.size(); i++) {
    auto txn_hdr = (XAie_TxnHeader *)txns.at(i).txn_.data();
    if (i == 0) {
      memcpy(fused_txn.data(), txn_hdr, sizeof(*txn_hdr));
    }
    num_ops += txn_hdr->NumOps;

    for (int j = sizeof(*hdr); j < txns.at(i).txn_.size(); j++) {
      fused_txn.push_back(txns.at(i).txn_.at(j));
    }
  }

  hdr = (XAie_TxnHeader *)fused_txn.data();
  hdr->NumOps = num_ops;
  hdr->TxnSize = fused_txn.size();

  std::ofstream txn_bin(txn_fname, std::ios::binary);
  txn_bin.write((char *)fused_txn.data(), hdr->TxnSize);

  return fused_txn;
}
*/
