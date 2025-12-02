// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include <cstdint>
#include <boost/interprocess/streams/bufferstream.hpp>
#include <iterator>

#include <elfio/elfio.hpp>
#include <elfio/elfio_section.hpp>
#include "aiebu/aiebu_assembler.h"
#include "aiebu/aiebu_error.h"
#include "specification/aie2ps/isa.h"
#include "ops/ops.h"
#include "common/symbol.h"
#include "common/utils.h"
#include "analyzer/transform_manager.h"

namespace aiebu {

/**
 * @brief Structure representing the apply_offset_57 opcode format
 *
 * This opcode is used to apply offsets from a table to buffer descriptors.
 * The opcode specifies a table pointer and number of entries to process.
 */
struct apply_offset_57 {
  uint8_t opcode;        // Opcode identifier
  uint8_t pad;           // Padding byte
  uint16_t table_ptr;    // Pointer to offset table
  uint16_t num_entries;  // Number of entries in table
  uint16_t offset;       // Offset value to be modified
};

/**
 * @brief Constructor - loads ELF data and initializes ISA map
 * @param elf_data Raw ELF binary data
 */
transform_manager::
transform_manager(const std::vector<char>& elf_data)
{
  load_elf(elf_data);
  isa_op_map = m_isa_disassembler.get_isa_map();
}

/**
 * @brief Load and validate ELF binary
 * @param elf_data Raw ELF binary data
 * @throws error if data is empty, invalid, or not AIE2PS/AIE4 format
 */
void
transform_manager::
load_elf(const std::vector<char>& elf_data)
{
  if (elf_data.empty())
    throw error(error::error_code::invalid_input, "Input buffer is empty");

  // Create in-memory stream from buffer
  boost::interprocess::ibufferstream istr(elf_data.data(), elf_data.size());

  if (!m_elfio.load(istr))
    throw error(error::error_code::invalid_input, "Failed to load ELF from buffer\n");

  // Only AIE2PS/AIE4 legacy ELF and group ELF formats are supported
  auto os_abi = m_elfio.get_os_abi();
  if (os_abi != elf_amd_aie2ps && os_abi != elf_amd_aie2ps_group)
    throw error(error::error_code::invalid_input, "Only aie2ps/aie4 elf supported\n");
}

/**
 * @brief Calculate total size of an instruction in bytes
 * @param op ISA operation descriptor
 * @return Size in bytes (opcode + pad + arguments)
 */
uint32_t
transform_manager::
size(const isa_op_disasm& op) const
{
  uint32_t total_width = 2; // 1 byte opcode + 1 byte padding
  for (const auto& arg : op.get_args())
    total_width += (arg.get_width() / byte_to_bits);

  return total_width;
}

/**
 * @brief Modify apply_offset_57 opcodes to use register offsets instead of table pointers
 * @param text_section_data Pointer to text section data
 * @param text_section_size Size of text section in bytes
 * @param section_idx Section index in ELF
 *
 * This function scans through the text section, finds apply_offset_57 opcodes,
 * and replaces table pointers with actual register offsets for kernel arguments.
 * Special patches (.ctrlpkt-idx, control-code-idx) are left unchanged.
 */
void
transform_manager::
modify_apply_offset_57(char* text_section_data, size_t text_section_size, uint32_t section_idx)
{
  // Skip ELF section header and process instructions
  for (size_t offset = elf_section_header_size; offset < text_section_size;) {
    uint8_t opcode = *reinterpret_cast<const uint8_t*>(text_section_data + offset);

    // Skip alignment padding bytes
    if (opcode == align_opcode) {
      ++offset;
      continue;
    }

    // Look up opcode in ISA map
    auto op_it = isa_op_map->find(opcode);
    if (op_it == isa_op_map->end())
      throw error(error::error_code::invalid_asm, "Unknown Opcode:" + std::to_string(opcode) + " at position " + std::to_string(offset) + "\n");

    // Process apply_offset_57 opcode
    if (opcode == OPCODE_APPLY_OFFSET_57) {
      auto code = reinterpret_cast<apply_offset_57*>(text_section_data + offset);
      auto key = get_key(code->table_ptr, section_idx);
      // If key found, it's a kernel arg; otherwise it's .ctrlpkt-idx or control-code-idx
      auto lookup_it = xrt_idx_lookup.find(key);
      if (lookup_it != xrt_idx_lookup.end())
        code->offset = static_cast<uint16_t>(lookup_it->second * num_32bit_register); // Convert xrit_id to register offset
    }

    // Move to next instruction
    offset += size(op_it->second);
  }
}

/**
 * @brief Process all text sections and modify apply_offset_57 opcodes
 *
 * Iterates through all ELF sections and processes .ctrltext sections
 * to update apply_offset_57 opcodes with register offsets.
 */
void
transform_manager::
process_sections() {
  ELFIO::Elf_Half num = 0;
  for (const auto& section_ptr : m_elfio.sections) {
    const ELFIO::section* section = section_ptr.get();
    const auto& section_name = section->get_name();

    // Process only .ctrltext sections with PROGBITS type
    if (is_text_section(section_name) && section->get_type() == ELFIO::SHT_PROGBITS)
      modify_apply_offset_57(const_cast<char *>(section->get_data()), section->get_size(), num);

    ++num;
  }
}

/**
 * @brief Parse column and page indices from section name
 * @param name Section name (e.g., ".ctrltext.2.5" or ".ctrltext.2.5.id")
 * @return Pair of (column, page) indices
 * @throws std::runtime_error if section name format is invalid
 *
 * Supported formats:
 * - .ctrltext.<col>.<page> or .ctrldata.<col>.<page>
 * - .ctrltext.<col>.<page>.<id> or .ctrldata.<col>.<page>.<id> (newer ELFs)
 */
std::pair<uint32_t, uint32_t>
transform_manager::
get_column_and_page(const std::string& name) const
{
  // Max expected tokens: prefix, col, page, id
  constexpr size_t col_token_id  = 1;
  constexpr size_t page_token_id = 2;

  // Split section name by '.' delimiter
  std::vector<std::string> tokens;
  std::stringstream ss(name);
  std::string token;
  while (std::getline(ss, token, '.')) {
    if (!token.empty())
      tokens.emplace_back(std::move(token));
  }

  try {
    if (tokens.size() <= col_token_id)
      return {0, 0}; // Only prefix present

    if (tokens.size() == (col_token_id + 1))
      return {std::stoul(tokens[col_token_id]), 0}; // Only col present

    return {std::stoul(tokens[col_token_id]), std::stoul(tokens[page_token_id])};
  }
  catch (const std::exception&) {
    throw std::runtime_error("Invalid section name passed to parse col or page index\n");
  }
}

/**
 * @brief Extract group ID from section name if it's a group ELF
 * @param name Section name
 * @return Group ID string if present, empty string otherwise
 * @throws std::runtime_error if section name format is invalid
 *
 * Newer group ELFs have section names like:
 * - .ctrltext.<col>.<page>.<id>
 * - .ctrldata.<col>.<page>.<id>
 */
std::string
transform_manager::
get_grp_id_if_group_elf(const std::string& name) const
{
  // Max expected tokens: prefix, col, page, id
  constexpr size_t group_elf_token = 4;

  // Split section name by '.' delimiter
  std::vector<std::string> tokens;
  std::stringstream ss(name);
  std::string token;
  while (std::getline(ss, token, '.')) {
    if (!token.empty())
      tokens.emplace_back(std::move(token));
  }

  try {
    if (tokens.size() == group_elf_token)
      return tokens[group_elf_token -1];  // Return the ID token
  }
  catch (const std::exception&) {
    throw std::runtime_error("Invalid section name passed to parse col or page index\n");
  }
  return "";  // Not a group ELF
}

/**
 * @brief Read 57-bit buffer descriptor base address for AIE2PS
 * @param bd_data_ptr Pointer to buffer descriptor data (32-bit words)
 * @return 57-bit base address
 *
 * AIE2PS BD format: bits [56:48] from bd_data_ptr[8], [47:32] from bd_data_ptr[2], [31:0] from bd_data_ptr[1]
 */
uint64_t
transform_manager::
read57(const uint32_t* bd_data_ptr) const
{
  uint64_t base_address =
    ((static_cast<uint64_t>(bd_data_ptr[8]) & 0x1FF) << 48) |                       // NOLINT
    ((static_cast<uint64_t>(bd_data_ptr[2]) & 0xFFFF) << 32) |                      // NOLINT
    bd_data_ptr[1];
  return base_address;
}

/**
 * @brief Read 57-bit buffer descriptor base address for AIE4
 * @param bd_data_ptr Pointer to buffer descriptor data (32-bit words)
 * @return 57-bit base address
 *
 * AIE4 BD format: bits [56:32] from bd_data_ptr[0], [31:0] from bd_data_ptr[1]
 */
uint64_t
transform_manager::
read57_aie4(const uint32_t* bd_data_ptr) const
{
  uint64_t base_address =
    ((static_cast<uint64_t>(bd_data_ptr[0]) & 0x1FFFFFF) << 32) |                   // NOLINT
    bd_data_ptr[1];
  return base_address;
}

/**
 * @brief Write 57-bit buffer descriptor base address for AIE2PS
 * @param bd_data_ptr Pointer to buffer descriptor data (32-bit words)
 * @param bd_offset 57-bit base address to write
 *
 * Preserves other bits in bd_data_ptr while updating address fields.
 */
void
transform_manager::
write57(uint32_t* bd_data_ptr, uint64_t bd_offset)
{
  bd_data_ptr[1] = static_cast<uint32_t>(bd_offset & 0xFFFFFFFF);                           // NOLINT
  bd_data_ptr[2] = static_cast<uint32_t>((bd_data_ptr[2] & 0xFFFF0000) | ((bd_offset >> 32) & 0xFFFF)); // NOLINT
  bd_data_ptr[8] = static_cast<uint32_t>((bd_data_ptr[8] & 0xFFFFFE00) | ((bd_offset >> 48) & 0x1FF));  // NOLINT
}

/**
 * @brief Write 57-bit buffer descriptor base address for AIE4
 * @param bd_data_ptr Pointer to buffer descriptor data (32-bit words)
 * @param bd_offset 57-bit base address to write
 *
 * Preserves other bits in bd_data_ptr while updating address fields.
 */
void
transform_manager::
write57_aie4(uint32_t* bd_data_ptr, uint64_t bd_offset)
{
  bd_data_ptr[1] = static_cast<uint32_t>(bd_offset & 0xFFFFFFFF);                           // NOLINT
  bd_data_ptr[0] = static_cast<uint32_t>((bd_data_ptr[0] & 0xFE000000) | ((bd_offset >> 32) & 0x1FFFFFF));// NOLINT
}

/**
 * @brief Read buffer descriptor offset from control packet for AIE2PS
 * @param bd_data_ptr Pointer to control packet header (32-bit words)
 * @return Buffer descriptor offset
 *
 * Control packet format: bits [43:32] from bd_data_ptr[3], [31:0] from bd_data_ptr[2]
 */
uint64_t
transform_manager::
ctrlpkt_read57(const uint32_t* bd_data_ptr) const
{
  uint64_t base_address =
    ((static_cast<uint64_t>(bd_data_ptr[3]) & 0xFFF) << 32) |                       // NOLINT
    ((static_cast<uint64_t>(bd_data_ptr[2])));

  return base_address;
}

/**
 * @brief Write buffer descriptor offset to control packet for AIE2PS
 * @param bd_data_ptr Pointer to control packet header (32-bit words)
 * @param bd_offset Buffer descriptor offset to write
 *
 * Preserves other bits while updating offset fields.
 */
void
transform_manager::
ctrlpkt_write57(uint32_t* bd_data_ptr, uint64_t bd_offset)
{
  bd_data_ptr[2] = static_cast<uint32_t>(bd_offset & 0xFFFFFFFC);                           // NOLINT
  bd_data_ptr[3] = static_cast<uint32_t>((bd_data_ptr[3] & 0xFFFF0000) | (bd_offset >> 32));            // NOLINT
}

/**
 * @brief Read buffer descriptor offset from control packet for AIE4
 * @param bd_data_ptr Pointer to control packet header (32-bit words)
 * @return Buffer descriptor offset
 *
 * Control packet format: bits [56:32] from bd_data_ptr[1], [31:0] from bd_data_ptr[2]
 */
uint64_t
transform_manager::
ctrlpkt_read57_aie4(const uint32_t* bd_data_ptr) const
{
  // bd_data_ptr is a pointer to the header of the control code
  uint64_t base_address = (((uint64_t)bd_data_ptr[1] & 0x1FFFFFF) << 32) | bd_data_ptr[2]; // NOLINT
  return base_address;
}

/**
 * @brief Write buffer descriptor offset to control packet for AIE4
 * @param bd_data_ptr Pointer to control packet header (32-bit words)
 * @param bd_offset Buffer descriptor offset to write
 *
 * Preserves other bits while updating offset fields.
 */
void
transform_manager::
ctrlpkt_write57_aie4(uint32_t* bd_data_ptr, uint64_t bd_offset)
{
  bd_data_ptr[2] = static_cast<uint32_t>(bd_offset & 0xFFFFFFFF);                                  // NOLINT
  bd_data_ptr[1] = static_cast<uint32_t>((bd_data_ptr[1] & 0xFE000000) | ((bd_offset >> 32) & 0x1FFFFFF));     // NOLINT
}

/**
 * @brief Get buffer descriptor offset from control code section
 * @param section_name: Section name containing the BD
 * @param offset: Offset within the combined ctrltext+ctrldata section
 * @param schema: Patch schema indicating format (AIE2PS or AIE4)
 * @return Buffer descriptor base address
 * @throws error if sections not found or offset invalid
 *
 * The offset is relative to the combined ctrltext+ctrldata section.
 * This function adjusts the offset to point into ctrldata and reads the BD.
 */
uint64_t
transform_manager::
get_controlcode_bd_offset(const std::string& section_name, uint32_t offset, symbol::patch_schema schema)
{
  auto [col, page] = get_column_and_page(section_name);
  auto id = get_grp_id_if_group_elf(section_name);
  auto ctrltext = m_elfio.sections[get_ctrltext_section_name(col, page, id)];
  auto ctrldata = m_elfio.sections[get_ctrldata_section_name(col, page, id)];
  if (!ctrltext || !ctrldata)
    throw error(error::error_code::internal_error, "ctrltext or ctrldata section for col:"
                + std::to_string(col) + " page:" + std::to_string(page) + " not found\n");

  if (offset < ctrltext->get_size())
    throw error(error::error_code::internal_error, "ctrltext size lesser than offset:"
                + std::to_string(offset) + "\n");

  // Adjust offset to point into ctrldata section
  offset -= ctrltext->get_size();
  offset += elf_section_header_size;

  const auto* bd_data_ptr = reinterpret_cast<const uint32_t*>(ctrldata->get_data() + offset);

  // Read BD based on schema (AIE2PS vs AIE4)
  switch(schema) {
  case symbol::patch_schema::shim_dma_57:
    return read57(bd_data_ptr);
  case symbol::patch_schema::shim_dma_57_aie4:
    return read57_aie4(bd_data_ptr);
  default:
    throw error(error::error_code::internal_error, "Invalid schema found\n");
  }
}

/**
 * @brief Set buffer descriptor offset in control code section
 * @param section_name: Section name containing the BD
 * @param offset: Offset within the combined ctrltext+ctrldata section
 * @param bd_offset: New buffer descriptor base address to write
 * @param schema: Patch schema indicating format (AIE2PS or AIE4)
 * @throws error if sections not found or offset invalid
 *
 * The offset is relative to the combined ctrltext+ctrldata section.
 * This function adjusts the offset to point into ctrldata and writes the BD.
 */
void
transform_manager::
set_controlcode_bd_offset(const std::string& section_name, uint32_t offset, uint64_t bd_offset, symbol::patch_schema schema)
{
  auto [col, page] = get_column_and_page(section_name);
  auto id = get_grp_id_if_group_elf(section_name);
  auto ctrltext = m_elfio.sections[get_ctrltext_section_name(col, page, id)];
  auto ctrldata = m_elfio.sections[get_ctrldata_section_name(col, page, id)];
  if (!ctrltext || !ctrldata)
    throw error(error::error_code::internal_error, "ctrltext or ctrldata section for col:"
                + std::to_string(col) + " page:" + std::to_string(page) + " not found\n");
  if (offset < ctrltext->get_size())
    throw error(error::error_code::internal_error, "ctrldata size lesser than offset:"
                + std::to_string(offset) + "\n");

  // Adjust offset to point into ctrldata section
  offset -= ctrltext->get_size();
  offset += elf_section_header_size;
  if (offset > ctrldata->get_size())
    throw error(error::error_code::internal_error, "ctrltext size lesser than offset:"
                + std::to_string(offset) + "\n");

  auto* bd_data_ptr = reinterpret_cast<uint32_t*>(const_cast<char*>(ctrldata->get_data()) + offset);

  // Write BD based on schema (AIE2PS vs AIE4)
  switch(schema) {
  case symbol::patch_schema::shim_dma_57:
    write57(bd_data_ptr, bd_offset);
    break;
  case symbol::patch_schema::shim_dma_57_aie4:
    write57_aie4(bd_data_ptr, bd_offset);
    break;
  default:
    throw error(error::error_code::internal_error, "Invalid schema found\n");
  }
}

/**
 * @brief Get buffer descriptor offset from control packet section
 * @param section_name: Control packet section name
 * @param offset: Offset within the control packet section
 * @param schema: Patch schema indicating format (AIE2PS or AIE4)
 * @return Buffer descriptor offset
 * @throws error if section not found or offset invalid
 */
uint64_t
transform_manager::
get_ctrlpkt_bd_offset(const std::string& section_name, uint32_t offset, symbol::patch_schema schema)
{
  auto ctrlpkt = m_elfio.sections[section_name];
  if (!ctrlpkt)
    throw error(error::error_code::internal_error, "ctrlpkt " + section_name + " not found\n");
  if (offset > ctrlpkt->get_size())
    throw error(error::error_code::internal_error, "ctrlpkt size lesser than offset:"
                + std::to_string(offset) + "\n");


  const auto* bd_data_ptr = reinterpret_cast<const uint32_t*>(ctrlpkt->get_data() + offset);

  // Read control packet BD based on schema
  switch(schema) {
  case symbol::patch_schema::control_packet_57:
    return ctrlpkt_read57(bd_data_ptr);
  case symbol::patch_schema::control_packet_57_aie4:
    return ctrlpkt_read57_aie4(bd_data_ptr);
  default:
    throw error(error::error_code::internal_error, "Invalid schema found\n");
  }
}

/**
 * @brief Set buffer descriptor offset in control packet section
 * @param section_name: Control packet section name
 * @param offset: Offset within the control packet section
 * @param bd_offset: New buffer descriptor offset to write
 * @param schema: Patch schema indicating format (AIE2PS or AIE4)
 * @throws error if section not found or offset invalid
 */
void
transform_manager::
set_ctrlpkt_bd_offset(const std::string& section_name, uint32_t offset, uint64_t bd_offset, symbol::patch_schema schema)
{
  auto ctrlpkt = m_elfio.sections[section_name];
  if (!ctrlpkt)
    throw error(error::error_code::internal_error, "ctrlpkt " + section_name + " not found\n");
  if (offset > ctrlpkt->get_size())
    throw error(error::error_code::internal_error, "ctrlpkt size lesser than offset:"
                + std::to_string(offset) + "\n");

  auto* bd_data_ptr = reinterpret_cast<uint32_t*>(const_cast<char*>(ctrlpkt->get_data()) + offset);

  // Write control packet BD based on schema
  switch(schema) {
  case symbol::patch_schema::control_packet_57:
    ctrlpkt_write57(bd_data_ptr, bd_offset);
    break;
  case symbol::patch_schema::control_packet_57_aie4:
    ctrlpkt_write57_aie4(bd_data_ptr, bd_offset);
    break;
  default:
    throw error(error::error_code::internal_error, "Invalid schema found\n");
  }
}

/**
 * @brief Extract argument information from ELF relocation sections
 * @return Vector of arginfo containing XRT ID and BD offset pairs
 *
 * This function:
 * 1. Parses .rela.dyn relocations along with .dynsym and .dynstr sections
 * 2. Extracts XRT argument indices from symbol names
 * 3. Reads current buffer descriptor offsets from control code/packet sections
 * 4. Skips special patches (control-code-idx, .ctrlpkt-idx)
 * 5. Returns arginfo for each kernel argument
 *
 * The returned vector can be used to inspect or modify argument mappings.
 */
std::vector<arginfo>
transform_manager::
extract_rela_sections()
{
  // Locate required ELF sections
  auto dynsym = m_elfio.sections[".dynsym"];
  auto dynstr = m_elfio.sections[".dynstr"];
  auto dynsec = m_elfio.sections[".rela.dyn"];

  if (!dynsym || !dynstr || !dynsec)
    return {};

  const auto dynsym_size = dynsym->get_size();
  const auto dynstr_size = dynstr->get_size();
  const auto rela_count = dynsec->get_size() / sizeof(ELFIO::Elf32_Rela);

  std::vector<arginfo> entries;

  auto begin = reinterpret_cast<const ELFIO::Elf32_Rela*>(dynsec->get_data());
  auto end = begin + rela_count;

  // Process each relocation entry
  for (auto rela = begin; rela != end; ++rela) {
    auto symidx = ELFIO::get_sym_and_type<ELFIO::Elf32_Rela>::get_r_sym(rela->r_info);
    auto type = ELFIO::get_sym_and_type<ELFIO::Elf32_Rela>::get_r_type(rela->r_info);

    // Look up symbol in .dynsym
    auto dynsym_offset = symidx * sizeof(ELFIO::Elf32_Sym);
    if (dynsym_offset >= dynsym_size)
      throw error(error::error_code::internal_error, "Invalid symbol index " + std::to_string(symidx));
    auto sym = reinterpret_cast<const ELFIO::Elf32_Sym*>(dynsym->get_data() + dynsym_offset);

    // Get symbol name from .dynstr
    auto dynstr_offset = sym->st_name;
    if (dynstr_offset >= dynstr_size)
      throw error(error::error_code::internal_error, "Invalid symbol name offset " + std::to_string(dynstr_offset));
    auto symname = dynstr->get_data() + dynstr_offset;

    // Skip special patches that don't represent kernel arguments
    if (is_ctrlpkt_patch_name(symname) || is_controlcode_patch_name(symname))
      continue;

    // Get the section being patched
    auto patch_sec = m_elfio.sections[sym->st_shndx];
    if (!patch_sec)
      throw error(error::error_code::internal_error, "Invalid section index " + std::to_string(sym->st_shndx));

    auto patch_sec_name = patch_sec->get_name();
    auto offset = rela->r_offset;
    auto xrt_id = to_uinteger<uint32_t>(symname);
    auto schema = static_cast<symbol::patch_schema>(type);

    // Read BD offset from appropriate section type
    uint64_t bd_offset = 0;
    if (is_text_or_data_section_name(patch_sec_name))
      bd_offset = get_controlcode_bd_offset(patch_sec_name, offset, schema);
    else if (is_ctrlpkt_section_name(patch_sec_name))
      bd_offset = get_ctrlpkt_bd_offset(patch_sec_name, offset, schema);
    else
      throw error(error::error_code::internal_error, "Invalid section name " + patch_sec_name);

    entries.push_back({xrt_id, bd_offset + rela->r_addend});
  }

  return entries;
}

/**
 * @brief Update ELF with new argument information and regenerate binary
 * @param entries: Vector of arginfo with new XRT indices and BD offsets
 * @return Modified ELF binary as vector of chars
 *
 * This is the main transformation function that:
 * 1. Updates symbol names in .dynsym with new XRT indices
 * 2. Rebuilds .dynstr with new symbol names (deduplicates symbols)
 * 3. Patches BD offsets in control code and control packet sections
 * 4. Builds lookup table for apply_offset_57 opcode transformation
 * 5. Processes all .ctrltext sections to update apply_offset_57 opcodes
 * 6. Clears relocation addends (r_addend = 0)
 * 7. Serializes modified ELF back to binary
 *
 * @throws error if input is invalid or sections are missing/malformed
 */
std::vector<char>
transform_manager::
update_rela_sections(const std::vector<arginfo>& entries) {
  xrt_idx_lookup.clear();

  // Locate required ELF sections
  auto dynsym = m_elfio.sections[".dynsym"];
  auto dynstr = m_elfio.sections[".dynstr"];
  auto dynsec = m_elfio.sections[".rela.dyn"];

  if (!dynsym || !dynstr || !dynsec)
    return {};

  const auto dynsym_size = dynsym->get_size();
  const auto dynstr_size = dynstr->get_size();
  const auto rela_count = dynsec->get_size() / sizeof(ELFIO::Elf32_Rela);

  // Initialize new string table (starts with null byte)
  std::string strtab_data(1, '\0');
  std::vector<char> dynsym_copy(dynsym->get_data(), dynsym->get_data() + dynsym_size);

  std::map<std::string, std::string> name_map;  // Old name -> new name mapping
  std::map<std::string, ELFIO::Elf_Word> hash;  // Deduplication: key -> name + offset
  size_t num = 0;  // Index into entries vector

  auto begin = reinterpret_cast<ELFIO::Elf32_Rela*>(const_cast<char*>(dynsec->get_data()));
  auto end = begin + rela_count;

  // Process each relocation entry
  for (auto rela = begin; rela != end; ++rela) {
    auto symidx = ELFIO::get_sym_and_type<ELFIO::Elf32_Rela>::get_r_sym(rela->r_info);
    auto type = ELFIO::get_sym_and_type<ELFIO::Elf32_Rela>::get_r_type(rela->r_info);

    // Look up symbol in .dynsym
    auto dynsym_offset = symidx * sizeof(ELFIO::Elf32_Sym);
    if (dynsym_offset >= dynsym_size)
      throw error(error::error_code::internal_error, "Invalid symbol index " + std::to_string(symidx));

    auto sym = reinterpret_cast<ELFIO::Elf32_Sym*>(const_cast<char*>(dynsym->get_data()) + dynsym_offset);
    auto sym_new = reinterpret_cast<ELFIO::Elf32_Sym*>(dynsym_copy.data() + dynsym_offset);

    // Get symbol name from .dynstr
    auto dynstr_offset = sym->st_name;
    if (dynstr_offset >= dynstr_size)
      throw error(error::error_code::internal_error, "Invalid symbol name offset " + std::to_string(dynstr_offset));
    auto symname = dynstr->get_data() + dynstr_offset;

    // Get the section being patched
    auto patch_sec = m_elfio.sections[sym->st_shndx];
    if (!patch_sec)
      throw error(error::error_code::internal_error, "Invalid section index " + std::to_string(sym->st_shndx));

    const auto& patch_sec_name = patch_sec->get_name();
    auto offset = rela->r_offset;

    // Special patches (control-code-idx, .ctrlpkt-idx) keep their original names
    const bool is_special_patch = is_ctrlpkt_patch_name(symname) || is_controlcode_patch_name(symname);
    std::string name = is_special_patch ? symname : std::to_string(entries[num].xrt_idx);
    
    // Verify consistency: all instances of a symbol should map to the same new name
    auto name_it = name_map.find(symname);
    if (name_it != name_map.end()) {
      if (name_it->second != name)
        throw error(error::error_code::invalid_input, "Invalid input xrt_id:" + std::string(symname) + " modified only at few places");
    } else {
      name_map[symname] = name;
    }

    // Deduplicate symbols: reuse existing string if same key already exists
    ELFIO::Elf_Word new_offset;
    std::string key = patch_sec_name + "_" + name + "_" + std::to_string(sym->st_size);
    auto hash_it = hash.find(key);
    if (hash_it == hash.end()) {
      // New unique symbol: add to string table
      new_offset = strtab_data.size();
      strtab_data.append(name).push_back('\0');
      hash[key] = new_offset;
    } else {
      // Reuse existing string offset
      new_offset = hash_it->second;
    }

    // Update symbol name offset in copied symbol table
    sym_new->st_name = new_offset;

    // Skip BD patching for special symbols (they don't change)
    if (is_special_patch == true)
      continue;

    // Build lookup table for apply_offset_57 opcode transformation
    auto lookup_key = get_key(offset, sym->st_shndx);
    if (xrt_idx_lookup.find(lookup_key) != xrt_idx_lookup.end())
      throw error(error::error_code::internal_error, "lookup_key (" + lookup_key + ") already found\n");

    xrt_idx_lookup[lookup_key] = entries[num].xrt_idx;

    // Patch BD offsets in the appropriate section type
    auto schema = static_cast<symbol::patch_schema>(type);
    if (is_text_or_data_section_name(patch_sec_name))
      set_controlcode_bd_offset(patch_sec_name, offset, entries[num].bd_offset, schema);
    else if (is_ctrlpkt_section_name(patch_sec_name))
      set_ctrlpkt_bd_offset(patch_sec_name, offset, entries[num].bd_offset, schema);
    else
      throw error(error::error_code::internal_error, "Invalid section name " + patch_sec_name);

    // Clear relocation addend (BD offset now embedded in section data)
    rela->r_addend = 0;
    ++num;
  }

  // Replace old symbol string table and symbol table with new versions
  dynstr->set_data(strtab_data);
  dynsym->set_data(dynsym_copy.data(), dynsym_copy.size());

  // Process all .ctrltext sections to update apply_offset_57 opcodes
  process_sections();

  // Serialize modified ELF back to binary format
  std::stringstream stream;
  stream << std::noskipws;
  m_elfio.save(stream);

  return std::vector<char>(std::istream_iterator<char>(stream), std::istream_iterator<char>());
}
} // End of Namespace aiebu

