/* SPDX-License-Identifier: MIT */
/*
 * Copyright (C) 2023-2025 Advanced Micro Devices, Inc. All rights reserved.
 */

#ifndef _ISA_DEFINES_H_
#define _ISA_DEFINES_H_

#include <stdint.h>
#include "isa_stubs.h"

// Operation implementation forward declarations
{% for operation in operations %}
static unsigned int control_op_{{operation.mnemonic.lower()}}(const uint8_t *_pc{% for arg in operation.arguments if arg.type != 'pad' and arg.type != 'patch_buf' %}, {{get_arg_c_type(arg)}} {{arg.name}}{% if arg.type == 'register' %}_reg{% endif %}{% endfor %});{% endfor %}


// Dispatchers
{% for operation in operations %}
FORCE_INLINE_FOR_RELEASE_ONLY static inline unsigned int control_dispatch_{{operation.mnemonic.lower()}}(const uint8_t *pc)
{
  return control_op_{{operation.mnemonic.lower()}}(
    pc{% for arg in operation.arguments if arg.type != 'pad' and arg.type != 'patch_buf' %},
    /* {{arg.name}} ({{arg.type}}) */ *({{get_arg_c_type(arg)}} *)(&pc[{{arg._offset}}]){% endfor %}
  );
}
{% endfor %}

// Case statements for regular operations

#define DISPATCH_REGULAR_OPS{% for operation in operations if operation.regular %} \
  case ISA_OPCODE_{{operation.mnemonic.upper()}}: pc += control_dispatch_{{operation.mnemonic.lower()}}(pc); break;{% endfor %}


#endif
