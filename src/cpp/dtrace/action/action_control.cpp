// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include <sstream>

namespace dtrace::action
{

//-------------------------action::action-------------------------//
/**
 * action() - Constructor with probe type and probe name.
 *
 * @param probe_type
 * @param probe_name
 */
action::
action(uint32_t probe_type, std::string probe_name)
    : m_probe_type(probe_type)
    , m_probe_name(std::move(probe_name))
    , m_control_location(0)
    , m_mem_location(0)
{
}

//-------------------------action::set_location-------------------------//
/**
 * set_location() - Sets the location based on the size of the control buffer.
 * 
 * @param buffer
 *  Control block buffer or memory block buffer.
 * @param is_mem_buffer
 *  Boolean indicating if the action is a memory buffer.
 */
void
action::
set_location(const std::vector<uint32_t>& buffer, bool is_mem_buffer)
{
    // If the action is a memory buffer, set location for memory block.
    // Otherwise, set location in control block.
    if (is_mem_buffer)
    {
        m_mem_location = static_cast<uint32_t>(buffer.size());
        return;
    }
    m_control_location = static_cast<uint32_t>(buffer.size());
}

//-------------------------action::get_location-------------------------//
/**
 * get_location() - Retrieves the location associated with the action.
 * 
 * @param is_mem_buffer
 *  Boolean indicating if the action is a memory buffer.
 * @return 
 *  Location as a 32-bit unsigned integer.
 */
uint32_t
action::
get_location(bool is_mem_buffer) const
{
    // If the action is a memory buffer, return the memory block location.
    // Otherwise, return the control block location.
    if (is_mem_buffer)
        return m_mem_location;
    
    return m_control_location;
}

//-------------------------action::create_string-------------------------//
/**
 * create_string() Creates a string representation of the action.
 *
 * @return 
 *  String representing the action.
 *
 * This function constructs a string that includes the probe type, probe name,
 * action name, arguments, and result of the action. The format of the string is as follows:
 * "action_name: <action_name>, arguments: [<arg1>, <arg2>, ...]"
 */
std::string
action::
create_string() const
{
    std::ostringstream output_action;
    output_action << "action_name: " << m_action_name << "; arguments: [";

    for (const auto& argument : m_arguments)
        output_action << argument << ", ";
    
    output_action << "]";
    if (!m_result.empty())
         output_action << "; result: " << m_result;

    return output_action.str();
}

//-------------------------action::strip-------------------------//
/**
 * strip() - Removes leading and trailing whitespace characters, including spaces, 
 *  newlines, carriage returns, and tabs. from a given string.
 *
 * @param token 
 *  Input string.
 * @return  
 *  Updated string with leading and trailing whitespace removed. If the input string
 *  contains only whitespace characters, an empty string is returned.
 */
std::string
action::
strip(const std::string& token)
{
    size_t first = token.find_first_not_of(" \n\r\t");
    if (first == std::string::npos)
        return "";

    size_t last = token.find_last_not_of(" \n\r\t");
    return token.substr(first, last - first + 1);
}

} // namespace dtrace::action
