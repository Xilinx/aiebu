// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include <sstream>
#include <stdexcept>

namespace dtrace::action
{

//-------------------------write_reg_action::write_reg_action-------------------------//
/**
 * write_reg_action() - Constructor with action token, probe type and probe name.
 * It parses the token and extracts the result, action name and arguments.
 *
 * @param token
 *  Write register action token: write_reg(addr, val)
 * @param probe_type
 * @param probe_name
 */
write_reg_action::
write_reg_action(std::string token, uint32_t probe_type, const std::string& probe_name)
    : action(probe_type, probe_name)
{
    std::vector<std::string> fields;
    std::stringstream token_stream(token);
    std::string item;
    while (std::getline(token_stream, item, '='))
        fields.push_back(strip(item));

    std::smatch action;
    if (!std::regex_match(fields[0], action, action_name::action_regex))
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'write_reg(addr, val)'");

    m_action_name = action[1];
    std::string argument_string = action[2]; 

    // Validate and parse the length argument
    std::stringstream argument_stream(argument_string);
    while (std::getline(argument_stream, item, ','))
        m_arguments.push_back(strip(item));

    if (m_arguments.size() < 2)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN_ARGUMENTS", 
            "Invalid arguments: '" << token << "' write_reg requires 1 arguments (addr, val)");
}

//-------------------------write_reg_action::actionize-------------------------//
/**
 * actionize() - Adds write register action values to the control buffer.
 *
 * @param last 
 *  Last action for the current probe.
 * @param control_buffer 
 * @param mem_buffer 
 */
void
write_reg_action::
actionize(uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>&)
{
    // control buffer
    // write register action header
    control_buffer.push_back(
        (last << dtrace::dtrace_ctrl::second_byte_shift) | action_type::reg_write
    );
    // write address
    control_buffer.push_back(std::stoul(m_arguments[0], nullptr, 16));
    // write value
    control_buffer.push_back(std::stoul(m_arguments[1], nullptr, 16));
}

//-------------------------write_reg_action::serialize-------------------------//
/**
 * serialize() - Serializes the register write action into a string format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 *
 * @return 
 *  String representing the serialized register write action.
 */
std::string
write_reg_action::
serialize(const std::vector<uint32_t>&, const std::vector<uint32_t>&, 
    const std::unordered_map<uint32_t, uint32_t>&) const
{
    std::ostringstream output_action;
    output_action << "  " << "#" << " " << m_action_name << "\n";
    return output_action.str();
}

} // namespace dtrace::action
