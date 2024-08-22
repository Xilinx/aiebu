// SPDX-License-Identifier: MIT
// Copyright (C) 2023-2024 Advanced Micro Devices, Inc.

#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <fstream>

// AIE Driver headers
#include "xaiengine.h"
#include "xaiengine/xaiegbl_params.h"

#include "aiebu_assembler.h"

#define XAIE_NUM_COLS_PHX			5
#define XAIE_NUM_COLS_STX			4
#define XAIE_NUM_ROWS				6
#define XAIE_BASE_ADDR				0
#define XAIE_COL_SHIFT				25
#define XAIE_ROW_SHIFT				20
#define XAIE_SHIM_ROW				0
#define XAIE_MEM_TILE_ROW_START		1
#define XAIE_MEM_TILE_NUM_ROWS		1
#define XAIE_AIE_TILE_ROW_START		2
#define XAIE_AIE_TILE_NUM_ROWS		4

#define DEVICE_PHX					0
#define DEVICE_STX					1
#define MEMTILE_SIZE_BYTES			(512 * 1024)

static std::string dev_to_str[] = {
	"PHX",
	"STX",
};

enum PreemptOp {
	PREEMPT_SAVE,
	PREEMPT_RESTORE,
};

constexpr auto START_OF_MEM = 0x80000;
constexpr auto OUT_BUFFER_KERNARG_IDX = 0;
constexpr auto IN_BUFFER_KERNARG_IDX = 1;
constexpr uint8_t patch_ddr_opcode = XAIE_IO_CUSTOM_OP_DDR_PATCH;

/* Creates the sequence to store data from MEM to external ddr memory dst*/
int MEM_Tile_Save_Context(XAie_DevInst* dev, uint64_t num_elems, uint32_t col) {
	assert(num_elems % 2 == 0);
	AieRC RC = XAIE_OK;
	uint64_t size_per_column = num_elems * sizeof(uint32_t);
	uint64_t DEFAULT_UNPATCHED_SAVE_ADDR = col * size_per_column;

	XAie_LocType Tile_M, Tile_S;
	Tile_M = XAie_TileLoc(col, 1);   // MEM Tile
	Tile_S = XAie_TileLoc(col, 0);   // SHIM Tile
	XAie_DmaDesc Tile_M_MM2S, Tile_S_S2MM;

	/* Configure stream switch ports to move data from MEM to SHIM */
	RC = XAie_StrmConnCctEnable(dev, Tile_M, DMA, 0, SOUTH, 0);   // DMA 0 to SOUTH 0
	RC = XAie_StrmConnCctEnable(dev, Tile_S, NORTH, 0, SOUTH, 2); // NORTH to SOUTH 2
	RC = XAie_EnableAieToShimDmaStrmPort(dev, Tile_S, 2); // this is needed because the south port 2 is also used as DMA

	// create BDs
	RC = XAie_DmaDescInit(dev, &Tile_M_MM2S, Tile_M);
	RC = XAie_DmaDescInit(dev, &Tile_S_S2MM, Tile_S);
	/* Configure address and length in dma software descriptors */
	RC = XAie_DmaSetAddrLen(&Tile_M_MM2S, START_OF_MEM, (num_elems / 2) * sizeof(uint32_t)); // Read from local address 0
	RC = XAie_DmaSetAddrLen(&Tile_S_S2MM, DEFAULT_UNPATCHED_SAVE_ADDR, num_elems * sizeof(uint32_t)); // Write to external ddr mem -- address obtained later
	RC = XAie_DmaEnableBd(&Tile_M_MM2S);
	RC = XAie_DmaEnableBd(&Tile_S_S2MM);

	// configure BDs
	RC = XAie_DmaSetAxi(&Tile_S_S2MM, 0U, 16U, 0U, 0U, 0U);
	RC = XAie_DmaSetBdIteration(&Tile_M_MM2S, num_elems / 2, 0, 0); // we are using newer API to support 512KB. Copy 256KB in one BD, execute BD twice

	RC = XAie_DmaWriteBd(dev, &Tile_M_MM2S, Tile_M, 0U); // BD 0
	RC = XAie_DmaWriteBd(dev, &Tile_S_S2MM, Tile_S, 0U); // BD 0

	// patch BD address
	patch_op_t patch_instr = {};
	uint64_t tile_offset = _XAie_GetTileAddr(dev, 0, col);
	patch_instr.regaddr = XAIEGBL_MEM_DMABD0ADDB + tile_offset;
	patch_instr.argidx = OUT_BUFFER_KERNARG_IDX;//argidx points to out_bo
	patch_instr.argplus = DEFAULT_UNPATCHED_SAVE_ADDR; //argplus

	XAie_AddCustomTxnOp(dev, patch_ddr_opcode, &patch_instr, sizeof(patch_instr)); // Create patch operation that will set actual address to use at runtime

	/* Push Bd numbers to aie dma channel queues and enable the channels */
	RC = XAie_DmaChannelSetStartQueue(dev, Tile_M, 0U, DMA_MM2S, 0, 2, 0); // Execute this BD twice. 256 * 2 = 512 KB
	RC = XAie_DmaChannelPushBdToQueue(dev, Tile_S, 0U, DMA_S2MM, 0U); // Push BD 0

	/* Enable the buffer descriptors in software dma descriptors */
	RC = XAie_DmaChannelEnable(dev, Tile_M, 0U, DMA_MM2S); // Enable channel 0
	RC = XAie_DmaChannelEnable(dev, Tile_S, 0U, DMA_S2MM); // Enable channel 0 for DMA

	RC = XAie_DmaWaitForDone(dev, Tile_S, 0, DMA_S2MM, 0); // wait for SHIM DMA to finish
	if (RC != XAIE_OK) {
		std::cout << "Error at " << __LINE__ << std::endl;
		return -1;
	}
	return 0;
}

