// SPDX-License-Identifier: MIT
// Copyright (C) 2024 Advanced Micro Devices, Inc.

/**
 * This file generates transaction commands for data copy from L3 -> L2 and from L2 -> L3.
 * Then wrap it into an ELF file.
 *
 * Current Implementation -
 *      This file have the address patching for DDR only.
 *
 * Next Steps -
 *      1. Patch L2 address.
 *      2. Patch the size of data.
 *      3. Patch the col index of MemTile.
**/


#include <cassert>
#include <iostream>
#include <fstream>

// AIE Driver headers
#include "xaiengine.h"
#include "xaiengine/xaiegbl_params.h"

#include "aiebu_assembler.h"

#include "gen-common.h"

#define XAIE_NUM_ROWS 6
#define XAIE_NUM_COLS 4
#define XAIE_BASE_ADDR 0
#define XAIE_COL_SHIFT 25
#define XAIE_ROW_SHIFT 20
#define XAIE_SHIM_ROW 0
#define XAIE_MEM_TILE_ROW_START 1
#define XAIE_MEM_TILE_NUM_ROWS 1
#define XAIE_AIE_TILE_ROW_START 2
#define XAIE_AIE_TILE_NUM_ROWS 4


#define Mem_Tile_to_DDR 0
#define DDR_to_Mem_Tile 1
#define SIZE 32

constexpr auto START_OF_MEM = 0x80000;
constexpr auto OUT_BUFFER_KERNARG_IDX = 0;
constexpr auto IN_BUFFER_KERNARG_IDX = 1;
constexpr uint8_t patch_ddr_opcode = XAIE_IO_CUSTOM_OP_DDR_PATCH;
constexpr auto DEFAULT_UNPATCHED_ADDR = 0;


/* Creates the sequence to store data from MEM to external ddr memory dst*/
int Mem_Tile_to_DDR_Copy(XAie_DevInst* dev, uint32_t num_elems, uint8_t col)
{
        AieRC RC = XAIE_OK;

        XAie_LocType Tile_M, Tile_S;
        Tile_M = XAie_TileLoc(col, UINT8_C(1));   // MEM Tile
        Tile_S = XAie_TileLoc(col, UINT8_C(0));   // SHIM Tile
        XAie_DmaDesc Tile_M_MM2S, Tile_S_S2MM;

        /* Configure stream switch ports to move data from MEM to SHIM */
        RC = XAie_StrmConnCctEnable(dev, Tile_M, DMA, 0, SOUTH, 0);   // DMA 0 to SOUTH 0
        gen_XAie_check(RC);
        RC = XAie_StrmConnCctEnable(dev, Tile_S, NORTH, 0, SOUTH, 2); // NORTH to SOUTH 2
        gen_XAie_check(RC);
        RC = XAie_EnableAieToShimDmaStrmPort(dev, Tile_S, 2); // this is needed because the south port 2 is also used as DMA
        gen_XAie_check(RC);

        // create BDs
        RC = XAie_DmaDescInit(dev, &Tile_M_MM2S, Tile_M);
        gen_XAie_check(RC);
        RC = XAie_DmaDescInit(dev, &Tile_S_S2MM, Tile_S);
        gen_XAie_check(RC);

        /* Configure address and length in dma software descriptors and enable DMA BDs */
        RC = XAie_DmaSetAddrLen(&Tile_M_MM2S, START_OF_MEM, num_elems * sizeof(uint32_t)); // Read from local address 0
        gen_XAie_check(RC);
        RC = XAie_DmaSetAddrLen(&Tile_S_S2MM, DEFAULT_UNPATCHED_ADDR, num_elems * sizeof(uint32_t)); // Write to external ddr mem
        gen_XAie_check(RC);

        RC = XAie_DmaEnableBd(&Tile_M_MM2S);
        gen_XAie_check(RC);
        RC = XAie_DmaEnableBd(&Tile_S_S2MM);
        gen_XAie_check(RC);

        // configure BDs
        RC = XAie_DmaSetAxi(&Tile_S_S2MM, 0U, 16U, 0U, 0U, 0U);
        gen_XAie_check(RC);

        RC = XAie_DmaWriteBd(dev, &Tile_M_MM2S, Tile_M, 0U); // BD 0
        gen_XAie_check(RC);

        RC = XAie_DmaWriteBd(dev, &Tile_S_S2MM, Tile_S, 0U); // BD 0
        gen_XAie_check(RC);

        // patch DDR BD address
        patch_op_t patch_ddr_instr = {};
        uint64_t tile_offset = _XAie_GetTileAddr(dev, 0, col);

        patch_ddr_instr.regaddr = XAIEGBL_MEM_DMABD0ADDB + tile_offset;
        patch_ddr_instr.argidx = OUT_BUFFER_KERNARG_IDX; //argidx points to out_bo
        patch_ddr_instr.argplus = 0; //argplus

        RC = XAie_AddCustomTxnOp(dev, patch_ddr_opcode, &patch_ddr_instr, sizeof(patch_ddr_instr)); // Create patch operation that will set actual address to use at runtime
        gen_XAie_check(RC);

        /* Push Bd numbers to aie dma channel queues and enable the channels */
        RC = XAie_DmaChannelPushBdToQueue(dev, Tile_M, 0U, DMA_MM2S, 0U); // Push BD 0.
        gen_XAie_check(RC);
        RC = XAie_DmaChannelPushBdToQueue(dev, Tile_S, 0U, DMA_S2MM, 0U); // Push BD 0
        gen_XAie_check(RC);

        /* Enable the buffer descriptors in software dma descriptors */
        RC = XAie_DmaChannelEnable(dev, Tile_M, 0U, DMA_MM2S); // Enable channel 0
        gen_XAie_check(RC);
        RC = XAie_DmaChannelEnable(dev, Tile_S, 0U, DMA_S2MM); // Enable channel 0 for DMA
        gen_XAie_check(RC);

        RC = XAie_DmaWaitForDone(dev, Tile_S, 1, DMA_S2MM, 0);
        gen_XAie_check(RC);

        return 0;
}


