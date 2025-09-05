// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ENCODER_AIE2_BLOB_ENCODER_H_
#define _AIEBU_ENCODER_AIE2_BLOB_ENCODER_H_

#include <memory>

#include "encoder.h"
#include "aie2_blob_preprocessed_output.h"

namespace aiebu {

class aie2_blob_encoder: public encoder
{
public:
  aie2_blob_encoder() {}

  std::vector<std::shared_ptr<writer>> process(std::shared_ptr<preprocessed_output> input) override
  {
    // encode : nothing to be done as blob is already encoded
    auto rinput = std::static_pointer_cast<aie2_blob_preprocessed_output>(input);
    std::vector<std::shared_ptr<writer>> rwriter;

    for(auto key : rinput->get_keys())
      if ( !key.compare(".ctrltext") )
        rwriter.emplace_back(std::make_shared<section_writer>(key, code_section::text, std::move(rinput->get_data(key))));
      else
        rwriter.emplace_back(std::make_shared<section_writer>(key, code_section::data, std::move(rinput->get_data(key))));

    std::shared_ptr<section_writer> element = std::dynamic_pointer_cast<section_writer>(rwriter[0]);
    element->add_symbols(rinput->get_symbols());
    return rwriter;
  }
};

class aie2_config_encoder : public encoder
{
  std::vector<std::shared_ptr<writer>> twriter;
  void check_partition_info(std::shared_ptr<const partition_info> source, std::shared_ptr<const partition_info> dest)
  {
    if(dest->get_numcolumn() != source->get_numcolumn())
      throw error(error::error_code::invalid_asm, "Partition column " + std::to_string(dest->get_numcolumn()) + " != " + std::to_string(source->get_numcolumn()) + "\n");
  }
public:
  std::vector<std::shared_ptr<writer>>
  process(std::shared_ptr<preprocessed_output> input) override
  {
    auto tinput = std::static_pointer_cast<aie2_config_preprocessed_output>(input);
    // lets get partition info for first instance and compare this with other
    // instances
    if (tinput->get_kernel_map().empty())
        return twriter;
    if (tinput->get_kernel_map().begin()->second.get_instance_map().empty())
        return twriter;
    auto pinfo_first_instance = tinput->get_kernel_map().begin()->second.get_instance_map().begin()->second->get_partition_info();
    auto output_writer = std::make_shared<aie2_config_writer>(pinfo_first_instance);
    twriter.push_back(output_writer);

    for (auto& [kernel, instances] : tinput->get_kernel_map()) {
       for(auto [dname, data] : instances.get_common())
         output_writer->add_kernel_common_data(kernel, std::make_shared<section_writer>(dname, code_section::text, std::move(data)));

      for(auto& [iname, instance] : instances.get_instance_map())
      {
        aie2_blob_encoder encoder_object;
        check_partition_info(instance->get_partition_info(), output_writer->get_partition_info());
        output_writer->add_kernel_map(kernel, iname, encoder_object.process(instance));
      }
    }
    return twriter;
  }
};
}
#endif //_AIEBU_ENCODER_AIE2_BLOB_ENCODER_H_
