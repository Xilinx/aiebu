// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _ISA_ASSEMBLER_STUBS_H_
#define _ISA_ASSEMBLER_STUBS_H_

#include "oparg.h"
#include "ops.h"

namespace aiebu {

class isa
{
private:
  std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> m_isa;

public:
  isa()
  {
    m_isa = std::make_shared<std::map<std::string, std::shared_ptr<isa_op>>>();

    {% for operation in operations %}(*m_isa)["{{operation.mnemonic.lower()}}"] = std::make_shared<isa_op>("{{operation.mnemonic.lower()}}", {{operation.opcode}}, std::vector<opArg>{
    {% for arg in operation.arguments %} opArg({%if arg.type != 'pad' %}"{{arg.name}}"{% else %}"_pad"{% endif %}, opArg::optype::{% if arg.type == 'register' %}REG{% else %}{{arg.type.upper()}}{% endif %}, {{get_arg_width(arg)}}),{% endfor %}
    });

    {% endfor %}

    (*m_isa)[".align"] = std::make_shared<isa_op>(".align", 0XA5, std::vector<opArg>{});
    (*m_isa)[".long"] = std::make_shared<isa_op>(".long", 0/* dummy*/, std::vector<opArg>{});
    (*m_isa)["uc_dma_bd"] = std::make_shared<isa_op>("uc_dma_bd", 0/* dummy*/, std::vector<opArg>{});
    (*m_isa)["uc_dma_bd_shim"] = std::make_shared<isa_op>("uc_dma_bd_shim", 0/* dummy*/, std::vector<opArg>{});
  }

  std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> get_isamap()
  {
    return m_isa;
  }

};

}
#endif //_ISA_ASSEMBLER_STUBS_H_
