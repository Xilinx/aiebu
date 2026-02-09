// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include <sstream>

namespace dtrace::action
{

//-------------------------break_action::break_action-------------------------//
/**
 * break_action() - Constructor with action token, probe type and probe name.
 *
 * @param token
 *  Break action token: break()
 * @param probe_type
 * @param probe_name
 */
break_action::
break_action(std::string token, uint32_t probe_type, const std::string& probe_name)
    : action(probe_type, probe_name)
    , m_token(std::move(token))
{
}

//-------------------------break_action::actionize-------------------------//
/**
 * actionize() - Adds break action value to the control buffer.
 *
 * @param last 
 *  Last action for the current probe.
 * @param control_buffer 
 * @param mem_buffer 
 */
void
break_action::
actionize(uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>&)
{
    // control buffer
    // break action header
    control_buffer.push_back(
        (last << dtrace::dtrace_ctrl::second_byte_shift) | action_type::break_action
    );
}

//-------------------------break_action::serialize-------------------------//
/**
 * serialize() - Serializes the break action into a string format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 *
 * @return 
 *  String representing the serialized break action.
 */
std::string
break_action::
serialize(std::vector<uint32_t>&, std::vector<uint32_t>&, 
    const std::unordered_map<uint32_t, uint32_t>&) const
{
    std::ostringstream output_action;
    output_action << "#" << " " << m_token << "\n";
    return output_action.str();
}

} // namespace dtrace::action
