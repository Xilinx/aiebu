// SPDX-License-Identifier: MIT
// Copyright (C) 2025-2026 Advanced Micro Devices, Inc. All rights reserved.

#include "tools/debug_tools.h"
#include "tools/opcode_decoder.h"
#include "aiebu/aiebu_error.h"
#include "elf/aie_elf_constants.h"
#include "common/utils.h"
#include "specification/aie2ps/isa.h"
#include "ops/ops.h"
#include "ops/oparg.h"

#include <elfio/elfio.hpp>
#include <set>
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

  // m_debug_json may be empty when the ELF was assembled with disabledump;
  // callers that require the .dump section must check get_dump_data() themselves.
}

/**
 * find_col_for_uc() - Resolve the ELF section column number for a firmware-reported uc_idx.
 *
 * ELF sections are named ".ctrltext.<col>" where col is the physical column number
 * set by the .attach_to_group directive. The firmware health report uses a sequential
 * uc_idx (0, 1, 2, ...) across ALL UCs in the partition, regardless of which physical
 * columns the partition spans.
 *
 * Mapping: col = sorted_unique_cols[uc_idx / uc_per_col]
 *
 *   uc_per_col = 2 for AIE4 (each physical column has UC_A and UC_B, both sharing
 *                             the same ".ctrltext.<col>" section but executing
 *                             independent code at different page_idx/offset).
 *   uc_per_col = 1 for AIE2PS (one UC per column, uc_idx == logical col index).
 *
 * Example — 3-column AIE4 partition at physical cols 0, 2, 4:
 *   sections:  {0, 2, 4}
 *   uc_idx 0,1 → sorted_cols[0] = 0  →  .ctrltext.0
 *   uc_idx 2,3 → sorted_cols[1] = 2  →  .ctrltext.2
 *   uc_idx 4,5 → sorted_cols[2] = 4  →  .ctrltext.4
 *
 * Note: a direct "uc_idx == col" lookup is NOT used because it fails for partitions
 * that start at a non-zero physical column (e.g. cols 2, 4, 6 where uc_idx=2 must
 * map to col 4, not col 2).
 *
 * @param elf     Pre-parsed ELFIO object.
 * @param uc_idx  Sequential UC index as reported by firmware health data.
 * @return Physical column number to use for section name lookup.
 */
static uint32_t
find_col_for_uc(const ELFIO::elfio& elf, uint32_t uc_idx)
{
  // Collect unique physical column numbers from .ctrltext sections (sorted by std::set)
  std::set<uint32_t> col_set;
  for (ELFIO::Elf_Half i = 0; i < elf.sections.size(); ++i) {
    const ELFIO::section* sec = elf.sections[i];
    if (!sec)
      continue;
    const std::string& name = sec->get_name();
    if (name.rfind(".ctrltext.", 0) != 0)
      continue;
    const std::string suffix = name.substr(10);  // strip ".ctrltext."
    const auto dot = suffix.find('.');
    const std::string col_str = (dot == std::string::npos) ? suffix : suffix.substr(0, dot);
    try { col_set.insert(static_cast<uint32_t>(std::stoul(col_str))); }
    catch (...) {}
  }

  if (col_set.empty())
    return uc_idx;  // no sections found — return as-is

  // Number of UCs sharing each column section:
  //   AIE4/4A/4Z: 2 (UC_A and UC_B both use the same .ctrltext.<col>)
  //   AIE2PS:     1 (uc_idx maps directly to column index)
  const uint8_t os_abi = elf.get_os_abi();
  const uint32_t uc_per_col =
      (os_abi == osabi_aie4 || os_abi == osabi_aie4a || os_abi == osabi_aie4z ||
       os_abi == osabi_aie2ps_group) ? 2u : 1u;

  const uint32_t col_list_idx = uc_idx / uc_per_col;
  if (col_list_idx >= col_set.size())
    return uc_idx;  // uc_idx out of range — return as-is

  auto it = col_set.begin();
  std::advance(it, col_list_idx);
  return *it;
}

/**
 * decode_opcode() - Static core: walk raw instruction bytes to identify opcode at offset
 *
 * @param elf       Pre-parsed ELFIO object — no re-parsing overhead.
 * @param uc_idx    Sequential UC index as reported by firmware (0, 1, 2, ...).
 *                  Mapped to the physical column section name via resolve_col_from_uc_idx().
 * @param page_idx  Page index within the column.
 * @param offset    Byte offset within the page (after page header).
 * @param out       On success, populated with opcode_name and opcode_size.
 * @return true if the instruction at the given offset was identified; false otherwise.
 */
