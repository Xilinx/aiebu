// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

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
    std::stringstream token_stream(m_token);
    std::string item;
    while (std::getline(token_stream, item, '='))
        fields.push_back(strip(item));

    if (fields.size() != 2)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN_FORMAT", 
            "Invalid token: '" << m_token << "' Expected 'val = opcode()'");

    m_result = fields[0];

    boost::smatch action;
    if (!boost::regex_match(fields[1], action, action_name::action_regex))
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << m_token << "' Expected 'opcode()'");

    m_action_name = action[1];
    std::string argument_string = action[2]; 

    std::stringstream argument_stream(argument_string);
    while (std::getline(argument_stream, item, ','))
        m_arguments.push_back(strip(item));

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
 *
 * @return 
 *  String representing the serialized profile action.
 */
std::string
profile_action::
serialize(const std::vector<uint32_t>&, const std::vector<uint32_t>&, 
    const std::unordered_map<uint32_t, uint32_t>&) const
{
    std::ostringstream output_action;
    output_action << "#" << " " << m_token << "\n";
    return output_action.str();
}

} // namespace dtrace::action
