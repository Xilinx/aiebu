// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#include "dtrace/action/action_control.h"
#include <filesystem>
#include <sstream>
#include <stdexcept>  

namespace dtrace::action
{

//-------------------------operation_action::operation_action-------------------------//
/**
 * operation_action() - Constructor with action token, probe type and probe name.
 *
 * @param token
 *  Print action token: print(fmt)
 * @param probe_type
 * @param probe_name
 * 
 */
operation_action::
operation_action(std::string token, uint32_t probe_type, const std::string& probe_name)
    : action(probe_type, probe_name)
    , m_token(std::move(token))
{
}

//-------------------------operation_action::actionize-------------------------//
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
operation_action::
actionize(uint32_t, std::vector<uint32_t>&, std::vector<uint32_t>&)
{
}

//-------------------------operation_action::serialize-------------------------//
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
operation_action::
serialize(const std::vector<uint32_t>&, const std::vector<uint32_t>&, 
    const std::unordered_map<uint32_t, uint32_t>&) const
{
    std::ostringstream output_action;
    output_action << "  " << m_token << "\n";
    return output_action.str();
}

} // namespace dtrace::action
