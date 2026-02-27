// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include <sstream>
#include <stdexcept>

namespace dtrace::action
{

//-------------------------host_timestamp_action::host_timestamp_action-------------------------//
/**
 * host_timestamp_action() - Constructor with action token, probe type and probe name.
 * It parses the token and extracts the result, action name and arguments.
 *
 * @param token
 *  Host Timestamp action token: val = host_timestamp()
 * @param probe_type
 * @param probe_name
 */
host_timestamp_action::
host_timestamp_action(std::string token, uint32_t probe_type, const std::string& probe_name)
    : action(probe_type, probe_name)
{
    std::vector<std::string> fields;
    std::stringstream token_stream(token);
    std::string item;
    while (std::getline(token_stream, item, '='))
        fields.push_back(strip(item));

    if (fields.size() != 2)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'val = host_timestamp()'");

    m_result = fields[0];

    aiebu::smatch action;
    if (!aiebu::regex_match(fields[1], action, action_name::action_regex))
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'host_timestamp()'");

    m_action_name = action[1];
}

//-------------------------host_timestamp_action::actionize-------------------------//
/**
 * actionize() - Adds timestamp action values to the control buffer.
 *
 * @param last 
 *  Last action for the current probe.
 * @param control_buffer 
 * @param mem_buffer 
 */
void
host_timestamp_action::
actionize(uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>&)
{
    // control_buffer 
    // timestamp header
    control_buffer.push_back(
        (last << dtrace::dtrace_ctrl::second_byte_shift) | action_type::host_timestamp
    );
    set_location(control_buffer, false);
    // timestamp high
    control_buffer.push_back(dtrace::dtrace_ctrl::result_value_init);
    // timestamp low
    control_buffer.push_back(dtrace::dtrace_ctrl::result_value_init);
}

//-------------------------host_timestamp_action::serialize-------------------------//
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
host_timestamp_action::
serialize(std::vector<uint32_t>& result_buffer, std::vector<uint32_t>&, 
    const std::unordered_map<uint32_t, uint32_t>& mapping) const
{
    std::ostringstream output_action;
    uint32_t location_h = mapping.at(get_location(false));
    uint32_t location_l = mapping.at(get_location(false) + 1);
    uint64_t high = 
        static_cast<uint64_t>(result_buffer[location_h]) << dtrace::dtrace_ctrl::forth_byte_shift;
    uint64_t low = result_buffer[location_l];
    output_action << "  " << m_result << " = " << (high + low) << "\n";
    // reset value after serialization
    result_buffer[location_h] = 0;
    result_buffer[location_l] = 0;
    return output_action.str();
}

} // namespace dtrace::action
