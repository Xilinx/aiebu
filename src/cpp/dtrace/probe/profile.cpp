// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/probe/probe_control.h"
#include <sstream>

namespace dtrace::probe
{

//-------------------------profile_probe::profile_probe-------------------------//
/**
 * profile_probe() - Constructor with probe type and probe name.
 *
 * @param probe_type
 * @param probe_name
 * 
 * Constructor initializes the profile object with the given probe type and
 * probe name. It also parses the probe name to extract probe fields and sets
 * the uC and frequency value based on the parsed fields.
 */
profile_probe::
profile_probe(uint32_t probe_type, const std::string& probe_name)
    : probe(probe_type, probe_name)
{
    std::vector<std::string> probe_fields;
    std::stringstream probe_stream(m_probe_name);
    std::string item;
    while (std::getline(probe_stream, item, ':'))
        probe_fields.push_back(item);
    
    m_uC = std::stoi(probe_fields[1].substr(2));
    m_frequency = std::stoi(probe_fields[2].substr(0, probe_fields[2].size() - 2));
}

//-------------------------profile_probe::enable-------------------------//
/**
 * enable() - 
 *  Enables the profile probe by adding probe header to control and memory buffers.
 *
 * @param control_buffer
 * @param mem_buffer
 * @return
 *  Memory action locations for host address patching. 
 */
std::vector<uint32_t>
profile_probe::
enable(std::vector<uint32_t>& control_buffer, std::vector<uint32_t>&)
{
    // Clear the control buffer and resize it to TRACE_PAGE_SIZE
    // profiling and other type of probes are not enabled concurrently 
    control_buffer.clear();
    control_buffer.resize(dtrace::dtrace_ctrl::trace_page_size >> 2, 0);
    // Add profile probe type and frequency to control buffer.
    control_buffer[0] = 
        (probe_ctrl::link_end << dtrace::dtrace_ctrl::second_byte_shift) | 
        (probe_type::profile << dtrace::dtrace_ctrl::first_byte_shift);
    control_buffer[1] = m_frequency;

    // No action for profile probe, return empty memory action locations.
    std::vector<uint32_t> mem_action_locations;
    return mem_action_locations;  
}

} // namespace dtrace::probe
