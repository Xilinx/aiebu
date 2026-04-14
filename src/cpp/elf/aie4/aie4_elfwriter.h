// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_ELF_AIE4_ELF_WRITER_H_
#define AIEBU_ELF_AIE4_ELF_WRITER_H_

#include <aie2ps_elfwriter.h>

namespace aiebu {

/**
 * @brief AIE4 family ELF writer
 *
 * Reuses aie2ps_elf_writer since both use same default values for non-config
 * (OSABI = osabi_aie2ps_group = 0x46, version = elf_version_legacy = 0x02).
 * Non-config ELFs always use legacy values regardless of .target directive.
 */
using aie4_elf_writer = aie2ps_elf_writer;

/**
 * @brief AIE4 family config ELF writer
 *
 * Reuses aie2ps_config_elf_writer which defaults to legacy values
 * (OSABI = osabi_aie2ps_group = 0x46, version = elf_version_legacy_config = 0x03).
 * When .target directive is present in ASM, assembler will call
 * set_os_abi() and set_abi_version() to set version 0x20 and specific OSABI.
 */
using aie4_config_elf_writer = aie2ps_config_elf_writer;

}
#endif //AIEBU_ELF_AIE4_ELF_WRITER_H_
