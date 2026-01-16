// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_TRANSFORM_MANAGER_H_
#define AIEBU_TRANSFORM_MANAGER_H_

#include <cstdint>
#include <unordered_map>

#include <elfio/elfio.hpp>
#include "specification/aie2ps/isa.h"
#include "common/symbol.h"

namespace aiebu {

/**
 * @struct arginfo
 * @brief Represents XRT argument index and buffer descriptor offset pair
 */
struct arginfo {
  uint32_t xrt_idx;
  uint64_t bd_offset;
};

/**
 * @class transform_manager
 * @brief Manages ELF transformation for AIE control code and data
 *
 * This class handles the transformation of AIE ELF files by:
 * - Loading and parsing AIE2PS/AIE4 ELF binaries
 * - Extracting and updating relocation sections (.rela.dyn)
 * - Modifying buffer descriptor (BD) offsets in control code and control packets
 * - Processing apply_offset_57 opcodes for argument passing
 * - Managing symbol tables and dynamic relocations
 */
class transform_manager {

  // Section and format constants
  static constexpr size_t elf_section_header_size = 16;    // ELF-specific header padding size
  static constexpr uint8_t align_opcode = 0xA5;            // .align pseudo-instruction opcode
  static constexpr size_t ctrltext_string_length = 9;      // Length of ".ctrltext" prefix
  static constexpr size_t ctrldata_string_length = 9;      // Length of ".ctrldata" prefix
  static constexpr size_t ctrlpkt_string_length = 8;       // Length of ".ctrlpkt" prefix
  static constexpr size_t ctrlcode_string_length = 13;     // Length of "control-code" prefix

  // AIE ELF OS ABI identifiers
  static constexpr uint8_t elf_amd_aie2ps_group = 70;      // AIE2PS/AIE4 group ELF format

  // Register offset multiplier (2 for 32-bit registers = 64-bit offset)
  static constexpr uint8_t num_32bit_register = 2;

  // Member variables
  ELFIO::elfio m_elfio;                                     // ELF file parser and writer
  std::unordered_map<std::string, uint32_t> xrt_idx_lookup;// Maps (offset,section) key to XRT argument index
  const std::map<uint8_t, isa_op_disasm>* isa_op_map;      // ISA opcode to instruction mapping
  isa_disassembler m_isa_disassembler;                      // ISA disassembler instance

  /**
   * @brief Generate unique lookup key from offset and section index
   * @param offset Offset within the section
   * @param section_idx Section index in ELF
   * @return Unique key string combining offset and section index
   */
  std::string get_key(uint32_t offset, uint32_t section_idx) const
  {
    // Lookup key as offset and section idx combination will be always unique
    return std::to_string(offset) + "_" + std::to_string(section_idx);
  }

  // Instruction processing methods
  // Calculate instruction size in bytes
  uint32_t size(const isa_op_disasm& op) const;

  /**
   * @brief Modify apply_offset_57 opcodes with XRT argument indices
   * @param text_section_data Pointer to text section data
   * @param text_section_size Size of text section
   * @param section_idx Section index in ELF
   */
  void modify_apply_offset_57(char* text_section_data, size_t text_section_size, uint32_t section_idx);

  void process_sections();

  /**
   * @brief Update UUID in .note.xrt.UID section based on all PROGBITS sections
   *
   * Computes MD5 hash of all SHT_PROGBITS section data and updates the
   * existing .note.xrt.UID note section with the new hash value.
   */
  void update_uid_section();

  /**
   * @brief Extract column and page from section name
   * @param name Section name (e.g., ".ctrltext.0.1" or ".ctrltext.0.1.id")
   * @return Pair of column and page indices
   */
  std::pair<uint32_t, uint32_t> get_column_and_page(const std::string& name) const;

  uint64_t read57(const uint32_t* bd_data_ptr) const;
  uint64_t read57_aie4(const uint32_t* bd_data_ptr) const;
  void write57(uint32_t* bd_data_ptr, uint64_t bd_offset);
  void write57_aie4(uint32_t* bd_data_ptr, uint64_t bd_offset);
  uint64_t ctrlpkt_read57(const uint32_t* bd_data_ptr) const;
  void ctrlpkt_write57(uint32_t* bd_data_ptr, uint64_t bd_offset);
  uint64_t ctrlpkt_read57_aie4(const uint32_t* bd_data_ptr) const;
  void ctrlpkt_write57_aie4(uint32_t* bd_data_ptr, uint64_t bd_offset);
  uint64_t get_controlcode_bd_offset(const std::string& section_name, uint32_t offset, symbol::patch_schema schema);
  void set_controlcode_bd_offset(const std::string& section_name, uint32_t offset, uint64_t bd_offset, symbol::patch_schema schema);
  uint64_t get_ctrlpkt_bd_offset(const std::string& section_name, uint32_t offset, symbol::patch_schema schema);
  void set_ctrlpkt_bd_offset(const std::string& section_name, uint32_t offset, uint64_t bd_offset, symbol::patch_schema schema);

