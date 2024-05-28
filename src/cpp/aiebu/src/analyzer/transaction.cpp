// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include <iomanip>
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <array>
#include <sstream>

// https://gitenterprise.xilinx.com/tsiddaga/dynamic_op_dispatch/blob/main/include/transaction.hpp

#include "ps/op_buf.hpp"
#include "ps/op_types.h"

#include "xaiengine.h"
#include "transaction.hpp"

struct transaction::implementation {
private:
    static constexpr unsigned int field_width = 32;
    static inline std::ostream& op_format(std::ostream& strm) {
        strm << std::setw(field_width) << std::left;
        return strm;
    }

    static inline std::ostream& dec_format(std::ostream& strm) {
        strm << std::setw(field_width/2) << std::right;
        return strm;
    }

private:
    std::vector<uint8_t> txn_;

    std::string get_txn_summary(const uint8_t *txn_ptr) const {

        std::stringstream ss;
        auto Hdr = (XAie_TxnHeader *)txn_ptr;

        ss << "v" << (int)Hdr->Major << "." << (int)Hdr->Minor << ", gen" << (int)Hdr->DevGen << std::endl;
        ss << (int)Hdr->NumRows << "x" << (int)Hdr->NumCols << " M" << (int)Hdr->NumMemTileRows << std::endl;
        ss << Hdr->TxnSize << "B, " << Hdr->NumOps << "ops" << std::endl;
        return ss.str();
    }

public:
    implementation(const char *txn, unsigned size) {

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

        uint8_t *ptr = txn_.data();
        std::memcpy(ptr, hdr, sizeof(XAie_TxnHeader));

        uint8_t *txn_ptr = ptr + sizeof(*hdr);
        std::memcpy((char *)txn_ptr, txn + sizeof(*hdr), hdr->TxnSize - sizeof(XAie_TxnHeader));
    }

    [[nodiscard]] std::string get_txn_summary() const {
        const uint8_t *ptr = txn_.data();
        std::array<unsigned int, XAIE_IO_CUSTOM_OP_NEXT> op_count = {};
        count_txn_ops(ptr, op_count);
        std::stringstream ss;

        ss << op_format << "XAIE_IO_WRITE " << dec_format << op_count[XAIE_IO_WRITE] << std::endl;
        ss << op_format << "XAIE_IO_BLOCKWRITE " << dec_format << op_count[XAIE_IO_BLOCKWRITE] << std::endl;
        ss << op_format << "XAIE_IO_MASKWRITE " << dec_format << op_count[XAIE_IO_MASKWRITE] << std::endl;
        ss << op_format << "XAIE_IO_MASKPOLL " << dec_format << op_count[XAIE_IO_MASKPOLL] << std::endl;
        ss << op_format << "XAIE_IO_CUSTOM_OP_TCT " << dec_format << op_count[XAIE_IO_CUSTOM_OP_TCT] << std::endl;
        ss << op_format << "XAIE_IO_CUSTOM_OP_DDR_PATCH " << dec_format << op_count[XAIE_IO_CUSTOM_OP_DDR_PATCH] << std::endl;
        /*
        ss << "Number of read ops: " << std::to_string(num_read_ops) << std::endl;
        ss << "Number of timer ops: " << std::to_string(num_readtimer_ops)
           << std::endl;
        ss << "Number of merge sync ops: " << std::to_string(num_merge_sync_ops)
           << std::endl;
        */
        return get_txn_summary(txn_.data()) + ss.str();
    }

    [[nodiscard]] std::string get_all_ops() const {
        return stringify_txn_ops();
    }

private:
    template <std::size_t N>
    void count_txn_ops(const uint8_t *ptr, std::array<unsigned int, N> &op_count) const {
        auto Hdr = (const XAie_TxnHeader *)(ptr);
        const auto num_ops = Hdr->NumOps;
        ptr += sizeof(*Hdr);

        for (auto i = 0; i < num_ops; i++) {
            auto op_hdr = (const XAie_OpHdr *)(ptr);
            op_count[op_hdr->Op]++;
            switch (op_hdr->Op) {
            case XAIE_IO_WRITE: {
                auto w_hdr = (const XAie_Write32Hdr *)(ptr);
                ptr += w_hdr->Size;
                break;
            }
            case XAIE_IO_BLOCKWRITE: {
                auto bw_header = (const XAie_BlockWrite32Hdr *)(ptr);
                ptr += bw_header->Size;
                break;
            }
            case XAIE_IO_MASKWRITE: {
                auto mw_header = (const XAie_MaskWrite32Hdr *)(ptr);
                ptr += mw_header->Size;
                break;
            }
            case XAIE_IO_MASKPOLL: {
                auto mp_header = (const XAie_MaskPoll32Hdr *)(ptr);
                ptr += mp_header->Size;
                break;
            }
            case (XAIE_IO_CUSTOM_OP_TCT): {
                auto Hdr = (const XAie_CustomOpHdr *)(ptr);
                ptr += Hdr->Size;
                break;
            }
            case (XAIE_IO_CUSTOM_OP_DDR_PATCH): {
                auto Hdr = (const XAie_CustomOpHdr *)(ptr);
                ptr += Hdr->Size;
                break;
            }
            case (XAIE_IO_CUSTOM_OP_BEGIN + 2): {
                auto Hdr = (const XAie_CustomOpHdr *)(ptr);
                ptr += Hdr->Size;
                break;
            }
            case (XAIE_IO_CUSTOM_OP_BEGIN + 3): {
                auto Hdr = (const XAie_CustomOpHdr *)(ptr);
                ptr += Hdr->Size;
                break;
            }
            case (XAIE_IO_CUSTOM_OP_BEGIN + 4): {
                auto Hdr = (const XAie_CustomOpHdr *)(ptr);
                ptr += Hdr->Size;
                break;
            }
            default:
                throw std::runtime_error("Unknown op to pass through");
                break;
            }
        }
    }

