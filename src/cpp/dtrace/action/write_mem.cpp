// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include <sstream>
#include <stdexcept>

namespace dtrace::action
{

//-------------------------write_mem_action::write_mem_action-------------------------//
/**
 * write_mem_action() - Constructor with action token, probe type and probe name.
 * It parses the token and extracts the result, action name and arguments.
 *
 * @param token
 *  Write memory action token: write_mem(addr, length, buffer)
 * @param probe_type
 * @param probe_name
 */
write_mem_action::
write_mem_action(std::string token, uint32_t probe_type, const std::string& probe_name, 
    const std::unordered_map<std::string, std::pair<std::vector<uint32_t>, std::vector<uint32_t>>>& buffer_map)
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
            "Invalid token: '" << token << "' Expected 'write_mem(addr, length, buffer)'");

    m_action_name = action[1];
    std::string argument_string = action[2]; 

    std::stringstream argument_stream(argument_string);
    while (std::getline(argument_stream, item, ','))
        m_arguments.push_back(strip(item));

    // Validate and parse the length argument
    if (m_arguments.size() < 3)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN_ARGUMENTS", 
            "Invalid arguments: '" << token << "' write_mem requires 3 arguments (addr, length, buffer)");

    // Check to parse a length string as hexadecimal or decimal
    m_length = std::stoull(m_arguments[1], nullptr, 0);
    std::string write_buffer_name = m_arguments[2];

    // check if write buffer name exists in the map and get the values
    if (buffer_map.find(write_buffer_name) != buffer_map.end())
    {
        m_write_buffer_addr = buffer_map.at(write_buffer_name).first;
        m_write_buffer_values = buffer_map.at(write_buffer_name).second;
    }
    else
        DTRACE_ERROR("DTRACE_ACTION_WRITE_BUFFER_NOT_FOUND", 
            "Write buffer name not found: " << write_buffer_name);
}

//-------------------------write_mem_action::actionize-------------------------//
/**
 * actionize() - Adds write register memory action values to the control buffer.
 *
 * @param last 
 *  Last action for the current probe.
 * @param control_buffer 
 * @param mem_buffer 
 */
void
write_mem_action::
actionize(uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer)
{
    // control buffer
    control_buffer.push_back(
        (last << dtrace::dtrace_ctrl::second_byte_shift) | action_type::mem_write
    );
    set_location(control_buffer, false);
    // aie_addr
    control_buffer.push_back(std::stoul(m_arguments[0], nullptr, dtrace::dtrace_ctrl::hexadecimal_base));
    // length
    control_buffer.push_back(m_length);
    // mem_host_addr high
    control_buffer.push_back(m_write_buffer_addr[0]);
    // mem_host_addr low
    control_buffer.push_back(m_write_buffer_addr[1]);

    // mem buffer
    // values
    mem_buffer.insert(
        mem_buffer.end(), m_write_buffer_values.begin(), m_write_buffer_values.end()
    );
}

//-------------------------write_mem_action::serialize-------------------------//
/**
 * serialize() - Serializes the write memory register action into a string format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 *
 * @return 
 *  String representing the serialized write memory register action.
 */
std::string
write_mem_action::
serialize(std::vector<uint32_t>&, std::vector<uint32_t>&, 
    const std::unordered_map<uint32_t, uint32_t>&) const
{
    std::ostringstream output_action;
    output_action << "  " << "#" << " " << m_action_name << "\n";
    return output_action.str();
}

} // namespace dtrace::action
