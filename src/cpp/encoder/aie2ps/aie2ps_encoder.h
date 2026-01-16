// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ENCODER_AIE2PS_ENCODER_H_
#define _AIEBU_ENCODER_AIE2PS_ENCODER_H_

#include <memory>

#include "encoder.h"
#include "writer.h"
#include "aie2ps_preprocessed_output.h"
#include "ops.h"
#include "specification/aie2ps/isa.h"
#include "report.h"

namespace aiebu {

class aie2ps_encoder : public encoder
{
  std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> m_isa;
  std::vector<std::shared_ptr<writer>> twriter;
  asm_report m_report;
  Debug m_debug;
  asm_dump_flag m_dump_flag = asm_dump_flag::disable;
public:
  aie2ps_encoder() {
    isa i;
    m_isa = i.get_isamap();
  }

  virtual std::vector<std::shared_ptr<writer>>
  process(std::shared_ptr<preprocessed_output> input) override;
  virtual std::string get_TextSectionName(uint32_t colnum, pageid_type pagenum) {return ".ctrltext." + std::to_string(colnum) + "." + std::to_string(pagenum); }
  virtual std::string get_DataSectionName(uint32_t colnum, pageid_type pagenum) {return ".ctrldata." + std::to_string(colnum) + "." + std::to_string(pagenum); }
  virtual std::string get_PadSectionName(uint32_t colnum) {return ".pad." + std::to_string(colnum); }
  void page_writer(page& lpage, std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpad,
  std::map<std::string, uint32_t>& labelpageindex, std::map<uint32_t, std::string>& ctrlpkt_id_map, uint32_t optimization_level);

  virtual void patch57(const std::shared_ptr<section_writer> textwriter, std::shared_ptr<section_writer> datawriter, offset_type offset, uint64_t patch);
  virtual void patch_cp_57(const std::shared_ptr<section_writer> ctrlpktwriter, offset_type offset, uint64_t patch);
  void fill_scratchpad(std::shared_ptr<section_writer> padwriter,const std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpads);
  void fill_controlpkt(std::shared_ptr<section_writer> padwriter, const std::vector<char>& ctrlpkt);
  void fill_control_packet_symbols(std::shared_ptr<section_writer> ctrlpktwriter, std::vector<symbol>& syms);

  std::string findKey(const std::map<std::string, std::vector<std::string>>& myMap, const std::string& value);

  virtual std::shared_ptr<assembler_state>
  create_assembler_state(std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> isa,
                         std::vector<std::shared_ptr<asm_data>>& data,
                         std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpad,
                         std::map<std::string, uint32_t>& labelpageindex,
                         std::map<uint32_t, std::string>& ctrlpkt_id_map, uint32_t optimize_level, bool makeunique)
  {
    return std::make_shared<assembler_state_aie2ps>(isa, data, scratchpad, labelpageindex, ctrlpkt_id_map, optimize_level, makeunique);
  }

  std::vector<std::shared_ptr<writer>> get_writers() { return twriter; }

  virtual void check_partition_info(std::shared_ptr<const partition_info> source, std::shared_ptr<const partition_info> dest)
  {
    if(dest->get_numcolumn() != source->get_numcolumn())
      throw error(error::error_code::invalid_asm, "Partition column " + std::to_string(dest->get_numcolumn()) + " != " + std::to_string(source->get_numcolumn()) + "\n");
  }
};

//asm_config_preprocessor<aie2ps_config_encoder, aie2ps_preprocessed_output>
template <typename T, typename input_tamplete>
class asm_config_encoder : public encoder
{
  std::vector<std::shared_ptr<writer>> twriter;
public:
  std::vector<std::shared_ptr<writer>>
  process(std::shared_ptr<preprocessed_output> input) override
  {
    auto tinput = std::static_pointer_cast<asm_config_preprocessed_output<input_tamplete>>(input);
    // lets get partition info for first instance and compare this with other instances
    auto pinfo_first_instance = tinput->get_kernel_map().begin()->second.begin()->second->get_partition_info();
    auto output_writer = std::make_shared<config_writer>(pinfo_first_instance);
    twriter.push_back(output_writer);

    for (auto& [kernel, instances] : tinput->get_kernel_map()) {
      for(auto& [iname, instance] : instances)
      {
        T encoder_object;
        encoder_object.check_partition_info(instance->get_partition_info(), output_writer->get_partition_info());
        output_writer->add_kernel_map(kernel, iname, encoder_object.process(instance));
      }
    }
    return twriter;
  }
};
}
#endif //_AIEBU_ENCODER_AIE2PS_ENCODER_H_
