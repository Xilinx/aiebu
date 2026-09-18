// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/probe/probe_control.h"
#include <limits>
#include <sstream>
#include <stdexcept>

namespace dtrace::probe
{

//-------------------------jprobe::jprobe-------------------------//
/**
 * jprobe() - Constructor with probe type, probe name and maps.
 *
 * @param probe_type
 * @param probe_name
 * 
 * Constructor initializes the jprobe object with the given probe type, 
 * probe name, and maps. It also parses the probe name to extract probe fields 
 * and sets the uC value based on the parsed fields.
 */
jprobe::
jprobe(uint32_t probe_type, const std::string& probe_name, 
    uint32_t probe_page, uint32_t probe_offset)
    : probe(probe_type, probe_name)
    , m_page(probe_page)
    , m_offset(probe_offset)
{
    std::stringstream probe_stream(m_probe_name);
    std::string item;
    while (std::getline(probe_stream, item, ':'))
        m_probe_fields.push_back(item);
}

//-------------------------jprobe::enable-------------------------//
/**
 * enable() - 
 *  Enables the job probe by adding probe header and actions to control and memory buffers.
 *
 * @param control_buffers
 * @param mem_buffers
 * @param uC
 * @return
 *  Memory action locations for host address patching. 
 */
std::vector<uint32_t>
jprobe::
enable(std::unordered_map<uint32_t, std::vector<uint32_t>>& control_buffers,
    std::unordered_map<uint32_t, std::vector<uint32_t>>& mem_buffers, uint32_t uC)
{
    auto& control_buffer = control_buffers.at(uC);
    auto& mem_buffer = mem_buffers.at(uC);
    auto& jprobe_link = control_buffers.at(uC + probe_ctrl::jprobe_link_offset);
    // Filter the print actions from the control actions.
    filter_action();
    // If there are no control actions, return.
    std::vector<uint32_t> mem_action_locations;
    if (m_control_actions.empty())
        return mem_action_locations;

    // Error out if the control buffer size exceeds the 32-bit word limit.
    if (control_buffer.size() > std::numeric_limits<uint32_t>::max())
        DTRACE_ERROR("DTRACE_JPROBE_OFFSET_EXCEEDS_CONTROL_BLOCK_LIMIT",
            "Jprobe offset for " << m_probe_name << " exceeds the 32-bit control block limit ("
            << std::to_string(control_buffer.size()) << " words).");

    // Get the location for jprobe in the control buffer and update the control buffer.
    jprobe_link.push_back(static_cast<uint32_t>(control_buffer.size()));
    control_buffer.push_back(
        (probe_ctrl::link_end <<  dtrace::dtrace_ctrl::second_byte_shift) | 
        (probe_type::jprobe << dtrace::dtrace_ctrl::first_byte_shift)
    );

    // Get the location of control code page and offset and 
    // add it to the control buffer for jprobe.
    control_buffer.push_back((m_offset <<  dtrace::dtrace_ctrl::second_byte_shift) | m_page);
    control_buffer.push_back(dtrace::dtrace_ctrl::placeholder_value);
    // Add the control actions to the control buffer and memory buffer for jprobe
    // and return the memory action locations for probe.
    mem_action_locations = actionize(control_buffer, mem_buffer);
    return mem_action_locations;
}

} // namespace dtrace::probe
