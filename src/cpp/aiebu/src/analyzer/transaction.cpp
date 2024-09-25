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
    implementation(const char *txn, uint64_t size) {

        // TXN with transaction_op_t header is not suupported.
        const auto *hdr = reinterpret_cast<const XAie_TxnHeader *>(txn);
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
        ss << op_format << "XAIE_IO_NOOP " << dec_format << op_count[XAIE_IO_NOOP] << std::endl;
        ss << op_format << "XAIE_IO_PREEMPT " << dec_format << op_count[XAIE_IO_PREEMPT] << std::endl;
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
    #define MAJOR_VER 1
    #define MINOR_VER 0

    template <std::size_t N>
    void count_tnx(const uint8_t *ptr, std::array<unsigned int, N> &op_count) const {
        auto Hdr = (const XAie_TxnHeader *)(ptr);
        const auto num_ops = Hdr->NumOps;
        ptr += sizeof(*Hdr);

        for (auto i = 0U; i < num_ops; i++) {
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
            case XAIE_IO_NOOP: {
                ptr +=  sizeof(XAie_NoOpHdr);
                break;
            }
            case XAIE_IO_PREEMPT: {
                ptr += sizeof(XAie_PreemptHdr);
                break;
            }
            case (XAIE_IO_CUSTOM_OP_TCT):
            case (XAIE_IO_CUSTOM_OP_DDR_PATCH):
            case (XAIE_IO_CUSTOM_OP_READ_REGS):
            case (XAIE_IO_CUSTOM_OP_RECORD_TIMER):
            case (XAIE_IO_CUSTOM_OP_MERGE_SYNC): {
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

    template <std::size_t N>
    void count_tnx_opt(const uint8_t *ptr, std::array<unsigned int, N> &op_count) const {
        auto Hdr = (const XAie_TxnHeader *)(ptr);
        const auto num_ops = Hdr->NumOps;
        ptr += sizeof(*Hdr);
        for (auto i = 0U; i < num_ops; i++) {
            auto op_hdr = (const XAie_OpHdr_opt *)(ptr);
            op_count[op_hdr->Op]++;
            switch (op_hdr->Op) {
            case XAIE_IO_WRITE: {
                ptr += sizeof(XAie_Write32Hdr_opt);
                break;
            }
            case XAIE_IO_BLOCKWRITE: {
                auto bw_header = (const XAie_BlockWrite32Hdr_opt *)(ptr);
                ptr += bw_header->Size;
                break;
            }
            case XAIE_IO_MASKWRITE: {
                ptr += sizeof(XAie_MaskWrite32Hdr_opt);
                break;
            }
            case XAIE_IO_MASKPOLL: {
                ptr += sizeof(XAie_MaskPoll32Hdr_opt);
                break;
            }
            case XAIE_IO_NOOP: {
                ptr +=  sizeof(XAie_NoOpHdr);
                break;
            }
            case XAIE_IO_PREEMPT: {
                ptr += sizeof(XAie_PreemptHdr);
                break;
            }
            case (XAIE_IO_CUSTOM_OP_TCT):
            case (XAIE_IO_CUSTOM_OP_DDR_PATCH):
            case (XAIE_IO_CUSTOM_OP_READ_REGS):
            case (XAIE_IO_CUSTOM_OP_RECORD_TIMER):
            case (XAIE_IO_CUSTOM_OP_MERGE_SYNC): {
                auto Hdr = (const XAie_CustomOpHdr_opt *)(ptr);
                ptr += Hdr->Size;
                break;
            }
            default:
                throw std::runtime_error("Unknown op to pass through");
                break;
            }
        }
    }

    template <std::size_t N>
    void count_txn_ops(const uint8_t *ptr, std::array<unsigned int, N> &op_count) const {
        auto Hdr = (const XAie_TxnHeader *)(ptr);

        /**
         * Check if Header Version is 1.0 then call optimized API else continue with this
         * function to service the TXN buffer.
         */
        if ((Hdr->Major == MAJOR_VER) && (Hdr->Minor == MINOR_VER)) {
            printf("Optimized HEADER version detected \n");
            count_tnx_opt(ptr, op_count);
        } else {
            count_tnx(ptr, op_count);
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

    size_t stringify_noop(const XAie_OpHdr *ptr, std::ostream &ss_ops_) const {
        (void)ptr;
        ss_ops_ << op_format << "XAIE_IO_NOOP, " << std::endl;
        return sizeof(XAie_NoOpHdr);
    }

    size_t stringify_preempt(const XAie_OpHdr *ptr, std::ostream &ss_ops_) const {
        auto mp_header = (const XAie_PreemptHdr *)(ptr);
        ss_ops_ << op_format << "XAIE_IO_PREEMPT, " << "@0x" << mp_header->Preempt_level << std::endl;
        return sizeof(XAie_PreemptHdr);
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

    size_t stringify_tct_opt(const XAie_OpHdr_opt *ptr, std::ostream &ss_ops_) const {
        auto co_header = (const XAie_CustomOpHdr_opt *)(ptr);
        ss_ops_ << op_format << "XAIE_IO_CUSTOM_OP_TCT " << std::endl;
        return co_header->Size;
    }

    size_t stringify_w32_opt(const XAie_OpHdr_opt *ptr, std::ostream &ss_ops_) const {
        auto w_hdr = (const XAie_Write32Hdr_opt *)(ptr);
        ss_ops_ << op_format << "XAIE_IO_WRITE, " << "@0x" << std::hex << w_hdr->RegOff << ", 0x" << w_hdr->Value
                << std::endl;
        return sizeof(XAie_Write32Hdr_opt);
    }

    size_t stringify_bw32_opt(const XAie_OpHdr_opt *ptr, std::ostream &ss_ops_) const {
        auto bw_header = (const XAie_BlockWrite32Hdr_opt *)(ptr);
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

    size_t stringify_mw32_opt(const XAie_OpHdr_opt *ptr, std::ostream &ss_ops_) const {
        auto mw_header = (const XAie_MaskWrite32Hdr_opt *)(ptr);
ss_ops_ << op_format << "XAIE_IO_MASKWRITE, " << "@0x" << std::hex << mw_header->RegOff << ", 0x" << mw_header->Mask
                << ", 0x" << mw_header->Value << std::endl;
        return sizeof(XAie_MaskWrite32Hdr_opt);
    }

    size_t stringify_mp32_opt(const XAie_OpHdr_opt *ptr, std::ostream &ss_ops_) const {
        auto mp_header = (const XAie_MaskPoll32Hdr_opt *)(ptr);
ss_ops_ << op_format << "XAIE_IO_MASKPOLL, " << "@0x" << std::hex << mp_header->RegOff << ", 0x" << mp_header->Mask
                << ", 0x" << mp_header->Value << std::endl;
        return sizeof(XAie_MaskPoll32Hdr_opt);
    }

    size_t stringify_noop_opt(const XAie_OpHdr_opt *ptr, std::ostream &ss_ops_) const {
        (void)ptr;
        ss_ops_ << op_format << "XAIE_IO_NOOP, " << std::endl;
        return sizeof(XAie_NoOpHdr);
    }

    size_t stringify_preempt_opt(const XAie_OpHdr_opt *ptr, std::ostream &ss_ops_) const {
        auto mp_header = (const XAie_PreemptHdr *)(ptr);
        ss_ops_ << op_format << "XAIE_IO_PREEMPT, " << "@0x" << std::hex << (uint32_t)mp_header->Preempt_level << std::endl;
        return sizeof(XAie_PreemptHdr);
    }

    size_t stringify_patchop_opt(const XAie_OpHdr_opt *ptr, std::ostream &ss_ops_) const {
        auto hdr = (const XAie_CustomOpHdr_opt *)(ptr);
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

    size_t stringify_rdreg_opt(const XAie_OpHdr_opt *ptr, std::ostream &ss_ops_) const {
        auto Hdr = (const XAie_CustomOpHdr_opt *)(ptr);
        u32 size = Hdr->Size;
        ss_ops_ << "ReadOp: " << std::endl;
        return size;
    }

    size_t stringify_rectimer_opt(const XAie_OpHdr_opt *ptr, std::ostream &ss_ops_) const {
        auto Hdr = (const XAie_CustomOpHdr_opt *)(ptr);
        u32 size = Hdr->Size;
        ss_ops_ << "TimerOp: " << std::endl;
        return size;
    }

    size_t stringify_merge_sync_opt(const XAie_OpHdr_opt *ptr, std::ostream &ss_ops_) const {
        auto Hdr = (const XAie_CustomOpHdr_opt *)(ptr);
        u32 size = Hdr->Size;
        ss_ops_ << "MergeSync Op: " << std::endl;
        return size;
    }

    [[nodiscard]] std::string stringify_txn() const {
        auto Hdr = (const XAie_TxnHeader *)txn_.data();
        auto num_ops = Hdr->NumOps;
        auto ptr = txn_.data() + sizeof(*Hdr);

        std::stringstream ss;

        for (auto i = 0U; i < num_ops; i++) {
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
            case XAIE_IO_NOOP:
                size = stringify_noop(op_hdr, ss);
                break;
            case XAIE_IO_PREEMPT:
                size = stringify_preempt(op_hdr, ss);
                break;
            case XAIE_IO_CUSTOM_OP_TCT:
                size = stringify_tct(op_hdr, ss);
                break;
            case XAIE_IO_CUSTOM_OP_DDR_PATCH:
                size = stringify_patchop(op_hdr, ss);
                break;
            case XAIE_IO_CUSTOM_OP_READ_REGS:
                size = stringify_rdreg(op_hdr, ss);
                break;
            case XAIE_IO_CUSTOM_OP_RECORD_TIMER:
                size = stringify_rectimer(op_hdr, ss);
                break;
            case XAIE_IO_CUSTOM_OP_MERGE_SYNC:
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

    [[nodiscard]] std::string stringify_txn_opt() const {
        auto Hdr = (const XAie_TxnHeader *)txn_.data();
        auto num_ops = Hdr->NumOps;
        auto ptr = txn_.data() + sizeof(*Hdr);

        std::stringstream ss;
        for (auto i = 0U; i < num_ops; i++) {
            const auto op_hdr = (const XAie_OpHdr_opt *)ptr;
            size_t size = 0;
            switch (op_hdr->Op) {
            case XAIE_IO_WRITE:
                size = stringify_w32_opt(op_hdr, ss);
                break;
            case XAIE_IO_BLOCKWRITE:
                size = stringify_bw32_opt(op_hdr, ss);
                break;
            case XAIE_IO_MASKWRITE:
                size = stringify_mw32_opt(op_hdr, ss);
                break;
            case XAIE_IO_MASKPOLL:
                size = stringify_mp32_opt(op_hdr, ss);
                break;
            case XAIE_IO_NOOP:
                size = stringify_noop_opt(op_hdr, ss);
                break;
            case XAIE_IO_PREEMPT:
                size = stringify_preempt_opt(op_hdr, ss);
                break;
            case XAIE_IO_CUSTOM_OP_TCT:
                size = stringify_tct_opt(op_hdr, ss);
                break;
            case XAIE_IO_CUSTOM_OP_DDR_PATCH:
                size = stringify_patchop_opt(op_hdr, ss);
                break;
            case XAIE_IO_CUSTOM_OP_READ_REGS:
                size = stringify_rdreg_opt(op_hdr, ss);
                break;
            case XAIE_IO_CUSTOM_OP_RECORD_TIMER:
                size = stringify_rectimer_opt(op_hdr, ss);
                break;
            case XAIE_IO_CUSTOM_OP_MERGE_SYNC:
                size = stringify_merge_sync_opt(op_hdr, ss);
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

    [[nodiscard]] std::string stringify_txn_ops() const {
        auto Hdr = (const XAie_TxnHeader *)txn_.data();
        /**
         * Check if Header Version is 1.0 then call optimized API else continue with this
         * function to service the TXN buffer.
         */
        if ((Hdr->Major == MAJOR_VER) && (Hdr->Minor == MINOR_VER)) {
            printf("Optimized HEADER version detected \n");
            return stringify_txn_opt();
        } else {
            return stringify_txn();
        }
    }
};

transaction::transaction(const char *txn, uint64_t size) : impl(std::make_shared<transaction::implementation>(txn, size)) {}

std::string transaction::get_txn_summary() const
{
    return impl->get_txn_summary();
}

std::string transaction::get_all_ops() const
{
    return impl->get_all_ops();
}
