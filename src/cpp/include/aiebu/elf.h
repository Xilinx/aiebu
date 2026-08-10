// SPDX-License-Identifier: MIT
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_ELF_H_
#define AIEBU_ELF_H_

#include "detail/span.h"

#include <array>
#include <cstdint>
#include <istream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace aiebu {

class elf_reader;

////////////////////////////////////////////////////////////////
// aiebu::elf - read-side representation of an AIE ELF binary
//
// Owns all parsed ELF data.  All span/reference accessors are
// valid for the lifetime of the elf object.
//
// This class is the canonical owner of AIE ELF knowledge inside
// AIEBU.  xrt::elf_impl delegates to it; direct ELFIO access is
// an implementation detail hidden here.
////////////////////////////////////////////////////////////////
class elf
{
public:
  ////////////////////////////////////////////////////////////////
  // Platform identity
  //
  // Canonical source for AIE OS/ABI values — replaces both
  // xrt::elf::platform and the aiebu::osabi_* constants.
  // Values match ELF e_ident[EI_OSABI].
  ////////////////////////////////////////////////////////////////
  enum class platform : uint8_t {
    aie2p         = 69,   // 0x45
    aie2ps_legacy = 70,   // 0x46  legacy group ELF
    aie2ps        = 64,   // 0x40
    aie4          = 75,   // 0x4B
    aie4a         = 86,   // 0x56
    aie4z         = 105   // 0x69
  };

  ////////////////////////////////////////////////////////////////
  // Patch-point types
  //
  // Transitional: used by xrt::elf_impl while patching still
  // lives in XRT.  Will be removed when patching moves to AIEBU.
  ////////////////////////////////////////////////////////////////

  // Identifies which ELF section a relocation targets.
  // Values and names mirror xrt_core::elf_patcher::buf_type.
  enum class buf_type : uint32_t {
    ctrltext        = 0,
    ctrldata        = 1,
    preempt_save    = 2,
    preempt_restore = 3,
    pdi             = 4,
    ctrlpkt_pm      = 5,
    pad             = 6,
    dump            = 7,
    ctrlpkt         = 8,
    buf_type_count  = 9
  };

  // Relocation type encoding — mirrors xrt_core::elf_patcher::symbol_type.
  enum class patch_schema : uint32_t {
    uc_dma_remote_ptr             = 1,
    shim_dma_base_addr            = 2,
    scalar_32bit                  = 3,
    control_packet_48             = 4,
    shim_dma_48                   = 5,
    shim_dma_aie4_base_addr       = 6,
    control_packet_57             = 7,
    address_64                    = 8,
    control_packet_57_aie4        = 9,
    unknown                       = 10,
    pl_ddr_64                     = 11
  };

  // One relocation entry expressed in AIEBU-owned types.
  // Transitional — will be removed when patching moves to AIEBU.
  struct patch_point {
    std::string  arg_name;
    patch_schema schema;
    buf_type     target_buf;
    uint64_t     section_offset;   // absolute offset into the assembled buffer
    uint32_t     base_bo_offset;   // from addend upper bits
    uint32_t     mask;             // only meaningful for scalar_32bit
  };

  ////////////////////////////////////////////////////////////////
  // Kernel metadata
  ////////////////////////////////////////////////////////////////

  struct arg {
    std::string name;
    std::string data_type;   // e.g. "void*", "char*"
    uint32_t    index = 0;
    bool        is_global = false;
  };

  struct kernel {
    std::string              name;
    std::vector<arg>         args;
    std::vector<std::string> instances;
  };

  ////////////////////////////////////////////////////////////////
  // Construction
  ////////////////////////////////////////////////////////////////

  explicit elf(const std::string& filename);
  explicit elf(std::istream& stream);
  elf(const void* data, size_t size);
  explicit elf(std::string_view data);

  ~elf();

  elf(const elf&) = delete;
  elf& operator=(const elf&) = delete;
  elf(elf&&) noexcept;
  elf& operator=(elf&&) noexcept;

  ////////////////////////////////////////////////////////////////
  // Platform / version
  ////////////////////////////////////////////////////////////////

  platform
  get_platform() const;

  uint8_t
  get_os_abi() const;

  // Returns (major, minor): upper nibble = major, lower nibble = minor
  std::pair<uint8_t, uint8_t>
  get_abi_version() const;

