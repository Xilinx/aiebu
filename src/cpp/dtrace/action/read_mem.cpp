// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include <iomanip>
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
read_mem_action(std::string token, uint32_t probe_type, const std::string& probe_name, uint64_t mem_host_addr,
    const std::unordered_map<std::string, std::pair<std::vector<uint32_t>, std::vector<uint32_t>>>& buffer_map)
    : action(probe_type, probe_name)
    , m_mem_host_addr(mem_host_addr)
    , m_read_buffer_initialized(false)
{
    std::vector<std::string> fields;
    std::stringstream token_stream(token);
    std::string item;
    while (std::getline(token_stream, item, '='))
        fields.push_back(action::strip(item));

    if (fields.size() != 2)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'buf = read_mem(addr, length)'");

    m_result = fields[0];

    aiebu::smatch action;
    if (!aiebu::regex_match(fields[1], action, action_name::action_regex))
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'read_mem(addr, length)'");

    m_action_name = action[1];
    std::string argument_string = action[2]; 

    // Validate and parse the length argument
    std::stringstream argument_stream(argument_string);
    while (std::getline(argument_stream, item, ','))
        m_arguments.push_back(action::strip(item));

    if (m_arguments.size() < 2)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN_ARGUMENTS", 
            "Invalid arguments: '" << token << "' read_mem requires 2 arguments (addr, length)");

    // Parse the length string as hexadecimal or decimal
    m_length = static_cast<uint32_t>(std::stoull(m_arguments[1], nullptr, 0));

    // check if write buffer name exists in the map and get the values
    if (buffer_map.find(m_result) != buffer_map.end())
    {
        m_read_buffer_initialized = true;
        m_read_buffer_addr = buffer_map.at(m_result).first;
    }
    else
    {
        m_read_buffer_addr.push_back(
            (m_mem_host_addr >> dtrace::dtrace_ctrl::forth_byte_shift) & dtrace::dtrace_ctrl::mask_32
        );
        m_read_buffer_addr.push_back(
            m_mem_host_addr & dtrace::dtrace_ctrl::mask_32
        );
    }
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
 * If read buffer is already initialized, return base memory host address without adding memory length.
 */
uint64_t
read_mem_action::
get_mem_host_addr() const
{
    auto mem_length = static_cast<uint64_t>(m_length) * dtrace::dtrace_ctrl::word_byte_size;
    return m_read_buffer_initialized ? m_mem_host_addr : (m_mem_host_addr + mem_length);
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
    control_buffer.push_back(std::stoul(m_arguments[0], nullptr, dtrace::dtrace_ctrl::hexadecimal_base));
    // length
    control_buffer.push_back(m_length);
    // mem_host_addr high
    control_buffer.push_back(m_read_buffer_addr[0]);
    // mem_host_addr low
    control_buffer.push_back(m_read_buffer_addr[1]);

    // mem buffer
    set_location(mem_buffer, true);
    // values
    if (!m_read_buffer_initialized)
        mem_buffer.insert(mem_buffer.end(), m_length, dtrace::dtrace_ctrl::result_value_init);
}

//-------------------------read_mem_action::serialize_helper-------------------------//
/**
 * serialize_helper() - Helper function to serialize action.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 *
 * @return 
 *  The value from the result buffer based on the location mapping and
 *  resets the value in the result buffer after serialization.
 */
std::vector<uint32_t>
read_mem_action::
serialize_helper(std::vector<uint32_t>& result_buffer, std::vector<uint32_t>& mem_buffer, 
    const std::unordered_map<uint32_t, uint32_t>& mapping) const
{
    std::vector<uint32_t> result;
    uint32_t index = get_location(true);
    uint32_t length = result_buffer[mapping.at(get_location(false) + 1)];
    for (uint32_t i = index; i < index+length; ++i)
    {        
        result.push_back(mem_buffer[i]);
        // reset value after serialization
        mem_buffer[i] = dtrace::dtrace_ctrl::result_value_init;
    }
    return result;
}

//-------------------------read_mem_action::serialize-------------------------//
/**
 * serialize() - Serializes the read memory register action into a string format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 * @param script_output
 */
void
read_mem_action::
serialize(std::vector<uint32_t>& result_buffer, std::vector<uint32_t>& mem_buffer, 
    const std::unordered_map<uint32_t, uint32_t>& mapping, std::ostream& script_output) const
{
    std::vector<uint32_t> result = read_mem_action::serialize_helper(result_buffer, mem_buffer, mapping);
    // serialize string format
    script_output << "  " << m_result << " = \"[";
    for (uint32_t i = 0; i < result.size(); ++i)
    {
        script_output << "0x" << std::hex << std::setfill('0') << std::setw(8) << result[i] << std::dec;
        if (i != result.size() - 1)
            script_output << ", ";
    }
    script_output << "]\"\n";
}

//-------------------------read_mem_action::serialize-------------------------//
/**
 * serialize() - Serializes the read memory register action into json format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 * @param json_output
 */
void
read_mem_action::
serialize(std::vector<uint32_t>& result_buffer, std::vector<uint32_t>& mem_buffer, 
    const std::unordered_map<uint32_t, uint32_t>& mapping, json& json_output) const
{
    std::vector<uint32_t> result = read_mem_action::serialize_helper(result_buffer, mem_buffer, mapping);
    // serialize json format
    json json_result = json::array();
    for (uint32_t i = 0; i < result.size(); ++i)
        json_result.push_back(result[i]);

    json_output[m_probe_name][m_result] = json_result;
}

} // namespace dtrace::action