  /**
   * @brief Extract group ID from section name if it's a group ELF
   * @param name Section name (e.g., ".ctrltext.0.1.groupid")
   * @return Group ID string or empty string if not a group ELF
   */
  std::string get_grp_id_if_group_elf(const std::string& name) const;

  /**
   * @brief Extract kernel name from C++ mangled symbol
   * @param symbol_name: Mangled symbol name (e.g., "_Z3DPUPcPc")
   * @return Kernel name if found, empty string otherwise
   *
   * Parses C++ mangled names in format _Z<length><name>... and extracts the identifier.
   * Example: "_Z3DPUPcPc" -> "DPU" (length=3, name=DPU)
   */
  std::string extract_kernel_name_from_mangled(const std::string& symbol_name) const;

  /**
   * @brief Get filtered section indices for a kernel:instance filter
   * @param kernel_instance_filter: Filter in format "kernel:instance" (e.g., "DPU:dpu")
   * @return Set of section indices that belong to the specified kernel:instance
   *
   * This method:
   * 1. Parses filter string to extract kernel and instance names
   * 2. Finds FUNC symbols matching kernel name (e.g., _Z3DPUPcPc -> "DPU")
   * 3. Finds OBJECT symbols matching instance name (e.g., "dpu")
   * 4. Traverses all sections to find group sections where sh_info points to instance
   * 5. Extracts member section indices from group data
   *
   * @throws error if filter format is invalid, kernel/instance not found, or required sections missing
   */
  std::set<ELFIO::Elf_Half> get_filtered_section_indices(const std::string& kernel_instance_filter);

  /**
   * @brief Generate control text section name
   * @param col Column index
   * @param page Page index
   * @param id Group ID (empty for non-group ELF)
   * @return Section name like ".ctrltext.col.page" or ".ctrltext.col.page.id"
   */
  static std::string get_ctrltext_section_name(uint32_t col, uint32_t page, const std::string& id)
  {
    if (id.empty())
      return ".ctrltext." + std::to_string(col) + "." + std::to_string(page);
    else
      return ".ctrltext." + std::to_string(col) + "." + std::to_string(page) + "." + id;
  }

  /**
   * @brief Generate control data section name
   * @param col Column index
   * @param page Page index
   * @param id Group ID (empty for non-group ELF)
   * @return Section name like ".ctrldata.col.page" or ".ctrldata.col.page.id"
   */
  static std::string get_ctrldata_section_name(uint32_t col, uint32_t page, const std::string& id)
  {
    if (id.empty())
      return ".ctrldata." + std::to_string(col) + "." + std::to_string(page);
    else
      return ".ctrldata." + std::to_string(col) + "." + std::to_string(page) + "." + id;
  }

  /**
   * @brief Check if section name is a control text section
   * @param section_name Section name to check
   * @return true if section starts with ".ctrltext"
   */
  static bool is_text_section(const std::string& section_name)
  {
    return section_name.size() >= ctrltext_string_length &&
           section_name.compare(0, ctrltext_string_length, ".ctrltext") == 0;
  }

  /**
   * @brief Check if section name is control text or data section
   * @param str Section name to check
   * @return true if section starts with ".ctrltext" or ".ctrldata"
   */
  static bool is_text_or_data_section_name(const std::string& str)
  {
    return (str.size() >= ctrltext_string_length &&
            str.compare(0, ctrltext_string_length, ".ctrltext") == 0) ||
           (str.size() >= ctrldata_string_length &&
            str.compare(0, ctrldata_string_length, ".ctrldata") == 0);
  }

  /**
   * @brief Check if section name is a control packet section
   * @param str Section name to check
   * @return true if section starts with ".ctrlpkt"
   */
  static bool is_ctrlpkt_section_name(const std::string& str)
  {
    return str.size() >= ctrlpkt_string_length &&
           str.compare(0, ctrlpkt_string_length, ".ctrlpkt") == 0;
  }

