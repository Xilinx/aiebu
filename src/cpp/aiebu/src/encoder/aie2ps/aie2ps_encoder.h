// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_ENCODER_AIE2PS_ENCODER_H_
#define _AIEBU_ENCODER_AIE2PS_ENCODER_H_

#include <memory>

#include "encoder.h"
#include "writer.h"
#include "aie2ps_preprocessed_output.h"
#include "ops.h"
#include "aie2ps/isa.h"

namespace aiebu {

class aie2ps_encoder : public encoder
{
  std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> m_isa;
  std::vector<writer> twriter;
public:
  aie2ps_encoder() {     
    isa i;
    m_isa = i.get_isamap();
  }

  virtual std::vector<writer>
  process(std::shared_ptr<preprocessed_output> input) override;
  std::string get_TextSectionName(uint32_t colnum, pageid_type pagenum) {return ".ctrltext." + std::to_string(colnum) + "." + std::to_string(pagenum); }
  std::string get_DataSectionName(uint32_t colnum, pageid_type pagenum) {return ".ctrldata." + std::to_string(colnum) + "." + std::to_string(pagenum); }
  std::string get_PadSectionName(uint32_t colnum) {return ".pad." + std::to_string(colnum); }
  std::string page_writer(page& lpage, std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpad, std::map<std::string, uint32_t>& labelpageindex, uint32_t control_packet_index);
  void patch57(const writer& textwriter, writer& datawriter, offset_type offset, uint64_t patch);
  void fill_scratchpad(writer& padwriter,const std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpads);
  void fill_control_packet_symbols(writer& padwriter,const uint32_t col, const std::string& controlpacket_padname, std::vector<symbol>& syms, const std::map<std::string, std::shared_ptr<scratchpad_info>>& scratchpads);
  std::string findKey(const std::map<std::string, std::vector<std::string>>& myMap, const std::string& value);
};

}
#endif //_AIEBU_ENCODER_AIE2PS_ENCODER_H_
