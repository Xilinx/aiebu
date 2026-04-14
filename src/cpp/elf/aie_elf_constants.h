// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_ELF_AIE_ELF_CONSTANTS_H_
#define AIEBU_ELF_AIE_ELF_CONSTANTS_H_

namespace aiebu {

// ============================================================================
// OS/ABI values for AIE architectures
// Values chosen with high Hamming distance for robustness against bit flips
// ============================================================================
constexpr unsigned char osabi_aie2ps_group = 0x46;  // 70 - Legacy group ELF (AIE2PS/AIE4 family)
constexpr unsigned char osabi_aie2p        = 0x45;  // 69 - AIE2P
constexpr unsigned char osabi_aie2ps       = 0x40;  // 64 - AIE2PS specific
constexpr unsigned char osabi_aie4         = 0x4B;  // 75 - AIE4
constexpr unsigned char osabi_aie4a        = 0x56;  // 86 - AIE4A
constexpr unsigned char osabi_aie4z        = 0x69;  // 105 - AIE4Z

// ============================================================================
// ELF ABI Version values
// Encoding: upper nibble = major version, lower nibble = minor version
// ============================================================================
constexpr unsigned char elf_version_legacy        = 0x02;  // 0.2 - Non-config (all architectures)
constexpr unsigned char elf_version_legacy_config = 0x03;  // 0.3 - Legacy config (aie2ps/aie4 without .target)
constexpr unsigned char elf_version_aie2p_config  = 0x10;  // 1.0 - Legacy AIE2P config (read-only, no longer written)
constexpr unsigned char elf_version_config        = 0x20;  // 2.0 - Config with .target or aie2/aie2p config

}
#endif // AIEBU_ELF_AIE_ELF_CONSTANTS_H_