/* Creates the sequence to store data to MEM from external ddr memory ddr_src*/
int MEM_Tile_Restore_Context(XAie_DevInst* dev, uint64_t num_elems, uint32_t col) {
	assert(num_elems % 2 == 0);
	AieRC RC = XAIE_OK;
	uint64_t size_per_column = num_elems * sizeof(uint32_t);
	uint64_t DEFAULT_UNPATCHED_RESTORE_ADDR = col * size_per_column;

	XAie_LocType Tile_M, Tile_S;
	Tile_M = XAie_TileLoc(col, 1);   // MEM Tile
	Tile_S = XAie_TileLoc(col, 0);   // SHIM Tile
	XAie_DmaDesc Tile_M_S2MM, Tile_S_MM2S;

	/* Configure stream switch ports to move data from DMA port to SOUTH*/
	RC = XAie_StrmConnCctEnable(dev, Tile_M, SOUTH, 0, DMA, 0);
	RC = XAie_StrmConnCctEnable(dev, Tile_S, SOUTH, 3, NORTH, 0);
	RC = XAie_EnableShimDmaToAieStrmPort(dev, Tile_S, 3); // this is needed because the south port 3 is also used as DMA. 3 for output

	// create BDs
	RC = XAie_DmaDescInit(dev, &Tile_M_S2MM, Tile_M);
	RC = XAie_DmaDescInit(dev, &Tile_S_MM2S, Tile_S);

	/* Configure address and length in dma software descriptors */
	RC = XAie_DmaSetAddrLen(&Tile_S_MM2S, DEFAULT_UNPATCHED_RESTORE_ADDR, num_elems * sizeof(uint32_t)); // Read from external ddr mem -- address patched in later
	RC = XAie_DmaSetAddrLen(&Tile_M_S2MM, START_OF_MEM, (num_elems / 2) * sizeof(uint32_t)); // Write to local address 0 of MEM tile
	RC = XAie_DmaEnableBd(&Tile_M_S2MM);
	RC = XAie_DmaEnableBd(&Tile_S_MM2S);

	RC = XAie_DmaSetAxi(&Tile_S_MM2S, 0U, 16U, 0U, 0U, 0U);
	RC = XAie_DmaSetBdIteration(&Tile_M_S2MM, num_elems / 2, 0, 0); // we are using newer API to support 512KB. Copy 256 KB in one BD

	RC = XAie_DmaWriteBd(dev, &Tile_S_MM2S, Tile_S, 0U); // BD 0
	RC = XAie_DmaWriteBd(dev, &Tile_M_S2MM, Tile_M, 0U); // BD 0

	// patch BD
	patch_op_t patch_instr = {};
	uint64_t tile_offset = _XAie_GetTileAddr(dev, 0, col);
	patch_instr.regaddr = XAIEGBL_MEM_DMABD0ADDB + tile_offset;
	patch_instr.argidx = IN_BUFFER_KERNARG_IDX;//argidx points to out_bo
	patch_instr.argplus = DEFAULT_UNPATCHED_RESTORE_ADDR; //argplus

	XAie_AddCustomTxnOp(dev, patch_ddr_opcode, &patch_instr, sizeof(patch_instr)); // Create patch operation that will set actual address to use at runtime

	/* Push Bd numbers to aie dma channel queues and enable the channels */
	RC = XAie_DmaChannelPushBdToQueue(dev, Tile_S, 0U, DMA_MM2S, 0U);
	RC = XAie_DmaChannelSetStartQueue(dev, Tile_M, 0U, DMA_S2MM, 0, 2, 0); // Execute this BD twice. 256 * 2 = 512 KB

	/* Enable the buffer descriptors in software dma descriptors */
	RC = XAie_DmaChannelEnable(dev, Tile_S, 0U, DMA_MM2S); // channel 0
	RC = XAie_DmaChannelEnable(dev, Tile_M, 0U, DMA_S2MM); // channel 0

	RC = XAie_DmaWaitForDone(dev, Tile_M, 0, DMA_S2MM, 0);

	if (RC != XAIE_OK) {
		std::cout << "Error at " << __LINE__ << std::endl;
		return -1;
	}
	return 0;
}