  /**
   * @brief Check if symbol name is a control packet patch
   * @param str Symbol name to check
   * @return true if name starts with ".ctrlpkt"
   */
  static bool is_ctrlpkt_patch_name(const std::string& str)
  {
    return str.size() >= ctrlpkt_string_length &&
           str.compare(0, ctrlpkt_string_length, ".ctrlpkt") == 0;
  }

  /**
   * @brief Check if symbol name is a control code patch
   * @param str Symbol name to check
   * @return true if name starts with "control-code"
   */
  static bool is_controlcode_patch_name(const std::string& str)
  {
    return str.size() >= ctrlcode_string_length &&
           str.compare(0, ctrlcode_string_length, "control-code") == 0;
  }

public:
  /**
   * @brief Construct transform_manager and load ELF data
   * @param elf_data Vector containing ELF binary data
   * @throws error if ELF data is invalid or not AIE2PS/AIE4 format
   */
  explicit transform_manager(const std::vector<char>& elf_data);

  /**
   * @brief Load and validate ELF data
   * @param elf_data Vector containing ELF binary data
   * @throws error if ELF data is invalid or not AIE2PS/AIE4 format
   */
  void load_elf(const std::vector<char>& elf_data);

  ~transform_manager() = default;

  transform_manager(const transform_manager &) = delete;
  transform_manager(transform_manager &&) = delete;
  transform_manager &operator=(const transform_manager &) = delete;
  transform_manager& operator=(transform_manager &&) = delete;

  /**
   * @brief Extract argument information from relocation sections
   * @param kernel_instance_filter:  filter in format "kernel:instance" (e.g., "DPU:dpu")
   *                                 If empty, extracts all relocations.
   *                                 Kernel name is matched from FUNC symbols (_Z3DPUPcPc -> "DPU")
   *                                 Instance name is matched from OBJECT symbols, then traverses all
   *                                 sections to find group sections where sh_info points to instance,
   *                                 extracts member sections from group data for filtering
   * @return Vector of arginfo containing XRT indices and BD offsets
   *
   * This method parses .rela.dyn, .dynsym, and .dynstr sections to extract
   * buffer descriptor offsets for kernel arguments, excluding control-code
   * and ctrlpkt special patches. When filtering, traverses all sections to find
   * group sections (SHT_GROUP) whose sh_info field references the instance symbol.
   */
  std::vector<arginfo> extract_rela_sections(const std::string& kernel_instance_filter);

  /**
   * @brief Update ELF with new argument information
   * @param entries: Vector of arginfo with updated XRT indices and BD offsets
   * @param kernel_instance_filter:  filter in format "kernel:instance" (e.g., "DPU::dpu")
   *                                 If empty, updates all relocations.
   *                                 If specified, only updates relocations for the matching kernel:instance
   * @return Modified ELF binary as vector of chars
   *
   * This method:
   * 1. Optionally filters relocations to specific kernel:instance (same logic as extract_rela_sections)
   * 2. Updates symbol names in .dynsym with new XRT indices
   * 3. Patches BD offsets in control code and control packet sections
   * 4. Updates apply_offset_57 opcodes with xrt_id
   * 5. Rebuilds .dynstr with new symbol names
   * 6. Returns the modified ELF binary
   */
  std::vector<char> update_rela_sections(const std::vector<arginfo>& entries,
                                         const std::string& kernel_instance_filter);

  /**
   * @brief Update kernel name in ELF .symtab and .strtab sections
   * @param orig_name: Original kernel name to find and replace
   * @param new_name: New kernel name to use as replacement
   * @return Modified ELF binary as vector of chars
   *
   * For C++ mangled names, matches the exact identifier (e.g., "DPU" matches "_Z3DPUPcPc"
   * but not "_Z4DPU1PcPc"). Automatically updates length prefixes (_Z3DPU -> _Z4XCVB).
   * For non-mangled names, does exact string matching.
   * Only supports OS ABI 0x46 and ABI version 0x3.
   *
   * @throws error if format unsupported, sections missing, or name not found
   */
   std::vector<char> update_kernel_name(const std::string& orig_name, const std::string& new_name);

  /**
   * @brief Get all instance names for a given kernel
   * @param kernel_name: Kernel name to find instances for (e.g., "DPU")
   * @return Vector of instance names belonging to the kernel
   *
   * This method:
   * 1. Finds FUNC symbol matching kernel name (extracts from mangled name like _Z3DPUPcPc)
   * 2. Finds all OBJECT symbols whose st_shndx points to the kernel symbol
   * 3. Returns the instance names
   *
   * @throws error if kernel not found or required sections missing
   */
   std::vector<std::string> get_kernel_instances(const std::string& kernel_name);
};

}

#endif
