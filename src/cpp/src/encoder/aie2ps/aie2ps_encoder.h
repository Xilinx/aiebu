// SPDX-License-Identifier: Apache-2.0
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
  std::shared_ptr<writer> twriter = std::make_shared<writer>();
public:
  aie2ps_encoder() {     
    isa i;
    m_isa = i.get_isamap();
  }

  virtual std::shared_ptr<writer>
  process(std::shared_ptr<preprocessed_output> input) override;
  
  void page_writer(page& lpage, std::vector<symbol>& sym);

};

}
#endif //_AIEBU_ENCODER_AIE2PS_ENCODER_H_
