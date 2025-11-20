// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include <sstream>
#include <stdexcept>

namespace dtrace::action
{

//-------------------------read_handshake_action::read_handshake_action-------------------------//
/**
 * read_handshake_action() - Constructor with action token, probe type and probe name.
 * It parses the token and extracts the result, action name and arguments.
 *
 * @param token
 *  Read handshake action token: val = read_handshake(offset)
 * @param probe_type
 * @param probe_name
 */
read_handshake_action::
read_handshake_action(std::string token, uint32_t probe_type, const std::string& probe_name)
    : action(probe_type, probe_name)
{
    std::vector<std::string> fields;
    std::stringstream token_stream(token);
    std::string item;
    while (std::getline(token_stream, item, '='))
        fields.push_back(strip(item));

    if (fields.size() != 2)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'val = read_handshake(offset)'");

    m_result = fields[0];

    boost::smatch action;
    if (!boost::regex_match(fields[1], action, action_name::action_regex))
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'read_handshake(offset)'");

    m_action_name = action[1];
    std::string argument_string = action[2]; 

    // Validate and parse the length argument
    std::stringstream argument_stream(argument_string);
    while (std::getline(argument_stream, item, ','))
        m_arguments.push_back(strip(item));

    if (m_arguments.size() < 1)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN_ARGUMENTS", 
            "Invalid arguments: '" << token << "' read_handshake requires 1 argument (offset)");

    // Validate the handshake offset
    if (std::stoul(m_arguments[0], nullptr, 0) % 4 != 0)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN_ARGUMENTS", 
            "Invalid arguments: '" << token << "' read_handshake offset must align 4-byte boundary");

    // write handshake word offset
    m_arguments[0] = std::to_string(std::stoul(m_arguments[0], nullptr, 0) >> 2);
}

//-------------------------read_handshake_action::actionize-------------------------//
/**
 * actionize() - Adds read handshake action values to the control buffer.
 *
 * @param last 
 *  Last action for the current probe.
 * @param control_buffer 
 * @param mem_buffer 
 */
void
read_handshake_action::
actionize(uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>&)
{
    // control buffer
    // read handshake action header
    control_buffer.push_back(
        (last << dtrace::dtrace_ctrl::second_byte_shift) | action_type::handshake_read
    );
    // read offset
    control_buffer.push_back(std::stoul(m_arguments[0], nullptr, 16));
    set_location(control_buffer, false);
    // return value
    control_buffer.push_back(0);
}

//-------------------------read_handshake_action::serialize-------------------------//
/**
 * serialize() - Serializes the handshake read action into a string format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 *
 * @return 
 *  String representing the serialized handshake read action.
 */
std::string
read_handshake_action::
serialize(const std::vector<uint32_t>& result_buffer, const std::vector<uint32_t>&, 
    const std::unordered_map<uint32_t, uint32_t>& mapping) const
{
    std::ostringstream output_action;
    uint32_t handshake_value = result_buffer[mapping.at(get_location(false))];
    if (handshake_value == dtrace::dtrace_ctrl::handshake_overflow)
    {
        std::stringstream handshake_offset;
        handshake_offset << "0x" << std::hex << (std::stoul(m_arguments[0], nullptr, 0) * sizeof(uint32_t));
        output_action << "  " << "print(\"[WARNING] HANDSHAKE OVERFLOW (" << handshake_offset.str() << ")\")\n";
    }
    output_action << "  " << m_result << " = " << handshake_value << "\n";
    return output_action.str();
}

} // namespace dtrace::action
