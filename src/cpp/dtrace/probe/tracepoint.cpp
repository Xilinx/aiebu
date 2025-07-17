// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/probe/probe_control.h"
#include <sstream>

namespace dtrace::probe
{

//-------------------------tracepoint_probe::tracepoint_probe-------------------------//
/**
 * tracepoint_probe() - Constructor with probe type and probe name.
 *
 * @param probe_type
 * @param probe_name
 * 
 * Constructor initializes the tracepoint object with the given probe type and
 * probe name. It also parses the probe name to extract probe fields and sets
 * the uC and tracepoint index value based on the parsed fields.
 */
tracepoint_probe::
tracepoint_probe(uint32_t probe_type, const std::string& probe_name)
    : probe(probe_type, probe_name)
{
    std::vector<std::string> probe_fields;
    std::stringstream probe_stream(m_probe_name);
    std::string item;
    while (std::getline(probe_stream, item, ':'))
        probe_fields.push_back(item);

    m_uC = std::stoi(probe_fields[1].substr(2));
    m_index = std::stoi(probe_fields[2].substr(2));
}

//-------------------------tracepoint_probe::enable-------------------------//
/**
 * enable() - 
 *  Enables the tracepoint probe by adding probe header and actions to control and memory buffers.
 *
 * @param control_buffer
 * @param mem_buffer
 * @return
 *  Memory action locations for host address patching. 
 */
std::vector<uint32_t>
tracepoint_probe::
enable(std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer)
{
    // Filter the print actions from the control actions.
    filter_action();
    // If there are no control actions, return.
    std::vector<uint32_t> mem_action_locations;
    if (m_control_actions.empty())
        return mem_action_locations;

    // Get the location for tracepoint probe in the control buffer and 
    // update the control buffer with the tracepoint probe.
    uint32_t link = 
        control_buffer[probe_type::tracepoint] >> dtrace::dtrace_ctrl::second_byte_shift;
    uint32_t location = probe_type::tracepoint;
    while (link != probe_ctrl::link_end)
    {
        location = link;
        link = control_buffer[link] >> dtrace::dtrace_ctrl::second_byte_shift;
    }
    control_buffer[location] = 
        (control_buffer.size() << dtrace::dtrace_ctrl::second_byte_shift) | 
        (probe_type::tracepoint << dtrace::dtrace_ctrl::first_byte_shift);
    control_buffer.push_back(
        (probe_ctrl::link_end << dtrace::dtrace_ctrl::second_byte_shift) | 
        (probe_type::tracepoint << dtrace::dtrace_ctrl::first_byte_shift)
    );
    control_buffer.push_back(m_index << dtrace::dtrace_ctrl::second_byte_shift);
    // Add the control actions to the control buffer and memory buffer for the tracepoint probe
    // and return the memory action locations for probe.
    mem_action_locations = actionize(control_buffer, mem_buffer);
    return mem_action_locations;
}

} // namespace dtrace::probe