// The two functions below only needed for col0 on PHX
// They work assuming the above functions could also be running on other cols
int MEM_Tile_Save_Context_Col0_PHX(XAie_DevInst* dev, uint64_t num_elems, uint32_t col) {
	assert(num_elems % 2 == 0);

	uint64_t size_per_column = num_elems * sizeof(uint32_t);
	AieRC RC = XAIE_OK;
	XAie_LocType Tile_M, Tile_S, Tile_S_E;
	uint64_t DEFAULT_UNPATCHED_SAVE_ADDR = col * size_per_column;

	Tile_M = XAie_TileLoc(0, 1);   // MEM Tile
	Tile_S = XAie_TileLoc(0, 0);   // SHIM Tile
	Tile_S_E = XAie_TileLoc(1, 0); // SHIM-col1 Tile

	XAie_DmaDesc Tile_M_MM2S, Tile_S_S2MM;

	/* Configure stream switch ports to move data from MEM to SHIM */
	RC = XAie_StrmConnCctEnable(dev, Tile_M, DMA, 0, SOUTH, 0);    // DMA 0 to SOUTH 0
	RC = XAie_StrmConnCctEnable(dev, Tile_S, NORTH, 0, EAST, 0);   // NORTH 0 to EAST 0
	RC = XAie_StrmConnCctEnable(dev, Tile_S_E, WEST, 0, SOUTH, 3); // WEST 0 to SOUTH 3
	RC = XAie_EnableAieToShimDmaStrmPort(dev, Tile_S_E, 3); // this is needed because the south port 3 is also used as DMA

	// create BDs
	RC = XAie_DmaDescInit(dev, &Tile_M_MM2S, Tile_M);
	RC = XAie_DmaDescInit(dev, &Tile_S_S2MM, Tile_S_E);
	/* Configure address and length in dma software descriptors */
	RC = XAie_DmaSetAddrLen(&Tile_M_MM2S, START_OF_MEM, (num_elems / 2) * sizeof(uint32_t));
	RC = XAie_DmaSetAddrLen(&Tile_S_S2MM, DEFAULT_UNPATCHED_SAVE_ADDR, num_elems * sizeof(uint32_t)); // write to external ddr address 0 -- patch address later
	RC = XAie_DmaEnableBd(&Tile_M_MM2S);
	RC = XAie_DmaEnableBd(&Tile_S_S2MM);

	// configure BDs
	RC = XAie_DmaSetAxi(&Tile_S_S2MM, 0U, 16U, 0U, 0U, 0U);
	RC = XAie_DmaSetBdIteration(&Tile_M_MM2S, num_elems / 2, 0, 0); // copy half the elements, run BD twice

	RC = XAie_DmaWriteBd(dev, &Tile_M_MM2S, Tile_M, 0); // BD num 0
	RC = XAie_DmaWriteBd(dev, &Tile_S_S2MM, Tile_S_E, 1); // BD num 1

	// patch BD address
	patch_op_t patch_instr = {};
	uint64_t tile_offset = _XAie_GetTileAddr(dev, 0, col + 1); // SHIM in col 1 is doing the DMA
	patch_instr.regaddr = XAIEGBL_MEM_DMABD1ADDB + tile_offset; // taking BD 1 again
	patch_instr.argidx = OUT_BUFFER_KERNARG_IDX; //argidx points to out_bo
	patch_instr.argplus = DEFAULT_UNPATCHED_SAVE_ADDR; //argplus

	XAie_AddCustomTxnOp(dev, patch_ddr_opcode, &patch_instr, sizeof(patch_instr)); // Create patch operation that will set actual address to use at runtime

	/* Push Bd numbers to aie dma channel queues and enable the channels */
	RC = XAie_DmaChannelSetStartQueue(dev, Tile_M, 0, DMA_MM2S, 0, 2, 0); // Execute this BD twice. 256 * 2 = 512 KB
	RC = XAie_DmaChannelPushBdToQueue(dev, Tile_S_E, 1, DMA_S2MM, 1); // BD num 1

	/* Enable the buffer descriptors in software dma descriptors */
	RC = XAie_DmaChannelEnable(dev, Tile_M, 0, DMA_MM2S);
	RC = XAie_DmaChannelEnable(dev, Tile_S_E, 1, DMA_S2MM);

	RC = XAie_DmaWaitForDone(dev, Tile_S_E, 1, DMA_S2MM, 0); // need to wait for channel 1

	if (RC != XAIE_OK) {
		std::cout << "Error at " << __LINE__ << std::endl;
		return -1;
	}
	return 0;
}

