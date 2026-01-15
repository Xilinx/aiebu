// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include <sstream>
#include <stdexcept>

namespace dtrace::action
{

//-------------------------sleep_action::sleep_action-------------------------//
/**
 * sleep_action() - Constructor with action token, probe type and probe name.
 * It parses the token and extracts the action name and arguments.
 *
 * @param token
 *  Sleep action token: sleep(target)
 * @param probe_type
 * @param probe_name
 */
sleep_action::
sleep_action(std::string token, uint32_t probe_type, const std::string& probe_name)
    : action(probe_type, probe_name)
{
    std::vector<std::string> fields;
    std::stringstream token_stream(token);
    std::string item;
    while (std::getline(token_stream, item, '='))
        fields.push_back(strip(item));

    aiebu::smatch action;
    if (!aiebu::regex_match(fields[0], action, action_name::action_regex))
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'sleep(target)'");

    m_action_name = action[1];
    std::string argument_string = action[2]; 

    // Validate and parse the length argument
    std::stringstream argument_stream(argument_string);
    while (std::getline(argument_stream, item, ','))
        m_arguments.push_back(strip(item));

    if (m_arguments.size() < 1)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN_ARGUMENTS", 
            "Invalid arguments: '" << token << "' sleep requires 1 arguments (target)");
}

//-------------------------sleep_action::actionize-------------------------//
/**
 * actionize() - Adds Sleep action values to the control buffer.
 *
 * @param last 
 *  Last action for the current probe.
 * @param control_buffer 
 * @param mem_buffer 
 */
void
sleep_action::
actionize(uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>&)
{
    // control buffer
    // sleep action header
    control_buffer.push_back(
        (last << dtrace::dtrace_ctrl::second_byte_shift) | action_type::sleep
    );
    // target sleep cycles
    control_buffer.push_back(std::stoul(m_arguments[0], nullptr, dtrace::dtrace_ctrl::decimal_hexadecimal_base));
}

//-------------------------sleep_action::serialize-------------------------//
/**
 * serialize() - Serializes the sleep action into a string format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 *
 * @return 
 *  String representing the serialized sleep action.
 */
std::string
sleep_action::
serialize(const std::vector<uint32_t>&, const std::vector<uint32_t>&, 
    const std::unordered_map<uint32_t, uint32_t>&) const
{
    std::ostringstream output_action;
    output_action << "  " << "#" << " " << m_action_name << " " << m_arguments[0] << "cycles" << "\n";
    return output_action.str();
}

} // namespace dtrace::action