bool
debug_tools::
decode_opcode(const ELFIO::elfio& elf, uint32_t uc_idx, uint32_t page_idx,
              uint32_t offset, op_info& out)
{
  const uint8_t abi_version = elf.get_abi_version();

  // Resolve firmware-reported uc_idx to the ELF section column number.
  const uint32_t col = find_col_for_uc(elf, uc_idx);

  // Locate the instruction bytes for (col, page_idx)
  const char* instr_data = nullptr;
  size_t      instr_size = 0;

  if (abi_version == elf_version_config) {
    // Merged format: one section per column holds all pages concatenated.
    // Each page occupies k_binary_page_size bytes: [header 16B][text][data][padding].
    // Non-group ELF: ".ctrltext.<col>"
    // Group ELF:     ".ctrltext.<col>.<id>"  where <id> is a string kernel-instance identifier.
    // Match either form using a prefix check; try all matching instances and pick
    // the first one where the requested page is in range.
    const std::string col_prefix = ".ctrltext." + std::to_string(col);

    for (ELFIO::Elf_Half i = 0; i < elf.sections.size(); ++i) {
      ELFIO::section* sec = elf.sections[i];
      if (!sec)
        continue;
      const std::string& sname = sec->get_name();
      // Accept ".ctrltext.<col>" (exact) or ".ctrltext.<col>.<id>" (group merged)
      const bool is_exact    = (sname == col_prefix);
      const bool is_with_id  = (sname.size() > col_prefix.size() &&
                                sname[col_prefix.size()] == '.' &&
                                sname.compare(0, col_prefix.size(), col_prefix) == 0);
      if (!is_exact && !is_with_id)
        continue;

      const size_t page_base = static_cast<size_t>(page_idx) * k_binary_page_size;
      out.diag_info = "section=" + sname +
                      " size=" + std::to_string(sec->get_size()) +
                      " page_base=" + std::to_string(page_base);
      if (page_base + k_binary_page_header_size > sec->get_size())
        continue;  // page out of range for this instance — try next

      instr_data = sec->get_data() + page_base + k_binary_page_header_size;
      const size_t page_end =
          std::min(static_cast<size_t>(sec->get_size()), page_base + k_binary_page_size);
      instr_size = page_end - page_base - k_binary_page_header_size;
      break;
    }
  } else {
    // Legacy per-page format: ".ctrltext.<col>.<page>" or ".ctrltext.<col>.<page>.<id>"
    // where <id> is a string kernel-instance identifier (e.g. "dpu", "dpu_0"), not a UC index.
    // For group ELFs multiple kernel instances may each produce a section for the same
    // (col, page) — all contain equivalent code. Try every matching section and select
    // the first one where the requested offset falls within the instruction region.
    const std::string col_page_prefix =
        ".ctrltext." + std::to_string(col) + "." + std::to_string(page_idx);

    for (ELFIO::Elf_Half i = 0; i < elf.sections.size(); ++i) {
      ELFIO::section* sec = elf.sections[i];
      if (!sec)
        continue;
      const std::string& sname = sec->get_name();
      // Match exact ".ctrltext.<col>.<page>" or ".ctrltext.<col>.<page>.<id>"
      const bool is_exact   = (sname == col_page_prefix);
      const bool is_with_id = (sname.size() > col_page_prefix.size() + 1 &&
                               sname.compare(0, col_page_prefix.size(), col_page_prefix) == 0 &&
                               sname[col_page_prefix.size()] == '.');
      if (!is_exact && !is_with_id)
        continue;
      if (sec->get_size() <= k_binary_page_header_size)
        continue;

      const char* candidate_data = sec->get_data() + k_binary_page_header_size;
      const size_t candidate_size = sec->get_size() - k_binary_page_header_size;

      out.diag_info = "section=" + sname + " size=" + std::to_string(sec->get_size());
      if (offset < candidate_size) {
        instr_data = candidate_data;
        instr_size = candidate_size;
        break;
      }
    }
  }

  if (!instr_data) {
    if (out.diag_info.empty()) {
      std::ostringstream oss;
      oss << "section not found; abi_version=" << std::to_string(abi_version)
          << " os_abi=0x" << std::hex << static_cast<int>(elf.get_os_abi())
          << " col=" << std::dec << col;
      out.diag_info = oss.str();
    }
    return false;
  }
  if (offset >= instr_size) {
    out.diag_info += " instr_size=" + std::to_string(instr_size) + " offset_oob";
    return false;
  }

  isa_disassembler isa;
  const std::map<uint8_t, isa_op_disasm>* isa_map = isa.get_isa_map();

  // Walk instructions from byte 0 up to `offset`
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

    auto it = isa_map->find(opbyte);
    if (it == isa_map->end())
      return false;  // unknown opcode — cannot continue walk

    // size = 1B opcode + 1B pad + arg bytes
    size_t op_size = 2;
    for (const auto& arg : it->second.get_args())
      op_size += arg.get_width() / byte_to_bits;

    if (pos == offset || pos + op_size > offset) {
      // Exact match or offset lands inside this instruction
      out.opcode_name = it->second.get_code_name();
      out.opcode_size = op_size;

      // Read argument values: bytes start at pos+2 (after opcode + pad bytes)
      size_t arg_pos = pos + 2;
      std::ostringstream args_ss;
      bool first_arg = true;
      for (const auto& arg : it->second.get_args()) {
        const size_t arg_bytes = arg.get_width() / byte_to_bits;
        if (arg.get_type() == opArg::optype::PAD) {
          arg_pos += arg_bytes;
          continue;
        }
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
