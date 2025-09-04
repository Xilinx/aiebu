// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

#include "ops.h"
#include "aiebu/aiebu_error.h"

#include <string>
#include <iomanip>
#include <cassert>

namespace aiebu {

static constexpr uint32_t offset_type_marker = 0xFFFF;

offset_type
isa_op_serializer::size(assembler_state& state)
{
  if (state.is_optimization_enabled_for_op(m_opcode->get_code_name())) return 0;
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
serialize(std::shared_ptr<assembler_state> state, std::vector<symbol>& symbols,
          uint32_t colnum, pageid_type pagenum)
{
  //encode isa_op
  std::vector<uint8_t> ret;
  // Patching is already done host side, so skip apply_offset_57 opcode
  // as part of text section, to avoid overhead on FW for processing apply_offset_57
  if (!state->is_optimization_enabled_for_op(m_opcode->get_code_name())) {
    ret.push_back(m_opcode->get_code());
    ret.push_back(pad);
  }

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
      sval = std::to_string(state->m_jobmap[jobid]->get_size());
      atype = opArg::optype::CONST;
    } else if (arg.m_type == opArg::optype::PAGE_ID)
    {
      if (state->m_labelpageindex.find(m_args[arg_index].substr(1)) == state->m_labelpageindex.end())
        throw error(error::error_code::invalid_asm, "Label " + m_args[arg_index].substr(1) + "not present in label list\n");
      sval = std::to_string(state->m_labelpageindex[m_args[arg_index].substr(1)]);
      atype = opArg::optype::CONST;
      ++arg_index;
    } else
    {
      sval = m_args[arg_index];
      atype = arg.m_type;
      ++arg_index;
    }

    if (atype == opArg::optype::REG && (!state->is_optimization_enabled_for_op(m_opcode->get_code_name())))
      ret.push_back(parse_register(sval) & BYTE_MASK);
    else if (atype == opArg::optype::BARRIER && (!state->is_optimization_enabled_for_op(m_opcode->get_code_name())))
      ret.push_back(parse_barrier(sval) & BYTE_MASK);
    else if (atype == opArg::optype::CONST)
    {
      try {
        val = state->parse_num_arg(sval);
      } catch (symbol_exception &s) {
        symbols.emplace_back(sval, state->get_pos()+(uint32_t)ret.size(),
                             colnum, pagenum, 0, 0, ".ctrltext." + std::to_string(colnum)
                             + "." + std::to_string(pagenum),
                             symbol::patch_schema::scaler_32);
      }

      if (arg.m_width == width_8 && (!state->is_optimization_enabled_for_op(m_opcode->get_code_name())))
      {
        ret.push_back(val & BYTE_MASK);
      } else if (arg.m_width == width_16)
      {
        // For opcode is 'apply_offset_57' and arg is 'offset',
        // if val is 0xFFFF means we need to patch the host address of 1st page of controlcode
        // and we can patch in host and firmware, we send "control-code-X" as symbol name and 0xFFFF in apply_offset_57
        // if val == self.state->control_packet_index, we add "control-code-X" as symbol name and 0xFFFF in apply_offset_57
        // if val is not 0xFFFF or self.state->control_packet_index, we can do patching in cert or host so add symbol info in elf
        //    we send "arg index" as symbol name and arg offset in apply_offset_57
        if (!m_opcode->get_code_name().compare("apply_offset_57") && !arg.get_name().compare("offset"))
        {
          if (val == state->m_control_packet_index || val == offset_type_marker)       // NOLINT
            sval = "control-code-" + std::to_string(colnum);

          size_t index = state->find_label_entry(m_args[0].substr(1));
          auto num_entries = state->parse_num_arg(m_args[1]);
          for (uint32_t numbd = 0; numbd < num_entries; ++numbd)
          {
            auto label = state->get_label_at(index);
            symbols.emplace_back(sval, state->parse_num_arg(label),
                                 colnum, pagenum, 0, 0, ".ctrltext." + std::to_string(colnum)
                                 + "." + std::to_string(pagenum),
                                 state->get_shim_dma_patching());
            ++index;
          }
          if (val == state->m_control_packet_index && !arg.get_name().compare("offset") && m_args.size() == 4)
            state->m_controlpacket_padname = m_args[3];

          // arg 0 to 6 and be patched in CERT.
          // Beyond that its elfloader/host responsibility to patch mandatorily
          if (val > 6 && val != offset_type_marker)
            std::cout <<"WARNING: Apply_offset_57 has arg index " << val << " > 6, Should be mandatorily patched in host!!!\n";
          if (val == state->m_control_packet_index)
            val = offset_type_marker;
          else if (val != offset_type_marker)
          {
            // val is arg index, to get offset x2
            val = val * 2;
          }

          index = state->find_label_entry(m_args[0].substr(1));
          if (!arg.get_name().compare("offset") && m_args.size() == 4)
          {
            auto usymbo = m_args[3].substr(1);
            if (state->m_scratchpad.find(usymbo) != state->m_scratchpad.end())
            {
              auto num_entries = state->parse_num_arg(m_args[1]);
              for (uint32_t numbd = 0; numbd < num_entries; ++numbd)
              {
                auto label = state->get_label_at(index);
                state->m_patch[m_args[3]].emplace_back(label);
                ++index;
              }
            }
          }

        }
        if (!state->is_optimization_enabled_for_op(m_opcode->get_code_name())){
          ret.push_back(val & BYTE_MASK);
          ret.push_back((val >> SECOND_BYTE_SHIFT) & BYTE_MASK);
        }
      } else if (arg.m_width == width_32 && (!state->is_optimization_enabled_for_op(m_opcode->get_code_name())))
      {
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
serialize(std::shared_ptr<assembler_state> state,
          std::vector<symbol>& /*symbols*/,
          uint32_t /*colnum*/, pageid_type /*pagenum*/)
{
  //encode ucDmaBd
  std::vector<uint8_t> ret;
  uint32_t remote_ptr_high = state->parse_num_arg(m_args[0]);
  uint32_t remote_ptr_low  = state->parse_num_arg(m_args[1]);
  uint32_t local_ptr_absolute  = state->parse_num_arg(m_args[2]);
  uint32_t size  = state->parse_num_arg(m_args[3]);
  bool ctrl_external  = state->parse_num_arg(m_args[4]) != 0;
  bool ctrl_next_BD  = state->parse_num_arg(m_args[5]) != 0;        //NOLINT
  bool ctrl_local_relative = true;

  // TODO assert
  uint32_t local_ptr = local_ptr_absolute - state->get_pos();

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
serialize(std::shared_ptr<assembler_state> state,
          std::vector<symbol>& /*symbols*/,
          uint32_t /*colnum*/,
          pageid_type /*pagenum*/)
{
  //encode long
  std::vector<uint8_t> ret;
  uint32_t val = state->parse_num_arg(m_args[0]);
  ret.push_back((val >> FIRST_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((val >> SECOND_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((val >> THIRD_BYTE_SHIFT) & BYTE_MASK);
  ret.push_back((val >> FORTH_BYTE_SHIFT) & BYTE_MASK);

  return ret;
}

std::vector<uint8_t>
align_op_serializer::
serialize(std::shared_ptr<assembler_state> state,
          std::vector<symbol>& /*symbols*/,
          uint32_t /*colnum*/, pageid_type /*pagenum*/)
{
  //encode align
  std::vector<uint8_t> ret;
  for (uint32_t i=0;i < size(*state); ++i)
    ret.push_back(m_opcode->get_code());

  return ret;
}

// Dedicated functions for handling different argument types in CONST case
std::string
isa_op_deserializer::
handle_tile_id_arg(uint32_t val, uint32_t& tile,
                   std::shared_ptr<disassembler_state> state)
{
  tile = val;
  return state->to_tile(val);
}

std::string
isa_op_deserializer::
handle_actor_id_arg(uint32_t val, uint32_t tile,
                    std::shared_ptr<disassembler_state> state)
{
  return state->to_actor(val, tile);
}

std::string
isa_op_deserializer::
handle_descriptor_ptr_arg(uint32_t val,
                          std::shared_ptr<disassembler_state> state)
{
  auto slabels = state->get_labels();
  if (slabels.find(val) == slabels.end()) {
    std::string label = get_label();
    state->add_label(val, label);
    return label;
  } else {
    return slabels.at(val);
  }
}

std::string
isa_op_deserializer::
handle_table_ptr_arg(uint32_t val,
                      std::shared_ptr<disassembler_state> state)
{
  auto slocal_ptrs = state->get_local_ptrs();
  if (slocal_ptrs.find(val) == slocal_ptrs.end()) {
    std::string label = get_label();
    state->add_local_ptr(val, label, shim_bd_len);
    return label;
  } else {
    return slocal_ptrs.at(val).first;
  }
}

std::string
isa_op_deserializer::
handle_generic_const_arg(const opArg& arg, uint32_t val)
{
  // Handle offset argument: if not 0xFFFF, divide by 2 to get the argument value.
  if (is_offset(arg) && val != offset_type_marker) {
    val = val / 2;
  }

  // Format as hexadecimal string
  std::ostringstream oss;
  oss << "0x" << std::uppercase << std::hex << val;
  return oss.str();
}

std::string
isa_op_deserializer::
handle_register_arg(uint32_t val)
{
  constexpr uint32_t local_register_count = 8;
  constexpr uint32_t global_register_count = 16;
  constexpr uint32_t total_register_count = local_register_count + global_register_count;

  if (val >= total_register_count) {
    throw std::runtime_error("Register number out of range: " + std::to_string(val));
  }

  // Local registers (r0-r7) vs Global registers (g0-g15)
  if (val < local_register_count) {
    return "$r" + std::to_string(val); // r0 to r7
  } else {
    return "$g" + std::to_string(val - local_register_count); // g0 to g15
  }
}

std::string
isa_op_deserializer::
handle_barrier_arg(uint32_t val)
{
  // Handle different barrier types based on opcode
  if (m_opcode->get_code_name() == "local_barrier") {
    return "$lb" + std::to_string(val);
  } else if (m_opcode->get_code_name() == "remote_barrier") {
    return "$rb" + std::to_string(val - 1);
  } else {
    throw std::runtime_error("Invalid barrier arg for " + m_opcode->get_code_name());
  }
}

std::string
isa_op_deserializer::
handle_page_id_arg(uint32_t val,
                    std::shared_ptr<disassembler_state> state)
{
  std::string label = get_label();
  state->add_external_label(val, label);
  return label;
}

uint32_t op_deserializer::numlabel = 0;

uint32_t
isa_op_deserializer::
deserialize(asm_writer& writer, std::shared_ptr<disassembler_state> state, const char* data)
{
    std::vector<std::string> result;
    uint32_t size = 2;//1 byte opcode and 1 byte pad
    uint32_t tile = 0;

    for (const auto& arg : m_opcode->get_args()) {
        uint32_t len = arg.get_width() / byte_to_bits;  // convert bits to byte
        uint32_t val = get_arg_val(data + size, len);
        size += len;

        switch (arg.get_type()) {
            case opArg::optype::CONST:
                // Handle different CONST argument types based on their names
                if (is_tile_id(arg)) {
                    result.push_back(handle_tile_id_arg(val, tile, state));
                } else if (is_actor_id(arg)) {
                    result.push_back(handle_actor_id_arg(val, tile, state));
                } else if (is_descriptor_ptr(arg)) {
                    result.push_back(handle_descriptor_ptr_arg(val, state));
                } else if (is_table_ptr(arg)) {
                    result.push_back(handle_table_ptr_arg(val, state));
                } else {
                    // Generic constant argument handling (includes offset and others)
                    result.push_back(handle_generic_const_arg(arg, val));
                }
                break;

            case opArg::optype::JOBSIZE:
                // TODO: Check if jobsize is valid
                break;

            case opArg::optype::REG:
                result.push_back(handle_register_arg(val));
                break;

            case opArg::optype::BARRIER:
                result.push_back(handle_barrier_arg(val));
                break;

            case opArg::optype::PAD:
                // No action required for padding
                break;

            case opArg::optype::PAGE_ID:
                result.push_back(handle_page_id_arg(val, state));
                break;

            default:
                throw std::runtime_error("Invalid argument type!");
        }
    }

    state->increment_address(size);
    writer.write_operation(m_opcode->get_code_name(), result, "");
    return size;
}

offset_type
isa_op_deserializer::
size(disassembler_state& /*state*/)
{
  uint32_t total_width = 2; // 1 opcode + 1 pad
  for (const auto& arg : m_opcode->get_args()) {
    total_width += (arg.get_width() / byte_to_bits);
  }
  return total_width;
}

uint32_t
align_op_deserializer::
deserialize(asm_writer& writer, std::shared_ptr<disassembler_state> state, const char* /*data*/)
{
  state->increment_address(1);
  writer.write_operation(m_opcode->get_code_name(), {}, "");
  return 1;
}
  
uint32_t
ucDmaBd_op_deserializer::
deserialize(asm_writer& writer, std::shared_ptr<disassembler_state> state, const char* data)
{
  assert(state->get_address() % align() == 0 && "uC DMA definition has to be 128-bit aligned!");
  std::string label = state->get_labels().at(state->get_address());
  writer.write_label(label);
  int ctrl_next_BD = 1;
  uint32_t count = 0;
  constexpr uint32_t size_offset = 0;
  constexpr uint32_t flags_offset = 2;
  constexpr uint32_t local_ptr_offset = 4;
  constexpr uint32_t remote_addr_low_offset = 8;
  constexpr uint32_t remote_addr_high_offset = 12;

  while (ctrl_next_BD == 1) {
    int ctrl_external = 0;
    //int ctrl_local_relative = 0;

    std::vector<std::string> result;
    std::vector<uint32_t> arg;

    arg.push_back(read_uint16(data + count + size_offset));        // size field
    arg.push_back(read_uint16(data + count + flags_offset));       // flags
    arg.push_back(read_uint32(data + count + local_ptr_offset));   // local_ptr_offset
    arg.push_back(read_uint32(data + count + remote_addr_low_offset));  // remote address low
    arg.push_back(read_uint32(data + count + remote_addr_high_offset)); // remote address high

    // Format address fields
    {
      std::ostringstream a3, a4;
      a4 << "0x" << std::uppercase << std::hex << std::setw(field_width) << std::setfill('0') << arg[4];
      a3 << "0x" << std::uppercase << std::hex << std::setw(field_width) << std::setfill('0') << arg[3];
      result.push_back(a4.str());
      result.push_back(a3.str());
    }

    uint32_t local_ptr_offset = arg[2] + state->get_address();
    auto slocal_ptr = state->get_local_ptrs();
    if (slocal_ptr.find(local_ptr_offset) == slocal_ptr.end()) {
      std::string ptr_label = get_label();
      result.push_back(ptr_label);
      state->add_local_ptr(local_ptr_offset, ptr_label, arg[0]);
    } else {
      result.push_back(slocal_ptr[local_ptr_offset].first);
    }

    result.push_back(std::to_string(arg[0]));

    if ((arg[1] & 0x1) == 0)
      ctrl_next_BD = 0;

    if ((arg[1] & 0x2) != 0)
      ctrl_external = 1;

    //if ((arg[1] & 0x4) != 0)
    //  ctrl_local_relative = 1;

    result.push_back(std::to_string(ctrl_external));
    result.push_back(std::to_string(ctrl_next_BD));

    state->increment_address(size(*state));
    count += size(*state);
    writer.write_operation("UC_DMA_BD", result, label);
  }

  return count;
}

uint32_t
long_op_deserializer::
deserialize(asm_writer& writer, std::shared_ptr<disassembler_state> state, const char* data)
{
  uint32_t lp = state->get_address();
  auto label_pair = state->get_local_ptrs().at(lp);
  std::string label = label_pair.first;
  writer.write_label(label);
  uint32_t count = label_pair.second;
  assert(count != 0 && ".long length is zero");
  uint32_t pos = 0;
  for (uint32_t i = 0; i < count; ++i) {
    std::vector<std::string> result;

    uint32_t val = read_uint32(data + pos);
    std::ostringstream oss;
    oss << "0x" << std::uppercase << std::hex
        << std::setw(field_width) << std::setfill('0') << val;
    result.push_back(oss.str());

    state->increment_address(size(*state));
    pos += size(*state);
    writer.write_operation(".long", result, label);
  }
  return pos;
}

} // namespace aiebu
