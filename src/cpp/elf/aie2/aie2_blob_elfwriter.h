// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ELF_AIE2_BLOB_ELF_WRITER_H_
#define _AIEBU_ELF_AIE2_BLOB_ELF_WRITER_H_

#include <elfwriter.h>

namespace aiebu {

class aie2_blob_elf_writer: public elf_writer
{
  constexpr static unsigned char ob_abi = 0x45;
  constexpr static unsigned char version = 0x02;
public:
  aie2_blob_elf_writer(): elf_writer(ob_abi, version)
  { }
};

class aie2_config_elf_writer: public elf_writer
{
  constexpr static unsigned char ob_abi = 0x45;
  constexpr static unsigned char version = 0x10;
  const std::string xrt_configuration = ".note.xrt.configuration";

public:
  aie2_config_elf_writer(): elf_writer(ob_abi, version)
  { }

  /**
   * This function gets config_writer object which has map<kernel, map<instance, vector<code>>.
   * It traverse each kernel, its instances and push corresponding pages in elf.
   * It add dynamic section correspondly.
   * It add partition info in note section and finalize to generate elf
   */
  std::vector<char>
  process(std::vector<std::shared_ptr<writer>>& mwriter) override
  {

    if (mwriter.empty())
      return {};

    auto mconfig_writer = std::dynamic_pointer_cast<aie2_config_writer>(mwriter[0]);
    init_symtab();
    uint32_t index=0;
    for( auto& [kernel, instances] : mconfig_writer->get_kernel_map())
    {
       auto kernel_index = add_symtab(kernel);
       std::vector<uint32_t> common_data = process_common_helper(instances.get_common(), "");
       for(auto& [iname, instance] : instances.get_instance_map())
       {
         auto instance_index = add_symtab_section(iname, kernel_index);
         std::vector<uint32_t> group_data = process_common_helper(instance, get_section_prefix(index));
         // first word is GRP_COMDAT
         group_data.insert(group_data.begin(), 1);
         //group_data.insert(group_data.end(), common_data.begin(), common_data.end());
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
#endif //_AIEBU_ELF_AIE2_BLOB_ELF_WRITER_H_