  ////////////////////////////////////////////////////////////////
  // Shape queries
  ////////////////////////////////////////////////////////////////

  // True when ELF contains .note.xrt.configuration (can replace xclbin)
  bool
  is_full_elf() const;

  // True when ELF uses .group sections (version-dependent)
  bool
  is_group_elf() const;

  ////////////////////////////////////////////////////////////////
  // Metadata
  ////////////////////////////////////////////////////////////////

  std::array<uint8_t, 16>
  get_cfg_uuid() const;

  uint32_t
  get_partition_size() const;

  ////////////////////////////////////////////////////////////////
  // Kernel metadata
  ////////////////////////////////////////////////////////////////

  std::vector<kernel>
  get_kernels() const;

  ////////////////////////////////////////////////////////////////
  // Section access
  //
  // Returned spans are zero-copy views into memory owned by this
  // elf object.  Do not use after the elf is destroyed.
  ////////////////////////////////////////////////////////////////

  // Named access for sections AIEBU does not model explicitly.
  // Returns empty span when section is not found.
  aiebu::detail::span<const std::byte>
  get_section(std::string_view name) const;

  // Serialise the ELF back to a stream (replaces elfio.save() call sites).
  void
  save(std::ostream& stream) const;

  ////////////////////////////////////////////////////////////////
  // Group / ctrl-code navigation
  //
  // Low-level scaffolding used by xrt::elf_impl during the
  // transition.  Will be replaced by a higher-level API later.
  ////////////////////////////////////////////////////////////////

  // section index -> group index  (UINT32_MAX = legacy / no group)
  std::map<uint32_t, uint32_t>
  get_section_to_group_map() const;

  // group index -> member section indices
  std::map<uint32_t, std::vector<uint32_t>>
  get_group_to_sections_map() const;

  // "kernel_name + subkernel_name" -> group index
  std::map<std::string, uint32_t>
  get_kernel_name_to_id_map() const;

  // Ctrl-code id for a named kernel/subkernel.
  // Accepts "kernel:subkernel" or bare "kernel" (single-instance only).
  // Returns UINT32_MAX for legacy ELFs with no group sections.
  uint32_t
  get_ctrlcode_id(const std::string& name) const;

  ////////////////////////////////////////////////////////////////
  // PDI / preemption access (AIE gen2 only)
  //
  // Buffers are exposed via size + copy pairs rather than spans to
  // avoid an intermediate heap allocation: callers allocate a BO of
  // the reported size, then copy directly from ELFIO memory into it.
  ////////////////////////////////////////////////////////////////

  // Returns 0 when symbol is not found.
  size_t
  get_pdi_size(const std::string& symbol_name) const;

  // Copies PDI data for symbol into dest.  dest.size() must be at least
  // get_pdi_size(symbol_name).  No-op when symbol not found.
  void
  copy_pdi(const std::string& symbol_name, aiebu::detail::span<std::byte> dest) const;

  std::set<std::string>
  get_ctrlpkt_pm_dynsyms() const;

  // Returns 0 when symbol is not found.
  size_t
  get_ctrlpkt_pm_buf_size(const std::string& symbol_name) const;

  // Copies ctrlpkt_pm data for symbol into dest.  dest.size() must be at least
  // get_ctrlpkt_pm_buf_size(symbol_name).  No-op when not found.
  void
  copy_ctrlpkt_pm_buf(const std::string& symbol_name, aiebu::detail::span<std::byte> dest) const;

  ////////////////////////////////////////////////////////////////
  // Patch-point access (transitional — see Phase 2 of spec)
  ////////////////////////////////////////////////////////////////

  // Returns all patch points grouped by ctrl-code-id.
  // The key string encodes arg_name + buf_type (see elf_patcher::generate_key_string).
  // Transitional: will be removed when patching moves to AIEBU.
  std::map<uint32_t, std::map<std::string, std::vector<patch_point>>>
  get_patch_points() const;

  // Control scratch-pad memory size derived from dynsym (AIE gen2 only).
  size_t
  get_ctrl_scratch_pad_mem_size() const;

private:
  std::unique_ptr<elf_reader> m_reader;
};

} // namespace aiebu

#endif // AIEBU_ELF_H_
