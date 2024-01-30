from ctrlcode.ops.isaOp import IsaOp
from ctrlcode.ops.wordOp import WordOp
from ctrlcode.ops.alignOp import AlignOp
from ctrlcode.ops.ucDmaOp import UcDmaOp
from ctrlcode.common.op_arg import OpArg

class ISA:
  UC_ISA_OPS = {
    # uC ISA:
  {% for operation in operations %}  '{{operation.mnemonic.lower()}}' : IsaOp({{operation.opcode}}, [
  {% for arg in operation.arguments %}    OpArg({%if arg.type != 'pad' %}'{{arg.name}}'{% else %}'_pad'{% endif %}, OpArg.{% if arg.type == 'register' %}REG{% else %}{{arg.type.upper()}}{% endif %}, {{get_arg_width(arg)}}),
  {% endfor %} 
    ]),
  {% endfor %}

    # Non-uC-ISA:
    'word': WordOp(),
    'align': AlignOp(),
    'uc_dma_bd': UcDmaOp()
  }

  UC_ISA_OPS_REVERSE = {
    # uC ISA:
  {% for operation in operations %}  {{operation.opcode}} : IsaOp('{{operation.mnemonic.lower()}}', [
  {% for arg in operation.arguments %}    OpArg({%if arg.type != 'pad' %}'{{arg.name}}'{% else %}'_pad'{% endif %}, OpArg.{% if arg.type == 'register' %}REG{% else %}{{arg.type.upper()}}{% endif %}, {{get_arg_width(arg)}}),
  {% endfor %} 
    ]),
  {% endfor %}

    # Non-uC-ISA:
    'word': WordOp(),
    165: AlignOp(),
    'uc_dma_bd': UcDmaOp()
  }
