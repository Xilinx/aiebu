// SPDX-License-Identifier: MIT
// Copyright (C) 2024-2025 Advanced Micro Devices, Inc. All rights reserved.

#ifndef CONTROL_H
#define CONTROL_H

// This header file contains the control class which is responsible for 
// controlling the entire flow of the tool.
#include "dtrace/parser/parser.h"
#include "dtrace/action/action_control.h"
#include "dtrace/probe/probe_control.h"
#include "dtrace/pager/pager.h"
#include "dtrace/utils.h"
#include "json/nlohmann/json.hpp"

#include <cstdint> 
#include <string>
#include <unordered_map>
#include <vector>

namespace dtrace
{
//-------------------------Control-------------------------//
/**
 * @class control
 *
 * @brief
 * dtrace::control represents the control object for the dtrace compiler.
 *
 * @details
 * This class is responsible for controlling the entire flow of the dtrace compiler.
 * It reads the script file, parses it, and then creates the control buffers and
 *  memory buffers. It also creates the result file.
 */
class control
{
private:
    dtrace::pager::pager m_pager;
    dtrace::parser::parser m_parser;
    std::unordered_map<uint32_t, std::vector<uint32_t>> m_control_buffers;
    std::unordered_map<uint32_t, std::vector<uint32_t>> m_mem_buffers;
    std::unordered_map<uint32_t, std::vector<uint32_t>> m_mem_action_locations;
    std::vector<std::pair<std::shared_ptr<dtrace::action::action>, uint32_t>> m_result_actions;
    dtrace::dtrace_output_format m_output_format;
    void populate_result_actions();

public:
    control(const std::string& script_file, const std::string& map_data);
    uint32_t m_num_uCs;
    std::set<uint32_t> m_control_uC_indices;
    bool m_mem_action_present;
    std::vector<uint32_t> create_control_buffer(uint32_t uC) const;
    std::vector<uint32_t> create_mem_buffer(uint32_t uC) const;
    void patch_control_buffer(const std::unordered_map<uint32_t, uint64_t>& mem_host_addr_map);
    void create_result_file(std::unordered_map<uint32_t, std::vector<uint32_t>>& result_buffers, 
        std::unordered_map<uint32_t, std::vector<uint32_t>>& mem_buffers, 
        const std::string& output_file) const;
    void create_result_buffer(std::unordered_map<uint32_t, std::vector<uint32_t>>& result_buffers,
        std::unordered_map<uint32_t, std::vector<uint32_t>>& mem_buffers,
        nlohmann::ordered_json& json_output) const;
};

} // namespace dtrace
#endif // CONTROL_H
