// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include "ops.h"
#include "aiebu_error.h"
#include <string>
namespace aiebu {

offset_type
isa_op_serializer::size(assembler_state& state)
{
  offset_type result = 2; // 2 bytes for opcode
  for (auto &arg : m_opcode->get_args())
    result += arg.m_width/width_8;
  return result; 
}

offset_type
align_op_serializer::size(assembler_state& state)
{

  uint32_t align = std::stoi(m_args[0]);
  //TODO return alignment
  return ((state.get_pos() % align) > 0 ) ? (align - (state.get_pos() % align)) : 0;
}


std::vector<uint8_t>
isa_op_serializer::
serialize(assembler_state& state, std::vector<symbol>& symbols,
          uint32_t colnum, pageid_type pagenum)
{
  //encode isa_op
  std::vector<uint8_t> ret;
  ret.push_back(m_opcode->get_code());
  ret.push_back(pad);

  int arg_index = 0;
  opArg::optype atype;
  uint32_t val = 0;
  std::string sval;
  for (auto arg : m_opcode->get_args())
  {
    if (arg.m_type == opArg::optype::PAD)
    {
      sval = "0";
      atype = opArg::optype::CONST;
    } else if (arg.m_type == opArg::optype::JOBSIZE)
    {
      jobid_type jobid = state.parse_num_arg(m_args[0]);
      sval = std::to_string(state.m_jobmap[jobid]->get_size());
      atype = opArg::optype::CONST;
    } else
    {
      sval = m_args[arg_index];
      atype = arg.m_type;
      ++arg_index;
    }

    if (atype == opArg::optype::REG)
      ret.push_back(parse_register(sval) & BYTE_MASK);
    else if (atype == opArg::optype::BARRIER)
      ret.push_back(parse_barrier(sval) & BYTE_MASK);
    else if (atype == opArg::optype::CONST)
    {
      try {
        val = state.parse_num_arg(sval);
      } catch (symbol_exception &s) {
        //TODO : assert
        symbols.emplace_back(sval, state.get_pos()+(uint32_t)ret.size(),
                             colnum, pagenum, 0, ".ctrltext_" + std::to_string(colnum)
                             + "_" + std::to_string(pagenum),
                             symbol::patch_schema::scaler_32);
      }

      if (arg.m_width == width_8)
      {
        if (val == -1)
          val = 0;
        ret.push_back(val & BYTE_MASK);
      } else if (arg.m_width == width_16)
      {
        if (val == -1)
          val = 0;
        ret.push_back(val & BYTE_MASK);
        ret.push_back((val >> SECOND_BYTE_SHIFT) & BYTE_MASK);
      } else if (arg.m_width == width_32)
      {
        if (val == -1)
          val = 0;
        ret.push_back((val >> FIRST_BYTE_SHIFT)& BYTE_MASK);
        ret.push_back((val >> SECOND_BYTE_SHIFT) & BYTE_MASK);
        ret.push_back((val >> THIRD_BYTE_SHIFT) & BYTE_MASK);
        ret.push_back((val >> FORTH_BYTE_SHIFT) & BYTE_MASK);
      } else
        throw error(error::error_code::internal_error, "Unsupported arg width!!!");
    } else
      throw error(error::error_code::internal_error, "Invalid arg type!!!");
  }
  return ret;
}

std::vector<uint8_t>
ucDmaBd_op_serializer::
serialize(assembler_state& state,
          std::vector<symbol>& symbols,
          uint32_t colnum, pageid_type pagenum)
{
  //encode ucDmaBd
  std::vector<uint8_t> ret;
  uint32_t remote_ptr_high = state.parse_num_arg(m_args[0]);
  uint32_t remote_ptr_low  = state.parse_num_arg(m_args[1]);
  uint32_t local_ptr_absolute  = state.parse_num_arg(m_args[2]);
  uint32_t size  = state.parse_num_arg(m_args[3]);
  bool ctrl_external  = state.parse_num_arg(m_args[4]) != 0;
  bool ctrl_next_BD  = state.parse_num_arg(m_args[5]) != 0;
  bool ctrl_local_relative = true;

  // TODO assert
  uint32_t local_ptr = local_ptr_absolute - state.get_pos();

  //TODO assert
  ret.push_back(size & BYTE_MASK);
  ret.push_back((size >> SECOND_BYTE_SHIFT) & 0x7F);
  uint8_t val = 0;
  val = val | (ctrl_next_BD ? 0x1 : 0x0);
  val = val | (ctrl_external  ? 0x2 : 0x0);
  val = val | (ctrl_local_relative  ? 0x4 : 0x0);
  ret.push_back(val);
  ret.push_back(pad);

  ret.push_back((local_ptr >> FIRST_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((local_ptr >> SECOND_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((local_ptr >> THIRD_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((local_ptr >> FORTH_BYTE_SHIFT) & BYTE_MASK);

  ret.push_back((remote_ptr_low >> FIRST_BYTE_SHIFT)& BYTE_MASK);
  ret.push_back((remote_ptr_low >> SECOND_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((remote_ptr_low >> THIRD_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((remote_ptr_low >> FORTH_BYTE_SHIFT) & BYTE_MASK);

  ret.push_back((remote_ptr_high >> FIRST_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((remote_ptr_high >> SECOND_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((remote_ptr_high >> THIRD_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((remote_ptr_high >> FORTH_BYTE_SHIFT) & 0x1F);

  return ret;
}

std::vector<uint8_t>
ucDmaShimBd_op_serializer::
serialize(assembler_state& state,
          std::vector<symbol>& symbols,
          uint32_t colnum, pageid_type pagenum)
{
  //encode ucDmaShimBd
  std::vector<uint8_t> ret;
  uint32_t remote_ptr_high = state.parse_num_arg(m_args[0]);
  uint32_t remote_ptr_low  = state.parse_num_arg(m_args[1]);
  uint32_t local_ptr_absolute  = state.parse_num_arg(m_args[2]);
  uint32_t size  = state.parse_num_arg(m_args[3]);
  bool ctrl_external  = state.parse_num_arg(m_args[4]) != 0;
  bool ctrl_next_BD  = state.parse_num_arg(m_args[5]) != 0;
  bool ctrl_local_relative = true;
  
  // TODO assert
  uint32_t local_ptr = local_ptr_absolute - state.get_pos();
  //TODO ADD symbol
  symbols.emplace_back(symbol(m_args[6],local_ptr_absolute, colnum, pagenum, 0,
                              ".ctrldata_" + std::to_string(colnum) + "_"
                              + std::to_string(pagenum),
                              symbol::patch_schema::shim_dma_57  ));
  //TODO assert

  ret.push_back(size & BYTE_MASK);
  ret.push_back((size >> SECOND_BYTE_SHIFT) & 0x7F);
  uint8_t val = 0;
  val = val | (ctrl_next_BD ? 0x1 : 0x0);
  val = val | (ctrl_external  ? 0x2 : 0x0);
  val = val | (ctrl_local_relative  ? 0x4 : 0x0);
  ret.push_back(val);
  ret.push_back(pad);

  ret.push_back((local_ptr >> FIRST_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((local_ptr >> SECOND_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((local_ptr >> THIRD_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((local_ptr >> FORTH_BYTE_SHIFT) & BYTE_MASK);

  ret.push_back((remote_ptr_low >> FIRST_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((remote_ptr_low >> SECOND_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((remote_ptr_low >> THIRD_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((remote_ptr_low >> FORTH_BYTE_SHIFT) & BYTE_MASK);

  ret.push_back((remote_ptr_high >> FIRST_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((remote_ptr_high >> SECOND_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((remote_ptr_high >> THIRD_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((remote_ptr_high >> FORTH_BYTE_SHIFT) & 0x1F);

  return ret;
}

std::vector<uint8_t>
long_op_serializer::
serialize(assembler_state& state,
          std::vector<symbol>& symbols,
          uint32_t colnum,
          pageid_type pagenum)
{
  //encode long
  std::vector<uint8_t> ret;
  uint32_t val = state.parse_num_arg(m_args[0]);
  ret.push_back((val >> FIRST_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((val >> SECOND_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((val >> THIRD_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((val >> FORTH_BYTE_SHIFT) & BYTE_MASK);

  return ret;
}

std::vector<uint8_t>
align_op_serializer::
serialize(assembler_state& state,
          std::vector<symbol>& symbols,
          uint32_t colnum, pageid_type pagenum)
{
  //encode align
  std::vector<uint8_t> ret;
  for (uint32_t i=0;i < size(state); ++i)
    ret.push_back(m_opcode->get_code());

  return ret;
}

}