int MEM_Tile_Restore_Context_Col0_PHX(XAie_DevInst* dev, uint64_t num_elems, uint32_t col) {
	assert(num_elems % 2 == 0);

	AieRC RC = XAIE_OK;
	uint64_t size_per_column = num_elems * sizeof(uint32_t);
	uint64_t DEFAULT_UNPATCHED_RESTORE_ADDR = col * size_per_column;

	XAie_LocType Tile_M, Tile_S, Tile_S_E;

	Tile_M =   XAie_TileLoc(0,   1);   // MEM Tile
	Tile_S =   XAie_TileLoc(0,   0);   // SHIM Tile
	Tile_S_E = XAie_TileLoc(1,   0);   // SHIM Tile col 1

	XAie_DmaDesc Tile_M_S2MM, Tile_S_MM2S;

	/* Configure stream switch ports to move data from SOUTH to DMA port*/
	RC = XAie_StrmConnCctEnable(dev, Tile_M, SOUTH, 0, DMA, 0);  // SOUTH 0 to DMA 0
	RC = XAie_StrmConnCctEnable(dev, Tile_S, EAST, 0, NORTH, 0); // EAST 0 to NORTH 0

	RC = XAie_StrmConnCctEnable(dev, Tile_S_E, SOUTH, 7, WEST, 0);  // SOUTH 7 to WEST 0
	RC = XAie_EnableShimDmaToAieStrmPort(dev, Tile_S_E, 7); // this is needed because the south port 7 is also used as DMA. 7 for input

	// create BDs
	RC = XAie_DmaDescInit(dev, &Tile_M_S2MM, Tile_M);
	RC = XAie_DmaDescInit(dev, &Tile_S_MM2S, Tile_S_E);

	/* Configure address and length in dma software descriptors */
	RC = XAie_DmaSetAddrLen(&Tile_S_MM2S, DEFAULT_UNPATCHED_RESTORE_ADDR, num_elems * sizeof(uint32_t));
	RC = XAie_DmaSetAddrLen(&Tile_M_S2MM, START_OF_MEM, (num_elems / 2) * sizeof(uint32_t));
	RC = XAie_DmaEnableBd(&Tile_M_S2MM);
	RC = XAie_DmaEnableBd(&Tile_S_MM2S);

	RC = XAie_DmaSetAxi(&Tile_S_MM2S, 0U, 16U, 0U, 0U, 0U);
	RC = XAie_DmaSetBdIteration(&Tile_M_S2MM, num_elems / 2, 0, 0); // we are using newer API to support 512KB. Copy 256 KB in one BD

	RC = XAie_DmaWriteBd(dev, &Tile_S_MM2S, Tile_S_E, 1U); // BD 1
	RC = XAie_DmaWriteBd(dev, &Tile_M_S2MM, Tile_M, 0U); // BD 0

	// patch BD address
	patch_op_t patch_instr = {};
	uint64_t tile_offset = _XAie_GetTileAddr(dev, 0, col + 1); // SHIM tile in col 1 is doing the DMA
	patch_instr.regaddr = XAIEGBL_MEM_DMABD1ADDB + tile_offset; // taking BD 1 again
	patch_instr.argidx = IN_BUFFER_KERNARG_IDX; //argidx points to inp_bo
	patch_instr.argplus = DEFAULT_UNPATCHED_RESTORE_ADDR; //argplus

	XAie_AddCustomTxnOp(dev, patch_ddr_opcode, &patch_instr, sizeof(patch_instr)); // Create patch operation that will set actual address to use at runtime

	/* Push Bd numbers to aie dma channel queues and enable the channels */
	RC = XAie_DmaChannelPushBdToQueue(dev, Tile_S_E, 1U, DMA_MM2S, 1U); // BD 1
	RC = XAie_DmaChannelSetStartQueue(dev, Tile_M, 0U, DMA_S2MM, 0, 2, 0); // Execute this BD twice. 256 * 2 = 512 KB

	/* Enable the buffer descriptors in software dma descriptors */
	RC = XAie_DmaChannelEnable(dev, Tile_S_E, 1U, DMA_MM2S); // Enable channel 1
	RC = XAie_DmaChannelEnable(dev, Tile_M, 0U, DMA_S2MM);   // Enable channel 0

	XAie_DmaWaitForDone(dev, Tile_M, 0, DMA_S2MM, 0);
	if (RC != XAIE_OK) {
		std::cout << "Error at " << __LINE__ << std::endl;
		return -1;
	}

	return 0;
}

