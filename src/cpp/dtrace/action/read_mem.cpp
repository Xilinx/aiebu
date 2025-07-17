// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include <sstream>
#include <stdexcept>

namespace dtrace::action
{

//-------------------------read_mem_action::read_mem_action-------------------------//
/**
 * read_mem_action() - Constructor with action token, probe type and probe name.
 * It parses the token and extracts the result, action name and arguments.
 *
 * @param token
 *  Read memory action token: buf = read_mem(addr, length)
 * @param probe_type
 * @param probe_name
 * @param mem_host_addr
 *  Memory buffer host address for read memory action in control buffer.
 */
read_mem_action::
read_mem_action(std::string token, uint32_t probe_type, const std::string& probe_name, 
    uint64_t mem_host_addr)
    : action(probe_type, probe_name)
    , m_mem_host_addr(mem_host_addr)
{
    std::vector<std::string> fields;
    std::stringstream token_stream(token);
    std::string item;
    while (std::getline(token_stream, item, '='))
        fields.push_back(strip(item));

    if (fields.size() != 2)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'buf = read_mem(addr, length)'");

    m_result = fields[0];

    std::smatch action;
    if (!std::regex_match(fields[1], action, action_name::action_regex))
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'read_mem(addr, length)'");

    m_action_name = action[1];
    std::string argument_string = action[2]; 

    // Validate and parse the length argument
    std::stringstream argument_stream(argument_string);
    while (std::getline(argument_stream, item, ','))
        m_arguments.push_back(strip(item));

    if (m_arguments.size() < 2)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN_ARGUMENTS", 
            "Invalid arguments: '" << token << "' read_mem requires 2 arguments (addr, length)");

    // Parse the length string as hexadecimal or decimal
    m_length = std::stoull(m_arguments[1], nullptr, 0);
}

//-------------------------read_mem_action::get_mem_host_addr-------------------------//
/**
 * get_mem_host_addr() - 
 *  Retrieves the memory host address after adjusting for the memory length.
 *
 * @return 
 *  Updated memory host address.
 *
 * This function calculates the memory length by converting the second argument 
 * (action length) in `m_arguments` into word bytes. It then adds this memory length to the 
 * base memory host address (`m_mem_host_addr`) and returns the result.
 */
uint64_t
read_mem_action::
get_mem_host_addr() const
{
    uint64_t mem_length = static_cast<uint64_t>(m_length * dtrace::dtrace_ctrl::word_byte_size);
    return m_mem_host_addr + mem_length;
}

//-------------------------read_mem_action::actionize-------------------------//
/**
 * actionize() - Adds read register memory action values to the control buffer.
 *
 * @param last 
 *  Last action for the current probe.
 * @param control_buffer 
 * @param mem_buffer 
 */
void
read_mem_action::
actionize(uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer)
{
    // control buffer
    control_buffer.push_back(
        (last << dtrace::dtrace_ctrl::second_byte_shift) | action_type::mem_read
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
    set_location(mem_buffer, true);
    // values
    mem_buffer.insert(mem_buffer.end(), m_length, 0);
}

//-------------------------read_mem_action::serialize-------------------------//
/**
 * serialize() - Serializes the read memory register action into a string format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 *
 * @return 
 *  String representing the serialized read memory register action.
 */
std::string
read_mem_action::
serialize(const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>& mem_buffer, 
    const std::unordered_map<uint32_t, uint32_t>& mapping) const
{
    std::ostringstream readable_result;
    uint32_t index = get_location(true);
    uint32_t aie_addr = result_buffer[mapping.at(get_location(false))];
    uint32_t length = result_buffer[mapping.at(get_location(false) + 1)];
    for (uint32_t i = index; i < index+length; ++i)
    {
        readable_result << "  0x" << std::hex << aie_addr
            << ": 0x" << std::hex << mem_buffer[i];
        if (i < index+length - 1)
            readable_result << "\n";
        aie_addr += dtrace::dtrace_ctrl::word_byte_size;
    }
    std::ostringstream output_action;
    output_action << "  " << m_result << " = \"\"\"\n" << readable_result.str() << "\"\"\"\n";
    return output_action.str();
}

} // namespace dtrace::action
