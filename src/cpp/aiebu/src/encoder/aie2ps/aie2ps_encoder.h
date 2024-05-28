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
  std::string get_TextSectionName(uint32_t colnum, pageid_type pagenum) {return ".ctrltext_" + std::to_string(colnum) + "_" + std::to_string(pagenum); }
  std::string get_DataSectionName(uint32_t colnum, pageid_type pagenum) {return ".ctrldata_" + std::to_string(colnum) + "_" + std::to_string(pagenum); }
  void page_writer(page& lpage);

};

}
#endif //_AIEBU_ENCODER_AIE2PS_ENCODER_H_
