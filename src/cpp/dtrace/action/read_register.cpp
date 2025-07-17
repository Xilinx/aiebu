// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include <sstream>
#include <stdexcept>

namespace dtrace::action
{

//-------------------------read_reg_action::read_reg_action-------------------------//
/**
 * read_reg_action() - Constructor with action token, probe type and probe name.
 * It parses the token and extracts the result, action name and arguments.
 *
 * @param token
 *  Read register action token: val = read_reg(addr)
 * @param probe_type
 * @param probe_name
 */
read_reg_action::
read_reg_action(std::string token, uint32_t probe_type, const std::string& probe_name)
    : action(probe_type, probe_name)
{
    std::vector<std::string> fields;
    std::stringstream token_stream(token);
    std::string item;
    while (std::getline(token_stream, item, '='))
        fields.push_back(strip(item));

    if (fields.size() != 2)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'val = read_reg(addr)'");

    m_result = fields[0];

    std::smatch action;
    if (!std::regex_match(fields[1], action, action_name::action_regex))
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'read_reg(addr)'");

    m_action_name = action[1];
    std::string argument_string = action[2]; 

    // Validate and parse the length argument
    std::stringstream argument_stream(argument_string);
    while (std::getline(argument_stream, item, ','))
        m_arguments.push_back(strip(item));

    if (m_arguments.size() < 1)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN_ARGUMENTS", 
            "Invalid arguments: '" << token << "' read_reg requires 1 arguments (addr)");
}

//-------------------------read_reg_action::actionize-------------------------//
/**
 * actionize() - Adds read register action values to the control buffer.
 *
 * @param last 
 *  Last action for the current probe.
 * @param control_buffer 
 * @param mem_buffer 
 */
void
read_reg_action::
actionize(uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>&)
{
    // control buffer
    // read register action header
    control_buffer.push_back(
        (last << dtrace::dtrace_ctrl::second_byte_shift) | action_type::reg_read
    );
    // read address
    control_buffer.push_back(std::stoul(m_arguments[0], nullptr, 16));
    set_location(control_buffer, false);
    // return value
    control_buffer.push_back(0);
}

//-------------------------read_reg_action::serialize-------------------------//
/**
 * serialize() - Serializes the register read action into a string format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 *
 * @return 
 *  String representing the serialized register read action.
 */
std::string
read_reg_action::
serialize(const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>&, 
    const std::unordered_map<uint32_t, uint32_t>& mapping) const
{
    std::ostringstream output_action;
    output_action << "  " << m_result << " = " << result_buffer[mapping.at(get_location(false))] << "\n";
    return output_action.str();
}

} // namespace dtrace::action
