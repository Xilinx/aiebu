// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/probe/probe_control.h"

namespace dtrace::probe
{

//-------------------------end_probe::end_probe-------------------------//
/**
 * end_probe() - Constructor with probe type and probe name.
 *
 * @param probe_type
 * @param probe_name
 */
end_probe::
end_probe(uint32_t probe_type, const std::string& probe_name)
    : probe(probe_type, probe_name)
{
}

//-------------------------end_probe::enable-------------------------//
/**
 * enable() - 
 *  Enables the end probe by adding probe header and actions to control and memory buffers.
 *
 * @param control_buffer
 * @param mem_buffer
 * @return
 *  Memory action locations for host address patching. 
 */
std::vector<uint32_t>
end_probe::
enable(std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer)
{
    // Filter the print actions from the control actions.
    filter_action();
    // If there are no control actions, return.
    std::vector<uint32_t> mem_action_locations;
    if (m_control_actions.empty())
        return mem_action_locations;

    // Get the end probe location and update the control buffer
    uint32_t location = probe_type::end;
    control_buffer[location] = 
        (control_buffer.size() << dtrace::dtrace_ctrl::second_byte_shift) | 
        (probe_type::end <<  dtrace::dtrace_ctrl::first_byte_shift);
    // Add the control actions to the control buffer and memory buffer for the end probe
    // and return the memory action locations for probe.
    mem_action_locations = actionize(control_buffer, mem_buffer);
    return mem_action_locations;
}

} // namespace dtrace::probe
