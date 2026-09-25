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
  bool m_merged_ctrltext_elf;

protected:
  bool use_merged_ctrltext_sections() const { return m_merged_ctrltext_elf; }

  std::string merged_ctrltext_section_name(uint32_t colnum) const
  {
    return ".ctrltext." + std::to_string(colnum);
  }

  /**
   * Encode one page. If merged_writer is set, append [header+text+data+pad] to that section and
   * append relocation symbols to *merged_sym (caller adjusts positions). Otherwise
   * emit per-page .ctrltext.<col>.<page> and .ctrldata.<col>.<page> sections.
   */
  void page_writer(page& lpage, std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpad,
                   std::map<std::string, uint32_t>& labelpageindex,
                   std::map<uint32_t, std::string>& ctrlpkt_id_map, uint32_t optimization_level,
                   std::shared_ptr<section_writer> merged_writer, std::vector<symbol>* merged_sym);

public:
  explicit aie2ps_encoder(bool merged_ctrltext_elf = false)
    : m_merged_ctrltext_elf(merged_ctrltext_elf)
  {
    isa i;
    m_isa = i.get_isamap();
    m_report.set_merged(merged_ctrltext_elf);
  }

  void set_merged_ctrltext_elf(bool merged) override { m_merged_ctrltext_elf = merged; m_report.set_merged(merged); }

  virtual std::vector<std::shared_ptr<writer>>
  process(std::shared_ptr<preprocessed_output> input) override;
  virtual std::string get_TextSectionName(uint32_t colnum, pageid_type pagenum)
  {
    return ".ctrltext." + std::to_string(colnum) + "." + std::to_string(pagenum);
  }
  virtual std::string get_DataSectionName(uint32_t colnum, pageid_type pagenum)
  {
    return ".ctrldata." + std::to_string(colnum) + "." + std::to_string(pagenum);
  }
  virtual std::string get_PadSectionName(uint32_t colnum) { return ".pad." + std::to_string(colnum); }

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
    return std::make_shared<assembler_state_aie2ps>(isa, data, scratchpad, labelpageindex, ctrlpkt_id_map,
                                                    optimize_level, makeunique, use_merged_ctrltext_sections());
  }

  std::vector<std::shared_ptr<writer>> get_writers() { return twriter; }

  virtual void check_partition_info(std::shared_ptr<const partition_info> source, std::shared_ptr<const partition_info> dest)
  {
    if(dest->get_numcolumn() != source->get_numcolumn())
      throw error(error::error_code::invalid_asm, "Partition column " + std::to_string(dest->get_numcolumn()) + " != " + std::to_string(source->get_numcolumn()) + "\n");
  }

  virtual void check_target_info(std::shared_ptr<const target_info> source, std::shared_ptr<const target_info> dest)
  {
    if (!source || !dest)
      return;  // Skip check if either is null
    if (!source->is_set() || !dest->is_set())
      return;  // Skip check if either is not set
    if (source->get_arch() != dest->get_arch())
      throw error(error::error_code::invalid_asm, "Target architecture mismatch: '" + source->get_full_target() + "' != '" + dest->get_full_target() + "'\n");
    if (source->get_sub_arch() != dest->get_sub_arch())
      throw error(error::error_code::invalid_asm, "Target sub-architecture mismatch: '" + source->get_full_target() + "' != '" + dest->get_full_target() + "'\n");
  }

  virtual void check_aie_row_topology_info(std::shared_ptr<const aie_row_topology_info> source, std::shared_ptr<const aie_row_topology_info> dest)
  {
    if (!source || !dest)
      return;  // Skip check if either is null
    if (!source->is_set() || !dest->is_set())
      return;  // Skip check if either is not set
    if (source->get_num_south_shim() != dest->get_num_south_shim() ||
        source->get_num_memtile_row() != dest->get_num_memtile_row() ||
        source->get_num_coretile_row() != dest->get_num_coretile_row() ||
        source->get_num_north_shim() != dest->get_num_north_shim())
      throw error(error::error_code::invalid_asm, "AIE row topology mismatch: '" + source->get_topology_string() + "' != '" + dest->get_topology_string() + "'\n");
  }
};

//asm_config_preprocessor<aie2ps_config_encoder, aie2ps_preprocessed_output>
template <typename T, typename input_tamplete>
class asm_config_encoder : public encoder
{
  bool m_merged_ctrltext_elf;
  std::vector<std::shared_ptr<writer>> twriter;
public:
  explicit asm_config_encoder(bool merged_ctrltext_elf = false)
    : m_merged_ctrltext_elf(merged_ctrltext_elf) {}

  void set_merged_ctrltext_elf(bool merged) override { m_merged_ctrltext_elf = merged; }

  std::vector<std::shared_ptr<writer>>
  process(std::shared_ptr<preprocessed_output> input) override
  {
    auto tinput = std::static_pointer_cast<asm_config_preprocessed_output<input_tamplete>>(input);
    // lets get partition, target, and row topology info for first instance and compare this with other instances
    auto first_instance = tinput->get_kernel_map().begin()->second.begin()->second;
    auto pinfo_first_instance = first_instance->get_partition_info();
    auto tinfo_first_instance = first_instance->get_target_info();
    auto rinfo_first_instance = first_instance->get_aie_row_topology_info();
    auto output_writer = std::make_shared<config_writer>(pinfo_first_instance);
    twriter.push_back(output_writer);

    output_writer->add_section_writers_from_custom_section_map(tinput->get_global_custom_sections());

    for (auto& [kernel, instances] : tinput->get_kernel_map()) {
      for(auto& [iname, instance] : instances)
      {
        T encoder_object(m_merged_ctrltext_elf);
        encoder_object.check_partition_info(instance->get_partition_info(), output_writer->get_partition_info());
        encoder_object.check_target_info(instance->get_target_info(), tinfo_first_instance);
        encoder_object.check_aie_row_topology_info(instance->get_aie_row_topology_info(), rinfo_first_instance);
        auto instance_writers = encoder_object.process(instance);
        output_writer->add_kernel_map(kernel, iname, instance_writers);
      }
    }
    return twriter;
  }
};
}
#endif //_AIEBU_ENCODER_AIE2PS_ENCODER_H_
