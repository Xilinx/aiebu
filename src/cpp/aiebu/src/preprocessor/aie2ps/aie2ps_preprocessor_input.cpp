// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include "aie2ps_preprocessor_input.h"

namespace aiebu {

  //TODO: this these functions are custom copy from aie2_preprocessor_input.cpp, todo we mode both in common place
  void
  aie2ps_preprocessor_input::
  validate_json(uint32_t /*offset*/, uint32_t /*size*/, uint32_t /*arg_index*/, offset_type /*type*/) const {
    // Return if the offset and arg_index are within their respective sizes.
    // TODO enable checks
    /*
    if ((offset <= size) && (arg_index <= MAX_ARG_INDEX)) {
      return;
    }
    std::string errorMessage;
    if (offset > size ) {
      errorMessage = std::string("INVALID JSON: Offset(")
      + std::to_string(offset)
      + ") is greater than size("
      + std::to_string(size)
      + ") for offset Type: "
      + (type == offset_type::CONTROL_PACKET ? "CONTROL PACKET" : "BUFFER")
      + " and arg index is "
      + (arg_index > MAX_ARG_INDEX ? "INVALID = " : "VALID = ")
      + std::to_string(arg_index) + ". ";
    }
    else {
      errorMessage = std::string("INVALID JSON: arg index (")
      + std::to_string(arg_index)
      + ") is greater than Max arg index ="
      + std::to_string(MAX_ARG_INDEX)
      + ". ";
    }
    throw error(error::error_code::invalid_asm, errorMessage);
    */
  }

  void
  aie2ps_preprocessor_input::
  extract_coalesed_buffers(const std::string& name,
                           const boost::property_tree::ptree& pt)
  {
    uint32_t buffer_size = get_32_bit_property(pt, "size_in_bytes");
    const auto coalesed_buffers_pt = pt.get_child_optional("coalesed_buffers");
    if (!coalesed_buffers_pt)
      return;

    const auto coalesed_buffers = coalesed_buffers_pt.get();
    for (auto coalesed_buffer : coalesed_buffers) {
      uint32_t buffer_offset = get_32_bit_property(coalesed_buffer.second, "offset_in_bytes");
      uint32_t arg_index = get_32_bit_property(pt, "xrt_id");
      // Check if the buffer offset is within the buffer size
      validate_json(buffer_offset, buffer_size, arg_index, offset_type::COALESED_BUFFER);
      // extract control packet patch
      extract_control_packet_patch(name, arg_index, coalesed_buffer.second);
    }
  }

  void
  aie2ps_preprocessor_input::
  extract_control_packet_patch(const std::string& name,
                               const uint32_t arg_index,
                               const boost::property_tree::ptree& pt)
  {
    const uint32_t addend = get_32_bit_property(pt, "offset_in_bytes", true);
    const auto control_packet_patch_pt = pt.get_child_optional("control_packet_patch_locations");
    if (!control_packet_patch_pt)
      return;
    const auto patchs = control_packet_patch_pt.get();
    for (auto pat : patchs)
    {
      auto patch = pat.second;
      uint32_t control_packet_size = m_data[".ctrldata"].size();
      uint32_t control_packet_offset = get_32_bit_property(patch, "offset");
      // Check if the control packet offset is within the control packet size
      validate_json(control_packet_offset, control_packet_size, arg_index, offset_type::CONTROL_PACKET);
      // move 8 bytes(header) up for unifying the patching scheme between DPU sequence and transaction-buffer
      uint32_t offset = control_packet_offset - 8;

      // TODO added symbols name hardcoded to ".pad.0" and col 0
      // this will change once compiler decide on how to generate multi col control packet design
      add_symbol({name, offset, 0, 0, addend, 0, ".pad.0", symbol::patch_schema::control_packet_48});
    }
  }

