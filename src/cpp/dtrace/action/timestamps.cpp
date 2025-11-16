// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include <sstream>
#include <stdexcept>

namespace dtrace::action
{

//-------------------------timestamps_action::timestamps_action-------------------------//
/**
 * timestamps_action() - Constructor with action token, probe type and probe name.
 * It parses the token and extracts the result, action name and arguments.
 *
 * @param token
 *  Multiple Timestamp action token: name[length] = timestamps()
 * @param probe_type
 * @param probe_name
 */
timestamps_action::
timestamps_action(std::string token, uint32_t probe_type, const std::string& probe_name)
    : action(probe_type, probe_name)
{
    std::vector<std::string> fields;
    std::stringstream token_stream(token);
    std::string item;
    while (std::getline(token_stream, item, '='))
        fields.push_back(strip(item));

    if (fields.size() != 2)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'name[length] = timestamps()'");

    // Regex pattern to match '<name>[<length>]' format
    boost::regex buffer_regex(R"(^(.+)\[(.+)\]$)");
    boost::smatch buffer;
    if (!boost::regex_match(fields[0], buffer, buffer_regex))
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'name[length]'");

    m_result = buffer[1];
    std::string length = buffer[2];

    boost::smatch action;
    if (!boost::regex_match(fields[1], action, action_name::action_regex))
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'timestamps()'");

    m_action_name = action[1];
    m_length = std::stoull(length, nullptr, 0);
}

//-------------------------timestamps_action::actionize-------------------------//
/**
 * actionize() - Adds timestamp action values to the control buffer.
 *
 * @param last 
 *  Last action for the current probe.
 * @param control_buffer 
 * @param mem_buffer 
 */
void
timestamps_action::
actionize(uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>&)
{
    // control_buffer 
    // timestamp header
    control_buffer.push_back(
        (last << dtrace::dtrace_ctrl::second_byte_shift) | action_type::timestamps
    );
    set_location(control_buffer, false);
    // timestamp length
    control_buffer.push_back(m_length);
    // timestamp values for m_length
    for (size_t i = 0; i < m_length; ++i)
    {   // timestamp high values
        control_buffer.push_back(0);
        // timestamp low values
        control_buffer.push_back(0);
    }
}

//-------------------------timestamps_action::serialize-------------------------//
/**
 * serialize() - Serializes the timestamp action into a string format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 *
 * @return 
 *  String representing the serialized timestamp action.
 */
std::string
timestamps_action::
serialize(const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>&, 
    const std::unordered_map<uint32_t, uint32_t>& mapping) const
{
    
    std::vector<uint64_t> result;
    for (uint32_t i = 0; i < m_length; ++i) {
        // action location + length word + high and low value
        uint32_t location = mapping.at(get_location(false)) + 1 + i*2;
        uint64_t high = static_cast<uint64_t>(result_buffer[location]) << dtrace::dtrace_ctrl::forth_byte_shift;
        uint64_t low = result_buffer[location + 1];
        result.push_back(high + low);
    }

    std::ostringstream output_action;
    output_action << "  " << m_result << " = [";
    for (size_t i = 0; i < result.size(); ++i) {
        output_action << result[i];
        if (i != result.size() - 1)
            output_action << ", ";
    }
    output_action << "]\n";
    
    return output_action.str();
}

} // namespace dtrace::action
