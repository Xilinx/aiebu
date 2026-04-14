// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.
#include "assembler.h"

#include "aie2ps_preprocessed_output.h"
#include "aie2_asm_preprocessor.h"
#include "aie2_blob_elfwriter.h"
#include "aie2_blob_encoder.h"
#include "aie2_blob_preprocessor.h"
#include "aie2ps_elfwriter.h"
#include "aie2ps_encoder.h"
#include "aie2ps_preprocessor.h"
#include "encoder/aie4/aie4_encoder.h"
#include "preprocessor/aie4/aie4_preprocessor.h"
#include "preprocessor/aie4/aie4_preprocessor_input.h"
#include "elf/aie4/aie4_elfwriter.h"
#include "elfwriter.h"
#include "encoder.h"
#include "preprocessor.h"

#include "aiebu/aiebu_error.h"

namespace aiebu {

// Normalize target string format only (dash removal)
// "aie4-a" -> "aie4a", "aie4-z" -> "aie4z"
static std::string
normalize_target_format(const std::string& target)
{
  std::string normalized = target;
  // Remove dash if present (e.g., "aie4-a" -> "aie4a")
  size_t dash_pos = normalized.find('-');
  if (dash_pos != std::string::npos)
    normalized.erase(dash_pos, 1);
  return normalized;
}

// Check if target belongs to aie4 family (aie4, aie4a, aie4z)
static bool
is_aie4_family(const std::string& normalized_target)
{
  return normalized_target == "aie4" ||
         normalized_target == "aie4a" ||
         normalized_target == "aie4z";
}

// Check if target is aie2ps
static bool
is_aie2ps_target(const std::string& normalized_target)
{
  return normalized_target == "aie2ps";
}

void
assembler::
validate_target_family(const std::string& parsed_target) const
{
  std::string normalized = normalize_target_format(parsed_target);

  switch (m_elf_type) {
    case elf_type::aie4_asm:
    case elf_type::aie4_config:
      // -t aie4 or aie4_config: .target must be aie4 family
      if (!is_aie4_family(normalized)) {
        throw error(error::error_code::invalid_asm,
                    "Incorrect target option provided: ASM is for target architecture '" +
                    parsed_target + "' which is not in the aie4 family");
      }
      break;

    case elf_type::aie2ps_asm:
    case elf_type::aie2ps_config:
      // -t aie2ps or aie2ps_config: .target must be aie2ps
      if (!is_aie2ps_target(normalized)) {
        throw error(error::error_code::invalid_asm,
                    "Incorrect target option provided: ASM is for target architecture '" +
                    parsed_target + "' while the target used is 'aie2ps'");
      }
      break;

    default:
      // No target family validation for other types (aie2 blob, etc.)
      break;
  }
}

void
assembler::
validate_target_family(std::shared_ptr<const target_info> target) const
{
  if (!target || !target->is_set())
    return;

  // Build full target string from target_info
  std::string full_target = target->get_full_target();
  validate_target_family(full_target);
}

void
assembler::
configure_elf_for_target(const std::string& parsed_target)
{
  // Only config ELFs get updated based on .target directive
  // Non-config ELFs always use legacy values (version 0x02, OSABI = aie2ps_group)
  if (m_elf_type != elf_type::aie4_config && m_elf_type != elf_type::aie2ps_config)
    return;

  std::string normalized = normalize_target_format(parsed_target);

  unsigned char os_abi = 0;

  // Determine OS ABI based on specific target
  if (normalized == "aie2ps") {
    os_abi = osabi_aie2ps;
  } else if (normalized == "aie4") {
    os_abi = osabi_aie4;
  } else if (normalized == "aie4a") {
    os_abi = osabi_aie4a;
  } else if (normalized == "aie4z") {
    os_abi = osabi_aie4z;
  } else {
    throw error(error::error_code::invalid_asm,
                "Unknown target in .target directive: " + parsed_target);
  }

  // Config ELFs with .target use version 0x20 (2.0) and specific OSABI
  m_elfwriter->set_os_abi(os_abi);
  m_elfwriter->set_abi_version(elf_version_config);
}

void
assembler::
configure_elf_for_target(std::shared_ptr<const target_info> target)
{
  if (!target || !target->is_set())
    return;

  // Build the normalized target string from target_info
  // arch = "aie4", sub_arch = "a" -> "aie4a"
  // arch = "aie4", sub_arch = "" -> "aie4"
  std::string full_target = target->get_arch();
  if (target->has_sub_arch())
    full_target += target->get_sub_arch();

  configure_elf_for_target(full_target);
}

assembler::
assembler(const elf_type type) : m_elf_type(type)
{

  if (type == elf_type::aie2_dpu_blob)  {
    m_preprocessor = std::make_unique<aie2_blob_preprocessor>();
    m_enoder = std::make_unique<aie2_blob_encoder>();
    m_elfwriter = std::make_unique<aie2_blob_elf_writer>();
    m_ppi = std::make_shared<aie2_blob_dpu_preprocessor_input>();
  }
  else if (type == elf_type::aie2_transaction_blob)  {
    m_preprocessor = std::make_unique<aie2_blob_preprocessor>();
    m_enoder = std::make_unique<aie2_blob_encoder>();
    m_elfwriter = std::make_unique<aie2_blob_elf_writer>();
    m_ppi = std::make_shared<aie2_blob_transaction_preprocessor_input>();
  }
  else if (type == elf_type::aie2_asm)  {
    m_preprocessor = std::make_unique<aie2_asm_preprocessor>();
    // Reuse encoder and elfwriter flow from the aie2 blob as they do not
    // see the ASM but instead see the same binary aie2 blob.
    m_enoder = std::make_unique<aie2_blob_encoder>();
    m_elfwriter = std::make_unique<aie2_blob_elf_writer>();
    m_ppi = std::make_shared<aie2_asm_preprocessor_input>();
  }
  else if (type == elf_type::aie2ps_asm)
  {
    m_preprocessor = std::make_unique<aie2ps_preprocessor>();
    m_enoder = std::make_unique<aie2ps_encoder>();
    m_elfwriter = std::make_unique<aie2ps_elf_writer>();
    m_ppi = std::make_shared<aie2ps_preprocessor_input>();
  }
  else if (type == elf_type::aie2_config)  {
    m_preprocessor = std::make_unique<aie2_config_preprocessor>();
    m_enoder = std::make_unique<aie2_config_encoder>();
    m_elfwriter = std::make_unique<aie2_config_elf_writer>();
    m_ppi = std::make_shared<aie2_config_preprocessor_input>();
  }
  else if (type == elf_type::aie4_asm)
  {
    // Handles all aie4 family (aie4, aie4a, aie4z) - specific OSABI set based on .target directive
    m_preprocessor = std::make_unique<aie4_preprocessor>();
    m_enoder = std::make_unique<aie4_encoder>();
    m_elfwriter = std::make_unique<aie4_elf_writer>();
    m_ppi = std::make_shared<aie4_preprocessor_input>();
  }
  else if (type == elf_type::aie2ps_config)
  {
    m_preprocessor = std::make_unique<asm_config_preprocessor<aie2ps_preprocessor, aie2ps_preprocessor_input, aie2ps_preprocessed_output>>();
    m_enoder = std::make_unique<asm_config_encoder<aie2ps_encoder, aie2ps_preprocessed_output>>();
    m_elfwriter = std::make_unique<aie2ps_config_elf_writer>();
    m_ppi = std::make_shared<controlcode_config_preprocessor_input<aie2ps_preprocessor_input>>();
  }
  else if (type == elf_type::aie4_config)
  {
    // Handles all aie4 family config (aie4, aie4a, aie4z) - specific OSABI set based on .target
    m_preprocessor = std::make_unique<asm_config_preprocessor<aie4_preprocessor, aie4_preprocessor_input, aie2ps_preprocessed_output>>();
    m_enoder = std::make_unique<asm_config_encoder<aie4_encoder, aie2ps_preprocessed_output>>();
    m_elfwriter = std::make_unique<aie4_config_elf_writer>();
    m_ppi = std::make_shared<controlcode_config_preprocessor_input<aie4_preprocessor_input>>();
  }
  else {
    throw error(error::error_code::invalid_buffer_type ,"Invalid elf type!!!");
  }
}

std::vector<char>
assembler::
process(const std::vector<char>& buffer1,
        const std::vector<std::string>& libs,
        const std::vector<std::string>& libpaths,
        const std::vector<char>& patch_json,
        const std::vector<char>& buffer2,
        const std::map<uint32_t, std::vector<char> >& ctrlpkt,
        const file_artifact* artifacts)
{
  m_ppi->set_args(buffer1, patch_json, buffer2, libs, libpaths, ctrlpkt, artifacts);
  auto ppo = m_preprocessor->process(m_ppi);

  // Check if .target directive is present and configure ELF accordingly
  // Try non-config output first
  std::shared_ptr<const target_info> target;
  auto aie2ps_output = std::dynamic_pointer_cast<aie2ps_preprocessed_output>(ppo);
  if (aie2ps_output) {
    target = aie2ps_output->get_target_info();
  } else {
    // Try config output - get target from first instance
    auto config_output = std::dynamic_pointer_cast<asm_config_preprocessed_output<aie2ps_preprocessed_output>>(ppo);
    if (config_output) {
      const auto& kernel_map = config_output->get_kernel_map();
      if (!kernel_map.empty()) {
        const auto& instances = kernel_map.begin()->second;
        if (!instances.empty()) {
          const auto& first_instance = instances.begin()->second;
          if (first_instance)
            target = first_instance->get_target_info();
        }
      }
    }
  }

  if (target && target->is_set()) {
    // .target is present in ASM:
    // 1. Validate that target belongs to correct family for the -t option
    // 2. For config ELFs: set version 0x20 and specific OSABI
    // 3. For non-config ELFs: keep legacy values (no change)
    validate_target_family(target);
    configure_elf_for_target(target);
  }
  // If no .target directive: ELF writer keeps legacy defaults
  // Non-config: version 0x02, OSABI = aie2ps_group (0x46)
  // Config: version 0x03, OSABI = aie2ps_group (0x46)

  auto w = m_enoder->process(ppo);
  auto u = m_elfwriter->process(w);
  return u;
}

}
