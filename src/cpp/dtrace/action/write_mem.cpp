// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026 Advanced Micro Devices, Inc. All rights reserved.

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
    buffer_map& buffer_map)
    : action(probe_type, probe_name)
    , m_write_buffer_addr(nullptr)
    , m_append_write_buffer(false)
{
    std::vector<std::string> fields;
    dtrace::getline(token, '=', fields);

    // Validate and parse the action name
    std::string argument_string;
    if (!dtrace::match(fields[0], m_action_name, argument_string))
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'write_mem(addr, length, buffer)'");

    dtrace::getline(argument_string, ',', m_arguments);

    // Validate and parse the arguments
    if (m_arguments.size() < 3)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN_ARGUMENTS", 
            "Invalid arguments: '" << token << "' write_mem requires 3 arguments (addr, length, buffer)");

    // Check to parse a length string as hexadecimal or decimal
    m_length = static_cast<uint32_t>(std::stoull(m_arguments[1], nullptr, 0));
    std::string write_buffer_name = m_arguments[2];

    // check if write buffer name exists in the map and get the values
    if (buffer_map.find(write_buffer_name) != buffer_map.end())
    {
        m_write_buffer_addr = &buffer_map.at(write_buffer_name).first;
        m_write_buffer_values = buffer_map.at(write_buffer_name).second;
        // Check if the write buffer is the first write_mem of this buffer on this uC.
        // Every write_mem of this buffer shares m_write_buffer_addr.
        m_append_write_buffer =
            ((*m_write_buffer_addr)[2] == dtrace::dtrace_ctrl::write_mem_buffer_not_appended);
        // Set the write buffer append flag to indicate that the write buffer
        // has been appended to the mem buffer
        (*m_write_buffer_addr)[2] = dtrace::dtrace_ctrl::write_mem_buffer_appended;
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
    // The appending write records payload address
    // Later writes of this buffer push same shared address.
    if (m_append_write_buffer)
    {
        uint64_t buffer_addr = static_cast<uint64_t>(mem_buffer.size()) *
            dtrace::dtrace_ctrl::word_byte_size;
        (*m_write_buffer_addr)[0] =
            (buffer_addr >> dtrace::dtrace_ctrl::forth_byte_shift) & dtrace::dtrace_ctrl::mask_32;
        (*m_write_buffer_addr)[1] =
            buffer_addr & dtrace::dtrace_ctrl::mask_32;
    }
    // mem_host_addr high
    control_buffer.push_back((*m_write_buffer_addr)[0]);
    // mem_host_addr low
    control_buffer.push_back((*m_write_buffer_addr)[1]);

    // mem buffer
    // values
    if (m_append_write_buffer)
        mem_buffer.insert(
            mem_buffer.end(), m_write_buffer_values.begin(), m_write_buffer_values.end()
        );
}

//-------------------------write_mem_action::serialize-------------------------//
/**
 * serialize() - Serializes the write register memory action into a string format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 * @param script_output
 */
void
write_mem_action::
serialize(uint32_t*, uint32_t*,
    const std::unordered_map<uint32_t, uint32_t>&, std::ostream&) const
{
}

//-------------------------write_mem_action::serialize-------------------------//
/**
 * serialize() - Serializes the write register memory action into json format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 * @param json_output
 */
void
write_mem_action::
serialize(uint32_t*, uint32_t*,
    const std::unordered_map<uint32_t, uint32_t>&, json&) const
{
}

} // namespace dtrace::action
