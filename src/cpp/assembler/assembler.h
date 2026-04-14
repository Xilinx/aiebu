// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _ADSM_COMMOM_ASSEMBLER_H_
#define _ADSM_COMMOM_ASSEMBLER_H_

#include <memory>
#include <vector>
#include <map>
#include "aiebu/aiebu_assembler.h"
#include "symbol.h"
#include "utils.h"

namespace aiebu {

class preprocessor;
class encoder;
class elf_writer;
class preprocessor_input;

class assembler
{
public:
  enum class elf_type
  {
    aie2_transaction_blob,
    aie2_dpu_blob,
    aie2ps_asm,
    aie2_asm,
    aie2_config,
    aie4_asm,       // Covers all aie4 family (aie4, aie4a, aie4z) - specific target from .target directive
    aie2ps_config,
    aie4_config     // Covers all aie4 family config - specific target from .target directive
  };

private:
  std::unique_ptr<preprocessor> m_preprocessor;
  std::unique_ptr<encoder> m_enoder;
  std::unique_ptr<elf_writer> m_elfwriter;
  std::shared_ptr<preprocessor_input> m_ppi;
  elf_type m_elf_type;

  // Check if target from .target directive belongs to expected family for elf_type
  // Returns true if valid, throws error if mismatch
  void validate_target_family(const std::string& parsed_target) const;
  void validate_target_family(std::shared_ptr<const target_info> target) const;

  // Configure ELF writer based on .target directive (sets OSABI and version)
  void configure_elf_for_target(const std::string& parsed_target);
  void configure_elf_for_target(std::shared_ptr<const target_info> target);

public:
  explicit assembler(const elf_type type);

  std::vector<char> process(const std::vector<char>& buffer1,
                            const std::vector<std::string>& libs = {},
                            const std::vector<std::string>& libpaths = {},
                            const std::vector<char>& patch_json = {},
                            const std::vector<char>& buffer2 = {},
                            const std::map<uint32_t, std::vector<char> >& ctrlpkt = {},
                            const file_artifact* artifacts = nullptr);

  virtual ~assembler() = default;

};

}

#endif //_ADSM_COMMOM_ASSEMBLER_H_
