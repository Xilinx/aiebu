// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/probe/probe_control.h"
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
 * @param control_buffer
 * @param mem_buffer
 * @return
 *  Memory action locations for host address patching. 
 */
std::vector<uint32_t>
jprobe::
enable(std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer)
{
    // Filter the print actions from the control actions.
    filter_action();
    // If there are no control actions, return.
    std::vector<uint32_t> mem_action_locations;
    if (m_control_actions.empty())
        return mem_action_locations;

    // Get the location for jprobe in the control buffer and update the control buffer.
    uint32_t link = 
        control_buffer[probe_type::jprobe] >> dtrace::dtrace_ctrl::second_byte_shift;
    uint32_t location = probe_type::jprobe;
    while (link != probe_ctrl::link_end)
    {
        location = link;
        link = control_buffer[link] >>  dtrace::dtrace_ctrl::second_byte_shift;
    }
    control_buffer[location] = 
        (control_buffer.size() <<  dtrace::dtrace_ctrl::second_byte_shift) | 
        (probe_type::jprobe << dtrace::dtrace_ctrl::first_byte_shift);
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
