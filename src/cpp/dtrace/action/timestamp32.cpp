// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include <sstream>
#include <stdexcept>

namespace dtrace::action
{

//-------------------------timestamp32_action::timestamp32_action-------------------------//
/**
 * timestamp32_action() - Constructor with action token, probe type and probe name.
 * It parses the token and extracts the result, action name and arguments.
 *
 * @param token
 *  Timestamp 32-bit action token: val = timestamp32()
 * @param probe_type
 * @param probe_name
 */
timestamp32_action::
timestamp32_action(std::string token, uint32_t probe_type, const std::string& probe_name)
    : action(probe_type, probe_name)
{
    std::vector<std::string> fields;
    std::stringstream token_stream(token);
    std::string item;
    while (std::getline(token_stream, item, '='))
        fields.push_back(strip(item));

    if (fields.size() != 2)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'val = timestamp32()'");
    
    m_result = fields[0];

    boost::smatch action;
    if (!boost::regex_match(fields[1], action, action_name::action_regex))
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'timestamp32()'");

    m_action_name = action[1];
}

//-------------------------timestamp32_action::actionize-------------------------//
/**
 * actionize() - Adds timestamp 32-bit action values to the control buffer.
 *
 * @param last 
 *  Last action for the current probe.
 * @param control_buffer 
 * @param mem_buffer 
 */
void
timestamp32_action::
actionize(uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>&)
{
    // control buffer
    // timestamp 32-bit action header
    control_buffer.push_back(
        (last << dtrace::dtrace_ctrl::second_byte_shift) | action_type::timestamp32
    );
    set_location(control_buffer, false);
    // timestamp value
    control_buffer.push_back(0);
}

//-------------------------timestamp32_action::serialize-------------------------//
/**
 * serialize() - Serializes the timestamp 32-bit action into a string format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 *
 * @return 
 *  String representing the serialized timestamp 32-bit action.
 */
std::string
timestamp32_action::
serialize(const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>&, 
    const std::unordered_map<uint32_t, uint32_t>& mapping) const
{
    std::ostringstream output_action;
    output_action << "  " << m_result << " = " << result_buffer[mapping.at(get_location(false))] << "\n";
    return output_action.str();
}

} // namespace dtrace::action
