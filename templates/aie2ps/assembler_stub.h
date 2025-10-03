// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef _ISA_ASSEMBLER_STUBS_H_
#define _ISA_ASSEMBLER_STUBS_H_

#include "oparg.h"
#include "ops.h"

namespace aiebu {

// Named constants for opcodes and bit widths
{% for operation in operations %}constexpr int OPCODE_{{operation.mnemonic.upper()}} = {{operation.opcode}};
{% endfor %}constexpr int OPCODE_ALIGN = 0xA5;
constexpr int BIT_WIDTH_8 = 8;
constexpr int BIT_WIDTH_16 = 16;
constexpr int BIT_WIDTH_32 = 32;

// ...existing code...
class isa
{
private:
  std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> m_isa;

public:
  isa()
  {
    m_isa = std::make_shared<std::map<std::string, std::shared_ptr<isa_op>>>();

    {% for operation in operations %}(*m_isa)["{{operation.mnemonic.lower()}}"] = std::make_shared<isa_op>("{{operation.mnemonic.lower()}}", OPCODE_{{operation.mnemonic.upper()}}, std::vector<opArg>{
    {% for arg in operation.arguments if arg.type != 'patch_buf' %} opArg({%if arg.type != 'pad' %}"{{arg.name}}"{% else %}"_pad"{% endif %}, opArg::optype::{% if arg.type == 'register' %}REG{% else %}{{arg.type.upper()}}{% endif %}, BIT_WIDTH_{{get_arg_width(arg)}}),{% endfor %}
    });

    {% endfor %}(*m_isa)[".align"] = std::make_shared<isa_op>(".align", OPCODE_ALIGN, std::vector<opArg>{});
    (*m_isa)[".long"] = std::make_shared<isa_op>(".long", 0/* dummy*/, std::vector<opArg>{});
    (*m_isa)["uc_dma_bd"] = std::make_shared<isa_op>("uc_dma_bd", 0/* dummy*/, std::vector<opArg>{});
    (*m_isa)["uc_dma_bd_shim"] = std::make_shared<isa_op>("uc_dma_bd_shim", 0/* dummy*/, std::vector<opArg>{});
  }

  std::shared_ptr<std::map<std::string, std::shared_ptr<isa_op>>> get_isamap()
  {
    return m_isa;
  }
};

class isa_disassembler
{
private:
  std::map<uint8_t, isa_op_disasm> m_isa_disasm;

public:
  isa_disassembler()
  {
    // All instruction entries by opcode - stack allocated
    {% for operation in operations | sort(attribute='opcode') %}m_isa_disasm.emplace(OPCODE_{{operation.mnemonic.upper()}}, isa_op_disasm("{{operation.mnemonic.lower()}}", OPCODE_{{operation.mnemonic.upper()}}, std::vector<opArg>{
     {% for arg in operation.arguments if arg.type != 'patch_buf' %} opArg({%if arg.type != 'pad' %}"{{arg.name}}"{% else %}"_pad"{% endif %}, opArg::optype::{% if arg.type == 'register' %}REG{% else %}{{arg.type.upper()}}{% endif %}, BIT_WIDTH_{{get_arg_width(arg)}}),{% endfor %}
    }));

    {% endfor %}// Pseudo-instructions
    m_isa_disasm.emplace(OPCODE_ALIGN, isa_op_disasm(".align", OPCODE_ALIGN, std::vector<opArg>{}));
  }

  const std::map<uint8_t, isa_op_disasm>* get_isa_map() const
  {
    return &m_isa_disasm;  // Return const pointer to owned data
  }
};

} // namespace aiebu
#endif // _ISA_ASSEMBLER_STUBS_H_
