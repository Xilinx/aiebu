// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/probe/probe_control.h"
#include <limits>

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
 * @param control_buffers
 * @param mem_buffers
 * @param uC
 * @return
 *  Memory action locations for host address patching. 
 */
std::vector<uint32_t>
end_probe::
enable(std::unordered_map<uint32_t, std::vector<uint32_t>>& control_buffers,
    std::unordered_map<uint32_t, std::vector<uint32_t>>& mem_buffers, uint32_t uC)
{
    auto& control_buffer = control_buffers.at(uC);
    auto& mem_buffer = mem_buffers.at(uC);
    auto& end_link = control_buffers.at(uC + probe_ctrl::end_link_offset);
    // Filter the print actions from the control actions.
    filter_action();
    // If there are no control actions, return.
    std::vector<uint32_t> mem_action_locations;
    if (m_control_actions.empty())
        return mem_action_locations;

    // Error out if the control buffer size exceeds the 32-bit word limit.
    if (control_buffer.size() > std::numeric_limits<uint32_t>::max())
        DTRACE_ERROR("DTRACE_END_OFFSET_EXCEEDS_CONTROL_BLOCK_LIMIT",
            "End offset for " << m_probe_name << " exceeds the 32-bit control block limit ("
            << std::to_string(control_buffer.size()) << " words).");

    // Get the end probe location and store the location.
    end_link.push_back(static_cast<uint32_t>(control_buffer.size()));

    // Add the control actions to the control buffer and memory buffer for the end probe
    // and return the memory action locations for probe.
    mem_action_locations = actionize(control_buffer, mem_buffer);
    return mem_action_locations;
}

} // namespace dtrace::probe
