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

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace aiebu::dwarf5 {

// ── DWARF version and address size used by this implementation ───────────
inline constexpr uint16_t DWARF_VERSION   = 5;
inline constexpr uint8_t  DWARF_ADDR_SIZE = 4;

// ── Address encoding: lower 13 bits = page offset, upper bits = page index.
// PAGE_SIZE must equal the .ctrltext page size used by the encoder (8 KiB).
inline constexpr uint32_t DWARF_PAGE_SIZE = 0x2000;

// ── Column name prefix used in DW_AT_name for DW_TAG_module DIEs ─────────
inline constexpr std::string_view COLUMN_PREFIX    = "column ";
inline constexpr std::size_t      COLUMN_PREFIX_LEN = COLUMN_PREFIX.size();

// ── Unit types (Table 7.2) ────────────────────────────────────────────────
inline constexpr uint8_t DW_UT_compile = 0x01;

// ── TAGs (Table 7.3) ─────────────────────────────────────────────────────
inline constexpr uint8_t DW_TAG_label        = 0x0a;  // named code-position (goto target)
inline constexpr uint8_t DW_TAG_variable     = 0x34;  // named data object
inline constexpr uint8_t DW_TAG_compile_unit = 0x11;
inline constexpr uint8_t DW_TAG_module       = 0x1e;

// ── Children flag (Table 7.4) ────────────────────────────────────────────
inline constexpr uint8_t DW_CHILDREN_no  = 0;
inline constexpr uint8_t DW_CHILDREN_yes = 1;

// ── Attributes (Table 7.5) ───────────────────────────────────────────────
inline constexpr uint8_t DW_AT_name        = 0x03;
inline constexpr uint8_t DW_AT_byte_size   = 0x0b;
inline constexpr uint8_t DW_AT_low_pc      = 0x11;  // code address (DW_TAG_label)
inline constexpr uint8_t DW_AT_stmt_list   = 0x10;
inline constexpr uint8_t DW_AT_language    = 0x13;
inline constexpr uint8_t DW_AT_const_value = 0x1c;
inline constexpr uint8_t DW_AT_location    = 0x02;  // data address (DW_TAG_variable)
inline constexpr uint8_t DW_AT_producer    = 0x25;
inline constexpr uint8_t DW_AT_description = 0x5a;

// ── Forms (Table 7.6) ────────────────────────────────────────────────────
inline constexpr uint8_t DW_FORM_addr       = 0x01;
inline constexpr uint8_t DW_FORM_data2      = 0x05;
inline constexpr uint8_t DW_FORM_data4      = 0x06;
inline constexpr uint8_t DW_FORM_string     = 0x08;  // inline NUL-terminated
inline constexpr uint8_t DW_FORM_strp       = 0x0e;  // 4-byte offset into .debug_str
inline constexpr uint8_t DW_FORM_exprloc    = 0x18;  // ULEB128 length + location expression bytes
inline constexpr uint8_t DW_FORM_sec_offset = 0x17;

// ── Location expression opcodes (Table 7.9) ──────────────────────────────
inline constexpr uint8_t DW_OP_addr = 0x03;  // followed by address_size bytes (absolute address)

inline constexpr uint16_t DW_LANG_Assembly = 0x0031;

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

// ── Special opcode range ──────────────────────────────────────────────────
// Opcodes in [opcode_base, 255] are special opcodes (DWARF v5 §6.2.5.1).
inline constexpr uint8_t DWARF_MAX_SPECIAL_OPCODE = 255;

// ── Line-number content-type codes (Table 7.27) ──────────────────────────
inline constexpr uint8_t DW_LNCT_path            = 0x01;
inline constexpr uint8_t DW_LNCT_directory_index = 0x02;

// ── DW_OP_addr expression size ────────────────────────────────────────────
// A DW_OP_addr location expression is: 1 opcode byte + 4-byte address = 5 bytes.
// Used when emitting DW_AT_location on DW_TAG_variable DIEs (writer)
// and when decoding it in the reader.
inline constexpr uint32_t DW_OP_ADDR_EXPR_SIZE = 5;

// ── LEB128 encoding constants (DWARF §7.6) ───────────────────────────────────
// Shared by dwarf_writer and dwarf_reader — defined once here to avoid drift.
inline constexpr uint8_t  LEB128_VALUE_MASK = 0x7F;  // low 7 bits of each LEB128 byte
inline constexpr uint8_t  LEB128_MORE_BIT   = 0x80;  // continuation bit
inline constexpr uint8_t  LEB128_SIGN_BIT   = 0x40;  // sign bit of last group (SLEB128)
inline constexpr unsigned LEB128_SHIFT      = 7;      // bits per LEB128 group
inline constexpr unsigned LEB128_MAX_SHIFT  = 64U;    // total bits in int64_t (SLEB128 sign extension)

} // namespace aiebu::dwarf5

#endif // AIEBU_COMMON_DWARF_CONSTANTS_H_
