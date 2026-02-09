// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include "dtrace/probe/probe_control.h"
#include <filesystem>
#include <sstream>
#include <stdexcept>  

namespace dtrace::action
{

//-------------------------print_action::print_action-------------------------//
/**
 * print_action() - Constructor with action token, probe type and probe name.
 *
 * @param token
 *  Print action token: print(fmt)
 * @param probe_type
 * @param probe_name
 * @param maps
 * 
 * Constructor processes the token to result, action name and arguments and
 * initializes built-in variables based on the probe type.
 */
print_action::
print_action(std::string token, uint32_t probe_type, const std::string& probe_name, 
    const std::unordered_map<std::string, boost::property_tree::ptree>& maps)
    : action(probe_type, probe_name)
    , m_token(std::move(token))
{
    std::vector<std::string> fields;
    std::stringstream token_stream(m_token);
    std::string item;
    while (std::getline(token_stream, item, '='))
        fields.push_back(strip(item));

    std::string temp = fields[0];
    size_t position = temp.find('(');
    if (position == std::string::npos)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN_FORMAT", 
            "Invalid token: '" << m_token << "' Expected 'print(fmt)'");

    m_action_name = temp.substr(0, position);
    // Validate and parse the length argument
    std::string argument_string = temp.substr(position + 1, temp.length() - position - 2);
    std::stringstream argument_stream(argument_string);
    while (std::getline(argument_stream, item, ','))
        m_arguments.push_back(strip(item));

    if (m_arguments.size() < 1)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN_ARGUMENTS", 
            "Invalid arguments: '" << m_token << "' print requires 1 arguments (fmt)");

    m_built_ins["probename"] = m_probe_name;

    // operation built-in variable for jprobe and check if operation is requested.
    if (m_token.find("operation") != std::string::npos) 
    {
        if (m_probe_type == dtrace::probe::probe_type::jprobe)
        {
            if (maps.find(m_probe_name) != maps.end()) 
            {
                const auto& value = maps.at(m_probe_name);
                m_built_ins["operation"] = value.get<std::string>("operation");
            }
            else 
                DTRACE_ERROR("DTRACE_ACTION_TOKEN_INVALID", 
                    "'operation' built-in variable is valid only in jprobe");
        }
    }
}

//-------------------------print_action::actionize-------------------------//
/**
 * actionize() - Print action does not require any action to be performed. 
 *  serialize() is called to print the action.
 *
 * @param last 
 *  Last action for the current probe.
 * @param control_buffer 
 * @param mem_buffer 
 */
void
print_action::
actionize(uint32_t, std::vector<uint32_t>&, std::vector<uint32_t>&)
{
}

//-------------------------print_action::serialize-------------------------//
/**
 * serialize() - Serializes the print action into a string format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 *
 * @return 
 *  String representing the serialized print action.
 */
std::string
print_action::
serialize(std::vector<uint32_t>&, std::vector<uint32_t>&, 
    const std::unordered_map<uint32_t, uint32_t>&) const
{
    std::ostringstream output_action;
    for (const auto& [key, value] : m_built_ins)
    {
        if (m_token.find(key) != std::string::npos)
            output_action << "  " << key << " = " << '"' << value << '"' << "\n";
    }
    output_action << "  " << m_token << "\n";
    return output_action.str();
}

} // namespace dtrace::action
