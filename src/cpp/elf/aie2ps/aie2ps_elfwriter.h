// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ELF_AIE2PS_ELF_WRITER_H_
#define _AIEBU_ELF_AIE2PS_ELF_WRITER_H_

#include <elfwriter.h>
#include <aie_elf_constants.h>

namespace aiebu {

class aie2ps_elf_writer: public elf_writer
{
public:
  aie2ps_elf_writer(): elf_writer(osabi_aie2ps_group, elf_version_legacy)
  { }
};

class aie2ps_config_elf_writer: public elf_writer
{
  const std::string const_configuration = "configuration";
  const std::string xrt_configuration = ".note.xrt.configuration";
  const std::string const_kernel_signature = "kernel.signature";

public:
  aie2ps_config_elf_writer(): elf_writer(osabi_aie2ps_group, elf_version_legacy_config)
  { }

  /**
   * This function gets config_writer object which has map<kernel, map<instance, vector<pages>>.
   * It traverse each kernel, its instances and push corresponding pages in elf.
   * It add dynamic section correspondly.
   * It add partition info in note section and finalize to generate elf
   */
  std::vector<char>
  process(std::vector<std::shared_ptr<writer>>& mwriter) override
  {
    auto mconfig_writer = std::dynamic_pointer_cast<config_writer>(mwriter[0]);
    init_symtab();
    uint32_t index=0;

    process_global_custom_sections_if_any(mconfig_writer->get_global_custom_sections());

    for( auto& [kernel, instances] : mconfig_writer->get_kernel_map())
    {
       auto kernel_index = add_symtab(kernel);
       for(auto& [iname, instance] : instances)
       {
         auto instance_index = add_symtab_section(iname, kernel_index);
         std::vector<uint32_t> group_data = process_common_helper(instance, get_section_prefix(index));
         // first word is GRP_COMDAT
         group_data.insert(group_data.begin(), 1);
         add_group(get_group_name(index), group_data, instance_index);
         index++;
       }
    }
    if (dstr_sec)
      add_dynamic_section();
    std::vector<char> configuration_vec(4);
    auto col = mconfig_writer->get_partition_info()->get_numcolumn();
    std::memcpy(configuration_vec.data(), &col, sizeof(uint32_t));

    add_note(NT_XRT_PARTITION_SIZE, xrt_configuration, configuration_vec);
    return finalize();
  }
};
}
#endif //_AIEBU_ELF_AIE2PS_ELF_WRITER_H_
