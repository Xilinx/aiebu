// SPDX-License-Identifier: MIT
// Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

#include "tools/debug_tools.h"
#include "tools/opcode_decoder.h"
#include "aiebu/aiebu_error.h"
#include "elf/aie_elf_constants.h"
#include "common/utils.h"
#include "specification/aie2ps/isa.h"
#include "ops/oparg.h"

#include <elfio/elfio.hpp>
#include <sstream>

namespace aiebu {

// Page layout constants (same as disassembler.cpp / transform_manager.cpp)
static constexpr size_t k_binary_page_size        = 8192;  // merged-format page size
static constexpr size_t k_binary_page_header_size = 16;    // header at the start of each page
static constexpr uint8_t k_align_opcode           = 0xA5;  // alignment pseudo-instruction byte

// Helper: format a uint64_t as a hex string for output
static std::string
format_hex_u64(uint64_t value)
{
  std::ostringstream oss;
  oss << "0x" << std::hex << value;
  return oss.str();
}

/**
 * make_transform() - Static helper function to create transform_manager from ELF buffer
 *
 * @param type
 *  Type of the ELF buffer (aie2ps / aie4 / config).
 * @param buffer
 *  ELF binary buffer from which the .dump section will be extracted.
 * @return
 *  transform_manager initialized with the ELF buffer.
 */
transform_manager
debug_tools::
make_transform(aiebu_assembler::buffer_type type, const std::vector<char>& buffer)
{
  if (buffer.empty())
    throw error(error::error_code::invalid_input, "Input buffer is empty");

  if (type != aiebu_assembler::buffer_type::elf_aie2ps &&
      type != aiebu_assembler::buffer_type::elf_aie4 &&
      type != aiebu_assembler::buffer_type::elf_aie2ps_config &&
      type != aiebu_assembler::buffer_type::elf_aie4_config)
    throw error(error::error_code::invalid_buffer_type, "Invalid ELF buffer for debug tools");

  return transform_manager(buffer);
}

/**
 * debug_tools() - Constructor with ELF buffer and type
 *
 * @param type
 *  Type of the ELF buffer (aie2ps / aie4 / config).
 * @param buffer
 *  ELF binary buffer from which the .dump section will be extracted.
 *
 * This constructor initializes the debug_tools object by creating a transform_manager
 * from the input ELF buffer and type so that the .dump section can be extracted
 * and the information can be written from the debug information contained in the ELF.
 */
debug_tools::
debug_tools(aiebu_assembler::buffer_type type, const std::vector<char>& buffer)
  : m_transform_manager(make_transform(type, buffer))
  , m_buffer_type(type)
{
  get_dump_section();
}

/**
 * get_dump_section() - Extract and cache .dump section from ELF buffer
 *
 * This function will extract and store the .dump section into m_debug_json, either
 * using the kernel instance name for config ELFs or without it for target ELFs.
 * The transform_manager is used to check the ELF format and extract the .dump section.
 */
void
debug_tools::
get_dump_section()
{
  if (m_transform_manager.check_config_elf()) {
    // TODO: hardcoding the kernel instance name for simplicity,
    // this should be extended to support multiple instances.
    std::string kernel_instance_name = "DPU:dpu";

    m_debug_json = m_transform_manager.get_dump_section_json(kernel_instance_name);
  } else {
    m_debug_json = m_transform_manager.get_dump_section_json();
  }

  if (m_debug_json.empty())
    throw error(error::error_code::invalid_input, "No debug information found in the ELF file");
}

/**
 * decode_opcode() - Decode the opcode at a firmware-reported (uc_idx, page_idx, offset).
 *
 * Section naming: .attach_to_group N creates .ctrltext.N, where N is the UC index.
 * The firmware reports that same UC index, so uc_idx maps directly to the section number.
 *
 * Opcode position (byte index into section data):
 *   Merged format (.ctrltext.<uc_idx>):           page_idx * PAGE_SIZE + PAGE_HEADER + offset
 *   Legacy format (.ctrltext.<uc_idx>.<page_idx>): PAGE_HEADER + offset
 *
 * @param elf       Pre-parsed ELFIO object.
 * @param uc_idx    UC index as reported by firmware — equals the section number.
 * @param page_idx  Page index within the UC.
 * @param offset    Byte offset of the opcode within the page (after page header).
 * @param out       Populated on success with opcode_name, args_str, and opcode_size.
 * @return true if the opcode at the given position was identified; false otherwise.
 */
bool
debug_tools::
decode_opcode(const ELFIO::elfio& elf, uint32_t uc_idx, uint32_t page_idx,
              uint32_t offset, op_info& out)
{
  const uint8_t abi_version = elf.get_abi_version();

  // uc_idx is the section number directly (.attach_to_group N → .ctrltext.N).
  const std::string uc_prefix = ".ctrltext." + std::to_string(uc_idx);

  // Find the section and compute the exact byte position of the opcode.
  const char* sec_data   = nullptr;
  size_t      sec_size   = 0;
  size_t      opcode_pos = 0;
  size_t      region_end = 0;  // end of the usable instruction region in the section

  if (abi_version == elf_version_config) {
    // Merged format: one section per UC holds all pages concatenated.
    // Accept ".ctrltext.<uc_idx>" (non-group) or ".ctrltext.<uc_idx>.<id>" (group ELF).
    for (ELFIO::Elf_Half i = 0; i < elf.sections.size(); ++i) {
      const ELFIO::section* sec = elf.sections[i];
      if (!sec) continue;
      const std::string& sname = sec->get_name();
      const bool is_exact   = (sname == uc_prefix);
      const bool is_with_id = (sname.size() > uc_prefix.size() &&
                               sname[uc_prefix.size()] == '.' &&
                               sname.compare(0, uc_prefix.size(), uc_prefix) == 0);
      if (!is_exact && !is_with_id)
        continue;

      const size_t page_base = static_cast<size_t>(page_idx) * k_binary_page_size;
      const size_t pos = page_base + k_binary_page_header_size + offset;
      out.diag_info = "section=" + sname + " size=" + std::to_string(sec->get_size())
                    + " pos=" + std::to_string(pos);
      if (pos >= sec->get_size())
        continue;  // out of range for this instance — try next

      if (!sec->get_data()) continue;
      sec_data   = sec->get_data();
      sec_size   = sec->get_size();
      opcode_pos = pos;
      region_end = std::min(sec_size, page_base + k_binary_page_size);
      break;
    }
  } else {
    // Legacy per-page format: ".ctrltext.<uc_idx>.<page_idx>" or with a trailing ".<id>".
    const std::string uc_page_prefix = uc_prefix + "." + std::to_string(page_idx);
    for (ELFIO::Elf_Half i = 0; i < elf.sections.size(); ++i) {
      const ELFIO::section* sec = elf.sections[i];
      if (!sec) continue;
      const std::string& sname = sec->get_name();
      const bool is_exact   = (sname == uc_page_prefix);
      const bool is_with_id = (sname.size() > uc_page_prefix.size() + 1 &&
                               sname.compare(0, uc_page_prefix.size(), uc_page_prefix) == 0 &&
                               sname[uc_page_prefix.size()] == '.');
      if (!is_exact && !is_with_id)
        continue;

      const size_t pos = k_binary_page_header_size + offset;
      out.diag_info = "section=" + sname + " size=" + std::to_string(sec->get_size())
                    + " pos=" + std::to_string(pos);
      if (pos >= sec->get_size())
        continue;

      if (!sec->get_data()) continue;
      sec_data   = sec->get_data();
      sec_size   = sec->get_size();
      opcode_pos = pos;
      region_end = sec_size;
      break;
    }
  }

  if (!sec_data) {
    if (out.diag_info.empty()) {
      std::ostringstream oss;
      oss << "section not found; abi_version=" << std::to_string(abi_version)
          << " os_abi=0x" << std::hex << static_cast<int>(elf.get_os_abi())
          << " uc_idx=" << std::dec << uc_idx;
      out.diag_info = oss.str();
    }
    return false;
  }

  // Walk instructions from byte 0 of the page up to offset.
  // offset may point anywhere within an instruction (not just the opcode byte),
  // so we walk to find the instruction that contains it.
  const char* instr_data = sec_data + opcode_pos - offset;  // start of instruction region
  const size_t instr_size = region_end - (opcode_pos - offset);

  isa_disassembler isa;
  const std::map<uint8_t, isa_op_disasm>* isa_map = isa.get_isa_map();

  size_t pos = 0;
  while (pos < instr_size) {
    const uint8_t opbyte = static_cast<uint8_t>(instr_data[pos]);

    if (opbyte == k_align_opcode) {
      if (pos == offset) {
        out.opcode_name = "align";
        out.opcode_size = 1;
        return true;
      }
      ++pos;
      continue;
    }

    const auto it = isa_map->find(opbyte);
    if (it == isa_map->end()) {
      out.diag_info += " unknown_opcode=0x" + ELFIO::to_hex_string(opbyte);
      return false;  // unknown opcode — cannot continue walk
    }

    // Instruction size: 1B opcode + 1B pad + arg bytes
    size_t op_size = 2;
    for (const auto& arg : it->second.get_args())
      op_size += arg.get_width() / byte_to_bits;

    if (pos == offset || pos + op_size > offset) {
      // Exact match or offset lands inside this instruction
      out.opcode_name = it->second.get_code_name();
      out.opcode_size = op_size;

      // Read argument values: bytes start at pos+2 (after opcode + pad)
      size_t arg_pos = pos + 2;
      std::ostringstream args_ss;
      bool first_arg = true;
      for (const auto& arg : it->second.get_args()) {
        const size_t arg_bytes = arg.get_width() / byte_to_bits;
        if (arg.get_type() == opArg::optype::PAD) {
          arg_pos += arg_bytes;
          continue;
        }
        if (arg_pos + arg_bytes > instr_size)
          break;  // args extend beyond page boundary — stop
        uint32_t val = 0;
        for (size_t b = 0; b < arg_bytes; ++b)
          val |= static_cast<uint32_t>(static_cast<uint8_t>(instr_data[arg_pos + b])) << (b * 8);
        arg_pos += arg_bytes;
        if (!first_arg)
          args_ss << ", ";
        args_ss << "0x" << std::hex << val;
        first_arg = false;
      }
      out.args_str = args_ss.str();
      return true;
    }

    pos += op_size;
  }
  return false;
}

/**
 * write_opcode_information() - Static overload: decode directly from pre-parsed ELFIO object
 *
 * @param stream      Output stream for the result.
 * @param elf         Pre-parsed ELFIO object (no ELF buffer round-trip).
 * @param filename    ELF file name (informational only).
 * @param kernel_name Kernel instance filter (unused, reserved for future .dump support).
 * @param uc_idx      Microcontroller index reported at timeout.
 * @param page_idx    Page index reported at timeout.
 * @param offset      Byte offset within the page reported at timeout.
 */
void
debug_tools::
write_opcode_information(std::ostream& stream, const ELFIO::elfio& elf,
                         const std::string& filename,
                         const std::string& /*kernel_name*/,
                         uint32_t uc_idx, uint32_t page_idx, uint32_t offset)
{
  op_info info{};
  info.uc_idx   = uc_idx;
  info.page_idx = page_idx;

  bool found = decode_opcode(elf, uc_idx, page_idx, offset, info);

  if (!filename.empty())
    stream << "ELF File:       " << filename                        << '\n';
  stream << "uC Index:       " << format_hex_u64(uc_idx)            << '\n';
  stream << "Page Index:     " << format_hex_u64(page_idx)          << '\n';
  stream << "Offset:         " << format_hex_u64(offset)            << '\n';
  stream << "Opcode Information:\n";
  if (!found) {
    stream << "Not found!\n";
    if (!info.diag_info.empty())
      stream << "Decode detail:  " << info.diag_info << '\n';
    return;
  }
  stream << "Opcode:         " << info.opcode_name;
  if (!info.args_str.empty())
    stream << "  " << info.args_str;
  stream << '\n';
  stream << "Opcode Size:    " << format_hex_u64(info.opcode_size)   << '\n';
}

// Free function wrapper — allows callers to use aiebu::write_opcode_information()
// without including debug_tools.h (and its transform_manager.h / isa.h chain).
void
write_opcode_information(std::ostream& stream, const ELFIO::elfio& elf,
                         const std::string& filename,
                         const std::string& kernel_name,
                         uint32_t uc_idx, uint32_t page_idx, uint32_t offset)
{
  debug_tools::write_opcode_information(stream, elf, filename, kernel_name,
                                        uc_idx, page_idx, offset);
}

} // namespace aiebu