    size_t stringify_w32(const XAie_OpHdr *ptr, std::ostream &ss_ops_) const {
        auto w_hdr = (const XAie_Write32Hdr *)(ptr);
        ss_ops_ << op_format << "XAIE_IO_WRITE, " << "@0x" << std::hex << w_hdr->RegOff << ", 0x" << w_hdr->Value
                << std::endl;
        return w_hdr->Size;
    }

    size_t stringify_bw32(const XAie_OpHdr *ptr, std::ostream &ss_ops_) const {
        auto bw_header = (const XAie_BlockWrite32Hdr *)(ptr);
        u32 bw_size = bw_header->Size;
        u32 Size = (bw_size - sizeof(*bw_header)) / 4;
        const char *curr = (const char *)ptr;
        curr += sizeof(*bw_header);
        u32 *Payload = (u32 *)curr;
        ss_ops_ << op_format << "XAIE_IO_BLOCKWRITE, " << "@0x" << std::hex << bw_header->RegOff;
        for (u32 i = 0; i < Size; i++) {
            ss_ops_ << ", 0x" << std::hex << *Payload;
            Payload++;
        }
        ss_ops_ << std::endl;
        return bw_size;
    }

    size_t stringify_mw32(const XAie_OpHdr *ptr, std::ostream &ss_ops_) const {
        auto mw_header = (const XAie_MaskWrite32Hdr *)(ptr);
ss_ops_ << op_format << "XAIE_IO_MASKWRITE, " << "@0x" << std::hex << mw_header->RegOff << ", 0x" << mw_header->Mask
                << ", 0x" << mw_header->Value << std::endl;
        return mw_header->Size;
    }

    size_t stringify_mp32(const XAie_OpHdr *ptr, std::ostream &ss_ops_) const {
        auto mp_header = (const XAie_MaskPoll32Hdr *)(ptr);
ss_ops_ << op_format << "XAIE_IO_MASKPOLL, " << "@0x" << std::hex << mp_header->RegOff << ", 0x" << mp_header->Mask
                << ", 0x" << mp_header->Value << std::endl;
        return mp_header->Size;
    }

    size_t stringify_tct(const XAie_OpHdr *ptr, std::ostream &ss_ops_) const {
        auto co_header = (const XAie_CustomOpHdr *)(ptr);
        ss_ops_ << op_format << "XAIE_IO_CUSTOM_OP_TCT " << std::endl;
        return co_header->Size;
    }

    size_t stringify_patchop(const XAie_OpHdr *ptr, std::ostream &ss_ops_) const {
        auto hdr = (const XAie_CustomOpHdr *)(ptr);
        u32 size = hdr->Size;
        ss_ops_ << op_format << "XAIE_IO_CUSTOM_OP_DDR_PATCH, ";
        const char *curr = (const char *)ptr;
        curr += sizeof(*hdr);
        auto op = (const patch_op_t *)curr;
        auto reg_off = op->regaddr;
        auto arg_idx = op->argidx;
        auto addr_offset = op->argplus;
        ss_ops_ << "@0x" << std::hex << reg_off << std::dec << ", " << arg_idx
                << std::hex << ", 0x" << addr_offset << std::endl;
        return size;
    }

    size_t stringify_rdreg(const XAie_OpHdr *ptr, std::ostream &ss_ops_) const {
        auto Hdr = (const XAie_CustomOpHdr *)(ptr);
        u32 size = Hdr->Size;
        ss_ops_ << "ReadOp: " << std::endl;
        return size;
    }

    size_t stringify_rectimer(const XAie_OpHdr *ptr, std::ostream &ss_ops_) const {
        auto Hdr = (const XAie_CustomOpHdr *)(ptr);
        u32 size = Hdr->Size;
        ss_ops_ << "TimerOp: " << std::endl;
        return size;
    }

    size_t stringify_merge_sync(const XAie_OpHdr *ptr, std::ostream &ss_ops_) const {
        auto Hdr = (const XAie_CustomOpHdr *)(ptr);
        u32 size = Hdr->Size;
        ss_ops_ << "MergeSync Op: " << std::endl;
        return size;
    }

