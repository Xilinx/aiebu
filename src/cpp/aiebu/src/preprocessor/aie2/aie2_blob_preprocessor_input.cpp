// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include <map>

#include "aie2_blob_preprocessor_input.h"
#include "op_types.h"

namespace aiebu {

  void
  aie2_blob_preprocessor_input::
  clear_shimBD_address_bits(std::vector<char>& mc_code, uint32_t offset) const
  {
    constexpr static uint32_t DMA_BD_1_IN_BYTES = 1 * 4;
    constexpr static uint32_t DMA_BD_2_IN_BYTES = 2 * 4;
    //Clearing address bits as they are set at runtime during patching(xrt/firmware).
    //Lower Base Address. 30 LSB of a 46-bit long 32-bit-word-address. (bits [31:2] in DMA_BD_1 of a 48-bit byte-address)
    //Upper Base Address. 16 MSB of a 46-bit long 32-bit-word-address. (bits [47:32] in DMA_BD_2 of a 48-bit byte-address)
    mc_code[offset + DMA_BD_1_IN_BYTES] = mc_code[offset + DMA_BD_1_IN_BYTES] & (0x03);
    mc_code[offset + DMA_BD_1_IN_BYTES + 1] = mc_code[offset + DMA_BD_1_IN_BYTES + 1] & (0x00);
    mc_code[offset + DMA_BD_1_IN_BYTES + 2] = mc_code[offset + DMA_BD_1_IN_BYTES + 2] & (0x00);
    mc_code[offset + DMA_BD_1_IN_BYTES + 3] = mc_code[offset + DMA_BD_1_IN_BYTES + 3] & (0x00);
    mc_code[offset + DMA_BD_2_IN_BYTES] = mc_code[offset + DMA_BD_2_IN_BYTES] & (0x00);
    mc_code[offset + DMA_BD_2_IN_BYTES + 1] = mc_code[offset + DMA_BD_2_IN_BYTES + 1] & (0x00);
  }

  uint32_t
  aie2_blob_transaction_preprocessor_input::
  extractSymbolFromBuffer(std::vector<char>& mc_code,
                          const std::string& section_name,
                          const std::string& argname)
  {
    // For transaction buffer flow. In Xclbin kernel argument, actual argument start from 3,
    // 0th is opcode, 1st is instruct buffer, 2nd is instruct buffer size.
    constexpr static uint32_t ARG_OFFSET = 3;
    std::map<uint32_t,uint32_t> blockWriteRegOffsetMap;
    const char *ptr = (mc_code.data());
    auto txn_header = reinterpret_cast<const XAie_TxnHeader *>(ptr);
    //printf("Header version %d.%d\n", txn_header->Major, txn_header->Minor);
    //printf("Device Generation: %d\n", txn_header->DevGen);
    //printf("Cols, Rows, NumMemRows : (%d, %d, %d)\n", txn_header->NumCols,
    //     txn_header.NumRows, txn_header->NumMemTileRows);
    //printf("TransactionSize: %u\n", txn_header->TxnSize);
    //printf("NumOps: %u\n", txn_header->NumOps);
    ptr += sizeof(XAie_TxnHeader);
    for(uint32_t i = 0; i < txn_header->NumOps; i++) {
        auto op_header = reinterpret_cast<const XAie_OpHdr *>(ptr);

        switch(op_header->Op) {
            case XAIE_IO_WRITE: {
                auto w_header = reinterpret_cast<const XAie_Write32Hdr *>(ptr);
                ptr += w_header->Size;
                break;
            }
            case XAIE_IO_BLOCKWRITE: {
                auto bw_header = reinterpret_cast<const XAie_BlockWrite32Hdr *>(ptr);
                auto payload = reinterpret_cast<const char*>(ptr + sizeof(XAie_BlockWrite32Hdr));
                auto offset = (size_t)(payload-mc_code.data());
                blockWriteRegOffsetMap[bw_header->RegOff] = offset;
                ptr += bw_header->Size;
                break;
            }
            case XAIE_IO_MASKWRITE: {
                auto mw_header = reinterpret_cast<const XAie_MaskWrite32Hdr *>(ptr);
                ptr += mw_header->Size;
                break;
            }
            case XAIE_IO_MASKPOLL: {
                auto mp_header = reinterpret_cast<const XAie_MaskPoll32Hdr *>(ptr);
                ptr += mp_header->Size;
                break;
            }
            case XAIE_IO_CUSTOM_OP_BEGIN: {
                auto co_header = reinterpret_cast<const XAie_CustomOpHdr *>(ptr);
                ptr += co_header->Size;
                break;
            }
            case XAIE_IO_CUSTOM_OP_BEGIN+1: {
                auto hdr = reinterpret_cast<const XAie_CustomOpHdr *>(ptr);
                auto op = reinterpret_cast<const patch_op_t *>(ptr + sizeof(*hdr));
                uint32_t reg = op->regaddr-4;  // regaddr point to 2nd word of BD
                auto it = blockWriteRegOffsetMap.find(reg);
                if ( it == blockWriteRegOffsetMap.end()) {
                   std::cout << "address "<< std::hex <<"0x" << reg << " have no block write opcode !!! removing all patching info";
                   m_sym.clear();
                   return txn_header->NumCols;
                }
                uint32_t offset = blockWriteRegOffsetMap[reg];
                clear_shimBD_address_bits(mc_code, offset);

                if (argname.empty())
                {
                  // added ARG_OFFSET to argidx to match with kernel argument index in xclbin
                  add_symbol(symbol(std::to_string(op->argidx + ARG_OFFSET), offset, 0, 0, op->argplus, section_name, symbol::patch_schema::shim_dma_48));
                } else
                  add_symbol(symbol(argname, offset, 0, 0, op->argplus, section_name, symbol::patch_schema::shim_dma_48));
                ptr += hdr->Size;
                break;
            }
            case XAIE_IO_CUSTOM_OP_BEGIN+2: {
                auto hdr = reinterpret_cast<const XAie_CustomOpHdr *>(ptr);
                ptr += hdr->Size;
                break;
            }
            case XAIE_IO_CUSTOM_OP_BEGIN+3: {
                auto hdr = reinterpret_cast<const XAie_CustomOpHdr *>(ptr);
                ptr += hdr->Size;
                break;
            }
            default:
                throw error(error::error_code::internal_error, "Invalid txn opcode: " + std::to_string(op_header->Op) + " !!!");
        }
    }
    return txn_header->NumCols;
  }

