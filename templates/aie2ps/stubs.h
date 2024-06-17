/* SPDX-License-Identifier: MIT */
/*
 * Copyright (c) 2023 Advanced Micro Devices, Inc. All rights reserved.
 */

#ifndef _ISA_STUBS_H_
#define _ISA_STUBS_H_

// macro define
{% for macro_define in macro_defines %}
//{{macro_define.brief.lower()}}
#define {{macro_define.mnemonic.upper()}} {{'0x{:02x}'.format(macro_define.value)}}{% endfor %}

// Op codes
{% for operation in operations %}
#define ISA_OPCODE_{{operation.mnemonic.upper()}} {{'0x{:02x}'.format(operation.opcode)}}{% endfor %}


// Operation sizes
{% for operation in operations %}
#define ISA_OPSIZE_{{operation.mnemonic.upper()}} {{'0x{:02x}'.format(get_operation_size(operation))}}{% endfor %}

#endif
