// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>  

namespace dtrace::action
{

//-------------------------printa_action::printa_action-------------------------//
/**
 * printa_action() - Constructor with action token, probe type and probe name.
 *
 * @param token
 *  Profile print action token: printa(fmt)
 * @param probe_type
 * @param probe_name
 * @param maps
 * 
 * Constructor processes the token to result, action name and arguments and 
 * initializes built-in variables based on the probe type.
 */
printa_action::
printa_action(std::string token, uint32_t probe_type, const std::string& probe_name, 
    std::unordered_map<std::string, boost::property_tree::ptree> maps)
    : action(probe_type, probe_name)
    , m_maps(std::move(maps)) 
{  
    std::vector<std::string> fields;
    std::stringstream token_stream(token);
    std::string item;

    while (std::getline(token_stream, item, '='))
        fields.push_back(action::strip(item));

    aiebu::smatch action;
    if (!aiebu::regex_match(fields[0], action, action_name::action_regex))
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN", 
            "Invalid token: '" << token << "' Expected 'printa(fmt)'");

    m_action_name = action[1];
    std::string argument_string = action[2];

    // Validate and parse the length argument
    std::stringstream argument_stream(argument_string);
    while (std::getline(argument_stream, item, ','))
        m_arguments.push_back(action::strip(item));

    if (m_arguments.size() < 1)
        DTRACE_ERROR("DTRACE_ACTION_INVALID_TOKEN_ARGUMENTS", 
            "Invalid arguments: '" << token << "' printa requires 1 arguments (fmt)");
}

//-------------------------printa_action::actionize-------------------------//
/**
 * actionize() - Profile print action does not require any action to be performed.
 *  serialize() is called to print the action.
 *
 * @param last 
 *  Last action for the current probe.
 * @param control_buffer 
 * @param mem_buffer 
 */
void
printa_action::
actionize(uint32_t, std::vector<uint32_t>&, std::vector<uint32_t>&)
{
}

//-------------------------printa_action::get_opcode-------------------------//
/**
 * get_opcode() - 
 *  Retrieves the opcode hits for a given page and offset from the result buffer.
 *
 * @param value 
 * @return 
 *  Pair of uint32_t values representing the hit counts for the opcodes.
 *
 * This function calculates the number of hits for two specific opcodes based 
 * on the provided page and offset values. It iterates through the result buffer 
 * and checks for matches with the constructed opcodes, accumulating the hit counts.
 */
std::pair<std::string, uint32_t>
printa_action::
get_opcode(const uint32_t& value) const
{
    for (const auto& line : m_maps)
    {
        uint32_t page = std::stoi(line.second.get<std::string>("page_index"));
        uint32_t offset = std::stoi(line.second.get<std::string>("page_offset")) - 
            static_cast<int>(page * dtrace::dtrace_ctrl::page_length_check);
        if ((value & dtrace::dtrace_ctrl::mask_16) == offset && 
            ((value >> dtrace::dtrace_ctrl::second_byte_shift) & dtrace::dtrace_ctrl::mask_8) == page)
            return {line.second.get<std::string>("operation"), page};
    }
    return {"", dtrace::dtrace_ctrl::mask_8};
}

//-------------------------printa_action::serialize_helper-------------------------//
/**
 * serialize_helper() - Formats and prints the results from the result buffer.
 *
 * @param result_buffer 
 *  A vector of uint32_t values containing the result data.
 *
 * This function takes a vector of uint32_t values representing the result buffer,
 * calculates the total number of samples, and prints the formatted results to the
 * standard output. The output includes the name, samples, and percentage of each operation.
 */
std::string
printa_action::
serialize_helper(const std::vector<uint32_t>& result_buffer) const
{
    constexpr int name_width = 50;
    constexpr int samples_width = 16;
    constexpr int percentage_width = 16;
    constexpr int percentage_multiplier = 100;
    
    uint32_t total = 0;
    uint32_t start = 0;

    for (size_t i = 0; i < result_buffer[0]; i += 2)
        total += result_buffer[i + 2];

    std::ostringstream output_format; 
    output_format << std::left << std::setw(name_width) << "name"
        << std::right << std::setw(samples_width) << "samples"
        << std::right << std::setw(percentage_width) << "percentage"
        << "\n";

    if (result_buffer[1] == dtrace::dtrace_ctrl::empty_buffer_check)
    {
        output_format << std::left << std::setw(name_width) << "pre-run"
            << std::right << std::setw(samples_width) 
            << (std::to_string(result_buffer[2]) + "/" + std::to_string(total))
            << std::right << std::setw(percentage_width) << std::fixed << std::setprecision(3) 
            << (static_cast<double>(result_buffer[2]) / total) * percentage_multiplier << "%"
            << "\n";
        start += 2;
    }

    for (size_t i = start; i < result_buffer[0]; i += 2)
    {
        std::string operation;
        if ((result_buffer[i+2] & dtrace::dtrace_ctrl::mask_16) == 0)
            continue;

        auto [opcode, page_index] = get_opcode(result_buffer[i+1]);
        if ((result_buffer[i+1] & dtrace::dtrace_ctrl::mask_high_bit) == 0)
            operation = opcode;
        else
            operation = " !!(block/preemption/end)";

        output_format << std::left << std::setw(name_width) << operation
            << std::right << std::setw(samples_width) 
            << (std::to_string(result_buffer[i + 2]) + "/" + std::to_string(total))
            << std::right << std::setw(percentage_width) << std::fixed << std::setprecision(3) 
            << (static_cast<double>(result_buffer[i+2]) / total) * percentage_multiplier << "%"
            << "\n";
    }

    if ((result_buffer[result_buffer[0]-1] & dtrace::dtrace_ctrl::mask_16) == 0) 
    {
        output_format << std::left << std::setw(name_width) << "post-run"
            << std::right << std::setw(samples_width) 
            << (std::to_string(result_buffer[result_buffer[0]]) + "/" + std::to_string(total))
            << std::right << std::setw(percentage_width) << std::fixed << std::setprecision(3) 
            << (static_cast<double>(result_buffer[result_buffer[0]]) / total) * percentage_multiplier << "%"
            << "\n";
    }
    return output_format.str();
}

//-------------------------printa_action::serialize-------------------------//
/**
 * serialize() - Serializes the profile print action into a string format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 * @param script_output
 */
void
printa_action::
serialize(std::vector<uint32_t>& result_buffer, std::vector<uint32_t>&, 
    const std::unordered_map<uint32_t, uint32_t>&, std::ostream& script_output) const
{
    // serialize string format
    script_output << "  " << "print(\"\"\"\n" << printa_action::serialize_helper(result_buffer) << "  " << "\"\"\")\n";
}

//-------------------------printa_action::serialize-------------------------//
/**
 * serialize() - Serializes the profile print action into json format.
 *
 * @param result_buffer
 * @param mem_buffer
 * @param mapping
 * @param json_output
 */
void
printa_action::
serialize(std::vector<uint32_t>&, std::vector<uint32_t>&, 
    const std::unordered_map<uint32_t, uint32_t>&, json&) const
{
}

} // namespace dtrace::action
