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
 * @param mem_host_addr
 *  Memory buffer host address for read memory action in control buffer.
 */
write_mem_action::
write_mem_action(std::string token, uint32_t probe_type, const std::string& probe_name, 
    uint64_t mem_host_addr, const std::unordered_map<std::string, std::vector<uint32_t>>& buffer_map)
    : action(probe_type, probe_name)
    , m_mem_host_addr(mem_host_addr)
{
    std::vector<std::string> fields;
    std::stringstream token_stream(token);
    std::string item;
    while (std::getline(token_stream, item, '='))
        fields.push_back(strip(item));

    boost::smatch action;
    if (!boost::regex_match(fields[0], action, action_name::action_regex))
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
        m_write_buffer_values = buffer_map.at(write_buffer_name);
    else
        DTRACE_ERROR("DTRACE_ACTION_WRITE_BUFFER_NOT_FOUND", 
            "Write buffer name not found: " << write_buffer_name);
}

//-------------------------write_mem_action::get_mem_host_addr-------------------------//
/**
 * get_mem_host_addr() - Retrieves the memory host address after adjusting for the memory length.
 *
 * @return 
 *  Updated memory host address.
 *
 * This function calculates the memory length by converting the second argument (action length)
 * in `m_arguments` into word bytes. It then adds this memory length to the base memory host address (`m_mem_host_addr`) 
 * and returns the result.
 */
uint64_t
write_mem_action::
get_mem_host_addr() const
{
    uint64_t mem_length = static_cast<uint64_t>(m_length * dtrace::dtrace_ctrl::word_byte_size);
    return m_mem_host_addr + mem_length;
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
    control_buffer.push_back(std::stoul(m_arguments[0], nullptr, 16));
    // length
    control_buffer.push_back(m_length);
    // mem_host_addr high
    control_buffer.push_back(
        (m_mem_host_addr >> dtrace::dtrace_ctrl::forth_byte_shift) & dtrace::dtrace_ctrl::mask_32
    );
    // mem_host_addr low
    control_buffer.push_back(m_mem_host_addr & dtrace::dtrace_ctrl::mask_32);

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
serialize(const std::vector<uint32_t>&, const std::vector<uint32_t>&, 
    const std::unordered_map<uint32_t, uint32_t>&) const
{
    std::ostringstream output_action;
    output_action << "  " << "#" << " " << m_action_name << "\n";
    return output_action.str();
}

} // namespace dtrace::action
