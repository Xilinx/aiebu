// SPDX-License-Identifier: MIT
// Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

#include "tools/debug_tools.h"
#include "aiebu/aiebu_error.h"

namespace aiebu {

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
      type != aiebu_assembler::buffer_type::elf_aie4a &&
      type != aiebu_assembler::buffer_type::elf_aie4z &&
      type != aiebu_assembler::buffer_type::elf_aie2ps_config &&
      type != aiebu_assembler::buffer_type::elf_aie4_config &&
      type != aiebu_assembler::buffer_type::elf_aie4a_config &&
      type != aiebu_assembler::buffer_type::elf_aie4z_config)
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
 * get_dump_section() - Extract and cache debug information from ELF buffer.
 *
 * Tries the legacy JSON .dump section first.  If absent, falls back to
 * DWARF v5 .debug_* sections (produced by merged-format / config ELFs with
 * abi_version=0x21 and a .target directive).  Throws if neither is found.
 */
void
debug_tools::
get_dump_section()
{
  // Try legacy .dump path first
  if (m_transform_manager.check_config_elf()) {
    // TODO: hardcoding the kernel instance name for simplicity,
    // this should be extended to support multiple instances.
    std::string kernel_instance_name = "DPU:dpu";
    m_debug_json = m_transform_manager.get_dump_section_json(kernel_instance_name);
  } else {
    m_debug_json = m_transform_manager.get_dump_section_json();
  }

  if (!m_debug_json.empty())
    return; // .dump found — done

  // .dump absent: try DWARF v5 .debug_* sections
  m_dwarf = std::make_unique<dwarf_reader>(m_transform_manager.get_elfio());
  if (m_dwarf->has_dwarf())
    return; // DWARF found — debug_tools methods will use m_dwarf

  // Neither .dump nor DWARF present
  throw error(error::error_code::invalid_input, "No debug information found in the ELF file");
}

} // namespace aiebu