/* Creates the sequence to store data to MEM from external ddr memory ddr_src*/
int DDR_to_Mem_Tile_Copy(XAie_DevInst* dev, uint32_t num_elems, uint8_t col)
{
        AieRC RC = XAIE_OK;

        XAie_LocType Tile_M, Tile_S;
        Tile_M = XAie_TileLoc(col, UINT8_C(1));   // MEM Tile
        Tile_S = XAie_TileLoc(col, UINT8_C(0));   // SHIM Tile

        XAie_DmaDesc Tile_M_S2MM, Tile_S_MM2S;

        /* Configure stream switch ports to move data from DMA port to SOUTH*/
        RC = XAie_StrmConnCctEnable(dev, Tile_M, SOUTH, 0, DMA, 0);
        gen_XAie_check(RC);
        RC = XAie_StrmConnCctEnable(dev, Tile_S, SOUTH, 3, NORTH, 0);
        gen_XAie_check(RC);
        RC = XAie_EnableShimDmaToAieStrmPort(dev, Tile_S, 3); // this is needed because the south port 3 is also used as DMA. 3 for output
        gen_XAie_check(RC);

        // create BDs
        RC = XAie_DmaDescInit(dev, &Tile_M_S2MM, Tile_M);
        gen_XAie_check(RC);
        RC = XAie_DmaDescInit(dev, &Tile_S_MM2S, Tile_S);
        gen_XAie_check(RC);

        /* Configure address and length in dma software descriptors */
        RC = XAie_DmaSetAddrLen(&Tile_S_MM2S, DEFAULT_UNPATCHED_ADDR, num_elems * sizeof(uint32_t)); // Read from external ddr mem
        gen_XAie_check(RC);
        RC = XAie_DmaSetAddrLen(&Tile_M_S2MM, START_OF_MEM, num_elems * sizeof(uint32_t)); // Write to local address 0 of MEM tile
        gen_XAie_check(RC);

        RC = XAie_DmaEnableBd(&Tile_M_S2MM);
        gen_XAie_check(RC);
        RC = XAie_DmaEnableBd(&Tile_S_MM2S);
        gen_XAie_check(RC);

        RC = XAie_DmaSetAxi(&Tile_S_MM2S, 0U, 16U, 0U, 0U, 0U);
        gen_XAie_check(RC);

        RC = XAie_DmaWriteBd(dev, &Tile_S_MM2S, Tile_S, 0U); // BD 0
        gen_XAie_check(RC);
        RC = XAie_DmaWriteBd(dev, &Tile_M_S2MM, Tile_M, 0U); // BD 0
        gen_XAie_check(RC);

        // patch BD
        patch_op_t patch_instr = {};
        uint64_t tile_offset = _XAie_GetTileAddr(dev, 0, col);

        patch_instr.regaddr = XAIEGBL_MEM_DMABD0ADDB + tile_offset;
        patch_instr.argidx = IN_BUFFER_KERNARG_IDX; //argidx points to out_bo
        patch_instr.argplus = 0; //argplus

        RC = XAie_AddCustomTxnOp(dev, patch_ddr_opcode, &patch_instr, sizeof(patch_instr)); // Create patch operation that will set actual address to use at runtime
        gen_XAie_check(RC);

        /* Push Bd numbers to aie dma channel queues and enable the channels */
        RC = XAie_DmaChannelPushBdToQueue(dev, Tile_S, 0U, DMA_MM2S, 0U); // Push BD 0
        gen_XAie_check(RC);
        RC = XAie_DmaChannelPushBdToQueue(dev, Tile_M, 0U, DMA_S2MM, 0U); // Push BD 0
        gen_XAie_check(RC);

        /* Enable the buffer descriptors in software dma descriptors */
        RC = XAie_DmaChannelEnable(dev, Tile_S, 0U, DMA_MM2S); // channel 0
        gen_XAie_check(RC);
        RC = XAie_DmaChannelEnable(dev, Tile_M, 0U, DMA_S2MM); // channel 0
        gen_XAie_check(RC);

        RC = XAie_DmaWaitForDone(dev, Tile_S, 1, DMA_S2MM, 0);
        gen_XAie_check(RC);

        return 0;
}