  void
  aie2_blob_dpu_preprocessor_input::
  patch_shimbd(const uint32_t* instr_ptr, size_t pc, const std::string& section_name)
  {
    uint32_t regId = (instr_ptr[pc] & 0x000000F0) >> 4;
    static std::map<uint32_t, std::string> arg2name = {
      {0, "ifm"},
      {1, "param"},
      {2, "ofm"},
      {3, "inter"},
      {4, "out2"},
      {5, "control-packet"}
    };

    auto it = arg2name.find(regId);
    if ( it == arg2name.end() )
      throw error(error::error_code::internal_error, "Invalid dpu arg:" + std::to_string(regId) + " !!!");

    add_symbol(symbol(arg2name[regId], (pc+1)*4, 0, 0, 0, section_name, symbol::patch_schema::shim_dma_48));
  }

  uint32_t
  aie2_blob_dpu_preprocessor_input::
  extractSymbolFromBuffer(std::vector<char>& mc_code,
                          const std::string& section_name,
                          const std::string& argname)
  {
    // For dpu 
    auto instr_ptr = reinterpret_cast<const uint32_t*>(mc_code.data());
    uint32_t inst_word_size = mc_code.size()/4;
    size_t pc = 0;																		

    while (pc < inst_word_size) {
      uint32_t opcode = (instr_ptr[pc] & 0xFF000000) >> 24;
      switch(opcode) {
        case OP_WRITESHIMBD: patch_shimbd(instr_ptr, pc, section_name);
          pc += OP_WRITESHIMBD_SIZE;
          break;
        case OP_WRITEBD:
        {
          uint8_t row = (instr_ptr[pc] & 0x0000FF00) >> 8;
          if (row == 0)
          {
            patch_shimbd(instr_ptr, pc, section_name);
            pc += OP_WRITEBD_SIZE_9;
          }
          else if (row == 1)
            pc += OP_WRITEBD_SIZE_9;
          else
            pc += OP_WRITEBD_SIZE_7;
          break;
        }
        case OP_NOOP: pc += OP_NOOP_SIZE;
          break;
        case OP_WRITE32: pc += OP_WRITE32_SIZE;
          break;
        case OP_WRITEBD_EXTEND_AIETILE: pc += OP_WRITEBD_EXTEND_AIETILE_SIZE;
          break;
        case OP_WRITE32_EXTEND_GENERAL: pc += OP_WRITE32_EXTEND_GENERAL_SIZE;
          break;
        case OP_WRITEBD_EXTEND_SHIMTILE: pc += OP_WRITEBD_EXTEND_SHIMTILE_SIZE;
          break;
        case OP_WRITEBD_EXTEND_MEMTILE: pc += OP_WRITEBD_EXTEND_MEMTILE_SIZE;
          break;
        case OP_WRITE32_EXTEND_DIFFBD: pc += OP_WRITE32_EXTEND_DIFFBD_SIZE;
          break;
        case OP_WRITEBD_EXTEND_SAMEBD_MEMTILE: pc += OP_WRITEBD_EXTEND_SAMEBD_MEMTILE_SIZE;
          break;
        case OP_DUMPDDR: pc += OP_DUMPDDR_SIZE;  //TODO: get pc based on hw/simnow
          break;
        case OP_WRITEMEMBD: pc += OP_WRITEMEMBD_SIZE;
          break;
        case OP_WRITE32_RTP: pc += OP_WRITE32_RTP_SIZE;
          break;
        case OP_READ32: pc += OP_READ32_SIZE;
          break;
        case OP_READ32_POLL: pc += OP_READ32_POLL_SIZE;
          break;
        case OP_SYNC: pc += OP_SYNC_SIZE;
          break;
        case OP_MERGESYNC: pc += OP_MERGESYNC_SIZE;
          break;
        case OP_DUMP_REGISTER:
        { pc += 1;
          uint32_t count = instr_ptr[pc] & 0x00FFFFFF;
          uint32_t total = count << 1;
          pc += total;
          break;
        } 
        case OP_RECORD_TIMESTAMP: pc += OP_RECORD_TIMESTAMP_SIZE;
          break;
        default:
          throw error(error::error_code::internal_error, "Invalid dpu opcode: " + std::to_string(opcode) + " !!!");
      }
    }
    return 0;
  }
}