  void
  aie2ps_preprocessor_input::
  aiecompiler_json_parser(const boost::property_tree::ptree& pt)
  {
    const auto pt_external_buffers = pt.get_child_optional("external_buffers");
    if (!pt_external_buffers)
      return;

    const auto external_buffers = pt_external_buffers.get();
    for (auto& external_buffer : external_buffers)
    {
      const auto pt_coalesed_buffers = external_buffer.second.get_child_optional("coalesed_buffers");
      // added ARG_OFFSET to argidx to match with kernel argument index in xclbin
      auto arg = get_32_bit_property(external_buffer.second, "xrt_id");
      std::string name = std::to_string(arg + ARG_OFFSET);
      if (external_buffer.second.get<bool>("ctrl_pkt_buffer", false))
        m_control_packet_index = arg;

      if (pt_coalesed_buffers)
        extract_coalesed_buffers(name, external_buffer.second);
      else
        extract_control_packet_patch(name, arg, external_buffer.second);
    }
  }

  void
  aie2ps_preprocessor_input::
  dmacompiler_json_parser(const boost::property_tree::ptree& pt)
  {
    const auto pt_ctrl_xrt_arg_idx = pt.get_optional<uint32_t>("ctrl_pkt_xrt_arg_idx");
    if (pt_ctrl_xrt_arg_idx)
    {
      // if "ctrl_pkt_xrt_arg_idx" present make that as controlpacket index
      m_control_packet_index = pt_ctrl_xrt_arg_idx.get();
    } else {
      // if "ctrl_pkt_xrt_arg_idx" not present default arg4 is controlpacket
      m_control_packet_index = 4;
    }

    const auto pt_ctrl_pkt_patch_info = pt.get_child_optional("ctrl_pkt_patch_info");
    if (!pt_ctrl_pkt_patch_info)
      return;

    const auto patchs = pt_ctrl_pkt_patch_info.get();
    for (auto pat : patchs)
    {
      auto patch = pat.second;
      uint32_t control_packet_offset = get_32_bit_property(patch, "offset");
      uint32_t control_packet_size = m_data[".ctrldata"].size();
      uint32_t arg_index = get_32_bit_property(patch, "xrt_arg_id");
      // check if the offset is less than the size of the control packet
      validate_json(control_packet_offset, control_packet_size, arg_index, offset_type::CONTROL_PACKET);
      // move 8 bytes(header) up for unifying the patching scheme between DPU sequence and transaction-buffer
      uint32_t offset = control_packet_offset - 8;
      const uint32_t addend = get_32_bit_property(patch, "bo_offset");

      // TODO added symbols name hardcoded to ".pad.0" and col 0
      // this will change once compiler decide on how to generate multi col control packet design
      add_symbol({std::to_string(arg_index + ARG_OFFSET), offset, 0, 0, addend, 0, ".ctrltext.0.0", symbol::patch_schema::control_packet_57});
    }

  }

  void
  aie2ps_preprocessor_input::
  readmetajson(std::istream& patch_json)
  {
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(patch_json, pt);

    const auto aiecompiler_json = pt.get_child_optional("external_buffers");
    if (aiecompiler_json)
    {
      aiecompiler_json_parser(pt);
      return;
    }

    const auto dmacompiler_json = pt.get_child_optional("ctrl_pkt_patch_info");
    if (dmacompiler_json)
    {
      dmacompiler_json_parser(pt);
      return;
    }
  }


  uint32_t
  aie2ps_preprocessor_input::
  get_32_bit_property(const boost::property_tree::ptree& pt, const std::string& property, bool defaultvalue) const
  {
    uint64_t value = 0;
    if (defaultvalue)
      value = pt.get<uint64_t>(property, 0);
    else
      value = pt.get<uint64_t>(property);
    // we dont support property greater then 32 bit
    if (value > RANGE_32BIT)
    {
      auto error_msg = boost::format("Invalid %s (0x%x) > 32bit found") % property % value;
      throw error(error::error_code::invalid_asm, error_msg.str());
    }
    return static_cast<uint32_t>(value);
  }

}
