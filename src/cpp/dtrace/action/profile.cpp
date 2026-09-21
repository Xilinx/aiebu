// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include <sstream>
#include <stdexcept>

namespace dtrace::action
{

//-------------------------profile_action::profile_action-------------------------//
/**
 * profile_action() - Constructor with action token, probe type and probe name.
 * It parses the token and extracts the result, action name and arguments.
 *
 * @param token
 *  Profile action token: val = profile(addr)
 * @param probe_type
 * @param probe_name
 */
profile_action::
profile_action(std::string token, uint32_t probe_type, const std::string& probe_name)
    : action(probe_type, probe_name)
    , m_token(std::move(token))
{
    std::vector<std::string> fields;
    action::getline(m_token, '=', fields);

    if (fields.size() != 2)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN_FORMAT", 
            "Invalid token: '" << m_token << "' Expected 'val = opcode()'");

    // Validate and parse the action name
    std::string argument_string;
    if (!action::match(fields[1], m_action_name, argument_string))
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << m_token << "' Expected 'opcode()'");

    // Validate and parse the arguments
    action::getline(argument_string, ',', m_arguments);
}

//-------------------------profile_action::actionize-------------------------//
/**
 * actionize() - Profile action does not require any action to be performed.
 *
 * @param last 
 *  Last action for the current probe.
 * @param control_buffer 
 * @param mem_buffer 
 */
void
profile_action::
actionize(uint32_t, std::vector<uint32_t>&, std::vector<uint32_t>&)
{
}

//-------------------------profile_action::serialize-------------------------//
/**
 * serialize() - Serializes the profile action into a string format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 * @param script_output
 */
void
profile_action::
serialize(uint32_t*, uint32_t*,
    const std::unordered_map<uint32_t, uint32_t>&, std::ostream&) const
{
}

//-------------------------profile_action::serialize-------------------------//
/**
 * serialize() - Serializes the profile action into json format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 * @param json_output
 */
void
profile_action::
serialize(uint32_t*, uint32_t*,
    const std::unordered_map<uint32_t, uint32_t>&, json&) const
{
}

} // namespace dtrace::action