static void generate_tran_and_elf(const std::string &filename, int type)
{
        // Set config ptr
        XAie_Config ConfigPtr {
                XAIE_DEV_GEN_AIE2P,
                XAIE_BASE_ADDR,
                XAIE_COL_SHIFT,
                XAIE_ROW_SHIFT,
                XAIE_NUM_ROWS,
                XAIE_NUM_COLS,
                XAIE_SHIM_ROW,
                XAIE_MEM_TILE_ROW_START,
                XAIE_MEM_TILE_NUM_ROWS,
                XAIE_AIE_TILE_ROW_START,
                XAIE_AIE_TILE_NUM_ROWS,
                {0}
        };

        // Declare and initialize device instance
        XAie_InstDeclare(DevInst, &ConfigPtr);
        AieRC RC = XAie_CfgInitialize(&(DevInst), &ConfigPtr);
        gen_XAie_check(RC);

        int data_size = (SIZE * 1024 / sizeof(uint32_t));

        RC = XAie_StartTransaction(&DevInst, XAIE_TRANSACTION_DISABLE_AUTO_FLUSH);
        gen_XAie_check(RC);

        if(type == Mem_Tile_to_DDR)
            Mem_Tile_to_DDR_Copy(&DevInst, (uint32_t)data_size, UINT8_C(0));
        else
            DDR_to_Mem_Tile_Copy(&DevInst, (uint32_t)data_size, UINT8_C(0));

        uint8_t *txn_ptr = XAie_ExportSerializedTransaction(&DevInst, 0, 0);

        XAie_TxnHeader* hdr = (XAie_TxnHeader*)txn_ptr;
        std::cout << "Txn Size: " << std::dec << hdr->TxnSize << " bytes" << std::endl;

        // store exported transactions into a binary
        std::ofstream outfile(filename, std::ios::binary);
        outfile.write(reinterpret_cast<const char *>(txn_ptr), hdr->TxnSize);
        outfile.close();

        // wrap transactions into an ELF and store it
        static_assert(std::is_same<unsigned char, uint8_t>::value, "uint8_t is not unsigned char");
        std::vector<char> buf1(txn_ptr, txn_ptr + hdr->TxnSize);
        aiebu::aiebu_assembler as(aiebu::aiebu_assembler::buffer_type::blob_instr_transaction, buf1);

        auto elf = as.get_elf();
        std::ofstream outelffile(filename + ".elf", std::ios::binary);
        outelffile.write(elf.data(), elf.size());
        outelffile.close();

        as.get_report(std::cout);
}

int main(int /* argc */, char** /* argv */)
{
        std::string filename = "copy_Mem_Tile_to_DDR.bin";
        generate_tran_and_elf(filename, Mem_Tile_to_DDR);

        filename = "copy_DDR_to_Mem_Tile.bin";
        generate_tran_and_elf(filename, DDR_to_Mem_Tile);

        return 0;
}
