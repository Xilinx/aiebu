// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include <sstream>
#include <stdexcept>

namespace dtrace::action
{

//-------------------------mask_write_reg_action::mask_write_reg_action-------------------------//
/**
 * mask_write_reg_action() - Constructor with action token, probe type and probe name.
 * It parses the token and extracts the result, action name and arguments.
 *
 * @param token
 *  Mask write action token: mask_write_reg(addr, mask, val)
 * @param probe_type
 * @param probe_name
 */
mask_write_reg_action::
mask_write_reg_action(std::string token, uint32_t probe_type, const std::string& probe_name, 
    const std::unordered_map<std::string, std::pair<std::vector<uint32_t>, std::vector<uint32_t>>>& buffer_map)
    : action(probe_type, probe_name)
    , m_mode(0)
{
    std::vector<std::string> fields;
    std::stringstream token_stream(token);
    std::string item;
    while (std::getline(token_stream, item, '='))
        fields.push_back(strip(item));

    aiebu::smatch action;
    if (!aiebu::regex_match(fields[0], action, action_name::action_regex))
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN_FORMAT", 
            "Invalid token: '" << token << "' Expected 'mask_write_reg(addr, mask, val)'");

    m_action_name = action[1];
    std::string argument_string = action[2]; 

    // Validate and parse the length argument
    std::stringstream argument_stream(argument_string);
    while (std::getline(argument_stream, item, ','))
        m_arguments.push_back(strip(item));

    if (m_arguments.size() < 3)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN_ARGUMENTS", 
            "Invalid arguments: '" << token << "' mask_write_reg requires 3 arguments (addr, mask, val)");

    // Check if the value argument is a buffer address reference with HIGH() or LOW()
    aiebu::smatch value;
    aiebu::regex high_low_regex(R"(^(HIGH|LOW)\(&(\w+)\)$)");
    if (aiebu::regex_match(m_arguments[2], value, high_low_regex)) 
    {
        std::string write_buffer_name = value[2];
        // check if write buffer name exists in the map and get the values
        if (buffer_map.find(write_buffer_name) != buffer_map.end())
        {
            m_result = write_buffer_name;
            m_write_buffer_values = buffer_map.at(write_buffer_name).second;

            // set mode and value argument based on HIGH or LOW
            std::vector<uint32_t> write_buffer_addr = buffer_map.at(write_buffer_name).first;
            std::stringstream argument;
            if (value[1] == "HIGH")
            {// HIGH mode
                m_mode = 1;
                argument << "0x" << std::hex << write_buffer_addr[0];
                m_arguments[2] = argument.str();
            }
            else
            {// LOW mode
                m_mode = 2;
                argument << "0x" << std::hex << write_buffer_addr[1];
                m_arguments[2] = argument.str();
            }
        }
        else
            DTRACE_ERROR("DTRACE_ACTION_WRITE_BUFFER_NOT_FOUND", 
                "Write buffer name not found: " << write_buffer_name);
    }
}

//-------------------------mask_write_reg_action::actionize-------------------------//
/**
 * actionize() - Adds mask write register action values to the control buffer.
 *
 * @param last 
 *  Last action for the current probe.
 * @param control_buffer 
 * @param mem_buffer 
 */
void
mask_write_reg_action::
actionize(uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer)
{
    // control buffer
    // mask write register action header
    if (m_mode == 0)
    {// value argument normal mode
        control_buffer.push_back(
            (last << dtrace::dtrace_ctrl::second_byte_shift) | action_type::reg_mask_write
        );
    }
    else
    {//  value argument HIGH or LOW mode
        control_buffer.push_back(
            (last << dtrace::dtrace_ctrl::second_byte_shift) | action_type::reg_mask_write | 
            (m_mode << dtrace::dtrace_ctrl::first_byte_shift)
        );
        set_location(control_buffer, false);
    }

    // write address
    control_buffer.push_back(std::stoul(m_arguments[0], nullptr, dtrace::dtrace_ctrl::hexadecimal_base));
    // mask value
    control_buffer.push_back(std::stoul(m_arguments[1], nullptr, dtrace::dtrace_ctrl::hexadecimal_base));
    // write value
    control_buffer.push_back(std::stoul(m_arguments[2], nullptr, dtrace::dtrace_ctrl::hexadecimal_base));

    if (m_mode == 2)
    {   // mem buffer 
        set_location(mem_buffer, true);
        // append values to mem buffer after LOW mode
        mem_buffer.insert(
            mem_buffer.end(), m_write_buffer_values.begin(), m_write_buffer_values.end()
        );
    }
}

//-------------------------mask_write_reg_action::serialize-------------------------//
/**
 * serialize() - Serializes the register mask write action into a string format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 *
 * @return 
 *  String representing the serialized register mask write action.
 */
std::string
mask_write_reg_action::
serialize(std::vector<uint32_t>&, std::vector<uint32_t>& mem_buffer, 
    const std::unordered_map<uint32_t, uint32_t>&) const
{
    std::ostringstream output_action;
    output_action << "  " << "#" << " " << m_action_name << "\n";
    if (m_mode == 2)
    {
        std::ostringstream readable_result;
        uint32_t index = get_location(true);
        uint32_t length = m_write_buffer_values.size();
        for (uint32_t i = index; i < index+length; ++i)
        {
            readable_result << "  0x" << std::hex << mem_buffer[i];
            if (i < index+length - 1)
                readable_result << "\n";

            // reset value after serialization
            mem_buffer[i] = 0;
        }
        output_action << "  " << m_result << " = \"\"\"\n" << readable_result.str() << "\"\"\"\n";
    }
    return output_action.str();
}

} // namespace dtrace::action