    [[nodiscard]] std::string stringify_txn_ops() const {
        auto Hdr = (const XAie_TxnHeader *)txn_.data();
        auto num_ops = Hdr->NumOps;
        auto ptr = txn_.data() + sizeof(*Hdr);

#if 0
        ss_ops_ << "TCT OP: " << XAIE_IO_CUSTOM_OP_TCT;
        ss_ops_ << "DDR PATCH OP: " << XAIE_IO_CUSTOM_OP_DDR_PATCH;
        ss_ops_ << "Read Reg OP: " << XAIE_IO_CUSTOM_OP_BEGIN + 2;
        ss_ops_ << "Record Timer OP: " << XAIE_IO_CUSTOM_OP_BEGIN + 3;
        ss_ops_ << "Merge Sync OP: " << XAIE_IO_CUSTOM_OP_BEGIN + 4;
#endif

        std::stringstream ss;

        for (auto i = 0; i < num_ops; i++) {
            const auto op_hdr = (const XAie_OpHdr *)ptr;
            size_t size = 0;
            switch (op_hdr->Op) {
            case XAIE_IO_WRITE:
                size = stringify_w32(op_hdr, ss);
                break;
            case XAIE_IO_BLOCKWRITE:
                size = stringify_bw32(op_hdr, ss);
                break;
            case XAIE_IO_MASKWRITE:
                size = stringify_mw32(op_hdr, ss);
                break;
            case XAIE_IO_MASKPOLL:
                size = stringify_mp32(op_hdr, ss);
                break;
            case XAIE_IO_CUSTOM_OP_TCT:
                size = stringify_tct(op_hdr, ss);
                break;
            case XAIE_IO_CUSTOM_OP_DDR_PATCH:
                size = stringify_patchop(op_hdr, ss);
                break;
            case XAIE_IO_CUSTOM_OP_BEGIN + 2:
                size = stringify_rdreg(op_hdr, ss);
                break;
            case XAIE_IO_CUSTOM_OP_BEGIN + 3:
                size = stringify_rectimer(op_hdr, ss);
                break;
            case XAIE_IO_CUSTOM_OP_BEGIN + 4:
                size = stringify_merge_sync(op_hdr, ss);
                break;
            default:
                throw std::runtime_error("Error: Unknown op code at offset at " +
                                         std::to_string(ptr - txn_.data()) +
                                         ". OpCode: " + std::to_string(op_hdr->Op));
                break;
            }
            ptr += size;
        }
        return ss.str();
    }

    void update_txns(struct arg_map &amap) {
        uint8_t *txn_ptr = txn_.data();

        // modify the txn in place.
        auto txn_hdr = (XAie_TxnHeader *)txn_ptr;
        auto ptr = txn_ptr + sizeof(*txn_hdr);
        for (int i = 0; i < txn_hdr->NumOps; i++) {
            auto op_hdr = (XAie_OpHdr *)ptr;
            switch (op_hdr->Op) {
            case XAIE_IO_CUSTOM_OP_DDR_PATCH: {
                auto hdr = (XAie_CustomOpHdr *)(ptr);
                u32 size = hdr->Size;
                auto op = (patch_op_t *)((ptr) + sizeof(*hdr));
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
                ptr += size;
            } break;
            default:
                ptr += get_txn_size(op_hdr);
                break;
            }
        }
    }

    size_t get_txn_size(const XAie_OpHdr *ptr) const {
        switch (ptr->Op) {
        case XAIE_IO_WRITE: {
            auto w_hdr = (const XAie_Write32Hdr *)(ptr);
            return w_hdr->Size;
            break;
        }
        case XAIE_IO_BLOCKWRITE: {
            auto bw_header = (const XAie_BlockWrite32Hdr *)(ptr);
            return bw_header->Size;
            break;
        }
        case XAIE_IO_MASKWRITE: {
            auto mw_header = (const XAie_MaskWrite32Hdr *)(ptr);
            return mw_header->Size;
            break;
        }
        case XAIE_IO_MASKPOLL: {
            auto mp_header = (const XAie_MaskPoll32Hdr *)(ptr);
            return mp_header->Size;
            break;
        }
        case (XAIE_IO_CUSTOM_OP_BEGIN):
        case (XAIE_IO_CUSTOM_OP_BEGIN + 2):
        case (XAIE_IO_CUSTOM_OP_BEGIN + 3): {
            auto hdr = (const XAie_CustomOpHdr *)(ptr);
            return hdr->Size;
            break;
        }
        default:
            throw std::runtime_error("Unknown op to pass through");
            return 0xffffffff;
        }
    }
};

transaction::transaction(const char *txn, unsigned size) : impl(std::make_shared<transaction::implementation>(txn, size)) {}

std::string transaction::get_txn_summary() const
{
    return impl->get_txn_summary();
}

std::string transaction::get_all_ops() const
{
    return impl->get_all_ops();
}
