// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include "ops.h"
#include "aiebu_error.h"
#include <string>
namespace aiebu {

offset_type
isa_op_serializer::size(assembler_state& /*state*/)
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
      jobid_type jobid = m_args[0];
      sval = std::to_string(state.m_jobmap[jobid]->get_size());
      atype = opArg::optype::CONST;
    } else if (arg.m_type == opArg::optype::PAGE_ID)
    {
      if (state.m_labelpageindex.find(m_args[arg_index].substr(1)) == state.m_labelpageindex.end())
        throw error(error::error_code::invalid_asm, "Label " + m_args[arg_index].substr(1) + "not present in label list\n");
      sval = std::to_string(state.m_labelpageindex[m_args[arg_index].substr(1)]);
      atype = opArg::optype::CONST;
      ++arg_index;
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
        symbols.emplace_back(sval, state.get_pos()+(uint32_t)ret.size(),
                             colnum, pagenum, 0, 0, ".ctrltext." + std::to_string(colnum)
                             + "." + std::to_string(pagenum),
                             symbol::patch_schema::scaler_32);
      }

      if (arg.m_width == width_8)
      {
        if (val == static_cast<uint32_t>(-1))
          val = 0;
        ret.push_back(val & BYTE_MASK);
      } else if (arg.m_width == width_16)
      {
        if (val == static_cast<uint32_t>(-1))
          val = 0;
        // For opcode is 'apply_offset_57' and arg is 'offset',
        // if val is 0xFFFF means we need to patch the host address of 1st page of controlcode
        // and we can patch in host and firmware, we send "control-code-X" as symbol name and 0xFFFF in apply_offset_57
        // if val == self.state.control_packet_index, we add "control-code-X" as symbol name and 0xFFFF in apply_offset_57
        // if val is not 0xFFFF or self.state.control_packet_index, we can do patching in cert or host so add symbol info in elf
        //    we send "arg index" as symbol name and arg offset in apply_offset_57
        if (!m_opcode->get_code_name().compare("apply_offset_57") && !arg.get_name().compare("offset"))
        {
          if (val == state.m_control_packet_index || val == 0xFFFF)
            sval = "control-code-" + std::to_string(colnum);
          symbols.emplace_back(sval, state.parse_num_arg(m_args[0]),
                               colnum, pagenum, 0, 0, ".ctrltext." + std::to_string(colnum)
                               + "." + std::to_string(pagenum),
                               symbol::patch_schema::shim_dma_57);

          if (val == state.m_control_packet_index && !arg.get_name().compare("offset") && m_args.size() == 4)
            state.m_controlpacket_padname = m_args[3];

          // arg 0 to 6 and be patched in CERT.
          // Beyond that its elfloader/host responsibility to patch mandatorily
          if (val > 6 && val != 0xFFFF)
            std::cout <<"WARNING: Apply_offset_57 has arg index " << val << " > 6, Should be mandatorily patched in host!!!\n";
          if (val == state.m_control_packet_index)
            val = 0xFFFF;
          else if (val != 0xFFFF)
          {
            // val is arg index, to get offset x2
            val = val * 2;
          }

          if (!arg.get_name().compare("offset") && m_args.size() == 4)
          {
            auto usymbo = m_args[3].substr(1);
            if (state.m_scratchpad.find(usymbo) != state.m_scratchpad.end())
            {
              state.m_patch[m_args[3]].emplace_back(m_args[0]);
            }
          }

        }

        ret.push_back(val & BYTE_MASK);
        ret.push_back((val >> SECOND_BYTE_SHIFT) & BYTE_MASK);
      } else if (arg.m_width == width_32)
      {
        if (val == static_cast<uint32_t>(-1))
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
          std::vector<symbol>& /*symbols*/,
          uint32_t /*colnum*/, pageid_type /*pagenum*/)
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
long_op_serializer::
serialize(assembler_state& state,
          std::vector<symbol>& /*symbols*/,
          uint32_t /*colnum*/,
          pageid_type /*pagenum*/)
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
          std::vector<symbol>& /*symbols*/,
          uint32_t /*colnum*/, pageid_type /*pagenum*/)
{
  //encode align
  std::vector<uint8_t> ret;
  for (uint32_t i=0;i < size(state); ++i)
    ret.push_back(m_opcode->get_code());

  return ret;
}

}
