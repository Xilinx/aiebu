// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025, Advanced Micro Devices, Inc. All rights reserved.

#include "aie2ps_preprocessor_input.h"

#include <cstdint>

namespace aiebu {

  //TODO: this these functions are custom copy from aie2_preprocessor_input.cpp, todo we mode both in common place
  void
  asm_preprocessor_input::
  validate_json(uint64_t offset, uint64_t size, uint32_t arg_index, offset_type type) const {
    std::string errorMessage;
    if (type == offset_type::CONTROL_PACKET && offset < m_control_packet_offset_correction) {
      errorMessage = std::string("INVALID JSON: Offset(")
      + std::to_string(offset)
      + ") is less than control packet offset correction("
      + std::to_string(m_control_packet_offset_correction)
      + ") for offset Type: CONTROL PACKET and arg index is "
      + (arg_index > MAX_ARG_INDEX ? "INVALID = " : "VALID = ")
      + std::to_string(arg_index) + ". ";
      throw error(error::error_code::invalid_asm, errorMessage);
    }
    if ((offset <= size) && (arg_index <= MAX_ARG_INDEX))
      return;
    if (offset > size) {
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
  }

  void
  asm_preprocessor_input::
  extract_coalesed_buffers(const std::string& name,
                           const boost::property_tree::ptree& pt)
  {
    const uint64_t buffer_size = get_64_bit_property(pt, "size_in_bytes");
    const auto coalesed_buffers_pt = pt.get_child_optional("coalesed_buffers");
    if (!coalesed_buffers_pt)
      return;

    const auto coalesed_buffers = coalesed_buffers_pt.get();
    for (auto coalesed_buffer : coalesed_buffers) {
      const uint64_t buffer_offset = get_64_bit_property(coalesed_buffer.second, "offset_in_bytes");
      uint32_t arg_index = get_32_bit_property(pt, "xrt_id");
      // Check if the buffer offset is within the buffer size
      validate_json(buffer_offset, buffer_size, arg_index, offset_type::COALESED_BUFFER);
      // extract control packet patch
      extract_control_packet_patch(name, arg_index, coalesed_buffer.second);
    }
  }

  void
  asm_preprocessor_input::
  patch_ctrl57_addend(std::vector<char>& buf, uint32_t offset, uint64_t addend) const
  {
    auto* bd = reinterpret_cast<uint32_t*>(buf.data() + offset);
    const uint64_t bd1 = bd[2]; // NOLINT
    const uint64_t bd2 = bd[3]; // NOLINT
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
    uint64_t arg = ((bd2 & 0xFFFF) << 32) + (bd1 & 0xFFFFFFFF);
    arg += addend;
    bd[2] = static_cast<uint32_t>(arg & 0xFFFFFFFF);
    bd[3] = static_cast<uint32_t>(((arg >> 32) & 0xFFFF) | (bd2 & 0xFFFF0000));
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
  }

  void
  asm_preprocessor_input::
  patch_ctrlpkt_addend(std::vector<char>& buf, uint32_t offset, uint64_t addend) const
  {
    patch_ctrl57_addend(buf, offset, addend);
  }

  void
  asm_preprocessor_input::
  extract_control_packet_patch(const std::string& name,
                               const uint32_t arg_index,
                               const boost::property_tree::ptree& pt)
  {
    const uint64_t addend_64 = get_64_bit_property(pt, "offset_in_bytes", true);
    const bool large_addend = addend_64 > RANGE_32BIT;
    const uint32_t addend = large_addend ? 0 : static_cast<uint32_t>(addend_64);
    const auto control_packet_patch_pt = pt.get_child_optional("control_packet_patch_locations");
    if (!control_packet_patch_pt)
      return;
    const auto patchs = control_packet_patch_pt.get();

    for (auto pat : patchs)
    {
      auto patch = pat.second;
      auto ctrlpkt_id = get_32_bit_property(patch, "xrt_id");
      auto ctrpkt_buf = m_ctrlpkt_id_map.at(ctrlpkt_id);
      auto control_packet_size = static_cast<uint32_t>(m_controlpkt[ctrpkt_buf].size());
      uint32_t control_packet_offset = get_32_bit_property(patch, "offset");
      // Check if the control packet offset is within the control packet size
      validate_json(control_packet_offset, control_packet_size, arg_index, offset_type::CONTROL_PACKET);
      // move 8 bytes(header) up for unifying the patching scheme between DPU sequence and transaction-buffer
      uint32_t offset = control_packet_offset - m_control_packet_offset_correction;
      if (large_addend)
      {
        log_info() << boost::format(
          "Warning: offset_in_bytes (0x%x) > 32bit for %s, baking into BD and patching with addend=0\n")
          % addend_64 % control_packet_patch_schema_name();
        patch_ctrlpkt_addend(m_controlpkt[ctrpkt_buf], offset, addend_64);
      }
      add_symbol({name, offset, 0, 0, addend, 0, ctrpkt_buf, control_packet_patching});
    }
  }

  void
  asm_preprocessor_input::
  aiecompiler_json_parser(const boost::property_tree::ptree& pt)
  {
    const auto pt_external_buffers = pt.get_child_optional("external_buffers");
    if (!pt_external_buffers)
      return;

    const auto external_buffers = pt_external_buffers.get();
    for (auto& external_buffer : external_buffers)
    {
      // added ARG_OFFSET to argidx to match with kernel argument index in xclbin
      auto arg = get_32_bit_property(external_buffer.second, "xrt_id");
      if (external_buffer.second.get<bool>("ctrl_pkt_buffer", false)) {
        m_control_packet_index = arg;
        std::string path = external_buffer.second.get<std::string>("path", "default_path");
        std::string ctrl_pkt_name = ".ctrlpkt-" + std::to_string(m_control_packet_index);
        std::vector<char> ctrl_pkt_code;
        ctrl_pkt_code = m_artifacts->get(path, m_libpaths);
        m_controlpkt[ctrl_pkt_name] = ctrl_pkt_code;
        m_ctrlpkt_id_map.insert({arg, ctrl_pkt_name});
      }
    }

    for (auto& external_buffer : external_buffers)
    {
      const auto pt_coalesed_buffers = external_buffer.second.get_child_optional("coalesed_buffers");
      // added ARG_OFFSET to argidx to match with kernel argument index in xclbin
      auto arg = get_32_bit_property(external_buffer.second, "xrt_id");
      std::string name = std::to_string(arg + ARG_OFFSET);
      if (external_buffer.second.get<bool>("ctrl_pkt_buffer", false)) 
        continue;

      if (pt_coalesed_buffers)
        extract_coalesed_buffers(name, external_buffer.second);
      else
        extract_control_packet_patch(name, arg, external_buffer.second);
    }
  }

  void
  asm_preprocessor_input::
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
      uint32_t control_packet_size = static_cast<uint32_t>(m_data[".ctrldata"].size());
      uint32_t arg_index = get_32_bit_property(patch, "xrt_arg_id");
      // check if the offset is less than the size of the control packet
      validate_json(control_packet_offset, control_packet_size, arg_index, offset_type::CONTROL_PACKET);
      uint32_t offset = control_packet_offset - m_control_packet_offset_correction;
      const uint64_t addend_64 = get_64_bit_property(patch, "bo_offset", true);
      const bool large_addend = addend_64 > RANGE_32BIT;
      const uint32_t addend = large_addend ? 0 : static_cast<uint32_t>(addend_64);
      if (large_addend)
      {
        log_info() << boost::format(
          "Warning: bo_offset (0x%x) > 32bit for %s, baking into BD and patching with addend=0\n")
          % addend_64 % control_packet_patch_schema_name();
        if (m_data.find(".ctrldata") != m_data.end())
          patch_ctrlpkt_addend(m_data[".ctrldata"], offset, addend_64);
      }

      // TODO added symbols name hardcoded to ".pad.0" and col 0
      // this will change once compiler decide on how to generate multi col control packet design
      add_symbol({std::to_string(arg_index + ARG_OFFSET), offset, 0, 0, addend, 0, ".ctrltext.0.0", control_packet_patching});
    }

  }

  void
  asm_preprocessor_input::
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

}
