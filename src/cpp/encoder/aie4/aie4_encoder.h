// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.

#ifndef AIEBU_ENCODER_AIE4_ENCODER_H_
#define AIEBU_ENCODER_AIE4_ENCODER_H_

#include <memory>

#include "aie2ps_encoder.h"
#include "writer.h"
#include "aie2ps_preprocessed_output.h"
#include "ops.h"
#include "specification/aie2ps/isa.h"

namespace aiebu {

class aie4_encoder : public aie2ps_encoder
{
protected:
  
public:
  aie4_encoder(): aie2ps_encoder() {}

  std::shared_ptr<assembler_state>
  create_assembler_state(std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> isa,
                         std::vector<std::shared_ptr<asm_data>>& data,
                         std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpad,
                         std::map<std::string, uint32_t>& labelpageindex,
                         uint32_t control_packet_index, uint32_t optimize_level, bool makeunique) override
  {
    return std::make_shared<assembler_state_aie4>(isa, data, scratchpad, labelpageindex, control_packet_index, optimize_level, makeunique);
  }

  void
  patch57(const std::shared_ptr<section_writer> textwriter, std::shared_ptr<section_writer> datawriter, offset_type offset, uint64_t patch) override
  {
    offset = offset - textwriter->tell();
    uint64_t bd0 = datawriter->read_word(offset);
    uint64_t bd1 = datawriter->read_word(offset + 1*4);             // NOLINT

    uint64_t arg = (bd1 & 0xFFFFFFFF) + ((bd0 & 0x1FFFFFF) << 32); // NOLINT
    patch = arg + patch;
    datawriter->write_word_at(offset + 1*4, patch & 0xFFFFFFFF);    // NOLINT
    datawriter->write_word_at(offset, (((patch >> 32) & 0x1FFFFFF) | (bd0 & 0xFE000000)));  // NOLINT
  }

  void check_partition_info(std::shared_ptr<const partition_info> source, std::shared_ptr<const partition_info> dest) override
  {
    if(dest->get_numcore() != source->get_numcore() || dest->get_nummem() != source->get_nummem())
      throw error(error::error_code::invalid_asm, "Partition (core, mem) (" + std::to_string(dest->get_numcore()) + ", " + std::to_string(dest->get_numcore()) + ") != (" + std::to_string(source->get_nummem()) + ", " + std::to_string(source->get_nummem()) + ")\n");
  }

};

}
#endif //AIEBU_ENCODER_AIE4_ENCODER_H_