// Function to convert a string to lowercase
std::string to_lower_copy(const std::string& input) {
	std::string result = input; // Create a copy of the input string
	std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
		return std::tolower(c);
	});
	return result;
}

static std::string tran_filename(uint8_t device, enum PreemptOp type, uint32_t ncol)
{
	std::string filename;
	switch (type) {
		case PREEMPT_SAVE:
			filename = "preempt_save_";
			break;
		case PREEMPT_RESTORE:
			filename = "preempt_restore_";
			break;
		default:
			std::cout << "Error: Invalid PreemptOp type\n";
			return NULL;
	};
	filename += to_lower_copy(dev_to_str[device]) + "_4x" + std::to_string(ncol) + ".bin";
	return filename;
}

static int generate_tran(uint8_t device, enum PreemptOp type, uint32_t start_col, uint32_t ncol)
{
	int data_sz = (MEMTILE_SIZE_BYTES / sizeof(uint32_t));
	uint8_t XAIE_DEV_GEN, XAIE_NUM_COLS;
	int ret;

	if (device == DEVICE_PHX && ncol == 1 && start_col == 0) {
		std::cout << "Invalid start column for device type\n";
		return -1;
	}

	if (device == DEVICE_STX) {
		if (ncol + start_col > XAIE_NUM_COLS_STX) {
			std::cout << "Invalid number of columns for device type\n";
			return -1;
		}
		XAIE_NUM_COLS = XAIE_NUM_COLS_STX;
		XAIE_DEV_GEN = XAIE_DEV_GEN_AIE2P;
	} else {
		if (ncol + start_col > XAIE_NUM_COLS_PHX) {
			std::cout << "Invalid number of columns for device type\n";
			return -1;
		}
		XAIE_NUM_COLS = XAIE_NUM_COLS_PHX;
		XAIE_DEV_GEN = XAIE_DEV_GEN_AIE2IPU;
	}

	XAie_Config ConfigPtr {
		XAIE_DEV_GEN,
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

	XAie_InstDeclare(DevInst, &ConfigPtr);
	XAie_SetupPartitionConfig(&DevInst, XAIE_BASE_ADDR, start_col, ncol);
	XAie_CfgInitialize(&(DevInst), &ConfigPtr);

	XAie_StartTransaction(&DevInst, XAIE_TRANSACTION_DISABLE_AUTO_FLUSH);

	for (int col = 0; col < ncol; col++) {
		if (type == PREEMPT_SAVE) {
			ret = MEM_Tile_Save_Context(&DevInst, data_sz, col);
			if (ret)
				return ret;
		} else {
			ret = MEM_Tile_Restore_Context(&DevInst, data_sz, col);
			if (ret)
				return ret;
		}
	}

	uint8_t *txn_ptr = XAie_ExportSerializedTransaction_opt(&DevInst, 0, 0);
	XAie_TxnHeader* hdr = (XAie_TxnHeader*)txn_ptr;
	std::cout << "Transaction Size: 0x" << std::hex << hdr->TxnSize << " bytes\n";

	std::string filename = tran_filename(device, type, ncol);
	std::ofstream outfile(filename, std::ios::binary);
	outfile.write(reinterpret_cast<const char *>(txn_ptr), hdr->TxnSize);
	outfile.close();

	static_assert(std::is_same<unsigned char, uint8_t>::value, "uint8_t is not unsigned char");
	std::vector<char> buf1(txn_ptr, txn_ptr + hdr->TxnSize);
	aiebu::aiebu_assembler as(aiebu::aiebu_assembler::buffer_type::blob_instr_transaction, buf1);
	auto elf = as.get_elf();
	std::ofstream outelffile(filename + ".elf", std::ios::binary);
	outelffile.write(elf.data(), elf.size());
	outelffile.close();
	as.get_report(std::cout);

	XAie_ClearTransaction(&(DevInst));
	return 0;
}

int main(int argc, char** argv)
{
	uint32_t start_col, ncol;
	int ret;

	const unsigned int columns[] = {1, 2, 4};

	/* STX */
	start_col = 0;
	for (auto ncol : columns) {
		ret = generate_tran(DEVICE_STX, PREEMPT_SAVE, start_col, ncol);
		if (ret) {
			std::cout << "Failed to generate save " << dev_to_str[DEVICE_STX] << " TXN bin for start col: " << start_col << " ncol: " << ncol  << "\n";
			return ret;
		}
		std::cout << "Successfully generated " << dev_to_str[DEVICE_STX] << " 4x" << ncol << " save transaction binary!\n";
		ret = generate_tran(DEVICE_STX, PREEMPT_RESTORE, start_col, ncol);
		if (ret) {
			std::cout << "Failed to generate restore " << dev_to_str[DEVICE_STX] << " TXN bin for start col: " << start_col << " ncol: " << ncol  << "\n";
			return ret;
		}
		std::cout << "Successfully generated " << dev_to_str[DEVICE_STX] << " 4x" << ncol << " restore transaction binary!\n";
	}

	/* PHX */
	start_col = 1;
	for (auto ncol : columns) {
		ret = generate_tran(DEVICE_PHX, PREEMPT_SAVE, start_col, ncol);
		if (ret) {
			std::cout << "Failed to generate save " << dev_to_str[DEVICE_PHX] << " TXN bin for start col: " << start_col << " ncol: " << ncol  << "\n";
			return ret;
		}
		std::cout << "Successfully generated " << dev_to_str[DEVICE_PHX] << " 4x" << ncol << " save transaction binary!\n";
		ret = generate_tran(DEVICE_PHX, PREEMPT_RESTORE, start_col, ncol);
		if (ret) {
			std::cout << "Failed to generate restore " << dev_to_str[DEVICE_PHX] << " TXN bin for start col: " << start_col << " ncol: " << ncol  << "\n";
			return ret;
		}
		std::cout << "Successfully generated " << dev_to_str[DEVICE_PHX] << " 4x" << ncol << " restore transaction binary!\n";
	}
	return 0;
}
