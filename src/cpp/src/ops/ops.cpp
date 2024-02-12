// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include "ops.h"
#include "aiebu_error.h"
namespace aiebu {

offset_type
isa_op_serializer::size(assembler_state& state)
{
  offset_type result = 2; // 2 bytes for opcode
  for (auto &arg : m_opcode->get_args())
    result += arg.m_width/8;
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
serialize(assembler_state& state, std::vector<symbol>& symbols, uint32_t colnum, pageid_type pagenum)
{
  //encode isa_op
  std::vector<uint8_t> ret;
  ret.push_back(m_opcode->get_code());
  ret.push_back(0x00);

  int arg_index = 0;
  opArg::optype atype;
  uint32_t val = 0;
  std::string sval;
  jobid_type jobid;
  for (auto arg : m_opcode->get_args())
  {
    if (arg.m_type == opArg::optype::PAD)
    {
      sval = "0";
      atype = opArg::optype::CONST;
    } else if (arg.m_type == opArg::optype::JOBSIZE)
    {
      jobid = state.parse_num_arg(m_args[0]);
      sval = std::to_string(state.m_jobmap[jobid]->get_size());
      atype = opArg::optype::CONST;
    } else
    {
      sval = m_args[arg_index];
      atype = arg.m_type;
      ++arg_index;
    }

    if (atype == opArg::optype::REG)
      ret.push_back(parse_register(sval) & 0xFF);
    else if (atype == opArg::optype::BARRIER)
      ret.push_back(parse_barrier(sval) & 0xFF);
    else if (atype == opArg::optype::CONST)
    {
      try {
        val = state.parse_num_arg(sval);
      } catch (symbol_exception s) {
        //TODO : assert
        symbols.emplace_back(symbol(sval, state.get_pos()+ret.size(), colnum, pagenum, 0, symbol::patch_schema::scaler_32  ));
      }

      if (arg.m_width == 8)
      {
        if (val == -1)
          val = 0;
        ret.push_back(val & 0xFF);
      } else if (arg.m_width == 16)
      {
        if (val == -1)
          val = 0;
        ret.push_back(val & 0xFF);
        ret.push_back((val >> 8) & 0xFF);
      } else if (arg.m_width == 32)
      {
        if (val == -1)
          val = 0;
        ret.push_back(val & 0xFF);
        ret.push_back((val >> 8) & 0xFF);
        ret.push_back((val >> 16) & 0xFF);
        ret.push_back((val >> 24) & 0xFF);
      } else
        throw error(error::error_code::internal_error, "Unsupported arg width!!!");
    } else
      throw error(error::error_code::internal_error, "Invalid arg type!!!");
  }
  return ret;
}

std::vector<uint8_t>
ucDmaBd_op_serializer::
serialize(assembler_state& state, std::vector<symbol>& symbols, uint32_t colnum, pageid_type pagenum)
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
  ret.push_back(size & 0xFF);
  ret.push_back((size >> 8) & 0x7F);
  uint8_t val = 0;
  val = val | (ctrl_next_BD ? 0x1 : 0x0);
  val = val | (ctrl_external  ? 0x2 : 0x0);
  val = val | (ctrl_local_relative  ? 0x4 : 0x0);
  ret.push_back(val);
  ret.push_back(0x00);

  ret.push_back(local_ptr & 0xFF);
  ret.push_back((local_ptr >> 8) & 0xFF);
  ret.push_back((local_ptr >> 16) & 0xFF);
  ret.push_back((local_ptr >> 24) & 0xFF);

  ret.push_back(remote_ptr_low & 0xFF);
  ret.push_back((remote_ptr_low >> 8) & 0xFF);
  ret.push_back((remote_ptr_low >> 16) & 0xFF);
  ret.push_back((remote_ptr_low >> 24) & 0xFF);

  ret.push_back(remote_ptr_high & 0xFF);
  ret.push_back((remote_ptr_high >> 8) & 0xFF);
  ret.push_back((remote_ptr_high >> 16) & 0xFF);
  ret.push_back((remote_ptr_high >> 24) & 0x1F);

  return ret;
}

std::vector<uint8_t>
ucDmaShimBd_op_serializer::
serialize(assembler_state& state, std::vector<symbol>& symbols, uint32_t colnum, pageid_type pagenum)
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
  symbols.emplace_back(symbol(m_args[6],local_ptr_absolute, colnum, pagenum, 0, symbol::patch_schema::shim_dma_57  ));
  //TODO assert

  ret.push_back(size & 0xFF);
  ret.push_back((size >> 8) & 0x7F);
  uint8_t val = 0;
  val = val | (ctrl_next_BD ? 0x1 : 0x0);
  val = val | (ctrl_external  ? 0x2 : 0x0);
  val = val | (ctrl_local_relative  ? 0x4 : 0x0);
  ret.push_back(val);
  ret.push_back(0x00);

  ret.push_back(local_ptr & 0xFF);
  ret.push_back((local_ptr >> 8) & 0xFF);
  ret.push_back((local_ptr >> 16) & 0xFF);
  ret.push_back((local_ptr >> 24) & 0xFF);

  ret.push_back(remote_ptr_low & 0xFF);
  ret.push_back((remote_ptr_low >> 8) & 0xFF);
  ret.push_back((remote_ptr_low >> 16) & 0xFF);
  ret.push_back((remote_ptr_low >> 24) & 0xFF);

  ret.push_back(remote_ptr_high & 0xFF);
  ret.push_back((remote_ptr_high >> 8) & 0xFF);
  ret.push_back((remote_ptr_high >> 16) & 0xFF);
  ret.push_back((remote_ptr_high >> 24) & 0x1F);

  return ret;
}

std::vector<uint8_t>
long_op_serializer::
serialize(assembler_state& state, std::vector<symbol>& symbols, uint32_t colnum, pageid_type pagenum)
{
  //encode long
  std::vector<uint8_t> ret;
  uint32_t val = state.parse_num_arg(m_args[0]);
  ret.push_back(val & 0xFF);
  ret.push_back((val >> 8) & 0xFF);
  ret.push_back((val >> 16) & 0xFF);
  ret.push_back((val >> 24) & 0xFF);

  return ret;
}

std::vector<uint8_t>
align_op_serializer::
serialize(assembler_state& state, std::vector<symbol>& symbols, uint32_t colnum, pageid_type pagenum)
{
  //encode align
  std::vector<uint8_t> ret;
  for (uint32_t i=0;i < size(state); ++i)
    ret.push_back(m_opcode->get_code());

  return ret;
}

}
