// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2026 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include <sstream>
#include <stdexcept>

namespace dtrace::action
{

//-------------------------host_timestamps_action::host_timestamps_action-------------------------//
/**
 * host_timestamps_action() - Constructor with action token, probe type and probe name.
 * It parses the token and extracts the result, action name and arguments.
 *
 * @param token
 *  Multiple Host Timestamp action token: name[length] = host_timestamps()
 * @param probe_type
 * @param probe_name
 * @param mem_host_addr
 *  Memory buffer host address for host timestamps action in control buffer.
 */
host_timestamps_action::
host_timestamps_action(std::string token, uint32_t probe_type, const std::string& probe_name,
    uint64_t mem_host_addr)
    : action(probe_type, probe_name)
    , m_mem_host_addr(mem_host_addr)
{
    std::vector<std::string> fields;
    std::stringstream token_stream(token);
    std::string item;
    while (std::getline(token_stream, item, '='))
        fields.push_back(action::strip(item));

    if (fields.size() != 2)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'name[length] = host_timestamps()'");

    // Regex pattern to match '<name>[<length>]' format
    aiebu::regex buffer_regex(R"(^(.+)\[(.+)\]$)");
    aiebu::smatch buffer;
    if (!aiebu::regex_match(fields[0], buffer, buffer_regex))
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'name[length]'");

    m_result = buffer[1];
    std::string length = buffer[2];

    aiebu::smatch action;
    if (!aiebu::regex_match(fields[1], action, action_name::action_regex))
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'host_timestamps()'");

    m_action_name = action[1];
    m_length = static_cast<uint32_t>(std::stoull(length, nullptr, 0));

    // Store the memory host address in the memory buffer address vector
    m_mem_buffer_addr.push_back(
        (m_mem_host_addr >> dtrace::dtrace_ctrl::forth_byte_shift) & dtrace::dtrace_ctrl::mask_32
    );
    m_mem_buffer_addr.push_back(
        m_mem_host_addr & dtrace::dtrace_ctrl::mask_32
    );
}

//-------------------------host_timestamps_action::get_mem_host_addr-------------------------//
/**
 * get_mem_host_addr() - Retrieves the memory host address after adjusting for the buffer length.
 *
 * @return
 *  Updated memory host address.
 *
 * This function calculates the memory length by converting the second argument 
 * (action length) in `m_arguments` into word bytes. It then adds this memory length to the 
 * base memory host address (`m_mem_host_addr`) and returns the result. 
 */
uint64_t
host_timestamps_action::
get_mem_host_addr() const
{
    return m_mem_host_addr +
        static_cast<uint64_t>(m_length) * dtrace::action::action_ctrl::timestamps_value_size *
        dtrace::dtrace_ctrl::word_byte_size;
}

//-------------------------host_timestamps_action::actionize-------------------------//
/**
 * actionize() - Adds host timestamp action values to the control and memory buffers.
 *
 * @param last 
 *  Last action for the current probe.
 * @param control_buffer 
 * @param mem_buffer 
 */
void
host_timestamps_action::
actionize(uint32_t last, std::vector<uint32_t>& control_buffer, std::vector<uint32_t>& mem_buffer)
{
    set_location(control_buffer, false);
    // control buffer
    control_buffer.push_back(
        (last << dtrace::dtrace_ctrl::second_byte_shift) | action_type::host_timestamps
    );
    // timestamp length
    control_buffer.push_back(m_length);
    // mem_host_addr high
    control_buffer.push_back(m_mem_buffer_addr[0]);
    // mem_host_addr low
    control_buffer.push_back(m_mem_buffer_addr[1]);

    // mem buffer
    set_location(mem_buffer, true);
    for (size_t i = 0; i < m_length; ++i)
    {
        // timestamp value high
        mem_buffer.push_back(dtrace::dtrace_ctrl::result_value_init);
        // timestamp value low
        mem_buffer.push_back(dtrace::dtrace_ctrl::result_value_init);
    }
}

//-------------------------host_timestamps_action::serialize_helper-------------------------//
/**
 * serialize_helper() - Helper function to serialize action.
 *
 * @param mem_buffer
 * @param mapping
 *
 * @return 
 *  The value from the memory buffer and resets the value after serialization.
 */
std::vector<uint64_t>
host_timestamps_action::
serialize_helper(uint32_t* mem_buffer,
    const std::unordered_map<uint32_t, uint32_t>&) const
{
    std::vector<uint64_t> result;
    for (uint32_t i = 0; i < m_length; ++i)
    {
        uint32_t location = get_location(true) + i * dtrace::action::action_ctrl::timestamps_value_size;
        uint64_t high = static_cast<uint64_t>(mem_buffer[location]) << dtrace::dtrace_ctrl::forth_byte_shift;
        uint64_t low = mem_buffer[location + 1];
        result.push_back(high + low);
        // reset value after serialization
        mem_buffer[location] = dtrace::dtrace_ctrl::result_value_init;
        mem_buffer[location + 1] = dtrace::dtrace_ctrl::result_value_init;
    }
    return result;
}

//-------------------------host_timestamps_action::serialize-------------------------//
/**
 * serialize() - Serializes the timestamp action into a string format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 * @param script_output
 *
 * @return
 *  true if action fired, false otherwise
 */
void
host_timestamps_action::
serialize(uint32_t*, uint32_t* mem_buffer,
    const std::unordered_map<uint32_t, uint32_t>& mapping, std::ostream& script_output) const
{
    std::vector<uint64_t> result = host_timestamps_action::serialize_helper(mem_buffer, mapping);
    uint64_t result_init = (static_cast<uint64_t>(dtrace::dtrace_ctrl::result_value_init) << dtrace::dtrace_ctrl::forth_byte_shift) 
                         + dtrace::dtrace_ctrl::result_value_init;
    // Check if probe fired
    bool has_data = false;
    for (const auto& timestamp : result) {
        if (timestamp != result_init) {
            has_data = true;
            break;
        }
    }
    if (!has_data) {
        m_result_type = action_result_type::read_action_not_fired;
        return;
    }

    // serialize string format
    script_output << "  " << m_result << " = [";
    for (size_t i = 0; i < result.size(); ++i)
    {
        script_output << result[i];
        if (i != result.size() - 1)
            script_output << ", ";
    }
    script_output << "]\n";
    m_result_type = action_result_type::read_action_fired;
}

//-------------------------host_timestamps_action::serialize-------------------------//
/**
 * serialize() - Serializes the timestamp action into json format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 * @param json_output
 */
void
host_timestamps_action::
serialize(uint32_t*, uint32_t* mem_buffer,
    const std::unordered_map<uint32_t, uint32_t>& mapping, json& json_output) const
{
    std::vector<uint64_t> result = host_timestamps_action::serialize_helper(mem_buffer, mapping);
    uint64_t result_init = (static_cast<uint64_t>(dtrace::dtrace_ctrl::result_value_init) << dtrace::dtrace_ctrl::forth_byte_shift) 
                         + dtrace::dtrace_ctrl::result_value_init;
    // Check if probe fired
    bool has_data = false;
    for (const auto& timestamp : result) {
        if (timestamp != result_init) {
            has_data = true;
            break;
        }
    }
    if (!has_data) {
        m_result_type = action_result_type::read_action_not_fired;
        return;
    }

    // serialize json format
    json json_result = json::array();
    for (uint32_t i = 0; i < result.size(); ++i)
        json_result.push_back(result[i]);

    json_output[m_probe_name][m_result] = json_result;
    m_result_type = action_result_type::read_action_fired;
}

} // namespace dtrace::action
