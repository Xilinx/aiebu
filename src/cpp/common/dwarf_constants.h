// SPDX-License-Identifier: MIT
// Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved.
//
// DWARF v5 encoding constants used by dwarf_writer and dwarf_reader.
//
// Values are taken verbatim from the DWARF Debugging Information Format
// Version 5 specification (https://dwarfstd.org/dwarf5std.html).
// No OS SDK or LLVM headers required — this file is the portable source
// of truth for the AIEBU DWARF implementation.

#ifndef AIEBU_COMMON_DWARF_CONSTANTS_H_
#define AIEBU_COMMON_DWARF_CONSTANTS_H_

#include <cstdint>

namespace aiebu::dwarf5 {

// ── DWARF version and address size used by this implementation ───────────
inline constexpr uint16_t DWARF_VERSION   = 5;
inline constexpr uint8_t  DWARF_ADDR_SIZE = 4;

// ── Address encoding: lower 13 bits = page offset, upper bits = page index.
// PAGE_SIZE must equal the .ctrltext page size used by the encoder (8 KiB).
inline constexpr uint32_t DWARF_PAGE_SIZE = 0x2000;

// ── Column name prefix used in DW_AT_name for DW_TAG_module DIEs ─────────
inline constexpr char   COLUMN_PREFIX[]   = "column ";
inline constexpr size_t COLUMN_PREFIX_LEN = 7;

// ── Unit types (Table 7.2) ────────────────────────────────────────────────
inline constexpr uint8_t DW_UT_compile = 0x01;

// ── TAGs (Table 7.3) ─────────────────────────────────────────────────────
inline constexpr uint8_t DW_TAG_label        = 0x0a;
inline constexpr uint8_t DW_TAG_compile_unit = 0x11;
inline constexpr uint8_t DW_TAG_module       = 0x1e;

// ── Children flag (Table 7.4) ────────────────────────────────────────────
inline constexpr uint8_t DW_CHILDREN_no  = 0;
inline constexpr uint8_t DW_CHILDREN_yes = 1;

// ── Attributes (Table 7.5) ───────────────────────────────────────────────
inline constexpr uint8_t DW_AT_name        = 0x03;
inline constexpr uint8_t DW_AT_byte_size   = 0x0b;
inline constexpr uint8_t DW_AT_stmt_list   = 0x10;
inline constexpr uint8_t DW_AT_low_pc      = 0x11;
inline constexpr uint8_t DW_AT_language    = 0x13;
inline constexpr uint8_t DW_AT_const_value = 0x1c;
inline constexpr uint8_t DW_AT_producer    = 0x25;
inline constexpr uint8_t DW_AT_description = 0x5a;

// ── Forms (Table 7.6) ────────────────────────────────────────────────────
inline constexpr uint8_t DW_FORM_addr       = 0x01;
inline constexpr uint8_t DW_FORM_data2      = 0x05;
inline constexpr uint8_t DW_FORM_data4      = 0x06;
inline constexpr uint8_t DW_FORM_string     = 0x08;  // inline NUL-terminated
inline constexpr uint8_t DW_FORM_strp       = 0x0e;  // 4-byte offset into .debug_str
inline constexpr uint8_t DW_FORM_sec_offset = 0x17;

// ── Language codes (Table 7.17) ──────────────────────────────────────────
// DW_LANG_Mips_Assembler is the de-facto standard for assembly-language CUs.
// GCC and Clang both emit 0x8001 for .s files.
inline constexpr uint16_t DW_LANG_Mips_Assembler = 0x8001;

// ── Standard line-number opcodes (Table 7.22) ────────────────────────────
inline constexpr uint8_t DW_LNS_copy              = 0x01;
inline constexpr uint8_t DW_LNS_advance_pc        = 0x02;
inline constexpr uint8_t DW_LNS_advance_line      = 0x03;
inline constexpr uint8_t DW_LNS_set_file          = 0x04;
inline constexpr uint8_t DW_LNS_set_column        = 0x05;
inline constexpr uint8_t DW_LNS_negate_stmt       = 0x06;
inline constexpr uint8_t DW_LNS_set_basic_block   = 0x07;
inline constexpr uint8_t DW_LNS_const_add_pc      = 0x08;
inline constexpr uint8_t DW_LNS_fixed_advance_pc  = 0x09;
inline constexpr uint8_t DW_LNS_set_prologue_end  = 0x0a;
inline constexpr uint8_t DW_LNS_set_epilogue_begin= 0x0b;
inline constexpr uint8_t DW_LNS_set_isa           = 0x0c;

// ── Extended line-number opcodes (Table 7.25) ────────────────────────────
inline constexpr uint8_t DW_LNE_end_sequence = 0x01;

// ── Line-number content-type codes (Table 7.27) ──────────────────────────
inline constexpr uint8_t DW_LNCT_path            = 0x01;
inline constexpr uint8_t DW_LNCT_directory_index = 0x02;

} // namespace aiebu::dwarf5

#endif // AIEBU_COMMON_DWARF_CONSTANTS_H_
